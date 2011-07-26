package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.SemanaLetivaFormPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.UniqueDomain;
import com.gapso.web.trieda.shared.util.view.UniqueTextField;

public class SemanaLetivaFormView extends MyComposite implements SemanaLetivaFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> codigoTF;
	private TextField<String> descricaoTF;
	private CheckBox oficialCB;
	private SemanaLetivaDTO semanaLetivaDTO;
	private CenarioDTO cenarioDTO;
	
	public SemanaLetivaFormView(CenarioDTO cenarioDTO) {
		this(new SemanaLetivaDTO(), cenarioDTO);
	}
	public SemanaLetivaFormView(SemanaLetivaDTO semanaLetivaDTO, CenarioDTO cenarioDTO) {
		this.semanaLetivaDTO = semanaLetivaDTO;
		this.cenarioDTO = cenarioDTO;
		initUI();
	}
	
	private void initUI()
	{
		String title = ( ( semanaLetivaDTO.getId() == null )? "Inserção de Semana Letiva" : "Edição de Semana Letiva" );
		simpleModal = new SimpleModal( title, Resources.DEFAULTS.semanaLetiva16() );
		simpleModal.setHeight( 158 );
		createForm();
		simpleModal.setContent( formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);

		codigoTF = new UniqueTextField(cenarioDTO, UniqueDomain.SEMANA_LETIVA);
		codigoTF.setName(SemanaLetivaDTO.PROPERTY_CODIGO);
		codigoTF.setValue(semanaLetivaDTO.getCodigo());
		codigoTF.setFieldLabel("Codigo");
		codigoTF.setAllowBlank(false);
		codigoTF.setMinLength(1);
		codigoTF.setMaxLength(20);
		codigoTF.setEmptyText("Preencha o código");
		formPanel.add(codigoTF, formData);

		descricaoTF = new TextField<String>();
		descricaoTF.setName(SemanaLetivaDTO.PROPERTY_DESCRICAO);
		descricaoTF.setValue(semanaLetivaDTO.getDescricao());
		descricaoTF.setFieldLabel("Descrição");
		descricaoTF.setAllowBlank(false);
		descricaoTF.setMaxLength(50);
		descricaoTF.setEmptyText("Preencha uma descrição");
		formPanel.add(descricaoTF, formData);

		oficialCB = new CheckBox();
		oficialCB.setName(SemanaLetivaDTO.PROPERTY_OFICIAL);
		oficialCB.setValue(semanaLetivaDTO.getOficial());
		oficialCB.setFieldLabel("Oficial?");
		formPanel.add(oficialCB, formData);

		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());

		simpleModal.setFocusWidget(codigoTF);
	}

	public boolean isValid()
	{
		return formPanel.isValid();
	}

	@Override
	public Button getSalvarButton()
	{
		return simpleModal.getSalvarBt();
	}

	@Override
	public TextField< String > getCodigoTextField()
	{
		return codigoTF;
	}

	@Override
	public SimpleModal getSimpleModal()
	{
		return simpleModal;
	}

	@Override
	public TextField< String > getDescricaoTextField()
	{
		return descricaoTF;
	}

	@Override
	public SemanaLetivaDTO getSemanaLetivaDTO()
	{
		return semanaLetivaDTO;
	}

	@Override
	public CheckBox getOficialCheckBox()
	{
		return oficialCB;
	}
}
