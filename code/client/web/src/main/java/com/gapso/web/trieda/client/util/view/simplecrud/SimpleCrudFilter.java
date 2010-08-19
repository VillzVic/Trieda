package com.gapso.web.trieda.client.util.view.simplecrud;

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

public class SimpleCrudFilter extends FormPanel {

	private FormData formDataFilter;
	private AbstractCrudModel crudModel;
	
	public SimpleCrudFilter(AbstractCrudModel crudModel) {
		super();
		this.crudModel = crudModel;
		configuration();
	}

	private void configuration() {
		formDataFilter = new FormData("-5");
		setHeading("Filtro");  
		setWidth(350);
		createFields();
		createButtons();
	}
	
	private void createFields() {
		for(String id : crudModel.getIds()) {
			if(!crudModel.isIdFilter(id)) continue;
			Field<?> field = crudModel.getField(id);
			field.setFieldLabel(crudModel.getLabel(id));  
			add(field, formDataFilter);			
		}
	}
	
	private void createButtons() {
		LayoutContainer lc = new LayoutContainer();
		HBoxLayout llc = new HBoxLayout();  
		llc.setPadding(new Padding(5));
		llc.setPack(BoxLayoutPack.CENTER);
		lc.setLayout(llc);
		HBoxLayoutData hbld = new HBoxLayoutData(new Margins(0, 5, 0, 0));
		lc.add(new Button("Filtrar", AbstractImagePrototype.create(Resources.SIMPLE_CRUD.filter16())), hbld);
		lc.add(new Button("Limpar",  AbstractImagePrototype.create(Resources.SIMPLE_CRUD.filterClean16())), hbld);  
		add(lc);
	}
}
