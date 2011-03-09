package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.GradeHorariaSalaGrid;
import com.gapso.web.trieda.client.util.view.SalaComboBox;
import com.gapso.web.trieda.client.util.view.TurnoComboBox;
import com.gapso.web.trieda.client.util.view.UnidadeComboBox;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.google.gwt.user.client.ui.Widget;

public class RelatorioVisaoSalaPresenter implements Presenter {

	public interface Display {
		Button getSubmitBuscaButton();
		CampusComboBox getCampusComboBox();
		UnidadeComboBox getUnidadeComboBox();
		SalaComboBox getSalaComboBox();
		TurnoComboBox getTurnoComboBox();
		TextField<String> getCapacidadeTextField();
		TextField<String> getTipoTextField();
		GradeHorariaSalaGrid getGrid();
		Component getComponent();
	}
	private Display display; 
	
	public RelatorioVisaoSalaPresenter(CenarioDTO cenario, Display display) {
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSubmitBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().setSalaDTO(display.getSalaComboBox().getValue());
				display.getGrid().setTurnoDTO(display.getTurnoComboBox().getValue());
				display.getGrid().requestAtendimentos();
			}
		});
		display.getSalaComboBox().addSelectionChangedListener(new SelectionChangedListener<SalaDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<SalaDTO> se) {
				final SalaDTO salaDTO = se.getSelectedItem();
				if(salaDTO == null) {
					display.getCapacidadeTextField().setValue("");
					display.getTipoTextField().setValue("");
				} else {
					display.getCapacidadeTextField().setValue(salaDTO.getCapacidade().toString());
					display.getTipoTextField().setValue(salaDTO.getTipoString());
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
