package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.LoadEvent;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.LoadListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.store.StoreEvent;
import com.extjs.gxt.ui.client.store.StoreListener;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.TabItem;
import com.extjs.gxt.ui.client.widget.TabPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.LabelField;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.CellEditor;
import com.extjs.gxt.ui.client.widget.grid.CheckColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.extjs.gxt.ui.client.widget.grid.HeaderGroupConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.CenterLayout;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.main.client.mvp.presenter.AlocacaoManualPresenter;
import com.gapso.web.trieda.shared.dtos.AlunoStatusDTO;
import com.gapso.web.trieda.shared.dtos.AulaDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorStatusDTO;
import com.gapso.web.trieda.shared.dtos.QuintetoDTO;
import com.gapso.web.trieda.shared.dtos.ResumoMatriculaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaStatusDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaSalaGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoSalaFiltro;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SalaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.SimpleUnpagedGrid;
import com.google.gwt.i18n.client.NumberFormat;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;
import com.google.gwt.user.client.ui.Widget;

public class AlocacaoManualView
	extends MyComposite
	implements AlocacaoManualPresenter.Display
{
	private ContentPanel panel;
	private GTabItem tabItem;
	private CenarioDTO cenarioDTO;
	private List<DemandaDTO> demandasDTO;
	private DisciplinaDTO disciplinaDTO;
	private CampusDTO campusDTO;
	private ResumoMatriculaDTO resumoMatriculaDTO;
	private Integer atendimentoNaoPlanejado;
	private Integer atendimentoPlanejado;
	private SimpleUnpagedGrid<TurmaStatusDTO> turmasGrid;
	private SimpleUnpagedGrid<AlunoStatusDTO> alunosGrid;
	private SimpleUnpagedGrid<ProfessorStatusDTO> professoresGrid;
	private SimpleToolBar toolBar;
	private SimpleToolBar alunosToolBar;
	private SimpleToolBar professoresToolBar;
	private LabelField planejadoLabel;
	private LabelField naoPlanejadoLabel;
	private LabelField naoAtendidoLabel;
	private ContentPanel demandaPanel;
	private TextField<String> novaTurmaNomeTF;
	private Button novaTurmaBt;
	private Button novaAulaBt;
	private TurmaDTO turmaSelecionada;
	private List<AulaDTO> aulasSelecionadas;
	private AulaDTO aulaNaGrade;
	private String turmaSelecionadaStatus;
	private ContentPanel turmaSelecionadaPanel;
	private LayoutContainer emptyText;
	private Button salvarTurmaBt = new Button();
	private Button confirmarTurmaBt = new Button();
	private Button desconfirmarTurmaBt = new Button();
	private Button editarTurmaBt = new Button();
	private Button removerTurmaBt = new Button();
	private List<Button> mostrarGradeBts;
	private List<Button> editarAulaBts;
	private List<Button> removerAulaBts;
	private SalaAutoCompleteBox salaComboBox;
	private Button proxAmbienteBt;
	private Button antAmbienteBt;
	private Button filtrarBt;
	private GradeHorariaSalaGrid salaGridPanel;
	private RelatorioVisaoSalaFiltro salaFiltro;
	private Button salvarMarcacoesAlunosBt;
	private Button cancelarMarcacoesAlunosBt;
	private Button marcarTodosAlunosBt;
	private Button desmarcarTodosAlunosBt;
	private Button salvarMarcacoesProfessoresBt;
	private Button cancelarMarcacoesProfessoresBt;
	private Button alocacoesAtalhoBt;
	private CheckColumnConfig checkAlunoColumn;
	private CheckColumnConfig checkProfessorColumn;
	private LabelField numAlunosMarcados;
	public AlocacaoManualView( CenarioDTO cenarioDTO, QuintetoDTO<CampusDTO, List<DemandaDTO>, DisciplinaDTO, Integer, Integer> quinteto, ResumoMatriculaDTO resumoMatriculaDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.campusDTO = quinteto.getPrimeiro();
		this.demandasDTO = quinteto.getSegundo();
		this.disciplinaDTO = quinteto.getTerceiro();
		this.resumoMatriculaDTO = resumoMatriculaDTO;
		this.atendimentoNaoPlanejado = quinteto.getQuinto();
		this.atendimentoPlanejado = quinteto.getQuarto();
		this.initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeadingHtml( cenarioDTO.getNome() + " » Alocação Manual" );

		createPanel();
		createTabItem();
		initComponent( this.tabItem );
	}
	
	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			"Alocação Manual", Resources.DEFAULTS.alocacaoManual16() );

		this.tabItem.setContent( this.panel );
	}
	
	private void createPanel()
	{
		BorderLayoutData bldNorth = new BorderLayoutData( LayoutRegion.NORTH );
		bldNorth.setMargins( new Margins( 5, 5, 5, 5 ) );
		bldNorth.setCollapsible(true);
		bldNorth.setSize(100);
		BorderLayoutData bldEast = new BorderLayoutData( LayoutRegion.EAST );
		bldEast.setMargins( new Margins( 5, 5, 5, 5 ) );
		bldEast.setSize(250);
		bldEast.setCollapsible(true);
		BorderLayoutData bldWest = new BorderLayoutData( LayoutRegion.WEST );
		bldWest.setMargins( new Margins( 5, 5, 5, 5 ) );
		bldWest.setSize(250);
		bldWest.setCollapsible(true);
		BorderLayoutData bldCenter = new BorderLayoutData( LayoutRegion.CENTER );
		bldCenter.setMargins( new Margins( 5, 5, 5, 5 ) );
		bldCenter.setCollapsible(true);

	    ContentPanel topPanel = new ContentPanel(new BorderLayout());
	    topPanel.setHeadingHtml("Turmas do Campus " + campusDTO.getNome() + "(" + campusDTO.getCodigo() + ")");
		BorderLayoutData bldNovaTurma = new BorderLayoutData( LayoutRegion.WEST );
		bldNovaTurma.setMargins( new Margins( 5, 5, 5, 5 ) );
	    topPanel.add(createNovaTurmaPanel(), bldNovaTurma);
		BorderLayoutData bldTurmaSelecionada = new BorderLayoutData( LayoutRegion.CENTER );
		bldTurmaSelecionada.setMargins( new Margins( 5, 5, 5, 5 ) );
	    topPanel.add(createTurmaSelecionadaPanel(), bldTurmaSelecionada);
	    
	    ContentPanel leftPanel = new ContentPanel(new BorderLayout());
	    BorderLayoutData bldDisciplina = new BorderLayoutData( LayoutRegion.NORTH );
	    bldDisciplina.setSize(170);
	    leftPanel.add(createDisciplinaDemandaPanel(), bldDisciplina);
	    leftPanel.add(createTurmasGridPanel(), new BorderLayoutData( LayoutRegion.CENTER ));
	    
	    ContentPanel rightPanel = new ContentPanel(new FitLayout());
	    rightPanel.add(createProfessoresAlunosPanel());
	    
	    ContentPanel centerPanel = new ContentPanel(new FitLayout());
	    centerPanel.setHeadingHtml("Grade Horária Visão Ambiente");
	    centerPanel.add(createGradeHorariaPanel());
	    

	    this.panel.add( topPanel, bldNorth );
	    this.panel.add( leftPanel, bldWest );
	    this.panel.add( rightPanel, bldEast );
	    this.panel.add( centerPanel, bldCenter );
	}
	
	private Widget createProfessoresAlunosPanel() {
		TabPanel professoresAlunosTabPanel = new TabPanel();
		
		TabItem alunosTabItem = new TabItem( "Alunos" );
		alunosTabItem.setLayout(new FitLayout());
		TabItem professoresTabItem = new TabItem( "Professores" );
		professoresTabItem.setLayout(new FitLayout());
		
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
		bld.setMargins( new Margins( 5, 5, 5, 5 ) );
		alunosTabItem.add(createAlunosGrid());
		professoresTabItem.add(createProfessoresGrid());
		
		professoresAlunosTabPanel.add(alunosTabItem);
		professoresAlunosTabPanel.add(professoresTabItem);
		
		
		return professoresAlunosTabPanel;
	}
	
	private SimpleUnpagedGrid<AlunoStatusDTO> createAlunosGrid()
	{
		this.alunosToolBar = new SimpleToolBar(
				false, false, false, false, false, this );
		
		salvarMarcacoesAlunosBt = alunosToolBar.createButton("Salvar marcações", Resources.DEFAULTS.salvarMarcacoes16());
		cancelarMarcacoesAlunosBt = alunosToolBar.createButton("Cancelar marcações", Resources.DEFAULTS.cancelarMarcacoes16());
		marcarTodosAlunosBt = alunosToolBar.createButton("Marcar todos alunos", Resources.DEFAULTS.marcarTodos16());
		desmarcarTodosAlunosBt = alunosToolBar.createButton("Desmarcar todos alunos", Resources.DEFAULTS.desmarcarTodos16());
    	salvarMarcacoesAlunosBt.disable();
    	cancelarMarcacoesAlunosBt.disable();
		
		alunosToolBar.add(salvarMarcacoesAlunosBt);
		alunosToolBar.add(cancelarMarcacoesAlunosBt);
		alunosToolBar.add(new SeparatorToolItem());
		alunosToolBar.add(marcarTodosAlunosBt);
		alunosToolBar.add(desmarcarTodosAlunosBt);
		
	    this.alunosGrid = new SimpleUnpagedGrid< AlunoStatusDTO >( new ColumnModel(getAlunosColumnList()), this, this.alunosToolBar )
	    {
			@Override
			public void afterRender()
			{
				super.afterRender();
				getGrid().getStore().addStoreListener(new StoreListener<AlunoStatusDTO>()
			    {
		    		@Override
		    	    public void storeUpdate(StoreEvent<AlunoStatusDTO> se) {
				        if(getAlunosGrid().getGrid().getStore().getModifiedRecords().size() > 0)
				        {
				        	salvarMarcacoesAlunosBt.enable();
				        	cancelarMarcacoesAlunosBt.enable();
				        }
				        else
				        {
				        	salvarMarcacoesAlunosBt.disable();
				        	cancelarMarcacoesAlunosBt.disable();
				        }
				        atualizaNumAlunosMarcados();
		    	    }
			    });
				
				this.addLoadListener( new LoadListener()
				{
					@Override
					public void loaderLoad( LoadEvent le )
					{
						atualizaNumAlunosMarcados();
					}
				});
			}
	    };
	    alunosGrid.addPlugin(checkAlunoColumn);
	    alunosGrid.setTopComponent(alunosToolBar);
	    numAlunosMarcados = new LabelField();
	    numAlunosMarcados.setStyleAttribute("margin-left", "75px");
	    numAlunosMarcados.setValue(getNumAlunosMarcados()+"/ "+getNumAlunosTotal() + " alunos");
	    alunosToolBar.add(numAlunosMarcados);
	    
	    return alunosGrid;
	}
	
	public String getNumAlunosMarcados()
	{
		int numAlunosMarcados = 0;
		if (alunosGrid.getGrid() != null)
		{
			for (AlunoStatusDTO model : alunosGrid.getGrid().getStore().getModels())
			{
				if (model.getMarcado())
					numAlunosMarcados++;
			}
		}
		return NumberFormat.getFormat("00").format(numAlunosMarcados);
	}
	
	public String getNumAlunosTotal()
	{
		int numAlunosTotal = 0;
		if (alunosGrid.getGrid() != null)
		{
			numAlunosTotal = alunosGrid.getGrid().getStore().getModels().size();
		}
		return NumberFormat.getFormat("00").format(numAlunosTotal);
	}
	
	private SimpleUnpagedGrid<ProfessorStatusDTO> createProfessoresGrid()
	{
		this.professoresToolBar = new SimpleToolBar(
				false, false, false, false, false, this );
		
		salvarMarcacoesProfessoresBt = alunosToolBar.createButton("Salvar marcações", Resources.DEFAULTS.salvarMarcacoes16());
		cancelarMarcacoesProfessoresBt = alunosToolBar.createButton("Cancelar marcações", Resources.DEFAULTS.cancelarMarcacoes16());
		salvarMarcacoesProfessoresBt.disable();
		cancelarMarcacoesProfessoresBt.disable();
		
    	professoresToolBar.add(salvarMarcacoesProfessoresBt);
    	professoresToolBar.add(cancelarMarcacoesProfessoresBt);
		
	    this.professoresGrid = new SimpleUnpagedGrid< ProfessorStatusDTO >( new ColumnModel(getProfessoresColumnList()), this, this.professoresToolBar )
	    {
			@Override
			public void afterRender()
			{
				super.afterRender();
				
				getGrid().getStore().addStoreListener(new StoreListener<ProfessorStatusDTO>()
				{
			    	@Override
			    	public void storeUpdate(StoreEvent<ProfessorStatusDTO> se) {
				        if(getProfessoresGrid().getGrid().getStore().getModifiedRecords().size() > 0)
				        {
				        	salvarMarcacoesProfessoresBt.enable();
				        	cancelarMarcacoesProfessoresBt.enable();
				        }
				        else
				        {
				        	salvarMarcacoesProfessoresBt.disable();
				        	cancelarMarcacoesProfessoresBt.disable();
				        }
					}
				});
			}
	    };
	    professoresGrid.addPlugin(checkProfessorColumn);
	    professoresGrid.setTopComponent(professoresToolBar);
	    
	    return professoresGrid;
	}

	private Widget createGradeHorariaPanel() {
		ContentPanel gradeHorariaPanel = new ContentPanel(new BorderLayout());
		gradeHorariaPanel.setHeaderVisible(false);
		gradeHorariaPanel.setBodyBorder(false);
		
		BorderLayoutData bldCenter = new BorderLayoutData( LayoutRegion.CENTER );
		BorderLayoutData bldNorth = new BorderLayoutData( LayoutRegion.NORTH );
		bldNorth.setSize(80);
		bldNorth.setCollapsible(true);
		ContentPanel filtro = new ContentPanel(new CenterLayout())
		{
			@Override
			public void afterRender()
			{
				super.afterRender();
				
				this.getBody().setStyleAttribute("background", "none");
			}
		};
		filtro.setHeadingHtml("Filtro");
		
		LayoutContainer filtroContainer = new LayoutContainer(new ColumnLayout());
		filtroContainer.setBorders(false);
		salaComboBox = new SalaAutoCompleteBox(cenarioDTO);
		getSalaComboBox().setWidth(200);
		getSalaComboBox().setStyleAttribute("margin-left", "20px");
		getSalaComboBox().setStyleAttribute("margin-right", "20px");
		proxAmbienteBt = new Button();
		proxAmbienteBt.disable();
		antAmbienteBt = new Button();
		antAmbienteBt.disable();
		filtrarBt = new Button("Filtrar", AbstractImagePrototype.create(Resources.DEFAULTS.filter16()));
		filtrarBt.setStyleAttribute("margin-right", "20px");
		proxAmbienteBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.prox16()));
		antAmbienteBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.ant16()));
		filtroContainer.add(antAmbienteBt);
		filtroContainer.add(salaComboBox);
		filtroContainer.add(filtrarBt);
		filtroContainer.add(proxAmbienteBt);
		filtro.add(filtroContainer, bldCenter);
		
		ContentPanel grade = new ContentPanel(new FitLayout());
		grade.setHeaderVisible(false);
		grade.setBodyBorder(false);
		
		salaGridPanel = new GradeHorariaSalaGrid(cenarioDTO, true);
		
		salaGridPanel.setEmptyTextBeforeSearch("Preencha o filtro acima e acione o" +
				" botão filtrar para visualizar a grade horária do ambiente desejado.");
		salaFiltro = new RelatorioVisaoSalaFiltro();
		getSalaGridPanel().setFiltro(salaFiltro);
		grade.add(salaGridPanel);
		
		gradeHorariaPanel.add(filtro, bldNorth);
		gradeHorariaPanel.add(grade, bldCenter);
		
		return gradeHorariaPanel;
	}

	private Widget createTurmaSelecionadaPanel() {
		turmaSelecionadaPanel = new ContentPanel(new ColumnLayout());
		turmaSelecionadaPanel.setHeaderVisible(false);
		
		emptyText = new LayoutContainer();
		emptyText.addText("Crie uma nova turma definindo um nome para a mesma ou selecione uma turma existente.");
		emptyText.setStyleAttribute("margin-left", "10px");
		emptyText.setStyleAttribute("margin-top", "20px");
		turmaSelecionadaPanel.add(emptyText);
		
		return turmaSelecionadaPanel;
	}

	private Widget createNovaTurmaPanel() {
		ContentPanel novaTurmaPanel = new ContentPanel();
		novaTurmaPanel.setHeaderVisible(false);
		novaTurmaPanel.setWidth(200);
		BorderLayoutData bldCenter = new BorderLayoutData( LayoutRegion.CENTER );
		bldCenter.setMargins( new Margins( 5, 5, 5, 5 ) );
		
		novaTurmaNomeTF = new TextField<String>();
		novaTurmaNomeTF.setStyleAttribute("margin", "4px");
		novaTurmaNomeTF.setWidth(190);
		novaTurmaNomeTF.setHeight(25);
		novaTurmaNomeTF.setEmptyText("Coloque o nome da turma");

		novaTurmaBt = new Button();
		novaTurmaBt.setText("Nova Turma");
		novaTurmaBt.setWidth(190);
		novaTurmaBt.setHeight(25);
		novaTurmaBt.setStyleAttribute("margin", "4px");
		novaTurmaBt.setIcon(AbstractImagePrototype.create(
			Resources.DEFAULTS.add16() ));
	
		novaTurmaPanel.add(novaTurmaNomeTF);
		novaTurmaPanel.add(novaTurmaBt);
		
		return novaTurmaPanel;
	}

	private Widget createDisciplinaDemandaPanel()
	{
		ContentPanel disciplinaDemandaPanel = new ContentPanel();
		disciplinaDemandaPanel.setHeaderVisible(false);
		disciplinaDemandaPanel.setBodyBorder(false);
		
		disciplinaDemandaPanel.add(createDisciplinaPanel());
		disciplinaDemandaPanel.add(createDemandaPanel());
		
		return disciplinaDemandaPanel;
	}

	private Widget createDisciplinaPanel() {
		ContentPanel disciplinaPanel = new ContentPanel();
		disciplinaPanel.setHeadingHtml(" Disciplina (" + demandasDTO.get(0).getDisciplinaString() + ")");
		disciplinaPanel.addText(disciplinaDTO.getNome());
		alocacoesAtalhoBt = createButton("Abrir Janela de Alocação Manual", Resources.DEFAULTS.alocacaoManual16());
		alocacoesAtalhoBt.setWidth(24);
		alocacoesAtalhoBt.setHeight(20);
		alocacoesAtalhoBt.setStyleAttribute("margin-top", "-4px");
		disciplinaPanel.getHeader().addTool(alocacoesAtalhoBt);
		
		LayoutContainer container = new LayoutContainer(new ColumnLayout());
		LabelField creditos = new LabelField();
		creditos.setValue("Créd: " + disciplinaDTO.getCreditosTeorico() + " Teo " + disciplinaDTO.getCreditosPratico() + " Prát");
		creditos.setStyleAttribute("margin-top", "2px");
		CheckBox cb = new CheckBox();
		cb.setHideLabel( true );
		cb.setBoxLabel( "Exige Lab?" );
		cb.setValue( disciplinaDTO.getLaboratorio() );
		cb.setReadOnly(true);
		
		container.add(creditos, new ColumnData(0.55));
		container.add(cb, new ColumnData(0.45));
		disciplinaPanel.add(container);
		
		
		return disciplinaPanel;
	}
	
	private Widget createDemandaPanel() {
		demandaPanel = new ContentPanel();
		demandaPanel.setHeadingHtml("Demanda");
		
		FormLayout formLayout = new FormLayout(LabelAlign.RIGHT);
		formLayout.setLabelWidth(170);
		LayoutContainer container = new LayoutContainer(formLayout);
		container.addStyleName("alocacaoManualDemanda");
		LabelField total = new LabelField();
		total.setFieldLabel("<b>Total:</b>");
		total.setValue(resumoMatriculaDTO.getDisDemandaP1());
		
		planejadoLabel = new LabelField();
		planejadoLabel.setFieldLabel("<b>Atendimento Planejado:</b>");
		planejadoLabel.setValue(atendimentoPlanejado);
		
		naoPlanejadoLabel = new LabelField();
		naoPlanejadoLabel.setFieldLabel("<b>Atendimento Não Planejado:</b>");
		naoPlanejadoLabel.setValue(atendimentoNaoPlanejado);
		
		naoAtendidoLabel = new LabelField();
		naoAtendidoLabel.setFieldLabel("<b>Não Atendimento:</b>");
		naoAtendidoLabel.setValue(resumoMatriculaDTO.getDisNaoAtendidosP1());
		
		container.add(total);
		container.add(planejadoLabel);
		container.add(naoPlanejadoLabel);
		container.add(naoAtendidoLabel);
		
		demandaPanel.add(container);
		
		return demandaPanel;
	}
	

	private Widget createTurmasGridPanel() {
		ContentPanel turmasGridPanel = new ContentPanel(new BorderLayout());
		turmasGridPanel.setHeadingHtml("Turmas");
		
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
		bld.setMargins( new Margins( 5, 5, 5, 5 ) );
		
		this.toolBar = new SimpleToolBar(
				true, true, true, false, false, this );
		
	    this.turmasGrid = new SimpleUnpagedGrid< TurmaStatusDTO >( getColumnList(), this, this.toolBar );
	    turmasGridPanel.setTopComponent(toolBar);
	    turmasGridPanel.add(turmasGrid, bld);
	    
	    return turmasGridPanel;
	}
	
	private ColumnModel getColumnList()
	{
		List< ColumnConfig > list
			= new ArrayList< ColumnConfig >();
		
	    GridCellRenderer<TurmaStatusDTO> change = new GridCellRenderer<TurmaStatusDTO>() {

			@Override
			public Object render(TurmaStatusDTO model, String property,
					com.extjs.gxt.ui.client.widget.grid.ColumnData config,
					int rowIndex, int colIndex,
					ListStore<TurmaStatusDTO> store, Grid<TurmaStatusDTO> grid) {
			    
		          String val = (String) model.get(property);  
		          String style = "";
		          if (val.equals("Planejada"))
		          {
		        	  style = "blue";
		          }
		          else if(val.equals("Parcial"))
		          {
		        	  style = "red";
		          }
		          else
		          {
		        	  style = "black";
		          }
		          return "<span style='color:" + style + "'>" + val + "</span>";
			}
	    }; 
		
		list.add( new ColumnConfig( TurmaStatusDTO.PROPERTY_NOME, "Nome", 60 ) );
		list.add( new ColumnConfig( TurmaStatusDTO.PROPERTY_QTDE_DISC_SELECIONADA, "Da Disc. Selec.", 90 ) );
		list.add( new ColumnConfig( TurmaStatusDTO.PROPERTY_QTDE_TOTAL, "Total", 50 ) );
		ColumnConfig column = new ColumnConfig( TurmaStatusDTO.PROPERTY_STATUS, "Status", 50 );
		column.setRenderer(change);
		list.add( column );
		
		ColumnModel cm = new ColumnModel(list);
		
		cm.addHeaderGroup(0, 0 , new HeaderGroupConfig("", 1, 1));
		cm.addHeaderGroup(0, 1, new HeaderGroupConfig("Qtd. Alunos", 1, 2));
		cm.addHeaderGroup(0, 3, new HeaderGroupConfig("", 1, 1));
		
		return cm;
	}
	
	private List< ColumnConfig > getAlunosColumnList()
	{
		List< ColumnConfig > list
			= new ArrayList< ColumnConfig >();
		
	    GridCellRenderer<AlunoStatusDTO> change = new GridCellRenderer<AlunoStatusDTO>() {

			@Override
			public Object render(AlunoStatusDTO model, String property,
					com.extjs.gxt.ui.client.widget.grid.ColumnData config,
					int rowIndex, int colIndex,
					ListStore<AlunoStatusDTO> store, Grid<AlunoStatusDTO> grid) {
			    
		          String val = (String) model.get(property);  
		          String style = "";
		          if (val.equals("Hor. Disponível"))
		          {
		        	  style = "green";
		          }
		          else if(val.equals("Conflito"))
		          {
		        	  style = "red";
		          }
		          else
		          {
		        	  style = "blue";
		          }
		          return "<span style='color:" + style + "'>" + val + "</span>";
			}
	    }; 
	    
	    checkAlunoColumn = new CheckColumnConfig( AlunoStatusDTO.PROPERTY_MARCADO, "", 30 );
		CellEditor checkBoxEditor = new CellEditor(new CheckBox());
		checkAlunoColumn.setEditor(checkBoxEditor);
		
		list.add( checkAlunoColumn );
		list.add( new ColumnConfig( AlunoStatusDTO.PROPERTY_MATRICULA, "Matrícula", 60 ) );
		ColumnConfig column = new ColumnConfig( AlunoStatusDTO.PROPERTY_STATUS, "Status", 70 );
		column.setRenderer(change);
		list.add( column );
		list.add( new ColumnConfig( AlunoStatusDTO.PROPERTY_EQUIVALENCIA_STRING, "Equivalência", 60 ) );
		list.add( new CheckColumnConfig( AlunoStatusDTO.PROPERTY_FORMANDO, "Formando", 60 ) );
		list.add( new ColumnConfig( AlunoStatusDTO.PROPERTY_NOME, "Nome", 120 ) );
		
		return list;
	}
	
	private List< ColumnConfig > getProfessoresColumnList()
	{
		List< ColumnConfig > list
			= new ArrayList< ColumnConfig >();
		
	    GridCellRenderer<ProfessorStatusDTO> change = new GridCellRenderer<ProfessorStatusDTO>() {

			@Override
			public Object render(ProfessorStatusDTO model, String property,
					com.extjs.gxt.ui.client.widget.grid.ColumnData config,
					int rowIndex, int colIndex,
					ListStore<ProfessorStatusDTO> store, Grid<ProfessorStatusDTO> grid) {
			    
		          String val = (String) model.get(property);  
		          String style = "";
		          if (val.equals("Disponível"))
		          {
		        	  style = "green";
		          }
		          else if(val.equals("Conflito"))
		          {
		        	  style = "red";
		          }
		          else if(val.equals("Alocado"))
		          {
		        	  style = "blue";
		          }
		          else
		          {
		        	  style = "black";
		          }
		          return "<span style='color:" + style + "'>" + val + "</span>";
			}
	    }; 
	    
	    checkProfessorColumn = new CheckColumnConfig( ProfessorStatusDTO.PROPERTY_MARCADO, "", 30 );
		CellEditor checkBoxEditor = new CellEditor(new CheckBox());        
		checkProfessorColumn.setEditor(checkBoxEditor);
		
		list.add( checkAlunoColumn );
		list.add( new ColumnConfig( ProfessorStatusDTO.PROPERTY_CPF, "Cpf", 60 ) );
		ColumnConfig column = new ColumnConfig( ProfessorStatusDTO.PROPERTY_STATUS, "Status", 70 );
		column.setRenderer(change);
		list.add( column );
		list.add( new ColumnConfig( ProfessorStatusDTO.PROPERTY_TITULACAO, "Titulação", 60 ) );
		list.add( new ColumnConfig( ProfessorStatusDTO.PROPERTY_CUSTO, "Custo", 60 ) );
		list.add( new ColumnConfig( ProfessorStatusDTO.PROPERTY_NOTA, "Nota", 60 ) );
		list.add( new ColumnConfig( ProfessorStatusDTO.PROPERTY_PREFERENCIA, "Pref.", 60 ) );
		list.add( new ColumnConfig( ProfessorStatusDTO.PROPERTY_NOME, "Nome", 60 ) );
		
		return list;
	}
	
	@Override
	public SimpleUnpagedGrid< TurmaStatusDTO > getGrid()
	{
		return this.turmasGrid;
	}
	
	@Override
	public SimpleUnpagedGrid< AlunoStatusDTO > getAlunosGrid()
	{
		return this.alunosGrid;
	}
	
	@Override
	public SimpleUnpagedGrid< ProfessorStatusDTO > getProfessoresGrid()
	{
		return this.professoresGrid;
	}
	
	@Override
	public void setProxy( RpcProxy< ListLoadResult< TurmaStatusDTO > > proxy )
	{
		this.turmasGrid.setProxy( proxy );
	}
	
	@Override
	public void setAlunosProxy( RpcProxy< ListLoadResult< AlunoStatusDTO > > proxy )
	{
		this.alunosGrid.setProxy( proxy );
	}
	
	@Override
	public void setProfessoresProxy( RpcProxy< ListLoadResult< ProfessorStatusDTO > > proxy )
	{
		this.professoresGrid.setProxy( proxy );
	}
	
	@Override
	public Button getTurmasRemoveButton()
	{
		return this.toolBar.getRemoveButton();
	}
	
	@Override
	public Button getTurmasEditarButton()
	{
		return this.toolBar.getEditButton();
	}
	
	@Override
	public Button getTurmasNewButton()
	{
		return this.toolBar.getNewButton();
	}
	
	@Override
	public Button getTurmasNewButton2()
	{
		return this.novaTurmaBt;
	}
	
	@Override
	public Button getNovaAulaButton()
	{
		return this.novaAulaBt;
	}
	
	
	@Override
	public TextField<String> getNovaTurmaNomeTextField()
	{
		return this.novaTurmaNomeTF;
	}
	
	@Override
	public DemandaDTO getDemanda()
	{
		return demandasDTO.get(0);
	}
	
	@Override
	public DisciplinaDTO getDisciplinaDTO()
	{
		return disciplinaDTO;
	}
	
	@Override
	public CampusDTO getCampusDTO()
	{
		return campusDTO;
	}
	
	@Override
	public Button getSalvarTurmaButton()
	{
		return salvarTurmaBt;
	}
	
	@Override
	public Button getConfirmarTurmaButton()
	{
		return confirmarTurmaBt;
	}
	
	@Override
	public Button getDesconfirmarTurmaButton()
	{
		return desconfirmarTurmaBt;
	}
	
	@Override
	public Button getEditarTurmaButton()
	{
		return editarTurmaBt;
	}
	
	@Override
	public Button getRemoverTurmaButton()
	{
		return removerTurmaBt;
	}
	
	@Override
	public void refreshDemandasPanel(int planejado, int naoPlanejado, int naoAtendido )
	{
		planejadoLabel.setValue(Integer.valueOf((String)planejadoLabel.getValue()) + planejado);
		naoPlanejadoLabel.setValue(Integer.valueOf((String)naoPlanejadoLabel.getValue()) + naoPlanejado);
		naoAtendidoLabel.setValue(Integer.valueOf((String)naoAtendidoLabel.getValue()) + naoAtendido);
		demandaPanel.layout();
	}
	
	@Override
	public void refreshTurmaSelecionadaPanel()
	{
		if (turmaSelecionada == null)
		{
			turmaSelecionadaPanel.removeAll();
			turmaSelecionadaPanel.add(emptyText);
			turmaSelecionadaPanel.layout();
		}
		else
		{
			turmaSelecionadaPanel.removeAll();
			turmaSelecionadaPanel.add(createTurmaSubPanel());
			criaBotoesAulas();
			for (int i = 0; i < aulasSelecionadas.size(); i++)
			{
				turmaSelecionadaPanel.add(createSeparator());
				turmaSelecionadaPanel.add(createAulasSubPanel(aulasSelecionadas.get(i), i));
			}
			turmaSelecionadaPanel.add(createSeparator());
			turmaSelecionadaPanel.add(createNovaAulaSubPanel());
			turmaSelecionadaPanel.layout();
		}
	}
	
	private void criaBotoesAulas() {
		mostrarGradeBts = new ArrayList<Button>();
		editarAulaBts = new ArrayList<Button>();
		removerAulaBts = new ArrayList<Button>();
		for (int i = 0; i < aulasSelecionadas.size(); i++)
		{
			mostrarGradeBts.add(toolBar.createButton("Mostrar na Grade Horária", 
				Resources.DEFAULTS.saidaSala16() ));
			editarAulaBts.add(toolBar.createButton("Editar Aula",
					Resources.DEFAULTS.aulaEdit16() ));
			removerAulaBts.add(toolBar.createButton("Remover Aula",
					Resources.DEFAULTS.aulaDelete16() ));
		}
		
	}

	@Override
	public void setTurmaSelecionada(TurmaDTO turmaDTO, List<AulaDTO> aulas, String status)
	{
		this.turmaSelecionada = turmaDTO;
		this.turmaSelecionadaStatus = status;
		this.aulasSelecionadas = aulas;
	}
	
	@Override
	public String getTurmaSelecionadaStatus()
	{
		return this.turmaSelecionadaStatus;
	}
	
	@Override
	public TurmaDTO getTurmaSelecionada()
	{
		return this.turmaSelecionada;
	}
	
	private Widget createTurmaSubPanel() {
		ContentPanel turmaSubPanel = new ContentPanel();
		turmaSubPanel.setHeaderVisible(false);
		turmaSubPanel.setBodyBorder(false);
		turmaSubPanel.setWidth(320);
		
		LayoutContainer textContainer = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));
		textContainer.addStyleName("alocacaoManualDemanda");
		textContainer.setWidth(200);
		LabelField turmaLabel = new LabelField();
		turmaLabel.setFieldLabel("<b>Turma:</b>");
		turmaLabel.setValue(turmaSelecionada.getNome() + " (" + (disciplinaDTO.getCodigo()) + ")");
		
		LabelField noAlunosLabel = new LabelField();
		noAlunosLabel.setFieldLabel("<b>Nº Alunos:</b>");
		noAlunosLabel.setValue(turmaSelecionada.getNoAlunos());
		
		LabelField credAlocadosLabel = new LabelField();
		credAlocadosLabel.setFieldLabel("<b>Cred. Alocados:</b>");
		credAlocadosLabel.setValue(turmaSelecionada.getCredAlocados() + " de " +
				(disciplinaDTO.getCreditosPratico() + disciplinaDTO.getCreditosTeorico()) );
		
		textContainer.add(turmaLabel);
		textContainer.add(noAlunosLabel);
		textContainer.add(credAlocadosLabel);
		
		LayoutContainer rightContainer = new LayoutContainer(new RowLayout());
		LayoutContainer buttonsContainer = new LayoutContainer(new ColumnLayout());
		salvarTurmaBt.setIcon(AbstractImagePrototype.create(
				Resources.DEFAULTS.disk16() ));
		salvarTurmaBt.disable();
		confirmarTurmaBt.setIcon(AbstractImagePrototype.create(
				Resources.DEFAULTS.senha16() ));
		desconfirmarTurmaBt.setIcon(AbstractImagePrototype.create(
				Resources.DEFAULTS.desconfirmarTurma16() ));
		
		if ((turmaSelecionada.getCredAlocados() == disciplinaDTO.getCreditosPratico() + disciplinaDTO.getCreditosTeorico())
				&& turmaSelecionada.getNoAlunos() > 0
				&& getTurmaSelecionadaStatus().equals("Parcial"))
		{
			salvarTurmaBt.enable();
		}
		editarTurmaBt.setIcon(AbstractImagePrototype.create(
				Resources.DEFAULTS.edit16() ));
		removerTurmaBt.setIcon(AbstractImagePrototype.create(
				Resources.DEFAULTS.del16() ));
		LabelField statusLabel = new LabelField();
		String tumaSelecionadaStatusColor;
		if (turmaSelecionadaStatus.equals("Parcial"))
		{
			tumaSelecionadaStatusColor = "<span style='color: red'>" + turmaSelecionadaStatus + "</span>";
		}
		else if (turmaSelecionadaStatus.equals("Planejada"))
		{
			tumaSelecionadaStatusColor = "<span style='color: blue'>" + turmaSelecionadaStatus + "</span>";
		}
		else
		{
			tumaSelecionadaStatusColor = "<span style='color: black'>" + turmaSelecionadaStatus + "</span>";
		}
		statusLabel.setValue("<b>Status: </b>" + tumaSelecionadaStatusColor);
		statusLabel.setStyleAttribute("font-size", "10px");
		if (turmaSelecionadaStatus.equals("Parcial"))
		{
			buttonsContainer.add(salvarTurmaBt);
		}
		else if (turmaSelecionadaStatus.equals("Planejada"))
		{
			buttonsContainer.add(desconfirmarTurmaBt);
		}
		else
		{
			buttonsContainer.add(confirmarTurmaBt);
		}
		buttonsContainer.add(editarTurmaBt);
		buttonsContainer.add(removerTurmaBt);
		rightContainer.add(buttonsContainer);
		rightContainer.add(statusLabel);
		
		LayoutContainer container = new LayoutContainer(new ColumnLayout());
		container.add(textContainer);
		container.add(rightContainer);
		
		turmaSubPanel.add(container);
		
		return turmaSubPanel;
	}
	
	private Widget createAulasSubPanel(AulaDTO aulaDTO, int index) {
		ContentPanel aulaSubPanel = new ContentPanel();
		aulaSubPanel.setHeaderVisible(false);
		aulaSubPanel.setBodyBorder(false);
		aulaSubPanel.setWidth(230);
		
		FormLayout layout = new FormLayout(LabelAlign.RIGHT);
		layout.setLabelWidth(45);
		LayoutContainer textContainer = new LayoutContainer(layout);
		textContainer.addStyleName("alocacaoManualDemanda");
		textContainer.setWidth(150);
		LabelField horarioLabel = new LabelField();
		horarioLabel.setFieldLabel(aulaDTO.getHorarioString());
		horarioLabel.setLabelStyle("width: 115px");
		
		LabelField ambienteLabel = new LabelField();
		ambienteLabel.setFieldLabel("<b>Amb.:</b>");
		ambienteLabel.setValue(aulaDTO.getSalaString());
		
		LabelField professorLabel = new LabelField();
		professorLabel.setFieldLabel("<b>Prof.:</b>");
		professorLabel.setValue( aulaDTO.getProfessorNome() );
		
		textContainer.add(horarioLabel);
		textContainer.add(ambienteLabel);
		textContainer.add(professorLabel);
		
		LayoutContainer rightContainer = new LayoutContainer(new RowLayout());
		LayoutContainer buttonsContainer = new LayoutContainer(new ColumnLayout());
		buttonsContainer.add(mostrarGradeBts.get(index));
		buttonsContainer.add(editarAulaBts.get(index));
		buttonsContainer.add(removerAulaBts.get(index));
		rightContainer.add(buttonsContainer);
		
		LayoutContainer container = new LayoutContainer(new ColumnLayout());
		container.add(textContainer);
		container.add(rightContainer);
		
		aulaSubPanel.add(container);
		
		return aulaSubPanel;
	}
	
	private Widget createNovaAulaSubPanel() {
		ContentPanel aulaSubPanel = new ContentPanel(new ColumnLayout());
		aulaSubPanel.setHeaderVisible(false);
		aulaSubPanel.setBodyBorder(false);
		aulaSubPanel.setWidth(200);
		
		novaAulaBt = new Button();
		novaAulaBt.setIcon(AbstractImagePrototype.create(
				Resources.DEFAULTS.novaTurma16() ));
		novaAulaBt.setStyleAttribute("margin-left", "5px");
		
		LabelField nome = new LabelField();
		nome.setValue("Nova Aula");
		nome.setStyleAttribute("margin-left", "5px");
		
		if (turmaSelecionada.getCredAlocados() == disciplinaDTO.getCreditosPratico() + disciplinaDTO.getCreditosTeorico())
		{
			novaAulaBt.disable();
		}
		
		aulaSubPanel.add(novaAulaBt);
		aulaSubPanel.add(nome);
		
		return aulaSubPanel;
	}
	
	public Widget createSeparator()
	{
		ContentPanel separator = new ContentPanel();
		separator.setHeight(50);
		separator.setWidth(2);
		separator.setHeaderVisible(false);
		separator.setStyleAttribute("margin-top", "6px");
		
		return separator;
	}
	
	public Button createButton( String toolTip, ImageResource img )
	{
		Button bt = new Button();

		bt.setIcon( AbstractImagePrototype.create( img ) );
		bt.setToolTip( toolTip );

		return bt;
	}

	@Override
	public SalaAutoCompleteBox getSalaComboBox() {
		return salaComboBox;
	}

	public Button getProxAmbienteButton() {
		return proxAmbienteBt;
	}

	@Override
	public Button getAntAmbienteButton() {
		return antAmbienteBt;
	}

	@Override
	public Button getFiltrarButton() {
		return filtrarBt;
	}
	
	@Override
	public GradeHorariaSalaGrid getSalaGridPanel() {
		return salaGridPanel;
	}

	@Override
	public RelatorioVisaoSalaFiltro getSalaFiltro() {
		return salaFiltro;
	}
	
	@Override
	public List<AulaDTO> getAulasSelecionadas()
	{
		return aulasSelecionadas;
	}
	
	@Override
	public List<Button> getMostrarGradeBts()
	{
		return mostrarGradeBts;
	}
	
	@Override
	public List<Button> getEditarAulaBts()
	{
		return editarAulaBts;
	}
	
	@Override
	public List<Button> getRemoverAulaBts()
	{
		return removerAulaBts;
	}
	
	@Override
	public Button getSalvarMarcacoesAlunosButton()
	{
		return salvarMarcacoesAlunosBt;
	}
	
	@Override
	public Button getSalvarMarcacoesProfessoresButton()
	{
		return salvarMarcacoesProfessoresBt;
	}
	
	@Override
	public Button getCancelarMarcacoesAlunosButton()
	{
		return cancelarMarcacoesAlunosBt;
	}
	
	@Override
	public Button getMarcarTodosAlunosButton()
	{
		return marcarTodosAlunosBt;
	}
	
	@Override
	public Button getDesmarcarTodosAlunosButton()
	{
		return desmarcarTodosAlunosBt;
	}
	
	@Override
	public Button getCancelarMarcacoesProfessoresButton()
	{
		return cancelarMarcacoesProfessoresBt;
	}
	
	@Override
	public Button getAlocacoesAtalhoButton()
	{
		return alocacoesAtalhoBt;
	}
	
	@Override
	public void setAulaNaGrade(AulaDTO aulaDTO)
	{
		aulaNaGrade = aulaDTO;
	}
	
	@Override
	public AulaDTO getAulaNaGrade()
	{
		return aulaNaGrade;
	}
	
	@Override
	public void atualizaNumAlunosMarcados()
	{
		numAlunosMarcados.setValue(getNumAlunosMarcados()+"/ "+getNumAlunosTotal() + " alunos");
		alunosToolBar.layout();
	}
}
