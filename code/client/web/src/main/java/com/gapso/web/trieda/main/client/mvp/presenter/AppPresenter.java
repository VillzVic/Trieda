package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.gapso.web.trieda.main.client.mvp.view.ToolBarView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CenariosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.services.UsuariosServiceAsync;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.CenarioPanel;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.google.gwt.user.client.Timer;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class AppPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		ContentPanel getPanel();
		GTab getGTab();
		Widget asWidget();
		Component getComponent();
		CenarioPanel getCenarioPanel();
	}

	private Display viewport;

	public AppPresenter( Display viewport )
	{
		this.viewport = viewport;
	}
	
	// Código relacionado com a issue:
	// http://jira.gapso.com.br/browse/TRIEDA-990
	private void createThreadAvoidSessionExpire()
	{
		final Timer t = new Timer()
		{
			@Override
			public void run()
			{
			    UsuariosServiceAsync usuariosService = Services.usuarios();
				usuariosService.avoidSessionExpire( new AsyncCallback< Boolean >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						System.out.println( "Erro ao conectar-se com mo servidor!!!" );					
					}

					@Override
					public void onSuccess( Boolean result )
					{
						// Ping realizado com sucesso
					}
				});
			}
		};

		Integer minutos = 10;
		t.scheduleRepeating( minutos * 60 * 1000 );
	}

	@Override
	public void go(final Widget widget) {
		CenariosServiceAsync cenarioService = Services.cenarios();
		UsuariosServiceAsync usuarioService = Services.usuarios();

		final FutureResult<CenarioDTO> futureCenarioDTO = new FutureResult<CenarioDTO>();
		final FutureResult<UsuarioDTO> futureUsuarioDTO = new FutureResult<UsuarioDTO>();
		final FutureResult<InstituicaoEnsinoDTO> futureInstituicaoEnsinoDTO = new FutureResult<InstituicaoEnsinoDTO>();

		cenarioService.getMasterData(futureCenarioDTO);
		usuarioService.getCurrentUser(futureUsuarioDTO);
		cenarioService.getInstituicaoEnsinoDTO(futureInstituicaoEnsinoDTO);

		FutureSynchronizer synch = new FutureSynchronizer(futureCenarioDTO,futureUsuarioDTO,futureInstituicaoEnsinoDTO);
		synch.addCallback(new AbstractAsyncCallbackWithDefaultOnFailure<Boolean>(viewport) {
			@Override
			public void onSuccess(Boolean result) {
				CenarioDTO cenario = futureCenarioDTO.result();
				UsuarioDTO usuario = futureUsuarioDTO.result();
				InstituicaoEnsinoDTO instituicaoEnsino =  futureInstituicaoEnsinoDTO.result();

				RootPanel rp = (RootPanel) widget;
				Presenter presenter = new ToolBarPresenter(instituicaoEnsino,cenario,usuario,viewport.getCenarioPanel(),new ToolBarView());
				presenter.go(viewport.asWidget());
				rp.add(viewport.asWidget());
				RootPanel.get("loading").setVisible(false);

				// Enquanto o browser estiver aberto, a sessão não irá expirar
				createThreadAvoidSessionExpire();
			}
		});
	}
}
