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
import com.gapso.web.trieda.main.client.mvp.presenter.DisciplinasPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.ComboBoxBoolean;
import com.gapso.web.trieda.shared.util.view.DificuldadeComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.OperadorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.TipoDisciplinaComboBox;

public class DisciplinasView
	extends MyComposite
	implements DisciplinasPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< DisciplinaDTO > gridPanel;
	private SimpleFilter filter;
	private TextField< String > nomeBuscaTextField;
	private TextField< String > codigoBuscaTextField;
	private TipoDisciplinaComboBox tipoDisciplinaBuscaComboBox;
	private NumberField creditosTeoricosField;
	private OperadorComboBox operadorCreditosTeoricos;
	private NumberField creditosPraticosField;
	private OperadorComboBox operadorCreditosPraticos;
	private ComboBoxBoolean exigeLaboratorio;
	private DificuldadeComboBox nivelDificuldade;
	private NumberField maxAlunosTeoricosField;
	private OperadorComboBox operadorMaxAlunosTeoricos;
	private NumberField maxAlunosPraticosField;
	private OperadorComboBox operadorMaxAlunosPraticos;
	private ComboBoxBoolean aulasContinuas;
	private ComboBoxBoolean professorUnico;
	private ComboBoxBoolean usaSabado;
	private ComboBoxBoolean usaDomingo;
	private Button disponibilidadeBT;
	private Button divisaoCreditoBT;
	private Button associarSalasBT;
	private Button associarGruposSalasBT;
	private ContentPanel panel;
	private GTabItem tabItem;
	private CenarioDTO cenarioDTO;

	public DisciplinasView( CenarioDTO cenarioDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeadingHtml( cenarioDTO.getNome() + " » Disciplinas" );

		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( this.tabItem );
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			"Disciplinas", Resources.DEFAULTS.disciplina16() );

		this.tabItem.setContent( this.panel );
	}

	private void createToolBar()
	{
		this.toolBar = new SimpleToolBar( this );
		this.toolBar.add( new SeparatorToolItem() );

		this.divisaoCreditoBT = this.toolBar.createButton(
			"Divisão de Créditos da disciplina",
			Resources.DEFAULTS.divisaoDeCreditos16() );

		this.toolBar.add( this.divisaoCreditoBT );

		this.disponibilidadeBT = this.toolBar.createButton(
			getI18nConstants().disponibilidadesSemanaLetiva(),
			Resources.DEFAULTS.disponibilidade16() );

		this.toolBar.add( this.disponibilidadeBT );
		
		this.associarSalasBT = this.toolBar.createButton(
				getI18nConstants().disciplinasSalas(),
				Resources.DEFAULTS.associacaoDisciplinaSala16() );

		this.toolBar.add( this.associarSalasBT );
		
		this.associarGruposSalasBT = this.toolBar.createButton(
				getI18nConstants().disciplinasSalas(),
				Resources.DEFAULTS.grupoSala16() );
		
		this.toolBar.add( this.associarGruposSalasBT );
		
		this.panel.setTopComponent( this.toolBar );
		
		this.toolBar.activateEmptyState();
	}

	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );

	    this.gridPanel = new SimpleGrid< DisciplinaDTO >( getColumnList(), this, this.toolBar );
	    this.panel.add( this.gridPanel, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( DisciplinaDTO.PROPERTY_NOME, getI18nConstants().nomeDisciplina(), 200 ) );
		list.add( new ColumnConfig( DisciplinaDTO.PROPERTY_CODIGO, getI18nConstants().codigoDisciplina(), 100 ) );
		list.add( new ColumnConfig( DisciplinaDTO.PROPERTY_CREDITOS_TEORICO, getI18nConstants().creditosTeoricos(), 100 ) );
		list.add( new ColumnConfig( DisciplinaDTO.PROPERTY_CREDITOS_PRATICO, getI18nConstants().creditosPraticos(), 100 ) );
		CheckColumnConfig column = new CheckColumnConfig( DisciplinaDTO.PROPERTY_LABORATORIO, getI18nConstants().exigeLaboratorio(), 100 );
		column.setToolTip("Se estiver marcada, o Trieda buscará associações desta disciplina à laboratórios para realizar a alocação. Caso não haja nenhum laboratório associado a esta disciplina o Trieda acusa um erro de cadastro e não permitirá que a otimização ocorra até que o erro seja resolvido.");
		list.add( column );
		ColumnConfig column2 = new ColumnConfig( DisciplinaDTO.PROPERTY_TIPO_STRING, getI18nConstants().tipoDisciplina(), 100 );
		column2.setToolTip("As disciplinas Presenciais serão alocadas pelo Trieda em salas e horários. As disciplinas Online não serão alocadas em salas nem horários pelo Trieda.");
		list.add( column2 );
		list.add( new ColumnConfig( DisciplinaDTO.PROPERTY_DIFICULDADE, getI18nConstants().nivelDificuldade(), 120 ) );
		ColumnConfig column3 = new ColumnConfig( DisciplinaDTO.PROPERTY_MAX_ALUNOS_TEORICO, getI18nConstants().maxAlunosTeorico(), 120 );
		column3.setToolTip("(OPCIONAL) Restrição pedagógica do número máximo de alunos em sala de aula para uma disciplina específica. Quanto omitido, o TRIEDA assume a capacidade da sala alocada.");
		list.add( column3 );
		ColumnConfig column4 = new ColumnConfig( DisciplinaDTO.PROPERTY_MAX_ALUNOS_PRATICO, getI18nConstants().maxAlunosPratico(), 120 );
		column4.setToolTip("(OPCIONAL) Restrição pedagógica do número máximo de alunos em laboratório para uma disciplina específica. Quanto omitido, o TRIEDA assume a capacidade do laboratório alocado.");
		list.add( column4 );
		CheckColumnConfig column5 = new CheckColumnConfig( DisciplinaDTO.PROPERTY_AULAS_CONTINUAS, getI18nConstants().aulasContinuas(), 100 );
		column5.setToolTip("(Apenas quando a disciplina tem créditos teóricos e práticos.) Determina se as aulas teóricas e práticas devem ser lecionadas em sequência.");
		list.add( column5 );
		CheckColumnConfig column6 = new CheckColumnConfig( DisciplinaDTO.PROPERTY_PROFESSOR_UNICO, getI18nConstants().professorUnico(), 100 );
		column6.setToolTip("(Apenas quando a disciplina tem créditos teóricos e práticos.) Determina se o professor deve ser o mesmo para as aulas teóricas e práticas.");
		list.add( column6 );
		CheckColumnConfig column7 = new CheckColumnConfig( DisciplinaDTO.PROPERTY_USA_SABADO, getI18nConstants().usaSabado(), 80 );
		column7.setToolTip("Determina se a disciplina deverá herdar os horários das semanas letivas também no sábado.");
		list.add( column7 );
		CheckColumnConfig column8 = new CheckColumnConfig( DisciplinaDTO.PROPERTY_USA_DOMINGO, getI18nConstants().usaDomingo(), 80 );
		column8.setToolTip("Determina se a disciplina deverá herdar os horários das semanas letivas também no domingo.");
		list.add( column8 );
		
		return list;
	}

	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST, 350 );
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		this.filter = new SimpleFilter();
		this.filter.setLabelWidth(150);
		this.nomeBuscaTextField = new TextField<String>();
		this.nomeBuscaTextField.setFieldLabel("Nome");
		this.codigoBuscaTextField = new TextField<String>();
		this.codigoBuscaTextField.setFieldLabel("Código");
		this.tipoDisciplinaBuscaComboBox = new TipoDisciplinaComboBox(cenarioDTO);
		this.tipoDisciplinaBuscaComboBox.setFieldLabel("Tipo de disciplina");
		
		this.operadorCreditosTeoricos = new OperadorComboBox();
		this.operadorCreditosTeoricos.setWidth(100);
		this.operadorCreditosTeoricos.setFieldLabel(getI18nConstants().creditosTeoricos());
		this.creditosTeoricosField = new NumberField();
		this.creditosTeoricosField.setWidth(75);
		
		this.operadorCreditosPraticos = new OperadorComboBox();
		this.operadorCreditosPraticos.setWidth(100);
		this.operadorCreditosPraticos.setFieldLabel(getI18nConstants().creditosPraticos());
		this.creditosPraticosField = new NumberField();
		this.creditosPraticosField.setWidth(75);
		
		this.exigeLaboratorio = new ComboBoxBoolean();
		this.exigeLaboratorio.setFieldLabel(getI18nConstants().exigeLaboratorio());
		
		this.nivelDificuldade = new DificuldadeComboBox(true);
		this.nivelDificuldade.setFieldLabel(getI18nConstants().nivelDificuldade());
		
		this.operadorMaxAlunosTeoricos = new OperadorComboBox();
		this.operadorMaxAlunosTeoricos.setWidth(100);
		this.operadorMaxAlunosTeoricos.setFieldLabel(getI18nConstants().maxAlunosTeorico());
		this.maxAlunosTeoricosField = new NumberField();
		this.maxAlunosTeoricosField.setWidth(75);
		
		this.operadorMaxAlunosPraticos = new OperadorComboBox();
		this.operadorMaxAlunosPraticos.setWidth(100);
		this.operadorMaxAlunosPraticos.setFieldLabel(getI18nConstants().maxAlunosPratico());
		this.maxAlunosPraticosField = new NumberField();
		this.maxAlunosPraticosField.setWidth(75);
		
		this.aulasContinuas = new ComboBoxBoolean();
		this.aulasContinuas.setFieldLabel(getI18nConstants().aulasContinuas());
		
		this.professorUnico = new ComboBoxBoolean();
		this.professorUnico.setFieldLabel(getI18nConstants().professorUnico());
		
		this.usaSabado = new ComboBoxBoolean();
		this.usaSabado.setFieldLabel(getI18nConstants().usaSabado());
		
		this.usaDomingo = new ComboBoxBoolean();
		this.usaDomingo.setFieldLabel(getI18nConstants().usaDomingo());
		
		this.filter.addField( this.nomeBuscaTextField );
		this.filter.addField( this.codigoBuscaTextField );
		this.filter.addField( this.tipoDisciplinaBuscaComboBox );
		this.filter.addMultiField(this.operadorCreditosTeoricos, this.creditosTeoricosField);
		this.filter.addMultiField(this.operadorCreditosPraticos, this.creditosPraticosField);
		this.filter.addField( this.exigeLaboratorio );
		this.filter.addField( this.nivelDificuldade );
		this.filter.addMultiField(this.operadorMaxAlunosTeoricos, this.maxAlunosTeoricosField);
		this.filter.addMultiField(this.operadorMaxAlunosPraticos, this.maxAlunosPraticosField);
		this.filter.addField( this.aulasContinuas );
		this.filter.addField( this.professorUnico );
		this.filter.addField( this.usaSabado );
		this.filter.addField( this.usaDomingo );

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
	public SimpleGrid< DisciplinaDTO > getGrid()
	{
		return this.gridPanel;
	}

	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< DisciplinaDTO > > proxy )
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
	public TipoDisciplinaComboBox getTipoDisciplinaBuscaComboBox()
	{
		return this.tipoDisciplinaBuscaComboBox;
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
	public Button getDivisaoCreditoButton()
	{
		return this.divisaoCreditoBT;
	}
	
	@Override
	public Button getAssociarSalasButton()
	{
		return this.associarSalasBT;
	}
	
	@Override
	public Button getAssociarGruposSalasButton()
	{
		return this.associarGruposSalasBT;
	}

	@Override
	public NumberField getCreditosTeoricosField() {
		return creditosTeoricosField;
	}

	@Override
	public OperadorComboBox getOperadorCreditosTeoricos() {
		return operadorCreditosTeoricos;
	}

	@Override
	public NumberField getCreditosPraticosField() {
		return creditosPraticosField;
	}

	@Override
	public OperadorComboBox getOperadorCreditosPraticos() {
		return operadorCreditosPraticos;
	}

	@Override
	public ComboBoxBoolean getExigeLaboratorio() {
		return exigeLaboratorio;
	}

	@Override
	public DificuldadeComboBox getNivelDificuldade() {
		return nivelDificuldade;
	}

	@Override
	public NumberField getMaxAlunosTeoricosField() {
		return maxAlunosTeoricosField;
	}

	@Override
	public OperadorComboBox getOperadorMaxAlunosTeoricos() {
		return operadorMaxAlunosTeoricos;
	}

	@Override
	public NumberField getMaxAlunosPraticosField() {
		return maxAlunosPraticosField;
	}

	@Override
	public OperadorComboBox getOperadorMaxAlunosPraticos() {
		return operadorMaxAlunosPraticos;
	}

	@Override
	public ComboBoxBoolean getUsaSabado() {
		return usaSabado;
	}

	@Override
	public ComboBoxBoolean getUsaDomingo() {
		return usaDomingo;
	}

	@Override
	public ComboBoxBoolean getAulasContinuas() {
		return aulasContinuas;
	}

	@Override
	public ComboBoxBoolean getProfessorUnico() {
		return professorUnico;
	}
}
