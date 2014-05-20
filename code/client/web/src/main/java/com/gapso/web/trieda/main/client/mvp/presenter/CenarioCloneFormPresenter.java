package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CenariosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CenarioCloneFormPresenter
	implements Presenter
{
	public interface Display
	{
		Button getSalvarButton();
		CheckBox getOficialCheckBox();
		TextField< String > getNomeTextField();
		NumberField getAnoTextField();
		NumberField getSemestreTextField();
		TextField< String > getComentarioTextField();
		CenarioDTO getCenarioDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private SimpleGrid< CenarioDTO > gridPanel;
	private Display display;
	
	public CenarioCloneFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		Display display, SimpleGrid< CenarioDTO > gridPanel )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
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
					final CenariosServiceAsync service = Services.cenarios();

					service.clonar( getDTO(), getDTO(), display.getOficialCheckBox().getValue(), new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!",
								"Deu falha na conexão", null);
						}

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

	private CenarioDTO getDTO()
	{
		CenarioDTO cenarioDTO = this.display.getCenarioDTO();

		cenarioDTO.setInstituicaoEnsinoId( this.instituicaoEnsinoDTO.getId() );
		cenarioDTO.setMasterData( false );
		cenarioDTO.setOficial( this.display.getOficialCheckBox().getValue() );
		cenarioDTO.setNome( this.display.getNomeTextField().getValue() );
		cenarioDTO.setAno( this.display.getAnoTextField().getValue().intValue() );
		cenarioDTO.setSemestre( this.display.getSemestreTextField().getValue().intValue() );
		cenarioDTO.setComentario( this.display.getComentarioTextField().getValue() );

		return cenarioDTO;
	}

	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
