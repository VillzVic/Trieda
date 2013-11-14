package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.gapso.web.trieda.main.client.mvp.view.AmbientesFaixaUtilizacaoHorariosView;
import com.gapso.web.trieda.main.client.mvp.view.AmbientesFaixaOcupacaoCapacidadeView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.FaixaCapacidadeSalaComboBox;
import com.gapso.web.trieda.shared.util.view.RelatorioPresenter;
import com.gapso.web.trieda.shared.util.view.RelatorioSalaFiltro;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;

public class RelatorioSalasPresenter  extends RelatorioPresenter
	implements Presenter
{
	public interface Display extends RelatorioPresenter.Display
	{
		RelatorioSalaFiltro getSalaFiltro();
		TurnoComboBox getTurnoComboBox();
		FaixaCapacidadeSalaComboBox getFaixaCapacidadeSalaComboBox();
	}
	
	
	public RelatorioSalasPresenter(InstituicaoEnsinoDTO instituicaoEnsinoDTO,
			CenarioDTO cenarioDTO, Display display) {
		super(instituicaoEnsinoDTO, cenarioDTO, display);
		setListeners();
	}
	
	private void setListeners()
	{
		final Display display = (Display) this.getDisplay();
		
		display.getSubmitBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce){
				if (display.getTurnoComboBox().getValue() != null){
					display.getSalaFiltro().setTurno(display.getTurnoComboBox().getValue());
				}
				if (display.getFaixaCapacidadeSalaComboBox().getValue() != null){
					display.getSalaFiltro().setFaixaCapacidadeSala(display.getFaixaCapacidadeSalaComboBox().getValue());
				}
				
				display.getTree().mask( display.getI18nMessages().loading() );
				Services.salas().getRelatorio(cenarioDTO, display.getSalaFiltro(), null,
					new AbstractAsyncCallbackWithDefaultOnFailure< List< RelatorioDTO > >( display )
					{
						@Override
						public void onSuccess( List< RelatorioDTO > list )
						{
							display.getStore().removeAll();
							display.getStore().add( list, true );
							display.getTree().unmask();
						}
					});
			}
		});
		
		display.getResetBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce){
				display.getTurnoComboBox().setValue(null);
				display.getFaixaCapacidadeSalaComboBox().setValue(null);
				
				display.getTree().mask( display.getI18nMessages().loading() );
				Services.salas().getRelatorio(cenarioDTO, display.getSalaFiltro(), null,
					new AbstractAsyncCallbackWithDefaultOnFailure< List< RelatorioDTO > >( display )
					{
						@Override
						public void onSuccess( List< RelatorioDTO > list )
						{
							display.getStore().removeAll();
							display.getStore().add( list, true );
							display.getTree().unmask();
						}
					});
			}
		});
		
		display.getHistogramasButton().get(0).addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce){
				Presenter presenter = new AmbientesFaixaUtilizacaoHorariosPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new AmbientesFaixaUtilizacaoHorariosView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		display.getHistogramasButton().get(1).addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce){
				Presenter presenter = new AmbientesFaixaOcupacaoCapacidadePresenter( instituicaoEnsinoDTO,
						cenarioDTO, new AmbientesFaixaOcupacaoCapacidadeView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
	}
}