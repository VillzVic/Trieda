package com.gapso.web.trieda.shared.util.relatorioVisao;

import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public abstract class RelatorioVisaoView extends MyComposite implements RelatorioVisaoPresenter.Display{
	private SimpleToolBar toolBar;
	protected GradeHorariaVisao grid;
	private Button submitBt;
	private ContentPanel panel;
	private GTabItem tabItem;
	
	public RelatorioVisaoView(){}

	public RelatorioVisaoView(RelatorioVisaoFiltro filtro){
		this.setFiltro(filtro);
		this.initUI();
	}
	
	protected abstract String getHeadingPanel();
	protected abstract GTabItem createGTabItem();
	protected abstract GradeHorariaVisao createGradeHorariaVisao();

	protected void initUI(){
		this.panel = new ContentPanel(new BorderLayout());
		this.panel.setHeading(this.getHeadingPanel());
		
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
		this.tabItem = this.createGTabItem();

		this.tabItem.setContent(this.panel);
	}

	private void createGrid(){
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));

	    this.grid = this.createGradeHorariaVisao();
	    this.grid.setFiltro(this.getFiltro());
	    
	    this.panel.add(this.grid, bld);
	}

	protected abstract void createFilter();
	
	protected void createFilter(Map<LabelAlign, List<Field<?>>> mapLayout){
		FormData formData = new FormData("100%");
		FormPanel panel = new FormPanel();
		
		panel.setHeaderVisible(true);
		panel.setHeading("Filtro");
		panel.setButtonAlign(HorizontalAlignment.RIGHT);

		final LayoutContainer main = new LayoutContainer(new ColumnLayout());
		LayoutContainer left = new LayoutContainer(new FormLayout(LabelAlign.LEFT));
		LayoutContainer right = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));
		LayoutContainer center = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));
		
		boolean threeColumns = mapLayout.get(LabelAlign.TOP) != null;

		for(Field<?> campo : mapLayout.get(LabelAlign.LEFT)) left.add(campo, formData);
		for(Field<?> campo : mapLayout.get(LabelAlign.RIGHT)) right.add(campo, formData);
		if(threeColumns) for(Field<?> campo : mapLayout.get(LabelAlign.TOP)) center.add(campo, formData);
		
		this.submitBt = new Button("Filtrar", AbstractImagePrototype.create(Resources.DEFAULTS.filter16()));
		panel.addButton(this.submitBt);
		
		main.add(left, new ColumnData(threeColumns ? 0.33 : 0.5));
		if(threeColumns) main.add(center, new ColumnData(0.33));
		main.add(right, new ColumnData(threeColumns ? 0.33 : 0.5));

		panel.add(main, new FormData("100%"));

		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.NORTH);

		bld.setMargins(new Margins(5, 5, 0, 5));
		bld.setCollapsible(true);
//		panel.setAutoHeight(true);
		
		int numRows = mapLayout.get(LabelAlign.RIGHT).size();
		if(numRows < mapLayout.get(LabelAlign.LEFT).size()) numRows = mapLayout.get(LabelAlign.LEFT).size();
		if(threeColumns && numRows < mapLayout.get(LabelAlign.TOP).size()) numRows = mapLayout.get(LabelAlign.TOP).size();
		bld.setSize(83 + numRows * 26);

		this.panel.add(panel, bld);
	}

	@Override
	public GradeHorariaVisao getGrid(){
		return this.grid;
	}

	@Override
	public Button getSubmitBuscaButton(){
		return this.submitBt;
	}

	@Override
	public MenuItem getExportXlsExcelButton(){
		return (MenuItem) this.toolBar.getExportExcelButton().getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getExportXlsxExcelButton(){
		return (MenuItem) this.toolBar.getExportExcelButton().getMenu().getItem(1);
	}
	
	public abstract RelatorioVisaoFiltro getFiltro();
	
	public abstract void setFiltro(RelatorioVisaoFiltro filtro);
	
}

