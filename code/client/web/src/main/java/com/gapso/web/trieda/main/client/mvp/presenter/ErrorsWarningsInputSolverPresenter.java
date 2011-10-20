package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.IconButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.button.ToolButton;
import com.gapso.web.trieda.main.client.mvp.view.OtimizarMessagesView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.OtimizarServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.Timer;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.AbstractImagePrototype;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class ErrorsWarningsInputSolverPresenter
	implements Presenter
{
	public interface Display
	{
		ContentPanel getMessagesWarningPanel();
		ContentPanel getMessagesErrorPanel();
		SimpleModal getSimpleModal();
		Button getSubmitButton();
		Button getCancelButton();
		ToolButton getCloseButton();
	}

	private CenarioDTO cenarioDTO;
	private ParametroDTO parametroDTO;
	private List< String > warnings;
	private List< String > errors;
	private Display display;
	private Boolean validInput;
	private Button submitButtonParametros;

	public ErrorsWarningsInputSolverPresenter( Boolean validInput,
		CenarioDTO cenarioDTO, ParametroDTO parametroDTO,
		List< String > errors, List< String > warnings,
		Display display, Button submitButtonParametros )
	{
		this.validInput = validInput;
		this.cenarioDTO = cenarioDTO;
		this.parametroDTO = parametroDTO;
		this.display = display;
		this.errors = errors;
		this.warnings = warnings;
		this.submitButtonParametros = submitButtonParametros;

		initUI();
		setListeners();
	}

	private void initUI()
	{
		for ( String msg : this.warnings )
		{
			this.display.getMessagesWarningPanel().addText( "• " + msg );
		}

		for ( String msg : this.errors )
		{
			this.display.getMessagesErrorPanel().addText( "• " + msg );
		}

		this.display.getMessagesWarningPanel().setVisible( !this.warnings.isEmpty() );
		this.display.getMessagesErrorPanel().setVisible( !this.errors.isEmpty() );

		this.display.getSubmitButton().setEnabled(
			this.validInput == null ? false : this.validInput );
	}

	private void setListeners()
	{
		this.display.getSubmitButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				desabilitaBotaoParametros();
				final OtimizarServiceAsync service = Services.otimizar();

				service.sendInput( parametroDTO, new AsyncCallback< Long >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!",
							"Não foi possível gerar a grade.", null );

						display.getSimpleModal().hide();
						habilitarBotaoParametros();
					}

					@Override
					public void onSuccess( final Long round )
					{
						Info.display( "Otimizando",
							"Otimizando com sucesso!" );

						checkSolver( round );
						display.getSimpleModal().hide();
					}
				});
			}
		});

		this.display.getCancelButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				habilitarBotaoParametros();
			}
		});

		// TODO
		if ( this.display.getCloseButton() != null )
		{
			this.display.getCloseButton().addSelectionListener(
				new SelectionListener< IconButtonEvent >()
			{
				@Override
				public void componentSelected( IconButtonEvent ce )
				{
					habilitarBotaoParametros();
				}
			});
		}
	}

	private void checkSolver( final Long round )
	{
		final Timer t = new Timer()
		{
			@Override
			public void run()
			{
				final OtimizarServiceAsync service = Services.otimizar();
				final FutureResult< Boolean > futureBoolean = new FutureResult< Boolean >();

				service.isOptimizing( round, futureBoolean );
				FutureSynchronizer synch = new FutureSynchronizer( futureBoolean );

				synch.addCallback( new AsyncCallback< Boolean >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!",
							"Impossível de verificar, servidor fora do ar", null );
					}

					@Override
					public void onSuccess( Boolean result )
					{
						if ( futureBoolean.result() )
						{
							checkSolver( round );
						}
						else
						{
							Info.display( "OTIMIZADO",
								"Otimização finalizada!" );

							atualizaSaida( round );
							habilitarBotaoParametros();
						}
					}
				});
			}
		};

		t.schedule( 5 * 1000 );
	}

	private void atualizaSaida( final Long round )
	{
		final OtimizarServiceAsync service = Services.otimizar();
		final FutureResult< Map< String, List< String > > > futureBoolean
			= new FutureResult< Map< String, List< String > > >();

		service.saveContent( cenarioDTO, round, futureBoolean );
		FutureSynchronizer synch = new FutureSynchronizer( futureBoolean );

		synch.addCallback( new AsyncCallback< Boolean >()
		{
			@Override
			public void onFailure( Throwable caught )
			{
				MessageBox.alert( "ERRO!",
					"Erro ao ler a saída do solver", null );
			}

			@Override
			public void onSuccess( Boolean result )
			{
				Map< String, List< String > > ret = futureBoolean.result();

				if ( ret.get( "warning" ).isEmpty()
					&& ret.get( "error" ).isEmpty() )
				{
					MessageBox.alert( "OTIMIZADO",
						"Saída salva com sucesso", null );
				}
				else
				{
					Presenter presenter = new OtimizarMessagesPresenter(
						ret.get( "warning" ), ret.get( "error" ), new OtimizarMessagesView() );

					presenter.go( null );
				}
			}
		});
	}

	private void desabilitaBotaoParametros()
	{
		this.submitButtonParametros.setIcon(
			AbstractImagePrototype.create( Resources.DEFAULTS.ajax16() ) );

		this.submitButtonParametros.disable();
	}

	private void habilitarBotaoParametros()
	{
		this.submitButtonParametros.enable();

		this.submitButtonParametros.setIcon(
			AbstractImagePrototype.create( Resources.DEFAULTS.gerarGrade16() ) );
	}

	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
