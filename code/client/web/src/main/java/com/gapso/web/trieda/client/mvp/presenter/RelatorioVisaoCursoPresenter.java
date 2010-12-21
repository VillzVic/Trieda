package com.gapso.web.trieda.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.services.CurriculosServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.CurriculoComboBox;
import com.gapso.web.trieda.client.util.view.CursoComboBox;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.GradeHorariaCursoGrid;
import com.gapso.web.trieda.client.util.view.TurnoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class RelatorioVisaoCursoPresenter implements Presenter {

	public interface Display {
		Button getSubmitBuscaButton();
		CursoComboBox getCursoComboBox();
		CurriculoComboBox getCurriculoComboBox();
		TurnoComboBox getTurnoComboBox();
		SimpleComboBox<Integer> getPeriodoComboBox();
		GradeHorariaCursoGrid getGrid();
		Component getComponent();
	}
	private Display display; 
	
	public RelatorioVisaoCursoPresenter(CenarioDTO cenario, Display display) {
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSubmitBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().setCurriculoDTO(display.getCurriculoComboBox().getValue());
				display.getGrid().setPeriodo(display.getPeriodoComboBox().getValue().getValue());
				display.getGrid().setTurnoDTO(display.getTurnoComboBox().getValue());
				display.getGrid().requestAtendimentos();
			}
		});
		display.getCurriculoComboBox().addSelectionChangedListener(new SelectionChangedListener<CurriculoDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<CurriculoDTO> se) {
				final CurriculoDTO curriculoDTO = se.getSelectedItem();
				display.getPeriodoComboBox().getStore().removeAll();
				display.getPeriodoComboBox().setEnabled(curriculoDTO != null);
				if(curriculoDTO != null) {
					CurriculosServiceAsync service = Services.curriculos();
					service.getPeriodos(curriculoDTO, new AsyncCallback<List<Integer>>() {
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert("Erro", "Erro no servidor ao pegar os períodos da Matriz Curricular", null);
						}
						@Override
						public void onSuccess(List<Integer> result) {
							display.getPeriodoComboBox().add(result);
						}
					});
				}
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		GTab tab = (GTab)widget;
		tab.add((GTabItem)display.getComponent());
	}

}
