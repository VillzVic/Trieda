package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.RelatorioAlunoFiltro;
import com.gapso.web.trieda.shared.util.view.RelatorioPresenter;

public class RelatorioAlunosPresenter extends RelatorioPresenter
	implements Presenter
{
	public interface Display extends RelatorioPresenter.Display
	{
		RelatorioAlunoFiltro getAlunoFiltro();
		CursoComboBox getCursoComboBox();
		NumberField getPeriodoComboBox();
		CheckBox getFormandoCheckBox();
	}
	
	
	public RelatorioAlunosPresenter(InstituicaoEnsinoDTO instituicaoEnsinoDTO,
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
					display.getAlunoFiltro().setCurso(display.getCursoComboBox().getValue());
				}
				if (display.getPeriodoComboBox().getValue() != null){
					display.getAlunoFiltro().setPeriodo(display.getPeriodoComboBox().getValue().intValue());
				}
				if (display.getFormandoCheckBox().getValue() != null && display.getFormandoCheckBox().getValue()){
					display.getAlunoFiltro().setFormando(display.getFormandoCheckBox().getValue());
				}
				
				display.getTree().mask( display.getI18nMessages().loading() );
				Services.alunos().getRelatorio(cenarioDTO, display.getAlunoFiltro(), null,
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
				display.getPeriodoComboBox().setValue(null);
				display.getFormandoCheckBox().setValue(null);
			}
		});
	}
}
