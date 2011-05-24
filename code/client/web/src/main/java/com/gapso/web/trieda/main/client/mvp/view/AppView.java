package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.ButtonArrowAlign;
import com.extjs.gxt.ui.client.Style.ButtonScale;
import com.extjs.gxt.ui.client.Style.IconAlign;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Viewport;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.AppPresenter;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CenarioPanel;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.ui.AbstractImagePrototype;
import com.google.gwt.user.client.ui.Widget;

public class AppView extends MyComposite implements AppPresenter.Display {

	private Viewport viewport;
	private ContentPanel panel;
	
	private GTab tab;
	private CenarioPanel cenarioPanel;
	
	public AppView() {
		initUI();
	}
	
	private void initUI() {
		viewport = new Viewport();
		viewport.setLayout(new FitLayout());
		panel = new ContentPanel(new BorderLayout());
		panel.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.logo()));
		panel.getHeader().addTool(getLogoutButton());
		viewport.add(panel);
		
		createWest();
		createCenter();
		
		initComponent(viewport);
	}
		
	private void createWest() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.WEST);
	    bld.setMargins(new Margins(5, 0, 5, 5));
	    bld.setCollapsible(true);
	    bld.setFloatable(true);
	    
	    cenarioPanel = new CenarioPanel();
//	    panel.add(cenarioPanel, bld);
	}
	
	private void createCenter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    tab = new GTab();
	    panel.add(tab, bld);
	}

	public Button getLogoutButton() {
		Button b = createButton("Bom dia, Claudio Escudero", "Sair", Resources.DEFAULTS.logout16());
		b.addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				 Window.open("../resources/j_spring_security_logout"+TriedaUtil.paramsDebug(), "_self", "");
			}
		});
		return b;
	}
	
	private Button createButton(String text, String toolTip, ImageResource icon) {
		Button bt = new Button(text, AbstractImagePrototype.create(icon));
		bt.setToolTip(toolTip);
		bt.setScale(ButtonScale.MEDIUM);
		bt.setIconAlign(IconAlign.RIGHT);
		bt.setArrowAlign(ButtonArrowAlign.BOTTOM);
		bt.setHeight(24);
		return bt;
	}
	
	@Override
	public ContentPanel getPanel() {
		return panel;
	}
	
	@Override
	public GTab getGTab() {
		return tab;
	}

	@Override
	public Widget asWidget() {
		return this;
	}

	@Override
	public CenarioPanel getCenarioPanel() {
		return cenarioPanel;
	}
	
}
