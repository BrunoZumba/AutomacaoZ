package androidclient.automacaoz.raspberry.bruno.azandroidclient;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;

import com.squareup.timessquare.CalendarPickerView;

import org.json.JSONArray;
import org.json.JSONException;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;

import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.RecurringActionClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses.RecurringActionCommand;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.R;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.Util;

/**
 * Created by Bruno on 26/06/2017.
 */
public class RecurringActionActivity extends AppCompatActivity {
    private static final String TAG = "RECURRING_ACTION_ACT";

    Button btAddRecurringAction; // Botão para ir para a tela de adicionar nova Ação Recorrente
    private static LinearLayout savedRecActLayout; //ScrollView para armazenar a lista de Ações Recorrentes
    private static Activity activity;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_recurring_action);
        activity = this;

        btAddRecurringAction = (Button) findViewById(R.id.btAddRecurringAction);
        savedRecActLayout = (LinearLayout) findViewById(R.id.savedRecActLayout);

    }

    @Override
    protected void onResume() {
        super.onResume();
        savedRecActLayout.removeAllViews();

        //Pega as ações recorrentes salvas no servidor e cria a lista com botões das ações recorrentes
        RecurringActionCommand recurringActionCommand = new RecurringActionCommand("getList", false, Util.SERVER_IP, Util.RECURRING_ACTION_BUTTON_PORT, null, null);
        recurringActionCommand.sendData(this);
    }


    //Depois função chamada depois que o sendData do ActionButtonCommand é retornado com o GetList
    public static void inflateRecurringActionLayout(String responseParm){
        Log.i(TAG, "Parm: " + responseParm);
        JSONArray jsonArray;

        try {
            jsonArray = new JSONArray(responseParm);
            for (int i = 0; i < jsonArray.length(); i++){
                RecurringActionClass recAct = new RecurringActionClass(jsonArray.get(i).toString());
                LinearLayout lineLayout = new LinearLayout(RecurringActionActivity.activity);
                LinearLayout datesLayout = new LinearLayout(RecurringActionActivity.activity);
                Log.i(TAG, "Dates: "+recAct.getRecActDates().toString());

                //Cria o botão com a Ação Recorrente
                Button btRecAct = new Button(RecurringActionActivity.activity);
                btRecAct.setText(recAct.getRecActName());
                btRecAct.setTag(jsonArray.get(i).toString());
                btRecAct.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT,0.5f));
                btRecAct.setOnClickListener(new EditRecurringActionClickListener());
                btRecAct.setTransformationMethod(null);
                lineLayout.addView(btRecAct);

                //Cria o texto com as datas de início e fim da Ação Recorrente
                TextView tvDataInicio = new TextView(RecurringActionActivity.activity);
                tvDataInicio.setGravity(Gravity.CENTER_HORIZONTAL);
                TextView tvDataFim = new TextView(RecurringActionActivity.activity);
                tvDataFim.setGravity(Gravity.CENTER_HORIZONTAL);
                Long[] dataMinMax = Util.getMinMax(recAct.getRecActDates());

                SimpleDateFormat dateFormat = new SimpleDateFormat("dd/MM/yyyy", Locale.getDefault());
                String temp = "Data Inicial " + dateFormat.format(new Date(dataMinMax[0]));
                tvDataInicio.setText(temp);
                temp = "Data Final " + dateFormat.format(new Date(dataMinMax[1]));
                tvDataFim.setText(temp);

                datesLayout.setOrientation(LinearLayout.VERTICAL);
                datesLayout.addView(tvDataInicio);
                datesLayout.addView(tvDataFim);
                datesLayout.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT,0.5f));

                lineLayout.addView(datesLayout);

                savedRecActLayout.addView(lineLayout);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }
    /**
     * Editar as ações de um botão de ação
     */
    private static class EditRecurringActionClickListener implements View.OnClickListener{
        @Override
        public void onClick(View v){
            Intent intent = new Intent(activity, ManageRecurringActionActivity.class);
            intent.putExtra("Tag", v.getTag().toString());
            activity.startActivity(intent);
        }
    }

    public void addRecurringAction(View v){
        Intent intent = new Intent(this, ManageRecurringActionActivity.class);
        startActivity(intent);
    }
}
