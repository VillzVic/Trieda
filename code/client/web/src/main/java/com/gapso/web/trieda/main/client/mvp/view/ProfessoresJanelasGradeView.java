package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessoresJanelasGradePresenter;
import com.gapso.web.trieda.shared.dtos.RelatorioQuantidadeDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.TipoProfessorComboBox;
import com.google.gwt.i18n.client.NumberFormat;

public class ProfessoresJanelasGradeView  extends MyComposite
	implements ProfessoresJanelasGradePresenter.Display
{
	
	private SimpleToolBar toolBar;
	private Grid< RelatorioQuantidadeDTO > gridPanel;
	private ListStore< RelatorioQuantidadeDTO > store = new ListStore< RelatorioQuantidadeDTO >();
		private ContentPanel panel;
	private GTabItem tabItem;
	private CampusComboBox campusCB;
	private SimpleFilter filter;
	private TipoProfessorComboBox tipoProfessorCB;
	private CenarioDTO cenarioDTO;
	
	public ProfessoresJanelasGradeView(CenarioDTO cenarioDTO)
	{
		this.cenarioDTO = cenarioDTO;
		initUI();
		createToolBar();
		createForm();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( this.tabItem );
	}
	
	private void initUI()
	{
		panel = new ContentPanel( new BorderLayout() );
		panel.setHeadingHtml( "Master Data » Professores por Quantidade de Janelas na Grade Horária" );
	}
	
	private void createTabItem()
	{
		tabItem = new GTabItem(
			"Professores por Quantidade de Janelas na Grade Horária", Resources.DEFAULTS.resumoDisciplinas16() );
	
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
	
		this.campusCB = new CampusComboBox(cenarioDTO);
		formPanel.add( this.campusCB, formData );
	
	    BorderLayoutData bld = new BorderLayoutData( LayoutRegion.NORTH );
	    bld.setMargins( new Margins( 2 ) );
	    bld.setSize( 35 );
		this.panel.add( formPanel, bld );
	}
	
	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST );

		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		this.filter = new SimpleFilter();

		this.tipoProfessorCB = new TipoProfessorComboBox();

		this.filter.addField( this.tipoProfessorCB );

		this.panel.add( this.filter, bld );
	}
	
	private void createGrid()
	{
	    this.gridPanel = new Grid< RelatorioQuantidadeDTO >( this.getStore(), new ColumnModel( this.getColumnList()) );
	    
	    ContentPanel contentPanel = new ContentPanel( new FitLayout() );
	    contentPanel.setHeaderVisible( false );
	    contentPanel.add( this.gridPanel );
	
	    BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5 ) );
	
	    this.panel.add( contentPanel, bld );
	}
	
	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();
	
		list.add( new ColumnConfig( RelatorioQuantidadeDTO.PROPERTY_FAIXA_CREDITO, "Quantidade de Janelas na Grade Horária", 240 ) );
		list.add( createIntegerColumnConfig( RelatorioQuantidadeDTO.PROPERTY_QUANTIDADE, "Professores (Qtde. Professores)", 170 ) );
		
		return list;
	}
	
	private ColumnConfig createIntegerColumnConfig(
			String id, String text, int width )
	{
		String pattern = "#,###";
		ColumnConfig cc = new ColumnConfig( id, text, width );
		cc.setNumberFormat(NumberFormat.getFormat(pattern));
	
		return cc;
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
	public Grid< RelatorioQuantidadeDTO > getGrid() {
		return this.gridPanel;
	}
	
	public void setStore( ListStore< RelatorioQuantidadeDTO > store )
	{
		this.store = store;
	}
	
	@Override
	public ListStore< RelatorioQuantidadeDTO > getStore()
	{
		return this.store;
	}
	
	@Override
	public CampusComboBox getCampusComboBox()
	{
		return this.campusCB;
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
	public TipoProfessorComboBox getTipoProfessorComboBox()
	{
		return this.tipoProfessorCB;
	}
}
