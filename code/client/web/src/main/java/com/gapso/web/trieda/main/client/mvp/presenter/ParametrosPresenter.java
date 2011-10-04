package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.MessageBoxEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.Radio;
import com.gapso.web.trieda.main.client.mvp.view.CompartilharCursosView;
import com.gapso.web.trieda.main.client.mvp.view.ErrorsWarningsInputSolverView;
import com.gapso.web.trieda.main.client.mvp.view.OtimizarMessagesView;
import com.gapso.web.trieda.main.client.mvp.view.SelecionarCursosView;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDescompartilhaDTO;
import com.gapso.web.trieda.shared.dtos.ErrorsWarningsInputSolverDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CampiServiceAsync;
import com.gapso.web.trieda.shared.services.OtimizarServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.services.TurnosServiceAsync;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CargaHorariaComboBox;
import com.gapso.web.trieda.shared.util.view.FuncaoObjetivoComboBox;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.Timer;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.AbstractImagePrototype;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class ParametrosPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		ParametroDTO getParametroDTO();
		Radio getTaticoRadio();
		Radio getOperacionalRadio();
		TurnoComboBox getTurnoComboBox();
		CampusComboBox getCampusComboBox();
		CheckBox getCargaHorariaAlunoCheckBox();
		CargaHorariaComboBox getCargaHorariaAlunoComboBox();
		CheckBox getAlunoDePeriodoMesmaSalaCheckBox();
		CheckBox getAlunoEmMuitosCampiCheckBox();
		CheckBox getMinimizarDeslocamentoAlunoCheckBox();
		CheckBox getCargaHorariaProfessorCheckBox();
		CargaHorariaComboBox getCargaHorariaProfessorComboBox();
		CheckBox getProfessorEmMuitosCampiCheckBox();
		CheckBox getMinimizarDeslocamentoProfessorCheckBox();
		NumberField getMinimizarDeslocamentoProfessorNumberField();
		CheckBox getMinimizarGapProfessorCheckBox();
		CheckBox getEvitarReducaoCargaHorariaProfessorCheckBox();
		NumberField getEvitarReducaoCargaHorariaProfessorNumberField();
		CheckBox getEvitarUltimoEPrimeiroHorarioProfessorCheckBox();
		CheckBox getPreferenciaDeProfessoresCheckBox();
		CheckBox getAvaliacaoDesempenhoProfessorCheckBox();
		CheckBox getNivelDificuldadeDisciplinaCheckBox();
		CheckBox getCompatibilidadeDisciplinasMesmoDiaCheckBox();
		CheckBox getRegrasGenericasDivisaoCreditoCheckBox();
		CheckBox getRegrasEspecificasDivisaoCreditoCheckBox();
		CheckBox getMaximizarNotaAvaliacaoCorpoDocenteCheckBox();
		CheckBox getMinimizarCustoDocenteCursosCheckBox();
		CheckBox getConsiderarEquivalenciaCheckBox();
		NumberField getMinAlunosParaAbrirTurmaValueNumberField();
		CheckBox getMinAlunosParaAbrirTurmaCheckBox();
		CheckBox getCompartilharDisciplinasCampiCheckBox();
		CheckBox getPercentuaisMinimosMestresCheckBox();
		CheckBox getPercentuaisMinimosDoutoresCheckBox();
		CheckBox getAreaTitulacaoProfessoresECursosCheckBox();
		CheckBox getLimitarMaximoDisciplinaProfessorCheckBox();
		FuncaoObjetivoComboBox getFuncaoObjetivoComboBox();
		Button getMaximizarNotaAvaliacaoCorpoDocenteButton();
		Button getMinimizarCustoDocenteCursosButton();
		Button getCompartilharDisciplinasCampiButton();
		Button getSubmitButton();
		Component getComponent();
	}

	private Display display; 
	private CenarioDTO cenarioDTO;

	public ParametrosPresenter(
		CenarioDTO cenarioDTO, Display display )
	{
		this.cenarioDTO = cenarioDTO;
		this.display = display;

		selectComboBoxs();
		setListeners();
	}

	private void selectComboBoxs()
	{
		final OtimizarServiceAsync service = Services.otimizar(); 

		service.getParametro( this.cenarioDTO,
			new AbstractAsyncCallbackWithDefaultOnFailure< ParametroDTO >( this.display )
		{
			@Override
			public void onSuccess( ParametroDTO parametroDTO )
			{
				if ( TriedaUtil.isBlank( parametroDTO.getCampusId() )
					&& TriedaUtil.isBlank( parametroDTO.getTurnoId() ) )
				{
					return;
				}

				final CampiServiceAsync campiService = Services.campi();
				final TurnosServiceAsync turnosService = Services.turnos();

				final FutureResult< CampusDTO > futureCampusDTO = new FutureResult< CampusDTO >();
				final FutureResult< TurnoDTO > futureTurnoDTO = new FutureResult< TurnoDTO >();

				campiService.getCampus( parametroDTO.getCampusId(), futureCampusDTO );
				turnosService.getTurno( parametroDTO.getTurnoId(), futureTurnoDTO );
				FutureSynchronizer synch = new FutureSynchronizer( futureCampusDTO, futureTurnoDTO );

				synch.addCallback(
					new AbstractAsyncCallbackWithDefaultOnFailure< Boolean >( display )
				{
					@Override
					public void onSuccess( Boolean result )
					{
						CampusDTO campusDTO = futureCampusDTO.result();
						TurnoDTO turnoDTO = futureTurnoDTO.result();

						display.getCampusComboBox().setValue( campusDTO );
						display.getTurnoComboBox().setValue( turnoDTO );
					}
				});
			}
		});
	}

	private void setListeners()
	{
		this.display.getSubmitButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				desabilitaBotao();
				final OtimizarServiceAsync service = Services.otimizar();

				MessageBox.confirm( "Otimizar?", "Deseja otimizar o cenário?",
					new Listener< MessageBoxEvent >()
					{
						@Override
						public void handleEvent( MessageBoxEvent be )
						{
							if ( be.getButtonClicked().getText().equalsIgnoreCase( "yes" ) )
							{
								service.validaInput( getDTO(),
									new AbstractAsyncCallbackWithDefaultOnFailure< ErrorsWarningsInputSolverDTO >( display )
									{
										@Override
										public void onFailure( Throwable caught )
										{
											MessageBox.alert( "ERRO!",
												"Não foi possível gerar a grade.", null );

											habilitarBotao();
										}

										@Override
										public void onSuccess( final ErrorsWarningsInputSolverDTO dto )
										{
											if ( dto.getValidInput() == true
												&& dto.getTotalErrorsWarnings() == 0 )
											{
												service.sendInput( getDTO(),
													new AbstractAsyncCallbackWithDefaultOnFailure< Long >( display )
													{
														@Override
														public void onFailure( Throwable caught )
														{
															MessageBox.alert( "ERRO!",
																"Não foi possível gerar a grade.", null );

															habilitarBotao();
														}

														@Override
														public void onSuccess( final Long round )
														{
															Info.display( "Otimizando",
																"Otimizando com sucesso!" );

															checkSolver( round );
															habilitarBotao();
														}
													});
											}
											else
											{
												List< String > errors = dto.getErrorsWarnings().get( "errors" );
												List< String > warnings = dto.getErrorsWarnings().get( "warnings" );

												Presenter presenter = new ErrorsWarningsInputSolverPresenter(
												 	dto.getValidInput(), cenarioDTO, getDTO(), errors,
												 	warnings, new ErrorsWarningsInputSolverView() );

												presenter.go( null );
												habilitarBotao();
											}
										}
									});
							}
							else
							{
								habilitarBotao();
							}
						}
				});
			}
		});

		this.display.getCampusComboBox().addSelectionChangedListener(
			new SelectionChangedListener< CampusDTO >()
			{
				@Override
				public void selectionChanged( SelectionChangedEvent< CampusDTO > se )
				{
					display.getParametroDTO().getMaximizarNotaAvaliacaoCorpoDocenteList().clear();
					display.getParametroDTO().getMinimizarCustoDocenteCursosList().clear();
					display.getParametroDTO().getDescompartilharDisciplinasList().clear();
				}
			});

		this.display.getMaximizarNotaAvaliacaoCorpoDocenteButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					List< CursoDTO > cursos = display.getParametroDTO().getMaximizarNotaAvaliacaoCorpoDocenteList();

					Presenter presenter = new SelecionarCursosPresenter(
						cursos, new SelecionarCursosView( display.getCampusComboBox().getValue() ) );

					presenter.go( null );
				}
			});

		this.display.getMinimizarCustoDocenteCursosButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					List< CursoDTO > cursos = display.getParametroDTO().getMinimizarCustoDocenteCursosList();

					Presenter presenter = new SelecionarCursosPresenter(
						cursos, new SelecionarCursosView( display.getCampusComboBox().getValue() ) );

					presenter.go( null );
				}
			});

		this.display.getCompartilharDisciplinasCampiButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					List< CursoDescompartilhaDTO > cursos
						= display.getParametroDTO().getDescompartilharDisciplinasList();

					Presenter presenter = new CompartilharCursosPresenter(
						new CompartilharCursosView( display.getParametroDTO(), cursos ) );

					presenter.go( null );
				}
			});
	}

	private ParametroDTO getDTO()
	{
		ParametroDTO dto = this.display.getParametroDTO();

		dto.setModoOtimizacao( this.display.getTaticoRadio().getValue() ?
			ParametroDTO.TATICO : ParametroDTO.OPERACIONAL );
		dto.setFuncaoObjetivo( this.display.getFuncaoObjetivoComboBox().getValue().getValue().ordinal() );

		CampusDTO campusDTO = this.display.getCampusComboBox().getValue();
		dto.setCampusId( campusDTO.getId() );
		dto.setCampusDisplay( campusDTO.getDisplayText() );

		TurnoDTO turnoDTO = this.display.getTurnoComboBox().getValue();
		dto.setTurnoId( turnoDTO.getId() );
		dto.setTurnoDisplay( turnoDTO.getDisplayText() );

		dto.setCargaHorariaAluno( this.display.getCargaHorariaAlunoCheckBox().getValue() );
		dto.setCargaHorariaAlunoSel( this.display.getCargaHorariaAlunoComboBox().getValueString() );
		dto.setAlunoDePeriodoMesmaSala( this.display.getAlunoDePeriodoMesmaSalaCheckBox().getValue() );
		dto.setAlunoEmMuitosCampi( this.display.getAlunoEmMuitosCampiCheckBox().getValue() );
		dto.setMinimizarDeslocamentoAluno( this.display.getMinimizarDeslocamentoAlunoCheckBox().getValue() );

		dto.setCargaHorariaProfessor( this.display.getCargaHorariaProfessorCheckBox().getValue() );
		dto.setCargaHorariaProfessorSel( this.display.getCargaHorariaProfessorComboBox().getValueString() );
		dto.setProfessorEmMuitosCampi( this.display.getProfessorEmMuitosCampiCheckBox().getValue() );
		dto.setMinimizarDeslocamentoProfessor( this.display.getMinimizarDeslocamentoProfessorCheckBox().getValue() );

		Number minimizarDeslocamentoProfessorValue
			= this.display.getMinimizarDeslocamentoProfessorNumberField().getValue();

		if ( minimizarDeslocamentoProfessorValue == null )
		{
			minimizarDeslocamentoProfessorValue = 0;
		}
		dto.setMinimizarDeslocamentoProfessorValue( minimizarDeslocamentoProfessorValue.intValue() );

		dto.setMinimizarGapProfessor( this.display.getMinimizarGapProfessorCheckBox().getValue() );
		dto.setEvitarReducaoCargaHorariaProfessor( this.display.getEvitarReducaoCargaHorariaProfessorCheckBox().getValue() );

		Number evitarReducaoCargaHorariaProfessorValue
			= this.display.getEvitarReducaoCargaHorariaProfessorNumberField().getValue();

		if ( evitarReducaoCargaHorariaProfessorValue == null )
		{
			evitarReducaoCargaHorariaProfessorValue = 0;
		}

		dto.setEvitarReducaoCargaHorariaProfessorValue( evitarReducaoCargaHorariaProfessorValue.intValue() );
		dto.setEvitarUltimoEPrimeiroHorarioProfessor( this.display.getEvitarUltimoEPrimeiroHorarioProfessorCheckBox().getValue() );
		dto.setPreferenciaDeProfessores( this.display.getPreferenciaDeProfessoresCheckBox().getValue() );
		dto.setAvaliacaoDesempenhoProfessor( this.display.getAvaliacaoDesempenhoProfessorCheckBox().getValue() );
		dto.setConsiderarEquivalencia( this.display.getConsiderarEquivalenciaCheckBox().getValue() );
		dto.setNivelDificuldadeDisciplina( this.display.getNivelDificuldadeDisciplinaCheckBox().getValue() );
		dto.setCompatibilidadeDisciplinasMesmoDia( this.display.getCompatibilidadeDisciplinasMesmoDiaCheckBox().getValue() );
		dto.setRegrasGenericasDivisaoCredito( this.display.getRegrasGenericasDivisaoCreditoCheckBox().getValue() );
		dto.setRegrasEspecificasDivisaoCredito( this.display.getRegrasEspecificasDivisaoCreditoCheckBox().getValue() );
		dto.setMaximizarNotaAvaliacaoCorpoDocente( this.display.getMaximizarNotaAvaliacaoCorpoDocenteCheckBox().getValue() );
		dto.setMinimizarCustoDocenteCursos( this.display.getMinimizarCustoDocenteCursosCheckBox().getValue() );
		dto.setMinAlunosParaAbrirTurma( this.display.getMinAlunosParaAbrirTurmaCheckBox().getValue() );

		Number minAlunosParaAbrirTurmaValue
			= this.display.getMinAlunosParaAbrirTurmaValueNumberField().getValue();

		if ( minAlunosParaAbrirTurmaValue == null )
		{
			minAlunosParaAbrirTurmaValue = 0;
		}

		dto.setMinAlunosParaAbrirTurmaValue(
			minAlunosParaAbrirTurmaValue.intValue() );

		dto.setCompartilharDisciplinasCampi( this.display.getCompartilharDisciplinasCampiCheckBox().getValue() );
		dto.setPercentuaisMinimosMestres( this.display.getPercentuaisMinimosMestresCheckBox().getValue() );
		dto.setPercentuaisMinimosDoutores( this.display.getPercentuaisMinimosDoutoresCheckBox().getValue() );
		dto.setAreaTitulacaoProfessoresECursos( this.display.getAreaTitulacaoProfessoresECursosCheckBox().getValue() );
		dto.setLimitarMaximoDisciplinaProfessor( this.display.getLimitarMaximoDisciplinaProfessorCheckBox().getValue() );

		return dto;
	}

	private void desabilitaBotao()
	{
		if ( this.display.getSubmitButton().isEnabled() )
		{
			this.display.getSubmitButton().setIcon(
				AbstractImagePrototype.create( Resources.DEFAULTS.ajax16() ) );

			this.display.getSubmitButton().disable();
		}
	}

	private void habilitarBotao()
	{
		if ( !this.display.getSubmitButton().isEnabled() )
		{
			this.display.getSubmitButton().enable();

			this.display.getSubmitButton().setIcon(
				AbstractImagePrototype.create( Resources.DEFAULTS.gerarGrade16() ) );
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
					"Erro ao pegar a saída", null );
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
						ret.get( "warning" ), ret.get( "error" ),
						new OtimizarMessagesView() );

					presenter.go( null );
				}
			}
		});
	}

	@Override
	public void go( Widget widget )
	{
		GTab tab = (GTab) widget;
		tab.add( (GTabItem) this.display.getComponent() );
	}
}
