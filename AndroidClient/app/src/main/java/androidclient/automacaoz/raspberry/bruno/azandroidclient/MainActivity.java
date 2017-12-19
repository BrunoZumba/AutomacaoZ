package androidclient.automacaoz.raspberry.bruno.azandroidclient;

import android.app.Activity;
import android.app.DatePickerDialog;
import android.app.Dialog;
import android.app.FragmentManager;
import android.content.ClipData;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.nfc.NfcAdapter;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.squareup.timessquare.CalendarPickerView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.w3c.dom.Text;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.Map;

import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.ActionButtonClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.SensorClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses.ActionButtonCommand;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses.Command;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses.SensorCommand;
/** Documentação técnica/teórica:
 *
 * DEVICE/DISPOSITIVO: Aparelho pré-configurado que recebe uma TASK/TAREFA e executa uma ação em si mesmo.
 *          Exemplo: Aparelho da NET, TV, Ar condicionado, Luz, etc.
 *
 * TASK/TAREFA: String JSON que realiza UMA ação no DEVICE/DISPOSITIVO. Essa string JSON é composta dos elementos JSON "deviceName", "buttonName" e "mode"
 *          Exemplo: {"deviceName":"ControleNet","buttonName":"KEY_EXIT","mode":"SEND_ONCE"}
 *
 * TASK BUTTON/BOTAO DE TAREFA: String JSON composta por uma TASK/TAREFA na json-tag "TAKS" e pelo id do botão na json-tag "ID"
 *          Exemplo: {"ID":2131558550,"Task":"{"deviceName":"ControleNet","buttonName":"KEY_EXIT","mode":"SEND_ONCE"}"}
 *
 * ACTION/AÇÃO: Array JSON contendo 2 ou mais TASK BUTTON/BOTAO DE TAREFA 
 *          Exemplo: [{"ID":2131558550,"Task":"{"deviceName":"ControleNet","buttonName":"KEY_EXIT","mode":"SEND_ONCE"}"},{"ID":2131558550,"Task":"{"deviceName":"ControleNet","buttonName":"KEY_EXIT","mode":"SEND_ONCE"}"}]
 *
 * ACTION BUTTON/BOTÃO DE AÇÃO: String JSON que composta pelo array ACTION/ACAO na json-tag "action" e pelo nome da acao na json-tag "actionName"
 *          Exemplo: {"actionName":"ChegarEmCasa","action":"[{"ID":2131558550,"Task":"{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_EXIT\",\"mode\":\"SEND_ONCE\"}"},{"ID":2131558550,"Task":"{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_EXIT\",\"mode\":\"SEND_ONCE\"}"}]
 */

/**
 * TODO: Quando eu adiciono botões no layout, o ID dos botões mudam, cagando as ações que já existem.
 * TODO: Colocar o IP do edit text do devicesDisplay no SERVER_IP da classe UTIL
 *
 * TODO: quando o usuario apaga uma ação, verificar se essa ação está relacionada em alguma Ação Recorrente
 *       R: Colocar o arquivo do RPi num BD.
 *
 * TODO: QUANDO ACTION TÁ GRANDE, VERIFICAR BUFFER NO RPI
 *
 * TODO: no GetAllActionButtons, alterar o server para só enviar o nome das ações e se executar a ação ou clicar nela para aparecer as tasks, aí sim ir no banco pegar os json.
 */
public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MAIN_ACTIVITY";

    //Layout que abriga todas as Ações gravadas em disco.
    //Feito static para que possa ser acessado/preenchido pela ActionButtonCommand
    public static LinearLayout savedActionsLayout;
    public static Activity activity;

    public TextView tvTemperatura, tvUmidade;


    static private CalendarPickerView calendar;
    static private List<Date> selectedDates;
    LinearLayout calendar_layout;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        savedActionsLayout = (LinearLayout) findViewById(R.id.savedActions);
        tvTemperatura = (TextView) findViewById(R.id.tvTemperatura);
        tvUmidade = (TextView) findViewById(R.id.tvUmidade);

        activity = this;

