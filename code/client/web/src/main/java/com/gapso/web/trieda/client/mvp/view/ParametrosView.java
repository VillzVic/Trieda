package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.Style.Scroll;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Label;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.extjs.gxt.ui.client.widget.form.FieldSet;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.gapso.web.trieda.client.mvp.model.ParametroDTO;
import com.gapso.web.trieda.client.mvp.presenter.ParametrosPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class ParametrosView extends MyComposite implements ParametrosPresenter.Display {

	private Button submitBt;
	private ContentPanel form;
	private GTabItem tabItem;
	private ParametroDTO parametroDTO;
	
	private CheckBox cargaHorariaAlunoCheckBox;
	private ComboBox cargaHorariaAlunoComboBox;
	private CheckBox alunoDePeriodoMesmaSalaCheckBox;
	private CheckBox alunoEmMuitosCampiCheckBox;
	private CheckBox minimizarDeslocamentoAlunoCheckBox;

	private CheckBox cargaHorariaProfessorCheckBox;
	private ComboBox cargaHorariaProfessorComboBox;
	private CheckBox professorEmMuitosCampiCheckBox;
	private CheckBox minimizarDeslocamentoProfessorCheckBox;
	private NumberField minimizarDeslocamentoProfessorNumberField;
	private CheckBox minimizarGapProfessorCheckBox;
	private CheckBox evitarReducaoCargaHorariaProfessorCheckBox;
	private NumberField evitarReducaoCargaHorariaProfessorNumberField;
	private CheckBox editarUltimoEPrimeiroHorarioProfessorCheckBox;
	private CheckBox preferenciaDeProfessoresCheckBox;
	private CheckBox avaliacaoDesempenhoProfessorCheckBox;

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
		tabItem.setContent(form);
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
		
		cargaHorariaAlunoCheckBox = createCheckBox("Distribuição da carga horária semanal do aluno");
		alunoDePeriodoMesmaSalaCheckBox = createCheckBox("Manter alunos do mesmo curso-período na mesma sala");
		alunoEmMuitosCampiCheckBox = createCheckBox("Permitir que o aluno estude em mais de um Campus");
		minimizarDeslocamentoAlunoCheckBox = createCheckBox("Minimizar Deslocamento de Alunos entre Campi");
		
		alunoLeft.add(cargaHorariaAlunoCheckBox, formData);
		alunoLeft.add(alunoDePeriodoMesmaSalaCheckBox, formData);
		alunoLeft.add(alunoEmMuitosCampiCheckBox, formData);
		alunoLeft.add(minimizarDeslocamentoAlunoCheckBox, formData);
		
		alunoRight.add(createComboBox());
		
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
		
		cargaHorariaProfessorCheckBox = createCheckBox("Distribuição da carga horária semanal do professor");
		professorEmMuitosCampiCheckBox = createCheckBox("Permitir que o professor ministre aulas em mais de um Campus");
		minimizarDeslocamentoProfessorCheckBox = createCheckBox("Minimizar Deslocamentos de Professores entre Campi");
		minimizarGapProfessorCheckBox = createCheckBox("Minimizar Gaps nos Horários dos Professores");
		evitarReducaoCargaHorariaProfessorCheckBox = createCheckBox("Evitar Redução de Carga Horária de Professor");
		editarUltimoEPrimeiroHorarioProfessorCheckBox = createCheckBox("Evitar alocação de professores no último horário do dia e primeiro do dia seguinte");
		preferenciaDeProfessoresCheckBox = createCheckBox("Considerar preferência de professores por disciplinas");
		avaliacaoDesempenhoProfessorCheckBox = createCheckBox("Considerar avaliação de desempenho de professores");
		
		professorLeft.add(cargaHorariaProfessorCheckBox, formData);
		professorLeft.add(professorEmMuitosCampiCheckBox, formData);
		professorLeft.add(minimizarDeslocamentoProfessorCheckBox, formData);
		professorLeft.add(minimizarGapProfessorCheckBox, formData);
		professorLeft.add(evitarReducaoCargaHorariaProfessorCheckBox, formData);
		professorLeft.add(editarUltimoEPrimeiroHorarioProfessorCheckBox, formData);
		professorLeft.add(preferenciaDeProfessoresCheckBox, formData);
		professorLeft.add(avaliacaoDesempenhoProfessorCheckBox, formData);
		
		professorRight.add(createComboBox());
		label = new Label();
		label.setHeight(22);
		professorRight.add(label);
		professorRight.add(createButton("Configurar Máx de deslocamento"));
		label = new Label();
		label.setHeight(22);
		professorRight.add(label);
		professorRight.add(createButton("Configurar % de tolerância"));
		
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
		
		minAlunosParaAbrirTurmaCheckBox = createCheckBox("Número mínimo de alunos para abertura de turma");
		nivelDificuldadeDisciplinaCheckBox = createCheckBox("Considerar nível de dificuldade de disciplinas");
		compatibilidadeDisciplinasMesmoDiaCheckBox = createCheckBox("Considerar compatibilidade de disciplinas no mesmo dia");
		regrasGenericasDivisaoCreditoCheckBox = createCheckBox("Considerar regras genéricas de divisão de créditos");
		regrasEspecificasDivisaoCreditoCheckBox = createCheckBox("Considerar regras específicas de divisão de créditos");
		maximizarNotaAvaliacaoCorpoDocenteCheckBox = createCheckBox("Maximizar nota da avaliação do corpo docente de cursos específicos");
		minimizarCustoDocenteCursosCheckBox = createCheckBox("Minimizar custo docente de cursos específicos");
		compartilharDisciplinasCampiCheckBox = createCheckBox("Permitir compartilhamento de disciplinas entre cursos");
		percentuaisMinimosMestresCheckBox = createCheckBox("Considerar percentuais mínimos de mestres");
		percentuaisMinimosDoutoresCheckBox = createCheckBox("Considerar percentuais mínimos de doutores");
		areaTitulacaoProfessoresECursosCheckBox = createCheckBox("Considerar áreas de titulação dos professores e cursos");
		limitarMaximoDisciplinaProfessorCheckBox = createCheckBox("Limitar máximo de disciplinas que um professor pode ministrar por curso");
		
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
		
		instituicaoRight.add(createButton("Configurar níveis de dificuldade"));
		instituicaoRight.add(createButton("Configurar níveis de dificuldade"));
		instituicaoRight.add(createButton("Configurar compatibilidades"));
		instituicaoRight.add(createButton("Configurar regras genéricas"));
		label = new Label();
		label.setHeight(22);
		instituicaoRight.add(label);
		instituicaoRight.add(createButton("Configurar cursos"));
		instituicaoRight.add(createButton("Configurar cursos"));
		instituicaoRight.add(createButton("Configurar cursos"));
		
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
	
	private CheckBox createCheckBox(String label) {
		CheckBox cb = new CheckBox();
		cb.setHideLabel(true);
		cb.setBoxLabel(label);
		return cb;
	}
	
	private SimpleComboBox<String> createComboBox() {
		SimpleComboBox<String> cb = new SimpleComboBox<String>();
		cb.add("Concentrar em poucos dias");
		cb.add("Distribuir em todos os dias");
		cb.add("Indiferente");
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
	public Button getSubmitButton() {
		return submitBt;
	}
	

}
