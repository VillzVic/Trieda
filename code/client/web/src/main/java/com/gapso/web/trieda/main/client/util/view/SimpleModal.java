package com.gapso.web.trieda.main.client.util.view;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.ComponentEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.util.KeyNav;
import com.extjs.gxt.ui.client.widget.Window;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.trieda.main.client.util.resources.Resources;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;
import com.google.gwt.user.client.ui.Widget;

public class SimpleModal extends Window {

	private Button salvarBt;
	private Button cancelarBt;
	
	private String textSalvar = "Salvar";
	private String textCancelar = "Cancelar";
	
	public SimpleModal(String textSalvar, String textCancelar, String heading, ImageResource icon) {
		this.textSalvar = textSalvar;
		this.textCancelar = textCancelar;
		setHeading(heading);
		setIcon(AbstractImagePrototype.create(icon));
		configuration();
	}
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
		setResizable(false);
	}
	
	public void setContent(Widget widget) {
		add(widget);
	}
	
	private void addButtons() {
		if(textSalvar != null) {
			salvarBt = new Button(textSalvar, AbstractImagePrototype.create(Resources.DEFAULTS.save16()));
			addButton(salvarBt);
			setDefaultButton(salvarBt);
		}
		
		if(textCancelar != null) {
			cancelarBt = new Button(textCancelar, AbstractImagePrototype.create(Resources.DEFAULTS.cancel16()));
			cancelarBt.addSelectionListener(new SelectionListener<ButtonEvent>() {
				@Override
				public void componentSelected(ButtonEvent ce) {
					hide();
				}
			});
			addButton(cancelarBt);
		}
	}

	public void setDefaultButton(final Button button) {
		new KeyNav<ComponentEvent>(this) {
			@Override
			public void onEnter(ComponentEvent ce) {
				super.onEnter(ce);
				button.fireEvent(Events.Select);
			}
		};
	}
	
	public Button getSalvarBt() {
		return salvarBt;
	}

	public Button getCancelarBt() {
		return cancelarBt;
	}
	
}
