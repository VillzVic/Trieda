package com.gapso.web.trieda.main.client.mvp.presenter;

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
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.mvp.view.CenarioCloneFormView;
import com.gapso.web.trieda.main.client.mvp.view.CenarioCriarFormView;
import com.gapso.web.trieda.main.client.mvp.view.CenarioEditarFormView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CenariosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.CenarioPanel;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CenariosPresenter implements Presenter {

	public interface Display {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getAbrirCenarioButton();
		Button getClonarCenarioButton();
		TextField<Integer> getAnoBuscaTextField();
		TextField<Integer> getSemestreBuscaTextField();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid<CenarioDTO> getGrid();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<CenarioDTO>> proxy);
	}
	private Display display; 
	private GTab gTab;
	private CenarioPanel cenarioPanel;
	
	public CenariosPresenter(CenarioPanel cenarioPanel, Display display) {
		this.display = display;
		this.cenarioPanel = cenarioPanel;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final CenariosServiceAsync service = Services.cenarios();
		RpcProxy<PagingLoadResult<CenarioDTO>> proxy = new RpcProxy<PagingLoadResult<CenarioDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<CenarioDTO>> callback) {
				Integer ano = display.getAnoBuscaTextField().getValue();
				Integer semestre = display.getSemestreBuscaTextField().getValue();
				service.getBuscaList(ano, semestre, (PagingLoadConfig)loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new CenarioCriarFormPresenter(new CenarioCriarFormView(new CenarioDTO()), display.getGrid());
				presenter.go(null);
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				CenarioDTO cenarioDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Presenter presenter = new CenarioEditarFormPresenter(new CenarioEditarFormView(cenarioDTO), display.getGrid());
				presenter.go(null);
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<CenarioDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final CenariosServiceAsync service = Services.cenarios();
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
		display.getResetBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getAnoBuscaTextField().setValue(null);
				display.getSemestreBuscaTextField().setValue(null);
				display.getGrid().updateList();
			}
		});
		display.getSubmitBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().updateList();
			}
		});
		display.getAbrirCenarioButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				CenarioDTO cenarioDTO = display.getGrid().getSelectionModel().getSelectedItem();
				cenarioPanel.addCenario(cenarioDTO);
			}
		});
		display.getClonarCenarioButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				CenarioDTO cenarioDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Presenter presenter = new CenarioCloneFormPresenter(new CenarioCloneFormView(cenarioDTO), display.getGrid());
				presenter.go(null);
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		gTab = (GTab)widget;
		gTab.add((GTabItem)display.getComponent());
	}

}
