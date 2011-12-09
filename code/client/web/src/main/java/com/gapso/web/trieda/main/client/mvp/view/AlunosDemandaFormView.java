package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.AlunosDemandaFormPresenter;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
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
	private NumberField periodoNF;

	public AlunosDemandaFormView(
		AlunoDemandaDTO alunoDemandaDTO, DemandaDTO demandaDTO )
	{
		this.alunoDemandaDTO = alunoDemandaDTO;
		this.demandaDTO = demandaDTO;
		this.initUI();
	}

	private void initUI()
	{
		String title = "Associar Aluno à Demanda";
		this.simpleModal = new SimpleModal( title,
			Resources.DEFAULTS.disciplina16() );

		this.simpleModal.setHeight( 280 );
		createForm();
		this.simpleModal.setContent( this.formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData( "-20" );
		this.formPanel = new FormPanel();
		this.formPanel.setHeaderVisible( false );

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
		this.alunosComboBox = new AlunosComboBox();
		this.alunosComboBox.setName( "aluno" );
		this.alunosComboBox.setFieldLabel( "Aluno" );
		this.alunosComboBox.setAllowBlank( false );
		this.alunosComboBox.setEmptyText( "Escolha o Aluno" );
		this.formPanel.add( this.alunosComboBox, formData );

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
}
