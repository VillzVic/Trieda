package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.main.client.mvp.presenter.DemandasPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.DisciplinaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;

public class DemandasView
	extends MyComposite
	implements DemandasPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< DemandaDTO > gridPanel;
	private SimpleFilter filter;
	private CampusComboBox campusBuscaCB;
	private CursoComboBox cursoBuscaCB;
	private CurriculoComboBox curriculoBuscaCB;
	private TurnoComboBox turnoBuscaCB;
	private DisciplinaAutoCompleteBox disciplinaBuscaCB;
	private ContentPanel panel;
	private GTabItem tabItem;
	private Button associarAlunosDemandaBT;
	private CenarioDTO cenarioDTO;

	public DemandasView( CenarioDTO cenarioDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeadingHtml( cenarioDTO.getNome() + " » Ofertas e Demandas" );

		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( this.tabItem );
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			"Ofertas e Demandas", Resources.DEFAULTS.demanda16() );

		this.tabItem.setContent( this.panel );
	}
	
	private void createToolBar()
	{
		this.toolBar = new SimpleToolBar( this );
		this.toolBar.add( new SeparatorToolItem() );
		
		this.associarAlunosDemandaBT = this.toolBar.createButton("Associar Alunos à Demanda",Resources.DEFAULTS.disciplina16());
		this.toolBar.add( this.associarAlunosDemandaBT );

		this.panel.setTopComponent( this.toolBar );
	}

	private void createGrid()
	{
		BorderLayoutData bld
			= new BorderLayoutData( LayoutRegion.CENTER );

	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );

	    this.gridPanel = new SimpleGrid< DemandaDTO >( getColumnList(), this, this.toolBar );
	    this.panel.add( this.gridPanel, bld );
	}

	private List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( DemandaDTO.PROPERTY_CAMPUS_STRING, "Campus", 100 ) );
		list.add( new ColumnConfig( DemandaDTO.PROPERTY_TURNO_STRING, "Turno", 100 ) );
		list.add( new ColumnConfig( DemandaDTO.PROPERTY_CURSO_STRING, "Curso", 250 ) );
		list.add( new ColumnConfig( DemandaDTO.PROPERTY_CURRICULO_STRING, "Código de Matriz Curricular", 150 ) );
		list.add( new ColumnConfig( DemandaDTO.PROPERTY_DISCIPLINA_STRING, "Disciplina", 100 ) );
		list.add( new ColumnConfig( DemandaDTO.PROPERTY_DEMANDA, "Demanda de Alunos", 150 ) );

		return list;
	}

	private void createFilter()
	{
		BorderLayoutData bld
			= new BorderLayoutData( LayoutRegion.EAST );

		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		this.filter = new SimpleFilter();

		this.campusBuscaCB = new CampusComboBox( cenarioDTO );
		this.filter.addField( this.campusBuscaCB );

		this.cursoBuscaCB = new CursoComboBox( cenarioDTO );
		this.filter.addField( this.cursoBuscaCB );

		this.curriculoBuscaCB = new CurriculoComboBox( cenarioDTO );
		this.filter.addField( this.curriculoBuscaCB );

		this.turnoBuscaCB = new TurnoComboBox( cenarioDTO );
		this.filter.addField( this.turnoBuscaCB );

		this.disciplinaBuscaCB = new DisciplinaAutoCompleteBox( cenarioDTO );
		this.filter.addField( this.disciplinaBuscaCB );

		this.panel.add( this.filter, bld );
	}

	@Override
	public Button getImportExcelButton()
	{
		return this.toolBar.getImportExcelButton();
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

	@Override
	public SimpleGrid< DemandaDTO > getGrid()
	{
		return this.gridPanel;
	}

	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< DemandaDTO > > proxy )
	{
		this.gridPanel.setProxy( proxy );
	}

	@Override
	public Button getNewButton()
	{
		return this.toolBar.getNewButton();
	}

	@Override
	public Button getEditButton()
	{
		return this.toolBar.getEditButton();
	}

	@Override
	public Button getRemoveButton()
	{
		return this.toolBar.getRemoveButton();
	}

	@Override
	public CampusComboBox getCampusBuscaComboBox()
	{
		return this.campusBuscaCB;
	}

	@Override
	public CursoComboBox getCursoBuscaComboBox()
	{
		return this.cursoBuscaCB;
	}

	@Override
	public CurriculoComboBox getCurriculoBuscaComboBox()
	{
		return this.curriculoBuscaCB;
	}

	@Override
	public TurnoComboBox getTurnoBuscaComboBox()
	{
		return this.turnoBuscaCB;
	}

	@Override
	public DisciplinaAutoCompleteBox getDisciplinaBuscaComboBox()
	{
		return this.disciplinaBuscaCB;
	}

	@Override
	public Button getSubmitBuscaButton()
	{
		return this.filter.getSubmitButton();
	}

	@Override
	public Button getResetBuscaButton()
	{
		return this.filter.getResetButton();
	}

	@Override
	public Button getAssociarAlunosDemanda()
	{
		return this.associarAlunosDemandaBT;
	}
}
