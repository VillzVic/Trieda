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
import com.gapso.web.trieda.main.client.mvp.presenter.OfertasPresenter;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;

public class OfertasView
	extends MyComposite
	implements OfertasPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< OfertaDTO > gridPanel;
	private SimpleFilter filter;
	private TurnoComboBox turnoBuscaComboBox;
	private CampusComboBox campusBuscaComboBox;
	private CursoComboBox cursoBuscaComboBox;
	private CurriculoComboBox curriculoBuscaComboBox;
	private ContentPanel panel;
	private GTabItem tabItem;

	public OfertasView()
	{
		initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeading(
			"Master Data » Oferta de Cursos em Campi" );

		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( this.tabItem );
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem( "Oferta de Cursos em Campi",
			Resources.DEFAULTS.ofertaCurso16() );

		this.tabItem.setContent( this.panel );
	}

	private void createToolBar()
	{
		this.toolBar = new SimpleToolBar( this );
		this.panel.setTopComponent( this.toolBar );
	}

	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );

	    this.gridPanel = new SimpleGrid< OfertaDTO >( getColumnList(), this );
	    this.panel.add( this.gridPanel, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( OfertaDTO.PROPERTY_CAMPUS_STRING, "Campus", 150 ) );
		list.add( new ColumnConfig( OfertaDTO.PROPERTY_CURSO_STRING, "Curso", 150 ) );
		list.add( new ColumnConfig( OfertaDTO.PROPERTY_MATRIZ_CURRICULAR_STRING, "Matriz Curricular", 150 ) );
		list.add( new ColumnConfig( OfertaDTO.PROPERTY_TURNO_STRING, "Turno", 200 ) );
		list.add( new ColumnConfig( OfertaDTO.PROPERTY_RECEITA, "Receita", 100 ) );

		return list;
	}

	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST );
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		this.filter = new SimpleFilter();
		this.turnoBuscaComboBox = new TurnoComboBox();
		this.turnoBuscaComboBox.setFieldLabel( "Turno" );
		this.campusBuscaComboBox = new CampusComboBox();
		this.campusBuscaComboBox.setFieldLabel( "Campus" );
		this.cursoBuscaComboBox = new CursoComboBox();
		this.cursoBuscaComboBox.setFieldLabel( "Curso" );
		this.curriculoBuscaComboBox = new CurriculoComboBox();
		this.curriculoBuscaComboBox.setFieldLabel( "Matriz Curricular" );

		this.filter.addField( this.turnoBuscaComboBox );
		this.filter.addField( this.campusBuscaComboBox );
		this.filter.addField( this.cursoBuscaComboBox );
		this.filter.addField( this.curriculoBuscaComboBox );

		this.panel.add( this.filter, bld );
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
	public SimpleGrid< OfertaDTO > getGrid()
	{
		return this.gridPanel;
	}

	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< OfertaDTO > > proxy )
	{
		this.gridPanel.setProxy( proxy );
	}

	@Override
	public TurnoComboBox getTurnoBuscaComboBox()
	{
		return this.turnoBuscaComboBox;
	}

	@Override
	public CampusComboBox getCampusBuscaComboBox()
	{
		return this.campusBuscaComboBox;
	}

	@Override
	public CursoComboBox getCursoBuscaComboBox()
	{
		return this.cursoBuscaComboBox;
	}

	@Override
	public CurriculoComboBox getCurriculoBuscaComboBox()
	{
		return this.curriculoBuscaComboBox;
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
}
