package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.gapso.web.trieda.client.mvp.presenter.OtimizarMessagesPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class OtimizarMessagesView extends MyComposite implements OtimizarMessagesPresenter.Display {

	private ContentPanel panel;
	private SimpleModal simpleModal;
	private ContentPanel messagesWarningPanel;
	private ContentPanel messagesErrorPanel;
	
	public OtimizarMessagesView() {
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = "Alertas e Erros";
		simpleModal = new SimpleModal(null, "Fechar", title, Resources.DEFAULTS.serverWarning16());
		simpleModal.setAutoHeight(true);
		panel = new ContentPanel(new RowLayout());
		panel.setHeaderVisible(false);
		
		messagesWarningPanel = new ContentPanel();
		messagesWarningPanel.setHeading("Alertas");
		messagesWarningPanel.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.warning16()));
		messagesWarningPanel.setBodyBorder(false);
		messagesWarningPanel.addStyleName("errorList");
		panel.add(messagesWarningPanel);
		
		messagesErrorPanel = new ContentPanel();
		messagesErrorPanel.setHeading("Erros");
		messagesErrorPanel.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.error16()));
		messagesErrorPanel.setBodyBorder(false);
		messagesErrorPanel.addStyleName("errorList");
		panel.add(messagesErrorPanel);
		
		panel.setAutoHeight(true);
		
		simpleModal.setContent(panel);
		simpleModal.setWidth(500);
	}

	@Override
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}

	@Override
	public ContentPanel getMessagesWarningPanel() {
		return messagesWarningPanel;
	}

	@Override
	public ContentPanel getMessagesErrorPanel() {
		return messagesErrorPanel;
	}


}
