package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessoresListPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.AreaTitulacaoComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.RelatorioProfessorFiltro;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.TipoContratoComboBox;
import com.gapso.web.trieda.shared.util.view.TitulacaoComboBox;

public abstract class ProfessoresListView
	extends MyComposite
	implements ProfessoresListPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< ProfessorDTO > gridPanel;
	private SimpleFilter filter;
	private TextField< String > cpfBuscaTF;
	private TipoContratoComboBox tipoContratoBuscaCB;
	private TitulacaoComboBox titulacaoBuscaCB;
	private AreaTitulacaoComboBox areaTitulacaoBuscaCB;
	private Button gradeHorariaBT;
	protected ContentPanel panel;
	private GTabItem tabItem;
	private RelatorioProfessorFiltro professorFiltro;
	protected CenarioDTO cenarioDTO;
	
	public ProfessoresListView( CenarioDTO cenarioDTO, RelatorioProfessorFiltro professorFiltro)
	{
		this.professorFiltro = professorFiltro;
		this.cenarioDTO = cenarioDTO;
		this.initUI();
	}

	private void initUI()
	{

		createPanel();
		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( this.tabItem );
	}
	
	protected abstract void createPanel();

	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			"Professores", Resources.DEFAULTS.professor16() );

		this.tabItem.setContent( this.panel );
	}

	private void createToolBar()
	{
		toolBar = new SimpleToolBar( false, false, false, false, true, this );
		toolBar.add( new SeparatorToolItem() );
		gradeHorariaBT = toolBar.createButton(
				"Grade Horária do Professor",
				Resources.DEFAULTS.saidaProfessor16() );

		toolBar.add( gradeHorariaBT );
		panel.setTopComponent( toolBar );
	}

	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
		bld.setMargins( new Margins( 5, 5, 5, 5 ) );

		this.gridPanel = new SimpleGrid< ProfessorDTO >( getColumnList(), this, this.toolBar );
		this.panel.add( this.gridPanel, bld );
	}

	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST );

		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		this.filter = new SimpleFilter();

		this.cpfBuscaTF = new TextField< String >();
		this.cpfBuscaTF.setFieldLabel( "CPF" );
		this.tipoContratoBuscaCB = new TipoContratoComboBox( cenarioDTO );
		this.tipoContratoBuscaCB.setValue(professorFiltro.getTipoContrato());
		this.titulacaoBuscaCB = new TitulacaoComboBox( cenarioDTO );
		this.titulacaoBuscaCB.setValue(professorFiltro.getTitulacao());
		this.areaTitulacaoBuscaCB = new AreaTitulacaoComboBox();
		this.areaTitulacaoBuscaCB.setValue(professorFiltro.getAreaTitulacao());

		this.filter.addField( this.cpfBuscaTF );
		this.filter.addField( this.tipoContratoBuscaCB );
		this.filter.addField( this.titulacaoBuscaCB );
		this.filter.addField( this.areaTitulacaoBuscaCB );

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
			getI18nConstants().areaTitulacao(), 100 ) );

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
	public Button getGradeHorariaButton()
	{
		return this.gradeHorariaBT;
	}
	
	@Override
	public RelatorioProfessorFiltro getProfessorFiltro()
	{
		return this.professorFiltro;
	}
}
