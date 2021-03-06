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
import com.gapso.web.trieda.main.client.mvp.presenter.ResumoAtendimentosFaixaDemandaPresenter;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaDemandaDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.google.gwt.i18n.client.NumberFormat;

public class ResumoAtendimentosFaixaDemandaView extends MyComposite
	implements ResumoAtendimentosFaixaDemandaPresenter.Display
{

	private SimpleToolBar toolBar;
	private Grid< AtendimentoFaixaDemandaDTO > gridPanel;
	private ListStore< AtendimentoFaixaDemandaDTO > store = new ListStore< AtendimentoFaixaDemandaDTO >();
 	private ContentPanel panel;
	private GTabItem tabItem;
	private CampusComboBox campusCB;
	private CenarioDTO cenarioDTO;
	
	public ResumoAtendimentosFaixaDemandaView(CenarioDTO cenarioDTO)
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
		panel.setHeadingHtml( cenarioDTO.getNome() + " » Resumo de Atendimentos por Faixa de Demanda" );
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
			false, true, false, false, true, this );
		
		getEditButton().setEnabled(false);
	
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
	    this.gridPanel = new Grid< AtendimentoFaixaDemandaDTO >( this.getStore(), new ColumnModel( this.getColumnList()) );
	    
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

		list.add( new ColumnConfig( AtendimentoFaixaDemandaDTO.PROPERTY_DEMANDA_DISCIPLINA, "Demanda da disciplina", 120 ) );
		list.add( createIntegerColumnConfig( AtendimentoFaixaDemandaDTO.PROPERTY_DEMANDA_P1, "Demanda P1", 90 ) );
		list.add( createIntegerColumnConfig( AtendimentoFaixaDemandaDTO.PROPERTY_ATENDIMENTO_P1, "Atendimento P1", 90 ) );
		list.add( new ColumnConfig( AtendimentoFaixaDemandaDTO.PROPERTY_ATENDIMENTO_PERCENT_P1, "% Atendimento P1", 100 ) );
		list.add( createIntegerColumnConfig( AtendimentoFaixaDemandaDTO.PROPERTY_ATENDIMENTO_SOMA, "Atendimento P1+P2", 110 ) );
		list.add( new ColumnConfig( AtendimentoFaixaDemandaDTO.PROPERTY_ATENDIMENTO_SOMA_PERCENT, " % Atendimento P1+P2", 120 ) );
		list.add( createIntegerColumnConfig( AtendimentoFaixaDemandaDTO.PROPERTY_DEMANDA_ACUM_P1, "Demanda P1 acum", 110 ) );
		list.add( createIntegerColumnConfig( AtendimentoFaixaDemandaDTO.PROPERTY_ATENDIMENTO_SOMA_ACUM, "Atendimento P1+P2 acum", 140 ) );
		list.add( new ColumnConfig( AtendimentoFaixaDemandaDTO.PROPERTY_ATENDIMENTO_ACUM_PERCENT, "% Atendimento acumulado", 140 ) );
		list.add( createIntegerColumnConfig( AtendimentoFaixaDemandaDTO.PROPERTY_TURMAS_ABERTAS, "Turmas abertas", 90 ) );
		list.add( createDecimalColumnConfig( AtendimentoFaixaDemandaDTO.PROPERTY_MEDIA_TURMA, "Media alunos por turma", 130 ) );
		list.add( createIntegerColumnConfig( AtendimentoFaixaDemandaDTO.PROPERTY_CREDITOS_PAGOS, "Creditos pagos", 90 ) );
		list.add( createDecimalColumnConfig( AtendimentoFaixaDemandaDTO.PROPERTY_RECEITA_SEMANAL, "Receita Semanal", 90 ) );
		list.add( createDecimalColumnConfig( AtendimentoFaixaDemandaDTO.PROPERTY_CUSTO_DOCENTE_SEMANAL, "Custo Docente Semanal", 130 ) );
		list.add( new ColumnConfig( AtendimentoFaixaDemandaDTO.PROPERTY_CUSTO_DOCENTE_POR_RECEITA_PERCENT, "Custo Docente / Receita", 130 ) );
		list.add( createDecimalColumnConfig( AtendimentoFaixaDemandaDTO.PROPERTY_RECEITA_ACUMULADA, "Receita Acumulada", 110 ) );
		list.add( createDecimalColumnConfig( AtendimentoFaixaDemandaDTO.PROPERTY_CUSTO_DOCENTE_ACUMULADO, "Custo Docente Acumulado", 130 ) );
		list.add( new ColumnConfig( AtendimentoFaixaDemandaDTO.PROPERTY_CUSTO_DOCENTE_POR_RECEITA_ACUMULADO_PERCENT, "Custo Docente / Receita Acum", 140 ) );
	
		return list;
	}
	
	private ColumnConfig createDecimalColumnConfig(
			String id, String text, int width)
	{
		String pattern = "#,##0.00";
		ColumnConfig cc = new ColumnConfig( id, text, width );
		cc.setNumberFormat(NumberFormat.getFormat(pattern));

		return cc;
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
	public Button getEditButton()
	{
		return this.toolBar.getEditButton();
	}
	
	@Override
	public Grid< AtendimentoFaixaDemandaDTO > getGrid() {
		return this.gridPanel;
	}
	
	public void setStore( ListStore< AtendimentoFaixaDemandaDTO > store )
	{
		this.store = store;
	}

	@Override
	public ListStore< AtendimentoFaixaDemandaDTO > getStore()
	{
		return this.store;
	}
	
	@Override
	public CampusComboBox getCampusComboBox()
	{
		return this.campusCB;
	}
}
