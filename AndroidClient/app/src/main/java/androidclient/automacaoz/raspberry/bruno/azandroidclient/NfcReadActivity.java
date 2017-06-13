package androidclient.automacaoz.raspberry.bruno.azandroidclient;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.nfc.NdefMessage;
import android.nfc.NfcAdapter;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class NfcReadActivity extends AppCompatActivity {
    private final String TAG = "NFC_READ_ACTIVITY";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_nfc_read);

    }

    @Override
    protected void onResume(){
        super.onResume();

        Intent intent = getIntent();

        //Verifica se o INTENT recebido e um ACTION_NDEF_DISCOVERED
        if (NfcAdapter.ACTION_NDEF_DISCOVERED.equals(getIntent().getAction())) {

            //Pega todas as NdefMessages do INTENT.
            // Só terá 1 NDEF = Nome da Acao
            NdefMessage[] msgs = Util.getInstance().getAllNdefMessages(intent);

            for (int i = 0; i < msgs.length; i++){
                for (int j = 0; j < msgs[i].getRecords().length; j++){

                    //Baseado no nome da acao, recuperado no NFC, busca a acao no SharedPref
                    String actionName =  parseNFCPayload(msgs[i].getRecords()[j].getPayload());
                    SharedPreferences sharedPref = getApplicationContext().getSharedPreferences(ManageActionActivity.ACTION_FILE_KEY, Context.MODE_PRIVATE);
                    String actionString = sharedPref.getString(actionName, "");

                    //Verifica se ha uma acao no SharedPref com esse nome
                    if (actionString == ""){
                        Toast.makeText(getApplicationContext(), "Não há ação cadastrada para este comando ("+actionName+")", Toast.LENGTH_LONG).show();
                        finish();
                        return;
                    }


                    Communication comm = new Communication(actionString, Util.SERVER_IP, Util.COMMAND_PORT, null);
                    if ((actionString == null) || (!comm.sendData())){
                        Toast.makeText(getApplicationContext(), "Erro ao enviar o comando", Toast.LENGTH_SHORT).show();
                        return;
                    }
                    Toast.makeText(getApplicationContext(), "Comando enviado", Toast.LENGTH_SHORT).show();

                }
            }
        }
        this.finish();
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
