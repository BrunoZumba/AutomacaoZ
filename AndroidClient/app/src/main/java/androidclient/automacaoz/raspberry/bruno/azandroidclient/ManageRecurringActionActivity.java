package androidclient.automacaoz.raspberry.bruno.azandroidclient;

import android.app.DatePickerDialog;
import android.app.Dialog;
import android.app.TimePickerDialog;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.support.v7.app.AppCompatActivity;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.TimePicker;

import com.squareup.timessquare.CalendarPickerView;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

public class ManageRecurringActionActivity extends AppCompatActivity {
    private static final String TAG = "MANAGE_RECURRING_ACTION";

    /** Array de textview contendo todos os horários escolhidos pelo usuário
     * Feito Static para poder ser acessado pelo TimePickerFragment */
    static List<TextView> hourMinuteList;

    /** Layouts onde serão inseridos os TextView com os horários e os botões de excluir os horarios
     * Feitos Static para poderem ser acessados pelo TimePickerFragment*/
    static LinearLayout layoutHourMinute;
    static LinearLayout layoutDeleteBt;


    private CalendarPickerView calendar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_manage_recurring_action);
        hourMinuteList = new ArrayList<>();
        layoutHourMinute = (LinearLayout) findViewById(R.id.layoutHourMinute);
        layoutDeleteBt = (LinearLayout) findViewById(R.id.layoutDeleteBt);
    }




    public void showDatePickerDialog(View v) {
        DialogFragment newFragment = new DatePickerFragment();
        newFragment.show(getSupportFragmentManager(), "datePicker");

    }

    public static class DatePickerFragment extends DialogFragment
            implements DatePickerDialog.OnDateSetListener {

        @Override
        public Dialog onCreateDialog(Bundle savedInstanceState) {
            // Use the current date as the default date in the picker
            final Calendar c = Calendar.getInstance();
            int year = c.get(Calendar.YEAR);
            int month = c.get(Calendar.MONTH);
            int day = c.get(Calendar.DAY_OF_MONTH);

            // Create a new instance of DatePickerDialog and return it
//            DateP
            return new DatePickerDialog(getActivity(), this, year, month, day);
        }

        public void onDateSet(DatePicker view, int year, int month, int day) {
            // Do something with the date chosen by the user
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

}


