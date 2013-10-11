package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.main.client.mvp.presenter.SelecionarCampiPresenter.ParametrosViewGateway;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.services.TurnosServiceAsync;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class SelecionarTurnosPresenter implements Presenter {
	public interface Display extends ITriedaI18nGateway {
		ListView< TurnoDTO > getNaoSelecionadoList();
		ListView< TurnoDTO > getSelecionadoList();
		Button getAdicionaBT();
		Button getRemoveBT();
		Button getFecharBT();
		Component getComponent();
		SimpleModal getSimpleModal();
	}
	
	private Display display;
	private ParametroDTO parametroDTO;
	private ParametrosViewGateway parametrosViewGateway;
	private CenarioDTO cenarioDTO;
	
	public SelecionarTurnosPresenter(CenarioDTO cenarioDTO, ParametroDTO parametroDTO, Display display, ParametrosViewGateway parametrosViewGateway) {
		this.display = display;
		this.parametroDTO = parametroDTO;
		this.parametrosViewGateway = parametrosViewGateway;
		this.cenarioDTO = cenarioDTO;

		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final TurnosServiceAsync service = Services.turnos();
		RpcProxy<ListLoadResult<TurnoDTO>> proxyNaoSelecionado = new RpcProxy<ListLoadResult<TurnoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<TurnoDTO>> callback) {
				service.getTurnosNaoSelecionadosParaOtimizacao(cenarioDTO, parametroDTO.getTurnos(),callback);
			}
		};
		display.getNaoSelecionadoList().setStore(new ListStore<TurnoDTO>(new BaseListLoader<ListLoadResult<TurnoDTO>>(proxyNaoSelecionado)));
		display.getNaoSelecionadoList().getStore().getLoader().load();
		
		ListStore<TurnoDTO> listStore = new ListStore<TurnoDTO>();
		listStore.add(new ArrayList<TurnoDTO>(parametroDTO.getTurnos()));
		display.getSelecionadoList().setStore(listStore);
		display.getFecharBT().setEnabled(false);
	}
	
	private void setListeners() {

		display.getAdicionaBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<TurnoDTO> turnosDTOList = display.getNaoSelecionadoList().getSelectionModel().getSelectedItems();
				transfere(display.getNaoSelecionadoList(), display.getSelecionadoList(), turnosDTOList);
				display.getFecharBT().setEnabled(!display.getSelecionadoList().getStore().getModels().isEmpty());
			}
		});
		
		display.getRemoveBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<TurnoDTO> turnosDTOList = display.getSelecionadoList().getSelectionModel().getSelectedItems();
				transfere(display.getSelecionadoList(), display.getNaoSelecionadoList(), turnosDTOList);
				display.getFecharBT().setEnabled(!display.getSelecionadoList().getStore().getModels().isEmpty());
			}
		});
		
		display.getFecharBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				parametroDTO.getTurnos().clear();
				parametroDTO.getTurnos().addAll(display.getSelecionadoList().getStore().getModels());
				display.getSimpleModal().hide();
				parametrosViewGateway.updateQuantidadeTurnosSelecionados(parametroDTO.getTurnos().size());
			}
		});
	}

	private void transfere(ListView<TurnoDTO> origem, ListView<TurnoDTO> destino, List<TurnoDTO> turnoDTOList) {
		ListStore<TurnoDTO> storeNaoVinculadas = origem.getStore();  
		ListStore<TurnoDTO> storeVinculadas = destino.getStore();
		
		storeVinculadas.add(turnoDTOList);
		storeVinculadas.sort(TurnoDTO.PROPERTY_DISPLAY_TEXT, SortDir.ASC);
		destino.refresh();
		
		for(TurnoDTO turnoDTO: turnoDTOList) {
			storeNaoVinculadas.remove(turnoDTO);
		}
		origem.refresh();
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}
}

