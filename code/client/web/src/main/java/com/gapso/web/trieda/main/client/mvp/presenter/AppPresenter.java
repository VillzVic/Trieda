package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.view.AlterarSenhaFormView;
import com.gapso.web.trieda.main.client.mvp.view.CenariosView;
import com.gapso.web.trieda.main.client.mvp.view.ToolBarView;
import com.gapso.web.trieda.main.client.mvp.view.UsuariosView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CenariosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.services.UsuariosServiceAsync;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.CenarioPanel;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.google.gwt.user.client.Timer;
import com.google.gwt.user.client.Window;
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
		MenuItem getGerenciarCenariosButton();
		MenuItem getGerenciarRequisicoesCenariosButton();
		Button getUsuariosButton();
		MenuItem getListarUsuariosButton();
		MenuItem getUsuariosAlterarSenhaButton();
		MenuItem getUsuariosSairButton();
		MenuItem getUsuariosNomeButton();
	}

	private Display viewport;
	
	private CenarioDTO cenario;
	private InstituicaoEnsinoDTO instituicaoEnsino;
	private UsuarioDTO usuario;
	private Presenter toolBar;

	public AppPresenter( Display viewport )
	{
		this.viewport = viewport;
		addListeners();
	}
	
	private void addListeners() {

		
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
		final FutureResult<Integer> futureDBVersion = new FutureResult<Integer>();

		cenarioService.getCurrentCenario(futureCenarioDTO);
		usuarioService.getCurrentUser(futureUsuarioDTO);
		cenarioService.getInstituicaoEnsinoDTO(futureInstituicaoEnsinoDTO);
		cenarioService.checkDBVersion(futureDBVersion);

		FutureSynchronizer synch = new FutureSynchronizer(futureCenarioDTO,futureUsuarioDTO,futureInstituicaoEnsinoDTO, futureDBVersion);
		synch.addCallback(new AbstractAsyncCallbackWithDefaultOnFailure<Boolean>(viewport) {
			@Override
			public void onSuccess(Boolean result) {
				cenario = futureCenarioDTO.result();
				usuario = futureUsuarioDTO.result();
				instituicaoEnsino =  futureInstituicaoEnsinoDTO.result();
				Integer dbVersion = futureDBVersion.result();
				Integer dbCurrentVersion = Integer.parseInt(viewport.getI18nConstants().dbVersion());
				
				if(dbVersion < dbCurrentVersion)
					MessageBox.alert( "Aviso!", "O banco de dados está na versão " + dbVersion +
							", porém a versão mais recente é " + dbCurrentVersion + "." , null );

				RootPanel rp = (RootPanel) widget;
				toolBar = new ToolBarPresenter(instituicaoEnsino,cenario,usuario,viewport.getCenarioPanel(),new ToolBarView(cenario));
				toolBar.go(viewport.asWidget());
				rp.add(viewport.asWidget());
				RootPanel.get("loading").setVisible(false);

				// Enquanto o browser estiver aberto, a sessão não irá expirar
				createThreadAvoidSessionExpire();
				
				viewport.getUsuariosNomeButton().setText(usuario.getNome());
				viewport.getUsuariosButton().setText(usuario.getUsername());
				
				viewport.getListarUsuariosButton().addSelectionListener(new SelectionListener<MenuEvent>() {
					@Override
					public void componentSelected( MenuEvent ce ) {
					Presenter presenter = new UsuariosPresenter( instituicaoEnsino,
							cenario, new UsuariosView( cenario ) );

					presenter.go( viewport.getGTab() );
					}
				});
				
				viewport.getUsuariosAlterarSenhaButton().addSelectionListener(new SelectionListener<MenuEvent>() {
					@Override
					public void componentSelected( MenuEvent ce ) {
					Presenter presenter = new AlterarSenhaFormPresenter( instituicaoEnsino,
							new AlterarSenhaFormView(usuario) );

					presenter.go( viewport.getGTab() );
					}
				});
				
				viewport.getUsuariosSairButton().addSelectionListener(new SelectionListener<MenuEvent>() {
					@Override
					public void componentSelected( MenuEvent ce ) {
						Window.open("../resources/j_spring_security_logout"+TriedaUtil.paramsDebug(), "_self", "");
					}
				});
				
/*				viewport.getGerenciarCenariosButton().addSelectionListener(new SelectionListener<MenuEvent>() {
					@Override
					public void componentSelected( MenuEvent ce ) {
					Presenter presenter = new CenariosPresenter( instituicaoEnsino,
							viewport.getCenarioPanel(), new CenariosView() );

					presenter.go( viewport.getGTab() );
					}
				});*/
				
				/*viewport.getGerenciarRequisicoesCenariosButton().addSelectionListener(new SelectionListener<MenuEvent>() {
					@Override
					public void componentSelected( MenuEvent ce ) {
						CenariosServiceAsync service = Services.cenarios();
						
						service.setCurrentCenario(9L, new AsyncCallback<Void>(){

							@Override
							public void onFailure(Throwable caught) {
								MessageBox.alert( "ERRO!",
										"Deu falha na conexão", null );
							}

							@Override
							public void onSuccess(Void result) {
								Services.cenarios().getCurrentCenario(new AsyncCallback<CenarioDTO>(){

									@Override
									public void onFailure(Throwable caught) {
										MessageBox.alert( "ERRO!",
												"Deu falha na conexão", null );
										
									}

									@Override
									public void onSuccess(CenarioDTO result) {
										cenario = result;
										((ToolBarPresenter) toolBar).changeCenario(result);
										MessageBox.alert( "sucesso!",
												"Mudou Cenario para " + result.getId(), null );
									}
									
								});
							}
							
						});
					}
				});*/
			}
		});
	}
}
