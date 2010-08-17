package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Window;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;
import com.google.gwt.user.client.ui.Widget;

public class SimpleModal extends Window {

	public SimpleModal(String heading, ImageResource icon) {
		setHeading(heading);
		setIcon(AbstractImagePrototype.create(icon));
		configuration();
	}
	
	private void configuration() {
		setModal(true);
		setLayout(new FitLayout());
		setBodyBorder(false);
		addButton();
		setWidth(600);
		setHeight(600);
	}
	
	public void setContent(Widget widget) {
		add(widget);
	}
	
	private boolean save() {
		return true;
	}
	
	private void addButton() {
		Button salvarBt = new Button("Salvar");
		salvarBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.save16()));
		salvarBt.addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(save()) hide();
			}
		});
		addButton(salvarBt);
	}
}
