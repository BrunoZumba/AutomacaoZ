package androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses;

import android.app.Activity;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.AsyncTask;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.ActionButtonClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.MainActivity;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.ManageActionActivity;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.ManageRecurringActionActivity;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.Util;

/**
 * Created by Bruno on 24/06/2017.
 */

public class ActionButtonCommand extends Command{
    private final String TAG = "ACTION_BUTTON_CMD";
    ActionButtonClass actionButton;


    AlertDialog alerta;

    public ActionButtonCommand(String requestAction, boolean requestOverwrite, String serverIP, int serverPort, TextView tvResponse, ActionButtonClass actionButton) {
        super(requestAction, requestOverwrite, serverIP, serverPort, tvResponse);
        this.actionButton = actionButton;

    }


    /**
     * Envia a mensagem para o servidor
     * @return True se sucesso; False caso contrario
     */
    public boolean sendData(Activity act){
        super.parseRequestToJson();

        if(actionButton != null) {
            try {
                requestJson.put("actionButton", new JSONObject(actionButton.parseToJson()));
            } catch (JSONException e) {
                e.printStackTrace();
                Log.i(TAG, "Erro ao criar o JSON");
                return false;
            }
        }

        if (getTvResponse()!= null) getTvResponse().setText("");
        new ActBtCmdAsyncSendData().execute(act);

        return true;
    }

    /**
     * AsyncTask usada para enviar a mensagem e não travar a UI MainThread do aplicativo
     */
    private class ActBtCmdAsyncSendData extends AsyncTask<Activity, Void, Activity> {
        @Override
        protected Activity doInBackground(Activity... b){
            fatherClass.sendAndReceiveData();

            return b[0];
        }

        @Override
        protected void onPostExecute(Activity c){
            //Analisa a resposta do servidor, analisando se já existe a ação e perguntando se o usuário quer overwrite
            if (fatherClass.getResponseAction() == null){
                //responseAction ser null significa que não houve conexão com o servidor
                TextView tv = new TextView(MainActivity.activity);
                tv.setText("Erro!\nVerifique a conexão com o servidor");
                MainActivity.savedActionsLayout.addView(tv);
                return;
            }

            switch (fatherClass.getResponseStatus()) {
                case (STATUS_ERROR):
                    if (getRequestAction().equals("executeActionButton")){
                        Toast.makeText(MainActivity.activity.getApplicationContext(), fatherClass.getResponseDesc(), Toast.LENGTH_SHORT).show();
                    } else {
                        if (fatherClass.getResponseParm().equals("requestOverwrite")) {
                            AlertDialog.Builder builder = new AlertDialog.Builder(ManageActionActivity.activity);
                            builder.setTitle("Salvar Ação");
                            builder.setMessage("Já existe uma Ação '" + actionButton.getActionName() + "'. Substituir pela nova?");
                            builder.setNegativeButton("Cancelar", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface arg0, int arg1) {
                                    alerta.dismiss();
                                    return;
                                }
                            });
                            builder.setPositiveButton("Salvar", new DialogInterface.OnClickListener() {
                                //Altera para overwrite = true e envia a ação de novo
                                public void onClick(DialogInterface arg0, int arg1) {
                                    fatherClass.setRequestOverwrite(true);
                                    sendData(ManageActionActivity.activity);

                                    alerta.dismiss();
                                }
                            });
                            alerta = builder.create();
                            alerta.show();
                        } else {
                            Toast.makeText(ManageActionActivity.activity.getApplicationContext(), fatherClass.getResponseDesc(), Toast.LENGTH_SHORT).show();
                        }
                    }
                    break;
                case (STATUS_OK):
                    if (getRequestAction().equals("executeActionButton")){
                        Toast.makeText(ManageActionActivity.activity.getApplicationContext(), fatherClass.getResponseDesc(), Toast.LENGTH_SHORT).show();
                    } else if(getRequestAction().equals("getList")){
                        //Quando o RequestAction é 'getList' a própria Atividade que chamou trata o retorno
                        if (c.getComponentName().getShortClassName().equals(".MainActivity")) {
                            MainActivity.inflateActionButtonLayout(fatherClass.getResponseParm());
                        } else if(c.getComponentName().getShortClassName().equals(".ManageRecurringActionActivity")) {
                            Log.i(TAG, "ResponseParm: "+fatherClass.getResponseParm());
                            ManageRecurringActionActivity.inflateActionButtonLayout(fatherClass.getResponseParm());
                        }
                    } else {
                        Toast.makeText(ManageActionActivity.activity.getApplicationContext(), fatherClass.getResponseDesc(), Toast.LENGTH_SHORT).show();
                        ManageActionActivity.activity.finish();
                    }
                    break;
            }
        }
    }

    public ActionButtonClass getActionButton() {return actionButton;}
    public void setActionButton(ActionButtonClass actionButton) {this.actionButton = actionButton;}
}
