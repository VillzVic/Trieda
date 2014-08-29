package com.gapso.web.trieda.shared.util.view;

import java.util.ArrayList;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.MessageBoxEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;

public class ConfirmationButton extends Button {

	private ArrayList<SelectionListener<ButtonEvent>> listeners = new ArrayList<SelectionListener<ButtonEvent>>();
	private ITriedaI18nGateway i18nGateway;
	private String confirmacao;
	
	public ConfirmationButton(ITriedaI18nGateway i18nGateway) {
		addConfirmationListener();
		this.i18nGateway = i18nGateway;
		this.confirmacao = i18nGateway.getI18nConstants().confirmacao();
	}
	
	public ConfirmationButton(ITriedaI18nGateway i18nGateway, String confirmacao) {
		addConfirmationListener();
		this.i18nGateway = i18nGateway;
		this.confirmacao = confirmacao;
	}
	
	public void addConfirmationListener() {
		super.addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(final ButtonEvent ce) {
				MessageBox.confirm(i18nGateway.getI18nMessages().confirmacaoButton(), confirmacao, new Listener<MessageBoxEvent>() {
					@Override
					public void handleEvent(MessageBoxEvent be) {
						if(be.getButtonClicked().getHtml().equalsIgnoreCase("yes") ||
								be.getButtonClicked().getHtml().equalsIgnoreCase("sim")) {
							for(SelectionListener<ButtonEvent> listener : listeners) {
								listener.componentSelected(ce);
							}
						}
					}
				});
			}
		});
	}

	@Override
	public void addSelectionListener(SelectionListener<ButtonEvent> listener) {
		listeners.add(listener);
	}

	@Override
	public void removeSelectionListener(SelectionListener<ButtonEvent> listener) {
		listeners.remove(listener);
	}
	
}
