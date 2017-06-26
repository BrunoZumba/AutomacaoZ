package androidclient.automacaoz.raspberry.bruno.azandroidclient;

import android.app.Activity;
import android.app.Dialog;
import android.app.TimePickerDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutCompat;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.TimePicker;
import android.widget.Toast;

import com.squareup.timessquare.CalendarPickerView;

import org.json.JSONArray;
import org.json.JSONException;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.Map;

import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.ActionButtonClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.ActionClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.RecurringActionClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.TaskButtonClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses.ActionButtonCommand;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses.RecurringActionCommand;

public class ManageRecurringActionActivity extends AppCompatActivity {
    private static final String TAG = "MANAGE_RECURRING_ACTION";

    /** Array de textview contendo todos os horários escolhidos pelo usuário
     * Feito Static para poder ser acessado pelo TimePickerFragment */
    static List<TextView> hourMinuteList;

    /** Layouts onde serão inseridos os TextView com os horários e os botões de excluir os horarios
     * Feitos Static para poderem ser acessados pelo TimePickerFragment*/
    static LinearLayout layoutHourMinute;
    static LinearLayout layoutDeleteBt;
//    static List<Long> times; //A Lista que armazendo as "horas" e "minutos" desta ação recorrente


    private CalendarPickerView calendar; //Instancia do Calendar dentro desta tela

    /** Feito static para ser acessado de dentro de uma subclasse */
    private static List<Date> selecteDates; //Datas selecionadas no CalendarPickerView
    private static final Calendar nextYear = Calendar.getInstance();
    private static final Calendar lastYear = Calendar.getInstance();

    /** DialogFragment usado para mostrar o calendario em tela cheia.
     * Feito static para ser acessado de dentro do clickListener dos bt "Cancelar" e "OK" */
    private static DialogFragment fragment;

    /**CloseListener utilizado para que quando seja chamado o Dismiss no dialogFragment,
     * as datas no calendario sejam atualizadas, conforme
     * https://stackoverflow.com/questions/24151891/callback-when-dialogfragment-is-dismissed */
    private static MyDialogCloseListener closeListener;

    static List<RadioButton> radioButtonList; //Gerenciar a lista com todas as ações cadastradas
    static RadioGroup rgActions; //RadioGroup onde estão os radioButtons com as Ações
    EditText etRecurringActionName; //Nome desta Ação Recorrente

    public static Activity activity; //public static para ser acessado pela RecActCommand

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        activity = this;

        hourMinuteList = new ArrayList<>();
        selecteDates = new ArrayList<>();
        radioButtonList = new ArrayList<>();
//        times = new ArrayList<>();

        setContentView(R.layout.activity_manage_recurring_action);
        layoutHourMinute = (LinearLayout) findViewById(R.id.layoutHourMinute);
        layoutDeleteBt = (LinearLayout) findViewById(R.id.layoutDeleteBt);
        calendar = (CalendarPickerView) findViewById(R.id.calendarPicker);
        rgActions = (RadioGroup) findViewById(R.id.rgActions);
        etRecurringActionName = (EditText) findViewById(R.id.etRecurringActionName);


