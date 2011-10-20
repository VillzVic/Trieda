package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.services.TurnosServiceAsync;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class TurnoFormPresenter
	implements Presenter
{
	public interface Display
	{
		Button getSalvarButton();
		TextField< String > getNomeTextField();
		NumberField getTempoTextField();
		TurnoDTO getTurnoDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private SimpleGrid< TurnoDTO > gridPanel;
	private Display display;

	public TurnoFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display,
		SimpleGrid< TurnoDTO > gridPanel )
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
					final TurnosServiceAsync service = Services.turnos();

					service.save( getDTO(), new AsyncCallback< Void >()
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
	
	private TurnoDTO getDTO()
	{
		TurnoDTO turnoDTO = this.display.getTurnoDTO();

		turnoDTO.setInstituicaoEnsinoId( this.instituicaoEnsinoDTO.getId() );
		turnoDTO.setNome( this.display.getNomeTextField().getValue() );
		turnoDTO.setTempo( this.display.getTempoTextField().getValue().intValue() );

		return turnoDTO;
	}
	
	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
