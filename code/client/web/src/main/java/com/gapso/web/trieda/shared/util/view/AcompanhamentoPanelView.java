package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.button.ToolButton;
import com.extjs.gxt.ui.client.widget.form.LabelField;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;

public class AcompanhamentoPanelView extends MyComposite
	implements AcompanhamentoPanelPresenter.Display
{
	private ContentPanel panel;
	private SimpleModal simpleModal;
	private ContentPanel messagesPanel;
	private LabelField label;

	public AcompanhamentoPanelView(){
		initUI();
	}

	private void initUI(){
		String title = "Painel de Acompanhamento do Processamento da Requisição";

		simpleModal = new SimpleModal("OK", null, title, Resources.DEFAULTS.serverWarning16());
		simpleModal.setWidth(450);
		
		panel = new ContentPanel(new RowLayout());
		panel.setHeaderVisible(false);
		panel.setAutoHeight(true);

		label = new LabelField();
		label.setId("msgRepository");
		label.setStyleAttribute("overflow", "auto");
		label.setHeight(400);
		
		panel.add(label);

		simpleModal.setContent(panel);
	}
	
	public static native void scrollLabelDown() /*-{
		$doc.getElementById('msgRepository').scrollTop = $doc.getElementById('msgRepository').scrollHeight;
	}-*/;
	
	@Override
	public void updateMessagePanel(String msg){
		String lblText = (String) label.getValue();
		if(lblText == null) lblText = "";
		label.setValue(lblText + "<div style=\"padding: 3px 10px; font-size: 11px;\">" + msg + "</div>");
		scrollLabelDown();
	}
	
	public void show(){
		this.simpleModal.show();
	}

	@Override
	public SimpleModal getSimpleModal(){
		return this.simpleModal;
	}

	@Override
	public ContentPanel getMessagesPanel(){
		return this.messagesPanel;
	}

	@Override
	public Button getOkButton(){
		return this.simpleModal.getSalvarBt();
	}

	@Override
	public ToolButton getCloseButton(){
		return this.simpleModal.getCloseBt();
	}
}

