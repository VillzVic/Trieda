package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.AreaTitulacaoComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.RelatorioPresenter;
import com.gapso.web.trieda.shared.util.view.RelatorioProfessorFiltro;
import com.gapso.web.trieda.shared.util.view.TipoContratoComboBox;
import com.gapso.web.trieda.shared.util.view.TitulacaoComboBox;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;

public class RelatorioProfessoresPresenter extends RelatorioPresenter
	implements Presenter
{
	public interface Display extends RelatorioPresenter.Display
	{
		RelatorioProfessorFiltro getProfessorFiltro();
		TurnoComboBox getTurnoComboBox();
		TitulacaoComboBox getTitulacaoComboBox();
		TipoContratoComboBox getTipoContratoComboBox();
		AreaTitulacaoComboBox getAreaTitulacaoComboBox();
		CursoComboBox getCursoComboBox();
	}
	
	
	public RelatorioProfessoresPresenter(InstituicaoEnsinoDTO instituicaoEnsinoDTO,
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
				if (display.getCursoComboBox().getValue() != null){
					display.getProfessorFiltro().setCurso(display.getCursoComboBox().getValue());
				}
				if (display.getTurnoComboBox().getValue() != null){
					display.getProfessorFiltro().setTurno(display.getTurnoComboBox().getValue());
				}
				if (display.getTitulacaoComboBox().getValue() != null){
					display.getProfessorFiltro().setTitulacao(display.getTitulacaoComboBox().getValue());
				}
				if (display.getTipoContratoComboBox().getValue() != null){
					display.getProfessorFiltro().setTipoContrato(display.getTipoContratoComboBox().getValue());
				}
				if (display.getAreaTitulacaoComboBox().getValue() != null){
					display.getProfessorFiltro().setAreaTitulacao(display.getAreaTitulacaoComboBox().getValue());
				}
				
				display.getTree().mask( display.getI18nMessages().loading() );
				Services.professores().getRelatorio(cenarioDTO, display.getProfessorFiltro(), null,
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
				display.getCursoComboBox().setValue(null);
				display.getTurnoComboBox().setValue(null);
				display.getTitulacaoComboBox().setValue(null);
				display.getAreaTitulacaoComboBox().setValue(null);
				display.getTipoContratoComboBox().setValue(null);
			}
		});
	}
}
