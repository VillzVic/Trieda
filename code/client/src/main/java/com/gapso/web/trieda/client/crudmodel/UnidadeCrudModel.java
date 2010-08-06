package com.gapso.web.trieda.client.crudmodel;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.services.Services;


public class UnidadeCrudModel extends AbstractCrudModel {

	@Override
	public String getServiceId() {
		return Services.UNIDADES;
	}
	
	@Override
	public List<String> getIds() {
		List<String> ids = new ArrayList<String>();
		ids.add("codigo");
		ids.add("nome");
		ids.add("campus");
		ids.add("cap_salas");
		return ids;
	}
	
	@Override
	public Field<?> getField(String id) {
		if(id.equals("cap_salas")) return new NumberField();
		ComboBox<ModelData> cb = new ComboBox<ModelData>();
		cb.setStore(new ListStore<ModelData>());
		if(id.equals("campus")) return cb;
		return new TextField<String>();
	}
	
	@Override
	public boolean isIdForm(String id) {
		if(id.equals("cap_salas")) return false;
		return true;
	}
	
	@Override
	public boolean isIdFilter(String id) {
		if(id.equals("cap_salas")) return false;
		return true;
	}
	
	@Override
	public String getHeader(String id) {
		if(id.equals("codigo")) return "Código";
		if(id.equals("nome")) return "Nome";
		if(id.equals("campus")) return "Campus";
		if(id.equals("cap_salas")) return "Capidade Média das Salas de Aula";
		return null;
	}
	
	@Override
	public String getLabel(String id) {
		return getHeader(id);
	}
	
	@Override
	public int getWidth(String id) {
		if(id.equals("codigo")) return 100;
		if(id.equals("nome")) return 150;
		if(id.equals("campus")) return 100;
		return 0;
	}
	
}
