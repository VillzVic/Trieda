package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.store.TreeStore;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.treepanel.TreePanel;

public class GTreePanel extends ContentPanel {
	
	private TreePanel<ModelData> treePanel;

	public GTreePanel() {
		super(new FitLayout());
		treePanel = new TreePanel<ModelData>(new TreeStore<ModelData>());
		add(treePanel);
		setHeading("Navegação");
	}

}
