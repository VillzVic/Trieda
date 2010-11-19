package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.DemandaDTO;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.google.gwt.user.client.ui.Widget;

public class DemandasPresenter implements Presenter {

	public interface Display {
		Button getSaveButton();
		Button getResetButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		SimpleGrid<DemandaDTO> getGrid();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<DemandaDTO>> proxy);
	}
	private CenarioDTO cenario;
	private Display display; 
	
	public DemandasPresenter(CenarioDTO cenario, Display display) {
		this.cenario = cenario;
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
//		final TurnosServiceAsync service = Services.turnos();
//		RpcProxy<PagingLoadResult<TurnoDTO>> proxy = new RpcProxy<PagingLoadResult<TurnoDTO>>() {
//			@Override
//			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<TurnoDTO>> callback) {
//				String nome = display.getNomeBuscaTextField().getValue();
//				Number tempo = display.getTempoBuscaTextField().getValue();
//				service.getBuscaList(nome, (tempo==null)?null:tempo.intValue(), (PagingLoadConfig)loadConfig, callback);
//			}
//		};
//		display.setProxy(proxy);
	}
	
	private void setListeners() {
//		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
//			@Override
//			public void componentSelected(ButtonEvent ce) {
//				List<TurnoDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
//				final TurnosServiceAsync service = Services.turnos();
//				service.remove(list, new AsyncCallback<Void>() {
//					@Override
//					public void onFailure(Throwable caught) {
//						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
//					}
//					@Override
//					public void onSuccess(Void result) {
//						display.getGrid().updateList();
//						Info.display("Removido", "Item removido com sucesso!");
//					}
//				});
//			}
//		});
	}
	
	@Override
	public void go(Widget widget) {
		GTab tab = (GTab)widget;
		tab.add((GTabItem)display.getComponent());
	}

}
