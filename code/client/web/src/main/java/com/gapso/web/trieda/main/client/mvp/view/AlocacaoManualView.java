package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.LabelField;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.extjs.gxt.ui.client.widget.toolbar.PagingToolBar;
import com.gapso.web.trieda.main.client.mvp.presenter.AlocacaoManualPresenter;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.QuartetoDTO;
import com.gapso.web.trieda.shared.dtos.ResumoMatriculaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaStatusDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.SimpleUnpagedGrid;
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
	private ResumoMatriculaDTO resumoMatriculaDTO;
	private Integer atendimentoNaoPlanejado;
	private Integer atendimentoPlanejado;
	private SimpleUnpagedGrid<TurmaStatusDTO> turmasGrid;
	private SimpleToolBar toolBar;
	private LabelField planejadoLabel;
	private LabelField naoPlanejadoLabel;
	private LabelField naoAtendidoLabel;
	private ContentPanel demandaPanel;

	public AlocacaoManualView( CenarioDTO cenarioDTO, QuartetoDTO<DemandaDTO, DisciplinaDTO, Integer, Integer> quarteto, ResumoMatriculaDTO resumoMatriculaDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.demandaDTO = quarteto.getPrimeiro();
		this.disciplinaDTO = quarteto.getSegundo();
		this.resumoMatriculaDTO = resumoMatriculaDTO;
		this.atendimentoNaoPlanejado = quarteto.getQuarto();
		this.atendimentoPlanejado = quarteto.getTerceiro();
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
			"Alocação Manual", Resources.DEFAULTS.confirmacao16() );

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

	    ContentPanel topPanel = new ContentPanel();
	    topPanel.setHeadingHtml("Turma");
	    
	    ContentPanel leftPanel = new ContentPanel(new BorderLayout());
	    BorderLayoutData bldDisciplina = new BorderLayoutData( LayoutRegion.NORTH );
	    bldDisciplina.setSize(170);
	    leftPanel.add(createDisciplinaDemandaPanel(), bldDisciplina);
	    leftPanel.add(createTurmasGridPanel(), new BorderLayoutData( LayoutRegion.CENTER ));
	    
	    ContentPanel rightPanel = new ContentPanel();
	    
	    ContentPanel centerPanel = new ContentPanel();
	    

	    this.panel.add( topPanel, bldNorth );
	    this.panel.add( leftPanel, bldWest );
	    this.panel.add( rightPanel, bldEast );
	    this.panel.add( centerPanel, bldCenter );
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
		
	    this.turmasGrid = new SimpleUnpagedGrid< TurmaStatusDTO >( getColumnList(), this, this.toolBar );
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
		          else if(val.equals("Pacial"))
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
	public DemandaDTO getDemanda()
	{
		return demandaDTO;
	}
	
	@Override
	public void refreshDemandasPanel(int planejado, int naoPlanejado, int naoAtendido )
	{
		planejadoLabel.setValue(Integer.valueOf((String)planejadoLabel.getValue()) + planejado);
		naoPlanejadoLabel.setValue(Integer.valueOf((String)naoPlanejadoLabel.getValue()) + naoPlanejado);
		naoAtendidoLabel.setValue(Integer.valueOf((String)naoAtendidoLabel.getValue()) + naoAtendido);
		demandaPanel.layout();
	}
}
