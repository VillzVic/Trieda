package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.RelatorioVisaoSalaPresenter;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.GradeHorariaSalaGrid;
import com.gapso.web.trieda.shared.util.view.SalaComboBox;
import com.gapso.web.trieda.shared.util.view.SemanaLetivaComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.gapso.web.trieda.shared.util.view.UnidadeComboBox;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class RelatorioVisaoSalaView
	extends MyComposite
	implements RelatorioVisaoSalaPresenter.Display
{
	private SimpleToolBar toolBar;
	private GradeHorariaSalaGrid grid;
	private Button submitBt;
	private CampusComboBox campusCB;
	private UnidadeComboBox unidadeCB;
	private SalaComboBox salaCB;
	private TurnoComboBox turnoCB;
	private SemanaLetivaComboBox semanaLetivaCB;
	private TextField< String > capacidadeTF;
	private TextField< String > tipoSalaTF;
	private ContentPanel panel;
	private GTabItem tabItem;

	public RelatorioVisaoSalaView()
	{
		this.initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeading( "Master Data » Grade Horária Visão Sala" );

		createToolBar();
		createGrid();
		createFilter();
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

	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			"Grade Horária Visão Sala",
			Resources.DEFAULTS.saidaSala16() );

		this.tabItem.setContent( this.panel );
	}
	
	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );

	    this.grid = new GradeHorariaSalaGrid();
	    this.panel.add( this.grid, bld );
	}

	private void createFilter()
	{
		FormData formData = new FormData( "100%" );
		FormPanel panel = new FormPanel();
		panel.setHeaderVisible( true );
		panel.setHeading( "Filtro" );
		panel.setButtonAlign( HorizontalAlignment.RIGHT );

		LayoutContainer main = new LayoutContainer( new ColumnLayout() );

		LayoutContainer left = new LayoutContainer( new FormLayout( LabelAlign.RIGHT ) );
		LayoutContainer right = new LayoutContainer( new FormLayout( LabelAlign.RIGHT ) );
		LayoutContainer center = new LayoutContainer( new FormLayout( LabelAlign.RIGHT ) );
		LayoutContainer button = new LayoutContainer( new FormLayout() );

		this.campusCB = new CampusComboBox();
		left.add( this.campusCB, formData );

		this.unidadeCB = new UnidadeComboBox( this.campusCB );
		left.add( this.unidadeCB, formData );

		this.salaCB = new SalaComboBox( this.unidadeCB );
		center.add( this.salaCB, formData );

		this.turnoCB = new TurnoComboBox();
		center.add( this.turnoCB, formData );

		this.semanaLetivaCB = new SemanaLetivaComboBox();
		center.add( this.semanaLetivaCB, formData );

		this.capacidadeTF = new TextField< String >();
		this.capacidadeTF.setFieldLabel( "Capacidade" );
		this.capacidadeTF.setReadOnly( true );
		right.add( this.capacidadeTF, formData );

		this.tipoSalaTF = new TextField< String >();
		this.tipoSalaTF.setFieldLabel( "Tipo" );
		this.tipoSalaTF.setReadOnly( true );
		right.add( this.tipoSalaTF, formData );

		this.submitBt = new Button( "Filtrar",
			AbstractImagePrototype.create( Resources.DEFAULTS.filter16() ) );
		button.setStyleAttribute( "paddingLeft", "10px" );
		button.setStyleAttribute( "paddingTop", "27px" );
		button.add( this.submitBt, formData );

		main.add( left, new ColumnData( 0.3 ) );
		main.add( center, new ColumnData( 0.3 ) );
		main.add( right, new ColumnData( 0.3 ) );
		main.add( button, new ColumnData( 0.1 ) );

		panel.add( main, new FormData( "100%" ) );

		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.NORTH );
		bld.setMargins( new Margins( 5, 5, 0, 5 ) );
		bld.setCollapsible( true );
		bld.setSize( 120 );

		this.panel.add( panel, bld );
	}

	@Override
	public GradeHorariaSalaGrid getGrid()
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
	public UnidadeComboBox getUnidadeComboBox()
	{
		return this.unidadeCB;
	}

	@Override
	public SalaComboBox getSalaComboBox()
	{
		return this.salaCB;
	}

	@Override
	public TurnoComboBox getTurnoComboBox()
	{
		return this.turnoCB;
	}

	@Override
	public TextField< String > getCapacidadeTextField()
	{
		return this.capacidadeTF;
	}

	@Override
	public TextField< String > getTipoTextField()
	{
		return this.tipoSalaTF;
	}

	@Override
	public Button getExportExcelButton()
	{
		return this.toolBar.getExportExcelButton();
	}

	@Override
	public SemanaLetivaComboBox getSemanaLetivaComboBox()
	{
		return this.semanaLetivaCB;
	}
}
