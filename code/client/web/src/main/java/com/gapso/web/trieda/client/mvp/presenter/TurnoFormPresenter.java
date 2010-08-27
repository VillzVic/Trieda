package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.services.TurnosServiceAsync;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class TurnoFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		TextField<String> getNomeTextField();
		NumberField getTempoTextField();
		TurnoDTO getTurnoDTO();
		
		SimpleModal getSimpleModal();
	}
	private SimpleGrid<TurnoDTO> gridPanel;
	private Display display;
	
	public TurnoFormPresenter(Display display, SimpleGrid<TurnoDTO> gridPanel) {
		this.gridPanel = gridPanel;
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					final TurnosServiceAsync service = Services.turnos();
					service.save(getDTO(), new AsyncCallback<Boolean>() {
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert("ERRO!", "Deu falha na conexão", null);
						}
						@Override
						public void onSuccess(Boolean result) {
							if(result) {
								display.getSimpleModal().hide();
								gridPanel.updateList();
							} else {
								MessageBox.alert("ERRO!", "Deu falha no server", null);	
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
	
	private TurnoDTO getDTO() {
		TurnoDTO turnoDTO = display.getTurnoDTO();
		turnoDTO.setNome(display.getNomeTextField().getValue());
		turnoDTO.setTempo(display.getTempoTextField().getValue().intValue());
		return turnoDTO;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
