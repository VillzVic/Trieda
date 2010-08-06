package com.gapso.web.trieda.client.util.view.simplecrud;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;

public class SimpleCrud<M extends ModelData> extends ContentPanel {

	private SimpleCrudGrid<M> grid;
	private SimpleCrudToolBar tb;
	private SimpleCrudFilter filter;
	
	private AbstractCrudModel crudModel;
	
	public SimpleCrud(AbstractCrudModel crudModel) {
		super(new BorderLayout());
		this.crudModel = crudModel;
		configuration();
	}
	
	private void configuration() {
		setHeaderVisible(false);
		createGrid();
		createToolBar();
		createFilter();
	}

	private void createGrid() {
		grid = new SimpleCrudGrid<M>(crudModel);
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
		add(grid, bld);
	}
	
	private void createToolBar() {
		tb = new SimpleCrudToolBar(crudModel);
		setTopComponent(tb);
	}
	
	private void createFilter() {
		filter = new SimpleCrudFilter(crudModel);
		
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.EAST);
		bld.setMargins(new Margins(0, 0, 0, 5));
		bld.setCollapsible(true);
		
		add(filter, bld);
	}
	
}
