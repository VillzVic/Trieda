package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.TabItem;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class GTabItem extends TabItem {

	private ContentPanel panel;
	private String idTabItem;
	
	public GTabItem() {
		configuration();
	}

	public GTabItem(String text) {
		this(text, null);
	}
	public GTabItem(String text, ImageResource icon) {
		super(text);
		if(icon != null) setIcon(AbstractImagePrototype.create(icon));
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

	public String getIdTabItem() {
		return idTabItem;
	}

	public void setIdTabItem(String idTabItem) {
		this.idTabItem = idTabItem;
	}
	
}
