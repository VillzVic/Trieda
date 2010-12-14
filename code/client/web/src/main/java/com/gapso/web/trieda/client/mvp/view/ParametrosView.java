package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.form.TextField;
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
	private ContentPanel panel;
	private GTabItem tabItem;
	
	public ParametrosView() {
		initUI();
	}
	
	private void initUI() {
		panel = new FormPanel();
		panel.setHeading("Master Data » Parâmetros de Planejamento");
		createForm();
		createTabItem();
		initComponent(tabItem);
	}

	private void createTabItem() {
		tabItem = new GTabItem("Parâmetros de Planejamento", Resources.DEFAULTS.outros16());
		tabItem.setContent(panel);
	}

	private void createForm() {
		FormData formData = new FormData("100%");
		panel.setHeaderVisible(true);
		panel.setButtonAlign(HorizontalAlignment.RIGHT);
		
		LayoutContainer main = new LayoutContainer();
		main.setLayout(new ColumnLayout());
		
		LayoutContainer left = new LayoutContainer();
		left.setStyleAttribute("paddingRight", "10px");
		FormLayout layout = new FormLayout();
		left.setLayout(layout);
		
		LayoutContainer right = new LayoutContainer();
		right.setStyleAttribute("paddingLeft", "10px");
		layout = new FormLayout();
		right.setLayout(layout);
		
		SimpleComboBox<String> simpleComboBox = new SimpleComboBox<String>();
		simpleComboBox.setFieldLabel("Carga Horária Semanal do Aluno");
		
		TextField<String> tf1 = new TextField<String>();
		tf1.setFieldLabel("Máximo de Deslocamentos por Dia");
		TextField<String> tf2 = new TextField<String>();
		tf2.setFieldLabel("Máximo de Redução Tolerada (%)");
		
		Button bt1 = new Button("Configurar Compatibilidade entre Disciplinas");
		bt1.setWidth("200px");
		
		left.add(simpleComboBox, new FormData("50%"));
		left.add(createCheckBox("Evitar Alteração de Sala para uma mesma Turma"), formData);
		left.add(createCheckBox("Considerar Nível de Dificuldade de Disciplinas"), formData);
		left.add(new Button("Configurar Níveis de Dificuldade de Disciplinas"), new FormData("50%"));
		left.add(createCheckBox("Equilibrar Diversidade de Assuntos em um mesmo Dia"), formData);
		left.add(bt1, new FormData("50%"));
		left.add(createCheckBox("Considerar Regras de Divisão de Créditos"), formData);
		left.add(createCheckBox("Minimizar Deslocamentos de Professores e/ou Alunos"), formData);
		left.add(createCheckBox("Limitar Deslocamentos de Professor entre Campi por Dia"), formData);
		left.add(tf1, new FormData("30%"));
		left.add(createCheckBox("Minimizar Gaps nos Horários dos Professores"), formData);
		left.add(createCheckBox("Minimizar Dias da Semana em que os Professores ministram Aulas"), formData);
		right.add(createCheckBox("Evitar Redução de Carga Horária de Professor"), formData);
		right.add(tf2, new FormData("30%"));
		right.add(createCheckBox("Evitar Alocação Simultânea de Professor no Último Horário do Dia e no Primeiro Horário do Dia Seguinte"), formData);		
		right.add(createCheckBox("Considerar Preferências de Professores por Disciplinas"), formData);
		right.add(createCheckBox("Maximizar Nota de Avaliação do Corpo Docente nos Cursos"), formData);
		right.add(new Button("Selecionar Cursos"), new FormData("50%"));
		right.add(createCheckBox("Minimizar Custo Docente de Cursos"), formData);
		right.add(new Button("Selecionar Cursos"), new FormData("50%"));
		right.add(createCheckBox("Permitir Compartilhamento de Turmas entre Cursos"), formData);
		right.add(new Button("Agrupar Cursos para Compartilhamento de Turmas"), new FormData("50%"));
		right.add(createCheckBox("Considerar Percentual Mínimo de Mestres por Curso"), formData);
		right.add(createCheckBox("Considerar Percentual Mínimo de Doutores por Curso"), formData);
		right.add(createCheckBox("Considerar Áreas de Titulação"), formData);

		main.add(left, new ColumnData(.5));
		main.add(right, new ColumnData(.5));
		
		submitBt = new Button("Gerar Grade", AbstractImagePrototype.create(Resources.SIMPLE_CRUD.filter16()));
		panel.addButton(submitBt);
		
		panel.add(main, new FormData("100%"));
		
	}
	
	private CheckBox createCheckBox(String label) {
		CheckBox cb = new CheckBox();
		cb.setHideLabel(true);
		cb.setBoxLabel(label);
		return cb;
	}

	@Override
	public Button getSubmitButton() {
		return submitBt;
	}
	

}
