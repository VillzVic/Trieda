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
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.TipoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.gapso.web.trieda.client.mvp.view.SalaFormView;
import com.gapso.web.trieda.client.services.SalasServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.services.UnidadesServiceAsync;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class SalasPresenter implements Presenter {

	public interface Display {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		Button getDisciplinasAssociadasButton();
		Button getGruposDeSalasButton();
		SimpleGrid<SalaDTO> getGrid();
		GTabItem getGTabItem();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<SalaDTO>> proxy);
	}

	private Display display;

	public SalasPresenter(Display display) {
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final SalasServiceAsync service = Services.salas();
		RpcProxy<PagingLoadResult<SalaDTO>> proxy = new RpcProxy<PagingLoadResult<SalaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<SalaDTO>> callback) {
				service.getList((PagingLoadConfig) loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}

	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new SalaFormPresenter(new SalaFormView(new SalaDTO(), null, null), display.getGrid());
				presenter.go(null);
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				final SalaDTO salaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				UnidadesServiceAsync unidadesService = Services.unidades();
				SalasServiceAsync salasService = Services.salas();

				final FutureResult<UnidadeDTO> futureUnidadeDTO = new FutureResult<UnidadeDTO>();
				final FutureResult<TipoSalaDTO> futureSalaDTO = new FutureResult<TipoSalaDTO>();
				unidadesService.getUnidade(salaDTO.getUnidadeId(), futureUnidadeDTO);
				salasService.getTipoSala(salaDTO.getTipoId(), futureSalaDTO);

				FutureSynchronizer synch = new FutureSynchronizer(futureUnidadeDTO, futureSalaDTO);
				synch.addCallback(new com.google.gwt.user.client.rpc.AsyncCallback<Boolean>() {
					public void onFailure(Throwable caught) {}
					public void onSuccess(Boolean result) {
						UnidadeDTO unidadeDTO = futureUnidadeDTO.result();
						TipoSalaDTO tipoSalaDTO = futureSalaDTO.result();
						Presenter presenter = new SalaFormPresenter(new SalaFormView(salaDTO, unidadeDTO, tipoSalaDTO), display.getGrid());
						presenter.go(null);
					}
				});

			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<SalaDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final SalasServiceAsync service = Services.salas();
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
		GTab tab = (GTab) widget;
		tab.add((GTabItem) display.getComponent());
	}
}
