package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TimeField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.model.PeriodoDeAulaDTO;
import com.gapso.web.trieda.client.mvp.presenter.PeriodoDeAulaFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CalendarioComboBox;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.TurnoComboBox;

public class PeriodoDeAulaFormView extends MyComposite implements PeriodoDeAulaFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private CalendarioComboBox calendarioCB;
	private TurnoComboBox turnoCB;
	private TimeField horarioInicioTF;
	private TimeField horarioFimTF;
	private PeriodoDeAulaDTO periodoDeAulaDTO;
	
	public PeriodoDeAulaFormView(PeriodoDeAulaDTO turnoDTO) {
		this.periodoDeAulaDTO = turnoDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		simpleModal = new SimpleModal("Turno", Resources.DEFAULTS.turnos16());
		simpleModal.setHeight(187);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		calendarioCB = new CalendarioComboBox();
		calendarioCB.setName("calendario");
		calendarioCB.setFieldLabel("Calendário");
		calendarioCB.setAllowBlank(false);
		formPanel.add(calendarioCB, formData);
		
		turnoCB = new TurnoComboBox();
		turnoCB.setName("turno");
		turnoCB.setFieldLabel("Turno");
		turnoCB.setAllowBlank(false);
		formPanel.add(turnoCB, formData);
		
		horarioInicioTF = new TimeField();
		horarioInicioTF.setName("horarioInicio");
		horarioInicioTF.setFieldLabel("Horário Início");
		horarioInicioTF.setAllowBlank(false);
		formPanel.add(horarioInicioTF, formData);
		
		horarioFimTF = new TimeField();
		horarioFimTF.setName("horarioFim");
		horarioFimTF.setFieldLabel("Horário Fim");
		horarioFimTF.setEditable(false);
		horarioFimTF.setHideTrigger(true);
		formPanel.add(horarioFimTF, formData);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
	}
	
	public boolean isValid() {
		return formPanel.isValid();
	}
	
	@Override
	public Button getSalvarButton() {
		return simpleModal.getSalvarBt();
	}

	@Override
	public CalendarioComboBox getCalendarioComboBox() {
		return calendarioCB;
	}

	@Override
	public TurnoComboBox getTurnoComboBox() {
		return turnoCB;
	}

	@Override
	public TimeField getHorarioInicioTextField() {
		return horarioInicioTF;
	}
	
	@Override
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}

	@Override
	public PeriodoDeAulaDTO getPeriodoDeAulaDTO() {
		return periodoDeAulaDTO;
	}
	

}
