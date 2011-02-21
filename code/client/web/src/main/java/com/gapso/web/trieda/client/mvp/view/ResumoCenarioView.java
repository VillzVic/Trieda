package com.gapso.web.trieda.client.mvp.view;

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
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.FileModel;
import com.gapso.web.trieda.client.mvp.presenter.ResumoCenarioPresenter;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class ResumoCenarioView extends MyComposite implements ResumoCenarioPresenter.Display {

	private TreeStore<FileModel> store;
	private TreePanel<FileModel> tree;

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
		tabItem = new GTabItem("Resumo do Cenário", Resources.DEFAULTS.cenario16());
		tabItem.setContent(panel);
	}
	
	private void createGrid() {
		tree = new TreePanel<FileModel>(getStore());
		tree.setDisplayProperty("name");
	    
	    ContentPanel contentPanel = new ContentPanel(new FitLayout());
	    contentPanel.setHeaderVisible(false);
	    contentPanel.add(tree);
	    
	    BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5));
	    panel.add(contentPanel, bld);
	}

	@Override
	public void setStore(TreeStore<FileModel> store) {
		this.store = store;
	}

	public TreeStore<FileModel> getStore() {
		return store;
	}
	
	@Override
	public TreePanel<FileModel> getTree() {
		return tree;
	}

	
	
	private void configureProxy() {
		RpcProxy<List<FileModel>> proxy = new RpcProxy<List<FileModel>>() {
			@Override
			protected void load(Object loadConfig, AsyncCallback<List<FileModel>> callback) {
				Services.cenarios().getResumos(cenario, callback);
			}
		};
		
		TreeLoader<FileModel> loader = new BaseTreeLoader<FileModel>(proxy) {
			@Override
			public boolean hasChildren(FileModel parent) {
				return parent.getFolha();
			}
		};
		
		store = new TreeStore<FileModel>(loader);
	}
}
