package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FieldSet;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.CellEditor;
import com.extjs.gxt.ui.client.widget.grid.CheckColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.EditorGrid;
import com.extjs.gxt.ui.client.widget.grid.HeaderGroupConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.DemandaFormPresenter;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDemandaDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.DisciplinaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.OfertaComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class DemandaFormView
	extends MyComposite
	implements DemandaFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private OfertaComboBox ofertaCB;
	private TextField<CampusDTO> campusTF;
	private TextField<CursoDTO> cursoTF;
	private TextField<CurriculoDTO> curriculoTF;
	private TextField<TurnoDTO> turnoTF;
	private DisciplinaAutoCompleteBox disciplinaCB;
	private SimpleComboBox<Integer> periodoCB;
	private NumberField demandaTF;
	private DemandaDTO demandaDTO;
	private CampusDTO campusDTO;
	private CursoDTO cursoDTO;
	private CurriculoDTO curriculoDTO;
	private TurnoDTO turnoDTO;
	private OfertaDTO ofertaDTO;
	private CenarioDTO cenarioDTO;
	private NumberField preencheTudoNF;
	private Button preencheTudoBT;
	private Button marcaEquivalenciasForcadasBt;
	private CheckColumnConfig checkColumn;
	private NumberField prioridadeAlunosNF;
	private ListStore< DisciplinaDemandaDTO > store = new ListStore< DisciplinaDemandaDTO >();
	
	private EditorGrid<DisciplinaDemandaDTO> grid;

	public DemandaFormView( CenarioDTO cenarioDTO, DemandaDTO demandaDTO,
		CampusDTO campusDTO, CursoDTO cursoDTO, CurriculoDTO curriculoDTO,
		TurnoDTO turnoDTO, OfertaDTO ofertaDTO )
	{
		this.demandaDTO = demandaDTO;
		this.campusDTO = campusDTO;
		this.cursoDTO = cursoDTO;
		this.curriculoDTO = curriculoDTO;
		this.turnoDTO = turnoDTO;
		this.ofertaDTO = ofertaDTO;
		this.cenarioDTO = cenarioDTO;

		initUI();
	}

	private void initUI()
	{
		String title = ( ( demandaDTO.getId() == null ) ?
			"Inserção de Demanda" : "Edição de Demanda" );

		simpleModal = new SimpleModal(
			title, Resources.DEFAULTS.demanda16() );

		simpleModal.setHeight( 565 );
		simpleModal.setWidth(600);
		createForm();
		simpleModal.setContent( formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData( "-20" );
		this.formPanel = new FormPanel();
		this.formPanel.setHeaderVisible( false );
		
		FieldSet ofertaFS = new FieldSet();
		ofertaFS.setHeadingHtml("Definição da oferta e Período curricular");
		ofertaFS.setCollapsible(true);
		ofertaFS.setLayout(new FormLayout());
		
		this.ofertaCB = new OfertaComboBox(cenarioDTO);
		ofertaCB.setAllowBlank( true );
		if (ofertaDTO != null)
		{
			ofertaCB.setValue(ofertaDTO);
		}
		ofertaFS.add( this.ofertaCB, formData );

		this.turnoTF = new TextField<TurnoDTO>();
		this.turnoTF.setFieldLabel("Turno");
		this.turnoTF.setAllowBlank( false );
		this.turnoTF.setValue( this.turnoDTO );
		this.turnoTF.setReadOnly(true);
		ofertaFS.add( this.turnoTF, formData );
		
		this.campusTF = new TextField<CampusDTO>();
		this.campusTF.setFieldLabel("Campus");
		this.campusTF.setAllowBlank( false );
		this.campusTF.setValue( this.campusDTO );
		this.campusTF.setReadOnly(true);
		ofertaFS.add( this.campusTF, formData );

		this.cursoTF = new TextField<CursoDTO>();
		this.cursoTF.setFieldLabel("Curso");
		this.cursoTF.setAllowBlank( false );
		this.cursoTF.setValue( this.cursoDTO );
		this.cursoTF.setReadOnly(true);
		ofertaFS.add( this.cursoTF, formData );

		this.curriculoTF = new TextField<CurriculoDTO>();
		this.curriculoTF.setFieldLabel("Martiz Curricular");
		this.curriculoTF.setAllowBlank( false );
		this.curriculoTF.setValue( this.curriculoDTO );
		this.curriculoTF.setReadOnly(true);
		ofertaFS.add( this.curriculoTF, formData );
		
		periodoCB = new SimpleComboBox<Integer>();
		periodoCB.setFieldLabel("Período");
		if (demandaDTO.getId() != null)
		{
			periodoCB.add(demandaDTO.getPeriodo());
		}
		else
		{
			periodoCB.disable();
		}
		ofertaFS.add( periodoCB, formData );
		
		prioridadeAlunosNF = new NumberField();
		prioridadeAlunosNF.setFieldLabel("Prioridade dos Alunos");
		prioridadeAlunosNF.setAllowBlank(false);
		prioridadeAlunosNF.setAllowDecimals(false);
		prioridadeAlunosNF.setValue(1);
		ofertaFS.add( prioridadeAlunosNF, formData );
		
		this.formPanel.add( ofertaFS, formData );
		
		FieldSet quantidadeAlunosFS = new FieldSet();
		quantidadeAlunosFS.setHeadingHtml("Definição da Quantidade de Alunos Virtuais por Disciplina");
		quantidadeAlunosFS.setCollapsible(true);
		quantidadeAlunosFS.setLayout(new HBoxLayout());
		
		createGrid();
		
		LayoutContainer container = new LayoutContainer();

		preencheTudoNF = new NumberField();
		preencheTudoNF.setWidth(50);
		preencheTudoNF.setStyleAttribute("left", "28px");
		preencheTudoNF.setAllowDecimals(false);
		
		preencheTudoBT = new Button();
		preencheTudoBT.setText("Preenche Tudo");
		preencheTudoBT.setStyleAttribute("margin-left", "9px");
		
		marcaEquivalenciasForcadasBt = new Button("Marcar Todas<br />Equivalencias");
		marcaEquivalenciasForcadasBt.setStyleAttribute("margin-left", "9px");
		marcaEquivalenciasForcadasBt.setStyleAttribute("margin-top", "10px");
		marcaEquivalenciasForcadasBt.setWidth(85);
		marcaEquivalenciasForcadasBt.setHeight(40);
		marcaEquivalenciasForcadasBt.hide();
		
		container.add(preencheTudoNF);
		container.add(preencheTudoBT);
		container.add(marcaEquivalenciasForcadasBt);
		
		quantidadeAlunosFS.add(grid);
		quantidadeAlunosFS.add(container);
		
		this.formPanel.add( quantidadeAlunosFS, formData );

		FormButtonBinding binding = new FormButtonBinding( this.formPanel );
		this.simpleModal.getSalvarBt().setText("Criar Demandas e Alunos Virtuais");
		binding.addButton( this.simpleModal.getSalvarBt() );

		this.simpleModal.setFocusWidget( this.ofertaCB );
	}
	
	private void createGrid()
	{
		BorderLayoutData bld
			= new BorderLayoutData( LayoutRegion.CENTER );

	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );
	    
	    this.grid = new EditorGrid<DisciplinaDemandaDTO>( this.getStore(),  getColumnList() );
	    grid.addPlugin(checkColumn);
	    grid.setHeight(200);
	    grid.setBorders(true);
	    grid.setWidth(430);
	}
	
	private ColumnModel getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( DisciplinaDemandaDTO.PROPERTY_DISCIPLINA, "Disciplina", 70 ) );
		list.add( new ColumnConfig( DisciplinaDemandaDTO.PROPERTY_DEMANDA_REAL, "Real", 60 ) );
		list.add( new ColumnConfig( DisciplinaDemandaDTO.PROPERTY_DEMANDA_VIRTUAL, "Virtual", 60 ) );
		list.add( new ColumnConfig( DisciplinaDemandaDTO.PROPERTY_DEMANDA_TOTAL, "Total", 60 ) );
		
		ColumnConfig novaDemanda = new ColumnConfig( DisciplinaDemandaDTO.PROPERTY_NOVA_DEMANDA, "Nova Demanda de Alunos Virtuais", 175 );
		NumberField fieldNumber2 = new NumberField();
		fieldNumber2.setPropertyEditorType(Integer.class);
		fieldNumber2.setAllowDecimals(false);
		novaDemanda.setEditor(new CellEditor(fieldNumber2));
		
		list.add( novaDemanda );
		checkColumn = new CheckColumnConfig(
				DisciplinaDemandaDTO.PROPERTY_EXIGE_EQUIVALENCIA_FORCADA, "Exige Equivalência Forçada", 150 );
		CellEditor checkBoxEditor = new CellEditor(new CheckBox());        
		checkColumn.setEditor(checkBoxEditor);
		list.add( checkColumn );

		ColumnModel cm = new ColumnModel(list);
		
		cm.addHeaderGroup(0, 0 , new HeaderGroupConfig("", 1, 1));
		cm.addHeaderGroup(0, 1, new HeaderGroupConfig("Demanda Existente de Alunos", 1, 3));
		cm.addHeaderGroup(0, 4, new HeaderGroupConfig("", 1, 2));
		
		return cm;
	}


	public boolean isValid()
	{
		return formPanel.isValid();
	}

	@Override
	public Button getSalvarButton()
	{
		return simpleModal.getSalvarBt();
	}

	@Override
	public SimpleModal getSimpleModal()
	{
		return simpleModal;
	}

	@Override
	public DemandaDTO getDemandaDTO()
	{
		return demandaDTO;
	}
	
	@Override
	public OfertaComboBox getOfertaComboBox() {
		return ofertaCB;
	}

	@Override
	public TextField<CampusDTO> getCampusTextField()
	{
		return campusTF;
	}

	@Override
	public TextField<CursoDTO> getCursoTextField()
	{
		return cursoTF;
	}

	@Override
	public TextField<CurriculoDTO> getCurriculoTextField()
	{
		return curriculoTF;
	}

	@Override
	public TextField<TurnoDTO> getTurnoTextField()
	{
		return turnoTF;
	}

	@Override
	public DisciplinaAutoCompleteBox getDisciplinaComboBox()
	{
		return disciplinaCB;
	}

	@Override
	public NumberField getDemandaTextField()
	{
		return demandaTF;
	}
	
	@Override
	public SimpleComboBox<Integer> getPeriodoComboBox()
	{
		return periodoCB;
	}
	
	@Override
	public ListStore< DisciplinaDemandaDTO > getStore()
	{
		return this.store;
	}
	
	@Override
	public EditorGrid<DisciplinaDemandaDTO> getGrid()
	{
		return this.grid;
	}
	
	@Override
	public Button getPreencheTudoButton()
	{
		return preencheTudoBT;
	}
	
	@Override
	public Button getMarcarEquivalenciasForcadasButton()
	{
		return marcaEquivalenciasForcadasBt;
	}
	
	@Override
	public NumberField getPreencheTudoNumberField()
	{
		return preencheTudoNF;
	}
	
	@Override
	public NumberField getPrioridadeAlunosNumberField()
	{
		return prioridadeAlunosNF;
	}
}
