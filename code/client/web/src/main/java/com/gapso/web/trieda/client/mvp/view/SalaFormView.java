package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.presenter.SalaFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.TipoSalaComboBox;
import com.gapso.web.trieda.client.util.view.UnidadeComboBox;

public class SalaFormView extends MyComposite implements SalaFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> codigoTF;
	private TextField<String> numeroTF;
	private TextField<String> andarTF;
	private NumberField capacidadeNF;
	private UnidadeComboBox unidadeCB;
	private TipoSalaComboBox tipoSalaCB;
	private SalaDTO salaDTO;
	
	public SalaFormView(SalaDTO salaDTO) {
		this.salaDTO = salaDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = (salaDTO.getId() == null)? "Inserção de Sala" : "Edição de Sala";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.sala16());
		simpleModal.setHeight(240);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		unidadeCB = new UnidadeComboBox();
		unidadeCB.setName("unidade");
		unidadeCB.setFieldLabel("Unidade");
		unidadeCB.setAllowBlank(false);
		formPanel.add(unidadeCB, formData);
		
		tipoSalaCB = new TipoSalaComboBox();
		tipoSalaCB.setName("tipoSala");
		tipoSalaCB.setFieldLabel("Tipo");
		tipoSalaCB.setAllowBlank(false);
		formPanel.add(tipoSalaCB, formData);
		
		codigoTF = new TextField<String>();
		codigoTF.setName("codigo");
		codigoTF.setValue(salaDTO.getCodigo());
		codigoTF.setFieldLabel("Código");
		codigoTF.setAllowBlank(false);
		codigoTF.setMinLength(3);
		codigoTF.setMaxLength(20);
		formPanel.add(codigoTF, formData);
		
		numeroTF = new TextField<String>();
		numeroTF.setName("numero");
		numeroTF.setValue(salaDTO.getNumero());
		numeroTF.setFieldLabel("Número");
		numeroTF.setAllowBlank(false);
		numeroTF.setMinLength(1);
		numeroTF.setMaxLength(20);
		formPanel.add(numeroTF, formData);
		
		andarTF = new TextField<String>();
		andarTF.setName("andar");
		andarTF.setValue(salaDTO.getAndar());
		andarTF.setFieldLabel("Andar");
		andarTF.setAllowBlank(false);
		andarTF.setMinLength(1);
		andarTF.setMaxLength(20);
		formPanel.add(andarTF, formData);
		
		capacidadeNF = new NumberField();
		capacidadeNF.setName("capacidade");
		capacidadeNF.setValue(salaDTO.getCapacidade());
		capacidadeNF.setFieldLabel("Capacidade");
		capacidadeNF.setAllowBlank(false);
		capacidadeNF.setMinLength(1);
		capacidadeNF.setMaxLength(20);
		formPanel.add(capacidadeNF, formData);
		
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
	public TextField<String> getCodigoTextField() {
		return codigoTF;
	}

	@Override
	public TextField<String> getNumeroTextField() {
		return numeroTF;
	}
	
	@Override
	public TextField<String> getAndarTextField() {
		return andarTF;
	}
	
	@Override
	public NumberField getCapacidadeNumberField() {
		return capacidadeNF;
	}
	
	@Override
	public UnidadeComboBox getUnidadeComboBox() {
		return unidadeCB;
	}
	
	@Override
	public TipoSalaComboBox getTipoComboBox() {
		return tipoSalaCB;
	}

	@Override
	public SalaDTO getSalaDTO() {
		return salaDTO;
	}
	
	@Override
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}

}
