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
import com.gapso.web.trieda.main.client.mvp.presenter.CurriculoDisciplinasPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class CurriculoDisciplinasView
	extends MyComposite
	implements CurriculoDisciplinasPresenter.Display
{
	private SimpleToolBar toolBar;
	private Grid< CurriculoDisciplinaDTO > grid;
	private CurriculoDTO curriculoDTO;
	private ContentPanel panel;
	private GTabItem tabItem;
	private RpcProxy< ListLoadResult< CurriculoDisciplinaDTO > > proxy;
	private CenarioDTO cenarioDTO;
	
	public CurriculoDisciplinasView( CenarioDTO cenarioDTO, CurriculoDTO curriculoDTO )
	{
		this.curriculoDTO = curriculoDTO;
		this.cenarioDTO = cenarioDTO;
		
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

		panel.setHeadingHtml( cenarioDTO.getNome() + " » Disciplinas na Matriz Curricular " +
			"(" + curriculoDTO.getCodigo() +
			" / " + curriculoDTO.getCursoString() + ")" );

		createToolBar();
		createTabItem();
		initComponent( tabItem );
	}

	private void createTabItem()
	{
		tabItem = new GTabItem( "Disciplinas",
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

		GroupingStore< CurriculoDisciplinaDTO > store
			= new GroupingStore< CurriculoDisciplinaDTO >( loader );
		store.groupBy( CurriculoDisciplinaDTO.PROPERTY_PERIODO );

		final ColumnModel columnModel = new ColumnModel(getColumnList());
		grid = new Grid< CurriculoDisciplinaDTO >( store, columnModel );

		GroupSummaryView view = new GroupSummaryView();  
		view.setShowGroupedColumn( false );  
		view.setForceFit( true );

		view.setGroupRenderer( new GridGroupRenderer()
		{  
			public String render( GroupColumnData data )
			{  
				String f = columnModel.getColumnById( data.field ).getHeaderHtml();  
				return f + " "+ data.group;  
			}
		});

		view.setEmptyText( "Não existe nenhuma disciplina cadastrada neste currículo" );
		grid.setView( view );
		grid.setStripeRows( true );
		grid.setBorders( true );
		grid.getSelectionModel().setSelectionMode( SelectionMode.MULTI );

	    panel.add( grid, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		SummaryColumnConfig< Integer > cc1 = new SummaryColumnConfig< Integer >(
			CurriculoDisciplinaDTO.PROPERTY_PERIODO, "Período", 100 );

		cc1.setSummaryType( SummaryType.COUNT );
		list.add( cc1 );

		cc1 = new SummaryColumnConfig< Integer >(
			CurriculoDisciplinaDTO.PROPERTY_DISPLAY_TEXT, "Disciplina", 100 );
		cc1.setSummaryType( SummaryType.COUNT );
		cc1.setSummaryRenderer( new SummaryRenderer()
		{  
			public String render( Number value, Map< String, Number > data )
			{  
				return ( value.intValue() > 1 ? "(" + value.intValue() + " Disciplinas)" : "(1 Disciplina)" );  
			}  
		});

		list.add( cc1 );

		SummaryRenderer creditosRenderer = new SummaryRenderer()
		{  
			public String render( Number value, Map< String, Number > data )
			{  
				return ( value.intValue() > 1 ? value.intValue() + " Créditos" : value.intValue() + " Crédito" );  
			}  
		};
		SummaryRenderer blankRenderer = new SummaryRenderer()
		{  
			public String render( Number value, Map< String, Number > data )
			{  
				return ("");
			}  
		};

		SummaryColumnConfig< Double > cc2 = new SummaryColumnConfig< Double >(
			CurriculoDisciplinaDTO.PROPERTY_CREDITOS_TEORICO, "Créditos Teóricos", 100 );
		cc2.setSummaryType( SummaryType.SUM );
		cc2.setSummaryRenderer( creditosRenderer );
		list.add( cc2 );

		cc2 = new SummaryColumnConfig< Double >(
			CurriculoDisciplinaDTO.PROPERTY_CREDITOS_PRATICO, "Créditos Práticos", 100 );
		cc2.setSummaryType( SummaryType.SUM );
		cc2.setSummaryRenderer( creditosRenderer );
		list.add( cc2 );

		cc2 = new SummaryColumnConfig< Double >(
			CurriculoDisciplinaDTO.PROPERTY_CREDITOS_TOTAL, "Total de Créditos", 100 );
		cc2.setSummaryType( SummaryType.SUM );
		cc2.setSummaryRenderer( creditosRenderer );
		list.add( cc2 );
		
		cc2 = new SummaryColumnConfig< Double >(
			CurriculoDisciplinaDTO.PROPERTY_MATURIDADE, "Maturidade", 100 );
		cc2.setSummaryType( SummaryType.SUM );
		cc2.setSummaryRenderer( blankRenderer );
		cc2.setToolTip("É o número mínimo de horas que o aluno deve ter cursado para poder cursar a disciplina em questão.");
		list.add( cc2 );

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
	public Grid< CurriculoDisciplinaDTO > getGrid()
	{
		return grid;
	}

	@Override
	public void setProxy(
		RpcProxy< ListLoadResult<CurriculoDisciplinaDTO > > proxy )
	{
		this.proxy = proxy;
	}

	@Override
	public CurriculoDTO getCurriculoDTO()
	{
		return curriculoDTO;
	}
}
