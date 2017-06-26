package androidclient.automacaoz.raspberry.bruno.azandroidclient;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.TaskClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses.TaskCommand;

public class DeviceDisplayActivity extends AppCompatActivity {
    private final String TAG = "DEVICE_DISPLAY_ACTIVITY";

    TextView tvResponse;
    EditText etIp;
    private String ip;

    static List<Button> buttonList;

    public DeviceDisplayActivity(){}

    /** Array com o nome de todos os botoes disponveis para o usuario. Utilizado para
     * criar as View "Button" a partir destes nomes
      */
    public static final String[] btListName = {"btNetExit", "btNetPower", "btNetEnter", "btNetAgora",
            "btNetChannelDown", "btNetChannelUp", "btNetDown", "btNetLeft","btNetRight","btNetUp",
            "btNetKey0","btNetKey1","btNetKey2","btNetKey3","btNetKey4","btNetKey5","btNetKey6",
            "btNetKey7","btNetKey8","btNetKey9",
            "btTVPower","btTVVolumeUp","btTvVolueDown","btTvMute",
            "btLuzOn","btLuzOff","btLuzDown", "btLuzUp","btLuz25","btLuz50","btLuz75","btLuz100"};


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_display);
        tvResponse = (TextView) findViewById(R.id.tvResponse);
        etIp = (EditText) findViewById(R.id.tvIp);
        ip = etIp.getText().toString();

        //Cria o array de botoes e adiciona o listener para executar a acao em cada um
        buttonList = new ArrayList<>();
        for (int i = 0; i < btListName.length; i++) {
            buttonList.add((Button) findViewById(getApplicationContext().getResources().
                    getIdentifier(btListName[i], "id", getApplicationContext().getPackageName())));

            buttonList.get(buttonList.size()-1).setOnClickListener(new MyOnClickListener());
        }
    }

    /**
     * Listener responsável por enviar os comandos para o servidor quando o botao e pressionado
     */
    private class MyOnClickListener implements View.OnClickListener {
        @Override
        public void onClick(View v) {
            String jsonCommand = v.getTag().toString();

            TaskCommand taskCommand = new TaskCommand("taskCommand", false, Util.SERVER_IP, Util.COMMAND_PORT, tvResponse, new TaskClass(jsonCommand));
            taskCommand.sendData();
//            Communication comm = new Communication(jsonCommand, Util.SERVER_IP, Util.COMMAND_PORT, tvResponse);
//            comm.sendData();
        }

    }
}