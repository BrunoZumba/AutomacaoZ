package androidclient.automacaoz.raspberry.bruno.azandroidclient;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.PendingIntent;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.nfc.NdefMessage;
import android.nfc.NdefRecord;
import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.nfc.tech.Ndef;
import android.nfc.tech.NdefFormatable;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.HorizontalScrollView;
import android.widget.LinearLayout;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.ByteArrayOutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.ActionButtonClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.AppClasses.ActionClass;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses.ActionButtonCommand;
import androidclient.automacaoz.raspberry.bruno.azandroidclient.CommandClasses.Command;

public class ManageActionActivity extends AppCompatActivity {
    private final String TAG = "MANAGE_ACTION_ACTIVITY";

    //Caminho onde todos os SharedPrefs serão gravados no dispositivo
    public final static String ACTION_FILE_KEY = "androidclient.automacaoz.raspberry.bruno.azandroidclient.ACTION_FILE_KEY";

    //Feito public static para criação do AlertDialog na classe ActionButtonCommand
    public static Activity activity;

    LinearLayout dropLayout;
    HorizontalScrollView scrollView;

    //Lista para guardar todos os botoes que o usuario selecionou para esta acao
    List<Button> actionsButtons = new ArrayList<Button>();
    EditText etNomeAcoes;

    //Lista para acesso centralizado a todos os botoes disponível ao usuário
    List<Button> buttonList = new ArrayList<>();

    //Variaveis utilizadas para gravar o nome da Acao no NFC
    //String feita global para ser acessível através do novo Intent criado para gravar o NFC
    Tag mytag;
    NdefMessage ndefMessage;
    NfcAdapter nfcAdpt;
    AlertDialog nfcAlert;


    static android.support.v7.app.AlertDialog overwriteAlert;

    //Ao salvar uma acao, este Array guarda o conjunto de tasks desta acao
    //Objeto feito global para ser acessível através da DialogBox
    JSONArray action;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_manage_action);
        dropLayout = (LinearLayout) findViewById(R.id.dropLayout);
        scrollView = (HorizontalScrollView) findViewById(R.id.scrollView);
        etNomeAcoes = (EditText) findViewById(R.id.etNomeAcoes);
        activity = this;
        nfcAdpt = NfcAdapter.getDefaultAdapter(this);

        //Cria o listener para ajustar o scroll quando o usuário colocar/retirar um botão
        dropLayout.addOnLayoutChangeListener(
                new View.OnLayoutChangeListener() {
                    @Override
                    public void onLayoutChange(View v, int left, int top, int right, int bottom, int oldLeft, int oldTop, int oldRight, int oldBottom) {
                        //Se o usuario adicinou botao, vai para final do scroll,
                        //caso contrario, permanece com scroll imovel
                        if (right > oldRight) {
                            scrollView.fullScroll(HorizontalScrollView.FOCUS_RIGHT);
                        }
                    }
                }
        );

        //Preenche a Lista centralizada com todos os botoes disponiveis
        buttonList = new ArrayList<>();
        for (int i = 0; i < DeviceDisplayActivity.btListName.length; i++) {
            buttonList.add((Button) findViewById(getApplicationContext().getResources().
                    getIdentifier(DeviceDisplayActivity.btListName[i], "id", getApplicationContext().getPackageName())));

            buttonList.get(buttonList.size()-1).setOnClickListener(new clickListenerAddAction());
        }

        /**
         * Verificar se o Intent que trouxe o usuario a esta tela é para editar
         * algum ActionButton especifico. Em caso positivo, a "Tag" do Intent possui a TAG
         * completa do ActionButton: {"actionName":"NOME_ACAO", "action":"[{TASK},{TASK},{TASK}]}
         */
        Intent intent = getIntent();
        String tag = intent.getStringExtra("Tag");

        /**Se houver um intent com EXTRA = ACTION, é para editar a ação **/
        if (tag != null){
            inflateAction(tag);
        }
    }

    /**
     * Grava a acao em questao em uma etiqueta NFC
     */
    public void gravarNFC(View v){

        if(!validarDados()) {
            return;
        }

        if (nfcAdpt == null){
            Toast.makeText(getApplicationContext(), "O dispositivo não possui a tecnologia NFC", Toast.LENGTH_SHORT).show();
            return;
        }
        if (!nfcAdpt.isEnabled()){
            Toast.makeText(getApplicationContext(), "Habilite a tecnologia NFC nas cofingurações do aparelho", Toast.LENGTH_SHORT).show();
            return;
        }

        String content = etNomeAcoes.getText().toString();
        ndefMessage = null;

        //Cria a NdefMessage com o Record contendo o nome da acao
        try {
            // Get UTF-8 byte
            byte[] lang = Locale.getDefault().getLanguage().getBytes("UTF-8");
            byte[] text = content.getBytes("UTF-8"); // Content in UTF-8

            int langSize = lang.length;
            int textLength = text.length;

            ByteArrayOutputStream payload = new ByteArrayOutputStream(1 + langSize + textLength);
            payload.write((byte) (langSize & 0x1F));
            payload.write(lang, 0, langSize);
            payload.write(text, 0, textLength);
            NdefRecord record = new NdefRecord(NdefRecord.TNF_WELL_KNOWN,
                    NdefRecord.RTD_TEXT, new byte[0],
                    payload.toByteArray());
            ndefMessage = new NdefMessage(new NdefRecord[]{record});
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        //Cria o intent para gravar a etiqueta NFC. Este intent chama o metodo onNewIntent
        try {
            PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, new Intent(this, getClass()).addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP), 0);
            IntentFilter[] intentFiltersArray = new IntentFilter[] {new IntentFilter(NfcAdapter.ACTION_NDEF_DISCOVERED)};
            String[][] techList = new String[][] {
                    { android.nfc.tech.Ndef.class.getName() },
                    { android.nfc.tech.NdefFormatable.class.getName() }
            };
            nfcAdpt.enableForegroundDispatch(this, pendingIntent, intentFiltersArray, techList);
        }
        catch(Exception e) {
            e.printStackTrace();
        }
