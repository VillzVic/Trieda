package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.util.Padding;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayoutData;
import com.extjs.gxt.ui.client.widget.layout.BoxLayout.BoxLayoutPack;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class SimpleFilter extends FormPanel {

	private FormData formDataFilter;
	private Button submitButton;
	private Button resetButton;
	
	public SimpleFilter() {
		super();
		configuration();
	}

	private void configuration() {
		formDataFilter = new FormData("-5");
		setHeading("Filtro");
		setWidth(350);
		createButtons();
	}

	public void addField(Field<?> field) {
		insert(field, getItemCount() - 1, formDataFilter);
	}
	
	private void createButtons() {
		LayoutContainer lc = new LayoutContainer();
		HBoxLayout llc = new HBoxLayout();  
		llc.setPadding(new Padding(5));
		llc.setPack(BoxLayoutPack.CENTER);
		lc.setLayout(llc);
		HBoxLayoutData hbld = new HBoxLayoutData(new Margins(0, 5, 0, 0));
		submitButton = new Button("Filtrar", AbstractImagePrototype.create(Resources.SIMPLE_CRUD.filter16()));
		lc.add(submitButton, hbld);
		resetButton = new Button("Limpar",  AbstractImagePrototype.create(Resources.SIMPLE_CRUD.filterClean16()));
		lc.add(resetButton, hbld);  
		add(lc);
	}

	public Button getSubmitButton() {
		return submitButton;
	}

	public Button getResetButton() {
		return resetButton;
	}
	
}