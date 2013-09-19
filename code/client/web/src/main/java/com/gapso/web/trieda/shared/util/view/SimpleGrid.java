package com.gapso.web.trieda.shared.util.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SelectionMode;
import com.extjs.gxt.ui.client.data.BaseModel;
import com.extjs.gxt.ui.client.data.BasePagingLoader;
import com.extjs.gxt.ui.client.data.LoadEvent;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoader;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.LoadListener;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.ComponentPlugin;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridSelectionModel;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.toolbar.PagingToolBar;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.google.gwt.user.client.Element;

public class SimpleGrid< M extends BaseModel >
	extends ContentPanel
{
	private Grid< M > grid;
	private RpcProxy< PagingLoadResult< M > > proxy;
	private PagingLoader< PagingLoadResult< ModelData > > loader;
	private List< ColumnConfig > columnList;
	private List< ComponentPlugin > plugins = new ArrayList< ComponentPlugin >();
	private ITriedaI18nGateway i18nGateway;
	private SimpleToolBar toolBar;

	public SimpleGrid( List< ColumnConfig > columnList,
		ITriedaI18nGateway i18nGateway, SimpleToolBar toolBar )
	{
		super( new FitLayout() );

		this.columnList = columnList;
		this.i18nGateway = i18nGateway;
		this.toolBar = toolBar;
		setHeaderVisible( false );
	}

	@Override
	protected void beforeRender()
	{
		super.beforeRender();

		this.loader = new BasePagingLoader< PagingLoadResult< ModelData > >( this.proxy );
		this.loader.setRemoteSort( true );

		ListStore< M > store = new ListStore< M >( this.loader );  

		this.grid = new Grid< M >( store, new ColumnModel( this.columnList ) );
		this.grid.setStripeRows( true );
		this.grid.setBorders( true );
		this.grid.getSelectionModel().setSelectionMode( SelectionMode.MULTI );

		for( ComponentPlugin plugin : plugins )
		{
			this.grid.addPlugin(plugin);
		}

		this.grid.setBorders( false );
		pagingPanel();
		addLoadingListener();
		add( this.grid );
	}

	private void addLoadingListener()
	{
		this.loader.addLoadListener( new LoadListener()
		{
			@Override
			public void loaderBeforeLoad( LoadEvent le )
			{
				grid.mask( i18nGateway.getI18nMessages().loading(), "loading" );
			}

			@Override
			public void loaderLoad( LoadEvent le )
			{
				grid.unmask();
				toolBar.activateEmptyState();
			}
		});
		
		this.getGrid().getSelectionModel().addSelectionChangedListener(new SelectionChangedListener<M>() {

			@Override
		    public void selectionChanged(SelectionChangedEvent<M> se) {
		        if(getGrid().getSelectionModel().getSelectedItems().size() == 1) {
		        	toolBar.enableSimpleState();
		        }
		        else if(getGrid().getSelectionModel().getSelectedItems().size() > 1) {
		        	toolBar.enableMultiState();
		        }
		    }
		});
	}

	@Override
	protected void onRender( Element parent, int pos )
	{
		super.onRender( parent, pos );
		this.loader.load();
	}

	public Grid< M > getGrid()
	{
		return this.grid;
	}

	public void updateList()
	{
		this.loader.load();
	}

	public void addPlugin( ComponentPlugin plugin )
	{
		this.plugins.add( plugin );
	}

	private void pagingPanel()
	{
		PagingToolBar paggingToolBar = new PagingToolBar( 25 );
		paggingToolBar.bind( this.loader );
		setBottomComponent( paggingToolBar );
	}

	public GridSelectionModel< M > getSelectionModel()
	{
		return this.grid.getSelectionModel();
	}

	public void setProxy(
		RpcProxy< PagingLoadResult< M > > proxy )
	{
		this.proxy = proxy;
	}
}
