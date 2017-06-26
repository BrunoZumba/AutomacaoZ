package androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses;

import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by Bruno on 18/06/2017.
 */

public class ActionButtonClass {
    private final String TAG = "ACTION_BUTTON_CLASS";

    String actionName;
    ActionClass action;

    public ActionButtonClass(String actionName, ActionClass action ) {
        this.actionName = actionName;
        this.action = action;
    }

    public ActionButtonClass(String actionName, String actionJsonStr){
        this.actionName = actionName;
        this.action = new ActionClass(actionJsonStr);
    }

    public ActionButtonClass(String jsonOjb){
        JSONObject json;
        try {
            json = new JSONObject(jsonOjb);

            this.actionName = json.getString("actionName");
            this.action = new ActionClass(json.getString("action"));
        } catch (JSONException e) {
            e.printStackTrace();
            Log.i(TAG, "Erro ao criar o JSON");
        }
    }

    public String getActionName() {return actionName;}
    public void setActionName(String actionName) {this.actionName = actionName;}
    public ActionClass getAction() {return action;}
    public void setAction(ActionClass action) {this.action = action;}

    public String parseToJson(){
        JSONObject json = new JSONObject();


        try {
            json.put("actionName", actionName);
            json.put("action", new JSONArray(action.parseToJson()));
        } catch (JSONException e) {
            e.printStackTrace();
            Log.i(TAG, "Erro ao criar o JSON");
            return null;
        }

        return json.toString();
    }
}
