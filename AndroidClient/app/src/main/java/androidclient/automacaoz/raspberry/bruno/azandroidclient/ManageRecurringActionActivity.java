package androidclient.automacaoz.raspberry.bruno.azandroidclient;

import android.app.Activity;
import android.app.Dialog;
import android.app.TimePickerDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.util.SortedList;
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
import org.json.JSONObject;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.TimeZone;

import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.ActionButtonClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.ActionClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.RecurringActionClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.TaskButtonClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses.ActionButtonCommand;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses.Command;
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


    private static CalendarPickerView calendar; //Instancia do Calendar dentro desta tela

    /** Feito static para ser acessado de dentro de uma subclasse */
    private static List<Date> selecteDates; //Datas selecionadas no CalendarPickerView

    /** DialogFragment usado para mostrar o calendario em tela cheia.
     * Feito static para ser acessado de dentro do clickListener dos bt "Cancelar" e "OK" */
    private static DialogFragment fragment;

    /**CloseListener utilizado para que quando seja chamado o Dismiss no dialogFragment,
     * as datas no calendario sejam atualizadas, conforme
     * https://stackoverflow.com/questions/24151891/callback-when-dialogfragment-is-dismissed */
    private static MyDialogCloseListener closeListener;

//    static List<RadioButton> radioButtonList; //Gerenciar a lista com todas as ações cadastradas
    static RadioGroup rgActions; //RadioGroup onde estão os radioButtons com as Ações
    static EditText etRecurringActionName; //Nome desta Ação Recorrente

    public static Activity activity; //public static para ser acessado pela RecActCommand

    /** Guarda a TAG do intent que trouxe o usuario aqui, caso ela tenha chegado nesta
     * tela a partir do botao de editar uma ação recorrente. Neste caso, a TAG possui
     * o JSON de toda a ação recorrente
     */
    private static String intentTag = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_manage_recurring_action);

        activity = this;

        hourMinuteList = new ArrayList<>();
        selecteDates = new ArrayList<>();
