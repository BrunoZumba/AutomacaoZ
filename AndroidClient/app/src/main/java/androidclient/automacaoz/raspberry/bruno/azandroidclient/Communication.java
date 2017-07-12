package androidclient.automacaoz.raspberry.bruno.azandroidclient;

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

/**
 * Created by Bruno on 08/06/2017.
 *
 * Utilizada para realizar a transferência de dados entre cliente e servidor.
 */

public class Communication {
    private final String TAG = "COMMUNICATION";
    public static final int STATUS_OK = 1;
    public static final int STATUS_ERROR = 0;

    //Dado que ira ser enviado ao servidor
    private String data;

    //IP do servidor
    private String serverIP;

    //Porta de comunicação com o servidor
    private int serverPort;

    //Textview onde será colocada a resposta do servidor
    private TextView tvResponse;

    //Status da resposta do servidor. Que pode ser STATUS_OK ou STATUS_ERROR
    private int responseStatus;

    //String com a resposta do servidor
    private String response;

    public Communication(String data, String serverIP, int port, TextView tvResponse) {
        this.data = data;
        this.serverIP = serverIP;
        this.serverPort = port;
        this.tvResponse = tvResponse;
    }

    /**
     * Envia a mensagem para o servidor atraves da classe Communication
     * @return True se sucesso; False caso contrario
     */
    public boolean sendData(){
        if (tvResponse != null) tvResponse.setText("");

        new AsyncActionSendData().execute();
        return true;
    }

    /**
     * AsyncTask usada para enviar a mensagem e não travar a UI MainThread do aplicativo
     */
    private class AsyncActionSendData extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... b){
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
                response = "Erro ao criar o socket com o servidor: "+e.getMessage();
                return null;
            }

            //Verifica se DATA a ser enviado eh apenas uma TASK/TAREFA (apenas um comando)
            //Ou se é uma ACTION/ACAO (com varios comandos)
            JSONArray taskButtonArray;
            JSONArray taskArray = new JSONArray();
            try {
                //Tenta colocar DATA como um json. Caso sucesso, eh pq DATA é apenas
                //uma TASK (um unico comando).
                JSONObject json = new JSONObject(data);

                //Adicionar esse objeto em um JSONArray e segue o bonde para enviar para server
                taskArray.put(json);

                //Se falhar, eh pq DATA eh um TASK BUTTON, aí separa em um array com varias TASKS
            } catch (JSONException e){
                try {
                    //Coloca DATA como um JSONArray
                    taskButtonArray = new JSONArray(data);
                    for (int i = 0; i < taskButtonArray.length(); i++){
                        JSONObject taskButton = new JSONObject(taskButtonArray.get(i).toString());
                        taskArray.put(taskButton.get("Task"));
                    }
                } catch (JSONException e1) {
                    e1.printStackTrace();
                }
            }

            //Envia todas as TASKS do array para o servidor enquanto a resposta do servidor
            //estiver sendo sucesso. Se alguma mensagem falhar e obtiver resposta de erro
            //já sai do "for" e emite nfcAlert de erro
            responseStatus = STATUS_OK;
            for (int i = 0; ((i < taskArray.length()) && (responseStatus == STATUS_OK)); i++) {
                try {
                    out.print(taskArray.get(i).toString());
                } catch (Exception e) {
                    e.printStackTrace();
                }
                out.flush();

                char[] buffer = new char[Util.BUFFER_SIZE];
                String msg = "";
                try {
                    int read = in.read(buffer);
                    msg = new String(buffer);
                } catch (Exception e) {
                    e.printStackTrace();
                    return null;
                }

                /**
                 * O servidor envia a resposta do comando no formato JSON:
                 * {"responseStatus":"STATUS_ERROR | STATUS_OK", "responseDesc":"DESCRICAO DA RESPOSTA"}
                 *
                 * O trecho abaixo do código formata essa resposta e a insere na classe Communication
                 */
                if (msg != "") {
                    JSONObject jsonResponse = null;
                    try {
                        jsonResponse = new JSONObject(msg);
                        responseStatus = jsonResponse.getInt("responseStatus");
                        response = jsonResponse.getString("responseDesc");
                    } catch (JSONException e) {
                        responseStatus = STATUS_ERROR;
                        response = "Erro ao formatar a resposta do servidor: " + e.getMessage();
                        e.printStackTrace();
                        Log.e(TAG, "Erro ao formatar o JSON de resposta: " + e.getMessage());
                    }
                } else {
                    responseStatus = STATUS_ERROR;
                    response = "Erro ao ler a resposta do servidor";
                    return null;
                }


                //TODO:Depois que coloocar no celular, verificar se isso funciona.
                try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                    Log.i(TAG, "Erro no sleep");
                    e.printStackTrace();
                }
            }

            try {
                sock.close();
            } catch (IOException e) {
                Log.e(TAG, "Fechar socket");
                e.printStackTrace();
            }

            return null;
        }

        @Override
        protected void onPostExecute(Void c){
            switch (serverPort){
                case Util.SENSOR_PORT:
                    if ((tvResponse == null) || (response == null)) return;
                    if (responseStatus == STATUS_OK) {
                        tvResponse.setText(response);
                    } else {
                        tvResponse.setText("??");
                    }
                    break;
                case Util.COMMAND_PORT:
                    if ((tvResponse == null) || (response == null)) return;
                    tvResponse.setText(response);
                    break;
                case Util.SYSTEM_ACTION_PORT:
                    Toast.makeText(MainActivity.activity.getApplicationContext(), response, Toast.LENGTH_SHORT).show();
            }
        }
    }


    public String getData() {return data;}
    public void setData(String data) {this.data = data;}
    public String getServerIP() {return serverIP;}
    public void setServerIP(String serverIP) {this.serverIP = serverIP;}
    public int getServerPort() {return serverPort;}
    public void setServerPort(int serverPort) {this.serverPort = serverPort;}
    public TextView getTvResponse() {return tvResponse;}
    public void setTvResponse(TextView tvResponse) {this.tvResponse = tvResponse;}
    public int getResponseStatus() {return responseStatus;}
    public void setResponseStatus(int responseStatus) {this.responseStatus = responseStatus;}
    public String getResponse() {return response;}
    public void setResponse(String response) {this.response = response;}
}
