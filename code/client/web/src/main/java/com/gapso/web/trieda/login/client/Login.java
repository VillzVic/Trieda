package com.gapso.web.trieda.login.client;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.ComponentEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.util.KeyNav;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.Viewport;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.Method;
import com.extjs.gxt.ui.client.widget.form.LabelField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.CenterLayout;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.services.UsuariosServiceAsync;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.RootPanel;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class Login
	implements EntryPoint
{
	private Viewport viewport;

	public void onModuleLoad()
	{
		redirect();
	}

	public void redirect()
	{
		final FutureResult< UsuarioDTO > futureUsuarioDTO
			= new FutureResult< UsuarioDTO >();

		UsuariosServiceAsync usuarioService = Services.usuarios();
		usuarioService.getCurrentUser( futureUsuarioDTO );

		FutureSynchronizer synch
			= new FutureSynchronizer( futureUsuarioDTO );

		synch.addCallback( new AsyncCallback< Boolean >()
		{
			@Override
			public void onSuccess( Boolean result )
			{
				UsuarioDTO usuario = futureUsuarioDTO.result();

				if ( usuario == null )
				{
					loadLogin();
				}
				else if( usuario.isAdministrador() )
				{
					 Window.open( "../trieda/"
						+ TriedaUtil.paramsDebug(), "_self", "" ); 
				}
				else if ( usuario.isProfessor() )
				{
					 Window.open( "../professor/"
						+ TriedaUtil.paramsDebug(), "_self", "" ); 
				}
			}

			@Override
			public void onFailure( Throwable caught )
			{
				MessageBox.alert( "ERRO!",
					"Não foi possível realizar o login", null );
			}
		});
	}

	private void loadLogin()
	{
		this.viewport = new Viewport();
		this.viewport.setLayout( new FitLayout() );

		ContentPanel panel = new ContentPanel( new CenterLayout() );

		final FormPanel form = new FormPanel()
		{
			@Override
			protected void onRender( Element target, int index )
			{
				super.onRender( target, index );
				getLayoutTarget().dom.setPropertyString( "target", "_self" );
			}
		};

		form.setAction( "../resources/j_spring_security_check" );
		form.setMethod( Method.POST );
		form.setFrame( true );
		form.setHeading( "Área de Acesso" );  
		form.setWidth( 320 );

		FormLayout layout = new FormLayout();  
		layout.setLabelWidth( 0 );  
		form.setLayout( layout );  

		FormData formData = new FormData( "-20" );
		
		LabelField labelLogin = new LabelField();
		labelLogin.setText("Login");
		
		form.add(labelLogin, formData);

		TextField< String > usernameTF = new TextField< String >()
		{
			@Override
			protected void onRender( Element target, int index )
			{
				super.onRender( target, index );
				getInputEl().setElementAttribute( "placeholder", "Nome de usuário" );
				getInputEl().setElementAttribute( "autocomplete", "off" );
			}
		};

		usernameTF.setName( "j_username" );
		usernameTF.setLabelSeparator("");
		usernameTF.setWidth(230);
		usernameTF.focus();
		form.add( usernameTF, formData );

		TextField< String > passwordTF = new TextField< String >(){
			@Override
			protected void onRender( Element target, int index )
			{
				super.onRender( target, index );
				getInputEl().setElementAttribute( "placeholder", "Senha" );
			}
		};  
		passwordTF.setName( "j_password" );
		passwordTF.setLabelSeparator("");
		passwordTF.setWidth(230);
		passwordTF.setPassword( true );
		form.add( passwordTF, formData );

		Button enviarBt = new Button( "OK" );
		enviarBt.setWidth(80);
		enviarBt.addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				form.submit();
			}
		});

		setDefaultButton( form, enviarBt );
		form.addButton( enviarBt );

		panel.add( form );

		this.viewport.add( panel );
		RootPanel.get().add( viewport );
		RootPanel.get( "loading" ).setVisible( false );
	}

	public void setDefaultButton(
		Component comp, final Button button )
	{
		new KeyNav< ComponentEvent >( comp )
		{
			@Override
			public void onEnter( ComponentEvent ce )
			{
				super.onEnter( ce );
				button.fireEvent( Events.Select );
			}
		};
	}
}
