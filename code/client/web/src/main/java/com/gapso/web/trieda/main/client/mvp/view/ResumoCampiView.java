package com.gapso.web.trieda.main.client.mvp.view;

import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.BaseTreeLoader;
import com.extjs.gxt.ui.client.data.LoadEvent;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.data.TreeLoader;
import com.extjs.gxt.ui.client.event.LoadListener;
import com.extjs.gxt.ui.client.store.TreeStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.extjs.gxt.ui.client.widget.treepanel.TreePanel;
import com.gapso.web.trieda.main.client.mvp.presenter.ResumoCampiPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class ResumoCampiView extends MyComposite
	implements ResumoCampiPresenter.Display
{
	private TreeStore< TreeNodeDTO > store;
	private TreePanel< TreeNodeDTO > tree;
	private ContentPanel panel;
	private GTabItem tabItem;
	private CenarioDTO cenario;
	private SimpleToolBar toolBar;

	public ResumoCampiView(
		CenarioDTO cenario )
	{
		this.cenario = cenario;
		initUI();
		configureProxy();
		createGrid();
		createTabItem();
		initComponent( tabItem );
	}

	private void initUI()
	{
		panel = new ContentPanel( new BorderLayout() );
		panel.setHeadingHtml( cenario.getNome() + " » Resumo por Campus" );
		toolBar = new SimpleToolBar( false, false, false, false, true, this );
		panel.setTopComponent(toolBar);
	}

	@Override
	protected void beforeRender()
	{
		super.beforeRender();
	}

	private void createTabItem()
	{
		tabItem = new GTabItem(
			"Resumo por Campus", Resources.DEFAULTS.resumoCursos16() );

		tabItem.setContent( panel );
	}

	private void createGrid()
	{
		tree = new TreePanel< TreeNodeDTO >( getStore() );
		tree.setDisplayProperty( TreeNodeDTO.PROPERTY_TEXT );
	    
	    ContentPanel contentPanel = new ContentPanel( new FitLayout() );
	    contentPanel.setHeaderVisible( false );
	    contentPanel.add( tree );

	    BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5 ) );
	    panel.add( contentPanel, bld );
	}

	@Override
	public void setStore( TreeStore< TreeNodeDTO > store )
	{
		this.store = store;
	}

	public TreeStore< TreeNodeDTO > getStore()
	{
		return store;
	}

	@Override
	public TreePanel< TreeNodeDTO > getTree()
	{
		return tree;
	}
	
	@Override
	public MenuItem getExportXlsExcelButton()
	{
		return (MenuItem) this.toolBar.getExportExcelButton().getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getExportXlsxExcelButton()
	{
		return (MenuItem) this.toolBar.getExportExcelButton().getMenu().getItem(1);
	}

	private void configureProxy()
	{
		RpcProxy< List< TreeNodeDTO > > proxy = new RpcProxy< List< TreeNodeDTO > >()
		{
			@Override
			protected void load( Object loadConfig, AsyncCallback< List< TreeNodeDTO > > callback )
			{
				Services.campi().getResumos( cenario, (TreeNodeDTO) loadConfig, callback );
			}
		};

		TreeLoader< TreeNodeDTO > loader = new BaseTreeLoader< TreeNodeDTO >( proxy )
		{
			@Override
			public boolean hasChildren( TreeNodeDTO parent )
			{
				return !parent.getLeaf();
			}
		};
		
		store = new TreeStore< TreeNodeDTO >( loader );
		addLoadingListener();
	}

	private void addLoadingListener()
	{
		store.getLoader().addLoadListener(
			new LoadListener()
			{
				@Override
				public void loaderBeforeLoad( LoadEvent le )
				{
					tree.mask( getI18nMessages().loading(), "loading" );
				}

				@Override
				public void loaderLoad( LoadEvent le )
				{
					tree.unmask();
				}
		});
	}
}
