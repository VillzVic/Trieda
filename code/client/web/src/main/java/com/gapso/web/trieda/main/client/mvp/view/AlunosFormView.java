package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.AlunosFormPresenter;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.UniqueDomain;
import com.gapso.web.trieda.shared.util.view.UniqueTextField;

public class AlunosFormView
	extends MyComposite
	implements AlunosFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private UniqueTextField matriculaTF;
	private TextField< String > nomeTF;
	private CheckBox formandoCB;
	private AlunoDTO alunoDTO;
	private CenarioDTO cenarioDTO;

	public AlunosFormView(
		CenarioDTO cenarioDTO, AlunoDTO alunoDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.alunoDTO = alunoDTO;

		initUI();
	}
	
	private void initUI()
	{
		String title = ( ( alunoDTO.getId() == null ) ?
			"Inserção de Aluno" : "Edição de Aluno" );

		simpleModal = new SimpleModal(
			title, Resources.DEFAULTS.professor16() );

		simpleModal.setHeight( 160 );
		createForm();
		simpleModal.setContent( formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible( false );

		nomeTF = new TextField< String >();
		nomeTF.setName( AlunoDTO.PROPERTY_ALUNO_NOME );
		nomeTF.setValue( alunoDTO.getNome() );
		nomeTF.setFieldLabel( "Nome" );
		nomeTF.setAllowBlank( false );
		nomeTF.setMinLength( 1 );
		nomeTF.setMaxLength( 50 );
		nomeTF.setEmptyText( "Preencha o nome" );
		formPanel.add( nomeTF, formData );

		matriculaTF = new UniqueTextField( cenarioDTO, UniqueDomain.ALUNO );
		matriculaTF.setName( AlunoDTO.PROPERTY_ALUNO_MATRICULA );
		matriculaTF.setValue( alunoDTO.getMatricula() );
		matriculaTF.setFieldLabel( "Matrícula" );
		matriculaTF.setAllowBlank( false );
		matriculaTF.setMinLength( 1 );
		matriculaTF.setMaxLength( 50 );
		matriculaTF.setEmptyText( "Preencha a matrícula" );
		formPanel.add( matriculaTF, formData );
		
		formandoCB = new CheckBox();
		formandoCB.setName(AlunoDTO.PROPERTY_ALUNO_FORMANDO);
		formandoCB.setValue(alunoDTO.getFormando());
		formandoCB.setFieldLabel("Formando?");
		formPanel.add( formandoCB, formData );

		FormButtonBinding binding = new FormButtonBinding( formPanel );
		binding.addButton( simpleModal.getSalvarBt() );

		simpleModal.setFocusWidget( nomeTF );
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
	public TextField< String > getNomeTextField()
	{
		return nomeTF;
	}

	@Override
	public SimpleModal getSimpleModal()
	{
		return simpleModal;
	}

	@Override
	public AlunoDTO getAlunoDTO()
	{
		return alunoDTO;
	}

	@Override
	public UniqueTextField getMatriculaTextField()
	{
		return matriculaTF;
	}
	
	@Override
	public CheckBox getFormandoCheckBox() {
		return formandoCB;
	}
}
