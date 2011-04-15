package com.gapso.web.professor.client.mvp.view;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Viewport;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.professor.client.mvp.presenter.AppPresenter;
import com.gapso.web.trieda.main.client.util.resources.Resources;
import com.gapso.web.trieda.main.client.util.view.GTab;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.google.gwt.user.client.ui.AbstractImagePrototype;
import com.google.gwt.user.client.ui.Widget;

public class AppView extends MyComposite implements AppPresenter.Display {

	private Viewport viewport;
	private ContentPanel panel;
	
	private GTab tab;
	
	public AppView() {
		initUI();
	}
	
	private void initUI() {
		viewport = new Viewport();
		viewport.setLayout(new FitLayout());
		panel = new ContentPanel(new BorderLayout());
		panel.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.logo()));
		viewport.add(panel);
		
		createCenter();
		
		initComponent(viewport);
	}
	
	private void createCenter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    tab = new GTab();
	    panel.add(tab, bld);
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

}
