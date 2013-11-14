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
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.presenter.AtendimentosFaixaDisciplinaPresenter;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaCreditoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.google.gwt.i18n.client.NumberFormat;

public class AtendimentosFaixaDisciplinaView extends MyComposite
	implements AtendimentosFaixaDisciplinaPresenter.Display
{
	
	private SimpleToolBar toolBar;
	private Grid< AtendimentoFaixaCreditoDTO > gridPanel;
	private ListStore< AtendimentoFaixaCreditoDTO > store = new ListStore< AtendimentoFaixaCreditoDTO >();
		private ContentPanel panel;
	private GTabItem tabItem;
	private CampusComboBox campusCB;
	private CenarioDTO cenarioDTO;
	
	public AtendimentosFaixaDisciplinaView(CenarioDTO cenarioDTO)
	{
		this.cenarioDTO = cenarioDTO;
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
		panel.setHeadingHtml( "Master Data » Atendimentos por Faixa de Disciplina" );
	}
	
	private void createTabItem()
	{
		tabItem = new GTabItem(
			"Atendimentos por Faixa de Disciplina", Resources.DEFAULTS.resumoDisciplinas16() );
	
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
	
	private void createGrid()
	{
	    this.gridPanel = new Grid< AtendimentoFaixaCreditoDTO >( this.getStore(), new ColumnModel( this.getColumnList()) );
	    
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
	
		list.add( new ColumnConfig( AtendimentoFaixaCreditoDTO.PROPERTY_FAIXA_CREDITO, "Quantidade de Disciplinas Não Atendidas", 240 ) );
		list.add( createIntegerColumnConfig( AtendimentoFaixaCreditoDTO.PROPERTY_CREDITOS_DEMANDADOS_P1, "Demandados P1 (Qtde. Alunos)", 170 ) );
		list.add( createIntegerColumnConfig( AtendimentoFaixaCreditoDTO.PROPERTY_CREDITOS_DEMANDADOS_P2, "Demandados P2 (Qtde. Alunos)", 170 ) );
		
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
	public Grid< AtendimentoFaixaCreditoDTO > getGrid() {
		return this.gridPanel;
	}
	
	public void setStore( ListStore< AtendimentoFaixaCreditoDTO > store )
	{
		this.store = store;
	}
	
	@Override
	public ListStore< AtendimentoFaixaCreditoDTO > getStore()
	{
		return this.store;
	}
	
	@Override
	public CampusComboBox getCampusComboBox()
	{
		return this.campusCB;
	}
}
