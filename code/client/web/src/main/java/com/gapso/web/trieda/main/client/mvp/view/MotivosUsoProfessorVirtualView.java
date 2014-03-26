package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.TabItem;
import com.extjs.gxt.ui.client.widget.TabPanel;
import com.extjs.gxt.ui.client.widget.form.FieldSet;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.extjs.gxt.ui.client.widget.grid.HeaderGroupConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.MotivosUsoProfessorVirtualPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ConfirmacaoTurmaDTO;
import com.gapso.web.trieda.shared.dtos.DicaEliminacaoProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.MotivoUsoProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class MotivosUsoProfessorVirtualView 
	extends MyComposite
	implements MotivosUsoProfessorVirtualPresenter.Display
{

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private CenarioDTO cenarioDTO;
	private ProfessorVirtualDTO professorVirtualDTO;
	private Grid<ConfirmacaoTurmaDTO> turmaGrid;
	private Grid<MotivoUsoProfessorVirtualDTO> motivosProfessorGrid;
	private Grid<DicaEliminacaoProfessorVirtualDTO> dicasProfessorGrid;
	private ListStore< ConfirmacaoTurmaDTO > turmaStore = new ListStore< ConfirmacaoTurmaDTO >();
	private ListStore< MotivoUsoProfessorVirtualDTO > motivosStore = new ListStore< MotivoUsoProfessorVirtualDTO >();
	private ListStore< DicaEliminacaoProfessorVirtualDTO > dicasStore = new ListStore< DicaEliminacaoProfessorVirtualDTO >();
	
	public MotivosUsoProfessorVirtualView( CenarioDTO cenarioDTO, ProfessorVirtualDTO professorVirtualDTO )
	{
		this.professorVirtualDTO = professorVirtualDTO;
		this.cenarioDTO = cenarioDTO;

		initUI();
	}
	
	private void initUI()
	{
		String title = cenarioDTO.getNome() + " » Motivos de Uso e Dicas para Eliminação (" + professorVirtualDTO.getNome() + ")";

		simpleModal = new SimpleModal(
			title, Resources.DEFAULTS.demanda16() );

		simpleModal.setHeight( 600 );
		simpleModal.setWidth(650);
		createForm();
		simpleModal.setContent( formPanel );
	}
	
	private void createForm()
	{
		FormData formData = new FormData( "-20" );
		this.formPanel = new FormPanel();
		this.formPanel.setHeaderVisible( false );
		
		FieldSet professorVirtualFS = new FieldSet();
		professorVirtualFS.setHeadingHtml("Professor Virtual");
		professorVirtualFS.setCollapsible(true);
		professorVirtualFS.setLayout(new FormLayout());
		
		TextField<String> profVirtualNome = new TextField<String>();
		profVirtualNome.setFieldLabel("Nome");
		profVirtualNome.setValue(professorVirtualDTO.getNome());
		profVirtualNome.setReadOnly(true);
		professorVirtualFS.add(profVirtualNome, formData);
		
		LayoutContainer left = new LayoutContainer(new FormLayout(LabelAlign.LEFT));
		LayoutContainer right = new LayoutContainer(new FormLayout(LabelAlign.LEFT));
		final LayoutContainer profVirtualContainer = new LayoutContainer(new ColumnLayout());
		
		TextField<String> profVirtualTitulacao = new TextField<String>();
		profVirtualTitulacao.setFieldLabel("Titulação");
		profVirtualTitulacao.setValue(professorVirtualDTO.getTitulacaoString());
		profVirtualTitulacao.setReadOnly(true);
		left.add(profVirtualTitulacao, formData);
		
		TextField<String> profVirtualTipoContrato = new TextField<String>();
		profVirtualTipoContrato.setFieldLabel("Tipo de Contrato");
		profVirtualTipoContrato.setValue(professorVirtualDTO.getTipoContratoString());
		profVirtualTipoContrato.setReadOnly(true);
		right.add(profVirtualTipoContrato, formData);
		
		profVirtualContainer.add(left, new ColumnData(0.5));
		profVirtualContainer.add(right, new ColumnData(0.5));
		
		professorVirtualFS.add(profVirtualContainer, formData);
		formPanel.add(professorVirtualFS);
		
		FieldSet turmasFS = new FieldSet();
		turmasFS.setHeadingHtml("Turmas Ministradas pelo Professor Virtual");
		turmasFS.setCollapsible(false);
		turmasFS.setLayout(new FormLayout());
		
		createGrids();
		
		turmasFS.add(turmaGrid);
		
		TabPanel motivosDicasTab = new TabPanel();
		motivosDicasTab.setHeight(188);
		motivosDicasTab.setWidth(594);
		
		TabItem motivosTabItem = new TabItem( "Motivos" );
		TabItem dicasTabItem = new TabItem( "Dicas" );
		motivosTabItem.add(motivosProfessorGrid);
		dicasTabItem.add(dicasProfessorGrid);
		motivosDicasTab.add(motivosTabItem);
		motivosDicasTab.add(dicasTabItem);
		
		turmasFS.add(motivosDicasTab);
		formPanel.add(turmasFS);
		
		this.simpleModal.getSalvarBt().hide();
		this.simpleModal.getCancelarBt().setText("Fechar");

		this.simpleModal.setFocusWidget( professorVirtualFS );
	}
	
	private void createGrids()
	{
		BorderLayoutData bld
			= new BorderLayoutData( LayoutRegion.CENTER );

	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );
	    
	    this.turmaGrid = new Grid<ConfirmacaoTurmaDTO>( this.getTurmaStore(),  getTurmaColumnList() );
	    turmaGrid.setBorders(true);
	    turmaGrid.setHeight(180);
	    turmaGrid.setWidth(594);
	    
	    this.motivosProfessorGrid = new Grid<MotivoUsoProfessorVirtualDTO>( this.getMotivosStore(),  getMotivosColumnList() );
	    motivosProfessorGrid.setBorders(true);
	    motivosProfessorGrid.setHeight(160);
	    motivosProfessorGrid.setWidth(594);
	    motivosProfessorGrid.getView().setEmptyText("Selecione uma turma na tabela acima");
	    motivosProfessorGrid.setStyleAttribute("white-space", "normal");
	    
	    this.dicasProfessorGrid = new Grid<DicaEliminacaoProfessorVirtualDTO>( this.getDicasStore(),  getDicasColumnList() );
	    dicasProfessorGrid.setBorders(true);
	    dicasProfessorGrid.setHeight(160);
	    dicasProfessorGrid.setWidth(594);
	    dicasProfessorGrid.getView().setEmptyText("Selecione uma turma na tabela acima");
	}
	
	private ColumnModel getTurmaColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( ConfirmacaoTurmaDTO.PROPERTY_DISCIPLINA_CODIGO, "Código", 65 ) );
		list.add( new ColumnConfig( ConfirmacaoTurmaDTO.PROPERTY_DISCIPLINA_NOME, "Nome", 70 ) );
		list.add( new ColumnConfig( ConfirmacaoTurmaDTO.PROPERTY_CREDITOS_TEORICO, "Teóricos", 60 ) );
		list.add( new ColumnConfig( ConfirmacaoTurmaDTO.PROPERTY_CREDITOS_PRATICO, "Práticos", 60 ) );
		list.add( new ColumnConfig( ConfirmacaoTurmaDTO.PROPERTY_TURMA, "Turma", 50 ) );
		list.add( new ColumnConfig( ConfirmacaoTurmaDTO.PROPERTY_QUANTIDADE_ALUNOS, "Qtde Alunos", 70 ) );
		list.add( new ColumnConfig( ConfirmacaoTurmaDTO.PROPERTY_SALA_STRING, "Ambientes", 70 ) );
		list.add( new ColumnConfig( ConfirmacaoTurmaDTO.PROPERTY_DIAS_HORARIOS, "Dias e Horários de Aula", 125 ) );

		ColumnModel cm = new ColumnModel(list);
		
		cm.addHeaderGroup(0, 0 , new HeaderGroupConfig("Disciplina", 1, 2));
		cm.addHeaderGroup(0, 2, new HeaderGroupConfig("Qtde Créditos", 1, 2));
		cm.addHeaderGroup(0, 4, new HeaderGroupConfig("", 1, 4));
		
		return cm;
	}
	
	private ColumnModel getMotivosColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();
		
	    GridCellRenderer<MotivoUsoProfessorVirtualDTO> change = new GridCellRenderer<MotivoUsoProfessorVirtualDTO>() {

			@Override
			public Object render(MotivoUsoProfessorVirtualDTO model, String property,
					com.extjs.gxt.ui.client.widget.grid.ColumnData config,
					int rowIndex, int colIndex,
					ListStore<MotivoUsoProfessorVirtualDTO> store, Grid<MotivoUsoProfessorVirtualDTO> grid) {
			    
		          String val = (String) model.get(property);  
		          return "<span style='white-space: normal'>" + val + "</span>";
			}
	    }; 

	    ColumnConfig nome = new ColumnConfig( MotivoUsoProfessorVirtualDTO.PROPERTY_MOTIVO, "Motivo", 360 );
	    nome.setRenderer(change);
		list.add( nome );
		list.add( new ColumnConfig( MotivoUsoProfessorVirtualDTO.PROPERTY_PROFESSOR_CPF, "Nome", 100 ) );
		list.add( new ColumnConfig( MotivoUsoProfessorVirtualDTO.PROPERTY_PROFESSOR_STRING, "Teóricos", 110 ) );

		ColumnModel cm = new ColumnModel(list);
		
		cm.addHeaderGroup(0, 0 , new HeaderGroupConfig("", 1, 1));
		cm.addHeaderGroup(0, 1, new HeaderGroupConfig("Professor Real", 1, 2));
		
		return cm;
	}

	private ColumnModel getDicasColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();
		
	    GridCellRenderer<DicaEliminacaoProfessorVirtualDTO> change = new GridCellRenderer<DicaEliminacaoProfessorVirtualDTO>() {

			@Override
			public Object render(DicaEliminacaoProfessorVirtualDTO model, String property,
					com.extjs.gxt.ui.client.widget.grid.ColumnData config,
					int rowIndex, int colIndex,
					ListStore<DicaEliminacaoProfessorVirtualDTO> store, Grid<DicaEliminacaoProfessorVirtualDTO> grid) {
			    
		          String val = (String) model.get(property);  
		          return "<span style='white-space: normal'>" + val + "</span>";
			}
	    };

	    ColumnConfig nome = new ColumnConfig( DicaEliminacaoProfessorVirtualDTO.PROPERTY_DICA, "Dica", 360 );
	    nome.setRenderer(change);
		list.add( nome );
		list.add( new ColumnConfig( DicaEliminacaoProfessorVirtualDTO.PROPERTY_PROFESSOR_CPF, "Cpf", 100 ) );
		list.add( new ColumnConfig( DicaEliminacaoProfessorVirtualDTO.PROPERTY_PROFESSOR_STRING, "Nome", 110 ) );

		ColumnModel cm = new ColumnModel(list);
		
		cm.addHeaderGroup(0, 0 , new HeaderGroupConfig("", 1, 1));
		cm.addHeaderGroup(0, 1, new HeaderGroupConfig("Professor Real", 1, 2));
		
		return cm;
	}
	
	@Override
	public SimpleModal getSimpleModal()
	{
		return simpleModal;
	}
	
	@Override 
	public ProfessorVirtualDTO getProfessorVirtualDTO()
	{
		return professorVirtualDTO;
	}
	
	@Override
	public ListStore< ConfirmacaoTurmaDTO > getTurmaStore()
	{
		return this.turmaStore;
	}
	
	@Override
	public ListStore< MotivoUsoProfessorVirtualDTO > getMotivosStore()
	{
		return this.motivosStore;
	}
	
	@Override
	public ListStore< DicaEliminacaoProfessorVirtualDTO > getDicasStore()
	{
		return this.dicasStore;
	}
	
	@Override
	public Grid<ConfirmacaoTurmaDTO> getTurmaGrid()
	{
		return this.turmaGrid;
	}
	
	@Override
	public Grid<MotivoUsoProfessorVirtualDTO> getMotivosGrid()
	{
		return this.motivosProfessorGrid;
	}
	
	@Override
	public Grid<DicaEliminacaoProfessorVirtualDTO> getDicasGrid()
	{
		return this.dicasProfessorGrid;
	}
}
