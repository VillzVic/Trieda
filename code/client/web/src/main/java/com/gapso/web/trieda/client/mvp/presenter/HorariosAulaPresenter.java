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
import com.gapso.web.trieda.client.mvp.model.HorarioAulaDTO;
import com.gapso.web.trieda.client.mvp.view.HorarioAulaFormView;
import com.gapso.web.trieda.client.services.HorariosAulaServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class HorariosAulaPresenter implements Presenter {

	public interface Display {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		SimpleGrid<HorarioAulaDTO> getGrid();
		GTabItem getGTabItem();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<HorarioAulaDTO>> proxy);
	}
	private Display display; 
	
	public HorariosAulaPresenter(Display display) {
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final HorariosAulaServiceAsync service = Services.horariosAula();
		RpcProxy<PagingLoadResult<HorarioAulaDTO>> proxy = new RpcProxy<PagingLoadResult<HorarioAulaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<HorarioAulaDTO>> callback) {
				service.getList((PagingLoadConfig)loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new HorarioAulaFormPresenter(new HorarioAulaFormView(new HorarioAulaDTO()), display.getGrid());
				presenter.go(null);
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				HorarioAulaDTO horarioAulaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Presenter presenter = new HorarioAulaFormPresenter(new HorarioAulaFormView(horarioAulaDTO), display.getGrid());
				presenter.go(null);
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<HorarioAulaDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final HorariosAulaServiceAsync service = Services.horariosAula();
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
