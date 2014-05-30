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
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
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
	
	private CenarioDTO cenarioDTO;

	public AlunoDemandaView( CenarioDTO cenarioDTO, DemandaDTO demandaDTO )
	{
		this.demandaDTO = demandaDTO;
		this.cenarioDTO = cenarioDTO;
		this.initUI();
	}
	
	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() )
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

		this.panel.setHeadingHtml( cenarioDTO.getNome() + " » Alunos relacionados à demanda " +
			"(" + demandaDTO.getDisciplinaString() + ")" );

		createToolBar();
		createTabItem();
		initComponent( this.tabItem );
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem( "Alunos",
			Resources.DEFAULTS.disciplina16() );

		this.tabItem.setContent( this.panel );
	}

	private void createToolBar()
	{
		this.toolBar = new SimpleToolBar(
			true, false, true, false, false, this );

		this.panel.setTopComponent( this.toolBar );
	}

	private void createGrid()
	{
		BorderLayoutData bld
			= new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );

		BaseListLoader< ListLoadResult< ModelData > > loader
			= new BaseListLoader< ListLoadResult< ModelData > >( this.proxy );
		loader.setRemoteSort( true );

		GroupingStore< AlunoDemandaDTO > store
			= new GroupingStore< AlunoDemandaDTO >( loader );

		final ColumnModel columnModel = new ColumnModel( getColumnList() );
		this.grid = new Grid< AlunoDemandaDTO >( store, columnModel );

		GroupSummaryView view = new GroupSummaryView();  
		view.setShowGroupedColumn( false );  
		view.setForceFit( true );

		view.setGroupRenderer( new GridGroupRenderer()
		{  
			public String render( GroupColumnData data )
			{  
				String f = columnModel.getColumnById( data.field ).getHeaderHtml();  
				return f + " " + data.group;  
			}
		});

		view.setEmptyText( "Não há nenhum aluno associado nessa demanda" );
		this.grid.setView( view );
		this.grid.setStripeRows( true );
		this.grid.setBorders( true );
		this.grid.getSelectionModel().setSelectionMode( SelectionMode.MULTI );

	    this.panel.add( this.grid, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		// Nome do aluno
		SummaryColumnConfig< Integer > cc1 = new SummaryColumnConfig< Integer >(
			AlunoDemandaDTO.PROPERTY_DISPLAY_TEXT, "Alunos", 50 );
		cc1.setSummaryType( SummaryType.COUNT );
		cc1.setSummaryRenderer( new SummaryRenderer()
		{  
			public String render( Number value, Map< String, Number > data )
			{  
				return ( value.intValue() > 1 ? "(" + value.intValue()
					+ " Alunos)" : "(" + value.intValue() + " Aluno)" );  
			}  
		});
		list.add( cc1 );

		// Período
		cc1 = new SummaryColumnConfig< Integer >(
			AlunoDemandaDTO.PROPERTY_PERIODO_STRING, "Período", 30 );
		list.add( cc1 );
		
		// Informa se a disciplina do aluno precisa ser atendida por equivalencia
		cc1 = new SummaryColumnConfig< Integer >(
			AlunoDemandaDTO.PROPERTY_EXIGE_EQUIVALENCIA_FORCADA_STRING, "Exige Equivalência Forçada", 30 );
		list.add( cc1 );

		// Informa se o aluno foi ou não atendido na otimização
		cc1 = new SummaryColumnConfig< Integer >(
			AlunoDemandaDTO.PROPERTY_ALUNO_ATENDIDO_STRING, "Atendido", 30 );
		list.add( cc1 );

		return list;
	}

	@Override
	public Button getNewButton()
	{
		return this.toolBar.getNewButton();
	}

	@Override
	public Button getRemoveButton()
	{
		return this.toolBar.getRemoveButton();
	}

	@Override
	public Grid< AlunoDemandaDTO > getGrid()
	{
		return this.grid;
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
		return this.demandaDTO;
	}
}
