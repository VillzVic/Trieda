package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.HashSet;
import java.util.Set;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.DualListField;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.services.CenariosServiceAsync;
import com.gapso.web.trieda.main.client.services.Services;
import com.gapso.web.trieda.main.client.util.view.SemanaLetivaComboBox;
import com.gapso.web.trieda.main.client.util.view.SimpleGrid;
import com.gapso.web.trieda.main.client.util.view.SimpleModal;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CenarioCriarFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		CheckBox getOficialCheckBox();
		TextField<String> getNomeTextField();
		NumberField getAnoTextField();
		NumberField getSemestreTextField();
		TextField<String> getComentarioTextField();
		SemanaLetivaComboBox getSemanaLetivaComboBox();
		CenarioDTO getCenarioDTO();
		DualListField<CampusDTO> getCampiDualList();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}
	private SimpleGrid<CenarioDTO> gridPanel;
	private Display display;
	
	public CenarioCriarFormPresenter(Display display, SimpleGrid<CenarioDTO> gridPanel) {
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
					SemanaLetivaDTO semanaLetivaDTO = display.getSemanaLetivaComboBox().getValue();
					Set<CampusDTO> campiDTO = new HashSet<CampusDTO>(display.getCampiDualList().getToField().getStore().getModels());
					service.criar(getDTO(), semanaLetivaDTO, campiDTO, new AsyncCallback<Void>() {
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
		SemanaLetivaDTO semanaLetivaDTO = display.getSemanaLetivaComboBox().getValue();
		cenarioDTO.setSemanaLetivaId(semanaLetivaDTO.getId());
		cenarioDTO.setSemanaLetivaString(semanaLetivaDTO.getCodigo());
		return cenarioDTO;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
