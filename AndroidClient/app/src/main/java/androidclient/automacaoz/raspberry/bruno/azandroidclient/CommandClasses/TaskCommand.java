package androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses;

import android.os.AsyncTask;
import android.util.Log;
import android.widget.TextView;

import org.json.JSONException;
import org.json.JSONObject;

import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.TaskClass;

/**
 * Created by Bruno on 24/06/2017.
 */

public class TaskCommand extends Command {
    private final String TAG = "TASK_COMMAND";
    TaskClass task;

    public TaskCommand(String requestAction, boolean requestOverwrite, String serverIP, int serverPort, TextView tvResponse, TaskClass task) {
        super(requestAction, requestOverwrite, serverIP, serverPort, tvResponse);
        this.task = task;
    }

    public boolean sendData(){
        super.parseRequestToJson();
        try {
            requestJson.put("task", new JSONObject(task.parseToJson()));
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
            fatherClass.getTvResponse().setText(fatherClass.getResponseDesc());
        }
    }

    public TaskClass getTask() {
        return task;
    }
    public void setTask(TaskClass task) {
        this.task = task;
    }
}