//        radioButtonList = new ArrayList<>();
//        times = new ArrayList<>();

        layoutHourMinute = (LinearLayout) findViewById(R.id.layoutHourMinute);
        layoutDeleteBt = (LinearLayout) findViewById(R.id.layoutDeleteBt);
        calendar = (CalendarPickerView) findViewById(R.id.calendarPicker);
        rgActions = (RadioGroup) findViewById(R.id.rgActions);
        etRecurringActionName = (EditText) findViewById(R.id.etRecurringActionName);


        Util.nextYear.add(Calendar.YEAR, 1);
        Util.lastYear.add(Calendar.YEAR, -1);
        calendar.init(Util.lastYear.getTime(), Util.nextYear.getTime()) //
                .inMode(CalendarPickerView.SelectionMode.MULTIPLE) //
                .withSelectedDates(selecteDates);

        /**
         * Verificar se o Intent que trouxe o usuario a esta tela é para editar
         * alguma Ação Recorrente especifica. Em caso positivo, a "Tag" do Intent possui a TAG
         * completa da Ação Recorrente: {"recActName":"NOME_ACAO", "recActDates":[...], etc...}
         *
         * Apenas guarda a TAG em 'intentTag'. A tela só pode ser preenchida após a lista
         * de radioButtons ser "inflada" no método 'InflateActionsButtonLaoyt'
         */
        Intent intent = getIntent();
        intentTag = intent.getStringExtra("Tag");

    }

    /**
     * Cria a tela de editar a Ação Recorrente com todos os campos preenchidos
     */
    private static void inflateRecurringAction(String tag){
        RecurringActionClass recAct = new RecurringActionClass(tag);
        etRecurringActionName.setText(recAct.getRecActName());

//        //Marca o RadioButton com a ação desejada
//        for (int i = 0; i < radioButtonList.size(); i++){
//            if (radioButtonList.get(i).getText().equals(recAct.getRecActActionButton().getActionName())){
//                rgActions.check(radioButtonList.get(i).getId());
//            }
//        }

        //Preenche a lista com horários
        for (int i = 0; i < recAct.getRecActTimes().size(); i++){
            SimpleDateFormat dateFormat = new SimpleDateFormat("HH:mm", Locale.getDefault());
            dateFormat.setTimeZone(TimeZone.getTimeZone("GMT"));
            TextView tv = new TextView(ManageRecurringActionActivity.activity);
            tv.setText(dateFormat.format(new Date(recAct.getRecActTimes().get(i))));
            tv.setGravity(Gravity.CENTER_HORIZONTAL);
            tv.setTextSize(35);
            tv.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, 100));

            ManageRecurringActionActivity.layoutHourMinute.addView(tv);
            //Adiciona o TextView na List
            ManageRecurringActionActivity.hourMinuteList.add(tv);

            //Cria o botao 'X' e adicionar no layout de botao
            Button bt = new Button(MainActivity.activity);
            bt.setText("X");
            bt.setTag(tv);
            bt.setOnClickListener(new clickListenerDeleteHour());
            bt.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, 100));
            ManageRecurringActionActivity.layoutDeleteBt.addView(bt);
        }

        //preenche a lista de datas no CalendarPickerView
        selecteDates = new ArrayList<>();
        for (int i = 0; i < recAct.getRecActDates().size(); i++){
            selecteDates.add(new Date(recAct.getRecActDates().get(i)));
        }
        calendar.init(Util.lastYear.getTime(), Util.nextYear.getTime()) //
                .inMode(CalendarPickerView.SelectionMode.MULTIPLE) //
                .withSelectedDates(selecteDates);

    }

    /**
     * Cria a lista de RadioButtons baseado nas ações disponíveis ao usuário
     * @param responseParm
     */
    public static void inflateActionButtonLayout(String responseParm){
        JSONArray jsonArray;
        try {
            jsonArray = new JSONArray(responseParm);
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
//                radioButtonList.add(radioAction);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }

        /**Se houver um intent com EXTRA = RECURRING ACTION, é para editar a ação **/
        if ((intentTag != null) && (!intentTag.equals(""))){
            inflateRecurringAction(intentTag);
        }
    }

    @Override
    protected void onResume(){
        super.onResume();

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
                calendar.init(Util.lastYear.getTime(), Util.nextYear.getTime()) //
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

            calendarInsideFragment.init(Util.lastYear.getTime(), Util.nextYear.getTime()) //
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
            SimpleDateFormat dateFormat = new SimpleDateFormat("HH:mm", Locale.getDefault());
            dateFormat.setTimeZone(TimeZone.getTimeZone("GMT"));
            //HH:mm transformado em milisegundos (do dia 01/01/1970)
            long milis = ((hourOfDay * 60) + minuteOfHour) * 60 * 1000;

            TextView tv = new TextView(ManageRecurringActionActivity.activity);
            tv.setText(dateFormat.format(new Date(milis)));
            tv.setGravity(Gravity.CENTER_HORIZONTAL);
            tv.setTextSize(35);
            tv.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, 100));
            ManageRecurringActionActivity.layoutHourMinute.addView(tv);

            //Adiciona o TextView na List
            ManageRecurringActionActivity.hourMinuteList.add(tv);

            //Cria o botao 'X' e adiciona no layout de botao
            Button bt = new Button(MainActivity.activity);
            bt.setText("X");
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

    public void deleteRecurringAction(View v){
        //Verifica se o nome da acao esta vazio
        if (etRecurringActionName.getText().toString() == ""){
            Toast.makeText(getApplicationContext(), "Digite o nome da Ação Recorrente que deseja excluir", Toast.LENGTH_SHORT).show();
            return;
        }

        //Para deletar a ação recorrente não é necessário passar toda o JSONString da ação recorrente, basta o nome. Portanto Dates, Times e ActionButtonClass são criados vazios
        RecurringActionClass recurringActionClass = new RecurringActionClass(etRecurringActionName.getText().toString(), new ArrayList<Long>(), new ArrayList<Long>(), new ActionButtonClass("", "[]"));
        RecurringActionCommand recurringActionCommand = new RecurringActionCommand("deleteRecurringAction", false, Util.SERVER_IP, Util.RECURRING_ACTION_BUTTON_PORT, null, recurringActionClass);

        recurringActionCommand.sendData(this);
    }

    public static void processResponse(final ActionButtonCommand response){
        if (response.getRequestAction().equals("getList")){
            switch (response.getResponseStatus()) {
                case (Command.STATUS_ERROR):
                    Toast.makeText(ManageRecurringActionActivity.activity.getApplicationContext(), response.getResponseDesc(), Toast.LENGTH_SHORT).show();
                    break;

                case (Command.STATUS_OK):
                    ManageRecurringActionActivity.inflateActionButtonLayout(response.getResponseParm());
                    break;
            }
        } else { //deleteRecurringAction e saveRecurringAction
            Toast.makeText(ManageRecurringActionActivity.activity.getApplicationContext(), response.getResponseDesc(), Toast.LENGTH_SHORT).show();
        }
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

        //Transforma o array de TextView contendo a hora no formato hh:mm em um array de Long em ordem crescente
        List<Long> times = new ArrayList<>();
        Date date;
        SimpleDateFormat dateFormat = new SimpleDateFormat("HH:mm", Locale.getDefault());
        dateFormat.setTimeZone(TimeZone.getTimeZone("GMT"));
        try {
            date = dateFormat.parse(hourMinuteList.get(0).getText().toString());
            //date = new SimpleDateFormat("HH:mm", Locale.getDefault()).parse(hourMinuteList.get(0).getText().toString());
            times.add(date.getTime());
            for (int i = 1; i < hourMinuteList.size(); i++){
                date = dateFormat.parse(hourMinuteList.get(i).getText().toString());

                times.add(date.getTime()); //adiciona no final
                for(int a = 0; a < times.size(); a++){
                    if (date.getTime() < times.get(a)){
                        times.add(a, date.getTime());
                        times.remove(times.size()-1); //remove o último, adicionado antes do for
                        break;
                    }
                }
            }
        } catch (ParseException e) {
            e.printStackTrace();
            Log.e(TAG, "Nao foi possivel realizer o DateParse do horário selecionado.");
        }

        //Transforma o array de Dates contendo as datas em um um array de long
        List<Long> dates = new ArrayList<>();
        for (int i = 0; i < calendar.getSelectedDates().size(); i++){
            dates.add(calendar.getSelectedDates().get(i).getTime());
        }

        //Transforma os dados do radioButton escolhido em um ActionButtonClass
        ActionButtonClass actionButton = new ActionButtonClass(selectedRadio.getText().toString(), selectedRadio.getTag().toString());



        RecurringActionClass recurringAction = new RecurringActionClass(
                etRecurringActionName.getText().toString(), dates, times, actionButton);

        RecurringActionCommand recurringActionCommand = new RecurringActionCommand("saveRecurringAction", false, Util.SERVER_IP, Util.RECURRING_ACTION_BUTTON_PORT, null, recurringAction);
        recurringActionCommand.sendData(this);
    }

}


