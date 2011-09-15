package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.Style.SelectionMode;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.GroupingStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridGroupRenderer;
import com.extjs.gxt.ui.client.widget.grid.GroupColumnData;
import com.extjs.gxt.ui.client.widget.grid.GroupSummaryView;
import com.extjs.gxt.ui.client.widget.grid.SummaryColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.SummaryRenderer;
import com.extjs.gxt.ui.client.widget.grid.SummaryType;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.gapso.web.trieda.main.client.mvp.presenter.AlunosDemandaPresenter;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class AlunoDemandaView
	extends MyComposite
	implements AlunosDemandaPresenter.Display
{
	private SimpleToolBar toolBar;
	private Grid< AlunoDemandaDTO > grid;
	private DemandaDTO demandaDTO;
	private ContentPanel panel;
	private GTabItem tabItem;
	private RpcProxy< ListLoadResult< AlunoDemandaDTO > > proxy;

	public AlunoDemandaView( DemandaDTO demandaDTO )
	{
		this.demandaDTO = demandaDTO;

		initUI();
	}
	
	private void initUI()
	{
		panel = new ContentPanel( new BorderLayout() )
		{
			@Override
			protected void beforeRender()
			{
				super.beforeRender();
				createGrid();
			}

			@Override
			protected void afterRender()
			{
				super.afterRender();
				grid.getStore().getLoader().load();
			}
		};

		panel.setHeading( "Master Data » Alunos na Demanda " +
			"(" + demandaDTO.getDisciplinaString() + ")" );

		createToolBar();
		createTabItem();
		initComponent( tabItem );
	}

	private void createTabItem()
	{
		tabItem = new GTabItem( "Alunos",
			Resources.DEFAULTS.disciplina16() );

		tabItem.setContent( panel );
	}

	private void createToolBar()
	{
		toolBar = new SimpleToolBar(
			true, false, true, false, false, this );

		panel.setTopComponent( toolBar );
	}

	private void createGrid()
	{
		BorderLayoutData bld
			= new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );

		BaseListLoader< ListLoadResult< ModelData > > loader
			= new BaseListLoader< ListLoadResult< ModelData > >( proxy );
		loader.setRemoteSort( true );

		GroupingStore< AlunoDemandaDTO > store
			= new GroupingStore< AlunoDemandaDTO >( loader );
		store.groupBy( AlunoDemandaDTO.PROPERTY_DEMANDA_ID );

		final ColumnModel columnModel = new ColumnModel( getColumnList() );
		grid = new Grid< AlunoDemandaDTO >( store, columnModel );

		GroupSummaryView view = new GroupSummaryView();  
		view.setShowGroupedColumn( false );  
		view.setForceFit( true );

		view.setGroupRenderer( new GridGroupRenderer()
		{  
			public String render( GroupColumnData data )
			{  
				String f = columnModel.getColumnById( data.field ).getHeader();  
				return f + " " + data.group;  
			}
		});

		view.setEmptyText( "Não existe nenhum aluno associado nessa demanda" );
		grid.setView( view );
		grid.setStripeRows( true );
		grid.setBorders( true );
		grid.getSelectionModel().setSelectionMode( SelectionMode.MULTI );

	    panel.add( grid, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		// Id da demanda, para agrupar os alunos
		SummaryColumnConfig< Integer > cc1 = new SummaryColumnConfig< Integer >(
			AlunoDemandaDTO.PROPERTY_DEMANDA_ID, "Demanda", 50 );

		cc1.setSummaryType( SummaryType.COUNT );
		list.add( cc1 );

		// Nome do aluno
		cc1 = new SummaryColumnConfig< Integer >(
			AlunoDemandaDTO.PROPERTY_DISPLAY_TEXT, "Alunos", 50 );
		cc1.setSummaryType( SummaryType.COUNT );
		cc1.setSummaryRenderer( new SummaryRenderer()
		{  
			public String render( Number value, Map< String, Number > data )
			{  
				return ( value.intValue() > 1 ? "(" + value.intValue() + " Alunos)" : "(1 Aluno)" );  
			}  
		});

		list.add( cc1 );

		return list;
	}

	@Override
	public Button getNewButton()
	{
		return toolBar.getNewButton();
	}

	@Override
	public Button getRemoveButton()
	{
		return toolBar.getRemoveButton();
	}

	@Override
	public Grid< AlunoDemandaDTO > getGrid()
	{
		return grid;
	}

	@Override
	public void setProxy(
		RpcProxy< ListLoadResult< AlunoDemandaDTO > > proxy )
	{
		this.proxy = proxy;
	}

	@Override
	public DemandaDTO getDemandaDTO()
	{
		return demandaDTO;
	}
}
