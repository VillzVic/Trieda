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

	private Button salvarBt;
	private Button cancelarBt;
	
	public SimpleModal(String heading, ImageResource icon) {
		setHeading(heading);
		setIcon(AbstractImagePrototype.create(icon));
		configuration();
	}
	
	private void configuration() {
		setModal(true);
		setLayout(new FitLayout());
		setBodyBorder(false);
		addButtons();
		setHeight(138);
	}
	
	public void setContent(Widget widget) {
		add(widget);
	}
	
	private void addButtons() {
		salvarBt = new Button("Salvar", AbstractImagePrototype.create(Resources.DEFAULTS.save16()));
		addButton(salvarBt);
		
		cancelarBt = new Button("Cancelar", AbstractImagePrototype.create(Resources.DEFAULTS.cancel16()));
		cancelarBt.addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				hide();
			}
		});
		addButton(cancelarBt);
	}

	public Button getSalvarBt() {
		return salvarBt;
	}

	public Button getCancelarBt() {
		return cancelarBt;
	}
	
}
