package androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses;

import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Bruno on 18/06/2017.
 */

public class ActionClass {
    private final String TAG = "ACTION_CLASS";
    List<TaskButtonClass> actions;

    public ActionClass(List<TaskButtonClass> actions) {
        this.actions = actions;
    }
    public ActionClass(String jsonArray){
        JSONArray array = null;
        try {
            array = new JSONArray(jsonArray);
        } catch (JSONException e) {
            e.printStackTrace();
            Log.i(TAG, "Erro ao criar o JSON");
            return;
        }

        actions = new ArrayList<>();
        try {
            for (int i = 0; i < array.length(); i++) {
                TaskButtonClass taskButton = new TaskButtonClass(array.get(i).toString());
                actions.add(taskButton);
            }
        } catch (JSONException e) {
            e.printStackTrace();
            Log.i(TAG, "Erro ao criar o JSON");
            return;
        }


    }

    public List<TaskButtonClass> getActions() {return actions;}
    public void setActions(List<TaskButtonClass> actions) {this.actions = actions;}

    public String parseToJson(){
        JSONArray jsonArray = new JSONArray();

        for (int i = 0; i < actions.size(); i++){
            try {
                jsonArray.put(new JSONObject(actions.get(i).parseToJson()));
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }

        return jsonArray.toString();
    }
}
