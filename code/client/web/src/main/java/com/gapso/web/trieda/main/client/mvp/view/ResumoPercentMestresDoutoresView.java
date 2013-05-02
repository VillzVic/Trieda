package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.HeaderGroupConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.shared.dtos.PercentMestresDoutoresDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.main.client.mvp.presenter.ResumoPercentMestresDoutoresPresenter;

public class ResumoPercentMestresDoutoresView extends MyComposite
	implements ResumoPercentMestresDoutoresPresenter.Display
{
	private SimpleToolBar toolBar;
	private Grid< PercentMestresDoutoresDTO > gridPanel;
	private ListStore< PercentMestresDoutoresDTO > store = new ListStore< PercentMestresDoutoresDTO >();
 	private ContentPanel panel;
	private GTabItem tabItem;
	private CampusComboBox campusCB;
	
	public ResumoPercentMestresDoutoresView()
	{
		initUI();
		createToolBar();
		createForm();
		createGrid();
		createTabItem();
		initComponent( this.tabItem );
	}
	
	private void initUI()
	{
		panel = new ContentPanel( new BorderLayout() );
		panel.setHeading( "Master Data » Resumo de Porcentagem de Mestres e Doutores" );
	}
	
	private void createTabItem()
	{
		tabItem = new GTabItem(
			"Porcentagem de Mestrese Doutores", Resources.DEFAULTS.resumoMatricula16() );
	
		tabItem.setContent( panel );
	}
	
	@Override
	protected void beforeRender()
	{
		super.beforeRender();
	}
	
	private void createToolBar()
	{
		// Exibe apenas o botão 'exportExcel'
		this.toolBar = new SimpleToolBar(
			false, false, false, false, true, this );
	
		this.panel.setTopComponent( this.toolBar );
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

		this.campusCB = new CampusComboBox(null, true);
		formPanel.add( this.campusCB, formData );


	    BorderLayoutData bld = new BorderLayoutData( LayoutRegion.NORTH );
	    bld.setMargins( new Margins( 2 ) );
	    bld.setSize( 35 );
		this.panel.add( formPanel, bld );
	}
	
	private void createGrid()
	{
	    this.gridPanel = new Grid< PercentMestresDoutoresDTO >( this.getStore(), this.getColumnList() );
	    
	    ContentPanel contentPanel = new ContentPanel( new FitLayout() );
	    contentPanel.setHeaderVisible( false );
	    contentPanel.add( this.gridPanel );

	    BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5 ) );

	    this.panel.add( contentPanel, bld );
	}
	
	public ColumnModel getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();
		
		list.add( new ColumnConfig( PercentMestresDoutoresDTO.PROPERTY_CURSO_STRING, "Curso", 200 ) );
		list.add( new ColumnConfig( PercentMestresDoutoresDTO.PROPERTY_DOUTORES, "Doutores", 80 ) );
		list.add( new ColumnConfig( PercentMestresDoutoresDTO.PROPERTY_MESTRES, "Mestres", 80 ) );
		list.add( new ColumnConfig( PercentMestresDoutoresDTO.PROPERTY_OUTROS, "Outros", 80 ) );
		list.add( new ColumnConfig( PercentMestresDoutoresDTO.PROPERTY_TOTAL, "Total", 80 ) );
		list.add( new ColumnConfig( PercentMestresDoutoresDTO.PROPERTY_DOUTORES_PERCENT, "% Doutores", 80 ) );
		list.add( new ColumnConfig( PercentMestresDoutoresDTO.PROPERTY_MESTRES_DOUTORES_PERCENT, "% Mestres e Doutores", 120 ) );
		list.add( new ColumnConfig( PercentMestresDoutoresDTO.PROPERTY_DOUTORES_MIN, "% Mínimo de Doutores", 120 ) );
		list.add( new ColumnConfig( PercentMestresDoutoresDTO.PROPERTY_MESTRES_DOUTORES_MIN, "% Mínimo de Mestres e Doutores", 170 ) );
		
		ColumnModel cm = new ColumnModel(list);
		
		cm.addHeaderGroup(0, 0 , new HeaderGroupConfig("", 1, 5));
		cm.addHeaderGroup(0, 5, new HeaderGroupConfig("ALOCAÇÃO", 1, 2));
		cm.addHeaderGroup(0, 7, new HeaderGroupConfig("MÍNIMO MEC", 1, 2));
		
		return cm;
		
	}
	
	@Override
	public MenuItem getExportXlsExcelButton() {
		return (MenuItem) this.toolBar.getExportExcelButton().getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getExportXlsxExcelButton() {
		return (MenuItem) this.toolBar.getExportExcelButton().getMenu().getItem(1);
	}
	
	@Override
	public Grid< PercentMestresDoutoresDTO > getGrid() {
		return this.gridPanel;
	}
	
	public void setStore( ListStore< PercentMestresDoutoresDTO > store )
	{
		this.store = store;
	}

	@Override
	public ListStore< PercentMestresDoutoresDTO > getStore()
	{
		return this.store;
	}
	
	@Override
	public CampusComboBox getCampusComboBox()
	{
		return this.campusCB;
	}

}
