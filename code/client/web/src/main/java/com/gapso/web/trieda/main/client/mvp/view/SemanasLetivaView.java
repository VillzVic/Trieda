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
import com.gapso.web.trieda.main.client.mvp.presenter.SemanasLetivaPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.ComboBoxBoolean;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.OperadorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class SemanasLetivaView
	extends MyComposite
	implements SemanasLetivaPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< SemanaLetivaDTO > gridPanel;
	private SimpleFilter filter;
	private TextField< String > codigoBuscaTextField;
	private TextField< String > descricaoBuscaTextField;
	private NumberField tempoBuscaTextField;
	private OperadorComboBox tempoBuscaCB;
	private ComboBoxBoolean permiteIntervaloAulaBuscaCB;
	private ContentPanel panel;
	private GTabItem tabItem;
	private Button diasDeAulaButton;
	private CenarioDTO cenarioDTO;

	public SemanasLetivaView( CenarioDTO cenarioDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeadingHtml( cenarioDTO.getNome() + " » Semanas Letivas" );

		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( this.tabItem );
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			"Semana Letiva", Resources.DEFAULTS.semanaLetiva16() );

		this.tabItem.setContent( this.panel );
	}
	
	private void createToolBar()
	{
		this.toolBar = new SimpleToolBar( this );
		this.toolBar.add( new SeparatorToolItem() );
		this.diasDeAulaButton = this.toolBar.createButton(
			"Dias e Horários de Aula", Resources.DEFAULTS.diaHorarioAula16() );
		this.toolBar.add( this.diasDeAulaButton );
		this.panel.setTopComponent( this.toolBar );
	}

	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );

	    this.gridPanel = new SimpleGrid< SemanaLetivaDTO >( getColumnList(), this, this.toolBar );
	    this.panel.add( this.gridPanel, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( SemanaLetivaDTO.PROPERTY_CODIGO, "Codigo", 120 ) );
		list.add( new ColumnConfig( SemanaLetivaDTO.PROPERTY_DESCRICAO, "Descrição", 150 ) );
		list.add( new ColumnConfig( SemanaLetivaDTO.PROPERTY_TEMPO, "Duração do Tempo de Aula (min)", 130 ) );
		
		CheckColumnConfig column = new CheckColumnConfig( SemanaLetivaDTO.PROPERTY_PERMITE_INTERVALO_AULA, "Permite Intervalo Entre Aulas", 150 );
		column.setToolTip("Quando o Trieda cria uma aula de dois tempos consecutivos (ou dois créditos), " +
				"por padrão, não é permitido que haja um intervalo posicionado entre estes dois tempos de aula. No entanto, caso o parâmetro \"Permite Intervalo entre Aulas\"" +
				" esteja marcado para a semana letiva em questão, então, o Trieda poderá criar aulas de dois tempos com um intervalo entre os tempos da aula.");
		list.add( column );
		

		return list;
	}

	private void createFilter()
	{
		BorderLayoutData bld
			= new BorderLayoutData( LayoutRegion.EAST, 350 );

		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		this.filter = new SimpleFilter();
		this.filter.setLabelWidth(150);
		this.codigoBuscaTextField = new TextField< String >();
		this.codigoBuscaTextField.setFieldLabel( "Código" );
		this.descricaoBuscaTextField = new TextField< String >();
		this.descricaoBuscaTextField.setFieldLabel( "Descrição" );
		this.tempoBuscaCB = new OperadorComboBox();
		this.tempoBuscaCB.setFieldLabel("Tempo");
		this.tempoBuscaCB.setWidth(100);
		this.tempoBuscaTextField = new NumberField();
		this.tempoBuscaTextField.setWidth( "75" );
		this.permiteIntervaloAulaBuscaCB = new ComboBoxBoolean();
		this.permiteIntervaloAulaBuscaCB.setFieldLabel(getI18nConstants().permiteIntervaloAula());
		
		
		this.filter.addField( this.codigoBuscaTextField );
		this.filter.addField( this.descricaoBuscaTextField );
		this.filter.addField( this.permiteIntervaloAulaBuscaCB );
		this.filter.addMultiField( this.tempoBuscaCB, this.tempoBuscaTextField );

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
	public SimpleGrid< SemanaLetivaDTO > getGrid()
	{
		return this.gridPanel;
	}

	@Override
	public Button getDiasDeAulaButton()
	{
		return this.diasDeAulaButton;
	}

	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< SemanaLetivaDTO > > proxy )
	{
		this.gridPanel.setProxy( proxy );
	}

	@Override
	public TextField< String > getCodigoBuscaTextField()
	{
		return this.codigoBuscaTextField;
	}

	@Override
	public TextField< String > getDescricaoBuscaTextField()
	{
		return this.descricaoBuscaTextField;
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
	public NumberField getTempoBuscaTextField() {
		return this.tempoBuscaTextField;
	}

	@Override
	public OperadorComboBox getTempoBuscaCB() {
		return this.tempoBuscaCB;
	}

	@Override
	public ComboBoxBoolean getPermiteIntervaloAulaBuscaCB() {
		return this.permiteIntervaloAulaBuscaCB;
	}
}
