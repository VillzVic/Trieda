package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.TabItem;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;

public class GTabItem extends TabItem {

	private ContentPanel panel;
	
	public GTabItem() {
		configuration();
	}

	public GTabItem(String text) {
		super(text);
		configuration();
	}
	
	private void configuration() {
		setClosable(true);
		setLayout(new FitLayout());
		panel = new ContentPanel(new BorderLayout());
		panel.setHeaderVisible(false);
		add(panel);
	}
	
	public void setTitle(String s) {
		super.setTitle(s);
		panel.setHeading(s);
		panel.setHeaderVisible(s != null);
	}
	
	public void setContent(Component widget) {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
		panel.add(widget, bld);
	}
	
}
