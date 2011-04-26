package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.Style.Scroll;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Label;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FieldSet;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.Radio;
import com.extjs.gxt.ui.client.widget.form.RadioGroup;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.ParametrosPresenter;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CargaHorariaComboBox;
import com.gapso.web.trieda.shared.util.view.FuncaoObjetivoComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class ParametrosView extends MyComposite implements ParametrosPresenter.Display {

	private Button submitBt;
	private ContentPanel form;
	private GTabItem tabItem;
	private ParametroDTO parametroDTO;
	
	private Radio taticoRadio;
	private Radio operacionalRadio;
	private TurnoComboBox turnoComboBox;
	private CampusComboBox campusComboBox;
	
	private CheckBox cargaHorariaAlunoCheckBox;
	private CargaHorariaComboBox cargaHorariaAlunoComboBox;
	private CheckBox alunoDePeriodoMesmaSalaCheckBox;
	private CheckBox alunoEmMuitosCampiCheckBox;
	private CheckBox minimizarDeslocamentoAlunoCheckBox;

	private CheckBox cargaHorariaProfessorCheckBox;
	private CargaHorariaComboBox cargaHorariaProfessorComboBox;
	private CheckBox professorEmMuitosCampiCheckBox;
	private CheckBox minimizarDeslocamentoProfessorCheckBox;
	private NumberField minimizarDeslocamentoProfessorNumberField;
	private CheckBox minimizarGapProfessorCheckBox;
	private CheckBox evitarReducaoCargaHorariaProfessorCheckBox;
	private NumberField evitarReducaoCargaHorariaProfessorNumberField;
	private CheckBox evitarUltimoEPrimeiroHorarioProfessorCheckBox;
	private CheckBox preferenciaDeProfessoresCheckBox;
	private CheckBox avaliacaoDesempenhoProfessorCheckBox;

	private FuncaoObjetivoComboBox funcaoObjetivoCheckBox;
	private CheckBox considerarEquivalenciaCheckBox;
	private CheckBox nivelDificuldadeDisciplinaCheckBox;
	private CheckBox compatibilidadeDisciplinasMesmoDiaCheckBox;
	private CheckBox regrasGenericasDivisaoCreditoCheckBox;
	private CheckBox regrasEspecificasDivisaoCreditoCheckBox;
	private CheckBox maximizarNotaAvaliacaoCorpoDocenteCheckBox;
	private CheckBox minimizarCustoDocenteCursosCheckBox;
	private CheckBox minAlunosParaAbrirTurmaCheckBox;
	private NumberField minAlunosParaAbrirTurmaValueNumberField;
	private CheckBox compartilharDisciplinasCampiCheckBox;
	private CheckBox percentuaisMinimosMestresCheckBox;
	private CheckBox percentuaisMinimosDoutoresCheckBox;
	private CheckBox areaTitulacaoProfessoresECursosCheckBox;
	private CheckBox limitarMaximoDisciplinaProfessorCheckBox;
	
	private Button maximizarNotaAvaliacaoCorpoDocenteButton; 
	private Button minimizarCustoDocenteCursosButton; 
	private Button compartilharDisciplinasCampiButton; 
	
	public ParametrosView(ParametroDTO parametroDTO) {
		this.parametroDTO = parametroDTO;
		initUI();
	}
	
	private void initUI() {
		form = new FormPanel();
		form.setHeading("Master Data » Parâmetros de Planejamento");
		createForm();
		createTabItem();
		initComponent(tabItem);
	}

	private void createTabItem() {
		tabItem = new GTabItem("Parâmetros de Planejamento", Resources.DEFAULTS.parametroPlanejamento16());
		tabItem.setContent(form, new Margins(5, 5, 0, 5));
	}

	private void createForm() {
		FormData formData = new FormData("100%");
		form.setScrollMode(Scroll.AUTO);
//		form.setHeaderVisible(false);
//		form.setBodyBorder(false);
		form.setButtonAlign(HorizontalAlignment.RIGHT);
		
		FormLayout formLayout = null;
		RowLayout rowLayout = null;
		Label label = null;
		
		formLayout = new FormLayout();
		formLayout.setLabelWidth(120);
		form.setLayout(formLayout);
		
		RadioGroup group = new RadioGroup();  
		group.setFieldLabel("Modo de Otimização");
		taticoRadio = new Radio();  
		taticoRadio.setBoxLabel("Tático");
		taticoRadio.setValue(parametroDTO.isTatico() || !parametroDTO.isOperacional());
		group.add(taticoRadio);  
		operacionalRadio = new Radio();  
		operacionalRadio.setBoxLabel("Operacional");
		operacionalRadio.setValue(parametroDTO.isOperacional());
		group.add(operacionalRadio);  
		form.add(group);
		
		funcaoObjetivoCheckBox = new FuncaoObjetivoComboBox();
		funcaoObjetivoCheckBox.setValue(parametroDTO.getFuncaoObjetivo());
		funcaoObjetivoCheckBox.setFieldLabel("Função Objetivo");
		form.add(funcaoObjetivoCheckBox);
		
		campusComboBox = new CampusComboBox();
		// TODO campusComboBox.setValue(parametroDTO.getCampusId());
		campusComboBox.setFieldLabel("Campus");
		form.add(campusComboBox);
		
		turnoComboBox = new TurnoComboBox(campusComboBox);
		// TODO turnoComboBox.setValue(parametroDTO.getTurnoId());
		turnoComboBox.setFieldLabel("Turno");
		form.add(turnoComboBox);
		
		FieldSet alunoFS = new FieldSet();
		alunoFS.setHeading("Preferências do Aluno");
		alunoFS.setCollapsible(true);
		alunoFS.setLayout(new ColumnLayout());
		formLayout = new FormLayout();
		formLayout.setExtraStyle("borderBottom");
		LayoutContainer alunoLeft = new LayoutContainer(formLayout);
		rowLayout = new RowLayout();
		rowLayout.setExtraStyle("x-form-item borderBottom");
		LayoutContainer alunoRight = new LayoutContainer(rowLayout);
		
		cargaHorariaAlunoCheckBox = createCheckBox("Distribuição da carga horária semanal do aluno", parametroDTO.getCargaHorariaAluno());
		alunoDePeriodoMesmaSalaCheckBox = createCheckBox("Manter alunos do mesmo curso-período na mesma sala", parametroDTO.getAlunoDePeriodoMesmaSala());
		alunoEmMuitosCampiCheckBox = createCheckBox("Permitir que o aluno estude em mais de um Campus", parametroDTO.getAlunoEmMuitosCampi());
		minimizarDeslocamentoAlunoCheckBox = createCheckBox("Minimizar Deslocamento de Alunos entre Campi", parametroDTO.getMinimizarDeslocamentoAluno());
		
		alunoLeft.add(cargaHorariaAlunoCheckBox, formData);
		alunoLeft.add(alunoDePeriodoMesmaSalaCheckBox, formData);
		alunoLeft.add(alunoEmMuitosCampiCheckBox, formData);
		alunoLeft.add(minimizarDeslocamentoAlunoCheckBox, formData);
		
		cargaHorariaAlunoComboBox = createComboBox(parametroDTO.getCargaHorariaAlunoSel());
		alunoRight.add(cargaHorariaAlunoComboBox);
		
		for(int i = 0; i<3; i++) {
			label = new Label();
			label.setHeight(22);
			alunoRight.add(label);
		}
		alunoFS.add(alunoLeft, new ColumnData(.5));
		alunoFS.add(alunoRight, new ColumnData(.4));
		
		FieldSet professorFS = new FieldSet();
		professorFS.setHeading("Preferências do Professor");
		professorFS.setCollapsible(true);
		professorFS.setLayout(new ColumnLayout());
		formLayout = new FormLayout();
		formLayout.setExtraStyle("borderBottom");
		LayoutContainer professorLeft = new LayoutContainer(formLayout);
		rowLayout = new RowLayout();
		rowLayout.setExtraStyle("x-form-item borderBottom");
		LayoutContainer professorRight = new LayoutContainer(rowLayout);
		
		cargaHorariaProfessorCheckBox = createCheckBox("Distribuição da carga horária semanal do professor", parametroDTO.getCargaHorariaProfessor());
		professorEmMuitosCampiCheckBox = createCheckBox("Permitir que o professor ministre aulas em mais de um Campus", parametroDTO.getProfessorEmMuitosCampi());
		minimizarDeslocamentoProfessorCheckBox = createCheckBox("Minimizar Deslocamentos de Professores entre Campi", parametroDTO.getMinimizarDeslocamentoProfessor());
		minimizarGapProfessorCheckBox = createCheckBox("Minimizar Gaps nos Horários dos Professores", parametroDTO.getMinimizarGapProfessor());
		evitarReducaoCargaHorariaProfessorCheckBox = createCheckBox("Evitar Redução de Carga Horária de Professor", parametroDTO.getEvitarReducaoCargaHorariaProfessor());
		evitarUltimoEPrimeiroHorarioProfessorCheckBox = createCheckBox("Evitar alocação de professores no último horário do dia e primeiro do dia seguinte", parametroDTO.getEvitarUltimoEPrimeiroHorarioProfessor());
		preferenciaDeProfessoresCheckBox = createCheckBox("Considerar preferência de professores por disciplinas", parametroDTO.getPreferenciaDeProfessores());
		avaliacaoDesempenhoProfessorCheckBox = createCheckBox("Considerar avaliação de desempenho de professores", parametroDTO.getAvaliacaoDesempenhoProfessor());
		
		professorLeft.add(cargaHorariaProfessorCheckBox, formData);
		professorLeft.add(professorEmMuitosCampiCheckBox, formData);
		professorLeft.add(minimizarDeslocamentoProfessorCheckBox, formData);
		professorLeft.add(minimizarGapProfessorCheckBox, formData);
		professorLeft.add(evitarReducaoCargaHorariaProfessorCheckBox, formData);
		professorLeft.add(evitarUltimoEPrimeiroHorarioProfessorCheckBox, formData);
		professorLeft.add(preferenciaDeProfessoresCheckBox, formData);
		professorLeft.add(avaliacaoDesempenhoProfessorCheckBox, formData);
		
		cargaHorariaProfessorComboBox = createComboBox(parametroDTO.getCargaHorariaProfessorSel());
		professorRight.add(cargaHorariaProfessorComboBox);
		label = new Label();
		label.setHeight(22);
		professorRight.add(label);
		minimizarDeslocamentoProfessorNumberField = new NumberField();
		minimizarDeslocamentoProfessorNumberField.setEmptyText("Configurar Máx de deslocamento");
		minimizarDeslocamentoProfessorNumberField.setValue(parametroDTO.getMinimizarDeslocamentoProfessorValue());
		professorRight.add(minimizarDeslocamentoProfessorNumberField);
		label = new Label();
		label.setHeight(22);
		professorRight.add(label);
		evitarReducaoCargaHorariaProfessorNumberField = new NumberField();
		evitarReducaoCargaHorariaProfessorNumberField.setEmptyText("Configurar % de tolerância");
		evitarReducaoCargaHorariaProfessorNumberField.setValue(parametroDTO.getEvitarReducaoCargaHorariaProfessorValue());
		professorRight.add(evitarReducaoCargaHorariaProfessorNumberField);
		
		for(int i = 0; i<3; i++) {
			label = new Label();
			label.setHeight(22);
			professorRight.add(label);
		}
		professorFS.add(professorLeft, new ColumnData(.5));
		professorFS.add(professorRight, new ColumnData(.4));
		
		FieldSet instituicaoFS = new FieldSet();
		instituicaoFS.setHeading("Preferências da Instituição");
		instituicaoFS.setCollapsible(true);
		instituicaoFS.setLayout(new ColumnLayout());
		formLayout = new FormLayout();
		formLayout.setExtraStyle("borderBottom");
		LayoutContainer instituicaoLeft = new LayoutContainer(formLayout);
		rowLayout = new RowLayout();
		rowLayout.setExtraStyle("x-form-item borderBottom");
		LayoutContainer instituicaoRight = new LayoutContainer(rowLayout);
		
		considerarEquivalenciaCheckBox = createCheckBox("Considerar Equivalências entre Disciplinas", parametroDTO.getConsiderarEquivalencia());
		minAlunosParaAbrirTurmaCheckBox = createCheckBox("Número mínimo de alunos para abertura de turma", parametroDTO.getMinAlunosParaAbrirTurma());
		nivelDificuldadeDisciplinaCheckBox = createCheckBox("Considerar nível de dificuldade de disciplinas", parametroDTO.getNivelDificuldadeDisciplina());
		compatibilidadeDisciplinasMesmoDiaCheckBox = createCheckBox("Considerar compatibilidade de disciplinas no mesmo dia", parametroDTO.getCompatibilidadeDisciplinasMesmoDia());
		regrasGenericasDivisaoCreditoCheckBox = createCheckBox("Considerar regras genéricas de divisão de créditos", parametroDTO.getRegrasGenericasDivisaoCredito());
		regrasEspecificasDivisaoCreditoCheckBox = createCheckBox("Considerar regras específicas de divisão de créditos", parametroDTO.getRegrasEspecificasDivisaoCredito());
		maximizarNotaAvaliacaoCorpoDocenteCheckBox = createCheckBox("Maximizar nota da avaliação do corpo docente de cursos específicos", parametroDTO.getMaximizarNotaAvaliacaoCorpoDocente());
		minimizarCustoDocenteCursosCheckBox = createCheckBox("Minimizar custo docente de cursos específicos", parametroDTO.getMinimizarCustoDocenteCursos());
		compartilharDisciplinasCampiCheckBox = createCheckBox("Permitir compartilhamento de disciplinas entre cursos", parametroDTO.getCompartilharDisciplinasCampi());
		percentuaisMinimosMestresCheckBox = createCheckBox("Considerar percentuais mínimos de mestres", parametroDTO.getPercentuaisMinimosMestres());
		percentuaisMinimosDoutoresCheckBox = createCheckBox("Considerar percentuais mínimos de doutores", parametroDTO.getPercentuaisMinimosDoutores());
		areaTitulacaoProfessoresECursosCheckBox = createCheckBox("Considerar áreas de titulação dos professores e cursos", parametroDTO.getAreaTitulacaoProfessoresECursos());
		limitarMaximoDisciplinaProfessorCheckBox = createCheckBox("Limitar máximo de disciplinas que um professor pode ministrar por curso", parametroDTO.getLimitarMaximoDisciplinaProfessor());
		
		instituicaoLeft.add(considerarEquivalenciaCheckBox, formData);
		instituicaoLeft.add(minAlunosParaAbrirTurmaCheckBox, formData);
		instituicaoLeft.add(nivelDificuldadeDisciplinaCheckBox, formData);
		instituicaoLeft.add(compatibilidadeDisciplinasMesmoDiaCheckBox, formData);
		instituicaoLeft.add(regrasGenericasDivisaoCreditoCheckBox, formData);
		instituicaoLeft.add(regrasEspecificasDivisaoCreditoCheckBox, formData);
		instituicaoLeft.add(maximizarNotaAvaliacaoCorpoDocenteCheckBox, formData);
		instituicaoLeft.add(minimizarCustoDocenteCursosCheckBox, formData);
		instituicaoLeft.add(compartilharDisciplinasCampiCheckBox, formData);
		instituicaoLeft.add(percentuaisMinimosMestresCheckBox, formData);
		instituicaoLeft.add(percentuaisMinimosDoutoresCheckBox, formData);
		instituicaoLeft.add(areaTitulacaoProfessoresECursosCheckBox, formData);
		instituicaoLeft.add(limitarMaximoDisciplinaProfessorCheckBox, formData);
		
		minAlunosParaAbrirTurmaValueNumberField = new NumberField();
		minAlunosParaAbrirTurmaValueNumberField.setEmptyText("Quantidade mínimo");
		minAlunosParaAbrirTurmaValueNumberField.setValue(parametroDTO.getMinAlunosParaAbrirTurmaValue());
		label = new Label();
		label.setHeight(22);
		instituicaoRight.add(label);
		instituicaoRight.add(minAlunosParaAbrirTurmaValueNumberField);
		instituicaoRight.add(createButton("Configurar níveis de dificuldade"));
		instituicaoRight.add(createButton("Configurar compatibilidades"));
		instituicaoRight.add(createButton("Configurar regras genéricas"));
		label = new Label();
		label.setHeight(22);
		instituicaoRight.add(label);
		maximizarNotaAvaliacaoCorpoDocenteButton = createButton("Configurar cursos");
		minimizarCustoDocenteCursosButton = createButton("Configurar cursos");
		compartilharDisciplinasCampiButton = createButton("Cursos que não compartilham");
		instituicaoRight.add(maximizarNotaAvaliacaoCorpoDocenteButton);
		instituicaoRight.add(minimizarCustoDocenteCursosButton);
		instituicaoRight.add(compartilharDisciplinasCampiButton);
		
		for(int i = 0; i<4; i++) {
			label = new Label();
			label.setHeight(22);
			instituicaoRight.add(label);
		}
		instituicaoFS.add(instituicaoLeft, new ColumnData(.5));
		instituicaoFS.add(instituicaoRight, new ColumnData(.4));
		
		form.add(alunoFS);
		form.add(professorFS);
		form.add(instituicaoFS);
		
		submitBt = new Button("Gerar Grade", AbstractImagePrototype.create(Resources.DEFAULTS.gerarGrade16()));
		form.addButton(submitBt);
		
	}
	
	private CheckBox createCheckBox(String label, boolean value) {
		CheckBox cb = new CheckBox();
		cb.setHideLabel(true);
		cb.setBoxLabel(label);
		cb.setValue(value);
		return cb;
	}
	
	private CargaHorariaComboBox createComboBox(String value) {
		CargaHorariaComboBox cb = new CargaHorariaComboBox();
		cb.setValue(value);
		cb.setWidth(200);
		return cb;
	}
	
	private Button createButton(String text) {
		Button bt = new Button(text);
		return bt;
	}
	
	@Override
	public ParametroDTO getParametroDTO() {
		return parametroDTO;
	}
	
	@Override
	public CheckBox getCargaHorariaAlunoCheckBox() {
		return cargaHorariaAlunoCheckBox;
	}

	@Override
	public CargaHorariaComboBox getCargaHorariaAlunoComboBox() {
		return cargaHorariaAlunoComboBox;
	}

	@Override
	public CheckBox getAlunoDePeriodoMesmaSalaCheckBox() {
		return alunoDePeriodoMesmaSalaCheckBox;
	}

	@Override
	public CheckBox getAlunoEmMuitosCampiCheckBox() {
		return alunoEmMuitosCampiCheckBox;
	}

	@Override
	public CheckBox getMinimizarDeslocamentoAlunoCheckBox() {
		return minimizarDeslocamentoAlunoCheckBox;
	}

	@Override
	public CheckBox getCargaHorariaProfessorCheckBox() {
		return cargaHorariaProfessorCheckBox;
	}

	@Override
	public CargaHorariaComboBox getCargaHorariaProfessorComboBox() {
		return cargaHorariaProfessorComboBox;
	}

	@Override
	public CheckBox getProfessorEmMuitosCampiCheckBox() {
		return professorEmMuitosCampiCheckBox;
	}

	@Override
	public CheckBox getMinimizarDeslocamentoProfessorCheckBox() {
		return minimizarDeslocamentoProfessorCheckBox;
	}

	@Override
	public NumberField getMinimizarDeslocamentoProfessorNumberField() {
		return minimizarDeslocamentoProfessorNumberField;
	}

	@Override
	public CheckBox getMinimizarGapProfessorCheckBox() {
		return minimizarGapProfessorCheckBox;
	}

	@Override
	public CheckBox getEvitarReducaoCargaHorariaProfessorCheckBox() {
		return evitarReducaoCargaHorariaProfessorCheckBox;
	}

	@Override
	public NumberField getEvitarReducaoCargaHorariaProfessorNumberField() {
		return evitarReducaoCargaHorariaProfessorNumberField;
	}

	@Override
	public CheckBox getEvitarUltimoEPrimeiroHorarioProfessorCheckBox() {
		return evitarUltimoEPrimeiroHorarioProfessorCheckBox;
	}

	@Override
	public CheckBox getPreferenciaDeProfessoresCheckBox() {
		return preferenciaDeProfessoresCheckBox;
	}

	@Override
	public CheckBox getAvaliacaoDesempenhoProfessorCheckBox() {
		return avaliacaoDesempenhoProfessorCheckBox;
	}

	@Override
	public CheckBox getNivelDificuldadeDisciplinaCheckBox() {
		return nivelDificuldadeDisciplinaCheckBox;
	}

	@Override
	public CheckBox getCompatibilidadeDisciplinasMesmoDiaCheckBox() {
		return compatibilidadeDisciplinasMesmoDiaCheckBox;
	}

	@Override
	public CheckBox getRegrasGenericasDivisaoCreditoCheckBox() {
		return regrasGenericasDivisaoCreditoCheckBox;
	}

	@Override
	public CheckBox getRegrasEspecificasDivisaoCreditoCheckBox() {
		return regrasEspecificasDivisaoCreditoCheckBox;
	}

	@Override
	public CheckBox getMaximizarNotaAvaliacaoCorpoDocenteCheckBox() {
		return maximizarNotaAvaliacaoCorpoDocenteCheckBox;
	}

	@Override
	public CheckBox getMinimizarCustoDocenteCursosCheckBox() {
		return minimizarCustoDocenteCursosCheckBox;
	}

	@Override
	public CheckBox getMinAlunosParaAbrirTurmaCheckBox() {
		return minAlunosParaAbrirTurmaCheckBox;
	}

	@Override
	public NumberField getMinAlunosParaAbrirTurmaValueNumberField() {
		return minAlunosParaAbrirTurmaValueNumberField;
	}

	@Override
	public CheckBox getCompartilharDisciplinasCampiCheckBox() {
		return compartilharDisciplinasCampiCheckBox;
	}

	@Override
	public CheckBox getPercentuaisMinimosMestresCheckBox() {
		return percentuaisMinimosMestresCheckBox;
	}

	@Override
	public CheckBox getPercentuaisMinimosDoutoresCheckBox() {
		return percentuaisMinimosDoutoresCheckBox;
	}

	@Override
	public CheckBox getAreaTitulacaoProfessoresECursosCheckBox() {
		return areaTitulacaoProfessoresECursosCheckBox;
	}

	@Override
	public CheckBox getLimitarMaximoDisciplinaProfessorCheckBox() {
		return limitarMaximoDisciplinaProfessorCheckBox;
	}

	@Override
	public Button getSubmitButton() {
		return submitBt;
	}

	@Override
	public Radio getTaticoRadio() {
		return taticoRadio;
	}

	@Override
	public Radio getOperacionalRadio() {
		return operacionalRadio;
	}

	@Override
	public Button getMaximizarNotaAvaliacaoCorpoDocenteButton() {
		return maximizarNotaAvaliacaoCorpoDocenteButton;
	}

	@Override
	public Button getMinimizarCustoDocenteCursosButton() {
		return minimizarCustoDocenteCursosButton;
	}

	@Override
	public Button getCompartilharDisciplinasCampiButton() {
		return compartilharDisciplinasCampiButton;
	}

	@Override
	public FuncaoObjetivoComboBox getFuncaoObjetivoComboBox() {
		return funcaoObjetivoCheckBox;
	}

	@Override
	public TurnoComboBox getTurnoComboBox() {
		return turnoComboBox;
	}

	@Override
	public CampusComboBox getCampusComboBox() {
		return campusComboBox;
	}

	@Override
	public CheckBox getConsiderarEquivalenciaCheckBox() {
		return considerarEquivalenciaCheckBox;
	}
	
}
