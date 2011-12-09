package com.gapso.web.trieda.shared.util.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.BaseModel;
import com.extjs.gxt.ui.client.data.BasePagingLoader;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoader;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.GridEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnData;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.extjs.gxt.ui.client.widget.grid.GridSelectionModel;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.google.gwt.user.client.Element;

public class SemanaLetivaGrid< M extends BaseModel >
	extends ContentPanel
{
	private Grid< M > grid;
	private RpcProxy< PagingLoadResult< M > > proxy;
	private PagingLoader< PagingLoadResult< ModelData > > loader;
	private ListStore< M > store;

	public SemanaLetivaGrid()
	{
		super( new FitLayout() );
		setHeaderVisible( false );
	}

	@Override
	protected void beforeRender()
	{
		super.beforeRender();

		this.loader = new BasePagingLoader< PagingLoadResult< ModelData > >( this.proxy );
		this.loader.setRemoteSort( true );

		this.store = new ListStore< M >( this.loader );

		this.grid = new Grid< M >( this.store,
			new ColumnModel( getColumnList() ) );

		this.grid.setBorders( true );
		this.grid.setTrackMouseOver( false );
		this.grid.addStyleName( "SemanaLetivaGrid" );
		this.grid.setBorders( false );

		this.grid.getView().setEmptyText(
			"Não existem horários de aula cadastrados no sistema" );

		this.grid.addListener( Events.BeforeSelect, new Listener< GridEvent< M > >()
		{
			@Override
			public void handleEvent( GridEvent< M > be )
			{
				be.setCancelled( true );
			}
		});

		add( this.grid );
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

	public List< ColumnConfig > getColumnList()
	{
		GridCellRenderer< M > mergeRenderer = new GridCellRenderer< M >()
		{
			private String lastTurno = "";

			@Override
			public Object render( M model, String property, ColumnData config,
				int rowIndex, int colIndex, ListStore< M > store, Grid< M > grid )
			{
				String turnoCelula = "";
				boolean ehUltimaLinhaDaTabela = ( rowIndex + 1 ) == store.getModels().size();
				boolean ehPrimeiraLinhaDeUmTurno = !model.get(property).equals(lastTurno);
				
				config.style += "border-right: 1px solid #EDEDED;";
				config.style += "border-top: 1px solid #FFF;";
				
				if (ehPrimeiraLinhaDeUmTurno) {
					config.style += ( "border-top: 1px solid #EDEDED;" );
					turnoCelula = model.get(property);
					lastTurno = turnoCelula; 
				}
				if (ehUltimaLinhaDaTabela) {
					config.style += ( "border-bottom: 1px solid #EDEDED;" );
					lastTurno = "";
				}
				
				return turnoCelula;
			}  
		};
		
		GridCellRenderer< M > horarioRenderer = new GridCellRenderer< M >()
		{
			@Override
			public Object render( M model, String property, ColumnData config,
				int rowIndex, int colIndex, ListStore< M > store, Grid< M > grid )
			{
				if ( rowIndex == 0 )
				{
					config.style += ( "border-top: 1px solid #EDEDED;" );
				}

				if ( ( rowIndex + 1 ) == store.getModels().size() )
				{
					config.style += ( "border-bottom: 1px solid #EDEDED;" );
				}

				config.style += ( "border-right: 1px solid #EDEDED;" );
				return model.get( property );
			}  
		};

		GridCellRenderer< M > buttonRenderer = new GridCellRenderer< M >()
		{
			@Override
			public Object render( final M model, final String property,
				ColumnData config, int rowIndex, int colIndex,
				ListStore< M > store, Grid< M > grid )
			{
				if ( rowIndex == 0 )
				{
					config.style += ( "border-top: 1px solid #EDEDED;" );
				}
				else if ( ( rowIndex + 1 ) == store.getModels().size() )
				{
					config.style += ( "border-bottom: 1px solid #EDEDED;" );
				}

				config.style += ( "border-right: 1px solid #EDEDED;" );

				ToggleImageButton tb = new ToggleImageButton( (Boolean) model.get( property ),
					Resources.DEFAULTS.save16(), Resources.DEFAULTS.cancel16() );

				tb.addSelectionListener( new SelectionListener< ButtonEvent >()
				{
					@Override
					public void componentSelected( ButtonEvent ce )
					{
						ToggleImageButton tib = (ToggleImageButton) ce.getComponent();
						model.set( property, tib.isPressed() );
					}
				});

				return tb;
			}  
		};
		
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( createColumnConfig( "turnoString", "Turno", 120, mergeRenderer ) );
		list.add( createColumnConfig( "horarioString", "Horario", 110, horarioRenderer ) );
		list.add( createColumnConfig( "segunda", "Seg", 50, buttonRenderer ) );
		list.add( createColumnConfig( "terca", "Ter", 50, buttonRenderer ) );
		list.add( createColumnConfig( "quarta", "Qua", 50, buttonRenderer ) );
		list.add( createColumnConfig( "quinta", "Qui", 50, buttonRenderer ) );
		list.add( createColumnConfig( "sexta", "Sex", 50, buttonRenderer ) );
		list.add( createColumnConfig( "sabado", "Sab", 50, buttonRenderer ) );
		list.add( createColumnConfig( "domingo", "Dom", 50, buttonRenderer ) );

		return list;
	}

	private ColumnConfig createColumnConfig( String id,
		String name, Integer width, GridCellRenderer< M > gcr )
	{
		ColumnConfig c = new ColumnConfig( id, name, width );

		c.setResizable( false );
		c.setMenuDisabled( true );
		c.setSortable( false );
		c.setRenderer( gcr );

		return c;
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

	public RpcProxy< PagingLoadResult< M > > getProxy()
	{
		return this.proxy;
	}

	public ListStore< M > getStore()
	{
		return this.store;
	}
}
