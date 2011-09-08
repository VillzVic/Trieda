package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.services.UsuariosServiceAsync;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.Widget;

public class UsuarioFormPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Button getSalvarButton();
		TextField< String > getNomeTextField();
		TextField< String > getEmailTextField();
		TextField< String > getUsernameTextField();
		TextField< String > getPasswordTextField();
		ProfessorComboBox getProfessorComboBox();
		UsuarioDTO getUsuarioDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
	}

	@SuppressWarnings("unused")
	private CenarioDTO cenario;

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private SimpleGrid< UsuarioDTO > gridPanel;
	private Display display;
	
	public UsuarioFormPresenter( InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display, SimpleGrid< UsuarioDTO > gridPanel )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenario = cenario;
		this.gridPanel = gridPanel;
		this.display = display;
		setListeners();
	}

	private void setListeners()
	{
		display.getSalvarButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					if ( isValid() )
					{
						final UsuariosServiceAsync service = Services.usuarios();
						service.save( getDTO(),
							new AbstractAsyncCallbackWithDefaultOnFailure< Void >( display )
							{
								@Override
								public void onSuccess( Void result )
								{
									display.getSimpleModal().hide();
									gridPanel.updateList();
									Info.display( "Salvo", "Item salvo com sucesso!" );
								}
							});
					}
					else
					{
						MessageBox.alert( "ERRO!", "Verifique os campos digitados", null );
					}
				}
			});
	}

	private boolean isValid()
	{
		return display.isValid();
	}

	private UsuarioDTO getDTO()
	{
		UsuarioDTO usuarioDTO = display.getUsuarioDTO();

		usuarioDTO.setNome( display.getNomeTextField().getValue());
		usuarioDTO.setEmail( display.getEmailTextField().getValue());
		usuarioDTO.setUsername( display.getUsernameTextField().getValue());
		usuarioDTO.setPassword( display.getPasswordTextField().getValue());
		usuarioDTO.setInstituicaoEnsinoId( instituicaoEnsinoDTO.getId() );
		ProfessorDTO professor = display.getProfessorComboBox().getValue();

		if ( professor != null )
		{
			usuarioDTO.setProfessorId( professor.getId() );
			usuarioDTO.setProfessorDisplayText( professor.getNome() );
		}

		return usuarioDTO;
	}
	
	@Override
	public void go( Widget widget )
	{
		display.getSimpleModal().show();
	}
}
