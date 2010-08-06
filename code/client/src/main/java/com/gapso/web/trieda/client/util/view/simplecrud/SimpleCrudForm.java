package com.gapso.web.trieda.client.util.view.simplecrud;

import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.FieldSet;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.client.crudmodel.AbstractCrudModel;

public class SimpleCrudForm extends FormPanel {

	private AbstractCrudModel crudModel;
	private FormData formDataItem;
	
	public SimpleCrudForm(AbstractCrudModel crudModel) {
		this.crudModel = crudModel;
		configuration();
	}
	
	public void configuration() {
		setHeaderVisible(false);
		setWidth(350);
		createFields();
	}

	private void createFields() {
		formDataItem = new FormData("-5");
		
		FormLayout layout = new FormLayout();  
		layout.setLabelWidth(75);
		
		FieldSet fieldSet = new FieldSet();
		fieldSet.setLayout(layout);
		fieldSet.setWidth(350);
		fieldSet.setHeading("Informações Gerais");	
		
		for(String id : crudModel.getIds()) {
			if(!crudModel.isIdForm(id)) continue;
			Field<?> field = crudModel.getField(id);
			field.setFieldLabel(crudModel.getLabel(id));
//			field.setAutoWidth(true);
			field.setId(id);
			fieldSet.add(field, formDataItem);
		}
		
		add(fieldSet);
		
	}

}
