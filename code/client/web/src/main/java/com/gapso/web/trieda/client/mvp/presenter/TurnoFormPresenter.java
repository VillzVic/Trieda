package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.mvp.model.TurnoModel;
import com.gapso.web.trieda.client.services.TurnosService;
import com.gapso.web.trieda.client.services.TurnosServiceAsync;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class TurnoFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		TextField<String> getNomeTextField();
		NumberField getTempoTextField();
		SimpleModal getSimpleModal();
	}
	private Display display; 
	
	public TurnoFormPresenter(Display display) {
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					TurnosServiceAsync service = GWT.create(TurnosService.class);
					service.save(getModel(), new AsyncCallback<Boolean>() {
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert("ERRO!", "Deu falha no servidor", null);
						}
						@Override
						public void onSuccess(Boolean result) {
							if(result) {
								display.getSimpleModal().hide();
							} else {
								MessageBox.alert("ERRO!", "Deu falha no servidor", null);	
							}
						}
					});
				} else {
					MessageBox.alert("ERRO!", "Verifique os campos digitados", null);
				}
			}
		});
	}
	
	private boolean isValid() {
		return (!display.getNomeTextField().getValue().isEmpty()) && (display.getTempoTextField().getValue().intValue() > 0);
	}
	
	private TurnoModel getModel() {
		return new TurnoModel(display.getNomeTextField().getValue(), display.getTempoTextField().getValue().intValue());
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
