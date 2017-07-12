package androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses;

import android.os.AsyncTask;
import android.util.Log;
import android.widget.TextView;

import org.json.JSONException;
import org.json.JSONObject;

import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.SensorClass;

/**
 * Created by Bruno on 24/06/2017.
 */

public class SensorCommand extends Command {
    private final String TAG = "SENSOR_COMMAND";
    SensorClass sensor;

    public SensorCommand(String requestAction, boolean requestOverwrite, String serverIP, int serverPort, TextView tvResponse, SensorClass sensor) {
        super(requestAction, requestOverwrite, serverIP, serverPort, tvResponse);
        this.sensor = sensor;
    }

    public boolean sendData(){
        super.parseRequestToJson();
        try {
            requestJson.put("sensor", new JSONObject(sensor.parseToJson()));
        } catch (JSONException e) {
            e.printStackTrace();
            Log.i(TAG, "Erro ao criar o JSON");
            return false;
        }


        if (getTvResponse()!= null) getTvResponse().setText("");
        new AsyncActionSendData().execute();

        return true;
    }

    /**
     * AsyncTask usada para enviar a mensagem e não travar a UI MainThread do aplicativo
     */
    private class AsyncActionSendData extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... b){
            fatherClass.sendAndReceiveData();

            return null;
        }

        @Override
        protected void onPostExecute(Void c){
            if ((fatherClass.getTvResponse() == null) || (fatherClass.getResponseDesc()== null)) return;
            if (fatherClass.getResponseStatus() == STATUS_OK) {
                String resposta = "";
                if(sensor.getSensorName().equals("temperatura")) {
                    resposta = fatherClass.getResponseDesc() + "ºC";
                }else if(sensor.getSensorName().equals("umidade")) {
                    resposta = fatherClass.getResponseDesc() + "%";
                }
                fatherClass.getTvResponse().setText(resposta);
            } else {
                fatherClass.getTvResponse().setText("??");
            }
        }
    }

    public SensorClass getSensor() {
        return sensor;
    }
    public void setSensor(SensorClass sensor) {
        this.sensor = sensor;
    }
}
