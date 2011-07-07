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
import com.gapso.web.trieda.main.client.mvp.view.ParametrosView;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CampiServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class SelecionarCampiPresenter implements Presenter {

	public interface Display extends ITriedaI18nGateway {
		ListView<CampusDTO> getNaoSelecionadoList();
		ListView<CampusDTO> getSelecionadoList();
		Button getAdicionaBT();
		Button getRemoveBT();
		Button getFecharBT();
		Component getComponent();
		
		SimpleModal getSimpleModal();
	}
	
	private Display display;
	private CenarioDTO cenario;
	private GTab gTab;
	
	public SelecionarCampiPresenter(CenarioDTO cenario, Display display) {
		this.display = display;
		this.cenario = cenario;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final CampiServiceAsync service = Services.campi();
		RpcProxy<ListLoadResult<CampusDTO>> proxyNaoSelecionado = new RpcProxy<ListLoadResult<CampusDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<CampusDTO>> callback) {
				service.getListAll(callback);
			}
		};
		display.getNaoSelecionadoList().setStore(new ListStore<CampusDTO>(new BaseListLoader<ListLoadResult<CampusDTO>>(proxyNaoSelecionado)));
		display.getNaoSelecionadoList().getStore().getLoader().load();
		
		ListStore<CampusDTO> listStore = new ListStore<CampusDTO>();
		listStore.add(new ArrayList<CampusDTO>());
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
				Services.otimizar().getParametro(cenario, new AbstractAsyncCallbackWithDefaultOnFailure<ParametroDTO>(display) {
					@SuppressWarnings("unused")
					@Override
					public void onSuccess(ParametroDTO parametroDTO) {
						display.getSimpleModal().hide();
						List<CampusDTO> campi = display.getSelecionadoList().getStore().getModels();
						Presenter presenter = new ParametrosPresenter(cenario, /*campi, */new ParametrosView(parametroDTO));
						presenter.go(gTab);
					}
				});
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
		this.gTab = (GTab) widget;
		display.getSimpleModal().show();
	}

}
