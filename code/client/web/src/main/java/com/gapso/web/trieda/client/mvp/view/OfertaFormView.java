package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.model.OfertaDTO;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.mvp.presenter.OfertaFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.CurriculoComboBox;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.TurnoComboBox;

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
		simpleModal.setHeight(160);
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
		formPanel.add(turnoCB, formData);
		
		campusCB = new CampusComboBox();
		campusCB.setName("campus");
		campusCB.setFieldLabel("Campus");
		campusCB.setAllowBlank(false);
		campusCB.setValue(campusDTO);
		formPanel.add(campusCB, formData);
		
		curriculoCB = new CurriculoComboBox();
		curriculoCB.setName("curriculo");
		curriculoCB.setFieldLabel("Curriculo");
		curriculoCB.setAllowBlank(false);
		curriculoCB.setValue(curriculoDTO);
		formPanel.add(curriculoCB, formData);
		
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
