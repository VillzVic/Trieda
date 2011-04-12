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
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.mvp.view.UsuarioFormView;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.services.UsuariosServiceAsync;
import com.gapso.web.trieda.client.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class UsuariosPresenter implements Presenter {

	public interface Display extends ITriedaI18nGateway {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		TextField<String> getNomeBuscaTextField();
		TextField<String> getUsernameBuscaTextField();
		TextField<String> getEmailBuscaTextField();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid<UsuarioDTO> getGrid();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<UsuarioDTO>> proxy);
	}
	private Display display; 
	private CenarioDTO cenario; 
	
	public UsuariosPresenter(CenarioDTO cenario, Display display) {
		this.display = display;
		this.cenario = cenario;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final UsuariosServiceAsync service = Services.usuarios();
		RpcProxy<PagingLoadResult<UsuarioDTO>> proxy = new RpcProxy<PagingLoadResult<UsuarioDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<UsuarioDTO>> callback) {
				String nome = display.getNomeBuscaTextField().getValue();
				String username = display.getUsernameBuscaTextField().getValue();
				String email = display.getEmailBuscaTextField().getValue();
				nome = (nome == null) ? "" : nome;
				username = (username == null) ? "" : username;
				email = (email == null) ? "" : email;
				service.getBuscaList(nome, username, email, (PagingLoadConfig)loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new UsuarioFormPresenter(cenario, new UsuarioFormView(new UsuarioDTO()), display.getGrid());
				presenter.go(null);
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				UsuarioDTO usuarioDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Presenter presenter = new UsuarioFormPresenter(cenario, new UsuarioFormView(usuarioDTO), display.getGrid());
				presenter.go(null);
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<UsuarioDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final UsuariosServiceAsync service = Services.usuarios();
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
				display.getNomeBuscaTextField().setValue(null);
				display.getUsernameBuscaTextField().setValue(null);
				display.getEmailBuscaTextField().setValue(null);
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
