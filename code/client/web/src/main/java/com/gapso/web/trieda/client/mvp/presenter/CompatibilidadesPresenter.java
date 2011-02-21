package com.gapso.web.trieda.client.mvp.presenter;

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
import com.gapso.web.trieda.client.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.DisciplinaIncompativelDTO;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.client.util.view.CurriculoComboBox;
import com.gapso.web.trieda.client.util.view.CursoComboBox;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.IncompatibilidadeGrid;
import com.google.gwt.user.client.ui.Widget;

public class CompatibilidadesPresenter implements Presenter {

	public interface Display extends ITriedaI18nGateway {
		Button getSalvarButton();
		Button getCancelarButton();
		CursoComboBox getCursoComboBox();
		CurriculoComboBox getCurriculoComboBox();
		SimpleComboBox<Integer> getPeriodoComboBox();
		IncompatibilidadeGrid getGrid();
		Component getComponent();
	}
	private Display display;
//	private CenarioDTO cenario;
	
	public CompatibilidadesPresenter(CenarioDTO cenario, Display display) {
		this.display = display;
//		this.cenario = cenario;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
	}
	
	private void setListeners() {

		display.getCurriculoComboBox().addSelectionChangedListener(new SelectionChangedListener<CurriculoDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<CurriculoDTO> se) {
				final CurriculoDTO curriculoDTO = se.getSelectedItem();
				display.getPeriodoComboBox().getStore().removeAll();
				display.getPeriodoComboBox().setEnabled(curriculoDTO != null);
				if(curriculoDTO != null) {
					Services.curriculos().getPeriodos(curriculoDTO, new AbstractAsyncCallbackWithDefaultOnFailure<List<Integer>>(display) {
						@Override
						public void onSuccess(List<Integer> result) {
							display.getPeriodoComboBox().add(result);
						}
					});
				}
			}
		});
	
		display.getPeriodoComboBox().addSelectionChangedListener(new SelectionChangedListener<SimpleComboValue<Integer>>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<SimpleComboValue<Integer>> se) {
				if(se.getSelectedItem() == null) return;
				Integer periodo = se.getSelectedItem().getValue();
				if(periodo == null) return;
				CurriculoDTO curriculoDTO = display.getCurriculoComboBox().getValue();
				Services.disciplinas().getDisciplinasIncompativeis(curriculoDTO, periodo, new AbstractAsyncCallbackWithDefaultOnFailure<List<DisciplinaIncompativelDTO>>(display) {
					@Override
					public void onSuccess(List<DisciplinaIncompativelDTO> list) {
						display.getGrid().updateList(list);
					}
				});
			}
		});
		
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<DisciplinaIncompativelDTO> list = display.getGrid().getModels();
				Services.disciplinas().saveDisciplinasIncompativeis(list, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
					@Override
					public void onSuccess(Void result) {
						Info.display("Salvo", "Incompatibilidade salvas com sucesso!");
					}
				});
			}
		});
		display.getCancelarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				Integer periodo = display.getPeriodoComboBox().getValue().getValue();
				CurriculoDTO curriculoDTO = display.getCurriculoComboBox().getValue();
				Services.disciplinas().getDisciplinasIncompativeis(curriculoDTO, periodo, new AbstractAsyncCallbackWithDefaultOnFailure<List<DisciplinaIncompativelDTO>>(display) {
					@Override
					public void onSuccess(List<DisciplinaIncompativelDTO> list) {
						display.getGrid().updateList(list);
					}
				});
			}
		});
	}

	
	@Override
	public void go(Widget widget) {
		GTab gTab = (GTab)widget;
		gTab.add((GTabItem)display.getComponent());
	}

}
