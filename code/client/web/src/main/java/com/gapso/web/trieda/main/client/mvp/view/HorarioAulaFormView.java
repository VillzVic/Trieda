package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.HorarioAulaFormPresenter;
import com.gapso.web.trieda.shared.dtos.HorarioAulaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SemanaLetivaComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TextFieldMask;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.i18n.client.DateTimeFormat;

public class HorarioAulaFormView extends MyComposite implements HorarioAulaFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private SemanaLetivaComboBox semanaLetivaCB;
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
		formPanel = new FormPanel() {
			@Override
			public boolean isValid() {
				return super.isValid() && (horarioInicioTF.getValue() != null) && !horarioInicioTF.getValue().isEmpty();
			}
		};
		formPanel.setHeaderVisible(false);
		
		semanaLetivaCB = new SemanaLetivaComboBox();
		semanaLetivaCB.setName(HorarioAulaDTO.PROPERTY_SEMANA_LETIVA_ID);
		semanaLetivaCB.setFieldLabel("Semana Letiva");
		semanaLetivaCB.setAllowBlank(false);
		semanaLetivaCB.setEmptyText("Selecione a Semana Letiva");
		formPanel.add(semanaLetivaCB, formData);
		
		turnoCB = new TurnoComboBox();
		turnoCB.setName(HorarioAulaDTO.PROPERTY_TURNO_ID);
		turnoCB.setFieldLabel("Turno");
		turnoCB.setAllowBlank(false);
		turnoCB.setEmptyText("Selecione o turno");
		formPanel.add(turnoCB, formData);
		
		horarioInicioTF = new TextFieldMask("99:99");
		horarioInicioTF.setRegex("([0-1][0-9]|2[0-4]):([0-5][0-9])$");
		horarioInicioTF.setName(HorarioAulaDTO.PROPERTY_INICIO);
		horarioInicioTF.setEmptyText("Preencha o horário de início de aula");
		if(horarioAulaDTO.getInicio() != null) {
			horarioInicioTF.setValue(df.format(horarioAulaDTO.getInicio()));
		}
		horarioInicioTF.setFieldLabel("Horário Início");
		formPanel.add(horarioInicioTF, formData);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
		
		simpleModal.setFocusWidget(semanaLetivaCB);
	}
	
	public boolean isValid() {
		return formPanel.isValid();
	}
	
	@Override
	public Button getSalvarButton() {
		return simpleModal.getSalvarBt();
	}

	@Override
	public SemanaLetivaComboBox getSemanaLetivaComboBox() {
		return semanaLetivaCB;
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
