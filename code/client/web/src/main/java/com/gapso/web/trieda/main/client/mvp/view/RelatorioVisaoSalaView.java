package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.RelatorioVisaoSalaPresenter;
import com.gapso.web.trieda.main.client.util.resources.Resources;
import com.gapso.web.trieda.main.client.util.view.CampusComboBox;
import com.gapso.web.trieda.main.client.util.view.GTabItem;
import com.gapso.web.trieda.main.client.util.view.GradeHorariaSalaGrid;
import com.gapso.web.trieda.main.client.util.view.SalaComboBox;
import com.gapso.web.trieda.main.client.util.view.TurnoComboBox;
import com.gapso.web.trieda.main.client.util.view.UnidadeComboBox;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class RelatorioVisaoSalaView extends MyComposite implements RelatorioVisaoSalaPresenter.Display {

	private GradeHorariaSalaGrid grid;
	private Button submitBt;
	private CampusComboBox campusCB;
	private UnidadeComboBox unidadeCB;
	private SalaComboBox salaCB;
	private TurnoComboBox turnoCB;
	private TextField<String> capacidadeTF;
	private TextField<String> tipoSalaTF;
	private ContentPanel panel;
	private GTabItem tabItem;
	
	public RelatorioVisaoSalaView() {
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Grade Horária Visão Sala");
		createGrid();
		createFilter();
		createTabItem();
		initComponent(tabItem);
	}

	private void createTabItem() {
		tabItem = new GTabItem("Grade Horária Visão Sala", Resources.DEFAULTS.saidaSala16());
		tabItem.setContent(panel);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    grid = new GradeHorariaSalaGrid();
	    panel.add(grid, bld);
	}

	private void createFilter() {
		FormData formData = new FormData("100%");
		FormPanel panel = new FormPanel();
		panel.setHeaderVisible(true);
		panel.setHeading("Filtro");
		panel.setButtonAlign(HorizontalAlignment.RIGHT);
		
		LayoutContainer main = new LayoutContainer(new ColumnLayout());
		
		LayoutContainer left = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));
		LayoutContainer right = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));
		LayoutContainer center = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));
		LayoutContainer button = new LayoutContainer(new FormLayout());
		
		campusCB = new CampusComboBox();
		left.add(campusCB, formData);
		
		unidadeCB = new UnidadeComboBox(campusCB);
		left.add(unidadeCB, formData);
		
		salaCB = new SalaComboBox(unidadeCB);
		center.add(salaCB, formData);
		
		turnoCB = new TurnoComboBox();
		center.add(turnoCB, formData);
		
		capacidadeTF = new TextField<String>();
		capacidadeTF.setFieldLabel("Capacidade");
		capacidadeTF.setReadOnly(true);
		right.add(capacidadeTF, formData);
		
		tipoSalaTF = new TextField<String>();
		tipoSalaTF.setFieldLabel("Tipo");
		tipoSalaTF.setReadOnly(true);
		right.add(tipoSalaTF, formData);
		
		submitBt = new Button("Filtrar", AbstractImagePrototype.create(Resources.DEFAULTS.filter16()));
		button.setStyleAttribute("paddingLeft", "10px");
		button.setStyleAttribute("paddingTop", "27px");
		button.add(submitBt, formData);
		
		main.add(left, new ColumnData(.3));
		main.add(center, new ColumnData(.3));
		main.add(right, new ColumnData(.3));
		main.add(button, new ColumnData(.1));
		
		panel.add(main, new FormData("100%"));
		
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.NORTH);
		bld.setMargins(new Margins(5, 5, 0, 5));
		bld.setCollapsible(true);
		bld.setSize(95);
		
		this.panel.add(panel, bld);
	}
	
	@Override
	public GradeHorariaSalaGrid getGrid() {
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
	public UnidadeComboBox getUnidadeComboBox() {
		return unidadeCB;
	}

	@Override
	public SalaComboBox getSalaComboBox() {
		return salaCB;
	}

	@Override
	public TurnoComboBox getTurnoComboBox() {
		return turnoCB;
	}

	@Override
	public TextField<String> getCapacidadeTextField() {
		return capacidadeTF;
	}

	@Override
	public TextField<String> getTipoTextField() {
		return tipoSalaTF;
	}

}
