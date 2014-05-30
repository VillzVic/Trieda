package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.AlunosDemandaFormPresenter;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.AlunosComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class AlunosDemandaFormView
	extends MyComposite
	implements AlunosDemandaFormPresenter.Display
{
	private AlunosComboBox alunosComboBox;
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private AlunoDemandaDTO alunoDemandaDTO;
	private DemandaDTO demandaDTO;
	private NumberField prioridade;
	private NumberField periodoNF;
	private CheckBox exigeEquivalenciaForcadaCB;
	private CenarioDTO cenarioDTO;

	public AlunosDemandaFormView( CenarioDTO cenarioDTO,
		AlunoDemandaDTO alunoDemandaDTO, DemandaDTO demandaDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.alunoDemandaDTO = alunoDemandaDTO;
		this.demandaDTO = demandaDTO;
		this.initUI();
	}

	private void initUI()
	{
		String title = "Associar Aluno à Demanda";
		this.simpleModal = new SimpleModal( title,
			Resources.DEFAULTS.disciplina16() );

		this.simpleModal.setHeight( 310 );
		this.simpleModal.setWidth( 330 );
		createForm();
		this.simpleModal.setContent( this.formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData( "-20" );
		this.formPanel = new FormPanel();
		this.formPanel.setHeaderVisible( false );
		this.formPanel.setLabelWidth(100);

		// Campus
		TextField< String > campusTF = new TextField< String >();
		campusTF.setValue( this.demandaDTO.getCampusString() );
		campusTF.setFieldLabel( "Campus" );
		campusTF.setReadOnly( true );
		this.formPanel.add( campusTF, formData );

		// Curriculo
		TextField< String > curriculoTF = new TextField< String >();
		curriculoTF.setValue( this.demandaDTO.getCurriculoString() );
		curriculoTF.setFieldLabel( "Matriz Curricular" );
		curriculoTF.setReadOnly( true );
		this.formPanel.add( curriculoTF, formData );

		// Turno
		TextField< String > turnoTF = new TextField< String >();
		turnoTF.setValue( this.demandaDTO.getTurnoString() );
		turnoTF.setFieldLabel( "Turno" );
		turnoTF.setReadOnly( true );
		this.formPanel.add( turnoTF, formData );

		// Disciplina
		TextField< String > disciplinaTF = new TextField< String >();
		disciplinaTF.setValue( this.demandaDTO.getDisciplinaString() );
		disciplinaTF.setFieldLabel( "Disciplina" );
		disciplinaTF.setReadOnly( true );
		this.formPanel.add( disciplinaTF, formData );

		// Período
		this.periodoNF = new NumberField();
		this.periodoNF.setName( DemandaDTO.PROPERTY_PERIODO );
		this.periodoNF.setValue( this.demandaDTO.getPeriodo() );
		this.periodoNF.setFieldLabel( "Período" );
		this.periodoNF.setAllowBlank( false );
		this.periodoNF.setAllowDecimals( false );
		this.periodoNF.setMaxValue( 999999 );
		this.periodoNF.setEmptyText( "Período" );
		this.formPanel.add( this.periodoNF, formData );
		
		// Aluno
		this.alunosComboBox = new AlunosComboBox(cenarioDTO);
		this.alunosComboBox.setName( "aluno" );
		this.alunosComboBox.setFieldLabel( "Aluno" );
		this.alunosComboBox.setAllowBlank( false );
		this.alunosComboBox.setEmptyText( "Escolha o Aluno" );
		this.formPanel.add( this.alunosComboBox, formData );
		
		// prioridade
		prioridade = new NumberField();
		prioridade.setFieldLabel("Prioridade");
		prioridade.setName(AlunoDemandaDTO.PROPERTY_ALUNO_PRIORIDADE);
		prioridade.setValue(alunoDemandaDTO.getAlunoPrioridade());
		prioridade.setAllowBlank(false);
		prioridade.setAllowDecimals(false);
		prioridade.setMaxValue(10);
		prioridade.setEmptyText("Prioridade");
		this.formPanel.add(prioridade, formData);
		
		exigeEquivalenciaForcadaCB = new CheckBox();
		exigeEquivalenciaForcadaCB.setName(AlunoDemandaDTO.PROPERTY_EXIGE_EQUIVALENCIA_FORCADA);
		exigeEquivalenciaForcadaCB.setValue(alunoDemandaDTO.getExigeEquivalenciaForcada());
		exigeEquivalenciaForcadaCB.setFieldLabel("Exige Equivalência Forçada?");
		formPanel.add( exigeEquivalenciaForcadaCB, formData );

		FormButtonBinding binding = new FormButtonBinding( this.formPanel );
		binding.addButton( this.simpleModal.getSalvarBt() );

		this.simpleModal.setFocusWidget( this.alunosComboBox );
	}

	public boolean isValid()
	{
		return this.formPanel.isValid();
	}

	@Override
	public DemandaDTO getDemandaDTO()
	{
		return this.demandaDTO;
	}

	@Override
	public Button getSalvarButton()
	{
		return this.simpleModal.getSalvarBt();
	}

	@Override
	public AlunoDemandaDTO getAlunoDemandaDTO()
	{
		return this.alunoDemandaDTO;
	}

	@Override
	public SimpleModal getSimpleModal()
	{
		return this.simpleModal;
	}

	@Override
	public AlunosComboBox getAlunoComboBox()
	{
		return this.alunosComboBox;
	}

	@Override
	public NumberField getPeriodoNumberField()
	{
		return this.periodoNF;
	}
	
	@Override
	public NumberField getPrioridadeField(){
		return this.prioridade;
	}
	
	@Override
	public CheckBox getExigeEquivalenciaForcadaCheckBox()
	{
		return this.exigeEquivalenciaForcadaCB;
	}
}
