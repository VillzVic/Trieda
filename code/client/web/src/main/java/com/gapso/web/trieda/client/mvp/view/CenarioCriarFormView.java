package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.DualListField;
import com.extjs.gxt.ui.client.widget.form.DualListField.Mode;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.ListField;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.presenter.CenarioCriarFormPresenter;
import com.gapso.web.trieda.client.services.CampiServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.SemanaLetivaComboBox;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class CenarioCriarFormView extends MyComposite implements CenarioCriarFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private CheckBox oficialCB;
	private TextField<String> nomeTF;
	private NumberField anoTF;
	private NumberField semestreTF;
	private TextField<String> comentarioTF;
	private SemanaLetivaComboBox semanaLetivaCB;
	private DualListField<CampusDTO> campiDualList;
	private CenarioDTO cenarioDTO;
	
	public CenarioCriarFormView(CenarioDTO cenarioDTO) {
		this.cenarioDTO = cenarioDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = "Inserção de Cenário";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.cenario16());
		simpleModal.setHeight(380);
		simpleModal.setWidth(400);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		
		FormLayout formLayout = new FormLayout(LabelAlign.RIGHT);
		formLayout.setLabelWidth(75);
		
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		formPanel.setLayout(formLayout);
		
		nomeTF = new TextField<String>();
		nomeTF.setName(CenarioDTO.PROPERTY_NOME);
		nomeTF.setValue(cenarioDTO.getNome());
		nomeTF.setFieldLabel("Nome");
		nomeTF.setAllowBlank(false);
		nomeTF.setMinLength(1);
		nomeTF.setMaxLength(50);
		nomeTF.setEmptyText("Preencha o nome");
		formPanel.add(nomeTF, formData);
		
		oficialCB = new CheckBox();
		oficialCB.setName(CenarioDTO.PROPERTY_OFICIAL);
		oficialCB.setValue(cenarioDTO.getOficial());
		oficialCB.setFieldLabel("Oficial");
		formPanel.add(oficialCB, formData);
		
		anoTF = new NumberField();
		anoTF.setName(CenarioDTO.PROPERTY_ANO);
		anoTF.setValue(cenarioDTO.getAno());
		anoTF.setFieldLabel("Ano");
		anoTF.setAllowDecimals(false);
		anoTF.setAllowBlank(false);
		anoTF.setMinValue(1);
		anoTF.setMaxValue(9999);
		anoTF.setEmptyText("Preencha o ano letivo");
		formPanel.add(anoTF, formData);
		
		semestreTF = new NumberField();
		semestreTF.setName(CenarioDTO.PROPERTY_SEMESTRE);
		semestreTF.setValue(cenarioDTO.getSemestre());
		semestreTF.setFieldLabel("Semestre");
		semestreTF.setAllowDecimals(false);
		semestreTF.setAllowBlank(false);
		semestreTF.setMinValue(1);
		semestreTF.setMaxValue(12);
		semestreTF.setEmptyText("Preencha semestre letivo");
		formPanel.add(semestreTF, formData);
		
		semanaLetivaCB = new SemanaLetivaComboBox();
		semanaLetivaCB.setName("semanaLetiva");
		semanaLetivaCB.setFieldLabel("Semana Letiva");
		semanaLetivaCB.setAllowBlank(false);
		semanaLetivaCB.setEmptyText("Seleciona a Semana Letiva do cenário");
		formPanel.add(semanaLetivaCB, formData);
		
		comentarioTF = new TextField<String>();
		comentarioTF.setName(CenarioDTO.PROPERTY_COMENTARIO);
		comentarioTF.setValue(cenarioDTO.getComentario());
		comentarioTF.setFieldLabel("Comentário");
		comentarioTF.setMaxLength(255);
		comentarioTF.setEmptyText("Preencha um comentário");
		formPanel.add(comentarioTF, formData);
		
		campiDualList = new DualListField<CampusDTO>();
		ListField<CampusDTO> from = campiDualList.getFromField();
		from.setDisplayField("codigo");
		from.setStore(new ListStore<CampusDTO>());
		ListField<CampusDTO> to = campiDualList.getToField();
		to.setDisplayField("codigo");
		to.setStore(new ListStore<CampusDTO>());
		campiDualList.setMode(Mode.APPEND);  
		campiDualList.setFieldLabel("Campi");
		populaCampi();
		formPanel.add(campiDualList, formData);
		
		formPanel.add(formPanel, formData);
		
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
		
		simpleModal.setFocusWidget(nomeTF);
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
	public CheckBox getOficialCheckBox() {
		return oficialCB;
	}
	
	@Override
	public TextField<String> getNomeTextField() {
		return nomeTF;
	}
	
	@Override
	public NumberField getAnoTextField() {
		return anoTF;
	}
	
	@Override
	public NumberField getSemestreTextField() {
		return semestreTF;
	}
	
	@Override
	public TextField<String> getComentarioTextField() {
		return comentarioTF;
	}
	
	@Override
	public SemanaLetivaComboBox getSemanaLetivaComboBox() {
		return semanaLetivaCB;
	}

	@Override
	public CenarioDTO getCenarioDTO() {
		return cenarioDTO;
	}

	@Override
	public DualListField<CampusDTO> getCampiDualList() {
		return campiDualList;
	}
	
	// TODO Tirar da View, colocar no Presenter
	private void populaCampi() {
		final CampiServiceAsync service = Services.campi();
		service.getListAll(new AsyncCallback<ListLoadResult<CampusDTO>>() {
			@Override
			public void onFailure(Throwable caught) {
				caught.printStackTrace();
			}
			@Override
			public void onSuccess(ListLoadResult<CampusDTO> result) {
				campiDualList.getFromField().getStore().add(result.getData());
			}
		});
	}

}
