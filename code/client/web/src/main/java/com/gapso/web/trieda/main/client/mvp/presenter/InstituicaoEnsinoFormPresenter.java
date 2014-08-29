package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.InstituicaoEnsinoServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.Widget;

public class InstituicaoEnsinoFormPresenter
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		Button getSalvarButton();
		TextField< String > getNomeTextField();
		InstituicaoEnsinoDTO getInstituicaoEnsinoDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
	}
	
	private SimpleGrid< InstituicaoEnsinoDTO > gridPanel;
	private Display display;
	
	public InstituicaoEnsinoFormPresenter( Display display, SimpleGrid< InstituicaoEnsinoDTO > gridPanel )
	{
		this.gridPanel = gridPanel;
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
					final InstituicaoEnsinoServiceAsync service = Services.instituicoesEnsino();
	
					service.save( getDTO(), new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display.getI18nMessages().erroAoSalvar("Instituição de Ensino"),display)
					{
						@Override
						public void onSuccess( Void result )
						{
							display.getSimpleModal().hide();
							gridPanel.updateList();
	
							Info.display( "Salvo",
								"Item salvo com sucesso!" );
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
	
	private InstituicaoEnsinoDTO getDTO()
	{
		InstituicaoEnsinoDTO instituicaoEnsinoDTO = this.display.getInstituicaoEnsinoDTO();
	
		instituicaoEnsinoDTO.setInstituicaoEnsinoId( this.display.getInstituicaoEnsinoDTO().getId() );
		instituicaoEnsinoDTO.setNomeInstituicao( this.display.getNomeTextField().getValue() );
	
		return instituicaoEnsinoDTO;
	}
	
	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}