package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.TipoCursoFormPresenter;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.TipoCursoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class TipoCursoFormView extends MyComposite
	implements TipoCursoFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField< String > codigoTF;
	private TextField< String > descricaoTF;
	private TipoCursoDTO tipoCursoDTO;

	public TipoCursoFormView( InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		TipoCursoDTO tipoCursoDTO )
	{
		this.tipoCursoDTO = tipoCursoDTO;
		this.tipoCursoDTO.setInstituicaoEnsinoId( instituicaoEnsinoDTO.getId() );
		this.tipoCursoDTO.setInstituicaoEnsinoString( instituicaoEnsinoDTO.getNomeInstituicao() );

		initUI();
	}

	private void initUI()
	{
		String title = ( ( tipoCursoDTO.getId() == null ) ?
			"Inserção de Tipo de Curso" : "Edição de Tipo de Curso" );

		simpleModal = new SimpleModal( title, Resources.DEFAULTS.curso16() );
		simpleModal.setHeight( 138 );
		createForm();
		simpleModal.setContent( formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);

		codigoTF = new TextField<String>();
		codigoTF.setName(TipoCursoDTO.PROPERTY_CODIGO);
		codigoTF.setValue(tipoCursoDTO.getCodigo());
		codigoTF.setFieldLabel("Código");
		codigoTF.setAllowBlank(false);
		codigoTF.setMaxLength(50);
		codigoTF.setEmptyText("Preencha o código");
		formPanel.add(codigoTF, formData);

		descricaoTF = new TextField<String>();
		descricaoTF.setName(TipoCursoDTO.PROPERTY_DESCRICAO);
		descricaoTF.setValue(tipoCursoDTO.getDescricao());
		descricaoTF.setFieldLabel("Descrição");
		descricaoTF.setMaxLength(255);
		descricaoTF.setEmptyText("Preencha uma descrição");
		formPanel.add(descricaoTF, formData);

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
	public TipoCursoDTO getTipoCursoDTO()
	{
		return tipoCursoDTO;
	}
}