        nextYear.add(Calendar.YEAR, 1);
        lastYear.add(Calendar.YEAR, -1);
        calendar.init(lastYear.getTime(), nextYear.getTime()) //
                .inMode(CalendarPickerView.SelectionMode.MULTIPLE) //
                .withSelectedDates(selecteDates);
    }

    public static void inflateActionButtonLayout(String responseParm){
        JSONArray jsonArray;
        try {
            jsonArray = new JSONArray(responseParm);
            List<ActionButtonClass> actionButtons = new ArrayList<>();
            for(int i = 0; i < jsonArray.length(); i++){
                ActionButtonClass actBt = new ActionButtonClass(jsonArray.get(i).toString());
                LinearLayout line = new LinearLayout(MainActivity.activity);

                //Cria o radioButton com esta ação
                RadioButton radioAction = new RadioButton(ManageRecurringActionActivity.activity.getApplicationContext());
                radioAction.setText(actBt.getActionName());
                radioAction.setTag(actBt.getAction().parseToJson());
                radioAction.setTextColor(Color.BLACK);
                radioAction.setBackground(MainActivity.activity.getResources().getDrawable(R.drawable.simple_black_border));
                radioAction.setLayoutParams(new LinearLayoutCompat.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));


                rgActions.addView(radioAction);
                radioButtonList.add(radioAction);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    @Override
    protected void onResume(){
        super.onResume();

//        RecurringActionCommand recurringActionCommand = new RecurringActionCommand()
        //Pega as ações salvas no servidor e cria a lista com botões das ações
        ActionButtonCommand actionButtonCommand = new ActionButtonCommand("getList", false, Util.SERVER_IP, Util.ACTION_BUTTON_PORT, null, null);
        actionButtonCommand.sendData(this);

    }

    /** Funcao chamada pelo botão de aumentar o calendário */
    public void showDatePickerDialog(View v){
        selecteDates = new ArrayList<>();
        selecteDates = calendar.getSelectedDates();
        fragment = new CalendarPickerFragment();

        /** Verificar o comentario da var closeListener no inicio da classe */
        closeListener = new MyDialogCloseListener() {
            @Override
            public void handleDialogClose(DialogInterface dialog) {
                calendar = (CalendarPickerView) findViewById(R.id.calendarPicker);
                calendar.init(lastYear.getTime(), nextYear.getTime()) //
                        .inMode(CalendarPickerView.SelectionMode.MULTIPLE) //
                        .withSelectedDates(selecteDates);
            }
        };

        fragment.show(getSupportFragmentManager(), "datePicker");

    }
    public static class CalendarPickerFragment extends DialogFragment {
        Button btOK;
        Button btCancela;
        MyDialogCloseListener closeListener;

        /**Instancia do calendar dentro do DialogFrament. Feito static para ser acessado
         * pelos setListeners dos botões*/
        static CalendarPickerView calendarInsideFragment;

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
            final View rootView = inflater.inflate(R.layout.layout_calendar_picker, container, false);
            calendarInsideFragment = (CalendarPickerView) rootView.findViewById(R.id.calendar_view);
            btOK = (Button) rootView.findViewById(R.id.btOK);
            btCancela = (Button) rootView.findViewById(R.id.btCancela);
            this.closeListener = ManageRecurringActionActivity.closeListener;

            //Listener para o botão de Cancelar
            btCancela.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    fragment.dismiss();
                }
            });
            //Listener para o botão OK
            btOK.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    selecteDates = calendarInsideFragment.getSelectedDates();
                    fragment.dismiss();
                }
            });

            calendarInsideFragment.init(lastYear.getTime(), nextYear.getTime()) //
                    .inMode(CalendarPickerView.SelectionMode.MULTIPLE) //
                    .withSelectedDates(selecteDates);

//            getDialog().setTitle("Dialog Fragment");
            return rootView;
        }

        /** Utilizado para que o DialogFragment tenha o mesmo tamanho da tela */
        @Override
        public void onResume() {
            super.onResume();
            ViewGroup.LayoutParams params = getDialog().getWindow().getAttributes();
            if (params == null) return;
            params.width = LinearLayout.LayoutParams.MATCH_PARENT;
            params.height = LinearLayout.LayoutParams.MATCH_PARENT;
            getDialog().getWindow().setAttributes((android.view.WindowManager.LayoutParams) params);
        }

        /** Verificar o comentario da var closeListener no inicio da classe */
        @Override
        public void onDismiss(DialogInterface dialog) {
            super.onDismiss(dialog);
            if(closeListener!=null) {
                closeListener.handleDialogClose(null);
            }

        }

    }

    /** Função chamada pelo botão de adicionar novo horário pelo usuário*/
    public void showTimePickerDialog(View v) {
        DialogFragment newFragment = new TimePickerFragment();
        newFragment.show(getSupportFragmentManager(), "timePicker");
    }
    public static class TimePickerFragment extends DialogFragment
            implements TimePickerDialog.OnTimeSetListener {

        @Override
        public Dialog onCreateDialog(Bundle savedInstanceState) {
            // Use the current time as the default values for the picker
            final Calendar c = Calendar.getInstance();
            Calendar.getInstance();
            int hour = c.get(Calendar.HOUR_OF_DAY);
            int minute = c.get(Calendar.MINUTE);

            // Create a new instance of TimePickerDialog and return it
            return new TimePickerDialog(getActivity(), this, hour, minute, true);
        }

        public void onTimeSet(TimePicker view, int hourOfDay, int minuteOfHour) {
//            Calendar now = Calendar.getInstance();
//            now.set(Calendar.HOUR_OF_DAY, hourOfDay);
//            now.set(Calendar.MINUTE, minuteOfHour);
//            times.add(now.getTimeInMillis());
//            now.getTime().getTime();

            //Cria a string com horario, adicionando '0' antes se for menor que 10
            String hour = "", minute = "";
            if (hourOfDay < 10) hour = hour.concat("0");
            hour = hour.concat(String.valueOf(hourOfDay));
            if (minuteOfHour < 10) minute = minute.concat("0");
            minute = minute.concat(String.valueOf(minuteOfHour));

            //Cria o text view e adiciona no layout de TextView
            TextView tv = new TextView(MainActivity.activity);
            tv.setText(hour+":"+minute);
            tv.setGravity(Gravity.CENTER_HORIZONTAL);
            tv.setTextSize(35);
            tv.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, 100));
            ManageRecurringActionActivity.layoutHourMinute.addView(tv);


            //Adiciona o TextView na List
            ManageRecurringActionActivity.hourMinuteList.add(tv);

            //Cria o botao 'X' e adicionar no layout de botao
            Button bt = new Button(MainActivity.activity);
            bt.setText("X");
