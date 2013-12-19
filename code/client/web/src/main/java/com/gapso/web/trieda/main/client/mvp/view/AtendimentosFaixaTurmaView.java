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
import com.gapso.web.trieda.main.client.mvp.presenter.AtendimentosFaixaTurmaPresenter;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaTurmaDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.google.gwt.i18n.client.NumberFormat;

public class AtendimentosFaixaTurmaView extends MyComposite
	implements AtendimentosFaixaTurmaPresenter.Display
{
	
	private SimpleToolBar toolBar;
	private Grid< AtendimentoFaixaTurmaDTO > gridPanel;
	private ListStore< AtendimentoFaixaTurmaDTO > store = new ListStore< AtendimentoFaixaTurmaDTO >();
		private ContentPanel panel;
	private GTabItem tabItem;
	private CampusComboBox campusCB;
	private CenarioDTO cenarioDTO;
	
	public AtendimentosFaixaTurmaView(CenarioDTO cenarioDTO)
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
		panel.setHeadingHtml( cenarioDTO.getNome() + " » Atendimentos por Faixa de Turma" );
	}
	
	private void createTabItem()
	{
		tabItem = new GTabItem(
			"Atendimentos por Faixa de Turma", Resources.DEFAULTS.disciplinaCurriculo16() );
	
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
	    this.gridPanel = new Grid< AtendimentoFaixaTurmaDTO >( this.getStore(), new ColumnModel( this.getColumnList()) );
	    
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
	
		list.add( new ColumnConfig( AtendimentoFaixaTurmaDTO.PROPERTY_CAMPUS_NOME, "Campus", 80 ) );
		list.add( createIntegerColumnConfig( AtendimentoFaixaTurmaDTO.PROPERTY_TAMANHO_TURMA, "Créditos Demandados P1", 140 ) );
		list.add( createIntegerColumnConfig( AtendimentoFaixaTurmaDTO.PROPERTY_ATENDIMENTO_CRED, "Atendimento (cred)", 140 ) );
		list.add( createIntegerColumnConfig( AtendimentoFaixaTurmaDTO.PROPERTY_ATENDIMENTO_CRED_ACUM, "Atendimento Acumulado (cred) ", 140 ) );
		list.add( createIntegerColumnConfig( AtendimentoFaixaTurmaDTO.PROPERTY_ATENDIMENTO_ALUNO, "Atendimento (alunos)", 140 ) );
		list.add( createIntegerColumnConfig( AtendimentoFaixaTurmaDTO.PROPERTY_ATENDIMENTO_ALUNO_ACUM, "Atendimento Acumulado (alunos)", 140 ) );
		list.add( createDecimalColumnConfig( AtendimentoFaixaTurmaDTO.PROPERTY_MEDIA_TURMA, "Media alunos por turma", 130 ) );
		list.add( createDecimalColumnConfig( AtendimentoFaixaTurmaDTO.PROPERTY_CREDITOS_PAGOS, "Creditos pagos", 90 ) );
		list.add( createDecimalColumnConfig( AtendimentoFaixaTurmaDTO.PROPERTY_RECEITA_SEMANAL, "Receita Semanal", 90 ) );
		list.add( createDecimalColumnConfig( AtendimentoFaixaTurmaDTO.PROPERTY_CUSTO_DOCENTE_SEMANAL, "Custo Docente Semanal", 130 ) );
		list.add( new ColumnConfig( AtendimentoFaixaTurmaDTO.PROPERTY_CUSTO_DOCENTE_POR_RECEITA_PERCENT, "Custo Docente / Receita", 130 ) );
		list.add( createDecimalColumnConfig( AtendimentoFaixaTurmaDTO.PROPERTY_RECEITA_ACUMULADA, "Receita Acumulada", 110 ) );
		list.add( createDecimalColumnConfig( AtendimentoFaixaTurmaDTO.PROPERTY_CUSTO_DOCENTE_ACUMULADO, "Custo Docente Acumulado", 130 ) );
		list.add( new ColumnConfig( AtendimentoFaixaTurmaDTO.PROPERTY_CUSTO_DOCENTE_POR_RECEITA_ACUMULADO_PERCENT, "Custo Docente / Receita Acum", 140 ) );
		
		
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
	
	private ColumnConfig createDecimalColumnConfig(
			String id, String text, int width)
	{
		String pattern = "#,##0.00";
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
	public Grid< AtendimentoFaixaTurmaDTO > getGrid() {
		return this.gridPanel;
	}
	
	public void setStore( ListStore< AtendimentoFaixaTurmaDTO > store )
	{
		this.store = store;
	}
	
	@Override
	public ListStore< AtendimentoFaixaTurmaDTO > getStore()
	{
		return this.store;
	}
	
	@Override
	public CampusComboBox getCampusComboBox()
	{
		return this.campusCB;
	}
}
