package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.FixacaoDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.gapso.web.trieda.client.mvp.presenter.FixacaoFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.client.util.view.SalaComboBox;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.UnidadeComboBox;

public class FixacaoFormView extends MyComposite implements FixacaoFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> codigoTF;
	private TextField<String> descricaoTF;
	private DisciplinaComboBox disciplinaCB;
	private CampusComboBox campusCB;
	private UnidadeComboBox unidadeCB;
	private SalaComboBox salaCB;
	
	private FixacaoDTO fixacaoDTO;
	private DisciplinaDTO disciplinaDTO;
	private CampusDTO campusDTO;
	private UnidadeDTO unidadeDTO;
	private SalaDTO salaDTO;
	
	public FixacaoFormView(FixacaoDTO fixacaoDTO, DisciplinaDTO disciplinaDTO, CampusDTO campusDTO, UnidadeDTO unidadeDTO, SalaDTO salaDTO) {
		this.fixacaoDTO = fixacaoDTO;
		this.disciplinaDTO = disciplinaDTO;
		this.campusDTO = campusDTO;
		this.unidadeDTO = unidadeDTO;
		this.salaDTO = salaDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = (fixacaoDTO.getId() == null)? "Inserção de Fixação" : "Edição de Fixação";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.outros16());
		simpleModal.setHeight(238);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		codigoTF = new TextField<String>();
		codigoTF.setName("codigo");
		codigoTF.setValue(fixacaoDTO.getCodigo());
		codigoTF.setFieldLabel("Código");
		codigoTF.setAllowBlank(false);
		codigoTF.setMinLength(1);
		codigoTF.setMaxLength(50);
		codigoTF.setEmptyText("Preencha o código");
		formPanel.add(codigoTF, formData);
		
		descricaoTF = new TextField<String>();
		descricaoTF.setName("descricao");
		descricaoTF.setValue(fixacaoDTO.getDescricao());
		descricaoTF.setFieldLabel("Descrição");
		descricaoTF.setAllowBlank(false);
		descricaoTF.setMinLength(1);
		descricaoTF.setMaxLength(50);
		descricaoTF.setEmptyText("Preencha a descrição");
		formPanel.add(descricaoTF, formData);
		
		disciplinaCB = new DisciplinaComboBox();
		disciplinaCB.setValue(disciplinaDTO);
		formPanel.add(disciplinaCB, formData);
		
		campusCB = new CampusComboBox();
		campusCB.setValue(campusDTO);
		formPanel.add(campusCB, formData);
		
		unidadeCB = new UnidadeComboBox(campusCB);
		unidadeCB.setValue(unidadeDTO);
		formPanel.add(unidadeCB, formData);
		
		salaCB = new SalaComboBox(unidadeCB);
		salaCB.setValue(salaDTO);
		formPanel.add(salaCB, formData);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
		
		simpleModal.setFocusWidget(codigoTF);
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
	public FixacaoDTO getFixacaoDTO() {
		return fixacaoDTO;
	}

	@Override
	public TextField<String> getCodigoTextField() {
		return codigoTF;
	}
	
	@Override
	public TextField<String> getDescricaoTextField() {
		return descricaoTF;
	}
	
	@Override
	public DisciplinaComboBox getDisciplinaComboBox() {
		return disciplinaCB;
	}
	
	@Override
	public CampusComboBox getCampusComboBox() {
		return campusCB;
	}
	
	@Override
	public UnidadeComboBox getUnidadeComboBox() {
		return unidadeCB;
	}
	
	@Override
	public SalaComboBox getSalaComboBox() {
		return salaCB;
	}

}