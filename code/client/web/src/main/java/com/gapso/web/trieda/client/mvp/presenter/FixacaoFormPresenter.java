package com.gapso.web.trieda.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.client.util.view.ProfessorComboBox;
import com.gapso.web.trieda.client.util.view.SalaComboBox;
import com.gapso.web.trieda.client.util.view.SemanaLetivaDoCenarioGrid;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.UnidadeComboBox;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.FixacaoDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
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
		SemanaLetivaDoCenarioGrid<HorarioDisponivelCenarioDTO> getGrid();
		boolean isValid();
		
		SimpleModal getSimpleModal();
		ProfessorComboBox getProfessorComboBox();
	}
	private SimpleGrid<FixacaoDTO> gridPanel;
	private Display display;
	
	public FixacaoFormPresenter(CenarioDTO cenario, Display display, SimpleGrid<FixacaoDTO> gridPanel) {
		this.gridPanel = gridPanel;
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		RpcProxy<PagingLoadResult<HorarioDisponivelCenarioDTO>> proxy = new RpcProxy<PagingLoadResult<HorarioDisponivelCenarioDTO>>() {
			@Override
			protected void load(Object loadConfig, AsyncCallback<PagingLoadResult<HorarioDisponivelCenarioDTO>> callback) {
				DisciplinaDTO disciplinaDTO = display.getDisciplinaComboBox().getValue();
				SalaDTO salaDTO = display.getSalaComboBox().getValue();
				ProfessorDTO professorDTO = display.getProfessorComboBox().getValue();
				Services.fixacoes().getHorariosDisponiveis(professorDTO, disciplinaDTO, salaDTO, callback);
			}
		};
		display.getGrid().setProxy(proxy);
	}
	
	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					List<HorarioDisponivelCenarioDTO> hdcDTOList = display.getGrid().getStore().getModels();
					Services.fixacoes().save(getDTO(), hdcDTOList, new AsyncCallback<Void>() {
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
		display.getProfessorComboBox().addSelectionChangedListener(new SelectionChangedListener<ProfessorDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<ProfessorDTO> se) {
				display.getGrid().updateList();
			}
		});
		display.getDisciplinaComboBox().addSelectionChangedListener(new SelectionChangedListener<DisciplinaDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<DisciplinaDTO> se) {
				display.getGrid().updateList();
			}
		});
		display.getSalaComboBox().addSelectionChangedListener(new SelectionChangedListener<SalaDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<SalaDTO> se) {
				display.getGrid().updateList();
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
		ProfessorDTO professor = display.getProfessorComboBox().getValue();
		if(professor != null) {
			fixacaoDTO.setProfessorId(professor.getId());
			fixacaoDTO.setProfessorString(professor.getNome());
		}
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
