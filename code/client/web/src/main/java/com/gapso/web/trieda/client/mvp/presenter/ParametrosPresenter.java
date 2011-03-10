package com.gapso.web.trieda.client.mvp.presenter;

import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.MessageBoxEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.Radio;
import com.gapso.web.trieda.client.mvp.view.OtimizarMessagesView;
import com.gapso.web.trieda.client.services.OtimizarServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CargaHorariaComboBox;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.google.gwt.user.client.Timer;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.AbstractImagePrototype;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class ParametrosPresenter implements Presenter {

	public interface Display {
		ParametroDTO getParametroDTO();
		Radio getTaticoRadio();
		Radio getOperacionalRadio();
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
		NumberField getMinAlunosParaAbrirTurmaValueNumberField();
		CheckBox getMinAlunosParaAbrirTurmaCheckBox();
		CheckBox getCompartilharDisciplinasCampiCheckBox();
		CheckBox getPercentuaisMinimosMestresCheckBox();
		CheckBox getPercentuaisMinimosDoutoresCheckBox();
		CheckBox getAreaTitulacaoProfessoresECursosCheckBox();
		CheckBox getLimitarMaximoDisciplinaProfessorCheckBox();
		Button getSubmitButton();
		Component getComponent();
	}
	private Display display; 
	private CenarioDTO cenario;
	
	public ParametrosPresenter(CenarioDTO cenario, Display display) {
		this.cenario = cenario;
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSubmitButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				desabilitaBotao();
				MessageBox.confirm("Otimizar?", "Deseja otimizar o cenário?", new Listener<MessageBoxEvent>() {
					@Override
					public void handleEvent(MessageBoxEvent be) {
						if(be.getButtonClicked().getText().equalsIgnoreCase("yes")) {
							OtimizarServiceAsync service = Services.otimizar();
							service.input(getDTO(), new AsyncCallback<Long>() {
								@Override
								public void onFailure(Throwable caught) {
									MessageBox.alert("ERRO!", "Deu falha na conexão", null);
									habilitarBotao();
								}
								@Override
								public void onSuccess(final Long round) {
									Info.display("Otimizando", "Otimizando com sucesso!");
									checkSolver(round);	
								}
							});
						} else {
							habilitarBotao();
						}
					}
				});
			}
		});
	}
	
	private void checkSolver(final Long round) {
		final Timer t = new Timer() {
			@Override
			public void run() {
				final OtimizarServiceAsync otimizarService = Services.otimizar();
				final FutureResult<Boolean> futureBoolean = new FutureResult<Boolean>();
				otimizarService.isOptimizing(round, futureBoolean);
				FutureSynchronizer synch = new FutureSynchronizer(futureBoolean);
				synch.addCallback(new AsyncCallback<Boolean>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Impossível de verificar, servidor fora do ar", null);
						habilitarBotao();
					}
					@Override
					public void onSuccess(Boolean result) {
						if(futureBoolean.result()) {
							checkSolver(round);
						} else {
							Info.display("OTIMIZADO", "Otimização finalizada!");
							atualizaSaida(round);
							habilitarBotao();
						}
					}
				});
			}
		};
		t.schedule(5 * 1000);
	}
	
	private void atualizaSaida(final Long round) {
		final OtimizarServiceAsync otimizarService = Services.otimizar();
		final FutureResult<Map<String, List<String>>> futureBoolean = new FutureResult<Map<String, List<String>>>();
		otimizarService.saveContent(cenario, round, futureBoolean);
		FutureSynchronizer synch = new FutureSynchronizer(futureBoolean);
		synch.addCallback(new AsyncCallback<Boolean>() {
			@Override
			public void onFailure(Throwable caught) {
				MessageBox.alert("ERRO!", "Erro ao pegar a saída", null);
			}
			@Override
			public void onSuccess(Boolean result) {
				Map<String, List<String>> ret = futureBoolean.result();
				if(ret.get("warning").isEmpty() && ret.get("error").isEmpty()) {
					MessageBox.alert("OTIMIZADO", "Saída salva com sucesso", null);
				} else {
					Presenter presenter = new OtimizarMessagesPresenter(ret.get("warning"), ret.get("error"), new OtimizarMessagesView());
					presenter.go(null);
				}
			}
		});
	}
	
	private ParametroDTO getDTO() {
		ParametroDTO dto = display.getParametroDTO();
		
		dto.setModoOtimizacao(display.getTaticoRadio().getValue() ? ParametroDTO.TATICO : ParametroDTO.OPERACIONAL);
		
		dto.setCargaHorariaAluno(display.getCargaHorariaAlunoCheckBox().getValue());
		dto.setCargaHorariaAlunoSel(display.getCargaHorariaAlunoComboBox().getValueString());
		dto.setAlunoDePeriodoMesmaSala(display.getAlunoDePeriodoMesmaSalaCheckBox().getValue());
		dto.setAlunoEmMuitosCampi(display.getAlunoEmMuitosCampiCheckBox().getValue());
		dto.setMinimizarDeslocamentoAluno(display.getMinimizarDeslocamentoAlunoCheckBox().getValue());

		dto.setCargaHorariaProfessor(display.getCargaHorariaProfessorCheckBox().getValue());
		dto.setCargaHorariaProfessorSel(display.getCargaHorariaProfessorComboBox().getValueString());
		dto.setProfessorEmMuitosCampi(display.getProfessorEmMuitosCampiCheckBox().getValue());
		dto.setMinimizarDeslocamentoProfessor(display.getMinimizarDeslocamentoProfessorCheckBox().getValue());
		Number minimizarDeslocamentoProfessorValue = display.getMinimizarDeslocamentoProfessorNumberField().getValue();
		if(minimizarDeslocamentoProfessorValue == null) minimizarDeslocamentoProfessorValue = 0;
		dto.setMinimizarDeslocamentoProfessorValue(minimizarDeslocamentoProfessorValue.intValue());
		dto.setMinimizarGapProfessor(display.getMinimizarGapProfessorCheckBox().getValue());
		dto.setEvitarReducaoCargaHorariaProfessor(display.getEvitarReducaoCargaHorariaProfessorCheckBox().getValue());
		Number evitarReducaoCargaHorariaProfessorValue = display.getEvitarReducaoCargaHorariaProfessorNumberField().getValue();
		if(evitarReducaoCargaHorariaProfessorValue == null) evitarReducaoCargaHorariaProfessorValue = 0;
		dto.setEvitarReducaoCargaHorariaProfessorValue(evitarReducaoCargaHorariaProfessorValue.intValue());
		dto.setEvitarUltimoEPrimeiroHorarioProfessor(display.getEvitarUltimoEPrimeiroHorarioProfessorCheckBox().getValue());
		dto.setPreferenciaDeProfessores(display.getPreferenciaDeProfessoresCheckBox().getValue());
		dto.setAvaliacaoDesempenhoProfessor(display.getAvaliacaoDesempenhoProfessorCheckBox().getValue());

		dto.setNivelDificuldadeDisciplina(display.getNivelDificuldadeDisciplinaCheckBox().getValue());
		dto.setCompatibilidadeDisciplinasMesmoDia(display.getCompatibilidadeDisciplinasMesmoDiaCheckBox().getValue());
		dto.setRegrasGenericasDivisaoCredito(display.getRegrasGenericasDivisaoCreditoCheckBox().getValue());
		dto.setRegrasEspecificasDivisaoCredito(display.getRegrasEspecificasDivisaoCreditoCheckBox().getValue());
		dto.setMaximizarNotaAvaliacaoCorpoDocente(display.getMaximizarNotaAvaliacaoCorpoDocenteCheckBox().getValue());
		dto.setMinimizarCustoDocenteCursos(display.getMinimizarCustoDocenteCursosCheckBox().getValue());
		dto.setMinAlunosParaAbrirTurma(display.getMinAlunosParaAbrirTurmaCheckBox().getValue());
		Number minAlunosParaAbrirTurmaValue = display.getMinAlunosParaAbrirTurmaValueNumberField().getValue();
		if(minAlunosParaAbrirTurmaValue == null) minAlunosParaAbrirTurmaValue = 0;
		dto.setMinAlunosParaAbrirTurmaValue(minAlunosParaAbrirTurmaValue.intValue());
		dto.setCompartilharDisciplinasCampi(display.getCompartilharDisciplinasCampiCheckBox().getValue());
		dto.setPercentuaisMinimosMestres(display.getPercentuaisMinimosMestresCheckBox().getValue());
		dto.setPercentuaisMinimosDoutores(display.getPercentuaisMinimosDoutoresCheckBox().getValue());
		dto.setAreaTitulacaoProfessoresECursos(display.getAreaTitulacaoProfessoresECursosCheckBox().getValue());
		dto.setLimitarMaximoDisciplinaProfessor(display.getLimitarMaximoDisciplinaProfessorCheckBox().getValue());
		
		return dto;
	}
	
	private void desabilitaBotao(){
		display.getSubmitButton().setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.ajax16()));
		display.getSubmitButton().disable();
	}
	private void habilitarBotao() {
		display.getSubmitButton().enable();
		display.getSubmitButton().setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.gerarGrade16()));
	}
	
	@Override
	public void go(Widget widget) {
		GTab tab = (GTab)widget;
		tab.add((GTabItem)display.getComponent());
	}

}
