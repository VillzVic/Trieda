package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.UnidadeFormPresenter;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.UniqueDomain;
import com.gapso.web.trieda.shared.util.view.UniqueTextField;

public class UnidadeFormView extends MyComposite
	implements UnidadeFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> nomeTF;
	private TextField<String> codigoTF;
	private CampusComboBox campusCB;
	private UnidadeDTO unidadeDTO;
	private CampusDTO campusDTO;
	private CenarioDTO cenarioDTO;

	public UnidadeFormView( CenarioDTO cenarioDTO )
	{
		this( new UnidadeDTO(), null, cenarioDTO );
	}

	public UnidadeFormView( UnidadeDTO unidadeDTO,
		CampusDTO campusDTO, CenarioDTO cenarioDTO )
	{
		this.unidadeDTO = unidadeDTO;
		this.campusDTO = campusDTO;
		this.cenarioDTO = cenarioDTO;

		initUI();
	}

	private void initUI()
	{
		String title = ( ( unidadeDTO.getId() == null ) ?
			( getI18nConstants().insercaoDe() + getI18nConstants().unidade() ) :
			( getI18nConstants().edicaoDe() + getI18nConstants().unidade() ) );

		simpleModal = new SimpleModal( title, Resources.DEFAULTS.unidade16() );
		simpleModal.setHeight( 160 );
		createForm();
		simpleModal.setContent( formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);

		codigoTF = new UniqueTextField( cenarioDTO, UniqueDomain.UNIDADE );
		codigoTF.setName(UnidadeDTO.PROPERTY_CODIGO);
		codigoTF.setValue(unidadeDTO.getCodigo());
		codigoTF.setFieldLabel(getI18nConstants().codigo());
		codigoTF.setAllowBlank(false);
		codigoTF.setMinLength(1);
		codigoTF.setMaxLength(20);
		codigoTF.setEmptyText(getI18nConstants().preenchaO() + getI18nConstants().codigo());
		formPanel.add(codigoTF, formData);

		nomeTF = new TextField<String>();
		nomeTF.setName(UnidadeDTO.PROPERTY_NOME);
		nomeTF.setValue(unidadeDTO.getNome());
		nomeTF.setFieldLabel(getI18nConstants().nome());
		nomeTF.setAllowBlank(false);
		nomeTF.setMinLength(1);
		nomeTF.setMaxLength(50);
		nomeTF.setEmptyText( getI18nConstants().preenchaO() + getI18nConstants().nome() );
		nomeTF.setToolTip("Uma unidade representa um prédio ou bloco do campus. Um campus pode conter uma ou mais unidades");
		formPanel.add(nomeTF, formData);

		campusCB = new CampusComboBox(cenarioDTO);
		campusCB.setName("campus");
		campusCB.setFieldLabel(getI18nConstants().campus());
		campusCB.setAllowBlank(false);
		campusCB.setValue(campusDTO);
		campusCB.setEmptyText(getI18nConstants().selecioneO() + getI18nConstants().campus());
		formPanel.add(campusCB, formData);
		
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
	public TextField<String> getNomeTextField()
	{
		return nomeTF;
	}

	@Override
	public SimpleModal getSimpleModal()
	{
		return simpleModal;
	}

	@Override
	public TextField<String> getCodigoTextField()
	{
		return codigoTF;
	}

	@Override
	public CampusComboBox getCampusComboBox()
	{
		return campusCB;
	}

	@Override
	public UnidadeDTO getUnidadeDTO()
	{
		return unidadeDTO;
	}
}
