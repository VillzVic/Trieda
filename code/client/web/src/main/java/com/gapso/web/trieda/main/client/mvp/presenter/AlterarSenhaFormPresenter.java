package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.AlterarSenhaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.services.UsuariosServiceAsync;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class AlterarSenhaFormPresenter
	implements Presenter
{
	public interface Display
	{
		Button getSalvarButton();
		TextField<String> getSenhaAntigaTextField();
		TextField<String> getNovaSenhaTextField();
		TextField<String> getNovaSenhaConfTextField();
		boolean isValid();
		SimpleModal getSimpleModal();
		TriedaI18nConstants getI18nConstants();
		TriedaI18nMessages getI18nMessages();
		UsuarioDTO getUsuarioDTO();
	}

	private Display display;

	public AlterarSenhaFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO, Display display )
	{
		this.display = display;

		setListeners();
	}

	private void setListeners()
	{
		this.display.getSalvarButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				if ( isValid() )
				{
					final UsuariosServiceAsync service = Services.usuarios();

					service.changePassword( display.getUsuarioDTO(), getDTO(), new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							if (caught instanceof TriedaException)
							{
								MessageBox.alert( "ERRO!",
										caught.getMessage(), null );
							}
							else
							{
								MessageBox.alert( "ERRO!",
										"Deu falha na conexão", null );
							}
						}

						@Override
						public void onSuccess( Void result )
						{
							display.getSimpleModal().hide();
							
							MessageBox.alert( "ERRO!",
									"Senha Alterada com sucesso!", null );
						}
					});
				}
				else
				{
					MessageBox.alert( "ERRO!",
						"Verifique os campos digitados", null );
				}
			}
		});
	}

	private boolean isValid()
	{
		return this.display.isValid();
	}

	private AlterarSenhaDTO getDTO()
	{
		AlterarSenhaDTO alterarSenhaDTO = new AlterarSenhaDTO();

		alterarSenhaDTO.setSenhaAntiga(TriedaUtil.toMD5(this.display.getSenhaAntigaTextField().getValue()));
		alterarSenhaDTO.setSenhaNova(TriedaUtil.toMD5(this.display.getNovaSenhaTextField().getValue()));
		alterarSenhaDTO.setSenhaNovaConf(TriedaUtil.toMD5(this.display.getNovaSenhaConfTextField().getValue()));

		return alterarSenhaDTO;
	}

	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}