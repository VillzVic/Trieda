package com.gapso.web.trieda.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.client.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.gapso.web.trieda.client.mvp.view.GrupoSalaAssociarSalaView;
import com.gapso.web.trieda.client.mvp.view.GrupoSalaFormView;
import com.gapso.web.trieda.client.services.GruposSalasServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class GruposSalasPresenter implements Presenter {

	public interface Display extends ITriedaI18nGateway {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		Button getAssociarSalasButton();
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
				Services.unidades().getUnidade(grupoSalaDTO.getUnidadeId(), new AbstractAsyncCallbackWithDefaultOnFailure<UnidadeDTO>(display) {
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
				Services.gruposSalas().remove(list, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
					@Override
					public void onSuccess(Void result) {
						display.getGrid().updateList();
						Info.display("Removido", "Item removido com sucesso!");
					}
				});
			}
		});
		display.getAssociarSalasButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				GrupoSalaDTO grupoSalaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Presenter presenter = new GrupoSalaAssociarSalaPresenter(new GrupoSalaAssociarSalaView(grupoSalaDTO), display.getGrid());
				presenter.go(null);
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		GTab tab = (GTab)widget;
		tab.add((GTabItem)display.getComponent());
	}

}
