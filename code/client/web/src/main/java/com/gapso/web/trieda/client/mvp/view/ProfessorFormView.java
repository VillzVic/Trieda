package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.model.AreaTitulacaoDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorDTO;
import com.gapso.web.trieda.client.mvp.model.TipoContratoDTO;
import com.gapso.web.trieda.client.mvp.model.TitulacaoDTO;
import com.gapso.web.trieda.client.mvp.presenter.ProfessorFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.AreaTitulacaoComboBox;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.TipoContratoComboBox;
import com.gapso.web.trieda.client.util.view.TitulacaoComboBox;

public class ProfessorFormView extends MyComposite implements ProfessorFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;

	private TextField<String> cpfTF;
	private TextField<String> nomeTF;
	private TipoContratoComboBox tipoContratoCB;
	private NumberField cargaHorariaMaxNF;
	private NumberField cargaHorariaMinNF;
	private TitulacaoComboBox titulacaoCB;
	private AreaTitulacaoComboBox areaTitulacaoCB;
	private NumberField creditoAnteriorNF;
	private NumberField valorCreditoNF;
	
	private ProfessorDTO professorDTO;
	private TipoContratoDTO tipoContratoDTO;
	private TitulacaoDTO titulacaoDTO;
	private AreaTitulacaoDTO areaTitulacaoDTO;
	
	public ProfessorFormView(ProfessorDTO professorDTO, TipoContratoDTO tipoContratoDTO, TitulacaoDTO titulacaoDTO, AreaTitulacaoDTO areaTitulacaoDTO) {
		this.professorDTO = professorDTO;
		this.tipoContratoDTO = tipoContratoDTO;
		this.titulacaoDTO = titulacaoDTO;
		this.areaTitulacaoDTO = areaTitulacaoDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = (professorDTO.getId() == null)? "Inserção de Professor" : "Edição de Professor";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.professor16());
		simpleModal.setHeight(380);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		cpfTF = new TextField<String>();
		cpfTF.setName("cpf");
		cpfTF.setValue(professorDTO.getCpf());
		cpfTF.setFieldLabel("CPF");
		cpfTF.setAllowBlank(false);
		cpfTF.setMaxLength(14);
		cpfTF.setMinLength(14);
		cpfTF.setEmptyText("Preencha o CPF");
		formPanel.add(cpfTF, formData);
		
		nomeTF = new TextField<String>();
		nomeTF.setName("nome");
		nomeTF.setValue(professorDTO.getNome());
		nomeTF.setFieldLabel("Nome");
		nomeTF.setAllowBlank(false);
		nomeTF.setMinLength(3);
		nomeTF.setMaxLength(50);
		nomeTF.setEmptyText("Preencha o nome");
		formPanel.add(nomeTF, formData);
		
		tipoContratoCB = new TipoContratoComboBox();
		tipoContratoCB.setAllowBlank(false);
		tipoContratoCB.setValue(tipoContratoDTO);
		formPanel.add(tipoContratoCB, formData);

		cargaHorariaMaxNF = new NumberField();
		cargaHorariaMaxNF.setName("cargaHorariaMax");
		cargaHorariaMaxNF.setValue(professorDTO.getCargaHorariaMax());
		cargaHorariaMaxNF.setFieldLabel("Carga Horária Máx.");
		cargaHorariaMaxNF.setAllowBlank(false);
		cargaHorariaMaxNF.setAllowDecimals(false);
		cargaHorariaMaxNF.setMaxValue(99);
		cargaHorariaMaxNF.setEmptyText("Somente números");
		formPanel.add(cargaHorariaMaxNF, formData);
		
		cargaHorariaMinNF = new NumberField();
		cargaHorariaMinNF.setName("cargaHorariaMin");
		cargaHorariaMinNF.setValue(professorDTO.getCargaHorariaMin());
		cargaHorariaMinNF.setFieldLabel("Carga Horária Min.");
		cargaHorariaMinNF.setAllowBlank(false);
		cargaHorariaMinNF.setAllowDecimals(false);
		cargaHorariaMinNF.setMaxValue(99);
		cargaHorariaMinNF.setEmptyText("Somente números");
		formPanel.add(cargaHorariaMinNF, formData);
		
		titulacaoCB = new TitulacaoComboBox();
		titulacaoCB.setAllowBlank(false);
		titulacaoCB.setValue(titulacaoDTO);
		formPanel.add(titulacaoCB, formData);
		
		areaTitulacaoCB = new AreaTitulacaoComboBox();
		areaTitulacaoCB.setAllowBlank(false);
		areaTitulacaoCB.setValue(areaTitulacaoDTO);
		formPanel.add(areaTitulacaoCB, formData);
		
		creditoAnteriorNF = new NumberField();
		creditoAnteriorNF.setName("creditoAnterior");
		creditoAnteriorNF.setValue(professorDTO.getCreditoAnterior());
		creditoAnteriorNF.setFieldLabel("Carga Horária Anterior");
		creditoAnteriorNF.setAllowBlank(false);
		creditoAnteriorNF.setAllowDecimals(false);
		creditoAnteriorNF.setMaxValue(99);
		creditoAnteriorNF.setEmptyText("Somente números");
		formPanel.add(creditoAnteriorNF, formData);
		
		valorCreditoNF = new NumberField();
		valorCreditoNF.setName("valorCredito");
		valorCreditoNF.setValue(professorDTO.getValorCredito());
		valorCreditoNF.setFieldLabel("Crédito (R$)");
		valorCreditoNF.setAllowBlank(false);
		valorCreditoNF.setAllowDecimals(true);
		valorCreditoNF.setMaxValue(999999);
		valorCreditoNF.setEmptyText("Somente números");
		formPanel.add(valorCreditoNF, formData);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
		
		simpleModal.setFocusWidget(cpfTF);
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
	public TextField<String> getNomeTextField() {
		return nomeTF;
	}
	
	@Override
	public TextField<String> getCpfTextField() {
		return cpfTF;
	}
	
	@Override
	public TipoContratoComboBox getTipoContratoComboBox() {
		return tipoContratoCB;
	}
	
	@Override
	public NumberField getCargaHorariaMaxNumberField() {
		return cargaHorariaMaxNF;
	}
	
	@Override
	public NumberField getCargaHorariaMinNumberField() {
		return cargaHorariaMinNF;
	}

	@Override
	public TitulacaoComboBox getTitulacaoComboBox() {
		return titulacaoCB;
	}
	
	@Override
	public AreaTitulacaoComboBox getAreaTitulacaoComboBox() {
		return areaTitulacaoCB;
	}
	
	@Override
	public NumberField getCreditoAnteriorNumberField() {
		return creditoAnteriorNF;
	}
	
	@Override
	public NumberField getValorCreditoNumberField() {
		return valorCreditoNF;
	}
	
	@Override
	public ProfessorDTO getProfessorDTO() {
		return professorDTO;
	}
	

}
