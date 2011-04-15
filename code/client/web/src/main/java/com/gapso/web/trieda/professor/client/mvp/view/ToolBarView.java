package com.gapso.web.trieda.professor.client.mvp.view;

import com.extjs.gxt.ui.client.Style.ButtonArrowAlign;
import com.extjs.gxt.ui.client.Style.ButtonScale;
import com.extjs.gxt.ui.client.Style.IconAlign;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.main.client.util.resources.Resources;
import com.gapso.web.trieda.professor.client.mvp.presenter.ToolBarPresenter;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class ToolBarView extends MyComposite implements ToolBarPresenter.Display {

	private LayoutContainer container;
	private ToolBar toolBar;
	
	private Button professoresCampusListprofessoresBt;
	private Button relatorioVisaoProfessorBt;
	
	public ToolBarView() {
		initUI();
	}
	
	private void initUI() {
		toolBar = new ToolBar();
		createButtons();
		
		container = new LayoutContainer(new BorderLayout());
		container.setHeight(65);
		
		BorderLayoutData centerData = new BorderLayoutData(LayoutRegion.CENTER);
		container.add(toolBar, centerData);
		
		initComponent(container);
	}
	

	/********************************
	 * CRIAÇÃO DOS BOTÕES
	 */
	
	private void createButtons() {
		professoresCampusListprofessoresBt = createButton("Campi de<br />Trabalho", "Campi de Trabalho", Resources.DEFAULTS.campiTrabalho24());
		toolBar.add(professoresCampusListprofessoresBt);
		
		relatorioVisaoProfessorBt = createButton("Grade Horária<br />Visão Professor", "Grade Horária Visão Professor", Resources.DEFAULTS.saidaProfessor24());
		toolBar.add(relatorioVisaoProfessorBt);		
	}
	
	
	private Button createButton(String text, String toolTip, ImageResource icon) {
		Button bt = new Button(text, AbstractImagePrototype.create(icon));
		bt.setToolTip(toolTip);
		bt.setScale(ButtonScale.MEDIUM);
		bt.setIconAlign(IconAlign.TOP);
		bt.setArrowAlign(ButtonArrowAlign.BOTTOM);
		bt.setHeight(60);
		return bt;
	}
	
	/********************************
	 * GET BUTTONS
	 */
	
	@Override
	public Button getProfessoresCampusListprofessoresBt() {
		return professoresCampusListprofessoresBt;
	}
	@Override
	public Button getRelatorioVisaoProfessorButton() {
		return relatorioVisaoProfessorBt;
	}
	
}
