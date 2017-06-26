package androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses;

import android.os.AsyncTask;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONArray;
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

import androidclient.automacaoz.raspberry.bruno.azandroidclient.MainActivity;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.Util;

/**
 * Created by Bruno on 24/06/2017.
 */

public class Command {
    private final String TAG = "COMMAND";

    public static final int STATUS_OK = 1;
    public static final int STATUS_ERROR = 0;

    private String requestAction;
    private boolean requestOverwrite;

    private int responseStatus;
    private String responseAction;
    private String responseDesc;
    private String responseParm;

    JSONObject requestJson;

    //IP do servidor
    private String serverIP;
    //Porta de comunicação com o servidor
    private int serverPort;
    //Textview onde será colocada a resposta do servidor
    private TextView tvResponse;

    //Usado para ter acesso à classe pai (Command) à partir da AsyncTask
    Command fatherClass;

    public Command(String requestAction, boolean requestOverwrite, String serverIP, int serverPort, TextView tvResponse) {
        this.requestAction = requestAction;
        this.requestOverwrite = requestOverwrite;
        this.serverIP = serverIP;
        this.serverPort = serverPort;
        this.tvResponse = tvResponse;

        fatherClass = this;

        responseAction = null;
        responseDesc= null;
        responseParm = null;
    }

    public boolean parseRequestToJson(){
        requestJson = new JSONObject();
        try {
            requestJson.put("requestAction", requestAction);
            requestJson.put("requestOverwrite", requestOverwrite);
        } catch (JSONException e) {
            e.printStackTrace();
            Log.i(TAG, "Erro ao criar o JSON");
            return false;
        }

        return true;
    }


    /**
     * Esta' como doInBackground pq é chamado pela doInBackgroud da asynctask da classe filha
     * @return
     */
    public boolean sendAndReceiveData(){
        //Abre o socket e pega os PrintWriter e BufferedReader
        Socket sock = new Socket();
        PrintWriter out = null;
        BufferedReader in = null;


        try {
            SocketAddress serverAddr = new InetSocketAddress(serverIP, serverPort);
            sock.connect(serverAddr, 2000); //2 second connection timeout
            out = new PrintWriter(new OutputStreamWriter(sock.getOutputStream()));
            in = new BufferedReader(new InputStreamReader(sock.getInputStream()));
        } catch (Exception e) {
            Log.e(TAG, "Erro ao criar socket/in/out");
            e.printStackTrace();
            responseStatus = STATUS_ERROR;
            responseDesc = "Erro ao criar o socket com o servidor: "+e.getMessage();
            return false;
        }


        out.print(requestJson.toString());
        out.flush();

        char[] buffer = new char[Util.BUFFER_SIZE];
        String msg = "";
        try {
            int read = in.read(buffer);
            msg = new String(buffer);
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }

        /**
         * Coloca a resposta do servidor nas variaveis de resposta
         */
        if (msg != "") {
            JSONObject jsonResponse = null;
            try {
                jsonResponse = new JSONObject(msg);
                responseStatus = jsonResponse.getInt("responseStatus");
                responseAction = jsonResponse.getString("responseAction");
                responseDesc = jsonResponse.getString("responseDesc");
                responseParm = jsonResponse.getString("responseParm");
            } catch (JSONException e) {
                responseStatus = STATUS_ERROR;
                responseDesc = "Erro ao formatar a resposta do servidor: " + e.getMessage();
                e.printStackTrace();
                Log.e(TAG, "Erro ao formatar o JSON de resposta: " + e.getMessage());
            }
        } else {
            responseStatus = STATUS_ERROR;
            responseDesc = "Erro ao ler a resposta do servidor";
            return false;
        }

        try {
            sock.close();
        } catch (IOException e) {
            Log.e(TAG, "Fechar socket");
            e.printStackTrace();
        }

        return true;
    }


    public String getRequestAction() {return requestAction;}
    public void setRequestAction(String requestAction) {this.requestAction = requestAction;}
    public boolean isRequestOverwrite() {return requestOverwrite;}
    public void setRequestOverwrite(boolean requestOverwrite) {this.requestOverwrite = requestOverwrite;}
    public int getResponseStatus() {return responseStatus;}
    public void setResponseStatus(int responseStatus) {this.responseStatus = responseStatus;}
    public String getResponseAction() {return responseAction;}
    public void setResponseAction(String responseAction) {this.responseAction = responseAction;}
    public String getResponseDesc() {return responseDesc;}
    public void setResponseDesc(String responseDesc) {this.responseDesc = responseDesc;}
    public String getResponseParm() {return responseParm;}
    public void setResponseParm(String responseParm) {this.responseParm = responseParm;}

    public TextView getTvResponse() {return tvResponse;}
    public void setTvResponse(TextView tvResponse) {this.tvResponse = tvResponse;}
}
