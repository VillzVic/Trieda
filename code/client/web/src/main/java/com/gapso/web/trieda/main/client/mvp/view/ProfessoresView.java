package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
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
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessoresPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.AreaTitulacaoComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.OperadorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.TipoContratoComboBox;
import com.gapso.web.trieda.shared.util.view.TitulacaoComboBox;

public class ProfessoresView extends MyComposite
	implements ProfessoresPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< ProfessorDTO > gridPanel;
	private SimpleFilter filter;
	private TextField< String > cpfBuscaTF;
	private TextField< String > nomeBuscaTF;
	private NumberField cargaHorariaMinBuscaField;
	private OperadorComboBox cargaHorariaMinOperadorCB;
	private NumberField cargaHorariaMaxBuscaField;
	private OperadorComboBox cargaHorariaMaxOperadorCB;
	private NumberField notaDesempenhoBuscaField;
	private OperadorComboBox notaDesempenhoBuscaOperadorCB;
	private NumberField cargaHorariaAnteriorBuscaField;
	private OperadorComboBox cargaHorariaAnteriorOperadorCB;
	private NumberField custoCreditoSemanalBuscaField;
	private OperadorComboBox custoCreditoSemanalBuscaOperadorCB;
	private NumberField maxDiasSemanaBuscaField;
	private OperadorComboBox maxDiasSemanaOperadorCB;
	private NumberField minCreditosSemanaisBuscaField;
	private OperadorComboBox minCreditosSemanaisOperadorCB;
	private NumberField totalCreditosSemanaisBuscaField;
	private OperadorComboBox totalCreditosSemanaisBuscaOperadorCB;
	private NumberField cargaHorariaSemanalBuscaField;
	private OperadorComboBox cargaHorariaSemanalOperadorCB;
	private TipoContratoComboBox tipoContratoBuscaCB;
	private TitulacaoComboBox titulacaoBuscaCB;
	private AreaTitulacaoComboBox areaTitulacaoBuscaCB;
	private Button disponibilidadeBT;
	private Button gradeHorariaBT;
	private ContentPanel panel;
	private GTabItem tabItem;
	private CenarioDTO cenarioDTO;
	
	public ProfessoresView( CenarioDTO cenarioDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeadingHtml( cenarioDTO.getNome() + " » Professores" );

		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( this.tabItem );
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			"Professores", Resources.DEFAULTS.professor16() );

		this.tabItem.setContent( this.panel );
	}

	private void createToolBar()
	{
		toolBar = new SimpleToolBar( this );
		toolBar.add( new SeparatorToolItem() );
		disponibilidadeBT = toolBar.createButton(
			getI18nConstants().disponibilidadesSemanaLetiva(),
			Resources.DEFAULTS.disponibilidade16() );
		gradeHorariaBT = toolBar.createButton(
				"Grade Horária do Professor",
				Resources.DEFAULTS.saidaProfessor16() );

		disponibilidadeBT.disable();
		gradeHorariaBT.disable();
		toolBar.add( disponibilidadeBT );
		toolBar.add( gradeHorariaBT );
		panel.setTopComponent( toolBar );
	}

	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
		bld.setMargins( new Margins( 5, 5, 5, 5 ) );

		this.gridPanel = new SimpleGrid< ProfessorDTO >( getColumnList(), this, this.toolBar ){
			
			@Override
			protected void afterRender() {
				super.afterRender();
				
				this.getSelectionModel().addSelectionChangedListener(new SelectionChangedListener<ProfessorDTO>() {
					
					@Override
				    public void selectionChanged(SelectionChangedEvent<ProfessorDTO> se) {
						if(getSelectionModel().getSelectedItems().size() == 1) {
				        	getGradeHorariaButton().enable();
				        	getDisponibilidadeButton().enable();
				        }
				    }
				});
			}
		};
		this.panel.add( this.gridPanel, bld );
	}

	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST, 450 );

		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		this.filter = new SimpleFilter();
		this.filter.setLabelWidth(205);
		this.filter.setScrollMode(Style.Scroll.AUTOY);

		this.cpfBuscaTF = new TextField< String >();
		this.cpfBuscaTF.setFieldLabel( "CPF" );
		this.nomeBuscaTF = new TextField< String >();
		this.nomeBuscaTF.setFieldLabel( "Nome" );
		this.tipoContratoBuscaCB = new TipoContratoComboBox( cenarioDTO );
		this.titulacaoBuscaCB = new TitulacaoComboBox( cenarioDTO );
		this.areaTitulacaoBuscaCB = new AreaTitulacaoComboBox();
		
		this.filter.addField( this.cpfBuscaTF );
		this.filter.addField( this.nomeBuscaTF );
		this.filter.addField( this.tipoContratoBuscaCB );
		this.filter.addField( this.titulacaoBuscaCB );
		this.filter.addField( this.areaTitulacaoBuscaCB );

		this.cargaHorariaMinOperadorCB = new OperadorComboBox();
		this.cargaHorariaMinOperadorCB.setFieldLabel(getI18nConstants().cargaHorariaMin());
		this.cargaHorariaMinOperadorCB.setWidth(100);
		
		this.cargaHorariaMinBuscaField = new NumberField();
		this.cargaHorariaMinBuscaField.setWidth( "75" );
		this.filter.addMultiField(this.cargaHorariaMinOperadorCB, this.cargaHorariaMinBuscaField);
		
		this.cargaHorariaMaxOperadorCB = new OperadorComboBox();
		this.cargaHorariaMaxOperadorCB.setFieldLabel(getI18nConstants().cargaHorariaMax());
		this.cargaHorariaMaxOperadorCB.setWidth(100);
		
		this.cargaHorariaMaxBuscaField = new NumberField();
		this.cargaHorariaMaxBuscaField.setWidth( "75" );
		this.filter.addMultiField(this.cargaHorariaMaxOperadorCB, this.cargaHorariaMaxBuscaField);
		
		this.notaDesempenhoBuscaOperadorCB = new OperadorComboBox();
		this.notaDesempenhoBuscaOperadorCB.setFieldLabel(getI18nConstants().notaDesempenho());
		this.notaDesempenhoBuscaOperadorCB.setWidth(100);
		
		this.notaDesempenhoBuscaField = new NumberField();
		this.notaDesempenhoBuscaField.setWidth( "75" );
		this.filter.addMultiField(this.notaDesempenhoBuscaOperadorCB, this.notaDesempenhoBuscaField);
		
		this.cargaHorariaAnteriorOperadorCB = new OperadorComboBox();
		this.cargaHorariaAnteriorOperadorCB.setFieldLabel(getI18nConstants().cargaHorariaAnterior());
		this.cargaHorariaAnteriorOperadorCB.setWidth(100);
		
		this.cargaHorariaAnteriorBuscaField = new NumberField();
		this.cargaHorariaAnteriorBuscaField.setWidth( "75" );
		this.filter.addMultiField(this.cargaHorariaAnteriorOperadorCB, this.cargaHorariaAnteriorBuscaField);
		
		this.custoCreditoSemanalBuscaOperadorCB = new OperadorComboBox();
		this.custoCreditoSemanalBuscaOperadorCB.setFieldLabel(getI18nConstants().valorCredito());
		this.custoCreditoSemanalBuscaOperadorCB.setWidth(100);
		
		this.custoCreditoSemanalBuscaField = new NumberField();
		this.custoCreditoSemanalBuscaField.setWidth( "75" );
		this.filter.addMultiField(this.custoCreditoSemanalBuscaOperadorCB, this.custoCreditoSemanalBuscaField);
		
		this.maxDiasSemanaOperadorCB = new OperadorComboBox();
		this.maxDiasSemanaOperadorCB.setFieldLabel(getI18nConstants().maxDiasSemana());
		this.maxDiasSemanaOperadorCB.setWidth(100);
		
		this.maxDiasSemanaBuscaField = new NumberField();
		this.maxDiasSemanaBuscaField.setWidth( "75" );
		this.filter.addMultiField(this.maxDiasSemanaOperadorCB, this.maxDiasSemanaBuscaField);
		
		this.minCreditosSemanaisOperadorCB = new OperadorComboBox();
		this.minCreditosSemanaisOperadorCB.setFieldLabel(getI18nConstants().minCreditosDia());
		this.minCreditosSemanaisOperadorCB.setWidth(100);
		
		this.minCreditosSemanaisBuscaField = new NumberField();
		this.minCreditosSemanaisBuscaField.setWidth( "75" );
		this.filter.addMultiField(this.minCreditosSemanaisOperadorCB, this.minCreditosSemanaisBuscaField);
		
		this.totalCreditosSemanaisBuscaOperadorCB = new OperadorComboBox();
		this.totalCreditosSemanaisBuscaOperadorCB.setFieldLabel(getI18nConstants().totalCreditosSemanais());
		this.totalCreditosSemanaisBuscaOperadorCB.setWidth(100);
		
		this.totalCreditosSemanaisBuscaField = new NumberField();
		this.totalCreditosSemanaisBuscaField.setWidth( "75" );
		this.filter.addMultiField(this.totalCreditosSemanaisBuscaOperadorCB, this.totalCreditosSemanaisBuscaField);

		this.cargaHorariaSemanalOperadorCB = new OperadorComboBox();
		this.cargaHorariaSemanalOperadorCB.setFieldLabel(getI18nConstants().cargaHorariaSemanal());
		this.cargaHorariaSemanalOperadorCB.setWidth(100);
		
		this.cargaHorariaSemanalBuscaField = new NumberField();
		this.cargaHorariaSemanalBuscaField.setWidth( "75" );
		this.filter.addMultiField(this.cargaHorariaSemanalOperadorCB, this.cargaHorariaSemanalBuscaField);


		this.panel.add( this.filter, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig(
			ProfessorDTO.PROPERTY_CPF, getI18nConstants().cpf(), 100 ) );

		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_NOME,
			getI18nConstants().nomeProfessor(), 115 ) );

		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_TIPO_CONTRATO_STRING,
			getI18nConstants().tipoContrato(), 100 ) );

		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_CARGA_HORARIA_MIN,
			getI18nConstants().cargaHorariaMin(), 100 ) );

		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_CARGA_HORARIA_MAX,
			getI18nConstants().cargaHorariaMax(), 170 ) );

		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_TITULACAO_STRING,
			getI18nConstants().titulacao(), 60 ) );

		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_AREA_TITULACAO_STRING,
			getI18nConstants().areaTitulacao(), 120 ) );

		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_NOTA_DESEMPENHO,
			getI18nConstants().notaDesempenho(), 100 ) );

		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_CREDITO_ANTERIOR,
			getI18nConstants().cargaHorariaAnterior(), 170 ) );

		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_VALOR_CREDITO,
			getI18nConstants().valorCredito(), 170 ) );
		
		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_MAX_DIAS_SEMANA,
				getI18nConstants().maxDiasSemana(), 140 ) );
		
		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_MIN_CREDITOS_DIA,
				getI18nConstants().minCreditosDia(), 140 ) );
		
		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_TOTAL_CRED,
				getI18nConstants().totalCreditosSemanais(), 140 ) );
		
		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_CARGA_HORARIA_SEMANAL,
				getI18nConstants().cargaHorariaSemanal(), 190 ) );

		return list;
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
	public SimpleGrid< ProfessorDTO > getGrid()
	{
		return this.gridPanel;
	}

	@Override
	public void setProxy( RpcProxy< PagingLoadResult< ProfessorDTO > > proxy )
	{
		this.gridPanel.setProxy( proxy );
	}

	@Override
	public TextField< String > getCpfBuscaTextField()
	{
		return this.cpfBuscaTF;
	}

	@Override
	public TipoContratoComboBox getTipoContratoBuscaComboBox()
	{
		return this.tipoContratoBuscaCB;
	}

	@Override
	public TitulacaoComboBox getTitulacaoBuscaComboBox()
	{
		return this.titulacaoBuscaCB;
	}

	@Override
	public AreaTitulacaoComboBox getAreaTitulacaoBuscaComboBox()
	{
		return this.areaTitulacaoBuscaCB;
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
	public Button getDisponibilidadeButton()
	{
		return this.disponibilidadeBT;
	}
	
	@Override
	public Button getGradeHorariaButton()
	{
		return this.gradeHorariaBT;
	}

	@Override
	public NumberField getCargaHorariaMinBuscaField() {
		return cargaHorariaMinBuscaField;
	}

	@Override
	public OperadorComboBox getCargaHorariaMinOperadorCB() {
		return cargaHorariaMinOperadorCB;
	}

	@Override
	public NumberField getCargaHorariaMaxBuscaField() {
		return cargaHorariaMaxBuscaField;
	}

	@Override
	public OperadorComboBox getCargaHorariaMaxOperadorCB() {
		return cargaHorariaMaxOperadorCB;
	}

	@Override
	public NumberField getNotaDesempenhoBuscaField() {
		return notaDesempenhoBuscaField;
	}

	@Override
	public OperadorComboBox getNotaDesempenhoBuscaOperadorCB() {
		return notaDesempenhoBuscaOperadorCB;
	}

	@Override
	public NumberField getCargaHorariaAnteriorBuscaField() {
		return cargaHorariaAnteriorBuscaField;
	}

	@Override
	public OperadorComboBox getCargaHorariaAnteriorOperadorCB() {
		return cargaHorariaAnteriorOperadorCB;
	}

	@Override
	public NumberField getCustoCreditoSemanalBuscaField() {
		return custoCreditoSemanalBuscaField;
	}

	@Override
	public OperadorComboBox getCustoCreditoSemanalBuscaOperadorCB() {
		return custoCreditoSemanalBuscaOperadorCB;
	}

	@Override
	public NumberField getMaxDiasSemanaBuscaField() {
		return maxDiasSemanaBuscaField;
	}

	@Override
	public OperadorComboBox getMaxDiasSemanaOperadorCB() {
		return maxDiasSemanaOperadorCB;
	}

	@Override
	public NumberField getMinCreditosSemanaisBuscaField() {
		return minCreditosSemanaisBuscaField;
	}

	@Override
	public OperadorComboBox getMinCreditosSemanaisOperadorCB() {
		return minCreditosSemanaisOperadorCB;
	}

	@Override
	public NumberField getTotalCreditosSemanaisBuscaField() {
		return totalCreditosSemanaisBuscaField;
	}

	@Override
	public OperadorComboBox getTotalCreditosSemanaisBuscaOperadorCB() {
		return totalCreditosSemanaisBuscaOperadorCB;
	}

	@Override
	public NumberField getCargaHorariaSemanalBuscaField() {
		return cargaHorariaSemanalBuscaField;
	}

	@Override
	public OperadorComboBox getCargaHorariaSemanalOperadorCB() {
		return cargaHorariaSemanalOperadorCB;
	}

	@Override
	public TextField<String> getNomeBuscaTextField() {
		return nomeBuscaTF;
	}
}
