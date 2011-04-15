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
import com.gapso.web.trieda.main.client.mvp.presenter.RelatorioVisaoProfessorPresenter;
import com.gapso.web.trieda.main.client.util.resources.Resources;
import com.gapso.web.trieda.main.client.util.view.CampusComboBox;
import com.gapso.web.trieda.main.client.util.view.GTabItem;
import com.gapso.web.trieda.main.client.util.view.GradeHorariaProfessorGrid;
import com.gapso.web.trieda.main.client.util.view.ProfessorComboBox;
import com.gapso.web.trieda.main.client.util.view.TurnoComboBox;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class RelatorioVisaoProfessorView extends MyComposite implements RelatorioVisaoProfessorPresenter.Display {

	private GradeHorariaProfessorGrid grid;
	private Button submitBt;
	private CampusComboBox campusCB;
	private TurnoComboBox turnoCB;
	private ProfessorComboBox professorCB;
	private ContentPanel panel;
	private GTabItem tabItem;
	
	public RelatorioVisaoProfessorView() {
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Grade Horária Visão Professor");
		createGrid();
		createFilter();
		createTabItem();
		initComponent(tabItem);
	}

	private void createTabItem() {
		tabItem = new GTabItem("Grade Horária Visão Professor", Resources.DEFAULTS.saidaProfessor16());
		tabItem.setContent(panel);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    grid = new GradeHorariaProfessorGrid();
	    panel.add(grid, bld);
	}

	private void createFilter() {
		FormData formData = new FormData("100%");
		FormPanel panel = new FormPanel();
		panel.setHeaderVisible(true);
		panel.setHeading("Filtro");
		
		LayoutContainer main = new LayoutContainer(new ColumnLayout());
		
		LayoutContainer left = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));
		LayoutContainer right = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));
		
		campusCB = new CampusComboBox();
		left.add(campusCB, formData);
		
		turnoCB = new TurnoComboBox();
		left.add(turnoCB, formData);
		
		professorCB = new ProfessorComboBox();
		right.add(professorCB, formData);
		
		submitBt = new Button("Filtrar", AbstractImagePrototype.create(Resources.DEFAULTS.filter16()));
		panel.addButton(submitBt);
		
		main.add(left, new ColumnData(.5));
		main.add(right, new ColumnData(.5));
		
		panel.add(main, new FormData("100%"));
		
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.NORTH);
		bld.setMargins(new Margins(5, 5, 0, 5));
		bld.setCollapsible(true);
		bld.setSize(130);
		
		this.panel.add(panel, bld);
	}
	
	@Override
	public GradeHorariaProfessorGrid getGrid() {
		return grid;
	}

	@Override
	public Button getSubmitBuscaButton() {
		return submitBt;
	}

	@Override
	public CampusComboBox getCampusComboBox() {
		return campusCB;
	}
	
	@Override
	public TurnoComboBox getTurnoComboBox() {
		return turnoCB;
	}

	@Override
	public ProfessorComboBox getProfessorComboBox() {
		return professorCB;
	}

}