//            bt.setTag(ManageRecurringActionActivity.hourMinuteList.size()-1);
            bt.setTag(tv);
            bt.setOnClickListener(new clickListenerDeleteHour());
            bt.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, 100));
            ManageRecurringActionActivity.layoutDeleteBt.addView(bt);

        }
    }

    /**
     * Chamada quando o usuário clica em botao excluir um horário da ação
     */
    private static class clickListenerDeleteHour implements View.OnClickListener{
        public void onClick(View v){
            //Retira o TextView do Array da Classe principal.
            //Retira as views (textivew e botão) de seus respectivos layouts
            Button bt = (Button) v;
            ManageRecurringActionActivity.layoutDeleteBt.removeView(bt);
            ManageRecurringActionActivity.layoutHourMinute.removeView((TextView) bt.getTag());
            ManageRecurringActionActivity.hourMinuteList.remove(bt.getTag());
        }
    }

    /** Verificar o comentario da var closeListener no inicio da classe */
    public interface MyDialogCloseListener {
        public void handleDialogClose(DialogInterface dialog);
    }

    /** Chamado quando o usuário clica para salvar uma nova Ação Recorrente */
    public void saveRecurringAction(View v){
        //Realiza checagens dos dados
        if((etRecurringActionName == null) || (etRecurringActionName.getText().toString().equals(""))){
            Toast.makeText(getApplicationContext(), "Digite um nome para esta Ação Recorrente", Toast.LENGTH_SHORT).show();
            return;
        }
        if(rgActions.getCheckedRadioButtonId() == -1) {
            Toast.makeText(getApplicationContext(), "Selecione uma ação", Toast.LENGTH_SHORT).show();
            return;
        }
        if(calendar.getSelectedDates().size() == 0) {
            Toast.makeText(getApplicationContext(), "Selecione pelo menos uma data", Toast.LENGTH_SHORT).show();
            return;
        }
        if(hourMinuteList.size() == 0){
            Toast.makeText(getApplicationContext(), "Adicione pelo menos um horário", Toast.LENGTH_SHORT).show();
            return;
        }

        //RadioButton selecionado
        RadioButton selectedRadio = (RadioButton) findViewById(rgActions.getCheckedRadioButtonId());

        //Transforma o array de TextView contendo a hora no formato hh:mm em um array de Long
        List<Long> times = new ArrayList<>();
        Calendar now = Calendar.getInstance();
        for (int i = 0; i < hourMinuteList.size(); i++){
            String[] split = hourMinuteList.get(i).getText().toString().split(":");
            int hour = Integer.valueOf(split[0]);
            int minutes = Integer.valueOf(split[1]);
            now.set(Calendar.HOUR_OF_DAY, hour);
            now.set(Calendar.MINUTE, minutes);
            times.add(now.getTimeInMillis() / 1000); /** Divido por 1000 para pegar em segundos*/
        }

        //Transforma o array de Dates contendo as datas em um um array de long
        List<Long> dates = new ArrayList<>();
        for (int i = 0; i < calendar.getSelectedDates().size(); i++){
            //Dividido por 1000 para pegar em segundos
            dates.add(calendar.getSelectedDates().get(i).getTime() / 1000);
        }

        //Transforma os dados do radioButton escolhido em um ActionButtonClass
        ActionButtonClass actionButton = new ActionButtonClass(selectedRadio.getText().toString(), selectedRadio.getTag().toString());
//        Log.i(TAG, "Action: "+actionButton.parseToJson());



        RecurringActionClass recurringAction = new RecurringActionClass(
                etRecurringActionName.getText().toString(), dates, times, actionButton);
        Log.i(TAG,"Dates: "+recurringAction.getRecActDates());

        RecurringActionCommand recurringActionCommand = new RecurringActionCommand("saveList", false, Util.SERVER_IP, Util.RECURRING_ACTION_BUTTON_PORT, null, recurringAction);
        recurringActionCommand.sendData();

    }

}


