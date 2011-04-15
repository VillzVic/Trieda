package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.services.CenariosServiceAsync;
import com.gapso.web.trieda.main.client.services.Services;
import com.gapso.web.trieda.main.client.util.view.SimpleGrid;
import com.gapso.web.trieda.main.client.util.view.SimpleModal;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CenarioCloneFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		CheckBox getOficialCheckBox();
		TextField<String> getNomeTextField();
		NumberField getAnoTextField();
		NumberField getSemestreTextField();
		TextField<String> getComentarioTextField();
		CenarioDTO getCenarioDTO();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}
	private SimpleGrid<CenarioDTO> gridPanel;
	private Display display;
	
	public CenarioCloneFormPresenter(Display display, SimpleGrid<CenarioDTO> gridPanel) {
		this.gridPanel = gridPanel;
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					final CenariosServiceAsync service = Services.cenarios();
					service.clonar(getDTO(), new AsyncCallback<Void>() {
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
	
	private CenarioDTO getDTO() {
		CenarioDTO cenarioDTO = display.getCenarioDTO();
		cenarioDTO.setMasterData(false);
		cenarioDTO.setOficial(display.getOficialCheckBox().getValue());
		cenarioDTO.setNome(display.getNomeTextField().getValue());
		cenarioDTO.setAno(display.getAnoTextField().getValue().intValue());
		cenarioDTO.setSemestre(display.getSemestreTextField().getValue().intValue());
		cenarioDTO.setComentario(display.getComentarioTextField().getValue());
		return cenarioDTO;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