//        Intent intent = new Intent(this, ManageRecurringActionActivity.class);
//        startActivity(intent);

    }

    //Depois função chamada depois que o sendData do ActionButtonCommand é retornado com o GetList
    public static void inflateActionButtonLayout(String responseParm){
        JSONArray jsonArray;
        try {
            jsonArray = new JSONArray(responseParm);
            List<ActionButtonClass> actionButtons = new ArrayList<>();
            for(int i = 0; i < jsonArray.length(); i++){
                ActionButtonClass actBt = new ActionButtonClass(jsonArray.get(i).toString());
                LinearLayout line = new LinearLayout(MainActivity.activity);

                //Cria o botão da ação em si
                Button btAction = new Button(MainActivity.activity);
                btAction.setText(actBt.getActionName());
                btAction.setTag(actBt.getAction().parseToJson());
                btAction.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT,150,0.5f));
                btAction.setOnClickListener(new ExecuteActionClickListener());
                btAction.setTransformationMethod(null);

                //Cria o botão para editar a ação
                Button btEdit = new Button(MainActivity.activity);
                btEdit.setText("Edit");
                btEdit.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT,100,1f));
                JSONObject actionButton = new JSONObject();
                try {
                    actionButton.put("actionName", btAction.getText().toString());
                    actionButton.put("action",new JSONArray(actBt.getAction().parseToJson()));
                } catch (JSONException e) {
                    e.printStackTrace();
                }

                btEdit.setTag(actionButton.toString());
                btEdit.setOnClickListener(new EditActionClickListener());

                line.addView(btAction);
                line.addView(btEdit);
                MainActivity.savedActionsLayout.addView(line);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    @Override
    protected void onStart(){
        super.onStart();
        savedActionsLayout.removeAllViews();

        //Pega as ações salvas no servidor e cria a lista com botões das ações
        ActionButtonCommand actionButtonCommand = new ActionButtonCommand("getList", false, Util.SERVER_IP, Util.ACTION_BUTTON_PORT, null, null);
        actionButtonCommand.sendData(this);

        //Atualiza os sensores de temperatura e umidade
        SensorCommand sensorCommandTemperatura = new SensorCommand("sensorCommand", false, Util.SERVER_IP, Util.SENSOR_PORT, tvTemperatura, new SensorClass(tvTemperatura.getTag().toString()));
        sensorCommandTemperatura.sendData();

        SensorCommand sensorCommandUmidade = new SensorCommand("sensorCommand", false, Util.SERVER_IP, Util.SENSOR_PORT, tvUmidade, new SensorClass(tvUmidade.getTag().toString()));
        sensorCommandUmidade.sendData();
    }

    public static void processResponse(final ActionButtonCommand response){
        if (response.getRequestAction().equals("getList")){
            switch (response.getResponseStatus()) {
                case (Command.STATUS_ERROR):
                    Toast.makeText(MainActivity.activity.getApplicationContext(), response.getResponseDesc(), Toast.LENGTH_SHORT).show();
                    break;

                case (Command.STATUS_OK):
                    MainActivity.inflateActionButtonLayout(response.getResponseParm());
                    break;
            }
        } else if (response.getRequestAction().equals("executeActionButton")){
            Toast.makeText(MainActivity.activity.getApplicationContext(), response.getResponseDesc(), Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * Editar as ações de um botão de ação
     */
    private static class EditActionClickListener implements View.OnClickListener{
        @Override
        public void onClick(View v){
            Intent intent = new Intent(MainActivity.activity, ManageActionActivity.class);
            intent.putExtra("Tag", v.getTag().toString());
            MainActivity.activity.startActivity(intent);
        }
    }

    /**
     * Executa a ação que o botão possui
     */
    private static class ExecuteActionClickListener implements View.OnClickListener{
        @Override
        public void onClick(View v){
            Button bt = (Button) v;
            JSONArray action = null;
            try {
                action = new JSONArray(v.getTag().toString());
            } catch (Exception e){
                e.printStackTrace();
            }

            ActionButtonClass actBtClass = new ActionButtonClass(bt.getText().toString(), bt.getTag().toString());
            ActionButtonCommand actionButtonCommand = new ActionButtonCommand("executeActionButton", false, Util.SERVER_IP, Util.ACTION_BUTTON_PORT, null, actBtClass);
            actionButtonCommand.sendData(MainActivity.activity);
        }
    }

    public void deviceDisplayActivity(View view){
        Intent intent = new Intent(this, DeviceDisplayActivity.class);
        startActivity(intent);
    }
    public void createActionActivity(View view){
        Intent intent = new Intent(this, ManageActionActivity.class);
        startActivity(intent);
    }


    //Funções do Menu
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu_main2, menu);

        // return true so that the menu pop up is opened
        return true;
    }
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
            case R.id.miShutdown:
                String shutDownJson = "{\"actionName\":\"shutdown\"}";
                Communication comm = new Communication(shutDownJson,Util.SERVER_IP, Util.SYSTEM_ACTION_PORT,null);
                comm.sendData();
                return true;
            case R.id.miRecAction:
                Intent intent = new Intent(this, RecurringActionActivity.class);
                startActivity(intent);
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }


}
