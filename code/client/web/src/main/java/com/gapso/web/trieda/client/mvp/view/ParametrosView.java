package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.Style.Scroll;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Label;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FieldSet;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.client.mvp.presenter.ParametrosPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class ParametrosView extends MyComposite implements ParametrosPresenter.Display {

	private Button submitBt;
	private ContentPanel form;
	private GTabItem tabItem;
	
	public ParametrosView() {
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
		
		FieldSet alunoFS = new FieldSet();
		alunoFS.setHeading("Preferências do Aluno");
		alunoFS.setCollapsible(true);
		alunoFS.setLayout(new ColumnLayout());
		LayoutContainer alunoLeft = new LayoutContainer(new FormLayout());
		LayoutContainer alunoRight = new LayoutContainer();
		alunoLeft.add(createCheckBox("Distribuição da carga horária semanal do aluno"), formData);
		alunoLeft.add(createCheckBox("Manter alunos do mesmo curso-período na mesma sala"), formData);
		alunoLeft.add(createCheckBox("Permitir que o aluno estude em mais de um Campus"), formData);
		alunoLeft.add(createCheckBox("Minimizar Deslocamento de Alunos entre Campi"), formData);
		alunoRight.add(createComboBox());
		alunoFS.add(alunoLeft, new ColumnData(.5));
		alunoFS.add(alunoRight, new ColumnData(.4));
		
		FieldSet professorFS = new FieldSet();
		professorFS.setHeading("Preferências do Professor");
		professorFS.setCollapsible(true);
		professorFS.setLayout(new ColumnLayout());
		LayoutContainer professorLeft = new LayoutContainer(new FormLayout());
		LayoutContainer professorRight = new LayoutContainer();
		professorLeft.add(createCheckBox("Distribuição da carga horária semanal do professor"), formData);
		professorLeft.add(createCheckBox("Permitir que o professor ministre aulas em mais de um Campus"), formData);
		professorLeft.add(createCheckBox("Minimizar Deslocamentos de Professores entre Campi"), formData);
		professorLeft.add(createCheckBox("Minimizar Gaps nos Horários dos Professores"), formData);
		professorLeft.add(createCheckBox("Evitar Redução de Carga Horária de Professor"), formData);
		professorLeft.add(createCheckBox("Evitar alocação de professores no último horário do dia e primeiro horário do dia seguinte"), formData);
		professorLeft.add(createCheckBox("Considerar preferência de professores por disciplinas"), formData);
		professorLeft.add(createCheckBox("Considerar avaliação de desempenho de professores"), formData);
		professorRight.add(createComboBox());
		professorRight.add(new Label());
		professorRight.add(createButton("Configurar Máx de deslocamento"));
		professorRight.add(new Label());
		professorRight.add(createButton("Configurar X de tolerância"));
		professorFS.add(professorLeft, new ColumnData(.5));
		professorFS.add(professorRight, new ColumnData(.4));
		
		FieldSet instituicaoFS = new FieldSet();
		instituicaoFS.setHeading("Preferências da Instituição");
		instituicaoFS.setCollapsible(true);
		instituicaoFS.setLayout(new ColumnLayout());
		LayoutContainer instituicaoLeft = new LayoutContainer(new FormLayout());
		LayoutContainer instituicaoRight = new LayoutContainer();
		instituicaoLeft.add(createCheckBox("Considerar nível de dificuldade de disciplinas"), formData);
		instituicaoLeft.add(createCheckBox("Considerar compatibilidade de disciplinas no mesmo dia"), formData);
		instituicaoLeft.add(createCheckBox("Considerar regras genéricas de divisão de créditos"), formData);
		instituicaoLeft.add(createCheckBox("Considerar regras específicas de divisão de créditos"), formData);
		instituicaoLeft.add(createCheckBox("Maximizar nota da avaliação do corpo docente de cursos específicos"), formData);
		instituicaoLeft.add(createCheckBox("Minimizar custo docente de cursos específicos"), formData);
		instituicaoLeft.add(createCheckBox("Permitir compartilhamento de disciplinas entre cursos"), formData);
		instituicaoLeft.add(createCheckBox("Considerar percentuais mínimos de mestres"), formData);
		instituicaoLeft.add(createCheckBox("Considerar percentuais mínimos de doutores"), formData);
		instituicaoLeft.add(createCheckBox("Considerar áreas de titulação dos professores e cursos"), formData);
		instituicaoLeft.add(createCheckBox("Limitar máximo de disciplinas que um professor pode ministrar por curso"), formData);
		instituicaoRight.add(createButton("Configurar níveis de dificuldade"));
		instituicaoRight.add(createButton("Configurar compatibilidades"));
		instituicaoRight.add(createButton("Configurar regras genéricas"));
		instituicaoRight.add(new Label());
		instituicaoRight.add(createButton("Configurar cursos"));
		instituicaoRight.add(createButton("Configurar cursos"));
		instituicaoRight.add(createButton("Configurar cursos"));
		instituicaoFS.add(instituicaoLeft, new ColumnData(.5));
		instituicaoFS.add(instituicaoRight, new ColumnData(.4));
		
		form.add(alunoFS);
		form.add(professorFS);
		form.add(instituicaoFS);
		
		submitBt = new Button("Gerar Grade", AbstractImagePrototype.create(Resources.DEFAULTS.gerarGrade16()));
		form.addButton(submitBt);
		
	}
	
	
//	private void createForm() {
//		FormData formData = new FormData("100%");
//		panel.setHeaderVisible(true);
//		panel.setButtonAlign(HorizontalAlignment.RIGHT);
//		
//		LayoutContainer main = new LayoutContainer();
//		main.setLayout(new ColumnLayout());
//		
//		LayoutContainer left = new LayoutContainer();
//		left.setStyleAttribute("paddingRight", "10px");
//		FormLayout layout = new FormLayout();
//		left.setLayout(layout);
//		
//		LayoutContainer right = new LayoutContainer();
//		right.setStyleAttribute("paddingLeft", "10px");
//		layout = new FormLayout();
//		right.setLayout(layout);
//		
//		SimpleComboBox<String> simpleComboBox = new SimpleComboBox<String>();
//		simpleComboBox.setFieldLabel("Carga Horária Semanal do Aluno");
//		
//		TextField<String> tf1 = new TextField<String>();
//		tf1.setFieldLabel("Máximo de Deslocamentos por Dia");
//		TextField<String> tf2 = new TextField<String>();
//		tf2.setFieldLabel("Máximo de Redução Tolerada (%)");
//		
//		Button bt1 = new Button("Configurar Compatibilidade entre Disciplinas");
//		bt1.setWidth("200px");
//		
//		left.add(simpleComboBox, new FormData("50%"));
//		left.add(createCheckBox("Evitar Alteração de Sala para uma mesma Turma"), formData);
//		left.add(createCheckBox("Considerar Nível de Dificuldade de Disciplinas"), formData);
//		left.add(new Button("Configurar Níveis de Dificuldade de Disciplinas"), new FormData("50%"));
//		left.add(createCheckBox("Equilibrar Diversidade de Assuntos em um mesmo Dia"), formData);
//		left.add(bt1, new FormData("50%"));
//		left.add(createCheckBox("Considerar Regras de Divisão de Créditos"), formData);
//		left.add(createCheckBox("Minimizar Deslocamentos de Professores e/ou Alunos"), formData);
//		left.add(createCheckBox("Limitar Deslocamentos de Professor entre Campi por Dia"), formData);
//		left.add(tf1, new FormData("30%"));
//		left.add(createCheckBox("Minimizar Gaps nos Horários dos Professores"), formData);
//		left.add(createCheckBox("Minimizar Dias da Semana em que os Professores ministram Aulas"), formData);
//		right.add(createCheckBox("Evitar Redução de Carga Horária de Professor"), formData);
//		right.add(tf2, new FormData("30%"));
//		right.add(createCheckBox("Evitar Alocação Simultânea de Professor no Último Horário do Dia e no Primeiro Horário do Dia Seguinte"), formData);		
//		right.add(createCheckBox("Considerar Preferências de Professores por Disciplinas"), formData);
//		right.add(createCheckBox("Maximizar Nota de Avaliação do Corpo Docente nos Cursos"), formData);
//		right.add(new Button("Selecionar Cursos"), new FormData("50%"));
//		right.add(createCheckBox("Minimizar Custo Docente de Cursos"), formData);
//		right.add(new Button("Selecionar Cursos"), new FormData("50%"));
//		right.add(createCheckBox("Permitir Compartilhamento de Turmas entre Cursos"), formData);
//		right.add(new Button("Agrupar Cursos para Compartilhamento de Turmas"), new FormData("50%"));
//		right.add(createCheckBox("Considerar Percentual Mínimo de Mestres por Curso"), formData);
//		right.add(createCheckBox("Considerar Percentual Mínimo de Doutores por Curso"), formData);
//		right.add(createCheckBox("Considerar Áreas de Titulação"), formData);
//
//		main.add(left, new ColumnData(.5));
//		main.add(right, new ColumnData(.5));
//		
//		submitBt = new Button("Gerar Grade", AbstractImagePrototype.create(Resources.DEFAULTS.gerarGrade16()));
//		panel.addButton(submitBt);
//		
//		panel.add(main, new FormData("100%"));
//		
//	}
	
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
	public Button getSubmitButton() {
		return submitBt;
	}
	

}
