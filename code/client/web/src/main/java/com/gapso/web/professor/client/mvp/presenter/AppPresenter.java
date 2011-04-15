package com.gapso.web.professor.client.mvp.presenter;

import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.gapso.web.professor.client.mvp.view.ToolBarView;
import com.gapso.web.trieda.main.client.services.CenariosServiceAsync;
import com.gapso.web.trieda.main.client.services.Services;
import com.gapso.web.trieda.main.client.services.UsuariosServiceAsync;
import com.gapso.web.trieda.main.client.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.main.client.util.view.GTab;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class AppPresenter implements Presenter {

	public interface Display extends ITriedaI18nGateway {
		ContentPanel getPanel();
		GTab getGTab();
		Widget asWidget();
		Component getComponent();
	}
	
	private Display viewport;
	
	public AppPresenter(Display viewport) {
		this.viewport = viewport;
	}

	@Override
	public void go(final Widget widget) {
		
		CenariosServiceAsync cenarioService = Services.cenarios();
		UsuariosServiceAsync usuarioService = Services.usuarios();
		
		final FutureResult<CenarioDTO> futureCenarioDTO = new FutureResult<CenarioDTO>();
		final FutureResult<UsuarioDTO> futureUsuarioDTO = new FutureResult<UsuarioDTO>();
		
		cenarioService.getMasterData(futureCenarioDTO);
		usuarioService.getUsuario("admin", futureUsuarioDTO);
		
		FutureSynchronizer synch = new FutureSynchronizer(futureCenarioDTO, futureUsuarioDTO);
		
		synch.addCallback(new AbstractAsyncCallbackWithDefaultOnFailure<Boolean>(viewport) {
			@Override
			public void onSuccess(Boolean result) {
				CenarioDTO cenario = futureCenarioDTO.result();
				UsuarioDTO usuario = futureUsuarioDTO.result();
				
				RootPanel rp = (RootPanel) widget;
				Presenter presenter = new ToolBarPresenter(cenario, usuario, new ToolBarView());
				presenter.go(viewport.asWidget());
				rp.add(viewport.asWidget());
				RootPanel.get("loading").setVisible(false);
			}
		});
		
	}

}
