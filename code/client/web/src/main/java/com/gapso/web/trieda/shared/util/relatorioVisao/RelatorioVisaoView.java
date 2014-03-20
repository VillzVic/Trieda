package com.gapso.web.trieda.shared.util.relatorioVisao;

import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.Style.ButtonArrowAlign;
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
import com.extjs.gxt.ui.client.widget.layout.HBoxLayoutData;
import com.extjs.gxt.ui.client.widget.menu.Menu;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.presenter.RelatorioVisaoAlunoPresenter.Display;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public abstract class RelatorioVisaoView extends MyComposite implements RelatorioVisaoPresenter.Display{
	protected GradeHorariaVisao grid;
	private Button submitBt;
	private ContentPanel panel;
	private GTabItem tabItem;
	private Button exportExcelBt;
	protected CenarioDTO cenarioDTO;
	
	public RelatorioVisaoView(){}

	public RelatorioVisaoView(CenarioDTO cenarioDTO, RelatorioVisaoFiltro filtro){
		this.cenarioDTO = cenarioDTO;
		this.setFiltro(filtro);
		this.initUI();
	}
	
	protected abstract String getHeadingPanel();
	protected abstract GTabItem createGTabItem();
	protected abstract GradeHorariaVisao createGradeHorariaVisao();

	protected void initUI(){
		this.panel = new ContentPanel(new BorderLayout());
		this.panel.setHeadingHtml(this.getHeadingPanel());

		this.createGrid();
		this.createFilter();
		this.createTabItem();
		this.initComponent(this.tabItem);
		this.checkRegistered();
	}
	
	public abstract void checkRegistered();

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
		panel.setHeadingHtml("Filtro");

		final LayoutContainer main = new LayoutContainer(new ColumnLayout());
		LayoutContainer left = new LayoutContainer(new FormLayout(LabelAlign.LEFT));
		LayoutContainer right = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));
		LayoutContainer center = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));
		LayoutContainer submit = new LayoutContainer(new FormLayout());
		LayoutContainer export = new LayoutContainer(new FormLayout());
		
		boolean threeColumns = mapLayout.get(LabelAlign.TOP) != null;

		for(Field<?> campo : mapLayout.get(LabelAlign.LEFT)) left.add(campo, formData);
		for(Field<?> campo : mapLayout.get(LabelAlign.RIGHT)) right.add(campo, formData);
		for(Field<?> campo : mapLayout.get(LabelAlign.TOP)) center.add(campo, formData);
		
		this.submitBt = new Button("Filtrar", AbstractImagePrototype.create(Resources.DEFAULTS.filter16()));
		submit.add(submitBt, new HBoxLayoutData(new Margins(0, 0, 0, 5)));
		
		this.exportExcelBt = new Button("ExportarExcel", AbstractImagePrototype.create(Resources.DEFAULTS.exportar16()));
		this.exportExcelBt.setArrowAlign(ButtonArrowAlign.RIGHT);
		Menu menu = new Menu();
		menu.add(new MenuItem("Exportar como xls"));
		menu.add(new MenuItem("Exportar como xlsx"));
		this.exportExcelBt.setMenu(menu);
		export.add(this.exportExcelBt, new HBoxLayoutData(new Margins(0, 0, 0, 5)));
		
		main.add(left, new ColumnData(0.29));
		main.add(center, new ColumnData(0.29));
		main.add(right, new ColumnData(0.28));
		main.add(submit, new ColumnData(0.05));
		main.add(export, new ColumnData(0.09));

		panel.add(main, new FormData("100%"));

		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.NORTH);

		bld.setMargins(new Margins(5, 5, 0, 5));
		bld.setCollapsible(true);
//		panel.setAutoHeight(true);
		
		int numRows = mapLayout.get(LabelAlign.RIGHT).size();
		if(numRows < mapLayout.get(LabelAlign.LEFT).size()) numRows = mapLayout.get(LabelAlign.LEFT).size();
		if(threeColumns && numRows < mapLayout.get(LabelAlign.TOP).size()) numRows = mapLayout.get(LabelAlign.TOP).size();
		bld.setSize(48 + numRows * 26);

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
	public Button getExportExcelButton(){
		return this.exportExcelBt;
	}
	
	@Override
	public MenuItem getExportXlsExcelButton(){
		return (MenuItem) this.exportExcelBt.getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getExportXlsxExcelButton(){
		return (MenuItem) this.exportExcelBt.getMenu().getItem(1);
	}
	
	public abstract RelatorioVisaoFiltro getFiltro();
	
	public abstract void setFiltro(RelatorioVisaoFiltro filtro);
	
	public abstract void disableView();
		
}

