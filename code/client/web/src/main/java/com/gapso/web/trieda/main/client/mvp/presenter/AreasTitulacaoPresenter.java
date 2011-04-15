package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.mvp.view.AreaTitulacaoFormView;
import com.gapso.web.trieda.main.client.services.AreasTitulacaoServiceAsync;
import com.gapso.web.trieda.main.client.services.Services;
import com.gapso.web.trieda.main.client.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.main.client.util.view.GTab;
import com.gapso.web.trieda.main.client.util.view.GTabItem;
import com.gapso.web.trieda.main.client.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class AreasTitulacaoPresenter implements Presenter {

	public interface Display extends ITriedaI18nGateway {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		TextField<String> getCodigoBuscaTextField();
		TextField<String> getDescricaoBuscaTextField();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid<AreaTitulacaoDTO> getGrid();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<AreaTitulacaoDTO>> proxy);
	}
	private Display display; 
	
	public AreasTitulacaoPresenter(Display display) {
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final AreasTitulacaoServiceAsync service = Services.areasTitulacao();
		RpcProxy<PagingLoadResult<AreaTitulacaoDTO>> proxy = new RpcProxy<PagingLoadResult<AreaTitulacaoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<AreaTitulacaoDTO>> callback) {
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
				Presenter presenter = new AreaTitulacaoFormPresenter(new AreaTitulacaoFormView(new AreaTitulacaoDTO()), display.getGrid());
				presenter.go(null);
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				AreaTitulacaoDTO areaTitulacaoDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Presenter presenter = new AreaTitulacaoFormPresenter(new AreaTitulacaoFormView(areaTitulacaoDTO), display.getGrid());
				presenter.go(null);
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override	
			public void componentSelected(ButtonEvent ce) {
				List<AreaTitulacaoDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final AreasTitulacaoServiceAsync service = Services.areasTitulacao();
				service.remove(list, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
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
