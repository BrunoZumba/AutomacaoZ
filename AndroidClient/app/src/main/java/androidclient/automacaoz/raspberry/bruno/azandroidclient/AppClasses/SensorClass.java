package androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses;

import android.util.Log;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by Bruno on 24/06/2017.
 */

public class SensorClass {
    private final String TAG = "SENSOR_CLASS";
    String sensorName;
    String sensorAction;

    public SensorClass(String sensorName, String sensorAction) {
        this.sensorName = sensorName;
        this.sensorAction = sensorAction;
    }

    public SensorClass(String jsonObj){
        JSONObject json;
        try {
            json = new JSONObject(jsonObj);
        } catch (Exception e){
            e.printStackTrace();
            Log.i(TAG, "Erro ao criar o JSON");
            return;
        }


        try {
            this.sensorName = json.getString("sensorName");
            this.sensorAction = json.getString("sensorAction");
        } catch (JSONException e) {
            e.printStackTrace();
        }

    }

    public String parseToJson(){
        JSONObject json = new JSONObject();
        try {
            json.put("sensorName", sensorName);
            json.put("sensorAction", sensorAction);
        } catch (JSONException e) {
            e.printStackTrace();
            Log.i(TAG, "Erro ao criar o JSON");
            return null;
        }

        return json.toString();
    }

    public String getSensorName() {return sensorName;}
    public void setSensorName(String sensorName) {this.sensorName = sensorName;}
    public String getSensorAction() {return sensorAction;}
    public void setSensorAction(String sensorAction) {this.sensorAction = sensorAction;}
}
