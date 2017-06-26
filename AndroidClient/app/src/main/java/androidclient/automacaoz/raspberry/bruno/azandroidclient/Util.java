package androidclient.automacaoz.raspberry.bruno.azandroidclient;

import android.content.Intent;
import android.nfc.NdefMessage;
import android.nfc.NfcAdapter;
import android.os.AsyncTask;
import android.os.Parcelable;
import android.util.Log;
import android.widget.TextView;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;

/**
 * Created by Bruno on 24/05/2017.
 */

public class Util {
    private static final String TAG = "UTIL_JAVA";

    public final static int COMMAND_PORT = 4391; //Comandos dos controles remotos
    public final static int SENSOR_PORT = 8742; //Sensores (temperatura, umidade, luminosidade)
    public final static int SYSTEM_ACTION_PORT = 8168; //Ação no sistema (reboot, shutdown, etc)
    public final static int ACTION_BUTTON_PORT = 5223; //Ação no sistema (reboot, shutdown, etc)
    public final static int RECURRING_ACTION_BUTTON_PORT = 6292; //Ação no sistema (reboot, shutdown, etc)
    public final static int BUFFER_SIZE = 4096;
//    public static String SERVER_IP = "192.168.25.50"; //IP do Raspberry
//    public static String SERVER_IP = "191.33.154.250"; //IP de Internet (port forwarding)
    public static String SERVER_IP = "192.168.25.7"; //IP VirtualBox

    private static Util instance = new Util();


    private TextView responseTextView;

    public Util(){
    }

    public static Util getInstance(){
        return instance;
    }

    /**
     * A partir do Intent de um NDEF_DISCOVERY_ACTION, busca todas NdefMessage desta acao
     * @param intent
     * @return
     */
    public NdefMessage[] getAllNdefMessages(Intent intent){
        NdefMessage[] msgs = null;
        Parcelable[] rawMsgs = intent.getParcelableArrayExtra(NfcAdapter.EXTRA_NDEF_MESSAGES);
        if (rawMsgs != null) {
            msgs = new NdefMessage[rawMsgs.length];
            for (int i = 0; i < rawMsgs.length; ++i) {
                msgs[i] = (NdefMessage)rawMsgs[i];
            }
        }

        return msgs;
    }


}
