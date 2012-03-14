package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.DemandaFormPresenter;
import com.gapso.web.trieda.shared.dtos.AbstractDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.shared.util.view.OfertaComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;

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
		simpleModal.setWidth(500);
		createForm();
		simpleModal.setContent( formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData( "-20" );
		this.formPanel = new FormPanel();
		this.formPanel.setHeaderVisible( false );
		
		this.ofertaCB = new OfertaComboBox();
		this.ofertaCB.setAllowBlank( true );
		this.formPanel.add( this.ofertaCB, formData );

		this.campusTF = new TextField<CampusDTO>();
		this.campusTF.setFieldLabel("Campus");
		this.campusTF.setAllowBlank( false );
		this.campusTF.setValue( this.campusDTO );
		this.campusTF.setReadOnly(true);
		this.formPanel.add( this.campusTF, formData );

		this.cursoTF = new TextField<CursoDTO>();
		this.cursoTF.setFieldLabel("Curso");
		this.cursoTF.setAllowBlank( false );
		this.cursoTF.setValue( this.cursoDTO );
		this.cursoTF.setReadOnly(true);
		this.formPanel.add( this.cursoTF, formData );

		this.curriculoTF = new TextField<CurriculoDTO>();
		this.curriculoTF.setFieldLabel("Currículo");
		this.curriculoTF.setAllowBlank( false );
		this.curriculoTF.setValue( this.curriculoDTO );
		this.curriculoTF.setReadOnly(true);
		this.formPanel.add( this.curriculoTF, formData );

		this.turnoTF = new TextField<TurnoDTO>();
		this.turnoTF.setFieldLabel("Turno");
		this.turnoTF.setAllowBlank( false );
		this.turnoTF.setValue( this.turnoDTO );
		this.turnoTF.setReadOnly(true);
		this.formPanel.add( this.turnoTF, formData );

		this.disciplinaCB = new DisciplinaComboBox(ofertaCB){
			@Override
			public void loadByCriteria(AbstractDTO abdto, AsyncCallback<ListLoadResult<DisciplinaDTO>> callback){
				OfertaDTO ofertaDTO = (OfertaDTO) abdto;
				Services.disciplinas().getListByCurriculoIdAndName(ofertaDTO.getMatrizCurricularId(), this.input.getValue(), callback);
			}
		};
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

		this.simpleModal.setFocusWidget( this.ofertaCB );
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
