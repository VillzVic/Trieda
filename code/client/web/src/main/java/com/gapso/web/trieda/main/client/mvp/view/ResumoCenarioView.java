package com.gapso.web.trieda.main.client.mvp.view;

import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.BaseTreeLoader;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.data.TreeLoader;
import com.extjs.gxt.ui.client.store.TreeStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.treepanel.TreePanel;
import com.gapso.web.trieda.main.client.mvp.presenter.ResumoCenarioPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class ResumoCenarioView extends MyComposite implements ResumoCenarioPresenter.Display {

	private TreeStore<TreeNodeDTO> store;
	private TreePanel<TreeNodeDTO> tree;

	private ContentPanel panel;
	private GTabItem tabItem;
	
	private CenarioDTO cenario;
	
	public ResumoCenarioView(CenarioDTO cenario) {
		this.cenario = cenario;
		initUI();
		configureProxy();
		createGrid();
		createTabItem();
		initComponent(tabItem);
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Resumo do Cenário");
	}

	@Override
	protected void beforeRender() {
		super.beforeRender();
	}

	private void createTabItem() {
		tabItem = new GTabItem("Resumo do Cenário", Resources.DEFAULTS.resumoCenario16());
		tabItem.setContent(panel);
	}
	
	private void createGrid() {
		tree = new TreePanel<TreeNodeDTO>(getStore());
		tree.setDisplayProperty(TreeNodeDTO.PROPERTY_TEXT);
	    
	    ContentPanel contentPanel = new ContentPanel(new FitLayout());
	    contentPanel.setHeaderVisible(false);
	    contentPanel.add(tree);
	    
	    BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5));
	    panel.add(contentPanel, bld);
	}

	@Override
	public void setStore(TreeStore<TreeNodeDTO> store) {
		this.store = store;
	}

	public TreeStore<TreeNodeDTO> getStore() {
		return store;
	}
	
	@Override
	public TreePanel<TreeNodeDTO> getTree() {
		return tree;
	}

	
	
	private void configureProxy() {
		RpcProxy<List<TreeNodeDTO>> proxy = new RpcProxy<List<TreeNodeDTO>>() {
			@Override
			protected void load(Object loadConfig, AsyncCallback<List<TreeNodeDTO>> callback) {
				Services.cenarios().getResumos(cenario, callback);
			}
		};
		
		TreeLoader<TreeNodeDTO> loader = new BaseTreeLoader<TreeNodeDTO>(proxy) {
			@Override
			public boolean hasChildren(TreeNodeDTO parent) {
				return parent.getLeaf();
			}
		};
		
		store = new TreeStore<TreeNodeDTO>(loader);
	}
}
