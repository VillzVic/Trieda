package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.LabelField;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.main.client.mvp.presenter.AlocacaoManualPresenter;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.QuintetoDTO;
import com.gapso.web.trieda.shared.dtos.ResumoMatriculaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaStatusDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SalaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.SimpleUnpagedGrid;
import com.google.gwt.user.client.ui.AbstractImagePrototype;
import com.google.gwt.user.client.ui.Widget;

public class AlocacaoManualView
	extends MyComposite
	implements AlocacaoManualPresenter.Display
{
	private ContentPanel panel;
	private GTabItem tabItem;
	private CenarioDTO cenarioDTO;
	private DemandaDTO demandaDTO;
	private DisciplinaDTO disciplinaDTO;
	private CampusDTO campusDTO;
	private ResumoMatriculaDTO resumoMatriculaDTO;
	private Integer atendimentoNaoPlanejado;
	private Integer atendimentoPlanejado;
	private SimpleUnpagedGrid<TurmaStatusDTO> turmasGrid;
	private SimpleToolBar toolBar;
	private LabelField planejadoLabel;
	private LabelField naoPlanejadoLabel;
	private LabelField naoAtendidoLabel;
	private ContentPanel demandaPanel;
	private TextField<String> novaTurmaNomeTF;
	private Button novaTurmaBt;
	private TurmaDTO turmaSelecionada;
	private String turmaSelecionadaStatus;
	private ContentPanel turmaSelecionadaPanel;
	private LayoutContainer emptyText;
	private Button salvarTurmaBt = new Button();
	private Button editarTurmaBt = new Button();
	private Button removerTurmaBt = new Button();
	private Button selecionarTurmaBt;
	public AlocacaoManualView( CenarioDTO cenarioDTO, QuintetoDTO<CampusDTO, DemandaDTO, DisciplinaDTO, Integer, Integer> quinteto, ResumoMatriculaDTO resumoMatriculaDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.campusDTO = quinteto.getPrimeiro();
		this.demandaDTO = quinteto.getSegundo();
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
	    topPanel.setHeadingHtml("Turma");
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
	    
	    ContentPanel rightPanel = new ContentPanel();
	    
	    ContentPanel centerPanel = new ContentPanel(new FitLayout());
	    centerPanel.setHeadingHtml("Grade Horária Visão Ambiente");
	    centerPanel.add(createGradeHorariaPanel());
	    

	    this.panel.add( topPanel, bldNorth );
	    this.panel.add( leftPanel, bldWest );
	    this.panel.add( rightPanel, bldEast );
	    this.panel.add( centerPanel, bldCenter );
	}
	
	private Widget createGradeHorariaPanel() {
		ContentPanel gradeHorariaPanel = new ContentPanel(new BorderLayout());
		gradeHorariaPanel.setHeaderVisible(false);
		gradeHorariaPanel.setBodyBorder(false);
		
		BorderLayoutData bldCenter = new BorderLayoutData( LayoutRegion.CENTER );
		BorderLayoutData bldNorth = new BorderLayoutData( LayoutRegion.NORTH );
		bldNorth.setSize(80);
		bldNorth.setCollapsible(true);
		ContentPanel filtro = new ContentPanel(new BorderLayout());
		filtro.setHeadingHtml("Filtro");
		filtro.setStyleAttribute("background", "none");
		
		LayoutContainer filtroContainer = new LayoutContainer();
		filtroContainer.setWidth(200);
		SalaAutoCompleteBox salaComboBox = new SalaAutoCompleteBox(cenarioDTO);
		filtroContainer.add(salaComboBox);
		filtro.add(filtroContainer, bldCenter);
		
		ContentPanel grade = new ContentPanel();
		grade.setHeaderVisible(false);
		
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
		disciplinaPanel.setHeadingHtml(" Disciplina (" + demandaDTO.getDisciplinaString() + ")");
		disciplinaPanel.addText(disciplinaDTO.getNome());
		
		LayoutContainer container = new LayoutContainer(new ColumnLayout());
		LabelField creditos = new LabelField();
		creditos.setValue("Créd: " + disciplinaDTO.getCreditosTeorico() + "Teo " + disciplinaDTO.getCreditosPratico() + "Pra");
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
		
		LayoutContainer container = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));
		container.addStyleName("alocacaoManualDemanda");
		LabelField total = new LabelField();
		total.setFieldLabel("<b>Total:</b>");
		total.setValue(resumoMatriculaDTO.getDisDemandaP1());
		
		planejadoLabel = new LabelField();
		planejadoLabel.setFieldLabel("<b>Atend. Plan.:</b>");
		planejadoLabel.setValue(atendimentoPlanejado);
		
		naoPlanejadoLabel = new LabelField();
		naoPlanejadoLabel.setFieldLabel("<b>Atend. Não Plan.:</b>");
		naoPlanejadoLabel.setValue(atendimentoNaoPlanejado);
		
		naoAtendidoLabel = new LabelField();
		naoAtendidoLabel.setFieldLabel("<b>Não Atend.:</b>");
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
		
		selecionarTurmaBt = toolBar.createButton(
			"Selecionar Turma",
			Resources.DEFAULTS.selecionarTurma16() );
		selecionarTurmaBt.disable();
		
		toolBar.add(new SeparatorToolItem());
		
		toolBar.add(selecionarTurmaBt);
		
	    this.turmasGrid = new SimpleUnpagedGrid< TurmaStatusDTO >( getColumnList(), this, this.toolBar )
	    {
			@Override
			public void afterRender()
			{
				super.afterRender();
				
				getGrid().getSelectionModel().addSelectionChangedListener(new SelectionChangedListener<TurmaStatusDTO>() {

					@Override
				    public void selectionChanged(SelectionChangedEvent<TurmaStatusDTO> se) {
				        if(getGrid().getSelectionModel().getSelectedItems().size() == 1) {
				        	selecionarTurmaBt.enable();
				        }
				        else{
				        	selecionarTurmaBt.disable();
				        }
				    }
				});
			}
	    };
	    turmasGridPanel.setTopComponent(toolBar);
	    turmasGridPanel.add(turmasGrid, bld);
	    
	    return turmasGridPanel;
	}
	
	private List< ColumnConfig > getColumnList()
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
		
		return list;
	}
	
	@Override
	public SimpleUnpagedGrid< TurmaStatusDTO > getGrid()
	{
		return this.turmasGrid;
	}
	
	@Override
	public void setProxy( RpcProxy< ListLoadResult< TurmaStatusDTO > > proxy )
	{
		this.turmasGrid.setProxy( proxy );
	}
	
	@Override
	public Button getTurmasRemoveButton()
	{
		return this.toolBar.getRemoveButton();
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
	public TextField<String> getNovaTurmaNomeTextField()
	{
		return this.novaTurmaNomeTF;
	}
	
	@Override
	public DemandaDTO getDemanda()
	{
		return demandaDTO;
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
			turmaSelecionadaPanel.layout();
		}
	}
	
	@Override
	public void setTurmaSelecionada(TurmaDTO turmaDTO, String status)
	{
		this.turmaSelecionada = turmaDTO;
		this.turmaSelecionadaStatus = status;
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
	
	@Override
	public Button getSelecionarTurmaButton()
	{
		return this.selecionarTurmaBt;
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
		noAlunosLabel.setFieldLabel("<b>No Alunos:</b>");
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
		buttonsContainer.add(salvarTurmaBt);
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
}
