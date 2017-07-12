package androidclient.automacaoz.raspberry.bruno.azandroidclient;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.nfc.NdefMessage;
import android.nfc.NfcAdapter;
import android.util.Log;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.ActionButtonClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.ActionClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses.ActionButtonCommand;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses.Command;

/**
 * Created by Bruno on 01/07/2017.
 */

public class NfcReadSingleton {
    private static final String TAG = "NFC_READ_SINGLETON";

    private static NfcReadSingleton instance;


    public static List<String> activeActions = new ArrayList<>();

    public static NfcReadSingleton getInstance(){
        if (instance == null) {
            instance = new NfcReadSingleton();
        }
        return instance;
    }


    public void processResponse(ActionButtonCommand response){
        if(response.getRequestAction().equals("getAction")){
            switch (response.getResponseStatus()) {
                case (Command.STATUS_ERROR):
                    activeActions.remove(response.getActionButton().getActionName());
                    Log.i("NFC_READ_ACTIVITY", "Return do getaction com erro. Retirou '"+response.getActionButton().getActionName());
//                    Toast.makeText(c.getApplicationContext(), response.getResponseDesc(), Toast.LENGTH_SHORT).show();
                    break;

                case (Command.STATUS_OK):
                    processReturn(new ActionButtonClass(response.getResponseParm()));
                    break;
            }
        } else if (response.getRequestAction().equals("executeActionButton")){
            switch (response.getResponseStatus()) {
                case (Command.STATUS_ERROR):
                    Toast.makeText(NfcReadActivity.activity.getApplicationContext(), response.getResponseDesc(), Toast.LENGTH_SHORT).show();
                    break;

                case (Command.STATUS_OK):
                    Toast.makeText(NfcReadActivity.activity.getApplicationContext(), response.getResponseDesc(), Toast.LENGTH_SHORT).show();
                        NfcReadSingleton.activeActions.remove(response.getActionButton().getActionName());
                        Log.i("NFC_READ_ACTIVITY", "Return do executeAction com sucesso. Retirou '"+response.getActionButton().getActionName());
                    break;
            }
        }
    }
    private void processReturn(ActionButtonClass actionButton){
        ActionButtonCommand actionButtonCommand = new ActionButtonCommand("executeActionButton", false, Util.SERVER_IP, Util.ACTION_BUTTON_PORT, null, actionButton);
//        Log.i("NFC_READ_ACTIVITY", "Chamando a ação. Lista: "+NfcReadActivity.activeActions.toString() );
        actionButtonCommand.sendData(NfcReadActivity.activity);

    }

    void readNFC(Activity activity, Intent intent){

        //Verifica se o INTENT recebido e um ACTION_NDEF_DISCOVERED
        if (NfcAdapter.ACTION_NDEF_DISCOVERED.equals(intent.getAction())) {
            //Pega todas as NdefMessages do INTENT.
            // Só terá 1 NDEF = Nome da Acao
            NdefMessage[] msgs = Util.getInstance().getAllNdefMessages(intent);

            for (int i = 0; i < msgs.length; i++){
                for (int j = 0; j < msgs[i].getRecords().length; j++){

                    //Baseado no nome da acao, recuperado no NFC, busca a acao no SharedPref
                    String actionName =  parseNFCPayload(msgs[i].getRecords()[j].getPayload());

                    //Para evitar que a mesma etiqueta fique disparando várias ações enquanto o usuário está com ela perto,
                    //Verifica se esta ação já não está sendo executada:
                    Log.i(TAG, "Active Actions size antes: "+NfcReadSingleton.activeActions.size());
                    for (int a = 0; a < NfcReadSingleton.activeActions.size(); a++){
                        Log.i(TAG, "A: "+a);
                        if (NfcReadSingleton.activeActions.get(a).equals(actionName)){
                            Log.i(TAG, "Ação '"+actionName+"' já está em execução");
                            return;
                        }
                    }
                    NfcReadSingleton.activeActions.add(actionName);
                    Log.i(TAG, "Adicionou '"+actionName+"' no array");
                    Log.i(TAG, "Active Actions size depois: "+NfcReadSingleton.activeActions.size()+"\n\n");


                    //Pega as ações salvas no servidor e cria a lista com botões das ações
                    ActionButtonClass actBtClass = new ActionButtonClass(actionName, new ActionClass("[]"));
                    ActionButtonCommand actionButtonCommand = new ActionButtonCommand("getAction", false, Util.SERVER_IP, Util.ACTION_BUTTON_PORT, null, actBtClass);
                    actionButtonCommand.sendData(activity);

                }
            }
        }
    }

    static String parseNFCPayload(byte[] payload){
        //Tirei essa função loka daqui: https://stackoverflow.com/questions/14607425/read-data-from-nfc-tag
        try {

        /*
         * payload[0] contains the "Status Byte Encodings" field, per the
         * NFC Forum "Text Record Type Definition" section 3.2.1.
         *
         * bit7 is the Text Encoding Field.
         *
         * if (Bit_7 == 0): The text is encoded in UTF-8 if (Bit_7 == 1):
         * The text is encoded in UTF16
         *
         * Bit_6 is reserved for future use and must be set to zero.
         *
         * Bits 5 to 0 are the length of the IANA language code.
         */

            //Get the Text Encoding
            String textEncoding = ((payload[0] & 0200) == 0) ? "UTF-8" : "UTF-16";

            //Get the Language Code
            int languageCodeLength = payload[0] & 0077;
            String languageCode = new String(payload, 1, languageCodeLength, "US-ASCII");

            //Get the Text
            String text = new String(payload, languageCodeLength + 1, payload.length - languageCodeLength - 1, textEncoding);
            return text;
//            return new TextRecord(text, languageCode);
        }
        catch(Exception e)
        {
            throw new RuntimeException("Record Parsing Failure!!");
        }
    }
}
