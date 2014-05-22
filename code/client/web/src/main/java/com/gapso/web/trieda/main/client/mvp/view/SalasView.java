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
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.main.client.mvp.presenter.SalasPresenter;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.TipoSalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.OperadorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.TipoSalaComboBox;
import com.gapso.web.trieda.shared.util.view.UnidadeComboBox;
import com.google.gwt.i18n.client.NumberFormat;

public class SalasView
	extends MyComposite
	implements SalasPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< SalaDTO > gridPanel;
	private SimpleFilter filter;
	private ContentPanel panel;
	private GTabItem tabItem;
	private Button disciplinasAssociadasBT;
	private Button disponibilidadeBT;
	private Button associarDisciplinasBT;
	private UnidadeComboBox unidadeCB;
	private CampusComboBox campusCB;
	private TipoSalaComboBox tipoSalaCB;
	private OperadorComboBox operadorCapacidadeInstaladaCB;
	private OperadorComboBox operadorCapacidadeMaximaCB;
	private OperadorComboBox operadorCustoOperacaoCB;
	private NumberField capacidadeInstalada;
	private NumberField capacidadeMaxima;
	private NumberField custoOperacao;
	private TextField< String > numeroTF;
	private TextField< String > descricaoTF;
	private TextField< String > andarTF;

	private CampusDTO campusDTO;
	private UnidadeDTO unidadeDTO;
	private CenarioDTO cenarioDTO;
	private TipoSalaDTO tipoSalaDTO; 

	public SalasView( CenarioDTO cenarioDTO )
	{
		this( cenarioDTO, null, null );
	}

	public SalasView( CenarioDTO cenarioDTO,
		CampusDTO campusDTO, UnidadeDTO unidadeDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.campusDTO = campusDTO;
		this.unidadeDTO = unidadeDTO;

		initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeadingHtml( cenarioDTO.getNome() + " » Ambientes" );

		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( this.tabItem );
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			getI18nConstants().salas(), Resources.DEFAULTS.sala16() );

		this.tabItem.setContent( this.panel );
	}

	private void createToolBar()
	{
		this.toolBar = new SimpleToolBar( this );
		this.toolBar.add( new SeparatorToolItem() );

		this.disciplinasAssociadasBT = this.toolBar.createButton(
			getI18nConstants() .disciplinasAssociadas(),
			Resources.DEFAULTS.disciplina16() );

		this.toolBar.add( this.disciplinasAssociadasBT );
		this.disponibilidadeBT = this.toolBar.createButton(
			getI18nConstants().disponibilidadesSemanaLetiva(),
			Resources.DEFAULTS.disponibilidade16() );

		this.toolBar.add( this.disponibilidadeBT );
		
		this.associarDisciplinasBT = this.toolBar.createButton(getI18nConstants().associacaoDisciplinas(),
				Resources.DEFAULTS.associacaoDisciplinaSala16());
		this.toolBar.add(this.associarDisciplinasBT);
		
		this.panel.setTopComponent( this.toolBar );
	}

	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
		bld.setMargins( new Margins( 5, 5, 5, 5 ) );

		this.gridPanel = new SimpleGrid< SalaDTO >( getColumnList(), this, this.toolBar );
		this.panel.add( this.gridPanel, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( SalaDTO.PROPERTY_CODIGO,
			getI18nConstants().codigoSala(), 100 ) );

		list.add( new ColumnConfig( SalaDTO.PROPERTY_TIPO_STRING,
			getI18nConstants().tipo(), 100 ) );
		list.add( new ColumnConfig( SalaDTO.PROPERTY_UNIDADE_STRING,
			getI18nConstants().codigoUnidade(), 100 ) );
		list.add( new ColumnConfig( SalaDTO.PROPERTY_NUMERO,
			getI18nConstants().numero(), 100 ) );
		list.add( new ColumnConfig( SalaDTO.PROPERTY_DESCRICAO,
				getI18nConstants().descricao(), 100 ) );
		list.add( new ColumnConfig( SalaDTO.PROPERTY_ANDAR,
			getI18nConstants().andar(), 100 ) );
		list.add( createIntegerColumnConfig( SalaDTO.PROPERTY_CAPACIDADE_INSTALADA,
			getI18nConstants().capacidadeInstaladaAlunos(), 160, "É a capacidade do ambiente em número de alunos, em seu uso normal." ) );
		list.add( createIntegerColumnConfig( SalaDTO.PROPERTY_CAPACIDADE_MAX,
				getI18nConstants().capacidadeMaxAlunos(), 160, "É a capacidade máxima do ambiente, em número de alunos, em situações extremas." ) );
		list.add( createDecimalColumnConfig( SalaDTO.PROPERTY_CUSTO_OPERACAO_CRED,
				getI18nConstants().custoOperacaoCred(), 185, "Custo de uso do ambiente por tempo de aula. Pode considerar custo de aluguel, limpeza, energia elétrica, móveis e equipamentos instalados, etc." ) );

		return list;
	}

	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST, 350);
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		this.filter = new SimpleFilter();
		this.filter.setLabelWidth(150);

		this.campusCB = new CampusComboBox(cenarioDTO);
		this.campusCB.setValue( this.campusDTO );
		this.unidadeCB = new UnidadeComboBox( this.campusCB );
		this.unidadeCB.setValue( this.unidadeDTO );
		this.tipoSalaCB = new TipoSalaComboBox(cenarioDTO);
		this.tipoSalaCB.setValue(this.tipoSalaDTO);
		this.operadorCapacidadeInstaladaCB = new OperadorComboBox();
		this.operadorCapacidadeInstaladaCB.setWidth(100);
		this.operadorCapacidadeInstaladaCB.setFieldLabel(getI18nConstants().capacidadeInstaladaAlunos());
		this.capacidadeInstalada = new NumberField();
		this.capacidadeInstalada.setWidth(75);
		
		this.operadorCapacidadeMaximaCB = new OperadorComboBox();
		this.operadorCapacidadeMaximaCB.setWidth(100);
		this.operadorCapacidadeMaximaCB.setFieldLabel(getI18nConstants().capacidadeMaxAlunos());
		this.capacidadeMaxima = new NumberField();
		this.capacidadeMaxima.setWidth(75);
		
		this.operadorCustoOperacaoCB = new OperadorComboBox();
		this.operadorCustoOperacaoCB.setWidth(100);
		this.operadorCustoOperacaoCB.setFieldLabel(getI18nConstants().custoOperacaoCred());
		this.custoOperacao = new NumberField();
		this.custoOperacao.setWidth(75);
		
		this.numeroTF = new TextField< String >();
		this.numeroTF.setFieldLabel(getI18nConstants().numero());
		this.descricaoTF = new TextField< String >();
		this.descricaoTF.setFieldLabel(getI18nConstants().descricao());
		this.andarTF = new TextField< String >();
		this.andarTF.setFieldLabel(getI18nConstants().andar());

		this.filter.addField( this.campusCB );
		this.filter.addField( this.unidadeCB );
		this.filter.addField( this.tipoSalaCB );
		this.filter.addField( this.numeroTF );
		this.filter.addField( this.descricaoTF );
		this.filter.addField( this.andarTF );
		this.filter.addMultiField(operadorCapacidadeInstaladaCB, capacidadeInstalada);
		this.filter.addMultiField(operadorCapacidadeMaximaCB, capacidadeMaxima);
		this.filter.addMultiField(operadorCustoOperacaoCB, custoOperacao);
		

		this.panel.add( this.filter, bld );
	}
	
	private ColumnConfig createIntegerColumnConfig(
			String id, String text, int width, String toolTip )
	{
		String pattern = "#,###";
		ColumnConfig cc = new ColumnConfig( id, text, width );
		cc.setNumberFormat(NumberFormat.getFormat(pattern));
		cc.setToolTip(toolTip);

		return cc;
	}
	
	private ColumnConfig createDecimalColumnConfig(
			String id, String text, int width, String toolTip)
	{
		String pattern = "#,##0.00";
		ColumnConfig cc = new ColumnConfig( id, text, width );
		cc.setNumberFormat(NumberFormat.getFormat(pattern));
		cc.setToolTip(toolTip);

		return cc;
	}

	@Override
	public Button getNewButton()
	{
		return toolBar.getNewButton();
	}

	@Override
	public Button getEditButton()
	{
		return toolBar.getEditButton();
	}

	@Override
	public Button getRemoveButton()
	{
		return toolBar.getRemoveButton();
	}

	@Override
	public Button getImportExcelButton()
	{
		return toolBar.getImportExcelButton();
	}

	@Override
	public MenuItem getExportXlsExcelButton()
	{
		return (MenuItem) toolBar.getExportExcelButton().getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getExportXlsxExcelButton()
	{
		return (MenuItem) toolBar.getExportExcelButton().getMenu().getItem(1);
	}

	@Override
	public GTabItem getGTabItem()
	{
		return tabItem;
	}

	@Override
	public SimpleGrid< SalaDTO > getGrid()
	{
		return gridPanel;
	}

	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< SalaDTO > > proxy )
	{
		gridPanel.setProxy( proxy );
	}

	@Override
	public Button getDisciplinasAssociadasButton()
	{
		return disciplinasAssociadasBT;
	}

	@Override
	public Button getDisponibilidadeButton()
	{
		return disponibilidadeBT;
	}

	@Override
	public UnidadeComboBox getUnidadeCB()
	{
		return unidadeCB;
	}

	@Override
	public CampusComboBox getCampusCB()
	{
		return campusCB;
	}

	@Override
	public Button getSubmitBuscaButton()
	{
		return filter.getSubmitButton();
	}

	@Override
	public Button getResetBuscaButton()
	{
		return filter.getResetButton();
	}

	@Override
	public OperadorComboBox getOperadorCapacidadeInstaladaCB() {
		return operadorCapacidadeInstaladaCB;
	}

	@Override
	public OperadorComboBox getOperadorCapacidadeMaximaCB() {
		return operadorCapacidadeMaximaCB;
	}

	@Override
	public OperadorComboBox getOperadorCustoOperacaoCB() {
		return operadorCustoOperacaoCB;
	}

	@Override
	public NumberField getCapacidadeInstalada() {
		return capacidadeInstalada;
	}

	@Override
	public NumberField  getCapacidadeMaxima() {
		return capacidadeMaxima;
	}

	@Override
	public NumberField  getCustoOperacao() {
		return custoOperacao;
	}

	@Override
	public TextField<String> getNumeroTF() {
		return numeroTF;
	}

	@Override
	public TextField<String> getDescricaoTF() {
		return descricaoTF;
	}

	@Override
	public TextField<String> getAndarTF() {
		return andarTF;
	}
	
	@Override
	public TipoSalaComboBox getTipoSalaCB() {
		return tipoSalaCB;
	}

	@Override
	public Button getAssociarDisciplinasButton() {
		return associarDisciplinasBT;
	}
}
