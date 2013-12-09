package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.form.Validator;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.AlterarSenhaFormPresenter;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class AlterarSenhaFormView
 extends MyComposite
	implements AlterarSenhaFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> senhaAntigaTF;
	private TextField<String> novaSenhaTF;
	private TextField<String> novaSenhaConfTF;
	private UsuarioDTO usuarioDTO;
	
	public AlterarSenhaFormView( UsuarioDTO usuarioDTO )
	{
		this.usuarioDTO = usuarioDTO;

		initUI();
	}

	private void initUI()
	{
		String title = "Alterar Senha do Usuário: " + usuarioDTO.getNome();

		simpleModal = new SimpleModal( title, Resources.DEFAULTS.senha16() );
		simpleModal.setHeight( 160 );
		createForm();
		simpleModal.setContent( formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);

		senhaAntigaTF = new TextField<String>();
		senhaAntigaTF.setName(UnidadeDTO.PROPERTY_NOME);
		senhaAntigaTF.setFieldLabel("Senha Antiga");
		senhaAntigaTF.setLabelStyle("width: 95px");
		senhaAntigaTF.setAllowBlank(false);
		senhaAntigaTF.setMinLength(1);
		senhaAntigaTF.setMaxLength(50);
		senhaAntigaTF.setPassword( true );
		formPanel.add(senhaAntigaTF, formData);

		novaSenhaTF = new TextField<String>();
		novaSenhaTF.setName(UnidadeDTO.PROPERTY_NOME);
		novaSenhaTF.setFieldLabel("Nova Senha");
		novaSenhaTF.setLabelStyle("width: 95px");
		novaSenhaTF.setAllowBlank(false);
		novaSenhaTF.setMinLength(5);
		novaSenhaTF.setMaxLength(50);
		novaSenhaTF.setPassword( true );
		formPanel.add(novaSenhaTF, formData);
		
		novaSenhaConfTF = new TextField<String>();
		novaSenhaConfTF.setName(UnidadeDTO.PROPERTY_NOME);
		novaSenhaConfTF.setFieldLabel("Confirmar Senha");
		novaSenhaConfTF.setLabelStyle("width: 95px");
		novaSenhaConfTF.setAllowBlank(false);
		novaSenhaConfTF.setMinLength(5);
		novaSenhaConfTF.setMaxLength(50);
		novaSenhaConfTF.setPassword( true );
		novaSenhaConfTF.setValidator( new Validator()
		{
			@Override
			public String validate( Field< ? > field, String value )
			{
				if ( novaSenhaTF.getValue().equals(novaSenhaConfTF.getValue()) )
				{
					return null;
				}
				else
				{
					return "A nova senha e a confirmação estão diferentes.";
				}
			}
		});
		formPanel.add(novaSenhaConfTF, formData);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
		
		simpleModal.setFocusWidget(senhaAntigaTF);
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
	public TextField<String> getSenhaAntigaTextField()
	{
		return senhaAntigaTF;
	}
	
	@Override
	public TextField<String> getNovaSenhaTextField()
	{
		return novaSenhaTF;
	}
	
	@Override
	public TextField<String> getNovaSenhaConfTextField()
	{
		return novaSenhaConfTF;
	}

	@Override
	public SimpleModal getSimpleModal()
	{
		return simpleModal;
	}

	@Override
	public UsuarioDTO getUsuarioDTO()
	{
		return usuarioDTO;
	}
}
