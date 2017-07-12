package androidclient.automacaoz.raspberry.bruno.azandroidclient;

import android.app.Activity;
import android.content.Intent;
import android.nfc.NfcAdapter;
import android.os.Bundle;

import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.ActionButtonClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses.ActionButtonCommand;

public class NfcReadActivity extends Activity {
    private static final String TAG = "NFC_READ_ACTIVITY";

    public static Activity activity;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        setContentView(R.layout.activity_nfc_read);

        activity = this;

    }

    @Override
    protected void onResume(){
        super.onResume();

        NfcAdapter.getDefaultAdapter(getApplicationContext()).disableForegroundDispatch(NfcReadActivity.activity);
        Intent intent = getIntent();
        NfcReadSingleton.getInstance().readNFC(this, intent);
        this.finish();

        //Verifica se o INTENT recebido e um ACTION_NDEF_DISCOVERED
//        if (NfcAdapter.ACTION_NDEF_DISCOVERED.equals(intent.getAction())) {
//            //Pega todas as NdefMessages do INTENT.
//            // Só terá 1 NDEF = Nome da Acao
//            NdefMessage[] msgs = Util.getInstance().getAllNdefMessages(intent);
//
//            for (int i = 0; i < msgs.length; i++){
//                for (int j = 0; j < msgs[i].getRecords().length; j++){
//
//                    //Baseado no nome da acao, recuperado no NFC, busca a acao no SharedPref
//                    String actionName =  parseNFCPayload(msgs[i].getRecords()[j].getPayload());
//
//                    //Para evitar que a mesma etiqueta fique disparando várias ações enquanto o usuário está com ela perto,
//                    //Verifica se esta ação já não está sendo executada:
//                    Log.i(TAG, "Active Actions size antes: "+NfcReadActivity.activeActions.size());
//                    for (int a = 0; a < NfcReadActivity.activeActions.size(); a++){
//                        Log.i(TAG, "A: "+a);
//                        if (NfcReadActivity.activeActions.get(a).equals(actionName)){
//                            Log.i(TAG, "Ação '"+actionName+"' já está em execução");
//                            return;
//                        }
//                    }
//                    NfcReadActivity.activeActions.add(actionName);
//                    Log.i(TAG, "Adicionou '"+actionName+"' no array");
//                    Log.i(TAG, "Active Actions size depois: "+NfcReadActivity.activeActions.size()+"\n\n");
//
//
//                    //Pega as ações salvas no servidor e cria a lista com botões das ações
//                    ActionButtonClass actBtClass = new ActionButtonClass(actionName, new ActionClass("[]"));
//                    ActionButtonCommand actionButtonCommand = new ActionButtonCommand("getAction", false, Util.SERVER_IP, Util.ACTION_BUTTON_PORT, null, actBtClass);
//                    actionButtonCommand.sendData(this);
//
//                }
//            }
//        }
//        this.finish();
    }

    /**
     * Processa o retorno da solicitação do 'getAction' enviada pelo metodo "onResume", acima.
     * Se o metodo 'getAction' retornar uma ação válida, o método abaixo envia essa ação para o servidor.
     *
     * Se o metodo 'getAction' não retornar uma ação válida, a resposta é tratada diretamente pelo 'actionButtonCommand.sendData(this);'
     * @param actionButton
     */
    public static void processReturn(ActionButtonClass actionButton){
        ActionButtonCommand actionButtonCommand = new ActionButtonCommand("executeActionButton", false, Util.SERVER_IP, Util.ACTION_BUTTON_PORT, null, actionButton);
//        Log.i("NFC_READ_ACTIVITY", "Chamando a ação. Lista: "+NfcReadActivity.activeActions.toString() );
        actionButtonCommand.sendData(NfcReadActivity.activity);
    }

    public static String parseNFCPayload(byte[] payload){
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
