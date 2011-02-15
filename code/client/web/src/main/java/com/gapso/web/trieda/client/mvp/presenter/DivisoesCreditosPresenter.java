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
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.DivisaoCreditoDTO;
import com.gapso.web.trieda.client.mvp.view.DivisaoCreditosFormView;
import com.gapso.web.trieda.client.services.DivisoesCreditosServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class DivisoesCreditosPresenter implements Presenter {

	public interface Display {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		SimpleGrid<DivisaoCreditoDTO> getGrid();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<DivisaoCreditoDTO>> proxy);
	}
	private CenarioDTO cenario;
	private Display display; 
	
	public DivisoesCreditosPresenter(CenarioDTO cenario, Display display) {
		this.cenario = cenario;
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final DivisoesCreditosServiceAsync service = Services.divisaoCreditos();
		RpcProxy<PagingLoadResult<DivisaoCreditoDTO>> proxy = new RpcProxy<PagingLoadResult<DivisaoCreditoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<DivisaoCreditoDTO>> callback) {
				service.getList(cenario, (PagingLoadConfig)loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new DivisaoCreditosFormPresenter(cenario, new DivisaoCreditosFormView(new DivisaoCreditoDTO()), display.getGrid());
				presenter.go(null);
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				DivisaoCreditoDTO divisaoCreditoDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Presenter presenter = new DivisaoCreditosFormPresenter(cenario, new DivisaoCreditosFormView(divisaoCreditoDTO), display.getGrid());
				presenter.go(null);
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<DivisaoCreditoDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final DivisoesCreditosServiceAsync service = Services.divisaoCreditos();
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
