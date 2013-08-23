package com.gapso.web.trieda.shared.mvp.view;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.shared.dtos.AbstractDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.ProfessorDisciplinaFormPresenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class ProfessorDisciplinaFormView extends MyComposite implements ProfessorDisciplinaFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	
	private ProfessorComboBox professorCB;
	private DisciplinaComboBox disciplinaCB;
	private NumberField preferenciaNF;
	private NumberField notaDesempenhoNF;

	private UsuarioDTO usuario;
	private ProfessorDisciplinaDTO professorDisciplinaDTO;
	private ProfessorDTO professorDTO;
	private DisciplinaDTO disciplinaDTO;
	private CenarioDTO cenarioDTO;
	
	public ProfessorDisciplinaFormView( CenarioDTO cenarioDTO, 
		UsuarioDTO usuario,	ProfessorDisciplinaDTO professorDisciplinaDTO,
		ProfessorDTO professorDTO, DisciplinaDTO disciplinaDTO )
	{
		this.usuario = usuario;
		this.professorDisciplinaDTO = professorDisciplinaDTO;
		this.professorDTO = professorDTO;
		this.disciplinaDTO = disciplinaDTO;
		this.cenarioDTO = cenarioDTO;
		initUI();
	}

	private void initUI()
	{
		String title = ( ( professorDisciplinaDTO.getId() == null ) ?
			"Edição de Habilitação" :
			"Cadastro de Habilitação do Professore" );

		simpleModal = new SimpleModal( title, Resources.DEFAULTS.professor16() );
		simpleModal.setHeight( 200 );
		createForm();
		simpleModal.setContent( formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);

		professorCB = new ProfessorComboBox(cenarioDTO, usuario.isProfessor());
		professorCB.setAllowBlank(false);
		professorCB.setValue(professorDTO);
		formPanel.add(professorCB, formData);

		disciplinaCB = new DisciplinaComboBox(cenarioDTO, professorCB){
			@Override
			public void loadByCriteria(AbstractDTO abdto, AsyncCallback<ListLoadResult<DisciplinaDTO>> callback){
				ProfessorDTO professorDTO = (ProfessorDTO) abdto;
				Services.disciplinas().getDisciplinaNaoAssociada(cenarioDTO, professorDTO, this.input.getValue() , callback);
			}
		};
		disciplinaCB.setAllowBlank(false);
		disciplinaCB.setValue(disciplinaDTO);
		formPanel.add(disciplinaCB, formData);

		preferenciaNF = new NumberField();
		preferenciaNF.setName(ProfessorDisciplinaDTO.PROPERTY_PREFERENCIA);
		preferenciaNF.setValue(professorDisciplinaDTO.getPreferencia());
		preferenciaNF.setFieldLabel("Preferência");
		preferenciaNF.setAllowBlank(false);
		preferenciaNF.setAllowNegative(false);
		preferenciaNF.setAllowDecimals(false);
		preferenciaNF.setMaxValue(10);
		preferenciaNF.setEmptyText("Preencha de 0 à 10");
		formPanel.add(preferenciaNF, formData);

		notaDesempenhoNF = new NumberField();
		notaDesempenhoNF.setName(ProfessorDisciplinaDTO.PROPERTY_NOTA_DESEMPENHO);
		notaDesempenhoNF.setValue(professorDisciplinaDTO.getNotaDesempenho());
		notaDesempenhoNF.setFieldLabel("Nota de Desempenho");
		notaDesempenhoNF.setAllowBlank(false);
		notaDesempenhoNF.setAllowNegative(false);
		notaDesempenhoNF.setAllowDecimals(false);
		notaDesempenhoNF.setMaxValue(10);
		notaDesempenhoNF.setEmptyText("Preencha de 0 à 10");
		notaDesempenhoNF.setReadOnly(usuario.isProfessor());
		formPanel.add(notaDesempenhoNF, formData);

		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());

		simpleModal.setFocusWidget(professorCB);
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
	public ProfessorComboBox getProfessorComboBox()
	{
		return professorCB;
	}

	@Override
	public DisciplinaComboBox getDisciplinaComboBox()
	{
		return disciplinaCB;
	}

	@Override
	public NumberField getPreferenciaNumberField()
	{
		return preferenciaNF;
	}

	@Override
	public NumberField getNotaDesempenhoNumberField()
	{
		return notaDesempenhoNF;
	}

	@Override
	public ProfessorDisciplinaDTO getProfessorDisciplinaDTO()
	{
		return professorDisciplinaDTO;
	}
}
