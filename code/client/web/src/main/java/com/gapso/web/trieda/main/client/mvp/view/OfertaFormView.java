package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.OfertaFormPresenter;
import com.gapso.web.trieda.main.client.util.resources.Resources;
import com.gapso.web.trieda.main.client.util.view.CampusComboBox;
import com.gapso.web.trieda.main.client.util.view.CurriculoComboBox;
import com.gapso.web.trieda.main.client.util.view.SimpleModal;
import com.gapso.web.trieda.main.client.util.view.TurnoComboBox;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;

public class OfertaFormView extends MyComposite implements OfertaFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TurnoComboBox turnoCB;
	private CampusComboBox campusCB;
	private CurriculoComboBox curriculoCB;
	private OfertaDTO ofertaDTO;
	private TurnoDTO turnoDTO;
	private CampusDTO campusDTO;
	private CurriculoDTO curriculoDTO;
	
	public OfertaFormView(OfertaDTO ofertaDTO, TurnoDTO turnoDTO, CampusDTO campusDTO, CurriculoDTO curriculoDTO) {
		this.ofertaDTO = ofertaDTO;
		this.turnoDTO = turnoDTO;
		this.campusDTO = campusDTO;
		this.curriculoDTO = curriculoDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = (ofertaDTO.getId() == null)? "Inserção de Oferta de Curso em Campus" : "Edição de de Oferta de Curso em Campus";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.matrizCurricular16());
		simpleModal.setHeight(165);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		turnoCB = new TurnoComboBox();
		turnoCB.setName("turno");
		turnoCB.setFieldLabel("Turno");
		turnoCB.setAllowBlank(false);
		turnoCB.setValue(turnoDTO);
		turnoCB.setEmptyText("Selecione o turno");
		formPanel.add(turnoCB, formData);
		
		campusCB = new CampusComboBox();
		campusCB.setName("campus");
		campusCB.setFieldLabel("Campus");
		campusCB.setAllowBlank(false);
		campusCB.setValue(campusDTO);
		campusCB.setEmptyText("Selecione o campus");
		formPanel.add(campusCB, formData);
		
		curriculoCB = new CurriculoComboBox();
		curriculoCB.setName("curriculo");
		curriculoCB.setFieldLabel("Matriz Curricular");
		curriculoCB.setAllowBlank(false);
		curriculoCB.setValue(curriculoDTO);
		curriculoCB.setEmptyText("Selecione uma Matriz Curricular");
		formPanel.add(curriculoCB, formData);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
		
		simpleModal.setFocusWidget(turnoCB);
	}
	
	public boolean isValid() {
		return formPanel.isValid();
	}
	
	@Override
	public Button getSalvarButton() {
		return simpleModal.getSalvarBt();
	}
	
	@Override
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}
	
	@Override
	public TurnoComboBox getTurnoComboBox() {
		return turnoCB;
	}
	
	@Override
	public CampusComboBox getCampusComboBox() {
		return campusCB;
	}
	
	@Override
	public CurriculoComboBox getCurriculoComboBox() {
		return curriculoCB;
	}

	@Override
	public OfertaDTO getOfertaDTO() {
		return ofertaDTO;
	}

}
