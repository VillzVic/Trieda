package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.Style.IconAlign;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.button.ToggleButton;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class ToggleImageButton extends ToggleButton {

	private ImageResource iconSelect, iconNotSelect;
	
	public ToggleImageButton(Boolean value, ImageResource iconSelect, ImageResource iconNotSelect) {
		super();
		this.iconSelect = iconSelect;
		this.iconNotSelect = iconNotSelect;
		toggle(value, false);
		configuration();
	}

	private void configuration() {
		addStyleName("toggle-image-button");
		updateIcon();
		setIconAlign(IconAlign.LEFT);
		addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				updateIcon();
			}
	    });
	}
	
	private void updateIcon() {
		setIcon(AbstractImagePrototype.create(isPressed()?iconSelect:iconNotSelect));
	}

	
	
}
