package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.DivisaoCreditosFormPresenter;
import com.gapso.web.trieda.main.client.util.resources.Resources;
import com.gapso.web.trieda.main.client.util.view.SimpleModal;
import com.gapso.web.trieda.shared.dtos.DivisaoCreditoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;

public class DivisaoCreditosFormView extends MyComposite implements DivisaoCreditosFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private NumberField dia1NF;
	private NumberField dia2NF;
	private NumberField dia3NF;
	private NumberField dia4NF;
	private NumberField dia5NF;
	private NumberField dia6NF;
	private NumberField dia7NF;
	private DivisaoCreditoDTO divisaoCreditoDTO;
	
	public DivisaoCreditosFormView(DivisaoCreditoDTO divisaoCreditoDTO) {
		this.divisaoCreditoDTO = divisaoCreditoDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = (divisaoCreditoDTO.getId() == null)? "Inserção de Divisão de Créditos" : "Edição de Divisão de Créditos";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.divisaoDeCreditos16());
		simpleModal.setHeight(265);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		dia1NF = createNumberField(DivisaoCreditoDTO.PROPERTY_DIA1, "Dia 1", divisaoCreditoDTO.getDia1());
		dia2NF = createNumberField(DivisaoCreditoDTO.PROPERTY_DIA2, "Dia 2", divisaoCreditoDTO.getDia2());
		dia3NF = createNumberField(DivisaoCreditoDTO.PROPERTY_DIA3, "Dia 3", divisaoCreditoDTO.getDia3());
		dia4NF = createNumberField(DivisaoCreditoDTO.PROPERTY_DIA4, "Dia 4", divisaoCreditoDTO.getDia4());
		dia5NF = createNumberField(DivisaoCreditoDTO.PROPERTY_DIA5, "Dia 5", divisaoCreditoDTO.getDia5());
		dia6NF = createNumberField(DivisaoCreditoDTO.PROPERTY_DIA6, "Dia 6", divisaoCreditoDTO.getDia6());
		dia7NF = createNumberField(DivisaoCreditoDTO.PROPERTY_DIA7, "Dia 7", divisaoCreditoDTO.getDia7());
		
		formPanel.add(dia1NF, formData);
		formPanel.add(dia2NF, formData);
		formPanel.add(dia3NF, formData);
		formPanel.add(dia4NF, formData);
		formPanel.add(dia5NF, formData);
		formPanel.add(dia6NF, formData);
		formPanel.add(dia7NF, formData);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
		
		simpleModal.setFocusWidget(dia1NF);
	}
	
	private NumberField createNumberField(String name, String label, Integer value) {
		NumberField nf = new NumberField();
		nf.setName(name);
		nf.setValue(value);
		nf.setFieldLabel(label);
		nf.setAllowDecimals(false);
		nf.setAllowNegative(false);
		nf.setMinValue(0);
		nf.setMaxValue(99);
		nf.setMaxLength(99);
		nf.setEmptyText("Somente números inteiros");
		return nf;
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

	@Override public NumberField getDia1NumberField() { return dia1NF; }
	@Override public NumberField getDia2NumberField() { return dia2NF; }
	@Override public NumberField getDia3NumberField() { return dia3NF; }
	@Override public NumberField getDia4NumberField() { return dia4NF; }
	@Override public NumberField getDia5NumberField() { return dia5NF; }
	@Override public NumberField getDia6NumberField() { return dia6NF; }
	@Override public NumberField getDia7NumberField() { return dia7NF; }

	@Override
	public DivisaoCreditoDTO getDivisaoCreditoDTO() {
		return divisaoCreditoDTO;
	}
	

}
