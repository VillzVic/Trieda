package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.SalasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TipoSalaComboBox;
import com.gapso.web.trieda.shared.util.view.UnidadeComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class SalaFormPresenter implements Presenter
{
	public interface Display
	{
		Button getSalvarButton();
		TextField<String> getCodigoTextField();
		TextField<String> getNumeroTextField();
		TextField<String> getAndarTextField();
		NumberField getCapacidadeNumberField();
		UnidadeComboBox getUnidadeComboBox();
		TipoSalaComboBox getTipoComboBox();
		SalaDTO getSalaDTO();
		boolean isValid();

		SimpleModal getSimpleModal();
	}

	private SimpleGrid< SalaDTO > gridPanel;
	private Display display;

	public SalaFormPresenter( Display display )
	{
		this( display, null );
	}

	public SalaFormPresenter( Display display, SimpleGrid< SalaDTO > gridPanel )
	{
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
							final SalasServiceAsync service = Services.salas();
							service.save( getDTO(), new AsyncCallback< Void >()
							{
								@Override
								public void onFailure( Throwable caught )
								{
									MessageBox.alert( "ERRO!", "Deu falha na conexão : ", null );
								}

								@Override
								public void onSuccess( Void result )
								{
									display.getSimpleModal().hide();
									if ( gridPanel != null )
									{
										gridPanel.updateList();
									}

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

	private SalaDTO getDTO()
	{
		SalaDTO salaDTO = display.getSalaDTO();

		salaDTO.setCodigo( display.getCodigoTextField().getValue() );
		salaDTO.setNumero( display.getNumeroTextField().getValue() );
		salaDTO.setAndar( display.getAndarTextField().getValue() );
		salaDTO.setCapacidade( display.getCapacidadeNumberField().getValue().intValue() );
		salaDTO.setTipoId( display.getTipoComboBox().getSelection().get( 0 ).getId() );
		salaDTO.setTipoString( display.getTipoComboBox().getSelection().get( 0 ).getNome() );
		salaDTO.setUnidadeId( display.getUnidadeComboBox().getSelection().get( 0 ).getId() );
		salaDTO.setUnidadeString( display.getUnidadeComboBox().getSelection().get( 0 ).getCodigo() );

		return salaDTO;
	}

	@Override
	public void go( Widget widget )
	{
		display.getSimpleModal().show();
	}
}
