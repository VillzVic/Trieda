package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.mvp.model.AreaTitulacaoDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorDTO;
import com.gapso.web.trieda.client.mvp.model.TipoContratoDTO;
import com.gapso.web.trieda.client.mvp.model.TitulacaoDTO;
import com.gapso.web.trieda.client.services.ProfessoresServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.AreaTitulacaoComboBox;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.TipoContratoComboBox;
import com.gapso.web.trieda.client.util.view.TitulacaoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class ProfessorFormPresenter implements Presenter {

	public interface Display {
		TextField<String> getCpfTextField();
		TextField<String> getNomeTextField();
		TipoContratoComboBox getTipoContratoComboBox();
		NumberField getCargaHorariaMaxNumberField();
		NumberField getCargaHorariaMinNumberField();
		TitulacaoComboBox getTitulacaoComboBox();
		AreaTitulacaoComboBox getAreaTitulacaoComboBox();
		NumberField getCreditoAnteriorNumberField();
		NumberField getValorCreditoNumberField();
		Button getSalvarButton();
		ProfessorDTO getProfessorDTO();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}
	private SimpleGrid<ProfessorDTO> gridPanel;
	private Display display;
	
	public ProfessorFormPresenter(Display display, SimpleGrid<ProfessorDTO> gridPanel) {
		this.gridPanel = gridPanel;
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					final ProfessoresServiceAsync service = Services.professores();
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
	
	private ProfessorDTO getDTO() {
		ProfessorDTO professorDTO = display.getProfessorDTO();
		professorDTO.setCpf(display.getCpfTextField().getValue());
		professorDTO.setNome(display.getNomeTextField().getValue());
		TipoContratoDTO tipoContratoDTO = display.getTipoContratoComboBox().getValue();
		professorDTO.setTipoContratoId(tipoContratoDTO.getId());
		professorDTO.setTipoContratoString(tipoContratoDTO.getNome());
		professorDTO.setCargaHorariaMax(display.getCargaHorariaMaxNumberField().getValue().intValue());
		professorDTO.setCargaHorariaMin(display.getCargaHorariaMinNumberField().getValue().intValue());
		TitulacaoDTO titulacaoDTO = display.getTitulacaoComboBox().getValue();
		professorDTO.setTitulacaoId(titulacaoDTO.getId());
		professorDTO.setTitulacaoString(titulacaoDTO.getNome());
		AreaTitulacaoDTO areaTitulacaoDTO = display.getAreaTitulacaoComboBox().getValue();
		professorDTO.setAreaTitulacaoId(areaTitulacaoDTO.getId());
		professorDTO.setAreaTitulacaoString(areaTitulacaoDTO.getCodigo());
		professorDTO.setCreditoAnterior(display.getCreditoAnteriorNumberField().getValue().intValue());
		professorDTO.setValorCredito(display.getValorCreditoNumberField().getValue().doubleValue());
		return professorDTO;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
