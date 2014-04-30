package com.gapso.web.trieda.main.client.mvp.presenter;

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
import com.gapso.web.trieda.main.client.mvp.view.CompararCenariosView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CenariosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CompararCenariosFormPresenter implements Presenter {
	public interface Display extends ITriedaI18nGateway {
		ListView< CenarioDTO > getNaoSelecionadoList();
		ListView< CenarioDTO > getSelecionadoList();
		Button getAdicionaBT();
		Button getRemoveBT();
		Button getFecharBT();
		Component getComponent();
		SimpleModal getSimpleModal();
	}
	
	private Display display;
	private GTab gTab;
	
	public CompararCenariosFormPresenter(Display display, GTab gTab) {
		this.display = display;
		this.gTab = gTab;

		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final CenariosServiceAsync service = Services.cenarios();
		RpcProxy<ListLoadResult<CenarioDTO>> proxyNaoSelecionado = new RpcProxy<ListLoadResult<CenarioDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<CenarioDTO>> callback) {
				service.getCenarios( callback );
			}
		};
		display.getNaoSelecionadoList().setStore(new ListStore<CenarioDTO>(new BaseListLoader<ListLoadResult<CenarioDTO>>(proxyNaoSelecionado)));
		display.getNaoSelecionadoList().getStore().getLoader().load();
		
		ListStore<CenarioDTO> listStore = new ListStore<CenarioDTO>();
		display.getSelecionadoList().setStore(listStore);
		display.getFecharBT().setEnabled(false);
	}
	
	private void setListeners() {

		display.getAdicionaBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<CenarioDTO> campiDTOList = display.getNaoSelecionadoList().getSelectionModel().getSelectedItems();
				transfere(display.getNaoSelecionadoList(), display.getSelecionadoList(), campiDTOList);
				display.getFecharBT().setEnabled(!display.getSelecionadoList().getStore().getModels().isEmpty());
			}
		});
		
		display.getRemoveBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<CenarioDTO> campiDTOList = display.getSelecionadoList().getSelectionModel().getSelectedItems();
				transfere(display.getSelecionadoList(), display.getNaoSelecionadoList(), campiDTOList);
				display.getFecharBT().setEnabled(!display.getSelecionadoList().getStore().getModels().isEmpty());
			}
		});
		
		display.getFecharBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new CompararCenariosPresenter( new CompararCenariosView(display.getSelecionadoList().getStore().getModels()) );

				presenter.go( gTab );

				display.getSimpleModal().hide();
			}
		});
	}

	private void transfere(ListView<CenarioDTO> origem, ListView<CenarioDTO> destino, List<CenarioDTO> CenarioDTOList) {
		ListStore<CenarioDTO> storeNaoVinculadas = origem.getStore();  
		ListStore<CenarioDTO> storeVinculadas = destino.getStore();
		
		storeVinculadas.add(CenarioDTOList);
		storeVinculadas.sort(CenarioDTO.PROPERTY_DISPLAY_TEXT, SortDir.ASC);
		destino.refresh();
		
		for(CenarioDTO CenarioDTO: CenarioDTOList) {
			storeNaoVinculadas.remove(CenarioDTO);
		}
		origem.refresh();
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}
}