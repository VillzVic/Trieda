package com.gapso.web.trieda.shared.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.GradeHorariaProfessorGrid;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.ProfessorVirtualComboBox;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.ui.Widget;

public class RelatorioVisaoProfessorPresenter implements Presenter {

	public interface Display {
		Button getSubmitBuscaButton();
		CampusComboBox getCampusComboBox();
		TurnoComboBox getTurnoComboBox();
		ProfessorComboBox getProfessorComboBox();
		GradeHorariaProfessorGrid getGrid();
		Component getComponent();
		ProfessorVirtualComboBox getProfessorVirtualComboBox();
	}
	private Display display; 
	private UsuarioDTO usuario; 
	
	public RelatorioVisaoProfessorPresenter(CenarioDTO cenario, UsuarioDTO usuario, Display display) {
		this.usuario = usuario;
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSubmitBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(usuario.isAdministrador()) {
					display.getGrid().setProfessorDTO(display.getProfessorComboBox().getValue());
					display.getGrid().setProfessorVirtualDTO(display.getProfessorVirtualComboBox().getValue());
				} else {
					ProfessorDTO professor = new ProfessorDTO();
					professor.setId(usuario.getProfessorId());
					display.getGrid().setProfessorDTO(professor);
				}
				display.getGrid().setTurnoDTO(display.getTurnoComboBox().getValue());
				display.getGrid().requestAtendimentos();
			}
		});
		display.getProfessorComboBox().addSelectionChangedListener(new SelectionChangedListener<ProfessorDTO>() {
			@Override
			public void selectionChanged(SelectionChangedEvent<ProfessorDTO> se) {
				display.getProfessorVirtualComboBox().setValue(null);
				display.getProfessorComboBox().setValue(se.getSelectedItem());
			}
		});
		display.getProfessorVirtualComboBox().addSelectionChangedListener(new SelectionChangedListener<ProfessorVirtualDTO>() {
			@Override
			public void selectionChanged(SelectionChangedEvent<ProfessorVirtualDTO> se) {
				display.getProfessorComboBox().setValue(null);
				display.getProfessorVirtualComboBox().setValue(se.getSelectedItem());
			}
		});

	}
	
	@Override
	public void go(Widget widget) {
		GTab tab = (GTab)widget;
		tab.add((GTabItem)display.getComponent());
	}

}
