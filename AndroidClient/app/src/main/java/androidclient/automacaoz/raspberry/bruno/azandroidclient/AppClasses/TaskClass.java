package androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses;

import android.util.Log;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by Bruno on 18/06/2017.
 */

public class TaskClass {
    private final String TAG = "TASK_CLASS";
    String deviceName;
    String buttonName;
    String mode;

    public TaskClass(String deviceName, String buttonName, String mode) {
        this.deviceName = deviceName;
        this.buttonName = buttonName;
        this.mode = mode;
    }

    public TaskClass(String jsonObj){
        JSONObject json;
        try {
            json = new JSONObject(jsonObj);
        } catch (Exception e){
            e.printStackTrace();
            Log.i(TAG, "Erro ao criar o JSON");
            return;
        }


        try {
            this.deviceName = json.getString("deviceName");
            this.buttonName = json.getString("buttonName");
            this.mode = json.getString("mode");
        } catch (JSONException e) {
            e.printStackTrace();
        }

    }

    public String parseToJson(){
        JSONObject json = new JSONObject();
        try {
            json.put("deviceName", deviceName);
            json.put("buttonName", buttonName);
            json.put("mode", mode);
        } catch (JSONException e) {
            e.printStackTrace();
            Log.i(TAG, "Erro ao criar o JSON");
            return null;
        }

        return json.toString();
    }

    public String getDeviceName() {return deviceName;}
    public void setDeviceName(String deviceName) {this.deviceName = deviceName;}
    public String getButtonName() {return buttonName;}
    public void setButtonName(String buttonName) {this.buttonName = buttonName;}
    public String getMode() {return mode;}
    public void setMode(String mode) {this.mode = mode;}
}
