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
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.view.FixacaoFormView;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.FixacaoDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class FixacoesPresenter implements Presenter {

	public interface Display {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		TextField<String> getCodigoBuscaTextField();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid<FixacaoDTO> getGrid();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<FixacaoDTO>> proxy);
	}
	private CenarioDTO cenario;
	private Display display; 
	
	public FixacoesPresenter(CenarioDTO cenario, Display display) {
		this.cenario = cenario;
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		RpcProxy<PagingLoadResult<FixacaoDTO>> proxy = new RpcProxy<PagingLoadResult<FixacaoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<FixacaoDTO>> callback) {
				String codigo = display.getCodigoBuscaTextField().getValue();
				Services.fixacoes().getBuscaList(codigo, (PagingLoadConfig)loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new FixacaoFormPresenter(cenario, new FixacaoFormView(new FixacaoDTO(), null, null, null, null, null, true), display.getGrid());
				presenter.go(null);
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				final FixacaoDTO fixacaoDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				
				final FutureResult<DisciplinaDTO> futureDisciplinaDTO = new FutureResult<DisciplinaDTO>();
				final FutureResult<CampusDTO> futureCampusDTO = new FutureResult<CampusDTO>();
				final FutureResult<UnidadeDTO> futureUnidadeDTO = new FutureResult<UnidadeDTO>();
				final FutureResult<SalaDTO> futureSalaDTO = new FutureResult<SalaDTO>();
				final FutureResult<List<HorarioDisponivelCenarioDTO>> futureHorariosDTO = new FutureResult<List<HorarioDisponivelCenarioDTO>>();
				
				Services.disciplinas().getDisciplina(fixacaoDTO.getDisciplinaId(), futureDisciplinaDTO);
				Services.campi().getCampus(fixacaoDTO.getCampusId(), futureCampusDTO);
				Services.unidades().getUnidade(fixacaoDTO.getUnidadeId(), futureUnidadeDTO);
				Services.salas().getSala(fixacaoDTO.getSalaId(), futureSalaDTO);
				Services.fixacoes().getHorariosSelecionados(fixacaoDTO, futureHorariosDTO);
				
				FutureSynchronizer synch = new FutureSynchronizer(futureCampusDTO, futureDisciplinaDTO, futureCampusDTO, futureUnidadeDTO, futureSalaDTO, futureHorariosDTO);
				
				synch.addCallback(new AsyncCallback<Boolean>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(Boolean result) {
						DisciplinaDTO disciplinaDTO = futureDisciplinaDTO.result();
						CampusDTO campusDTO = futureCampusDTO.result();
						UnidadeDTO unidadeDTO = futureUnidadeDTO.result();
						SalaDTO salaDTO = futureSalaDTO.result();
						List<HorarioDisponivelCenarioDTO> horariosDTOList = futureHorariosDTO.result();
						
						Presenter presenter = new FixacaoFormPresenter(cenario, new FixacaoFormView(fixacaoDTO, disciplinaDTO, campusDTO, unidadeDTO, salaDTO, horariosDTOList, false), display.getGrid());
						presenter.go(null);
					}
				});
				
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<FixacaoDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				Services.fixacoes().remove(list, new AsyncCallback<Void>() {
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
				display.getCodigoBuscaTextField().setValue(null);
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
		GTab tab = (GTab)widget;
		tab.add((GTabItem)display.getComponent());
	}

}
