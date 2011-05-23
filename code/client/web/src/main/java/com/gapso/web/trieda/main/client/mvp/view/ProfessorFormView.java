package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessorFormPresenter;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.AreaTitulacaoComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TipoContratoComboBox;
import com.gapso.web.trieda.shared.util.view.TitulacaoComboBox;
import com.gapso.web.trieda.shared.util.view.UniqueDomain;
import com.gapso.web.trieda.shared.util.view.UniqueTextField;

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
	private CenarioDTO cenarioDTO;
	
	public ProfessorFormView(CenarioDTO cenarioDTO) {
		this(new ProfessorDTO(), null, null, null, cenarioDTO);
	}
	public ProfessorFormView(ProfessorDTO professorDTO, TipoContratoDTO tipoContratoDTO, TitulacaoDTO titulacaoDTO, AreaTitulacaoDTO areaTitulacaoDTO, CenarioDTO cenarioDTO) {
		this.professorDTO = professorDTO;
		this.tipoContratoDTO = tipoContratoDTO;
		this.titulacaoDTO = titulacaoDTO;
		this.areaTitulacaoDTO = areaTitulacaoDTO;
		this.cenarioDTO = cenarioDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = (professorDTO.getId() == null)? "Inserção de Professor" : "Edição de Professor";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.professor16());
		simpleModal.setHeight(395);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		cpfTF = new UniqueTextField(cenarioDTO, UniqueDomain.PROFESSOR);
		cpfTF.setName(ProfessorDTO.PROPERTY_CPF);
		cpfTF.setValue(professorDTO.getCpf());
		cpfTF.setFieldLabel("CPF");
		cpfTF.setAllowBlank(false);
		cpfTF.setMaxLength(14);
		cpfTF.setMinLength(14);
		cpfTF.setEmptyText("Preencha o CPF");
		formPanel.add(cpfTF, formData);
		
		nomeTF = new TextField<String>();
		nomeTF.setName(ProfessorDTO.PROPERTY_NOME);
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
		cargaHorariaMaxNF.setName(ProfessorDTO.PROPERTY_CARGA_HORARIA_MAX);
		cargaHorariaMaxNF.setValue(professorDTO.getCargaHorariaMax());
		cargaHorariaMaxNF.setFieldLabel("Carga Horária Máx.");
		cargaHorariaMaxNF.setAllowBlank(false);
		cargaHorariaMaxNF.setAllowDecimals(false);
		cargaHorariaMaxNF.setMaxValue(99);
		cargaHorariaMaxNF.setEmptyText("Somente números");
		formPanel.add(cargaHorariaMaxNF, formData);
		
		cargaHorariaMinNF = new NumberField();
		cargaHorariaMinNF.setName(ProfessorDTO.PROPERTY_CARGA_HORARIA_MIN);
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
		creditoAnteriorNF.setName(ProfessorDTO.PROPERTY_CREDITO_ANTERIOR);
		creditoAnteriorNF.setValue(professorDTO.getCreditoAnterior());
		creditoAnteriorNF.setFieldLabel("Carga Horária Anterior");
		creditoAnteriorNF.setAllowBlank(false);
		creditoAnteriorNF.setAllowDecimals(false);
		creditoAnteriorNF.setMaxValue(99);
		creditoAnteriorNF.setEmptyText("Somente números");
		formPanel.add(creditoAnteriorNF, formData);
		
		valorCreditoNF = new NumberField();
		valorCreditoNF.setName(ProfessorDTO.PROPERTY_VALOR_CREDITO);
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
