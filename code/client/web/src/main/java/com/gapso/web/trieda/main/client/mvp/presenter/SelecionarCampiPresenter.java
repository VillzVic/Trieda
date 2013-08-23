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
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CampiServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class SelecionarCampiPresenter implements Presenter {
	public interface Display extends ITriedaI18nGateway {
		ListView< CampusDTO > getNaoSelecionadoList();
		ListView< CampusDTO > getSelecionadoList();
		Button getAdicionaBT();
		Button getRemoveBT();
		Button getFecharBT();
		Component getComponent();
		SimpleModal getSimpleModal();
	}
	
	public interface ParametrosViewGateway {
		void updateQuantidadeCampiSelecionados(int qtdCampiSelecionados);
	}
	
	private Display display;
	private ParametroDTO parametroDTO;
	private ParametrosViewGateway parametrosViewGateway;
	private CenarioDTO cenarioDTO;
	
	public SelecionarCampiPresenter(CenarioDTO cenarioDTO, ParametroDTO parametroDTO, Display display, ParametrosViewGateway parametrosViewGateway) {
		this.display = display;
		this.parametroDTO = parametroDTO;
		this.parametrosViewGateway = parametrosViewGateway;
		this.cenarioDTO = cenarioDTO;

		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final CampiServiceAsync service = Services.campi();
		RpcProxy<ListLoadResult<CampusDTO>> proxyNaoSelecionado = new RpcProxy<ListLoadResult<CampusDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<CampusDTO>> callback) {
				service.getCampiNaoSelecionadosParaOtimizacao(cenarioDTO, parametroDTO.getCampi(),callback);
			}
		};
		display.getNaoSelecionadoList().setStore(new ListStore<CampusDTO>(new BaseListLoader<ListLoadResult<CampusDTO>>(proxyNaoSelecionado)));
		display.getNaoSelecionadoList().getStore().getLoader().load();
		
		ListStore<CampusDTO> listStore = new ListStore<CampusDTO>();
		listStore.add(new ArrayList<CampusDTO>(parametroDTO.getCampi()));
		display.getSelecionadoList().setStore(listStore);
		display.getFecharBT().setEnabled(false);
	}
	
	private void setListeners() {

		display.getAdicionaBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<CampusDTO> campiDTOList = display.getNaoSelecionadoList().getSelectionModel().getSelectedItems();
				transfere(display.getNaoSelecionadoList(), display.getSelecionadoList(), campiDTOList);
				display.getFecharBT().setEnabled(!display.getSelecionadoList().getStore().getModels().isEmpty());
			}
		});
		
		display.getRemoveBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<CampusDTO> campiDTOList = display.getSelecionadoList().getSelectionModel().getSelectedItems();
				transfere(display.getSelecionadoList(), display.getNaoSelecionadoList(), campiDTOList);
				display.getFecharBT().setEnabled(!display.getSelecionadoList().getStore().getModels().isEmpty());
			}
		});
		
		display.getFecharBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				parametroDTO.getCampi().clear();
				parametroDTO.getCampi().addAll(display.getSelecionadoList().getStore().getModels());
				parametroDTO.getMaximizarNotaAvaliacaoCorpoDocenteList().clear();
				parametroDTO.getMinimizarCustoDocenteCursosList().clear();
				display.getSimpleModal().hide();
				parametrosViewGateway.updateQuantidadeCampiSelecionados(parametroDTO.getCampi().size());
			}
		});
	}

	private void transfere(ListView<CampusDTO> origem, ListView<CampusDTO> destino, List<CampusDTO> campusDTOList) {
		ListStore<CampusDTO> storeNaoVinculadas = origem.getStore();  
		ListStore<CampusDTO> storeVinculadas = destino.getStore();
		
		storeVinculadas.add(campusDTOList);
		storeVinculadas.sort(CampusDTO.PROPERTY_DISPLAY_TEXT, SortDir.ASC);
		destino.refresh();
		
		for(CampusDTO campusDTO: campusDTOList) {
			storeNaoVinculadas.remove(campusDTO);
		}
		origem.refresh();
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}
}
