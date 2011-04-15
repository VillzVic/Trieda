package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.main.client.util.view.CampusComboBox;
import com.gapso.web.trieda.main.client.util.view.GTab;
import com.gapso.web.trieda.main.client.util.view.GTabItem;
import com.gapso.web.trieda.main.client.util.view.GradeHorariaProfessorGrid;
import com.gapso.web.trieda.main.client.util.view.ProfessorComboBox;
import com.gapso.web.trieda.main.client.util.view.TurnoComboBox;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.google.gwt.user.client.ui.Widget;

public class RelatorioVisaoProfessorPresenter implements Presenter {

	public interface Display {
		Button getSubmitBuscaButton();
		CampusComboBox getCampusComboBox();
		TurnoComboBox getTurnoComboBox();
		ProfessorComboBox getProfessorComboBox();
		GradeHorariaProfessorGrid getGrid();
		Component getComponent();
	}
	private Display display; 
	
	public RelatorioVisaoProfessorPresenter(CenarioDTO cenario, Display display) {
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSubmitBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().setProfessorDTO(display.getProfessorComboBox().getValue());
				display.getGrid().setTurnoDTO(display.getTurnoComboBox().getValue());
				display.getGrid().requestAtendimentos();
			}
		});

	}
	
	@Override
	public void go(Widget widget) {
		GTab tab = (GTab)widget;
		tab.add((GTabItem)display.getComponent());
	}

}
