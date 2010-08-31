package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TimeField;
import com.gapso.web.trieda.client.mvp.model.PeriodoDeAulaDTO;
import com.gapso.web.trieda.client.services.PeriodosDeAulaServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.CalendarioComboBox;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.TurnoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class PeriodoDeAulaFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		CalendarioComboBox getCalendarioComboBox();
		TurnoComboBox getTurnoComboBox();
		TimeField getHorarioInicioTextField();
		PeriodoDeAulaDTO getPeriodoDeAulaDTO();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}
	private SimpleGrid<PeriodoDeAulaDTO> gridPanel;
	private Display display;
	
	public PeriodoDeAulaFormPresenter(Display display, SimpleGrid<PeriodoDeAulaDTO> gridPanel) {
		this.gridPanel = gridPanel;
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					final PeriodosDeAulaServiceAsync service = Services.periodosAula();
					service.save(getDTO(), new AsyncCallback<Void>() {
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert("ERRO!", "Deu falha na conexão", null);
						}
						@Override
						public void onSuccess(Void result) {
							display.getSimpleModal().hide();
							gridPanel.updateList();
							Info.display("Salvo", "Item salvo com sucesso!");
						}
					});
				} else {
					MessageBox.alert("ERRO!", "Verifique os campos digitados", null);
				}
			}
		});
	}
	
	private boolean isValid() {
		return display.isValid();
	}
	
	private PeriodoDeAulaDTO getDTO() {
		PeriodoDeAulaDTO dto = display.getPeriodoDeAulaDTO();
		dto.setCalendario(display.getCalendarioComboBox().getSelection().get(0).getId());
		dto.setTurno(display.getTurnoComboBox().getSelection().get(0).getId());
		dto.setInicio(display.getHorarioInicioTextField().getValue().getDate());
		return dto;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
