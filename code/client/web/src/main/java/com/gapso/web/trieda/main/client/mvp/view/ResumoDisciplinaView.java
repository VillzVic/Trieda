package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.store.TreeStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnData;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.treegrid.TreeGrid;
import com.extjs.gxt.ui.client.widget.treegrid.TreeGridCellRenderer;
import com.gapso.web.trieda.main.client.mvp.presenter.ResumoDisciplinaPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ResumoDisciplinaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.TriedaCurrency;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class ResumoDisciplinaView
	extends MyComposite
	implements ResumoDisciplinaPresenter.Display
{
	private SimpleToolBar toolBar;
	private TreeStore< ResumoDisciplinaDTO > store = new TreeStore< ResumoDisciplinaDTO >();
	private TreeGrid< ResumoDisciplinaDTO > tree;
	private ContentPanel panel;
	private GTabItem tabItem;
	private CampusComboBox campusCB;

	public ResumoDisciplinaView( CenarioDTO cenario )
	{
		this.initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeading( "Master Data » Resultados por Disciplina" );

		createToolBar();
		createForm();
		createGrid();
		createTabItem();
		initComponent( this.tabItem );
	}

	private void createToolBar()
	{
		// Exibe apenas o botão 'exportExcel'
		this.toolBar = new SimpleToolBar(
			false, false, false, false, true, this );

		this.panel.setTopComponent( this.toolBar );
	}

	@Override
	protected void beforeRender()
	{
		super.beforeRender();
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem( "Resumo por Disciplina",
			Resources.DEFAULTS.resumoDisciplinas16() );

		this.tabItem.setContent( this.panel );
	}

	private void createForm()
	{
		FormData formData = new FormData( "100%" );
		FormPanel formPanel = new FormPanel();

		formPanel.setBodyBorder( false );
		formPanel.setLabelWidth( 100 );
		formPanel.setLabelAlign( LabelAlign.RIGHT );
		formPanel.setHeaderVisible( false );
		formPanel.setAutoHeight( true );

		this.campusCB = new CampusComboBox();
		formPanel.add( this.campusCB, formData );

	    BorderLayoutData bld = new BorderLayoutData( LayoutRegion.NORTH );
	    bld.setMargins( new Margins( 2 ) );
	    bld.setSize( 35 );
		this.panel.add( formPanel, bld );
	}

	private void createGrid()
	{
		this.tree = new TreeGrid< ResumoDisciplinaDTO >(
			this.getStore(), new ColumnModel( this.getColumnList() ) );

	    ContentPanel contentPanel = new ContentPanel( new FitLayout() );
	    contentPanel.setHeaderVisible( false );
	    contentPanel.add( this.tree );

	    BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5 ) );

	    this.panel.add( contentPanel, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		GridCellRenderer< ResumoDisciplinaDTO > percenteRenderer =
			new GridCellRenderer< ResumoDisciplinaDTO >()
			{
				@Override
				public String render( ResumoDisciplinaDTO model, String property,
					ColumnData config, int rowIndex, int colIndex,
					ListStore< ResumoDisciplinaDTO > store, Grid< ResumoDisciplinaDTO > grid )
				{
					if ( model.get( property ) == null )
					{
						return "";
					}

					TriedaCurrency tc = new TriedaCurrency( (Double) model.get( property ) );
					String percent = ( tc.toString() + "%" ); 
					return ( percent );
				}
			};

		GridCellRenderer< ResumoDisciplinaDTO > tipoDeCreditoRenderer =
			new GridCellRenderer< ResumoDisciplinaDTO >()
			{
				@Override
				public String render( ResumoDisciplinaDTO model, String property,
					ColumnData config, int rowIndex, int colIndex,
					ListStore< ResumoDisciplinaDTO > store, Grid< ResumoDisciplinaDTO > grid )
				{
					if ( model.get( property ) == null )
					{
						return "";
					}

					return ( ( (Boolean)model.get( property ) ) ?
						getI18nConstants().teorico() : getI18nConstants().pratico() );
				}
			};

		List< ColumnConfig > list = new ArrayList< ColumnConfig >();
		ColumnConfig campusColumnConfig = new ColumnConfig(
			ResumoDisciplinaDTO.PROPERTY_DISCIPLINA_STRING, getI18nConstants().disciplina(), 250 );
		campusColumnConfig.setRenderer( new TreeGridCellRenderer< ResumoDisciplinaDTO >() );

		list.add( campusColumnConfig );
		list.add( new ColumnConfig( ResumoDisciplinaDTO.PROPERTY_TURMA_STRING,
			this.getI18nConstants().turma(), 80 ) );

		ColumnConfig tipoDeCreditoColumnConfig = new ColumnConfig(
			ResumoDisciplinaDTO.PROPERTY_TIPO_CREDITO_TEORICO_BOOLEAN,
			this.getI18nConstants().TipoCredito(), 120 );

		tipoDeCreditoColumnConfig.setRenderer( tipoDeCreditoRenderer );
		list.add( tipoDeCreditoColumnConfig );

		list.add( new ColumnConfig( ResumoDisciplinaDTO.PROPERTY_CREDITOS_INT,
			this.getI18nConstants().totalDecreditos(), 100 ) );

		list.add( new ColumnConfig( ResumoDisciplinaDTO.PROPERTY_CREDITOS_INT,
			this.getI18nConstants().creditos(), 100 ) );

		list.add( new ColumnConfig( ResumoDisciplinaDTO.PROPERTY_QUANTIDADE_ALUNOS_INT,
			this.getI18nConstants().quantidadeAlunos(), 100 ) );

		list.add( new ColumnConfig( ResumoDisciplinaDTO.PROPERTY_CUSTO_DOCENTE_DOUBLE,
			this.getI18nConstants().custoDocente(), 100 ) );

		list.add( new ColumnConfig( ResumoDisciplinaDTO.PROPERTY_RECEITA_DOUBLE,
			this.getI18nConstants().receita(), 100 ) );

		list.add( new ColumnConfig( ResumoDisciplinaDTO.PROPERTY_MARGEM_DOUBLE,
			this.getI18nConstants().margem(), 100 ) );

		ColumnConfig margemPercenteColumnConfig = new ColumnConfig(
			ResumoDisciplinaDTO.PROPERTY_MARGEM_PERCENTE_DOUBLE,
			this.getI18nConstants().margemPercente(), 100 );

		margemPercenteColumnConfig.setRenderer( percenteRenderer );
		list.add( margemPercenteColumnConfig );

		return list;
	}

	public void setStore(
		TreeStore< ResumoDisciplinaDTO > store )
	{
		this.store = store;
	}

	@Override
	public TreeStore< ResumoDisciplinaDTO > getStore()
	{
		return this.store;
	}

	@Override
	public TreeGrid< ResumoDisciplinaDTO > getTree()
	{
		return this.tree;
	}

	@Override
	public CampusComboBox getCampusComboBox()
	{
		return this.campusCB;
	}

	@Override
	public Button getExportExcelButton()
	{
		return this.toolBar.getExportExcelButton();
	}	
}
