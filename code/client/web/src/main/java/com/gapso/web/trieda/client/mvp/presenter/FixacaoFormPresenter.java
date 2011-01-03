package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.FixacaoDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.client.util.view.SalaComboBox;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.UnidadeComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class FixacaoFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		TextField<String> getCodigoTextField();
		TextField<String> getDescricaoTextField();
		DisciplinaComboBox getDisciplinaComboBox();
		CampusComboBox getCampusComboBox();
		UnidadeComboBox getUnidadeComboBox();
		SalaComboBox getSalaComboBox();
		FixacaoDTO getFixacaoDTO();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}
	private CenarioDTO cenario;
	private SimpleGrid<FixacaoDTO> gridPanel;
	private Display display;
	
	public FixacaoFormPresenter(CenarioDTO cenario, Display display, SimpleGrid<FixacaoDTO> gridPanel) {
		this.cenario = cenario;
		this.gridPanel = gridPanel;
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					Services.fixacoes().save(getDTO(), new AsyncCallback<Void>() {
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
	
	private FixacaoDTO getDTO() {
		FixacaoDTO fixacaoDTO = display.getFixacaoDTO();
		fixacaoDTO.setCodigo(display.getCodigoTextField().getValue());
		fixacaoDTO.setDescricao(display.getDescricaoTextField().getValue());
		DisciplinaDTO disciplina = display.getDisciplinaComboBox().getValue();
		if(disciplina != null) {
			fixacaoDTO.setDisciplinaId(disciplina.getId());
			fixacaoDTO.setDisciplinaString(disciplina.getCodigo());
		}
		CampusDTO campus = display.getCampusComboBox().getValue();
		if(campus != null) {
			fixacaoDTO.setCampusId(campus.getId());
			fixacaoDTO.setCampusString(campus.getCodigo());
		}
		UnidadeDTO unidade = display.getUnidadeComboBox().getValue();
		if(unidade != null) {
			fixacaoDTO.setUnidadeId(unidade.getId());
			fixacaoDTO.setUnidadeString(unidade.getCodigo());
		}
		SalaDTO sala = display.getSalaComboBox().getValue();
		if(sala != null) {
			fixacaoDTO.setSalaId(sala.getId());
			fixacaoDTO.setSalaString(sala.getCodigo());
		}
		return fixacaoDTO;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
