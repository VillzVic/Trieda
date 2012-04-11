package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.RelatorioVisaoAlunoPresenter;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.AlunosComboBox;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.GradeHorariaAlunoGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class RelatorioVisaoAlunoView extends MyComposite implements RelatorioVisaoAlunoPresenter.Display{
	private SimpleToolBar toolBar;
	private GradeHorariaAlunoGrid grid;
	private Button submitBt;
	private CampusComboBox campusCB;
	private TurnoComboBox turnoCB;
	private AlunosComboBox alunoCB;
	private ContentPanel panel;
	private GTabItem tabItem;

	public RelatorioVisaoAlunoView(){
		this.initUI();
	}

	private void initUI(){
		this.panel = new ContentPanel(new BorderLayout());
		this.panel.setHeading("Master Data » Grade Horária Visão Aluno");

		this.createToolBar();
		this.createGrid();
		this.createFilter();
		this.createTabItem();
		this.initComponent(this.tabItem);
	}
	
	private void createToolBar(){
		// Exibe apenas o botão 'exportExcel'
		this.toolBar = new SimpleToolBar(false, false, false, false, true, this);

		this.panel.setTopComponent(this.toolBar);
	}

	private void createTabItem(){
		this.tabItem = new GTabItem("Grade Horária Visão Aluno", Resources.DEFAULTS.saidaProfessor16());

		this.tabItem.setContent(this.panel);
	}

	private void createGrid(){
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));

	    this.grid = new GradeHorariaAlunoGrid();
	    this.panel.add(this.grid, bld);
	}

	private void createFilter(){
		FormData formData = new FormData("100%");
		FormPanel panel = new FormPanel();
		panel.setHeaderVisible(true);
		panel.setHeading("Filtro");

		LayoutContainer main = new LayoutContainer(new ColumnLayout());
		LayoutContainer left = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));
		LayoutContainer right = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));

		this.campusCB = new CampusComboBox();
		left.add(this.campusCB, formData);
		
		this.turnoCB = new TurnoComboBox(this.campusCB);
		left.add(this.turnoCB, formData);
		
		this.alunoCB = new AlunosComboBox(this.campusCB);
		right.add(this.alunoCB, formData);

		this.submitBt = new Button("Filtrar", AbstractImagePrototype.create(Resources.DEFAULTS.filter16()));
		panel.addButton(this.submitBt);

		main.add(left, new ColumnData(0.5));
		main.add(right, new ColumnData(0.5));

		panel.add(main, new FormData("100%"));

		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.NORTH);

		bld.setMargins(new Margins(5, 5, 0, 5));
		bld.setCollapsible(true);
		bld.setSize(123);

		this.panel.add(panel, bld);
	}

	@Override
	public GradeHorariaAlunoGrid getGrid(){
		return this.grid;
	}

	@Override
	public Button getSubmitBuscaButton(){
		return this.submitBt;
	}

	@Override
	public CampusComboBox getCampusComboBox(){
		return this.campusCB;
	}

	@Override
	public TurnoComboBox getTurnoComboBox(){
		return this.turnoCB;
	}

	@Override
	public AlunosComboBox getAlunosComboBox(){
		return this.alunoCB;
	}

	@Override
	public Button getExportExcelButton(){
		return this.toolBar.getExportExcelButton();
	}
	
}
