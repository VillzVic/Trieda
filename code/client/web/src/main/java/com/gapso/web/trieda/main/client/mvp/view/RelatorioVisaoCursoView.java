package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.ComboBox.TriggerAction;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.RelatorioVisaoCursoPresenter;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.GradeHorariaCursoGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class RelatorioVisaoCursoView extends MyComposite
	implements RelatorioVisaoCursoPresenter.Display
{
	private SimpleToolBar toolBar;
	private GradeHorariaCursoGrid grid;
	private Button submitBt;
	private CampusComboBox campusCB;
	private CursoComboBox cursoCB;
	private CurriculoComboBox curriculoCB;
	private TurnoComboBox turnoCB;
	private SimpleComboBox< Integer > periodoCB;
	private ContentPanel panel;
	private GTabItem tabItem;

	public RelatorioVisaoCursoView()
	{
		initUI();
	}

	private void initUI()
	{
		panel = new ContentPanel( new BorderLayout() );
		panel.setHeading( "Master Data » Grade Horária Visão Curso" );
		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( tabItem );
	}

	private void createToolBar()
	{
		// Exibe apenas o botão 'exportExcel'
		toolBar = new SimpleToolBar( false, false, false, false, true, this );
		panel.setTopComponent( toolBar );
	}

	private void createTabItem()
	{
		tabItem = new GTabItem( "Grade Horária Visão Curso",
			Resources.DEFAULTS.saidaCurso16() );

		tabItem.setContent( panel );
	}

	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
		bld.setMargins( new Margins( 5, 5, 5, 5 ) );
		grid = new GradeHorariaCursoGrid();
		panel.add( grid, bld );
	}

	private void createFilter()
	{
		FormData formData = new FormData( "100%" );
		FormPanel panel = new FormPanel();
		panel.setHeaderVisible( true );
		panel.setHeading( "Filtro" );
		panel.setButtonAlign( HorizontalAlignment.RIGHT );

		LayoutContainer main = new LayoutContainer();
		main.setLayout( new ColumnLayout() );

		LayoutContainer left = new LayoutContainer();
		left.setStyleAttribute( "paddingRight", "10px" );
		FormLayout layout = new FormLayout();
		left.setLayout( layout );

		cursoCB = new CursoComboBox();
		left.add( cursoCB, formData );

		curriculoCB = new CurriculoComboBox( cursoCB );
		curriculoCB.setUseQueryCache( false );
		left.add( curriculoCB, formData );

		LayoutContainer right = new LayoutContainer();
		right.setStyleAttribute( "paddingLeft", "10px" );
		layout = new FormLayout();
		right.setLayout( layout );

		campusCB = new CampusComboBox( curriculoCB );
		right.add( campusCB, formData );

		periodoCB = new SimpleComboBox< Integer >();
		periodoCB.setFieldLabel( "Período" );
		periodoCB.setEditable( false );
		periodoCB.setTriggerAction( TriggerAction.ALL );

		right.add( periodoCB, formData );

		turnoCB = new TurnoComboBox( campusCB );
		right.add( turnoCB, formData );

		main.add( left, new ColumnData( 0.5 ) );
		main.add( right, new ColumnData( 0.5 ) );

		submitBt = new Button( "Filtrar", AbstractImagePrototype.create(
			Resources.DEFAULTS.filter16() ) );
		panel.addButton( submitBt );

		panel.add( main, new FormData( "100%" ) );

		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.NORTH );
		bld.setMargins( new Margins( 5, 5, 0, 5 ) );
		bld.setCollapsible( true );
		bld.setSize( 155 );

		this.panel.add( panel, bld );
	}

	@Override
	public GradeHorariaCursoGrid getGrid() {
		return grid;
	}

	@Override
	public Button getSubmitBuscaButton() {
		return submitBt;
	}

	@Override
	public TurnoComboBox getTurnoComboBox() {
		return turnoCB;
	}

	@Override
	public CursoComboBox getCursoComboBox() {
		return cursoCB;
	}

	@Override
	public CurriculoComboBox getCurriculoComboBox() {
		return curriculoCB;
	}

	@Override
	public SimpleComboBox<Integer> getPeriodoComboBox() {
		return periodoCB;
	}

	@Override
	public CampusComboBox getCampusComboBox() {
		return campusCB;
	}

	@Override
	public Button getExportExcelButton() {
		return toolBar.getExportExcelButton();
	}
}
