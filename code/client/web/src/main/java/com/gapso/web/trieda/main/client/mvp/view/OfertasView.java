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
		panel = new ContentPanel( new BorderLayout() );
		panel.setHeading( "Master Data » Oferta de Cursos em Campi" );

		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( tabItem );
	}
	
	private void createTabItem()
	{
		tabItem = new GTabItem( "Oferta de Cursos em Campi",
			Resources.DEFAULTS.ofertaCurso16() );

		tabItem.setContent( panel );
	}
	
	private void createToolBar()
	{
		toolBar = new SimpleToolBar( this );
		panel.setTopComponent( toolBar );
	}
	
	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );
	    
	    gridPanel = new SimpleGrid< OfertaDTO >( getColumnList(), this );
	    panel.add( gridPanel, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( OfertaDTO.PROPERTY_CAMPUS_STRING, "Campus", 100 ) );
		list.add( new ColumnConfig( OfertaDTO.PROPERTY_CURSO_STRING, "Curso", 100 ) );
		list.add( new ColumnConfig( OfertaDTO.PROPERTY_MATRIZ_CURRICULAR_STRING, "Matriz Curricular", 100 ) );
		list.add( new ColumnConfig( OfertaDTO.PROPERTY_TURNO_STRING, "Turno", 200 ) );
		list.add( new ColumnConfig( OfertaDTO.PROPERTY_RECEITA, "Receita", 100 ) );

		return list;
	}

	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST );
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		filter = new SimpleFilter();
		turnoBuscaComboBox = new TurnoComboBox();
		turnoBuscaComboBox.setFieldLabel( "Turno" );
		campusBuscaComboBox = new CampusComboBox();
		campusBuscaComboBox.setFieldLabel( "Campus" );
		cursoBuscaComboBox = new CursoComboBox();
		cursoBuscaComboBox.setFieldLabel( "Curso" );
		curriculoBuscaComboBox = new CurriculoComboBox();
		curriculoBuscaComboBox.setFieldLabel( "Matriz Curricular" );

		filter.addField( turnoBuscaComboBox );
		filter.addField( campusBuscaComboBox );
		filter.addField( cursoBuscaComboBox );
		filter.addField( curriculoBuscaComboBox );

		panel.add( filter, bld );
	}

	@Override
	public Button getNewButton()
	{
		return toolBar.getNewButton();
	}

	@Override
	public Button getEditButton()
	{
		return toolBar.getEditButton();
	}

	@Override
	public Button getRemoveButton()
	{
		return toolBar.getRemoveButton();
	}

	@Override
	public Button getImportExcelButton()
	{
		return toolBar.getImportExcelButton();
	}

	@Override
	public Button getExportExcelButton()
	{
		return toolBar.getExportExcelButton();
	}

	@Override
	public SimpleGrid< OfertaDTO > getGrid()
	{
		return gridPanel;
	}

	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< OfertaDTO > > proxy )
	{
		gridPanel.setProxy( proxy );
	}

	@Override
	public TurnoComboBox getTurnoBuscaComboBox()
	{
		return turnoBuscaComboBox;
	}

	@Override
	public CampusComboBox getCampusBuscaComboBox()
	{
		return campusBuscaComboBox;
	}

	@Override
	public CursoComboBox getCursoBuscaComboBox()
	{
		return cursoBuscaComboBox;
	}

	@Override
	public CurriculoComboBox getCurriculoBuscaComboBox()
	{
		return curriculoBuscaComboBox;
	}

	@Override
	public Button getSubmitBuscaButton()
	{
		return filter.getSubmitButton();
	}

	@Override
	public Button getResetBuscaButton()
	{
		return filter.getResetButton();
	}
}
