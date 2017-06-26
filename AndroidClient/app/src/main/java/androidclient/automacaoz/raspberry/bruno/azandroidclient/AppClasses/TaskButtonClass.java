package androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses;

import android.util.Log;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by Bruno on 18/06/2017.
 */

public class TaskButtonClass {
    private final String TAG = "TASK_BUTTON_CLASS";
    String buttonId;
    TaskClass task;

    public TaskButtonClass(String buttonId, TaskClass task) {
        this.buttonId = buttonId;
        this.task = task;
    }

    public TaskButtonClass(String jsonObj){
        JSONObject json;
        try {
            json = new JSONObject(jsonObj);
            this.buttonId = json.getString("buttonId");
            this.task = new TaskClass(json.getString("task"));
        } catch (JSONException e) {
            e.printStackTrace();
            Log.i(TAG, "Erro ao criar JSON");
            return;
        }
//        Log.i(TAG, "Button ID: "+buttonId+". Task: "+task.parseToJson());
    }

    public String getButtonId() {return buttonId;}
    public void setButtonId(String buttonId) {this.buttonId = buttonId;}
    public TaskClass getTask() {return task;}
    public void setTask(TaskClass task) {this.task = task;}

    public String parseToJson(){
        JSONObject json = new JSONObject();
        try {
            json.put("buttonId", buttonId);
            json.put("task", new JSONObject(task.parseToJson()));
        } catch (JSONException e) {
            e.printStackTrace();
            Log.i(TAG, "Erro ao criar o Json");
            return null;
        }


        return json.toString();
    }
}
