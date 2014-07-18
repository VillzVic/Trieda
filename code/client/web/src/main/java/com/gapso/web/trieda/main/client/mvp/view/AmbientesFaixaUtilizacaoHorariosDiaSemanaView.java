package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.charts.client.Chart;
import com.extjs.gxt.charts.client.model.ChartModel;
import com.extjs.gxt.charts.client.model.axis.XAxis;
import com.extjs.gxt.charts.client.model.axis.YAxis;
import com.extjs.gxt.charts.client.model.charts.CylinderBarChart;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.presenter.AmbientesFaixaUtilizacaoHorariosDiaSemanaPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioQuantidadeDoubleDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.google.gwt.i18n.client.NumberFormat;

public class AmbientesFaixaUtilizacaoHorariosDiaSemanaView extends MyComposite
	implements AmbientesFaixaUtilizacaoHorariosDiaSemanaPresenter.Display
{

	private SimpleToolBar toolBar;
	private Grid< RelatorioQuantidadeDoubleDTO > gridPanel;
	private ListStore< RelatorioQuantidadeDoubleDTO > store = new ListStore< RelatorioQuantidadeDoubleDTO >();
		private ContentPanel panel;
	private GTabItem tabItem;
	private CampusComboBox campusCB;
	private CenarioDTO cenarioDTO;
    private Chart chart;
	
	public AmbientesFaixaUtilizacaoHorariosDiaSemanaView(CenarioDTO cenarioDTO)
	{
		this.cenarioDTO = cenarioDTO;
		initUI();
		createToolBar();
		createForm();
		createGrid();
		createChart();
		createTabItem();
		initComponent( this.tabItem );
	}
	
	private void initUI()
	{
		panel = new ContentPanel( new BorderLayout() );
		panel.setHeadingHtml( cenarioDTO.getNome() + " » Utilização dos Horários dos Ambientes por Dia da Semana" );
	}
	
	private void createTabItem()
	{
		tabItem = new GTabItem(
			"Utilização dos Horários dos Ambientes por Dia da Semana", Resources.DEFAULTS.sala16() );
	
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
	    this.gridPanel = new Grid< RelatorioQuantidadeDoubleDTO >( this.getStore(), new ColumnModel( this.getColumnList()) );
	    
	    ContentPanel contentPanel = new ContentPanel( new FitLayout() );
	    contentPanel.setHeaderVisible( false );
	    contentPanel.add( this.gridPanel );
	
	    BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5 ) );
	
	    this.panel.add( contentPanel, bld );
	}
	
	private void createChart()
	{
	    BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST );
	    bld.setMargins( new Margins( 5 ) );
	    bld.setSize(400);
	    chart = new Chart("gwt/chart/open-flash-chart.swf");
	    chart.setBorders(true);  
	    ContentPanel contentPanel = new ContentPanel( new FitLayout() );
	    contentPanel.setHeaderVisible( false );
	    contentPanel.add( chart );
	    chart.setChartModel( getChartModel() );  
	    this.panel.add( contentPanel, bld );
	}
	
	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();
		
		list.add( new ColumnConfig( RelatorioQuantidadeDoubleDTO.PROPERTY_FAIXA_CREDITO, "Tipo Ambiente", 140 ) );
		list.add( createDecimalColumnConfig( RelatorioQuantidadeDoubleDTO.PROPERTY_QUANTIDADE, "Segunda", 80 ) );
		list.add( createDecimalColumnConfig( RelatorioQuantidadeDoubleDTO.PROPERTY_QUANTIDADE2, "Terça", 80 ) );
		list.add( createDecimalColumnConfig( RelatorioQuantidadeDoubleDTO.PROPERTY_QUANTIDADE3, "Quarta", 80 ) );
		list.add( createDecimalColumnConfig( RelatorioQuantidadeDoubleDTO.PROPERTY_QUANTIDADE4, "Quinta", 80 ) );
		list.add( createDecimalColumnConfig( RelatorioQuantidadeDoubleDTO.PROPERTY_QUANTIDADE5, "Sexta", 80 ) );
		list.add( createDecimalColumnConfig( RelatorioQuantidadeDoubleDTO.PROPERTY_QUANTIDADE6, "Sábado", 80 ) );
		
		return list;
	}
	
	private ColumnConfig createDecimalColumnConfig(
			String id, String text, int width)
	{
		String pattern = "#,##0.00";
	    GridCellRenderer<RelatorioQuantidadeDoubleDTO> change = new GridCellRenderer<RelatorioQuantidadeDoubleDTO>() {

			@Override
			public Object render(RelatorioQuantidadeDoubleDTO model, String property,
					com.extjs.gxt.ui.client.widget.grid.ColumnData config,
					int rowIndex, int colIndex,
					ListStore<RelatorioQuantidadeDoubleDTO> store, Grid<RelatorioQuantidadeDoubleDTO> grid) {
			    
		          Double val = (Double) model.get(property);  

		          return val + "%";
			}
	    }; 
		
		ColumnConfig cc = new ColumnConfig( id, text, width );
		cc.setNumberFormat(NumberFormat.getFormat(pattern));
		cc.setRenderer(change);

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
	public Grid< RelatorioQuantidadeDoubleDTO > getGrid() {
		return this.gridPanel;
	}
	
	public void setStore( ListStore< RelatorioQuantidadeDoubleDTO > store )
	{
		this.store = store;
	}
	
	@Override
	public ListStore< RelatorioQuantidadeDoubleDTO > getStore()
	{
		return this.store;
	}
	
	@Override
	public CampusComboBox getCampusComboBox()
	{
		return this.campusCB;
	}
	
	@Override
	public Chart getChart()
	{
		return this.chart;
	}
	
    public ChartModel getChartModel() {  
      ChartModel cm = new ChartModel("Utilização dos Horários por Semana",  
          "font-size: 14px; font-family: Verdana; text-align: center;");  
      cm.setBackgroundColour("#ffffff");  
      XAxis xa = new XAxis();  
      xa.setZDepth3D(5);  
      xa.setTickHeight(4);  
      xa.setOffset(true);  
      xa.setColour("#909090");  
      cm.setXAxis(xa);  
      YAxis ya = new YAxis();  
      ya.setSteps(10);  
      ya.setMax(100);  
      cm.setYAxis(ya);  
      CylinderBarChart bchart = new CylinderBarChart();  
      bchart.setColour("#440088");  
      bchart.setAlpha(.8f);
      bchart.setTooltip("#val#");  
      cm.addChartConfig(bchart);  
      return cm;  
    }
}