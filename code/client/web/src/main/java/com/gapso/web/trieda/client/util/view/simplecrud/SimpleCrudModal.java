package com.gapso.web.trieda.client.util.view.simplecrud;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Window;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.google.gwt.user.client.ui.AbstractImagePrototype;
import com.google.gwt.user.client.ui.Widget;

public class SimpleCrudModal extends Window {

	private Button saveBt;
	private Button cancelBt;
	private Widget widgetLast;
	
	public SimpleCrudModal(String heading) {
		setHeading(heading);
		configuration();
	}

	private void configuration() {
		setModal(true);
		setLayout(new FitLayout());
		setBodyBorder(false);
		setWidth(400);
		
		saveBt = new Button("Salvar");
		saveBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.save16()));
		addButton(saveBt);
		
		cancelBt = new Button("Cancelar");
		cancelBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.cancel16()));
		cancelBt.addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				hide();
			}
		});
		addButton(cancelBt);
	}

	@Override
	public boolean add(Widget widget) {
		if(widgetLast != null) remove(widgetLast);
		boolean flag = super.add(widget);
		if(flag) widgetLast = widget;
		return flag;
	}

}
