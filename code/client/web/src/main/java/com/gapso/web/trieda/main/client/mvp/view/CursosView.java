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
import com.gapso.web.trieda.main.client.mvp.presenter.CursosPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.ComboBoxBoolean;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.OperadorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.TipoCursoComboBox;

public class CursosView
	extends MyComposite
	implements CursosPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< CursoDTO > gridPanel;
	private SimpleFilter filter;
	private TextField< String > nomeBuscaTextField;
	private TextField< String > codigoBuscaTextField;
	private TipoCursoComboBox tipoCursoBuscaComboBox;
	private OperadorComboBox operadorMinPercentualDoutor;
	private OperadorComboBox operadorMinPercentualMestre;
	private OperadorComboBox operadorMinTempoIntegralParcial;
	private OperadorComboBox operadorMinTempoIntegral;
	private OperadorComboBox operadorMaxDisciplinasProfessor;
	private NumberField minPercentualDoutor;
	private NumberField minPercentualMestre;
	private NumberField minTempoIntegralParcial;
	private NumberField minTempoIntegral;
	private NumberField maxDisciplinasProfessor;
	private ComboBoxBoolean maisDeUmaDisciplinaProfessor;
	private Button curriculosBT;
	private ContentPanel panel;
	private GTabItem tabItem;
	private CenarioDTO cenarioDTO;

	public CursosView( CenarioDTO cenarioDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeadingHtml( cenarioDTO.getNome() + " » Cursos" );

		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( this.tabItem );
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			"Cursos", Resources.DEFAULTS.curso16() );

		this.tabItem.setContent( this.panel );
	}
	
	private void createToolBar()
	{
		this.toolBar = new SimpleToolBar( this );
		this.toolBar.add( new SeparatorToolItem() );

		this.curriculosBT = toolBar.createButton(
			"Matrizes Curriculares do Curso Selecionado",
			Resources.DEFAULTS.matrizCurricular16() );

		this.toolBar.add( this.curriculosBT );
		this.panel.setTopComponent( this.toolBar );
	}

	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );

	    this.gridPanel = new SimpleGrid< CursoDTO >( getColumnList(), this, this.toolBar );
	    this.panel.add( this.gridPanel, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( CursoDTO.PROPERTY_NOME, getI18nConstants().nomeCurso(), 100 ) );
		list.add( new ColumnConfig( CursoDTO.PROPERTY_CODIGO, getI18nConstants().codigoCurso(), 100 ) );
		list.add( new ColumnConfig( CursoDTO.PROPERTY_TIPO_STRING, getI18nConstants().tipoCurso(), 100 ) );
		list.add( new ColumnConfig( CursoDTO.PROPERTY_NUM_MIN_DOUTORES, getI18nConstants().minPercentualDoutor(), 100 ) );
		list.add( new ColumnConfig( CursoDTO.PROPERTY_NUM_MIN_MESTRES, getI18nConstants().minPercentualMestre(), 100 ) );
		list.add( new ColumnConfig( CursoDTO.PROPERTY_MIN_TEMPO_INTEGRAL_PARCIAL, getI18nConstants().minTempoIntegralParcial(), 100 ) );
		list.add( new ColumnConfig( CursoDTO.PROPERTY_MIN_TEMPO_INTEGRAL, getI18nConstants().minTempoIntegral(), 100 ) );
		ColumnConfig column = new ColumnConfig( CursoDTO.PROPERTY_MAX_DISCIPLINAS_PELO_PROFESSOR, getI18nConstants().maxDisciplinasProfessor(), 110 );
		column.setToolTip("Determina o número máximo de disciplinas que um professor pode ministrar para um mesmo curso, a cada semestre. Caso não queria determinar um número máximo, usar 99.");
		list.add( column );
		list.add( new CheckColumnConfig( CursoDTO.PROPERTY_ADM_MAIS_DE_UMA_DISCIPLINA, getI18nConstants().maisDeUmaDisciplinaProfessor(), 200 ) );

		return list;
	}

	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST, 350 );
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		this.filter = new SimpleFilter();
		this.filter.setLabelWidth(150);
		this.tipoCursoBuscaComboBox = new TipoCursoComboBox( cenarioDTO );
		this.tipoCursoBuscaComboBox.setFieldLabel( "Tipo" );
		this.nomeBuscaTextField = new TextField< String >();
		this.nomeBuscaTextField.setFieldLabel( "Nome" );
		this.codigoBuscaTextField = new TextField< String >();
		this.codigoBuscaTextField.setFieldLabel( "Código" );
		
		this.operadorMinPercentualDoutor = new OperadorComboBox();
		this.operadorMinPercentualDoutor.setWidth(100);
		this.operadorMinPercentualDoutor.setFieldLabel(getI18nConstants().minPercentualDoutor());
		this.minPercentualDoutor = new NumberField();
		this.minPercentualDoutor.setWidth(75);
		
		this.operadorMinPercentualMestre = new OperadorComboBox();
		this.operadorMinPercentualMestre.setWidth(100);
		this.operadorMinPercentualMestre.setFieldLabel(getI18nConstants().minPercentualMestre());
		this.minPercentualMestre = new NumberField();
		this.minPercentualMestre.setWidth(75);
		
		this.operadorMinTempoIntegralParcial = new OperadorComboBox();
		this.operadorMinTempoIntegralParcial.setWidth(100);
		this.operadorMinTempoIntegralParcial.setFieldLabel(getI18nConstants().minTempoIntegralParcial());
		this.minTempoIntegralParcial = new NumberField();
		this.minTempoIntegralParcial.setWidth(75);
		
		this.operadorMinTempoIntegral= new OperadorComboBox();
		this.operadorMinTempoIntegral.setWidth(100);
		this.operadorMinTempoIntegral.setFieldLabel(getI18nConstants().minTempoIntegral());
		this.minTempoIntegral = new NumberField();
		this.minTempoIntegral.setWidth(75);
		
		this.operadorMaxDisciplinasProfessor= new OperadorComboBox();
		this.operadorMaxDisciplinasProfessor.setWidth(100);
		this.operadorMaxDisciplinasProfessor.setFieldLabel(getI18nConstants().maxDisciplinasProfessor());
		this.maxDisciplinasProfessor = new NumberField();
		this.maxDisciplinasProfessor.setWidth(75);
		
		this.maisDeUmaDisciplinaProfessor = new ComboBoxBoolean();
		this.maisDeUmaDisciplinaProfessor.setFieldLabel(getI18nConstants().maisDeUmaDisciplinaProfessor());
		
		this.filter.addField( this.tipoCursoBuscaComboBox );
		this.filter.addField( this.nomeBuscaTextField );
		this.filter.addField( this.codigoBuscaTextField );
		this.filter.addMultiField( this.operadorMinPercentualDoutor, this.minPercentualDoutor );
		this.filter.addMultiField( this.operadorMinPercentualMestre, this.minPercentualMestre );
		this.filter.addMultiField( this.operadorMinTempoIntegralParcial, this.minTempoIntegralParcial);
		this.filter.addMultiField( this.operadorMinTempoIntegral, this.minTempoIntegral);
		this.filter.addMultiField( this.operadorMaxDisciplinasProfessor, this.maxDisciplinasProfessor );
		this.filter.addField( this.maisDeUmaDisciplinaProfessor );

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
	public SimpleGrid< CursoDTO > getGrid()
	{
		return this.gridPanel;
	}
	
	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< CursoDTO > > proxy )
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
	public TipoCursoComboBox getTipoCursoBuscaComboBox()
	{
		return this.tipoCursoBuscaComboBox;
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
	public Button getCurriculosButton()
	{
		return this.curriculosBT;
	}

	@Override
	public OperadorComboBox getOperadorMinPercentualDoutor() {
		return operadorMinPercentualDoutor;
	}

	@Override
	public OperadorComboBox getOperadorMinPercentualMestre() {
		return operadorMinPercentualMestre;
	}

	@Override
	public OperadorComboBox getOperadorMinTempoIntegralParcial() {
		return operadorMinTempoIntegralParcial;
	}

	@Override
	public OperadorComboBox getOperadorMinTempoIntegral() {
		return operadorMinTempoIntegral;
	}

	@Override
	public OperadorComboBox getOperadorMaxDisciplinasProfessor() {
		return operadorMaxDisciplinasProfessor;
	}

	@Override
	public NumberField getMinPercentualDoutor() {
		return minPercentualDoutor;
	}

	@Override
	public NumberField getMinPercentualMestre() {
		return minPercentualMestre;
	}

	@Override
	public NumberField getMinTempoIntegralParcial() {
		return minTempoIntegralParcial;
	}

	@Override
	public NumberField getMinTempoIntegral() {
		return minTempoIntegral;
	}

	@Override
	public NumberField getMaxDisciplinasProfessor() {
		return maxDisciplinasProfessor;
	}

	@Override
	public ComboBoxBoolean getMaisDeUmaDisciplinaProfessor() {
		return maisDeUmaDisciplinaProfessor;
	}
}
