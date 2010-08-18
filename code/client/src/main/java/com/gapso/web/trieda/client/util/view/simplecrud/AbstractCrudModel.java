package com.gapso.web.trieda.client.util.view.simplecrud;

import java.util.List;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.form.TimeField;

public abstract class AbstractCrudModel {
	
	public abstract List<String> getIds();
	
	public abstract String getServiceId();
	
	public abstract String getHeader(String id);
	
	public Field<?> getField(String id) {
		return new TextField<String>();
	}
	
	public boolean isNumber(String id) {
		Field<?> f = getField(id);
		return (f instanceof NumberField);
	}

	public boolean isBoolean(String id) {
		Field<?> f = getField(id);
		return (f instanceof CheckBox);
	}
	
	public boolean isTime(String id) {
		Field<?> f = getField(id);
		return (f instanceof TimeField);
	}
	
	public String groupBy() {
		return null;
	}
	
	public boolean isIdTable(String id) {
		return true;
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
	
	public List<Button> getExtrasButtons() {
		return null;
	}
	
}
