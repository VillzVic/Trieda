package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.form.SimpleComboValue;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaIncompativelDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.IncompatibilidadeGrid;
import com.google.gwt.user.client.ui.Widget;

public class CompatibilidadesPresenter
	implements Presenter
{
	public interface Display	
		extends ITriedaI18nGateway
	{
		Button getSalvarButton();
		Button getCancelarButton();
		CursoComboBox getCursoComboBox();
		CurriculoComboBox getCurriculoComboBox();
		SimpleComboBox< Integer > getPeriodoComboBox();
		IncompatibilidadeGrid getGrid();
		Component getComponent();
	}

	private Display display;

	public CompatibilidadesPresenter(
		CenarioDTO cenario, Display display )
	{
		this.display = display;

		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{

	}

	private void setListeners()
	{
		display.getPeriodoComboBox().addSelectionChangedListener(new SelectionChangedListener<SimpleComboValue<Integer>>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<SimpleComboValue<Integer>> se) {
				display.getGrid().mask(display.getI18nMessages().loading(), "loading");
				if(se.getSelectedItem() == null) return;
				Integer periodo = se.getSelectedItem().getValue();
				if(periodo == null) return;
				CurriculoDTO curriculoDTO = display.getCurriculoComboBox().getValue();
				Services.disciplinas().getDisciplinasIncompativeis(curriculoDTO, periodo, new AbstractAsyncCallbackWithDefaultOnFailure<List<DisciplinaIncompativelDTO>>(display) {
					@Override
					public void onSuccess(List<DisciplinaIncompativelDTO> list) {
						display.getGrid().updateList(list);
						display.getGrid().unmask();
					}
				});
			}
		});

		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().mask(display.getI18nMessages().loading(), "loading");
				List<DisciplinaIncompativelDTO> list = display.getGrid().getModels();
				Services.disciplinas().saveDisciplinasIncompativeis(list, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
					@Override
					public void onSuccess(Void result) {
						Info.display(display.getI18nConstants().informacao(),display.getI18nMessages().sucessoSalvarNoBD(display.getI18nConstants().compatibilidadeEntreDisciplinas()));
						display.getGrid().unmask();
					}
				});
			}
		});

		display.getCancelarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().mask(display.getI18nMessages().loading(), "loading");
				Integer periodo = display.getPeriodoComboBox().getValue().getValue();
				CurriculoDTO curriculoDTO = display.getCurriculoComboBox().getValue();
				Services.disciplinas().getDisciplinasIncompativeis(curriculoDTO, periodo, new AbstractAsyncCallbackWithDefaultOnFailure<List<DisciplinaIncompativelDTO>>(display) {
					@Override
					public void onSuccess(List<DisciplinaIncompativelDTO> list) {
						display.getGrid().updateList(list);
						display.getGrid().unmask();
					}
				});
			}
		});
	}

	@Override
	public void go( Widget widget )
	{
		GTab gTab = (GTab)widget;
		gTab.add( (GTabItem)display.getComponent() );
	}
}
