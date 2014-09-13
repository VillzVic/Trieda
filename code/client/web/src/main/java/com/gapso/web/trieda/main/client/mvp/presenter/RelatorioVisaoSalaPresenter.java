package com.gapso.web.trieda.main.client.mvp.presenter;


import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.services.SalasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaSalaGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoPresenter;
import com.gapso.web.trieda.shared.util.view.SalaAutoCompleteBox;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class RelatorioVisaoSalaPresenter extends RelatorioVisaoPresenter{
	public interface Display extends RelatorioVisaoPresenter.Display{
		SalaAutoCompleteBox getSalaTextField();
		GradeHorariaSalaGrid getGrid();
		Button getAntAmbienteButton();
		Button getProxAmbienteButton();
	}

	public RelatorioVisaoSalaPresenter(InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display)
	{
		super(instituicaoEnsinoDTO, cenario, display);
		setButtonsListener();
	}
	
	public void setButtonsListener()
	{
		final Display display = (Display) this.getDisplay();
		
		display.getSalaTextField().addSelectionChangedListener(
			new SelectionChangedListener< SalaDTO >()
		{
			@Override
			public void selectionChanged(
				SelectionChangedEvent< SalaDTO > se )
			{
				final SalaDTO salaDTO = se.getSelectedItem();
				if ( salaDTO == null )
				{
					display.getProxAmbienteButton().disable();
					display.getAntAmbienteButton().disable();
				}
			}
		});
		
		this.display.getSubmitBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce){
				final SalaDTO salaDTO = display.getSalaTextField().getValue();
				if ( salaDTO == null )
				{
					display.getProxAmbienteButton().disable();
					display.getAntAmbienteButton().disable();
				}
				else
				{
					display.getProxAmbienteButton().enable();
					display.getAntAmbienteButton().enable();
				}
			}
		});
		
		display.getProxAmbienteButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				if (display.getSalaTextField().getValue() != null)
				{
					SalasServiceAsync service = Services.salas();

					service.getProxSala(cenarioDTO, display.getSalaTextField().getValue(), new AsyncCallback< SalaDTO >()
					{
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert( "ERRO!", "Não foi possível obter o próximo ambiente", null );
							
						}
						@Override
						public void onSuccess(SalaDTO result) {
							display.getSalaTextField().getStore().add(result);
							display.getSalaTextField().setValue(result);
							if (display.getSalaTextField().getValue() != null)
							{
								display.getProxAmbienteButton().enable();
								display.getAntAmbienteButton().enable();
								display.getSubmitBuscaButton().el().click();
								
							}
							else
							{
								display.getProxAmbienteButton().disable();
								display.getAntAmbienteButton().disable();
							}
						}
					});
				}
			}
		});
		
		display.getAntAmbienteButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				if (display.getSalaTextField().getValue() != null)
				{
					SalasServiceAsync service = Services.salas();

					service.getAntSala(cenarioDTO, display.getSalaTextField().getValue(), new AsyncCallback< SalaDTO >()
					{
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert( "ERRO!", "Não foi possível obter o próximo ambiente", null );
							
						}
						@Override
						public void onSuccess(SalaDTO result) {
							display.getSalaTextField().getStore().add(result);
							display.getSalaTextField().setValue(result);
							if (display.getSalaTextField().getValue() != null)
							{
								display.getProxAmbienteButton().enable();
								display.getAntAmbienteButton().enable();
								display.getSubmitBuscaButton().el().click();
								
							}
							else
							{
								display.getProxAmbienteButton().disable();
								display.getAntAmbienteButton().disable();
							}
						}
					});
				}
			}
		});
	}

}
