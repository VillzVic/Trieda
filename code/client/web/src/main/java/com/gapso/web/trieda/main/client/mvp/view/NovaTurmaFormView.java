package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.LabelField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.NovaTurmaFormPresenter;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class NovaTurmaFormView extends MyComposite
	implements NovaTurmaFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField< String > nomeTF;
	private TurmaDTO turmaDTO;
	private CenarioDTO cenarioDTO;
	private CampusDTO campusDTO;
	private DisciplinaDTO disciplinaDTO;
	private boolean edit;
	
	public NovaTurmaFormView( CenarioDTO cenarioDTO, CampusDTO campusDTO,
			DisciplinaDTO disciplinaDTO, TurmaDTO turmaDTO, boolean edit )
	{
		this.turmaDTO = turmaDTO;
		this.campusDTO = campusDTO;
		this.cenarioDTO = cenarioDTO;
		this.disciplinaDTO = disciplinaDTO;
		this.edit = edit;
		this.initUI();
	}
	
	private void initUI()
	{
		String title = ( cenarioDTO.getNome() + " » " + (!edit ?
			"Inserção de Turma" : "Edição de Turma") );
	
		this.simpleModal = new SimpleModal(
			title, Resources.DEFAULTS.turmaAdd16() );
		this.simpleModal.setHeight( 180 );
		this.simpleModal.setWidth( 400 );
		createForm();
	
		this.simpleModal.setContent( this.formPanel );
	}
	
	private void createForm()
	{
		FormData formData = new FormData( "-20" );
		this.formPanel = new FormPanel();
		this.formPanel.setHeaderVisible( false );
		
		LabelField campus = new LabelField();
		campus.setFieldLabel("Campus");
		campus.setValue(campusDTO.getNome() + " (" + campusDTO.getCodigo() + ")");
		this.formPanel.add( campus, formData );
		
		LabelField disciplina = new LabelField();
		disciplina.setFieldLabel("Disciplina");
		disciplina.setValue(disciplinaDTO.getNome() + " (" + disciplinaDTO.getCodigo() + ")");
		this.formPanel.add( disciplina, formData );
		
		this.nomeTF = new TextField< String >();
		this.nomeTF.setName( TurmaDTO.PROPERTY_NOME );
		this.nomeTF.setValue( this.turmaDTO.getNome() );
		this.nomeTF.setFieldLabel( "Nome da Turma" );
		this.nomeTF.setAllowBlank( false );
		this.nomeTF.setMinLength( 1 );
		this.nomeTF.setMaxLength( 50 );
		this.nomeTF.setEmptyText( "Preencha o nome" );
		this.formPanel.add( this.nomeTF, formData );
	
		FormButtonBinding binding = new FormButtonBinding( this.formPanel );
		binding.addButton( this.simpleModal.getSalvarBt() );
	
		this.simpleModal.setFocusWidget( this.nomeTF );
	}
	
	public boolean isValid()
	{
		return this.formPanel.isValid();
	}
	
	@Override
	public boolean getEdit()
	{
		return edit;
	}
	
	@Override
	public Button getSalvarButton()
	{
		return this.simpleModal.getSalvarBt();
	}
	
	@Override
	public TextField< String > getNomeTextField()
	{
		return this.nomeTF;
	}
	
	@Override
	public SimpleModal getSimpleModal()
	{
		return this.simpleModal;
	}
	
	@Override
	public TurmaDTO getTurmaDTO()
	{
		return this.turmaDTO;
	}
	
	@Override
	public DisciplinaDTO getDisciplinaDTO()
	{
		return this.disciplinaDTO;
	}
	
	@Override
	public CampusDTO getCampusDTO()
	{
		return this.campusDTO;
	}
}