package com.gapso.web.trieda.client.crudmodel;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.simplecrud.AbstractCrudModel;


public class EquivalenciaCrudModel extends AbstractCrudModel {

	@Override
	public String getServiceId() {
		return Services.EQUIVALENCIAS;
	}
	
	@Override
	public List<String> getIds() {
		List<String> ids = new ArrayList<String>();
		ids.add("cursou");
		ids.add("elimina");
		return ids;
	}
	
	@Override
	public Field<?> getField(String id) {
		ComboBox<ModelData> cb = new ComboBox<ModelData>();
		cb.setStore(new ListStore<ModelData>());
		if(id.equals("cursou")) return cb;
		if(id.equals("elimina")) return cb;
		return new TextField<String>();
	}
	
	@Override
	public String getHeader(String id) {
		if(id.equals("cursou")) return "Cursou";
		if(id.equals("elimina")) return "Elimina";
		return null;
	}
	
	@Override
	public int getWidth(String id) {
		if(id.equals("cursou")) return 200;
		if(id.equals("elimina")) return 200;
		return super.getWidth(id);
	}
	
}
