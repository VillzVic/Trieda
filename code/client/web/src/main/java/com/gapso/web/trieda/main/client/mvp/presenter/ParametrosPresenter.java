package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.BaseEvent;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.MessageBoxEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Label;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.Radio;
import com.gapso.web.trieda.main.client.mvp.presenter.SelecionarCampiPresenter.ParametrosViewGateway;
import com.gapso.web.trieda.main.client.mvp.view.CompartilharCursosView;
import com.gapso.web.trieda.main.client.mvp.view.ErrorsWarningsInputSolverView;
import com.gapso.web.trieda.main.client.mvp.view.SelecionarCampiView;
import com.gapso.web.trieda.main.client.mvp.view.SelecionarCursosView;
import com.gapso.web.trieda.main.client.mvp.view.SelecionarTurnosView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDescompartilhaDTO;
import com.gapso.web.trieda.shared.dtos.ErrorsWarningsInputSolverDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.OtimizarServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.CargaHorariaComboBox;
import com.gapso.web.trieda.shared.util.view.FuncaoObjetivoComboBox;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.ui.AbstractImagePrototype;
import com.google.gwt.user.client.ui.Widget;

public class ParametrosPresenter extends AbstractRequisicaoOtimizacaoPresenter {
	public interface Display extends ITriedaI18nGateway {
		ParametroDTO getParametroDTO();
		Radio getTaticoRadio();
		Radio getOperacionalRadio();
		Radio getOtimizarPorAlunoRadio();
		Radio getOtimizarPorBlocoRadio();
		Label getCampiLabel();
		Button getSelecionarCampiButton();
		Label getTurnosLabel();
		Button getSelecionarTurnosButton();
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
		CheckBox getProibeCiclosEmEquivalenciaCheckBox();
		CheckBox getConsideraTransitividadeEmEquivalenciaCheckBox();
		CheckBox getProibeTrocaPorDisciplinasOnlineOuSemCreditosEmEquivalenciaCheckBox();
		NumberField getMinAlunosParaAbrirTurmaValueNumberField();
		CheckBox getMinAlunosParaAbrirTurmaCheckBox();
		CheckBox getViolarMinAlunosAbrirTurmaParaFormandosCheckBoxCheckBox();
		CheckBox getCompartilharDisciplinasCampiCheckBox();
		CheckBox getPercentuaisMinimosMestresCheckBox();
		CheckBox getPercentuaisMinimosDoutoresCheckBox();
		CheckBox getAreaTitulacaoProfessoresECursosCheckBox();
		CheckBox getLimitarMaximoDisciplinaProfessorCheckBox();
		CheckBox getConsiderarDemandasPrioridade2CheckBox();
		CheckBox getConsiderarCapacidadeMaximaDasSalasCheckBox();
		FuncaoObjetivoComboBox getFuncaoObjetivoComboBox();
		Button getMaximizarNotaAvaliacaoCorpoDocenteButton();
		Button getMinimizarCustoDocenteCursosButton();
		Button getCompartilharDisciplinasCampiButton();
		Button getSubmitButton();
		Component getComponent();
	}

	private Display display; 
	private CenarioDTO cenarioDTO;
	private ParametrosViewGateway parametrosViewGateway;

	public ParametrosPresenter(CenarioDTO cenarioDTO, Display display, ParametrosViewGateway parametrosViewGateway) {
		super(display);
		this.cenarioDTO = cenarioDTO;
		this.display = display;
		this.parametrosViewGateway = parametrosViewGateway;

		preencheInfoDeCampi_e_Turno();
		setListeners();
	}

	private void preencheInfoDeCampi_e_Turno() {
		final OtimizarServiceAsync service = Services.otimizar(); 

		service.getParametrosDaRequisicaoDeOtimizacao(this.cenarioDTO,new AbstractAsyncCallbackWithDefaultOnFailure<ParametroDTO>(this.display) {
			@Override
			public void onSuccess(ParametroDTO parametroDTO) {
				if (!parametroDTO.isValid()) {
					return;
				}
				
				display.getCampiLabel().setHtml(parametroDTO.getCampi().size() + " camp(us)(i) selecionado(s).");
				display.getTurnosLabel().setHtml(parametroDTO.getTurnos().size() + " turno(s) selecionado(s).");
			}
		});
	}
	
