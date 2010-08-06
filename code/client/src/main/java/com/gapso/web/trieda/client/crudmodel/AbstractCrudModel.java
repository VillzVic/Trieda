package com.gapso.web.trieda.client.crudmodel;

import java.util.List;

import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.TextField;

public abstract class AbstractCrudModel {
	
	public abstract List<String> getIds();
	
	public abstract String getServiceId();
	
	public abstract String getHeader(String id);
	
	public Field<?> getField(String id) {
		return new TextField<String>();
	}

	public boolean isBoolean(String id) {
		Field<?> f = getField(id);
		return (f instanceof CheckBox);
	}
	
	public boolean isIdForm(String id) {
		return true;
	}
	
	public boolean isIdFilter(String id) {
		return true;
	}
	
	public String getLabel(String id) {
		return getHeader(id);
	}
	
	public int getWidth(String id) {
		return 100;
	}
	
	
}
