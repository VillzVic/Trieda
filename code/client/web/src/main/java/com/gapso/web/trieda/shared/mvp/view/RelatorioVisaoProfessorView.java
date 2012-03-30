package com.gapso.web.trieda.shared.mvp.view;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.RelatorioVisaoProfessorPresenter;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.GradeHorariaProfessorGrid;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.ProfessorVirtualComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class RelatorioVisaoProfessorView
	extends MyComposite
	implements RelatorioVisaoProfessorPresenter.Display
{
	private SimpleToolBar toolBar;
	private GradeHorariaProfessorGrid grid;
	private Button submitBt;
	private CampusComboBox campusCB;
	private TurnoComboBox turnoCB;
	private ProfessorComboBox professorCB;
	private ProfessorVirtualComboBox professorVirtualCB;
	private ContentPanel panel;
	private GTabItem tabItem;
	private UsuarioDTO usuario;
	private boolean isVisaoProfessor; 

	public RelatorioVisaoProfessorView(
		UsuarioDTO usuario, boolean isVisaoProfessor )
	{
		this.usuario = usuario;
		this.isVisaoProfessor = isVisaoProfessor;

		this.initUI();
	}

	public boolean isVisaoProfessor()
	{
		return this.isVisaoProfessor;
	}

	public void setVisaoProfessor( boolean isVisaoProfessor )
	{
		this.isVisaoProfessor = isVisaoProfessor;
	}

	private void createToolBar()
	{
		// Exibe apenas o botão 'exportExcel'
		this.toolBar = new SimpleToolBar(
			false, false, false, false, true, this );

		this.panel.setTopComponent( this.toolBar );
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeading( "Master Data » Grade Horária Visão Professor" );

		this.createToolBar();
		this.createGrid();
		this.createFilter();
		this.createTabItem();
		this.initComponent( this.tabItem );
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem( "Grade Horária Visão Professor",
			Resources.DEFAULTS.saidaProfessor16() );

		this.tabItem.setContent( this.panel );
	}

	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );

	    this.grid = new GradeHorariaProfessorGrid( this.isVisaoProfessor() );
	    this.panel.add( this.grid, bld );
	}

	private void createFilter()
	{
		FormData formData = new FormData( "100%" );
		FormPanel panel = new FormPanel();
		panel.setHeaderVisible( true );
		panel.setHeading( "Filtro" );

		LayoutContainer main = new LayoutContainer( new ColumnLayout() );
		LayoutContainer left = new LayoutContainer( new FormLayout( LabelAlign.RIGHT ) );
		LayoutContainer right = new LayoutContainer( new FormLayout( LabelAlign.RIGHT ) );

		if ( this.usuario.isAdministrador() )
		{
			this.campusCB = new CampusComboBox();
			left.add( this.campusCB, formData );
		}

		this.turnoCB = new TurnoComboBox(
			this.campusCB, usuario.isProfessor() );
		left.add( this.turnoCB, formData );

		if ( this.usuario.isAdministrador() )
		{
			this.professorCB = new ProfessorComboBox( this.campusCB );
			right.add( this.professorCB, formData );

			this.professorVirtualCB = new ProfessorVirtualComboBox( this.campusCB );
			right.add( this.professorVirtualCB, formData );
		}

		this.submitBt = new Button( "Filtrar",
			AbstractImagePrototype.create( Resources.DEFAULTS.filter16() ) );
		panel.addButton( this.submitBt );

		main.add( left, new ColumnData( 0.5 ) );
		main.add( right, new ColumnData( 0.5 ) );

		panel.add( main, new FormData( "100%" ) );

		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.NORTH );

		bld.setMargins( new Margins( 5, 5, 0, 5 ) );
		bld.setCollapsible( true );

		if ( this.usuario.isAdministrador() )
		{
			bld.setSize( 150 );
		}
		else
		{
			bld.setSize( 123 );
		}

		this.panel.add( panel, bld );
	}

	@Override
	public GradeHorariaProfessorGrid getGrid()
	{
		return this.grid;
	}

	@Override
	public Button getSubmitBuscaButton()
	{
		return this.submitBt;
	}

	@Override
	public CampusComboBox getCampusComboBox()
	{
		return this.campusCB;
	}

	@Override
	public TurnoComboBox getTurnoComboBox()
	{
		return this.turnoCB;
	}

	@Override
	public ProfessorComboBox getProfessorComboBox()
	{
		return this.professorCB;
	}

	@Override
	public ProfessorVirtualComboBox getProfessorVirtualComboBox()
	{
		return this.professorVirtualCB;
	}

	@Override
	public Button getExportExcelButton()
	{
		return this.toolBar.getExportExcelButton();
	}
}
