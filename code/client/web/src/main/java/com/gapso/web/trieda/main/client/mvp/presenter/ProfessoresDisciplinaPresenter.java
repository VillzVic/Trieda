package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.main.client.mvp.view.ProfessorDisciplinaFormView;
import com.gapso.web.trieda.main.client.services.DisciplinasServiceAsync;
import com.gapso.web.trieda.main.client.services.ProfessoresDisciplinaServiceAsync;
import com.gapso.web.trieda.main.client.services.ProfessoresServiceAsync;
import com.gapso.web.trieda.main.client.services.Services;
import com.gapso.web.trieda.main.client.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.main.client.util.view.GTab;
import com.gapso.web.trieda.main.client.util.view.GTabItem;
import com.gapso.web.trieda.main.client.util.view.ProfessorComboBox;
import com.gapso.web.trieda.main.client.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDisciplinaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class ProfessoresDisciplinaPresenter implements Presenter {

	public interface Display {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		ProfessorComboBox getProfessorBuscaComboBox();
		DisciplinaComboBox getDisciplinaBuscaComboBox();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid<ProfessorDisciplinaDTO> getGrid();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<ProfessorDisciplinaDTO>> proxy);
	}
	private Display display; 
	private GTab gTab;
	
	public ProfessoresDisciplinaPresenter(CenarioDTO cenario, Display display) {
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final ProfessoresDisciplinaServiceAsync service = Services.professoresDisciplina();
		RpcProxy<PagingLoadResult<ProfessorDisciplinaDTO>> proxy = new RpcProxy<PagingLoadResult<ProfessorDisciplinaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<ProfessorDisciplinaDTO>> callback) {
				ProfessorDTO professorDTO = display.getProfessorBuscaComboBox().getValue();
				DisciplinaDTO disciplinaDTO = display.getDisciplinaBuscaComboBox().getValue();
				service.getBuscaList(professorDTO, disciplinaDTO, (PagingLoadConfig)loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new ProfessorDisciplinaFormPresenter(new ProfessorDisciplinaFormView(new ProfessorDisciplinaDTO(), null, null), display.getGrid());
				presenter.go(null);
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				final ProfessorDisciplinaDTO professorDisciplinaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				
				final ProfessoresServiceAsync professoresService = Services.professores();
				final DisciplinasServiceAsync disciplinasService = Services.disciplinas();
				
				final FutureResult<ProfessorDTO> futureProfessorDTO = new FutureResult<ProfessorDTO>();
				final FutureResult<DisciplinaDTO> futureDisciplinaDTO = new FutureResult<DisciplinaDTO>();
				
				professoresService.getProfessor(professorDisciplinaDTO.getProfessorId(), futureProfessorDTO);
				disciplinasService.getDisciplina(professorDisciplinaDTO.getDisciplinaId(), futureDisciplinaDTO);
				
				FutureSynchronizer synch = new FutureSynchronizer(futureProfessorDTO, futureDisciplinaDTO);
				
				synch.addCallback(new AsyncCallback<Boolean>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(Boolean result) {
						ProfessorDTO professorDTO = futureProfessorDTO.result();
						DisciplinaDTO disciplinaDTO = futureDisciplinaDTO.result();
						
						Presenter presenter = new ProfessorDisciplinaFormPresenter(new ProfessorDisciplinaFormView(professorDisciplinaDTO, professorDTO, disciplinaDTO), display.getGrid());
						presenter.go(null);
					}
				});
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<ProfessorDisciplinaDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final ProfessoresDisciplinaServiceAsync service = Services.professoresDisciplina();
				service.remove(list, new AsyncCallback<Void>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(Void result) {
						display.getGrid().updateList();
						Info.display("Removido", "Item removido com sucesso!");
					}
				});
			}
		});
		display.getResetBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getProfessorBuscaComboBox().setValue(null);
				display.getDisciplinaBuscaComboBox().setValue(null);
				display.getGrid().updateList();
			}
		});
		display.getSubmitBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().updateList();
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		gTab = (GTab)widget;
		gTab.add((GTabItem)display.getComponent());
	}

}
