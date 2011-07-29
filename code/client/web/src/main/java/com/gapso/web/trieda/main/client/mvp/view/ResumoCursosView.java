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
import com.gapso.web.trieda.main.client.mvp.presenter.ResumoCursosPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ResumoCursoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class ResumoCursosView extends MyComposite
	implements ResumoCursosPresenter.Display
{
	private SimpleToolBar toolBar;
	private TreeStore< ResumoCursoDTO > store = new TreeStore< ResumoCursoDTO >();
	private TreeGrid< ResumoCursoDTO > tree;
	private ContentPanel panel;
	private GTabItem tabItem;
	private CampusComboBox campusCB;

	public ResumoCursosView( CenarioDTO cenario )
	{
		initUI();
	}

	private void initUI()
	{
		panel = new ContentPanel( new BorderLayout() );
		panel.setHeading( "Master Data » Resultados por Cursos" );

		createToolBar();
		createForm();
		createGrid();
		createTabItem();
		initComponent( tabItem );
	}

	private void createToolBar()
	{
		// Exibe apenas o botão 'exportExcel'
		toolBar = new SimpleToolBar( false, false, false, false, true, this );
		panel.setTopComponent( toolBar );
	}

	@Override
	protected void beforeRender()
	{
		super.beforeRender();
	}

	private void createTabItem()
	{
		tabItem = new GTabItem( "Resumo por Cursos",
			Resources.DEFAULTS.resumoCampi16() );
		tabItem.setContent( panel );
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

		campusCB = new CampusComboBox();
		formPanel.add( campusCB, formData );

	    BorderLayoutData bld = new BorderLayoutData( LayoutRegion.NORTH );
	    bld.setMargins( new Margins( 2 ) );
	    bld.setSize( 35 );
	    this.panel.add( formPanel, bld );
	}

	private void createGrid()
	{
		tree = new TreeGrid< ResumoCursoDTO >(
			getStore(), new ColumnModel( getColumnList() ) );

		ContentPanel contentPanel = new ContentPanel( new FitLayout() );
		contentPanel.setHeaderVisible( false );
		contentPanel.add( tree );

		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
		bld.setMargins( new Margins( 5 ) );
		panel.add( contentPanel, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		GridCellRenderer< ResumoCursoDTO > percenteRenderer =
			new GridCellRenderer< ResumoCursoDTO >()
			{
				@Override
				public String render( ResumoCursoDTO model, String property,
					ColumnData config, int rowIndex, int colIndex,
					ListStore< ResumoCursoDTO > store, Grid< ResumoCursoDTO > grid )
				{
					if ( model.get( property ) == null )
					{
						return "";
					}

					return ( ( ( (Double) model.get( property ) ) * 100 ) + "%" );
				}
		};
		GridCellRenderer< ResumoCursoDTO > tipoDeCreditoRenderer =
			new GridCellRenderer<ResumoCursoDTO>()
			{
				@Override
				public String render( ResumoCursoDTO model, String property,
					ColumnData config, int rowIndex, int colIndex,
					ListStore< ResumoCursoDTO > store, Grid< ResumoCursoDTO > grid )
				{
					if ( model.get( property ) == null )
					{
						return "";
					}

					return ( (Boolean) model.get( property) ) ?
						getI18nConstants().teorico() : getI18nConstants().pratico();
				}
		};

		List< ColumnConfig > list = new ArrayList< ColumnConfig >();
		ColumnConfig campusColumnConfig = new ColumnConfig(
			ResumoCursoDTO.PROPERTY_CAMPUS_STRING, getI18nConstants().campus(), 80 );

		campusColumnConfig.setRenderer( new TreeGridCellRenderer< ResumoCursoDTO >() );
		list.add( campusColumnConfig );
		list.add( new ColumnConfig( ResumoCursoDTO.PROPERTY_TURNO_STRING,
			getI18nConstants().turno(), 80 ) );
		list.add( new ColumnConfig(ResumoCursoDTO.PROPERTY_CURSO_STRING,
			getI18nConstants().curso(), 80 ) );
		list.add( new ColumnConfig( ResumoCursoDTO.PROPERTY_MATRIZCURRICULAR_STRING,
			getI18nConstants().matrizCurricular(), 80 ) );
		list.add( new ColumnConfig(ResumoCursoDTO.PROPERTY_PERIODO_INT,
			getI18nConstants().periodo(), 55 ) );
		list.add(new ColumnConfig( ResumoCursoDTO.PROPERTY_DISCIPLINA_STRING,
			getI18nConstants().disciplina(), 80 ) );
		list.add( new ColumnConfig( ResumoCursoDTO.PROPERTY_TURMA_STRING,
			getI18nConstants().turma(), 80 ) );

		ColumnConfig tipoDeCreditoColumnConfig = new ColumnConfig(
			ResumoCursoDTO.PROPERTY_TIPO_CREDITO_TEORICO_BOOLEAN,
			getI18nConstants().TipoCredito(), 80);

		tipoDeCreditoColumnConfig.setRenderer( tipoDeCreditoRenderer );
		list.add( tipoDeCreditoColumnConfig );
		list.add( new ColumnConfig( ResumoCursoDTO.PROPERTY_CREDITOS_INT,
			getI18nConstants().creditos(), 60 ) );
		list.add( new ColumnConfig( ResumoCursoDTO.PROPERTY_QUANTIDADE_ALUNOS_INT,
			getI18nConstants().quantidadeAlunos(), 70 ) );

		ColumnConfig rateioColumnConfig = new ColumnConfig(
			ResumoCursoDTO.PROPERTY_RATEIO_DOUBLE,
			getI18nConstants().rateio(), 80 );

		rateioColumnConfig.setRenderer( percenteRenderer );
		list.add( rateioColumnConfig );
		list.add( new ColumnConfig( ResumoCursoDTO.PROPERTY_CUSTO_DOCENTE_DOUBLE,
			getI18nConstants().custoDocente(), 100 ) );
		list.add( new ColumnConfig(ResumoCursoDTO.PROPERTY_RECEITA_DOUBLE,
			getI18nConstants().receita(), 100 ) );
		list.add(new ColumnConfig( ResumoCursoDTO.PROPERTY_MARGEM_DOUBLE,
			getI18nConstants().margem(), 100 ) );

		ColumnConfig margemPercenteColumnConfig = new ColumnConfig(
			ResumoCursoDTO.PROPERTY_MARGEM_PERCENTE_DOUBLE,
			getI18nConstants().margemPercente(), 100 );

		margemPercenteColumnConfig.setRenderer( percenteRenderer );
		list.add( margemPercenteColumnConfig );

		return list;
	}

	@Override
	public TreeStore< ResumoCursoDTO > getStore()
	{
		return store;
	}

	@Override
	public TreeGrid< ResumoCursoDTO > getTree()
	{
		return tree;
	}

	@Override
	public CampusComboBox getCampusComboBox()
	{
		return campusCB;
	}

	@Override
	public Button getExportExcelButton()
	{
		return toolBar.getExportExcelButton();
	}
}
