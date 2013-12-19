package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.Style.Scroll;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FieldSet;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.Radio;
import com.extjs.gxt.ui.client.widget.form.RadioGroup;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.ParametrosGeracaoDemandaPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ParametroGeracaoDemandaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class ParametrosGeracaoDemandaView extends MyComposite 
	implements ParametrosGeracaoDemandaPresenter.Display
{
	private Button submitBt;
	private ContentPanel form;
	private GTabItem tabItem;
	private TurnoComboBox turnoComboBox;
	private CampusComboBox campusComboBox;
	private CheckBox usarDemandasPrioridade2CheckBox;
	private NumberField distanciaMaxEmPeriodosParaPrioridade2NumberField;
	private CheckBox considerarPreRequisitosCheckBox;
	private CheckBox considerarCoRequisitosCheckBox;
	private CheckBox considerarMaturidadeCheckBox;
	private Radio creditoPeriodoRadio;
	private Radio creditoManualRadio;
	private NumberField creditoManualNumberField;
	private CheckBox aumentaMaxCreditosParaAlunosComDisciplinasAtrasadasCheckBox;
	private NumberField fatorAumentoDeMaxCreditosNumberField;
	
	private CenarioDTO cenarioDTO;
	private ParametroGeracaoDemandaDTO parametroGeracaoDemandaDTO;
	public ParametrosGeracaoDemandaView(CenarioDTO cenarioDTO, ParametroGeracaoDemandaDTO parametroGeracaoDemandaDTO) {
		this.cenarioDTO = cenarioDTO;
		this.parametroGeracaoDemandaDTO = parametroGeracaoDemandaDTO;
		initUI();
	}

	private void initUI() {
		createForm();
		createTabItem();
		initComponent(this.tabItem);
	}

	private void createTabItem() {
		this.tabItem = new GTabItem("Parâmetros para Geração de Demanda",Resources.DEFAULTS.parametroPlanejamento16());
		this.tabItem.setContent(this.form,new Margins(5,5,0,5));
	}

	private void createForm() {
		this.form = new FormPanel();
		this.form.setHeadingHtml(cenarioDTO.getNome() + " » Parâmetros de Planejamento");
		this.form.setScrollMode(Scroll.AUTO);
		this.form.setButtonAlign(HorizontalAlignment.RIGHT);
		FormLayout formLayout = new FormLayout();
		formLayout.setLabelWidth(120);
		this.form.setLayout(formLayout);
		
		// CAMPI
		campusComboBox = new CampusComboBox(cenarioDTO);
		campusComboBox.setFieldLabel("Campus");
		this.form.add(campusComboBox);

		// TURNO
		turnoComboBox = new TurnoComboBox(cenarioDTO);
		turnoComboBox.setFieldLabel("Turno");
		this.form.add(turnoComboBox);
		
		this.form.add(criaParametros());

		this.submitBt = new Button("Gerar Demandas",AbstractImagePrototype.create(Resources.DEFAULTS.gerarGrade16()));
		this.form.addButton(this.submitBt);
	}
	
	private FieldSet criaParametros() {
		this.usarDemandasPrioridade2CheckBox = createCheckBox("Usar demandas de prioridade 2 para maximzar o atendimento", 
				parametroGeracaoDemandaDTO.getUsarDemandasDePrioridade2());
		this.distanciaMaxEmPeriodosParaPrioridade2NumberField = new NumberField();
		this.distanciaMaxEmPeriodosParaPrioridade2NumberField.setFieldLabel("Distância máxima (em períodos) do período atual do aluno");
		this.distanciaMaxEmPeriodosParaPrioridade2NumberField.setLabelStyle("width: 330px; padding-left: 30px");
		this.distanciaMaxEmPeriodosParaPrioridade2NumberField.setEmptyText("Distância máxima");
		this.distanciaMaxEmPeriodosParaPrioridade2NumberField.setValue(parametroGeracaoDemandaDTO.getDistanciaMaxEmPeriodosParaPrioridade2());
		this.considerarPreRequisitosCheckBox = createCheckBox("Considerar pré-requisitos", parametroGeracaoDemandaDTO.getConsiderarPreRequisitos());
		this.considerarCoRequisitosCheckBox = createCheckBox("Considerar co-requisitos", parametroGeracaoDemandaDTO.getConsiderarCoRequisitos());
		this.considerarMaturidadeCheckBox = createCheckBox("Considerar maturidade",false);
		this.considerarMaturidadeCheckBox.disable();
		
		RadioGroup maxCreditosRadioGroup = new RadioGroup();
		maxCreditosRadioGroup.setLabelStyle("width: 275px");
		this.creditoPeriodoRadio = new Radio();  
		this.creditoPeriodoRadio.setBoxLabel("Crédito máximo igual ao total de créditos do periodo do aluno");
		this.creditoPeriodoRadio.setValue(parametroGeracaoDemandaDTO.getMaxCreditosPeriodo());
		maxCreditosRadioGroup.add(this.creditoPeriodoRadio);
		this.creditoManualRadio = new Radio();
		this.creditoManualRadio.setBoxLabel("Crédito máximo informado manualmente");
		this.creditoManualRadio.setValue(!parametroGeracaoDemandaDTO.getMaxCreditosPeriodo());
		maxCreditosRadioGroup.add(this.creditoManualRadio);
		this.creditoManualNumberField = new NumberField();
		this.creditoManualNumberField.setLabelStyle("width: 200px; padding-left: 30px");
		this.creditoManualNumberField.setEmptyText("Número de créditos");
		this.creditoManualNumberField.setValue(parametroGeracaoDemandaDTO.getMaxCreditosManual());
		
		LayoutContainer maxCreditosContainer = new LayoutContainer(new ColumnLayout());
		maxCreditosContainer.add(maxCreditosRadioGroup);
		maxCreditosContainer.add(creditoManualNumberField);
		
		this.aumentaMaxCreditosParaAlunosComDisciplinasAtrasadasCheckBox = createCheckBox("Considerar acréscimo de créditos máximos (em %) se o aluno estiver irregular",
				parametroGeracaoDemandaDTO.getAumentaMaxCreditosParaAlunosComDisciplinasAtrasadas());
		this.fatorAumentoDeMaxCreditosNumberField = new NumberField();
		this.fatorAumentoDeMaxCreditosNumberField.setEmptyText("Configurar % de acréscimo");
		this.fatorAumentoDeMaxCreditosNumberField.setValue(parametroGeracaoDemandaDTO.getFatorDeAumentoDeMaxCreditos());
		
		LayoutContainer aumentaMaxCreditosParaAlunosComDisciplinasAtrasadasContainer = new LayoutContainer(new ColumnLayout());
		aumentaMaxCreditosParaAlunosComDisciplinasAtrasadasContainer.add(this.aumentaMaxCreditosParaAlunosComDisciplinasAtrasadasCheckBox);
		aumentaMaxCreditosParaAlunosComDisciplinasAtrasadasContainer.add(this.fatorAumentoDeMaxCreditosNumberField);

		FieldSet parametrosFS = new FieldSet();
		parametrosFS.setHeadingHtml("Parâmetros");
		parametrosFS.setLayout(new FormLayout());
		parametrosFS.add(usarDemandasPrioridade2CheckBox);
		parametrosFS.add(distanciaMaxEmPeriodosParaPrioridade2NumberField);
		parametrosFS.add(considerarPreRequisitosCheckBox);
		parametrosFS.add(considerarCoRequisitosCheckBox);
		parametrosFS.add(considerarMaturidadeCheckBox);
		parametrosFS.add(maxCreditosContainer);
		parametrosFS.add(aumentaMaxCreditosParaAlunosComDisciplinasAtrasadasContainer);
		
		return parametrosFS;
	}
	
	private CheckBox createCheckBox(
			String label, boolean value )
		{
			CheckBox cb = new CheckBox();

			cb.setHideLabel( true );
			cb.setBoxLabel( label );
			cb.setValue( value );

			return cb;
		}

	@Override
	public TurnoComboBox getTurnoComboBox()
	{
		return this.turnoComboBox;
	}

	@Override
	public CampusComboBox getCampusComboBox() {
		return this.campusComboBox;
	}
	
	@Override
	public Button getSubmitButton() {
		return submitBt;
	}
	

	@Override
	public CheckBox getUsarDemandasPrioridade2CheckBox() {
		return usarDemandasPrioridade2CheckBox;
	}

	@Override
	public NumberField getDistanciaMaxEmPeriodosParaPrioridade2NumberField() {
		return distanciaMaxEmPeriodosParaPrioridade2NumberField;
	}

	@Override
	public CheckBox getConsiderarPreRequisitosCheckBox() {
		return considerarPreRequisitosCheckBox;
	}

	@Override
	public CheckBox getConsiderarCoRequisitosCheckBox() {
		return considerarCoRequisitosCheckBox;
	}

	@Override
	public CheckBox getConsiderarMaturidadeCheckBox() {
		return considerarMaturidadeCheckBox;
	}

	@Override
	public Radio getCreditoPeriodoRadio() {
		return creditoPeriodoRadio;
	}

	@Override
	public Radio getCreditoManualRadio() {
		return creditoManualRadio;
	}
	
	@Override
	public NumberField getCreditoManualNumberField() {
		return creditoManualNumberField;
	}

	@Override
	public CheckBox getAumentaMaxCreditosParaAlunosComDisciplinasAtrasadasCheckBox() {
		return aumentaMaxCreditosParaAlunosComDisciplinasAtrasadasCheckBox;
	}

	@Override
	public NumberField getFatorAumentoDeMaxCreditosNumberField() {
		return fatorAumentoDeMaxCreditosNumberField;
	}
}
