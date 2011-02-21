package com.gapso.web.trieda.client.mvp.presenter;

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
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.mvp.model.AreaTitulacaoDTO;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorDTO;
import com.gapso.web.trieda.client.mvp.model.TipoContratoDTO;
import com.gapso.web.trieda.client.mvp.model.TitulacaoDTO;
import com.gapso.web.trieda.client.mvp.view.HorarioDisponivelProfessorFormView;
import com.gapso.web.trieda.client.mvp.view.ProfessorFormView;
import com.gapso.web.trieda.client.services.AreasTitulacaoServiceAsync;
import com.gapso.web.trieda.client.services.ProfessoresServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.AreaTitulacaoComboBox;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.TipoContratoComboBox;
import com.gapso.web.trieda.client.util.view.TitulacaoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class ProfessoresPresenter implements Presenter {

	public interface Display {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		TextField<String> getCpfBuscaTextField();
		TipoContratoComboBox getTipoContratoBuscaComboBox();
		TitulacaoComboBox getTitulacaoBuscaComboBox();
		AreaTitulacaoComboBox getAreaTitulacaoBuscaComboBox();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		Button getDisponibilidadeButton();
		SimpleGrid<ProfessorDTO> getGrid();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<ProfessorDTO>> proxy);
	}
	private CenarioDTO cenario;
	private Display display; 
	private GTab gTab;
	
	public ProfessoresPresenter(CenarioDTO cenario, Display display) {
		this.cenario = cenario;
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final ProfessoresServiceAsync service = Services.professores();
		RpcProxy<PagingLoadResult<ProfessorDTO>> proxy = new RpcProxy<PagingLoadResult<ProfessorDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<ProfessorDTO>> callback) {
				String cpf = display.getCpfBuscaTextField().getValue();
				TipoContratoDTO tipoContratoDTO = display.getTipoContratoBuscaComboBox().getValue();
				TitulacaoDTO titulacaoDTO = display.getTitulacaoBuscaComboBox().getValue();
				AreaTitulacaoDTO areaTitulacaoDTO = display.getAreaTitulacaoBuscaComboBox().getValue();
				service.getBuscaList(cpf, tipoContratoDTO, titulacaoDTO, areaTitulacaoDTO, (PagingLoadConfig)loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new ProfessorFormPresenter(cenario, new ProfessorFormView(new ProfessorDTO(), null, null, null), display.getGrid());
				presenter.go(null);
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				final ProfessorDTO professorDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				
				final ProfessoresServiceAsync professoresService = Services.professores();
				final AreasTitulacaoServiceAsync areasTitulacaoService = Services.areasTitulacao();
				
				final FutureResult<TipoContratoDTO> futureTipoContratoDTO = new FutureResult<TipoContratoDTO>();
				final FutureResult<TitulacaoDTO> futureTitulacaoDTO = new FutureResult<TitulacaoDTO>();
				final FutureResult<AreaTitulacaoDTO> futureAreaTitulacaoDTO = new FutureResult<AreaTitulacaoDTO>();
				
				professoresService.getTipoContrato(professorDTO.getTipoContratoId(), futureTipoContratoDTO);
				professoresService.getTitulacao(professorDTO.getTitulacaoId(), futureTitulacaoDTO);
				areasTitulacaoService.getAreaTitulacao(professorDTO.getAreaTitulacaoId(), futureAreaTitulacaoDTO);
				
				FutureSynchronizer synch = new FutureSynchronizer(futureTipoContratoDTO, futureTitulacaoDTO, futureAreaTitulacaoDTO);
				
				synch.addCallback(new AsyncCallback<Boolean>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(Boolean result) {
						TipoContratoDTO tipoContratoDTO = futureTipoContratoDTO.result();
						TitulacaoDTO titulacaoDTO = futureTitulacaoDTO.result();
						AreaTitulacaoDTO areaTitulacaoDTO = futureAreaTitulacaoDTO.result();
						
						Presenter presenter = new ProfessorFormPresenter(cenario, new ProfessorFormView(professorDTO, tipoContratoDTO, titulacaoDTO, areaTitulacaoDTO), display.getGrid());
						presenter.go(null);
					}
				});
			}
		});
		display.getDisponibilidadeButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				final ProfessorDTO professorDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Services.professores().getHorariosDisponiveis(professorDTO, new AsyncCallback<List<HorarioDisponivelCenarioDTO>>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(List<HorarioDisponivelCenarioDTO> result) {
						Presenter presenter = new HorarioDisponivelProfessorFormPresenter(cenario, new HorarioDisponivelProfessorFormView(professorDTO, result));
						presenter.go(null);
					}
				});
				
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<ProfessorDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final ProfessoresServiceAsync service = Services.professores();
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
				display.getCpfBuscaTextField().setValue(null);
				display.getTipoContratoBuscaComboBox().setValue(null);
				display.getTitulacaoBuscaComboBox().setValue(null);
				display.getAreaTitulacaoBuscaComboBox().setValue(null);
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