	@Override
	protected void enviaRequisicaoOtimizacaoOnFailure() {
		habilitarBotao();
	}

	@Override
	protected void enviaRequisicaoOtimizacaoOnSuccess() {
		habilitarBotao();
	}

	@Override
	protected void otimizacaoFinalizada() {}

	private void setListeners() {
		this.display.getSubmitButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				desabilitaBotao();
				final OtimizarServiceAsync service = Services.otimizar();

				MessageBox.confirm("Otimizar?","Deseja otimizar o cenário?",new Listener<MessageBoxEvent>() {
					@Override
					public void handleEvent(MessageBoxEvent be) {
						if (be.getButtonClicked().getHtml().equalsIgnoreCase("yes") ||
								be.getButtonClicked().getHtml().equalsIgnoreCase("sim")) {
							try {
						    	new AcompanhamentoPanelPresenter("chaveOtimizacao", new AcompanhamentoPanelView());
								service.checkInputDataBeforeRequestOptimization(getDTO(),new AbstractAsyncCallbackWithDefaultOnFailure<ErrorsWarningsInputSolverDTO>("Não foi possível gerar a grade de horários.",display) {
									@Override
									public void onFailure(Throwable caught) {
										super.onFailure(caught);
										habilitarBotao();
									}

									@Override
									public void onSuccess(final ErrorsWarningsInputSolverDTO dto) {
										if (dto.getTotalErrorsWarnings() == 0) {
											enviaRequisicaoDeOtimizacao(getDTO(),cenarioDTO);
										} else {
											Presenter presenter = new ErrorsWarningsInputSolverPresenter(cenarioDTO,getDTO(),dto.getErrors(),dto.getWarnings(),new ErrorsWarningsInputSolverView(),display.getSubmitButton());
											presenter.go(null);
										}
									}
								});
							} catch(Exception e) {
								MessageBox.alert("ERRO!","Não foi possível gerar a grade de horários.",null);
								habilitarBotao();
							}
						} else {
							habilitarBotao();
						}
					}
				});
			}
		});

		this.display.getSelecionarCampiButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new SelecionarCampiPresenter(cenarioDTO, display.getParametroDTO(),new SelecionarCampiView(),parametrosViewGateway);
				presenter.go(null);
			}
		});
		
		this.display.getSelecionarTurnosButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new SelecionarTurnosPresenter(cenarioDTO, display.getParametroDTO(),new SelecionarTurnosView(),parametrosViewGateway);
				presenter.go(null);
			}
		});
		
		this.display.getMaximizarNotaAvaliacaoCorpoDocenteButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<CursoDTO> cursos = display.getParametroDTO().getMaximizarNotaAvaliacaoCorpoDocenteList();
				Presenter presenter = new SelecionarCursosPresenter(cursos,new SelecionarCursosView(display.getParametroDTO().getCampi()));
				presenter.go(null);
			}
		});

		this.display.getMinimizarCustoDocenteCursosButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<CursoDTO> cursos = display.getParametroDTO().getMinimizarCustoDocenteCursosList();
				Presenter presenter = new SelecionarCursosPresenter(cursos,new SelecionarCursosView(display.getParametroDTO().getCampi()));
				presenter.go(null);
			}
		});

		this.display.getCompartilharDisciplinasCampiButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Services.cursos().getParesCursosQueNaoPermitemCompartilhamentoDeTurmas(display.getParametroDTO(),new AbstractAsyncCallbackWithDefaultOnFailure<List<CursoDescompartilhaDTO>>(display) {
					@Override
					public void onSuccess(List<CursoDescompartilhaDTO> result) {
						Presenter presenter = new CompartilharCursosPresenter(new CompartilharCursosView(cenarioDTO, display.getParametroDTO(),result));
						presenter.go(null);
					}
				});
			}
		});
		
		this.display.getConsiderarEquivalenciaCheckBox().addListener(Events.Change,new Listener<BaseEvent>() {
			@Override
			public void handleEvent(BaseEvent be) {
				boolean value = ((CheckBox)be.getSource()).getValue();
				if (value) {
					display.getProibeCiclosEmEquivalenciaCheckBox().setEnabled(!display.getConsideraTransitividadeEmEquivalenciaCheckBox().getValue());
				} else {
					display.getProibeCiclosEmEquivalenciaCheckBox().setEnabled(false);
				}
				display.getConsideraTransitividadeEmEquivalenciaCheckBox().setEnabled(value);
				display.getProibeTrocaPorDisciplinasOnlineOuSemCreditosEmEquivalenciaCheckBox().setEnabled(value);
			}
		});
		
		this.display.getConsideraTransitividadeEmEquivalenciaCheckBox().addListener(Events.Change,new Listener<BaseEvent>() {
			@Override
			public void handleEvent(BaseEvent be) {
				boolean value = ((CheckBox)be.getSource()).getValue();
				if (value) {
					// se o parâmetro de considerar transitividade em regras de equivalência estiver ligado, então, é necessário
					// proibir ciclos, do contrário, os algoritmos que checam inconsistências em regras de equivalências podem entrar
					// em loop infinito
					display.getProibeCiclosEmEquivalenciaCheckBox().setValue(true);
					display.getProibeCiclosEmEquivalenciaCheckBox().setEnabled(false);
				} else {
					display.getProibeCiclosEmEquivalenciaCheckBox().setEnabled(true);
				}
			}
		});
	}

	private ParametroDTO getDTO() {
		ParametroDTO dto = this.display.getParametroDTO();

		dto.setModoOtimizacao(this.display.getTaticoRadio().getValue() ? ParametroDTO.TATICO : ParametroDTO.OPERACIONAL);
		dto.setOtimizarPor(this.display.getOtimizarPorAlunoRadio().getValue() ? ParametroDTO.OTIMIZAR_POR_ALUNO : ParametroDTO.OTIMIZAR_POR_BLOCO);
		dto.setFuncaoObjetivo(this.display.getFuncaoObjetivoComboBox().getValue().getValue().ordinal());

		dto.setCargaHorariaAluno(this.display.getCargaHorariaAlunoCheckBox().getValue());
		dto.setCargaHorariaAlunoSel(this.display.getCargaHorariaAlunoComboBox().getValueString());
		dto.setAlunoDePeriodoMesmaSala(this.display.getAlunoDePeriodoMesmaSalaCheckBox().getValue());
		dto.setAlunoEmMuitosCampi(this.display.getAlunoEmMuitosCampiCheckBox().getValue());
		dto.setMinimizarDeslocamentoAluno(this.display.getMinimizarDeslocamentoAlunoCheckBox().getValue());
		dto.setCargaHorariaProfessor(this.display.getCargaHorariaProfessorCheckBox().getValue());
		dto.setCargaHorariaProfessorSel(this.display.getCargaHorariaProfessorComboBox().getValueString());
		dto.setProfessorEmMuitosCampi(this.display.getProfessorEmMuitosCampiCheckBox().getValue());
		dto.setMinimizarDeslocamentoProfessor(this.display.getMinimizarDeslocamentoProfessorCheckBox().getValue());

		Number minimizarDeslocamentoProfessorValue = this.display.getMinimizarDeslocamentoProfessorNumberField().getValue();
		if (minimizarDeslocamentoProfessorValue == null) {
			minimizarDeslocamentoProfessorValue = 0;
		}
		dto.setMinimizarDeslocamentoProfessorValue(minimizarDeslocamentoProfessorValue.intValue());

		dto.setMinimizarGapProfessor(this.display.getMinimizarGapProfessorCheckBox().getValue());
		dto.setEvitarReducaoCargaHorariaProfessor(this.display.getEvitarReducaoCargaHorariaProfessorCheckBox().getValue());

		Number evitarReducaoCargaHorariaProfessorValue = this.display.getEvitarReducaoCargaHorariaProfessorNumberField().getValue();
		if (evitarReducaoCargaHorariaProfessorValue == null) {
			evitarReducaoCargaHorariaProfessorValue = 0;
		}
		dto.setEvitarReducaoCargaHorariaProfessorValue(evitarReducaoCargaHorariaProfessorValue.intValue());

		dto.setEvitarUltimoEPrimeiroHorarioProfessor(this.display.getEvitarUltimoEPrimeiroHorarioProfessorCheckBox().getValue());
		dto.setPreferenciaDeProfessores(this.display.getPreferenciaDeProfessoresCheckBox().getValue());
		dto.setAvaliacaoDesempenhoProfessor(this.display.getAvaliacaoDesempenhoProfessorCheckBox().getValue());
		dto.setConsiderarEquivalencia(this.display.getConsiderarEquivalenciaCheckBox().getValue());
		dto.setProibirCiclosEmEquivalencia(this.display.getProibeCiclosEmEquivalenciaCheckBox().getValue());
		dto.setConsiderarTransitividadeEmEquivalencia(this.display.getConsideraTransitividadeEmEquivalenciaCheckBox().getValue());
		dto.setProibirTrocaPorDiscOnlineOuCredZeradosEmEquivalencia(this.display.getProibeTrocaPorDisciplinasOnlineOuSemCreditosEmEquivalenciaCheckBox().getValue());
		dto.setNivelDificuldadeDisciplina(this.display.getNivelDificuldadeDisciplinaCheckBox().getValue());
		dto.setCompatibilidadeDisciplinasMesmoDia(this.display.getCompatibilidadeDisciplinasMesmoDiaCheckBox().getValue());
		dto.setRegrasGenericasDivisaoCredito(this.display.getRegrasGenericasDivisaoCreditoCheckBox().getValue());
		dto.setRegrasEspecificasDivisaoCredito(this.display.getRegrasEspecificasDivisaoCreditoCheckBox().getValue());
		dto.setMaximizarNotaAvaliacaoCorpoDocente(this.display.getMaximizarNotaAvaliacaoCorpoDocenteCheckBox().getValue());
		dto.setMinimizarCustoDocenteCursos(this.display.getMinimizarCustoDocenteCursosCheckBox().getValue());
		dto.setMinAlunosParaAbrirTurma(this.display.getMinAlunosParaAbrirTurmaCheckBox().getValue());
		dto.setViolarMinAlunosAbrirTurmaParaFormandos(this.display.getViolarMinAlunosAbrirTurmaParaFormandosCheckBoxCheckBox().getValue());

		Number minAlunosParaAbrirTurmaValue = this.display.getMinAlunosParaAbrirTurmaValueNumberField().getValue();
		if (minAlunosParaAbrirTurmaValue == null) {
			minAlunosParaAbrirTurmaValue = 0;
		}
		dto.setMinAlunosParaAbrirTurmaValue(minAlunosParaAbrirTurmaValue.intValue());

		dto.setCompartilharDisciplinasCampi(this.display.getCompartilharDisciplinasCampiCheckBox().getValue());
		dto.setPercentuaisMinimosMestres(this.display.getPercentuaisMinimosMestresCheckBox().getValue());
		dto.setPercentuaisMinimosDoutores(this.display.getPercentuaisMinimosDoutoresCheckBox().getValue());
		dto.setAreaTitulacaoProfessoresECursos(this.display.getAreaTitulacaoProfessoresECursosCheckBox().getValue());
		dto.setLimitarMaximoDisciplinaProfessor(this.display.getLimitarMaximoDisciplinaProfessorCheckBox().getValue());
		dto.setConsiderarDemandasDePrioridade2(this.display.getConsiderarDemandasPrioridade2CheckBox().getValue());
		
		dto.setConsiderarCapacidadeMaxSalas(this.display.getConsiderarCapacidadeMaximaDasSalasCheckBox().getValue());

		return dto;
	}

	private void desabilitaBotao() {
		this.display.getSubmitButton().setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.ajax16()));
		this.display.getSubmitButton().disable();
	}

	private void habilitarBotao() {
		this.display.getSubmitButton().enable();
		this.display.getSubmitButton().setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.gerarGrade16()));
	}

	@Override
	public void go(Widget widget) {
		GTab tab = (GTab) widget;
		tab.add((GTabItem)this.display.getComponent());
	}
}