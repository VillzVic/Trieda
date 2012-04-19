package com.gapso.web.trieda.shared.util.view;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
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
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.google.gwt.user.client.Element;

public class SemanaLetivaDoCenarioGrid< M extends BaseModel >
	extends ContentPanel
{
	private Grid< M > grid;
	private RpcProxy< PagingLoadResult< M > > proxy;
	private PagingLoader< PagingLoadResult< ModelData > > loader;
	private List< M > horariosDisponiveisDisponivel;
	private Map< String, List< ToggleImageButton > > comboboxWeek;

	private ToggleImageButton segCB;
	private ToggleImageButton terCB;
	private ToggleImageButton quaCB;
	private ToggleImageButton quiCB;
	private ToggleImageButton sexCB;
	private ToggleImageButton sabCB;
	private ToggleImageButton domCB;
	
	private Map<Integer, Boolean> checkHeader;

	private boolean selectDefault = false;
	private String horarioAulaIdPropertyName;
	private String lastTurno = "";

	public SemanaLetivaDoCenarioGrid(
		List< M > horariosDisponiveisDisponivel,
		String horarioAulaIdPropertyName )
	{
		super( new FitLayout() );

		this.horariosDisponiveisDisponivel = horariosDisponiveisDisponivel;
		this.horarioAulaIdPropertyName = horarioAulaIdPropertyName;
		this.checkHeader = new HashMap<Integer, Boolean>();

		setHeaderVisible( false );
		setBodyBorder( false );
		createCheckboxs();
	}

	@Override
	protected void beforeRender()
	{
		super.beforeRender();

		this.loader = new BasePagingLoader< PagingLoadResult< ModelData > >( this.proxy );
		this.loader.setRemoteSort( true );

		ListStore< M > store = new ListStore< M >( this.loader );

		this.grid = new Grid< M >(
			store, new ColumnModel( getColumnList() ) );

		this.grid.setBorders( true );
		this.grid.setTrackMouseOver( false );
		this.grid.addStyleName( "SemanaLetivaGrid" );

		this.grid.getView().setEmptyText(
			"Não existe horários na Semana Letiva" );

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
		createComboboxWeek();

		GridCellRenderer< M > mergeRenderer = new GridCellRenderer< M >()
		{
			@Override
			public Object render( M model, String property, ColumnData config,
				int rowIndex, int colIndex, ListStore< M > store, Grid< M > grid )
			{
				config.style += ( "border-right: 1px solid #EDEDED;" );
				config.style += ( "border-top: 1px solid #FFF;" );

				if ( ( rowIndex + 1 ) == store.getModels().size() )
				{
					config.style += ( "border-bottom: 1px solid #EDEDED;" );
				}

				if ( model.get( property ).equals( lastTurno ) )
				{
					return "";
				}

				lastTurno = model.get( property );
				config.style += ( "border-top: 1px solid #EDEDED;" );
				return lastTurno;
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
			public Object render( final M modelCenario, final String property,
				ColumnData config, int rowIndex, int colIndex,
				ListStore< M > store, Grid< M > grid )
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

				if ( !(Boolean) modelCenario.get( property ) )
				{
					return "";
				}

				M model = getModel( modelCenario );
				Boolean flag = ( ( model == null ) ? false : (Boolean) model.get( property ) );
				modelCenario.set( property, ( model == null ) ? false : model.get( property ) );

				checkColumnUpdate(colIndex, rowIndex, flag);
				if((rowIndex + 1) == store.getModels().size()) checkColumn(colIndex);
				
				if ( isSelectDefault() )
				{
					modelCenario.set( property, true );
					flag = true;
				}

				ToggleImageButton tb = new ToggleImageButton( flag,
					Resources.DEFAULTS.save16(), Resources.DEFAULTS.cancel16() );

				tb.addSelectionListener( new SelectionListener< ButtonEvent >()
				{
					@Override
					public void componentSelected( ButtonEvent ce )
					{
						ToggleImageButton tib = (ToggleImageButton) ce.getComponent();
						modelCenario.set( property, tib.isPressed() );
					}
				});

				comboboxWeek.get( property ).add( tb );
				return tb;
			}  
		};

		List< ColumnConfig > list = new ArrayList< ColumnConfig >( 9 );

		list.add( createColumnConfig( "turnoString", "Turno", 100, mergeRenderer, null ) );
		list.add( createColumnConfig( "horarioString", "Horario", 90, horarioRenderer, null ) );
		list.add( createColumnConfig( "segunda", "Seg", 55, buttonRenderer, this.segCB ) );
		list.add( createColumnConfig( "terca",   "Ter", 55, buttonRenderer, this.terCB ) );
		list.add( createColumnConfig( "quarta",  "Qua", 55, buttonRenderer, this.quaCB ) );
		list.add( createColumnConfig( "quinta",  "Qui", 55, buttonRenderer, this.quiCB ) );
		list.add( createColumnConfig( "sexta",   "Sex", 55, buttonRenderer, this.sexCB ) );
		list.add( createColumnConfig( "sabado",  "Sab", 55, buttonRenderer, this.sabCB ) );
		list.add( createColumnConfig( "domingo", "Dom", 55, buttonRenderer, this.domCB ) );
		
		this.checkHeader.put(2, false);
		this.checkHeader.put(3, false);
		this.checkHeader.put(4, false);
		this.checkHeader.put(4, false);
		this.checkHeader.put(5, false);
		this.checkHeader.put(6, false);
		this.checkHeader.put(7, false);

		return list;
	}

	private void createComboboxWeek()
	{
		this.comboboxWeek = new HashMap<String, List<ToggleImageButton>>();

		this.comboboxWeek.put( "segunda", new ArrayList< ToggleImageButton >() );
		this.comboboxWeek.put( "terca", new ArrayList< ToggleImageButton >() );
		this.comboboxWeek.put( "quarta", new ArrayList< ToggleImageButton >() );
		this.comboboxWeek.put( "quinta", new ArrayList< ToggleImageButton >() );
		this.comboboxWeek.put( "sexta", new ArrayList< ToggleImageButton >() );
		this.comboboxWeek.put( "sabado", new ArrayList< ToggleImageButton >() );
		this.comboboxWeek.put( "domingo", new ArrayList< ToggleImageButton >() );
	}

	private M getModel( M model )
	{
		if ( model != null )
		{
			Long horarioDeAulaId = model.get( this.horarioAulaIdPropertyName );

			for ( M o : this.horariosDisponiveisDisponivel )
			{
				if ( o.get( this.horarioAulaIdPropertyName ).equals( horarioDeAulaId ) )
				{
					return o;
				}
			}
		}

		return null;		
	}

	private ColumnConfig createColumnConfig( String id, String name,
		Integer width, GridCellRenderer< M > gcr, ToggleImageButton checkbox )
	{
		ColumnConfig c = new ColumnConfig( id, name, width );

		c.setResizable( false );
		c.setMenuDisabled( true );
		c.setSortable( false );
		c.setRenderer( gcr );

		if ( checkbox != null )
		{
			c.setAlignment( HorizontalAlignment.LEFT );
			c.setWidget( checkbox, name );
		}

		return c;
	}

	private void createCheckboxs()
	{
		this.segCB = createCheckboxs( "Seg" );
		this.terCB = createCheckboxs( "Ter" );
		this.quaCB = createCheckboxs( "Qua" );
		this.quiCB = createCheckboxs( "Qui" );
		this.sexCB = createCheckboxs( "Sex" );
		this.sabCB = createCheckboxs( "Sab" );
		this.domCB = createCheckboxs( "Dom" );
	}

	private ToggleImageButton createCheckboxs( String text )
	{
		ToggleImageButton cb = new ToggleImageButton( true,
			Resources.DEFAULTS.checkBoxChecked(),
			Resources.DEFAULTS.checkBoxNotChecked() );

		cb.setText( "<span style='padding-left: 15px;'>" + text + "</span>" );
		cb.addSelectionListener( getSelectionListener() );

		return cb;
	}

	private SelectionListener< ButtonEvent > getSelectionListener()
	{
		return new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				ToggleImageButton cb = (ToggleImageButton) ce.getSource();

				if ( cb.equals( segCB ) )
				{
					selectWeek( "segunda", cb.isPressed() );
				}
				else if ( cb.equals( terCB ) )
				{
					selectWeek( "terca", cb.isPressed() );
				}
				else if ( cb.equals( quaCB ) )
				{
					selectWeek( "quarta", cb.isPressed() );
				}
				else if ( cb.equals( quiCB ) )
				{
					selectWeek( "quinta", cb.isPressed() );
				}
				else if ( cb.equals( sexCB ) )
				{
					selectWeek( "sexta", cb.isPressed() );
				}
				else if ( cb.equals( sabCB ) )
				{
					selectWeek( "sabado", cb.isPressed() );
				}
				else if ( cb.equals( domCB ) )
				{
					selectWeek( "domingo", cb.isPressed() );
				}
			}
		};
	}

	public void selectWeek( String week, boolean flag )
	{
		for ( ToggleImageButton tb : this.comboboxWeek.get( week ) )
		{
			tb.toggle( flag );
			tb.fireEvent( Events.Select );
		}
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
		return this.grid.getStore();
	}
	
	public void updateList()
	{
		this.lastTurno = "";
		this.loader.load();
	}

	public boolean isSelectDefault()
	{
		return this.selectDefault;
	}

	public void setSelectDefault( boolean selectDefault )
	{
		this.selectDefault = selectDefault;
	}
	
	private void checkColumnUpdate(int col, int row, boolean value){
		if(row == 0) this.checkHeader.put(col, value);
		else  this.checkHeader.put(col, this.checkHeader.get(col) && value);
	}
	
	private void checkColumn(int col){
		ToggleImageButton[] tibs = {null, null, segCB, terCB, quaCB, quiCB, sexCB, sabCB, domCB};
		ToggleImageButton tib = tibs[col];
		if(this.checkHeader.get(col)){
			tib.toggle(true);
			tib.fireEvent(Events.Select);
		}
	}
	
	public void unCheckAllHeaders(){
		ToggleImageButton[] tibs = {segCB, terCB, quaCB, quiCB, sexCB, sabCB, domCB};
		for(ToggleImageButton tib: Arrays.asList(tibs)){
			tib.toggle(false);
			tib.fireEvent(Events.Select);
		}
	}
}
