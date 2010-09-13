package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.model.HorarioAulaDTO;
import com.gapso.web.trieda.client.mvp.presenter.HorarioAulaFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CalendarioComboBox;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.TurnoComboBox;
import com.google.gwt.i18n.client.DateTimeFormat;

public class HorarioAulaFormView extends MyComposite implements HorarioAulaFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private CalendarioComboBox calendarioCB;
	private TurnoComboBox turnoCB;
	private TextField<String> horarioInicioTF;
	private HorarioAulaDTO horarioAulaDTO;
	
	DateTimeFormat df = DateTimeFormat.getFormat("HH:mm");
	
	public HorarioAulaFormView(HorarioAulaDTO horarioAulaDTO) {
		this.horarioAulaDTO = horarioAulaDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = (horarioAulaDTO.getId() == null)? "Inserção de Horário de Aula" : "Edição de Horário de Aula";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.horarioAula16());
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
		
		horarioInicioTF = new TextField<String>();
		horarioInicioTF.setRegex("([0-1][0-9]|2[0-4]):([0-5][0-9])$");
		horarioInicioTF.setName("horarioInicio");
		if(horarioAulaDTO.getInicio() != null) {
			horarioInicioTF.setValue(df.format(horarioAulaDTO.getInicio()));
		}
		horarioInicioTF.setFieldLabel("Horário Início");
		horarioInicioTF.setAllowBlank(false);
		formPanel.add(horarioInicioTF, formData);
		
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
	public TextField<String> getHorarioInicioTextField() {
		return horarioInicioTF;
	}
	
	@Override
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}

	@Override
	public HorarioAulaDTO getHorarioAulaDTO() {
		return horarioAulaDTO;
	}
	

}
