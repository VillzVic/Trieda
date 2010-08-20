package com.gapso.web.trieda.client.mvc.view;

import com.extjs.gxt.ui.client.Registry;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.extjs.gxt.ui.client.mvc.View;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Viewport;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GToolBar;
import com.gapso.web.trieda.client.util.view.GTreePanel;
import com.google.gwt.user.client.ui.RootPanel;

public class AppView extends View {

	public static final String TOOLBAR = "toolBar";
	public static final String TAB = "tabPanel";
	
	private Viewport viewport;
	private ContentPanel panel;
	private GToolBar toolBar;
	private GTab tab;
	private GTreePanel treePanel;
	
	public AppView(Controller controller) {
		super(controller);
	}

	@Override
	public void initialize() {
	}
	
	private void initUI() {
		viewport = new Viewport();
		viewport.setLayout(new FitLayout());
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Trieda");
		viewport.add(panel);
		
		createNorth();
		createWest();
		createCenter();
		
		Registry.register(TOOLBAR, toolBar);
		Registry.register(TAB, tab);
		
		RootPanel.get().add(viewport);
	}
	
	private void createNorth() {
		toolBar = new GToolBar();
		panel.setTopComponent(toolBar);
	}
	
	private void createWest() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.WEST);
	    bld.setMargins(new Margins(5, 0, 5, 5));
	    bld.setCollapsible(true);
	    bld.setFloatable(true);
	    
	    treePanel = new GTreePanel();
	    panel.add(treePanel, bld);
	}
	
	private void createCenter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    tab = new GTab();
	    panel.add(tab, bld);
	}
	
	@Override
	protected void handleEvent(AppEvent event) {
		if (event.getType() == AppEvents.Init) {
			initUI();
		}
	}

}
