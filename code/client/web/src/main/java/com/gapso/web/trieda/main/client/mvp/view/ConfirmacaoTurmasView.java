package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.LabelField;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.grid.CellEditor;
import com.extjs.gxt.ui.client.widget.grid.CheckColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.main.client.mvp.presenter.ConfirmacaoTurmasPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ConfirmacaoTurmaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.PeriodoComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class ConfirmacaoTurmasView extends MyComposite
	implements ConfirmacaoTurmasPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< ConfirmacaoTurmaDTO > gridPanel;
	private SimpleFilter filter;
	private CursoComboBox cursoCB;
	private PeriodoComboBox periodoCB;
	private CheckColumnConfig checkColumn;
	private ContentPanel panel;
	private GTabItem tabItem;
	private CenarioDTO cenarioDTO;
	private Button saveBT;
	private Button cancelBT;
	private Button marcarTudoBt;
	private Button marcarMaiorXBt;
	private Button desmarcarTudoBt;
	private Button desmarcarMenorXBt;
	private Button marcarFormandosBt;
	private FormPanel painelIndicadores;
	private LabelField leftLabel;
	private LabelField centerLabel;
	private LabelField rightLabel;
	
	public ConfirmacaoTurmasView( CenarioDTO cenarioDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.initUI();
	}
	
	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeadingHtml( cenarioDTO.getNome() + " » Confirmação de Turmas" );
	
		createToolBar();
		createGrid();
		createFilter();
		createPainelIndicadores();
		createTabItem();
		initComponent( this.tabItem );
	}
	
	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			"Confirmação de Turmas", Resources.DEFAULTS.confirmacao16() );
	
		this.tabItem.setContent( this.panel );
	}
	
	private void createToolBar()
	{
		toolBar = new SimpleToolBar( false, false, false, false, false, this );
		saveBT = createButton("Salvar", Resources.DEFAULTS.save16());
		cancelBT = createButton("Cancelar", Resources.DEFAULTS.cancel16());
		marcarTudoBt = createButton("Marcar todas", Resources.DEFAULTS.confirmarTodos16());
		marcarMaiorXBt = createButton("Marcar turmas com X ou mais alunos", Resources.DEFAULTS.confirmarQtdeAlunos16());
		desmarcarTudoBt = createButton("Desmarcar todas", Resources.DEFAULTS.desconfirmarTodos16());
		desmarcarMenorXBt = createButton("Desmarcar turmas com X ou menos alunos", Resources.DEFAULTS.desconfirmarQtdeAlunos16());
		marcarFormandosBt = createButton("Marcar turmas com alunos formandos", Resources.DEFAULTS.marcarFormandos16());
	
		toolBar.add( saveBT );
		toolBar.add( cancelBT );
		toolBar.add( new SeparatorToolItem() );
		toolBar.add( marcarTudoBt );
		toolBar.add( marcarMaiorXBt );
		toolBar.add( desmarcarTudoBt );
		toolBar.add( desmarcarMenorXBt );
		toolBar.add( marcarFormandosBt );
		panel.setTopComponent( toolBar );
	}
	
	private void createGrid()
	{	
		this.gridPanel = new SimpleGrid< ConfirmacaoTurmaDTO >( getColumnList(), this, this.toolBar );
		gridPanel.addPlugin(checkColumn);
		
	    ContentPanel contentPanel = new ContentPanel( new FitLayout() );
	    contentPanel.setHeaderVisible( false );
	    contentPanel.add( gridPanel );

	    BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5 ) );
	    panel.add( contentPanel, bld );
	}
	
	private void createPainelIndicadores()
	{
		painelIndicadores = new FormPanel();
		painelIndicadores.setHeadingHtml( "Painel de Indicadores Gerais" );
		
		final LayoutContainer main = new LayoutContainer(new ColumnLayout())
		{
			protected void afterRender()
			{
				
			};
		};
		LayoutContainer left = new LayoutContainer(new FormLayout(LabelAlign.LEFT));
		//left.setStyleAttribute("margin-right", "20px");
		LayoutContainer right = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));
		LayoutContainer center = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));
		//center.setStyleAttribute("margin-right", "20px");
		
		main.add(left, new ColumnData(0.23));
		main.add(center, new ColumnData(0.25));
		main.add(right, new ColumnData(0.23));
		main.addStyleName("painelIndicadores");
		
		leftLabel = new LabelField();
		left.add(leftLabel);
		
		centerLabel = new LabelField();
		center.add(centerLabel);
		
		rightLabel = new LabelField();
		right.add(rightLabel);
		
	    BorderLayoutData bld = new BorderLayoutData( LayoutRegion.SOUTH );
	    bld.setMargins( new Margins( 5 ) );
	    bld.setSize(120);
	    bld.setCollapsible(true);
	    
	    painelIndicadores.add(main, new FormData("100%"));
	    panel.add( painelIndicadores, bld );
	}
	
	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST );
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		this.filter = new SimpleFilter();
		this.cursoCB = new CursoComboBox( cenarioDTO );
		this.cursoCB.disable();
		this.periodoCB = new PeriodoComboBox( cenarioDTO, this);
		this.periodoCB.disable();
		this.filter.getSubmitButton().disable();
		this.filter.getResetButton().disable();

		this.filter.addField( this.cursoCB );
		this.filter.addField( this.periodoCB );

		this.panel.add( this.filter, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();
	
		checkColumn = new CheckColumnConfig(
				ConfirmacaoTurmaDTO.PROPERTY_CONFIRMADA, getI18nConstants().confirmacao(), 70 );
		CellEditor checkBoxEditor = new CellEditor(new CheckBox());        
		checkColumn.setEditor(checkBoxEditor);
		
		list.add( checkColumn );
		
		list.add( new ColumnConfig(
				ConfirmacaoTurmaDTO.PROPERTY_DISCIPLINA_CODIGO, getI18nConstants().codigoDisciplina(), 100 ) );
	
		list.add( new ColumnConfig( ConfirmacaoTurmaDTO.PROPERTY_DISCIPLINA_NOME,
			getI18nConstants().nomeDisciplina(), 115 ) );
	
		list.add( new ColumnConfig( ConfirmacaoTurmaDTO.PROPERTY_CREDITOS_TEORICO,
			getI18nConstants().creditosTeoricos(), 100 ) );
	
		list.add( new ColumnConfig( ConfirmacaoTurmaDTO.PROPERTY_CREDITOS_PRATICO,
			getI18nConstants().creditosPraticos(), 100 ) );
	
		list.add( new ColumnConfig( ConfirmacaoTurmaDTO.PROPERTY_TURMA,
			getI18nConstants().turma(), 60 ) );
	
		list.add( new ColumnConfig( ConfirmacaoTurmaDTO.PROPERTY_QUANTIDADE_ALUNOS,
			getI18nConstants().quantidadeAlunos(), 100 ) );
	
		list.add( new ColumnConfig( ConfirmacaoTurmaDTO.PROPERTY_SALA_STRING,
			getI18nConstants().sala(), 100 ) );
	
		list.add( new ColumnConfig( ConfirmacaoTurmaDTO.PROPERTY_PROFESSOR_STRING,
			getI18nConstants().professor(), 150 ) );
	
		list.add( new ColumnConfig( ConfirmacaoTurmaDTO.PROPERTY_DIAS_HORARIOS,
			getI18nConstants().diasHorariosAula(),240 ) );
	
		return list;
	}
	
	public Button createButton(String toolTip, ImageResource img) {
		Button bt = new Button();
		bt.setIcon(AbstractImagePrototype.create(img));
		bt.setToolTip(toolTip);
		return bt;
	}
	
	public Button getSaveButton() {
		return saveBT;
	}

	public Button getCancelButton() {
		return cancelBT;
	}
	
	public Button getMarcarTudoButton() {
		return marcarTudoBt;
	}
	
	public Button getMarcarMaiorXButton() {
		return marcarMaiorXBt;
	}
	
	public Button getDesmarcarTudoButton() {
		return desmarcarTudoBt;
	}
	
	public Button getDesmarcarMenorXButton() {
		return desmarcarMenorXBt;
	}
	
	public Button getMarcarFormandosButton() {
		return marcarFormandosBt;
	}
	
	public FormPanel getPainelIndicadores() {
		return painelIndicadores;
	}
	
	public LabelField getLeftLabelButton() {
		return leftLabel;
	}
	
	public LabelField getRightLabelButton() {
		return rightLabel;
	}
	
	public LabelField getCenterLabelButton() {
		return centerLabel;
	}
	
	@Override
	public SimpleGrid< ConfirmacaoTurmaDTO > getGrid()
	{
		return this.gridPanel;
	}
	
	@Override
	public void setProxy( RpcProxy< PagingLoadResult< ConfirmacaoTurmaDTO > > proxy )
	{
		this.gridPanel.setProxy( proxy );
	}
	
	@Override
	public CursoComboBox getCursoComboBox()
	{
		return this.cursoCB;
	}
	
	@Override
	public PeriodoComboBox getPeriodoComboBox()
	{
		return this.periodoCB;
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
}
