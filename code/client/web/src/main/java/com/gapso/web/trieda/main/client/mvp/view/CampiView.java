package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.CheckColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.main.client.mvp.presenter.CampiPresenter;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.ComboBoxBoolean;
import com.gapso.web.trieda.shared.util.view.EstadoComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.OperadorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class CampiView
	extends MyComposite
	implements CampiPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< CampusDTO > gridPanel;
	private SimpleFilter filter;
	private TextField< String > nomeBuscaTextField;
	private TextField< String > codigoBuscaTextField;
	private EstadoComboBox estadoBuscaComboBox;
	private TextField< String > municipioBuscaTextField;
	private TextField< String > bairroBuscaTextField;
	private NumberField custoMedioCreditoBuscaField;
	private OperadorComboBox custoMedioCreditoBuscaOperadorCB;
	private ComboBoxBoolean otimizadoTaticoBuscaCB;
	private ComboBoxBoolean otimizadoOperacionalBuscaCB;
	private Button unidadesDeslocamentoBT;
	private Button disponibilidadeBT;
	private ContentPanel panel;
	private GTabItem tabItem;
	private CenarioDTO cenarioDTO;

	public CampiView( CenarioDTO cenarioDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeadingHtml( cenarioDTO.getNome() + " » Campi" );

		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( this.tabItem );
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem( getI18nConstants().campi(),
			Resources.DEFAULTS.campus16() );

		this.tabItem.setContent( this.panel );
	}

	private void createToolBar()
	{
		this.toolBar = new SimpleToolBar( this );
		this.toolBar.add( new SeparatorToolItem() );

		this.unidadesDeslocamentoBT = this.toolBar.createButton(
			getI18nConstants().deslocamentoUnidadesCampus(),
			Resources.DEFAULTS.deslocamentoUnidade16() );

		this.toolBar.add( this.unidadesDeslocamentoBT );
		this.disponibilidadeBT = toolBar.createButton(
			getI18nConstants().disponibilidadesSemanaLetiva(),
			Resources.DEFAULTS.disponibilidade16() );

		this.toolBar.add( this.disponibilidadeBT );
		this.panel.setTopComponent( this.toolBar );
	}

	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );

	    this.gridPanel = new SimpleGrid< CampusDTO >( getColumnList(), this, this.toolBar );
	    this.panel.add( this.gridPanel, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( CampusDTO.PROPERTY_CODIGO, getI18nConstants().codigoCampus(), 100 ) );
		list.add( new ColumnConfig( CampusDTO.PROPERTY_NOME, getI18nConstants().nome(), 200 ) );
		ColumnConfig column = new ColumnConfig( CampusDTO.PROPERTY_VALOR_CREDITO, getI18nConstants().custoMedioCredito(), 170 );
		column.setToolTip("Representa o custo médio ponderado do tempo de aula, ou seja, o custo docente médio do campus, incluindo encargos.");
		list.add( column );
		list.add( new ColumnConfig( CampusDTO.PROPERTY_ESTADO, getI18nConstants().estado(), 100 ) );
		list.add( new ColumnConfig( CampusDTO.PROPERTY_MUNICIPIO, getI18nConstants().municipio(), 100 ) );
		list.add( new ColumnConfig( CampusDTO.PROPERTY_BAIRRO, getI18nConstants().bairro(), 200 ) );	
		list.add( createCheckColumnConfig( CampusDTO.PROPERTY_OTIMIZADO_TATICO, getI18nConstants().otimizadoTatico() + "?", 100, false ) );
		list.add( createCheckColumnConfig( CampusDTO.PROPERTY_OTIMIZADO_OPERACIONAL, getI18nConstants().otimizadoOperacional() + "?", 100, false ) );

		return list;
	}
	
	private CheckColumnConfig createCheckColumnConfig(
			String id, String text, int width, boolean sortable )
	{
		CheckColumnConfig cc = new CheckColumnConfig( id, text, width );
		cc.setSortable( sortable );

		return cc;
	}

	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST, 350 );
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		this.filter = new SimpleFilter();
		this.filter.setLabelWidth(150);

		this.nomeBuscaTextField = new TextField< String >();
		this.nomeBuscaTextField.setFieldLabel( getI18nConstants().nome() );

		this.codigoBuscaTextField = new TextField< String >();
		this.codigoBuscaTextField.setFieldLabel( getI18nConstants().codigo() );

		this.estadoBuscaComboBox = new EstadoComboBox(true);
		this.estadoBuscaComboBox.setFieldLabel( getI18nConstants().estado() );

		this.municipioBuscaTextField = new TextField< String >();
		this.municipioBuscaTextField.setFieldLabel( getI18nConstants().municipio() );

		this.bairroBuscaTextField = new TextField< String >();
		this.bairroBuscaTextField.setFieldLabel( getI18nConstants().bairro() );
		
		this.custoMedioCreditoBuscaOperadorCB = new OperadorComboBox();
		this.custoMedioCreditoBuscaOperadorCB.setFieldLabel(getI18nConstants().custoMedioCredito());
		this.custoMedioCreditoBuscaOperadorCB.setWidth(100);
		
		this.custoMedioCreditoBuscaField = new NumberField();
		this.custoMedioCreditoBuscaField.setWidth( "75" );
		
		this.otimizadoTaticoBuscaCB = new ComboBoxBoolean();
		this.otimizadoTaticoBuscaCB.setFieldLabel(getI18nConstants().otimizadoTatico());
		
		this.otimizadoOperacionalBuscaCB = new ComboBoxBoolean();
		this.otimizadoOperacionalBuscaCB.setFieldLabel(getI18nConstants().otimizadoOperacional());

		this.filter.addField( nomeBuscaTextField );
		this.filter.addField( codigoBuscaTextField ); 
		this.filter.addField( municipioBuscaTextField ); 
		this.filter.addField( estadoBuscaComboBox ); 
		this.filter.addField( bairroBuscaTextField ); 
		this.filter.addMultiField(this.custoMedioCreditoBuscaOperadorCB, this.custoMedioCreditoBuscaField);
		this.filter.addField( otimizadoTaticoBuscaCB ); 
		this.filter.addField( otimizadoOperacionalBuscaCB ); 

		this.panel.add( this.filter, bld );
	}

	@Override
	public Button getNewButton()
	{
		return this.toolBar.getNewButton();
	}

	@Override
	public Button getEditButton()
	{
		return this.toolBar.getEditButton();
	}

	@Override
	public Button getRemoveButton()
	{
		return this.toolBar.getRemoveButton();
	}

	@Override
	public Button getImportExcelButton()
	{
		return this.toolBar.getImportExcelButton();
	}

	@Override
	public MenuItem getExportXlsExcelButton()
	{
		return (MenuItem) this.toolBar.getExportExcelButton().getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getExportXlsxExcelButton()
	{
		return (MenuItem) this.toolBar.getExportExcelButton().getMenu().getItem(1);
	}

	@Override
	public SimpleGrid< CampusDTO > getGrid()
	{
		return this.gridPanel;
	}

	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< CampusDTO > > proxy )
	{
		this.gridPanel.setProxy( proxy );
	}

	@Override
	public TextField< String > getNomeBuscaTextField()
	{
		return this.nomeBuscaTextField;
	}

	@Override
	public TextField< String > getCodigoBuscaTextField()
	{
		return this.codigoBuscaTextField;
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
	public EstadoComboBox getEstadoBuscaComboBox()
	{
		return this.estadoBuscaComboBox;
	}

	@Override
	public TextField< String > getMunicipioBuscaTextField()
	{
		return this.municipioBuscaTextField;
	}

	@Override
	public TextField< String > getBairroBuscaTextField()
	{
		return this.bairroBuscaTextField;
	}

	@Override
	public Button getUnidadeDeslocamentosButton()
	{
		return this.unidadesDeslocamentoBT;
	}

	@Override
	public Button getDisponibilidadeButton()
	{
		return this.disponibilidadeBT;
	}

	@Override
	public NumberField getCustoMedioCreditoBuscaField() {
		return custoMedioCreditoBuscaField;
	}

	@Override
	public OperadorComboBox getCustoMedioCreditoBuscaOperadorCB() {
		return custoMedioCreditoBuscaOperadorCB;
	}

	@Override
	public ComboBoxBoolean getOtimizadoTaticoBuscaCB() {
		return otimizadoTaticoBuscaCB;
	}

	@Override
	public ComboBoxBoolean getOtimizadoOperacionalBuscaCB() {
		return otimizadoOperacionalBuscaCB;
	}
}
