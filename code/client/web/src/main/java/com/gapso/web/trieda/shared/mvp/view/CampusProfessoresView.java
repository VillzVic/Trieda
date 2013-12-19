package com.gapso.web.trieda.shared.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorCampusDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.CampusProfessoresPresenter;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class CampusProfessoresView extends MyComposite
	implements CampusProfessoresPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< ProfessorCampusDTO > gridPanel;
	private SimpleFilter filter;
	private CampusComboBox campusBuscaComboBox;
	private ProfessorComboBox professorBuscaComboBox;
	private ContentPanel panel;
	private GTabItem tabItem;
	private UsuarioDTO usuario;
	private CenarioDTO cenarioDTO;

	public CampusProfessoresView( CenarioDTO cenarioDTO, UsuarioDTO usuario )
	{
		this.cenarioDTO = cenarioDTO;
		this.usuario = usuario;
		initUI();
	}

	private void initUI()
	{
		panel = new ContentPanel( new BorderLayout() );
		String title = ( ( !usuario.isAdministrador() ) ?
			"Professor » Campi de Trabalho" : cenarioDTO.getNome() + " » Campi de Trabalho" );

		panel.setHeadingHtml( title );

		if ( usuario.isAdministrador() )
		{
			createToolBar();
			createFilter();
		}
		createGrid();
		createTabItem();
		initComponent( tabItem );
	}

	private void createTabItem()
	{
		tabItem = new GTabItem( "Campi de Trabalho",
			Resources.DEFAULTS.campiTrabalho16() );

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

	    gridPanel = new SimpleGrid< ProfessorCampusDTO >( getColumnList(), this, this.toolBar );
	    panel.add( gridPanel, bld );
	}

	private List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( createColumnConfig( ProfessorCampusDTO.PROPERTY_CAMPUS_STRING, "Campus", 150, false ) );	
		list.add( createColumnConfig( ProfessorCampusDTO.PROPERTY_PROFESSOR_CPF, "CPF Professor", 110, false ) );
		list.add( createColumnConfig( ProfessorCampusDTO.PROPERTY_PROFESSOR_STRING, "Professor", 250, false ) );

		return list;
	}
	
	private ColumnConfig createColumnConfig(
			String id, String text, int width, boolean sortable )
	{
		ColumnConfig cc = new ColumnConfig( id, text, width );
		cc.setSortable( sortable );

		return cc;
	}


	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST );
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		filter = new SimpleFilter();
		campusBuscaComboBox = new CampusComboBox(cenarioDTO);
		professorBuscaComboBox = new ProfessorComboBox(cenarioDTO);
		filter.addField( campusBuscaComboBox );
		filter.addField( professorBuscaComboBox );

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
	public MenuItem getExportXlsExcelButton()
	{
		return (MenuItem) toolBar.getExportExcelButton().getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getExportXlsxExcelButton()
	{
		return (MenuItem) toolBar.getExportExcelButton().getMenu().getItem(1);
	}
	
	@Override
	public SimpleGrid< ProfessorCampusDTO > getGrid()
	{
		return gridPanel;
	}

	@Override
	public CampusComboBox getCampusBuscaComboBox()
	{
		return campusBuscaComboBox;
	}

	@Override
	public ProfessorComboBox getProfessorBuscaComboBox()
	{
		return professorBuscaComboBox;
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
