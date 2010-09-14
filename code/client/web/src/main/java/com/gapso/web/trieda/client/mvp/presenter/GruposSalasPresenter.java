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
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.gapso.web.trieda.client.mvp.view.GrupoSalaFormView;
import com.gapso.web.trieda.client.services.GruposSalasServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.services.UnidadesServiceAsync;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class GruposSalasPresenter implements Presenter {

	public interface Display {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		SimpleGrid<GrupoSalaDTO> getGrid();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<GrupoSalaDTO>> proxy);
	}
	private Display display; 
	
	public GruposSalasPresenter(Display display) {
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final GruposSalasServiceAsync service = Services.gruposSalas();
		RpcProxy<PagingLoadResult<GrupoSalaDTO>> proxy = new RpcProxy<PagingLoadResult<GrupoSalaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<GrupoSalaDTO>> callback) {
				service.getBuscaList(null, null, null, (PagingLoadConfig)loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new GrupoSalaFormPresenter(new GrupoSalaFormView(new GrupoSalaDTO(), null), display.getGrid());
				presenter.go(null);
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				final GrupoSalaDTO grupoSalaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				final UnidadesServiceAsync unidadesService = Services.unidades();				
				unidadesService.getUnidade(grupoSalaDTO.getUnidadeId(), new AsyncCallback<UnidadeDTO>() {
					@Override
					public void onFailure(Throwable caught) {
						caught.printStackTrace();
					}
					@Override
					public void onSuccess(UnidadeDTO unidadeDTO) {
						Presenter presenter = new GrupoSalaFormPresenter(new GrupoSalaFormView(grupoSalaDTO, unidadeDTO), display.getGrid());
						presenter.go(null);
					}
				});
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<GrupoSalaDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final GruposSalasServiceAsync service = Services.gruposSalas();
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
	}
	
	@Override
	public void go(Widget widget) {
		GTab tab = (GTab)widget;
		tab.add((GTabItem)display.getComponent());
	}

}
