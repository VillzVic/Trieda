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
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.mvp.view.HorarioDisponivelCenarioFormView;
import com.gapso.web.trieda.client.mvp.view.SemanaLetivaFormView;
import com.gapso.web.trieda.client.services.SemanasLetivaServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class SemanasLetivaPresenter implements Presenter {

	public interface Display {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		Button getDiasDeAulaButton();
		TextField<String> getCodigoBuscaTextField();
		TextField<String> getDescricaoBuscaTextField();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid<SemanaLetivaDTO> getGrid();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<SemanaLetivaDTO>> proxy);
	}
	private Display display; 
	
	public SemanasLetivaPresenter(Display display) {
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final SemanasLetivaServiceAsync service = Services.semanasLetiva();
		RpcProxy<PagingLoadResult<SemanaLetivaDTO>> proxy = new RpcProxy<PagingLoadResult<SemanaLetivaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<SemanaLetivaDTO>> callback) {
//				service.getList((PagingLoadConfig)loadConfig, callback);
				String codigo = display.getCodigoBuscaTextField().getValue();
				String descricao = display.getDescricaoBuscaTextField().getValue();
				service.getBuscaList(codigo, descricao, (PagingLoadConfig)loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new SemanaLetivaFormPresenter(new SemanaLetivaFormView(new SemanaLetivaDTO()), display.getGrid());
				presenter.go(null);
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				SemanaLetivaDTO dto = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Presenter presenter = new SemanaLetivaFormPresenter(new SemanaLetivaFormView(dto), display.getGrid());
				presenter.go(null);
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<SemanaLetivaDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final SemanasLetivaServiceAsync service = Services.semanasLetiva();
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
		display.getDiasDeAulaButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
//				Dispatcher.forwardEvent(AppEvents.SemanaLetivaView);
				SemanaLetivaDTO dto = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Presenter presenter = new HorarioDisponivelCenarioFormPresenter(dto, new HorarioDisponivelCenarioFormView(dto));
				presenter.go(null);
			}
		});
		display.getResetBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getCodigoBuscaTextField().setValue(null);
				display.getDescricaoBuscaTextField().setValue(null);
				display.getGrid().updateList();
			}
		});
		display.getSubmitBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().updateList();
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		GTab tab = (GTab)widget;
		tab.add((GTabItem)display.getComponent());
	}

}
