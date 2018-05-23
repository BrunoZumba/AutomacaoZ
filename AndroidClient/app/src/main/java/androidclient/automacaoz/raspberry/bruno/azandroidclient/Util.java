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
import java.util.Calendar;
import java.util.List;

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
//    public static String SERVER_IP = "192.168.25.10"; //IP VirtualBox
    public static String SERVER_IP = "192.168.25.6"; //IP VirtualBox
//    public static String SERVER_IP = "192.168.43.169"; //IP VirtualBox

    private static Util instance = new Util();

    public static final Calendar nextYear = Calendar.getInstance();
    public static final Calendar lastYear = Calendar.getInstance();


    private TextView responseTextView;

    public Util(){
    }

    public static Util getInstance(){
        return instance;
    }

    /**
     * Compara uma List<Long> retornando o menor e o maior valor
     * @param list
     * @return [0] retorno o menor valor e [1] retorna o maior
     */
    public static Long[] getMinMax(List<Long> list){
        if(list.size() == 0) {
            Log.e(TAG, "Tentativa de comprar Lista vazia");
            return null;
        }
        Long[] longArray = {list.get(0), list.get(0)};

        for(int i = 0; i < list.size(); i++){
            if(list.get(i) < longArray[0]) longArray[0] = list.get(i);
            else if(list.get(i) > longArray[1]) longArray[1] = list.get(i);
        }
        return longArray;
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
