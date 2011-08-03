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
import com.gapso.web.trieda.main.client.mvp.view.OtimizarMessagesView;
import com.gapso.web.trieda.main.client.mvp.view.SelecionarCursosView;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDescompartilhaDTO;
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
	private CenarioDTO cenario;

	public ParametrosPresenter( CenarioDTO cenario, Display display )
	{
		this.cenario = cenario;
		this.display = display;
		selectComboBoxs();
		setListeners();
	}

	private void selectComboBoxs()
	{
		Services.otimizar().getParametro( cenario,
			new AbstractAsyncCallbackWithDefaultOnFailure<ParametroDTO>( display )
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
		display.getSubmitButton().addSelectionListener( new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				desabilitaBotao();
				MessageBox.confirm( "Otimizar?", "Deseja otimizar o cenário?",
					new Listener< MessageBoxEvent >()
					{
						@Override
						public void handleEvent( MessageBoxEvent be )
						{
							if ( be.getButtonClicked().getText().equalsIgnoreCase( "yes" ) )
							{
								Services.otimizar().sendInput( getDTO(),
									new AbstractAsyncCallbackWithDefaultOnFailure< Long >( display )
									{
										@Override
										public void onSuccess( final Long round )
										{
											Info.display( "Otimizando", "Otimizando com sucesso!" );
											checkSolver( round );	
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

		display.getCampusComboBox().addSelectionChangedListener(
			new SelectionChangedListener< CampusDTO >()
			{
				@Override
				public void selectionChanged(SelectionChangedEvent< CampusDTO > se )
				{
					display.getParametroDTO().getMaximizarNotaAvaliacaoCorpoDocenteList().clear();
					display.getParametroDTO().getMinimizarCustoDocenteCursosList().clear();
					display.getParametroDTO().getDescompartilharDisciplinasList().clear();
				}
			});

		display.getMaximizarNotaAvaliacaoCorpoDocenteButton().addSelectionListener(
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

		display.getMinimizarCustoDocenteCursosButton().addSelectionListener(
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

		display.getCompartilharDisciplinasCampiButton().addSelectionListener(
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

	private void checkSolver( final Long round )
	{
		final Timer t = new Timer()
		{
			@Override
			public void run()
			{
				final OtimizarServiceAsync otimizarService = Services.otimizar();
				final FutureResult< Boolean > futureBoolean = new FutureResult< Boolean >();
				otimizarService.isOptimizing( round, futureBoolean );
				FutureSynchronizer synch = new FutureSynchronizer( futureBoolean );

				synch.addCallback( new AsyncCallback< Boolean >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!",
							"Impossível de verificar, servidor fora do ar", null );

						habilitarBotao();
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
							Info.display( "OTIMIZADO", "Otimização finalizada!" );
							atualizaSaida( round );
							habilitarBotao();
						}
					}
				});
			}
		};

		t.schedule( 5 * 1000 );
	}

	private void atualizaSaida( final Long round )
	{
		final OtimizarServiceAsync otimizarService = Services.otimizar();
		final FutureResult< Map< String, List< String > > > futureBoolean
			= new FutureResult< Map< String, List< String > > >();

		otimizarService.saveContent( cenario, round, futureBoolean );
		FutureSynchronizer synch = new FutureSynchronizer( futureBoolean );
		synch.addCallback( new AsyncCallback< Boolean >()
		{
			@Override
			public void onFailure( Throwable caught )
			{
				MessageBox.alert( "ERRO!", "Erro ao pegar a saída", null );
			}

			@Override
			public void onSuccess( Boolean result )
			{
				Map< String, List< String > > ret = futureBoolean.result();

				if ( ret.get( "warning" ).isEmpty()
					&& ret.get( "error" ).isEmpty() )
				{
					MessageBox.alert( "OTIMIZADO", "Saída salva com sucesso", null );
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

	private ParametroDTO getDTO()
	{
		ParametroDTO dto = display.getParametroDTO();

		dto.setModoOtimizacao( display.getTaticoRadio().getValue() ? ParametroDTO.TATICO : ParametroDTO.OPERACIONAL );
		dto.setFuncaoObjetivo( display.getFuncaoObjetivoComboBox().getValue().getValue().ordinal() );

		CampusDTO campusDTO = display.getCampusComboBox().getValue();
		dto.setCampusId( campusDTO.getId() );
		dto.setCampusDisplay( campusDTO.getDisplayText() );

		TurnoDTO turnoDTO = display.getTurnoComboBox().getValue();
		dto.setTurnoId( turnoDTO.getId() );
		dto.setTurnoDisplay( turnoDTO.getDisplayText() );

		dto.setCargaHorariaAluno( display.getCargaHorariaAlunoCheckBox().getValue() );
		dto.setCargaHorariaAlunoSel( display.getCargaHorariaAlunoComboBox().getValueString() );
		dto.setAlunoDePeriodoMesmaSala( display.getAlunoDePeriodoMesmaSalaCheckBox().getValue() );
		dto.setAlunoEmMuitosCampi( display.getAlunoEmMuitosCampiCheckBox().getValue() );
		dto.setMinimizarDeslocamentoAluno( display.getMinimizarDeslocamentoAlunoCheckBox().getValue() );

		dto.setCargaHorariaProfessor( display.getCargaHorariaProfessorCheckBox().getValue() );
		dto.setCargaHorariaProfessorSel( display.getCargaHorariaProfessorComboBox().getValueString() );
		dto.setProfessorEmMuitosCampi( display.getProfessorEmMuitosCampiCheckBox().getValue() );
		dto.setMinimizarDeslocamentoProfessor( display.getMinimizarDeslocamentoProfessorCheckBox().getValue() );

		Number minimizarDeslocamentoProfessorValue = display.getMinimizarDeslocamentoProfessorNumberField().getValue();
		if ( minimizarDeslocamentoProfessorValue == null )
		{
			minimizarDeslocamentoProfessorValue = 0;
		}
		dto.setMinimizarDeslocamentoProfessorValue( minimizarDeslocamentoProfessorValue.intValue() );

		dto.setMinimizarGapProfessor( display.getMinimizarGapProfessorCheckBox().getValue() );
		dto.setEvitarReducaoCargaHorariaProfessor( display.getEvitarReducaoCargaHorariaProfessorCheckBox().getValue() );

		Number evitarReducaoCargaHorariaProfessorValue
			= display.getEvitarReducaoCargaHorariaProfessorNumberField().getValue();
		if ( evitarReducaoCargaHorariaProfessorValue == null )
		{
			evitarReducaoCargaHorariaProfessorValue = 0;
		}
		dto.setEvitarReducaoCargaHorariaProfessorValue( evitarReducaoCargaHorariaProfessorValue.intValue() );

		dto.setEvitarUltimoEPrimeiroHorarioProfessor( display.getEvitarUltimoEPrimeiroHorarioProfessorCheckBox().getValue() );
		dto.setPreferenciaDeProfessores( display.getPreferenciaDeProfessoresCheckBox().getValue() );
		dto.setAvaliacaoDesempenhoProfessor( display.getAvaliacaoDesempenhoProfessorCheckBox().getValue() );
		dto.setConsiderarEquivalencia( display.getConsiderarEquivalenciaCheckBox().getValue() );
		dto.setNivelDificuldadeDisciplina( display.getNivelDificuldadeDisciplinaCheckBox().getValue() );
		dto.setCompatibilidadeDisciplinasMesmoDia( display.getCompatibilidadeDisciplinasMesmoDiaCheckBox().getValue() );
		dto.setRegrasGenericasDivisaoCredito( display.getRegrasGenericasDivisaoCreditoCheckBox().getValue() );
		dto.setRegrasEspecificasDivisaoCredito( display.getRegrasEspecificasDivisaoCreditoCheckBox().getValue() );
		dto.setMaximizarNotaAvaliacaoCorpoDocente( display.getMaximizarNotaAvaliacaoCorpoDocenteCheckBox().getValue() );
		dto.setMinimizarCustoDocenteCursos( display.getMinimizarCustoDocenteCursosCheckBox().getValue() );
		dto.setMinAlunosParaAbrirTurma( display.getMinAlunosParaAbrirTurmaCheckBox().getValue() );

		Number minAlunosParaAbrirTurmaValue
			= display.getMinAlunosParaAbrirTurmaValueNumberField().getValue();
		if ( minAlunosParaAbrirTurmaValue == null )
		{
			minAlunosParaAbrirTurmaValue = 0;
		}
		dto.setMinAlunosParaAbrirTurmaValue( minAlunosParaAbrirTurmaValue.intValue() );

		dto.setCompartilharDisciplinasCampi( display.getCompartilharDisciplinasCampiCheckBox().getValue() );
		dto.setPercentuaisMinimosMestres( display.getPercentuaisMinimosMestresCheckBox().getValue() );
		dto.setPercentuaisMinimosDoutores( display.getPercentuaisMinimosDoutoresCheckBox().getValue() );
		dto.setAreaTitulacaoProfessoresECursos( display.getAreaTitulacaoProfessoresECursosCheckBox().getValue() );
		dto.setLimitarMaximoDisciplinaProfessor( display.getLimitarMaximoDisciplinaProfessorCheckBox().getValue() );

		return dto;
	}

	private void desabilitaBotao()
	{
		display.getSubmitButton().setIcon(
			AbstractImagePrototype.create( Resources.DEFAULTS.ajax16() ) );
		display.getSubmitButton().disable();
	}

	private void habilitarBotao()
	{
		display.getSubmitButton().enable();
		display.getSubmitButton().setIcon(
			AbstractImagePrototype.create( Resources.DEFAULTS.gerarGrade16() ) );
	}

	@Override
	public void go( Widget widget )
	{
		GTab tab = (GTab)widget;
		tab.add( (GTabItem) display.getComponent() );
	}
}

