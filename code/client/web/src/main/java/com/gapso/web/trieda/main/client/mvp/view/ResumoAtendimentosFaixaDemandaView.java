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
import com.gapso.web.trieda.main.client.mvp.presenter.ResumoAtendimentosFaixaDemandaPresenter;
import com.gapso.web.trieda.shared.dtos.ResumoFaixaDemandaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class ResumoAtendimentosFaixaDemandaView extends MyComposite
	implements ResumoAtendimentosFaixaDemandaPresenter.Display
{

	private SimpleToolBar toolBar;
	private Grid< ResumoFaixaDemandaDTO > gridPanel;
	private ListStore< ResumoFaixaDemandaDTO > store = new ListStore< ResumoFaixaDemandaDTO >();
 	private ContentPanel panel;
	private GTabItem tabItem;
	private CampusComboBox campusCB;
	
	public ResumoAtendimentosFaixaDemandaView()
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
		panel.setHeading( "Master Data » Resumo de Atendimentos por Faixa de Demanda" );
	}
	
	private void createTabItem()
	{
		tabItem = new GTabItem(
			"Atendimentos por Faixa de Demanda", Resources.DEFAULTS.resumoFaixaDemanda16() );
	
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

		this.campusCB = new CampusComboBox();
		formPanel.add( this.campusCB, formData );

	    BorderLayoutData bld = new BorderLayoutData( LayoutRegion.NORTH );
	    bld.setMargins( new Margins( 2 ) );
	    bld.setSize( 35 );
		this.panel.add( formPanel, bld );
	}
	
	private void createGrid()
	{
	    this.gridPanel = new Grid< ResumoFaixaDemandaDTO >( this.getStore(), new ColumnModel( this.getColumnList()) );
	    
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
		
		list.add( new ColumnConfig( ResumoFaixaDemandaDTO.PROPERTY_DEMANDA_DISCIPLINA, "Demanda da disciplina", 120 ) );
		list.add( new ColumnConfig( ResumoFaixaDemandaDTO.PROPERTY_DEMANDA_P1, "Demanda P1", 80 ) );
		list.add( new ColumnConfig( ResumoFaixaDemandaDTO.PROPERTY_ATENDIMENTO_P1, "Atendimento P1", 90 ) );
		list.add( new ColumnConfig( ResumoFaixaDemandaDTO.PROPERTY_ATENDIMENTO_PERCENT_P1, "% Atendimento P1", 100 ) );
		list.add( new ColumnConfig( ResumoFaixaDemandaDTO.PROPERTY_ATENDIMENTO_SOMA, "Atendimento P1+P2", 110 ) );
		list.add( new ColumnConfig( ResumoFaixaDemandaDTO.PROPERTY_ATENDIMENTO_SOMA_PERCENT, " % Atendimento P1+P2", 120 ) );
		list.add( new ColumnConfig( ResumoFaixaDemandaDTO.PROPERTY_TURMAS_ABERTAS, "Turmas abertas", 90 ) );
		list.add( new ColumnConfig( ResumoFaixaDemandaDTO.PROPERTY_MEDIA_TURMA, "Media alunos por turma", 130 ) );
		list.add( new ColumnConfig( ResumoFaixaDemandaDTO.PROPERTY_CREDITOS_PAGOS, "Creditos pagos", 90 ) );
		list.add( new ColumnConfig( ResumoFaixaDemandaDTO.PROPERTY_DEMANDA_ACUM_P1, "Demanda P1 acum", 110 ) );
		list.add( new ColumnConfig( ResumoFaixaDemandaDTO.PROPERTY_ATENDIMENTO_SOMA_ACUM, "Atendimento P1+P2 acum", 140 ) );
		list.add( new ColumnConfig( ResumoFaixaDemandaDTO.PROPERTY_ATENDIMENTO_ACUM_PERCENT, "% Atendimento acumulado", 140 ) );
		list.add( new ColumnConfig( ResumoFaixaDemandaDTO.PROPERTY_RECEITA_SEMANAL, "Receita Semanal", 90 ) );
		list.add( new ColumnConfig( ResumoFaixaDemandaDTO.PROPERTY_CUSTO_DOCENTE_SEMANAL, "Custo Docente Semanal", 130 ) );
		list.add( new ColumnConfig( ResumoFaixaDemandaDTO.PROPERTY_CUSTO_DOCENTE_POR_RECEITA_PERCENT, "Custo Docente / Receita", 130 ) );
		list.add( new ColumnConfig( ResumoFaixaDemandaDTO.PROPERTY_RECEITA_ACUMULADA, "Receita Acumulada", 90 ) );
		list.add( new ColumnConfig( ResumoFaixaDemandaDTO.PROPERTY_CUSTO_DOCENTE_ACUMULADO, "Custo Docente Acumulado", 130 ) );
		list.add( new ColumnConfig( ResumoFaixaDemandaDTO.PROPERTY_CUSTO_DOCENTE_POR_RECEITA_ACUMULADO_PERCENT, "Custo Docente / Receita Acum", 140 ) );
	
		return list;
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
	public Grid< ResumoFaixaDemandaDTO > getGrid() {
		return this.gridPanel;
	}
	
	public void setStore( ListStore< ResumoFaixaDemandaDTO > store )
	{
		this.store = store;
	}

	@Override
	public ListStore< ResumoFaixaDemandaDTO > getStore()
	{
		return this.store;
	}
	
	@Override
	public CampusComboBox getCampusComboBox()
	{
		return this.campusCB;
	}
}
