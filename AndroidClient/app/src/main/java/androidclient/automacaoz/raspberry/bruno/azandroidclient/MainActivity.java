package androidclient.automacaoz.raspberry.bruno.azandroidclient;

import android.app.Activity;
import android.app.DatePickerDialog;
import android.app.Dialog;
import android.app.FragmentManager;
import android.content.ClipData;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
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

import com.squareup.timessquare.CalendarPickerView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.w3c.dom.Text;

import java.util.Calendar;
import java.util.Date;
import java.util.Map;
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
 * TODO: Se o usuário já gravou uma ação e altera algo no botão, não muda a string da ação. R: Gravar o ID do botão e pegar a string de comando on the fly?
 * TODO: Quando eu adiciono botões no layout, o ID dos botões mudam, cagando as ações que já existem.
 * TODO: Colocar o IP do edit text do devicesDisplay no SERVER_IP da classe UTIL
 */
public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MAIN_ACTIVITY";

    LinearLayout savedActionsLayout;
    public static Activity activity;

    public TextView tvTemperatura, tvUmidade;


    static private CalendarPickerView calendar;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        savedActionsLayout = (LinearLayout) findViewById(R.id.savedActions);
        tvTemperatura = (TextView) findViewById(R.id.tvTemperatura);
        tvUmidade = (TextView) findViewById(R.id.tvUmidade);

//        calendar = (CalendarPickerView) findViewById(R.id.cal);

        activity = this;

//        criaOsTremDoCalendar();

    }

    private void criaOsTremDoCalendar(){
        final Calendar nextYear = Calendar.getInstance();
        nextYear.add(Calendar.YEAR, 1);

        final Calendar lastYear = Calendar.getInstance();
        lastYear.add(Calendar.YEAR, -1);

        calendar.init(lastYear.getTime(), nextYear.getTime()) //
                .inMode(CalendarPickerView.SelectionMode.MULTIPLE) //
                .withSelectedDate(new Date());
    }

    public void done(View v){
        Log.i(TAG, "Datas seleciondas: "+calendar.getSelectedDates().toString());
    }
    public void aumentar(View v){
        DialogFragment newFragment = new CalendarPickerFragment();
        newFragment.show(getSupportFragmentManager(), "datePicker");

    }

    public static class CalendarPickerFragment extends DialogFragment {

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
            View rootView = inflater.inflate(R.layout.layout_calendar_picker, container, false);
            calendar = (CalendarPickerView) rootView.findViewById(R.id.calendar_view);

            final Calendar nextYear = Calendar.getInstance();
            nextYear.add(Calendar.YEAR, 1);

            final Calendar lastYear = Calendar.getInstance();
            lastYear.add(Calendar.YEAR, -1);

            calendar.init(lastYear.getTime(), nextYear.getTime()) //
                    .inMode(CalendarPickerView.SelectionMode.MULTIPLE) //
                    .withSelectedDate(new Date());

            getDialog().setTitle("Simple Dialog");
            return rootView;
        }
        @Override
        public void onResume() {
            ViewGroup.LayoutParams params = getDialog().getWindow().getAttributes();
            params.width = LinearLayout.LayoutParams.MATCH_PARENT;
            params.height = LinearLayout.LayoutParams.MATCH_PARENT;
            getDialog().getWindow().setAttributes((android.view.WindowManager.LayoutParams) params);

            super.onResume();
        }
    }

    @Override
    protected void onResume(){
        super.onResume();
        savedActionsLayout.removeAllViews();

        //Atualiza os sensores de temperatura e umidade
        Communication commTemperatura = new Communication(tvTemperatura.getTag().toString(), Util.SERVER_IP, Util.SENSOR_PORT, tvTemperatura);
        commTemperatura.sendData();

        Communication commUmidade= new Communication(tvUmidade.getTag().toString(), Util.SERVER_IP, Util.SENSOR_PORT, tvUmidade);
        commUmidade.sendData();

        //Pega as ações salvas no sharedPref e cria a lista com botões das ações
        SharedPreferences sharedPref = getApplicationContext().getSharedPreferences(ManageActionActivity.ACTION_FILE_KEY, Context.MODE_PRIVATE);
        Map<String, ?> allEntries = sharedPref.getAll();
        for (Map.Entry<String, ?> entry : allEntries.entrySet()) {
            LinearLayout line = new LinearLayout(getApplicationContext());

            //Cria o botão da ação em si
            Button btAction = new Button(getApplicationContext());
            btAction.setText(entry.getKey());
            btAction.setTag(entry.getValue().toString());
            btAction.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT,150,0.5f));
            btAction.setOnClickListener(new ExecuteActionClickListener());
            btAction.setTransformationMethod(null);

            //Cria o botão para editar a ação
            Button btEdit = new Button(getApplicationContext());
            btEdit.setText("Edit");
            btEdit.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT,100,1f));
            JSONObject actionButton = new JSONObject();
            try {
                actionButton.put("actionName", btAction.getText().toString());
                actionButton.put("action",entry.getValue().toString());
            } catch (JSONException e) {
                e.printStackTrace();
            }

            btEdit.setTag(actionButton.toString());
            btEdit.setOnClickListener(new EditActionClickListener());

            line.addView(btAction);
            line.addView(btEdit);
            savedActionsLayout.addView(line);
        }
    }

    /**
     * Editar as ações de um botão de ação
     */
    private class EditActionClickListener implements View.OnClickListener{
        @Override
        public void onClick(View v){
            Intent intent = new Intent(getApplicationContext(), ManageActionActivity.class);
            intent.putExtra("Tag", v.getTag().toString());
            startActivity(intent);
        }
    }

    /**
     * Executa a ação que o botão possui
     */
    private class ExecuteActionClickListener implements View.OnClickListener{
        @Override
        public void onClick(View v){
            JSONArray action = null;
            try {
                action = new JSONArray(v.getTag().toString());
            } catch (Exception e){
                e.printStackTrace();
            }


            Communication comm = new Communication(v.getTag().toString(), Util.SERVER_IP, Util.COMMAND_PORT, null);
            comm.sendData();
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
                Intent intent = new Intent(this, ManageRecurringActionActivity.class);
                startActivity(intent);
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }


}
