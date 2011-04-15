package com.gapso.web.trieda.professor.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.CampusProfessoresPresenter;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.mvp.view.CampusProfessoresView;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.google.gwt.user.client.ui.Widget;

public class ToolBarPresenter implements Presenter {

	public interface Display {
		Component getComponent();
		Button getProfessoresCampusListprofessoresBt();
		Button getRelatorioVisaoProfessorButton();
	}
	
	private CenarioDTO masterData;
	private UsuarioDTO usuario;
	private Display toolBar;
	private GTab gTab;
	
	public ToolBarPresenter(CenarioDTO masterData, UsuarioDTO usuario, Display toolBar) {
		this.masterData = masterData;
		this.usuario = usuario;
		this.toolBar = toolBar;
		addListeners();
	}

	private void addListeners() {
		toolBar.getProfessoresCampusListprofessoresBt().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new CampusProfessoresPresenter(masterData, /*usuario, */new CampusProfessoresView(/*usuario*/));
				presenter.go(gTab);
			}
		});
		toolBar.getRelatorioVisaoProfessorButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
//				Presenter presenter = new RelatorioVisaoProfessorPresenter(masterData, new RelatorioVisaoProfessorView());
//				presenter.go(gTab);
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		AppPresenter.Display container = (AppPresenter.Display) widget;
		this.gTab = container.getGTab();
		container.getPanel().setTopComponent(toolBar.getComponent());
	}

}
