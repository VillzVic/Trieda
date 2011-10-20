package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.services.UnidadesServiceAsync;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class UnidadeFormPresenter
	implements Presenter
{
	public interface Display
	{
		Button getSalvarButton();
		TextField< String > getNomeTextField();
		TextField< String > getCodigoTextField();
		CampusComboBox getCampusComboBox();
		UnidadeDTO getUnidadeDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
		TriedaI18nConstants getI18nConstants();
		TriedaI18nMessages getI18nMessages();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private SimpleGrid< UnidadeDTO > gridPanel;
	private Display display;

	public UnidadeFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO, Display display )
	{
		this( instituicaoEnsinoDTO, display, null );
	}

	public UnidadeFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		Display display, SimpleGrid< UnidadeDTO > gridPanel )
	{
		this.gridPanel = gridPanel;
		this.display = display;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;

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
					final UnidadesServiceAsync service = Services.unidades();
					final UnidadeDTO dto = getDTO();

					service.save( dto, new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!",
								"Deu falha na conexão", null );
						}

						@Override
						public void onSuccess( Void result )
						{
							display.getSimpleModal().hide();

							if ( gridPanel != null )
							{
								gridPanel.updateList();
							}

							Info.display( display.getI18nConstants().informacao(),
								display.getI18nMessages().sucessoSalvarNoBD( dto.getCodigo() ) );
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

	private UnidadeDTO getDTO()
	{
		UnidadeDTO unidadeDTO = this.display.getUnidadeDTO();

		unidadeDTO.setNome( this.display.getNomeTextField().getValue() );
		unidadeDTO.setCodigo( this.display.getCodigoTextField().getValue() );
		unidadeDTO.setCampusId( this.display.getCampusComboBox().getSelection().get( 0 ).getId() );
		unidadeDTO.setCampusString( this.display.getCampusComboBox().getSelection().get( 0 ).getCodigo() );
		unidadeDTO.setInstituicaoEnsinoId( this.instituicaoEnsinoDTO.getId() );

		return unidadeDTO;
	}

	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