//        MainActivity.activity = this;


        //Cria o gerador do AlertDialog
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Gravar etiqueta NFC");
        builder.setMessage("Aproxime uma etiqueta NFC para gravar esta ação");
        builder.setNegativeButton("Cancelar", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface arg0, int arg1) {
                nfcAdpt.disableForegroundDispatch(activity);
            }
        });
        //cria o AlertDialog
        nfcAlert = builder.create();
        //Exibe
        nfcAlert.show();


    }

    /**
     * Disparado quando a aplicacao esta pronta para gravar uma etiqueta e o usuario
     * aproxima a etiqueta do aparelho
     * @param intent
     */
    @Override
    protected void onNewIntent(Intent intent){
        if((NfcAdapter.ACTION_TAG_DISCOVERED.equals(intent.getAction())) || (NfcAdapter.ACTION_TECH_DISCOVERED.equals(intent.getAction()))){
            mytag = intent.getParcelableExtra(NfcAdapter.EXTRA_TAG);

            //Grava a NdefMessage na etiqueta NFC
            if (mytag != null) {
                try {
                    Ndef ndefTag = Ndef.get(mytag);
                    if (ndefTag == null)  {
                        // Let's try to format the Tag in NDEF
                        NdefFormatable nForm = NdefFormatable.get(mytag);
                        if (nForm != null) {
                            nForm.connect();
                            nForm.format(ndefMessage);
                            nForm.close();
                        }
                    } else {
                        ndefTag.connect();
                        ndefTag.writeNdefMessage(ndefMessage);
                        ndefTag.close();
                        Toast.makeText(getApplicationContext(), "Etiqueta gravada com sucesso", Toast.LENGTH_SHORT).show();
                        nfcAlert.dismiss();
                    }
                    nfcAdpt.disableForegroundDispatch(activity);
                }
                catch(Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     * Cria a tela de Editar o ActionButton com todos os campos preenchidos
     */
    private void inflateAction(String tag){
        JSONObject actionButton = null;
        JSONArray action = null;
        try {
            actionButton = new JSONObject(tag);
            etNomeAcoes.setText(actionButton.getString("actionName"));
            action = new JSONArray(actionButton.getString("action"));
        } catch (Exception e) {
            e.printStackTrace();
            return;
        }

        //Para cada BOTAO {ID+TASK}...
        for (int i = 0; i < action.length(); i++){

            //...pega o ID, busca o botao deste ID na Lista centralizada e...
            int btId = 0;
            try {
                JSONObject json = new JSONObject(action.get(i).toString());
                btId = json.getInt("buttonId");
            } catch (JSONException e) {
                e.printStackTrace();
                Log.e(TAG, "Erro ao criar o obj json");
            }

            //..executa a acao de click neste botao (que o adiciona ao array de botoes da acao)
            for (int j = 0; j < buttonList.size(); j++){
                if (buttonList.get(j).getId() == btId){
                    buttonList.get(j).performClick();
                }
            }
        }
    }


    /**
     * Botao definido no arquivo de layout para deletar uma ação do SharedPref
     * @param v
     */
    public void deletarAcao(View v){
        //Verifica se o nome da acao esta vazio
        if (etNomeAcoes.getText().toString() == ""){
            Toast.makeText(getApplicationContext(), "Digite o nome da Ação que deseja excluir", Toast.LENGTH_SHORT).show();
            return;
        }

        //Para deletar a ação não é necessário passar toda o JSONString da ação, basta o nome. Portanto a ActionClass é criada como um array vazio "[]"
        ActionButtonClass actionButtonClass = new ActionButtonClass(etNomeAcoes.getText().toString(), new ActionClass("[]"));
        ActionButtonCommand actionButtonCommand = new ActionButtonCommand("deleteActionButton", false, Util.SERVER_IP, Util.ACTION_BUTTON_PORT, null, actionButtonClass);

        actionButtonCommand.sendData(this);

    }

    /**
     * Verificar se os dados estao validos para salvar a acao no SharedPref e gravar o NFC
     *      Nome da acao nao esta vazio
     *      Ha botoes de acao
     */
    private boolean validarDados(){
        //Verifica se a string com o nome da acao esta vazio
        if ((etNomeAcoes.getText() == null) || (etNomeAcoes.getText().toString().length() == 0)){
            Toast.makeText(getApplicationContext(), "Insira um nome para o conjunto de ações",Toast.LENGTH_SHORT).show();
            return false;
        }

        if ((etNomeAcoes.getText().toString().contains("%")) || (etNomeAcoes.getText().toString().contains("&"))) {
            Toast.makeText(getApplicationContext(), "O nomne não deve possuir os caracteres % ou &",Toast.LENGTH_SHORT).show();
            return false;
        }

        //Verifica se a acao possui algum botao
        if(actionsButtons.size() == 0){
            Toast.makeText(getApplicationContext(), "Insira ao menos uma ação",Toast.LENGTH_SHORT).show();
            return false;
        }

        return true;

    }


    /**
     * Botao definidio no arquivo de layout para salvar uma acao no SharedPref
     * @param v
     */
    public void salvarAcao(View v){
        //Valida os dados do nome da acao e se ha botoes de acao
        if(!validarDados()) return;

        action = new JSONArray();
        //acoes = "";
        for (int i = 0; i < actionsButtons.size(); i++){
            Button bt = (Button) actionsButtons.get(i);
            JSONObject jsonObj = new JSONObject();
            try {
                jsonObj.put("buttonId", bt.getId());
                jsonObj.put("task", new JSONObject(bt.getTag().toString()));
            } catch (JSONException e) {
                e.printStackTrace();
                Log.e(TAG, "Erro ao criar o obj jason");
                return;
            }

            action.put(jsonObj);
        }

        ActionButtonClass actionButtonClass = new ActionButtonClass(etNomeAcoes.getText().toString(), new ActionClass(action.toString()));
        ActionButtonCommand actionButtonCommand = new ActionButtonCommand("saveActionButton", false, Util.SERVER_IP, Util.ACTION_BUTTON_PORT, null, actionButtonClass);

        actionButtonCommand.sendData(this);
    }

    public static void processResponse(final ActionButtonCommand response){
        if (response.getRequestAction().equals("saveActionButton")){
            switch (response.getResponseStatus()) {
                case (Command.STATUS_ERROR):
                    if (response.getResponseParm().equals("requestOverwrite")) {
                        android.support.v7.app.AlertDialog.Builder builder = new android.support.v7.app.AlertDialog.Builder(ManageActionActivity.activity);
                        builder.setTitle("Salvar Ação");
                        builder.setMessage("Já existe uma Ação '" + response.getActionButton().getActionName() + "'. Substituir pela nova?");
                        builder.setNegativeButton("Cancelar", new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface arg0, int arg1) {
                                overwriteAlert.dismiss();
                                return;
                            }
                        });
                        builder.setPositiveButton("Salvar", new DialogInterface.OnClickListener() {
                            //Altera para overwrite = true e envia a ação de novo
                            public void onClick(DialogInterface arg0, int arg1) {
                                response.setRequestOverwrite(true);
                                response.sendData(ManageActionActivity.activity);

                                overwriteAlert.dismiss();
                            }
                        });
                        overwriteAlert = builder.create();
                        overwriteAlert.show();
                    } else {
                        Toast.makeText(ManageActionActivity.activity.getApplicationContext(), response.getResponseDesc(), Toast.LENGTH_SHORT).show();
                    }
                    break;

                case (Command.STATUS_OK):
                    Toast.makeText(ManageActionActivity.activity.getApplicationContext(), response.getResponseDesc(), Toast.LENGTH_SHORT).show();
                    ManageActionActivity.activity.finish();
                    break;
            }
        } else if (response.getRequestAction().equals("deleteActionButton")){
            Toast.makeText(ManageActionActivity.activity.getApplicationContext(), response.getResponseDesc(), Toast.LENGTH_SHORT).show();
            ManageActionActivity.activity.finish();
        }
    }

    /**
     * Chamada quando o usuário clica em botao para adiciona-lo a acao
     */
    private class clickListenerAddAction implements View.OnClickListener{
        public void onClick(View v){
            Button oldButton = (Button) v;
            Button newButton = new Button(oldButton.getContext());
            newButton.setText(oldButton.getText());
            newButton.setTag(oldButton.getTag());
            newButton.setId(oldButton.getId());

            actionsButtons.add(newButton);
            dropLayout.addView(newButton);
            newButton.setOnClickListener(new clickListenerRemoveAction());
        }
    }

    /**
     * Chamada quando o usuário clica nos botões da acao, para remove-lo
     */
    private class clickListenerRemoveAction implements View.OnClickListener{
        public void onClick(View v){
            dropLayout.removeView(v);
            actionsButtons.remove((Button) v);
        }
    }




}
