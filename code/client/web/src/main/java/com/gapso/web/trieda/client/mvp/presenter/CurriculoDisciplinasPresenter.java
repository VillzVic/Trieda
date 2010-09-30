package com.gapso.web.trieda.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.client.mvp.view.CurriculoDisciplinaFormView;
import com.gapso.web.trieda.client.services.CurriculosServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CurriculoDisciplinasPresenter implements Presenter {

	public interface Display {
		Button getNewButton();
		Button getRemoveButton();
		CurriculoDTO getCurriculoDTO();
		Grid<CurriculoDisciplinaDTO> getGrid();
		Component getComponent();
		void setProxy(RpcProxy<ListLoadResult<CurriculoDisciplinaDTO>> proxy);
	}
	private Display display; 
	private GTab gTab;
	
	public CurriculoDisciplinasPresenter(Display display) {
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final CurriculosServiceAsync service = Services.curriculos();
		RpcProxy<ListLoadResult<CurriculoDisciplinaDTO>> proxy = new RpcProxy<ListLoadResult<CurriculoDisciplinaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<CurriculoDisciplinaDTO>> callback) {
				CurriculoDTO curriculoDTO = display.getCurriculoDTO();
				service.getDisciplinasList(curriculoDTO, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new CurriculoDisciplinaFormPresenter(new CurriculoDisciplinaFormView(new CurriculoDisciplinaDTO(), display.getCurriculoDTO()), display.getGrid());
				presenter.go(null);
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<CurriculoDisciplinaDTO> list = display.getGrid().getSelectionModel().getSelectedItems();
				final CurriculosServiceAsync service = Services.curriculos();
				service.removeDisciplina(list, new AsyncCallback<Void>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(Void result) {
						display.getGrid().getStore().getLoader().load();
						Info.display("Removido", "Item removido com sucesso!");
					}
				});
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		gTab = (GTab)widget;
		gTab.add((GTabItem)display.getComponent());
	}

}
