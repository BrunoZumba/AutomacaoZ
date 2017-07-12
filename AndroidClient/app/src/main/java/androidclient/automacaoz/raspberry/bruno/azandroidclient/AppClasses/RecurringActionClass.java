package androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses;

import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Bruno on 18/06/2017.
 *
 * Gerencia uma Ação Recorrente
 */

public class RecurringActionClass {
    private final String TAG = "RECURRING_ACTION_CLASS";

    private String recActName; //nome da ação recorrente
    private List<Long> recActDates; //datas que ela deve executar
    private List<Long> recActTimes; //horários que ela deve executar

    private ActionButtonClass recActActionButton;


    public RecurringActionClass(String name, List<Long> recActDates, List<Long> recActTimes, ActionButtonClass recActActionButton) {
        this.recActName = name;
        this.recActDates = recActDates;
        this.recActTimes = recActTimes;
        this.recActActionButton = recActActionButton;
    }

    public RecurringActionClass(String name, List<Long> recActDates, List<Long> recActTimes, String actBtJsonStr) {
        this.recActName = name;
        this.recActDates = recActDates;
        this.recActTimes = recActTimes;
        this.recActActionButton = new ActionButtonClass(actBtJsonStr);
    }

    public RecurringActionClass(String recActJsonStr) {
        JSONObject json;
        try {
            json = new JSONObject(recActJsonStr);

            this.recActName = json.getString("recActName");

            this.recActDates = new ArrayList<>();
            JSONArray datesJson = json.getJSONArray("recActDates");
            for(int i = 0; i < datesJson.length(); i++){
                this.recActDates.add(Long.valueOf(datesJson.get(i).toString()));
            }

            this.recActTimes = new ArrayList<>();
            JSONArray timesJson = json.getJSONArray("recActTimes");
            for(int i = 0; i < timesJson.length(); i++){
                this.recActTimes.add(Long.valueOf(timesJson.get(i).toString()));
            }

            this.recActActionButton = new ActionButtonClass(json.getString("recActActionButton"));
        } catch (JSONException e) {
            e.printStackTrace();
            Log.i(TAG, "Erro ao criar o JSON");
        }
    }


    public String parseToJson(){
        JSONArray jsonTimes = new JSONArray();
        JSONArray jsonDates = new JSONArray();
        JSONObject json = new JSONObject();

        for (int i = 0; i < recActDates.size(); i ++){
            jsonDates.put(recActDates.get(i).toString());
        }
        for (int i = 0; i < recActTimes.size(); i ++){
            jsonTimes.put(recActTimes.get(i).toString());
        }

        try {
            json.put("recActName", recActName);
            json.put("recActDates", jsonDates);
            json.put("recActTimes", jsonTimes);
            json.put("recActActionButton", new JSONObject(recActActionButton.parseToJson()));
        } catch (JSONException e) {
            e.printStackTrace();
            Log.i(TAG, "Erro ao criar o JSON");
            return null;
        }

        Log.i(TAG, "ParseJson todo: "+json.toString());
        return json.toString();
    }


    public String getRecActName() {return recActName; }
    public void setRecActName(String recActName) {this.recActName = recActName; }
    public List<Long> getRecActDates() {return recActDates; }
    public void setRecActDates(List<Long> recActDates) {this.recActDates = recActDates; }
    public List<Long> getRecActTimes() {return recActTimes;}
    public void setRecActTimes(List<Long> recActTimes) {this.recActTimes = recActTimes; }
    public ActionButtonClass getRecActActionButton() {return recActActionButton; }
    public void setRecActActionButton(ActionButtonClass recActActionButton) {this.recActActionButton = recActActionButton; }
}
