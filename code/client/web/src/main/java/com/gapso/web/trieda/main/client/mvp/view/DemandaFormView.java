package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.DemandaFormPresenter;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;

public class DemandaFormView
	extends MyComposite
	implements DemandaFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private CampusComboBox campusCB;
	private CursoComboBox cursoCB;
	private CurriculoComboBox curriculoCB;
	private TurnoComboBox turnoCB;
	private DisciplinaComboBox disciplinaCB;
	private NumberField demandaTF;
	private DemandaDTO demandaDTO;
	private CampusDTO campusDTO;
	private CursoDTO cursoDTO;
	private CurriculoDTO curriculoDTO;
	private TurnoDTO turnoDTO;
	private DisciplinaDTO disciplinaDTO;

	public DemandaFormView( DemandaDTO demandaDTO,
		CampusDTO campusDTO, CursoDTO cursoDTO, CurriculoDTO curriculoDTO,
		TurnoDTO turnoDTO, DisciplinaDTO disciplinaDTO )
	{
		this.demandaDTO = demandaDTO;
		this.campusDTO = campusDTO;
		this.cursoDTO = cursoDTO;
		this.curriculoDTO = curriculoDTO;
		this.turnoDTO = turnoDTO;
		this.disciplinaDTO = disciplinaDTO;

		initUI();
	}

	private void initUI()
	{
		String title = ( ( demandaDTO.getId() == null ) ?
			"Inserção de Demanda" : "Edição de Demanda" );

		simpleModal = new SimpleModal(
			title, Resources.DEFAULTS.demanda16() );

		simpleModal.setHeight( 280 );
		createForm();
		simpleModal.setContent( formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData( "-20" );
		this.formPanel = new FormPanel();
		this.formPanel.setHeaderVisible( false );

		this.campusCB = new CampusComboBox();
		this.campusCB.setAllowBlank( false );
		this.campusCB.setValue( this.campusDTO );
		this.campusCB.setEmptyText( "Selecione o campus" );
		this.formPanel.add( this.campusCB, formData );

		this.cursoCB = new CursoComboBox();
		this.cursoCB.setAllowBlank( false );
		this.cursoCB.setValue( this.cursoDTO );
		this.cursoCB.setEmptyText( "Selecione o curso" );
		this.formPanel.add( this.cursoCB, formData );

		this.curriculoCB = new CurriculoComboBox();
		this.curriculoCB.setAllowBlank( false );
		this.curriculoCB.setValue( this.curriculoDTO );
		this.curriculoCB.setEmptyText( "Selecione o curriculo" );
		this.formPanel.add( this.curriculoCB, formData );

		this.turnoCB = new TurnoComboBox();
		this.turnoCB.setAllowBlank( false );
		this.turnoCB.setValue( this.turnoDTO );
		this.turnoCB.setEmptyText( "Selecione o turno" );
		this.formPanel.add( this.turnoCB, formData );

		this.disciplinaCB = new DisciplinaComboBox();
		this.disciplinaCB.setAllowBlank( false );
		this.disciplinaCB.setValue( this.disciplinaDTO );
		this.disciplinaCB.setEmptyText( "Selecione a disciplina" );
		this.formPanel.add( this.disciplinaCB, formData );

		this.demandaTF = new NumberField();
		this.demandaTF.setValue( this.demandaDTO.getDemanda() );
		this.demandaTF.setFieldLabel( "Quantidade" );
		this.demandaTF.setAllowBlank( false );
		this.demandaTF.setAllowNegative( false );
		this.demandaTF.setAllowDecimals( false );
		this.demandaTF.setEmptyText( "Selecione a quantidade" );
		this.demandaTF.setMinValue(1);

		if ( this.demandaDTO == null
			|| this.demandaDTO.getQuantidadeDemandaEnable() == null
			|| this.demandaDTO.getQuantidadeDemandaEnable() == true )
		{
			this.demandaTF.setEnabled( true );
		}
		else
		{
			this.demandaTF.setEnabled( false );
		}

		this.formPanel.add( this.demandaTF, formData );

		FormButtonBinding binding = new FormButtonBinding( this.formPanel );
		binding.addButton( this.simpleModal.getSalvarBt() );

		this.simpleModal.setFocusWidget( this.campusCB );
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
	public CampusComboBox getCampusComboBox()
	{
		return campusCB;
	}

	@Override
	public CursoComboBox getCursoComboBox()
	{
		return cursoCB;
	}

	@Override
	public CurriculoComboBox getCurriculoComboBox()
	{
		return curriculoCB;
	}

	@Override
	public TurnoComboBox getTurnoComboBox()
	{
		return turnoCB;
	}

	@Override
	public DisciplinaComboBox getDisciplinaComboBox()
	{
		return disciplinaCB;
	}

	@Override
	public NumberField getDemandaTextField()
	{
		return demandaTF;
	}
}
