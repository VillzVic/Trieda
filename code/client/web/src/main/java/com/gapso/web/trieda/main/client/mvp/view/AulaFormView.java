package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.LabelField;
import com.extjs.gxt.ui.client.widget.form.Radio;
import com.extjs.gxt.ui.client.widget.form.RadioGroup;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.AulaFormPresenter;
import com.gapso.web.trieda.shared.dtos.AulaDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.HorarioComboBox;
import com.gapso.web.trieda.shared.util.view.SalaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.SemanaLetivaComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class AulaFormView extends MyComposite
implements AulaFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TurmaDTO turmaDTO;
	private CenarioDTO cenarioDTO;
	private CampusDTO campusDTO;
	private DisciplinaDTO disciplinaDTO;
	private AulaDTO aulaDTO;
	private SalaDTO salaDTO;
	private SemanaLetivaDTO semanaLetivaDTO;
	private HorarioDisponivelCenarioDTO horarioDisponivelCenarioDTO;
	private SalaAutoCompleteBox salaCB;
	private SemanaLetivaComboBox semanaLetivaCB;
	private HorarioComboBox horarioCB;
	private SimpleComboBox<Integer> qtdeCreditosCB;
	private RadioGroup diaSemanaRadioGroup;
	private RadioGroup tipoCreditoRadioGroup;
	private String diaSemanaSelecionada;
	
	
	public AulaFormView( CenarioDTO cenarioDTO, CampusDTO campusDTO,
			DisciplinaDTO disciplinaDTO, SalaDTO salaDTO, 
			SemanaLetivaDTO semanaLetivaDTO,
			HorarioDisponivelCenarioDTO horarioDisponivelCenarioDTO,
			TurmaDTO turmaDTO, AulaDTO aulaDTO )
	{
		this.turmaDTO = turmaDTO;
		this.campusDTO = campusDTO;
		this.cenarioDTO = cenarioDTO;
		this.disciplinaDTO = disciplinaDTO;
		this.aulaDTO = aulaDTO;
		this.salaDTO = salaDTO;
		this.semanaLetivaDTO = semanaLetivaDTO;
		this.horarioDisponivelCenarioDTO = horarioDisponivelCenarioDTO;
		this.initUI();
	}
	
	private void initUI()
	{
		String title = ( cenarioDTO.getNome() + " » " + (( this.aulaDTO.getId() == null ) ?
			"Inserção de Aula" : "Edição de Aula") + " (" + turmaDTO.getNome() + " (" + disciplinaDTO.getCodigo() + "))" );
	
		this.simpleModal = new SimpleModal(
			title, Resources.DEFAULTS.novaTurma16() );
		this.simpleModal.setHeight( 380 );
		this.simpleModal.setWidth( 520 );
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
		
		salaCB = new SalaAutoCompleteBox(cenarioDTO);
		salaCB.setValue(salaDTO);
		salaCB.setAllowBlank(false);
		this.formPanel.add( salaCB, formData );
		
		diaSemanaRadioGroup = new RadioGroup();
		diaSemanaRadioGroup.setFieldLabel("Dia da Semana");
		Radio seg = new Radio();  
		seg.setBoxLabel("SEG");
		seg.setValue(seg.getBoxLabel().equals(aulaDTO.getSemanaString()));
		diaSemanaRadioGroup.add(seg);
		Radio ter = new Radio();  
		ter.setBoxLabel("TER");
		ter.setValue(ter.getBoxLabel().equals(aulaDTO.getSemanaString()));
		diaSemanaRadioGroup.add(ter);
		Radio qua = new Radio();  
		qua.setBoxLabel("QUA");
		qua.setValue(qua.getBoxLabel().equals(aulaDTO.getSemanaString()));
		diaSemanaRadioGroup.add(qua);
		Radio qui = new Radio();  
		qui.setBoxLabel("QUI");
		qui.setValue(qui.getBoxLabel().equals(aulaDTO.getSemanaString()));
		diaSemanaRadioGroup.add(qui);
		Radio sex = new Radio();  
		sex.setBoxLabel("SEX");
		sex.setValue(sex.getBoxLabel().equals(aulaDTO.getSemanaString()));
		diaSemanaRadioGroup.add(sex);
		Radio sab = new Radio();  
		sab.setBoxLabel("SAB");
		sab.setValue(sab.getBoxLabel().equals(aulaDTO.getSemanaString()));
		diaSemanaRadioGroup.add(sab);
		Radio dom = new Radio();  
		dom.setBoxLabel("DOM");
		dom.setValue(dom.getBoxLabel().equals(aulaDTO.getSemanaString()));
		diaSemanaRadioGroup.add(dom);
		this.formPanel.add( diaSemanaRadioGroup, formData );
		
		semanaLetivaCB = new SemanaLetivaComboBox(cenarioDTO);
		semanaLetivaCB.setValue(semanaLetivaDTO);
		semanaLetivaCB.setAllowBlank(false);
		this.formPanel.add( semanaLetivaCB, formData );
		
		horarioCB = new HorarioComboBox(cenarioDTO, salaDTO, disciplinaDTO, semanaLetivaDTO,
				diaSemanaRadioGroup.getValue() == null ? null : diaSemanaRadioGroup.getValue().getFieldLabel());
		horarioCB.setValue(horarioDisponivelCenarioDTO);
		horarioCB.setAllowBlank(false);
		if (aulaDTO.getId() == null)
		{
			horarioCB.disable();
		}
		this.formPanel.add( horarioCB, formData );
		
		tipoCreditoRadioGroup = new RadioGroup();
		tipoCreditoRadioGroup.setFieldLabel("Tipo de Crédito");
		Radio teorico = new Radio();  
		teorico.setBoxLabel("Teórico");
		teorico.setValue(false);
		tipoCreditoRadioGroup.add(teorico);
		Radio pratico = new Radio();  
		pratico.setBoxLabel("Prático");
		pratico.setValue(false);
		tipoCreditoRadioGroup.add(pratico);
		this.formPanel.add( tipoCreditoRadioGroup, formData );
		
		qtdeCreditosCB = new SimpleComboBox<Integer>();
		qtdeCreditosCB.setFieldLabel("Quantidade de Créditos");
		qtdeCreditosCB.setSimpleValue(aulaDTO.getCreditosPraticos());
		qtdeCreditosCB.setEditable(false);
		qtdeCreditosCB.setAllowBlank(false);
		if (aulaDTO.getId() == null)
		{
			qtdeCreditosCB.disable();
		}
		this.formPanel.add( qtdeCreditosCB, formData );
		
		FormButtonBinding binding = new FormButtonBinding( this.formPanel );
		binding.addButton( this.simpleModal.getSalvarBt() );
	
		this.simpleModal.setFocusWidget( this.salaCB );
	}
	
	public boolean isValid()
	{
		return this.formPanel.isValid();
	}
	
	@Override
	public Button getSalvarButton()
	{
		return this.simpleModal.getSalvarBt();
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
	public AulaDTO getAulaDTO()
	{
		return this.aulaDTO;
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
	
	@Override
	public SalaAutoCompleteBox getSalaComboBox()
	{
		return this.salaCB;
	}
	
	@Override
	public RadioGroup getDiaSemanaRadioGroup()
	{
		return diaSemanaRadioGroup;
	}
	
	@Override
	public RadioGroup getTipoCreditoRadioGroup()
	{
		return tipoCreditoRadioGroup;
	}
	
	@Override
	public SemanaLetivaComboBox getSemanaLetivaComboBox()
	{
		return semanaLetivaCB;
	}
	
	@Override
	public HorarioComboBox getHorarioComboBox()
	{
		return horarioCB;
	}
	
	@Override
	public SimpleComboBox<Integer> getQtdeCreditosComboBox()
	{
		return qtdeCreditosCB;
	}
}
