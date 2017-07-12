package androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses;

import android.app.Activity;
import android.support.v7.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.AsyncTask;
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

import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.RecurringActionClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.MainActivity;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.ManageRecurringActionActivity;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.RecurringActionActivity;

/**
 * Created by Bruno on 25/06/2017.
 */

public class RecurringActionCommand extends Command {
    private final String TAG = "RECURRING_ACTION_CMD";

    RecurringActionClass recurringAction;

    AlertDialog alerta;
    public RecurringActionCommand(String requestAction, boolean requestOverwrite, String serverIP, int serverPort, TextView tvResponse, RecurringActionClass recurringAction) {
        super(requestAction, requestOverwrite, serverIP, serverPort, tvResponse);
        this.recurringAction = recurringAction;
    }

    /**
     * Envia a mensagem para o servidor
     * @return True se sucesso; False caso contrario
     */
    public boolean sendData(Activity act){
        super.parseRequestToJson();

        if (recurringAction != null) {
            try {
                requestJson.put("recurringAction", new JSONObject(recurringAction.parseToJson()));
            } catch (JSONException e) {
                e.printStackTrace();
                Log.i(TAG, "Erro ao criar o JSON");
                return false;
            }
        }


        if (getTvResponse()!= null) getTvResponse().setText("");
        new RecActCmdAsyncSendData().execute(act);

        return true;
    }

    /**
     * AsyncTask usada para enviar a mensagem e não travar a UI MainThread do aplicativo
     */
    private class RecActCmdAsyncSendData extends AsyncTask<Activity, Void, Activity> {
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
                            android.support.v7.app.AlertDialog.Builder builder = new android.support.v7.app.AlertDialog.Builder(ManageRecurringActionActivity.activity);
                            builder.setTitle("Salvar Ação Recorrente");
                            builder.setMessage("Já existe uma Ação Recorrente '" + recurringAction.getRecActName() + "'. Substituir pela nova?");
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
                                    sendData(ManageRecurringActionActivity.activity);

                                    alerta.dismiss();
                                }
                            });
                            alerta = builder.create();
                            alerta.show();
                        } else {
                            Toast.makeText(ManageRecurringActionActivity.activity.getApplicationContext(), fatherClass.getResponseDesc(), Toast.LENGTH_SHORT).show();
                        }
                    }
                    break;
                case (STATUS_OK):
                    if(getRequestAction().equals("getList")){
                        RecurringActionActivity.inflateRecurringActionLayout(fatherClass.getResponseParm());
                    } else {
                        Toast.makeText(ManageRecurringActionActivity.activity.getApplicationContext(), fatherClass.getResponseDesc(), Toast.LENGTH_SHORT).show();
                        ManageRecurringActionActivity.activity.finish();
                    }
                    break;
            }
        }

        /**
         * Editar as ações de um botão de ação
         */

//        private class EditActionClickListener implements View.OnClickListener{
//            @Override
//            public void onClick(View v){
//                Intent intent = new Intent(MainActivity.activity, ManageActionActivity.class);
//                intent.putExtra("Tag", v.getTag().toString());
//                MainActivity.activity.startActivity(intent);
//            }
//        }
//
//        /**
//         * Executa a ação que o botão possui
//         */
//        private class ExecuteActionClickListener implements View.OnClickListener{
//            @Override
//            public void onClick(View v){
//                Button bt = (Button) v;
//                JSONArray action = null;
//                try {
//                    action = new JSONArray(v.getTag().toString());
//                } catch (Exception e){
//                    e.printStackTrace();
//                }
//
//                ActionButtonClass actBtClass = new ActionButtonClass(bt.getText().toString(), bt.getTag().toString());
//                ActionButtonCommand actionButtonCommand = new ActionButtonCommand("executeActionButton", false, Util.SERVER_IP, Util.ACTION_BUTTON_PORT, null, actBtClass);
//                actionButtonCommand.sendData();
////                Log.i(TAG, "TAG: '"+v.getTag().toString()+"'");
////                Communication comm = new Communication(v.getTag().toString(), Util.SERVER_IP, Util.COMMAND_PORT, null);
////                comm.sendData();
//            }
//        }
    }
}
