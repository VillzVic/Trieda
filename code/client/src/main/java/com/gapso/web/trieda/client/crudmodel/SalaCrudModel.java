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


public class SalaCrudModel extends AbstractCrudModel {
	
	@Override
	public String getServiceId() {
		return Services.SALAS;
	}
	
	@Override
	public List<String> getIds() {
		List<String> ids = new ArrayList<String>();
		ids.add("codigo");
		ids.add("unidade");
		ids.add("num_sala");
		ids.add("andar");
		ids.add("tipo");
		ids.add("capacidade");
		return ids;
	}
	
	@Override
	public Field<?> getField(String id) {
		ComboBox<ModelData> cb = new ComboBox<ModelData>();
		cb.setStore(new ListStore<ModelData>());
		if(id.equals("unidade")) return cb;
		if(id.equals("tipo")) return cb;
		if(id.equals("capacidade")) return new NumberField();
		return new TextField<String>();
	}
	
	@Override
	public boolean isIdFilter(String id) {
		if(id.equals("codigo")) return true;
		if(id.equals("unidade")) return true;
		if(id.equals("andar")) return true;
		if(id.equals("tipo")) return true;
		return false;
	}
	
	@Override
	public String getHeader(String id) {
		if(id.equals("codigo")) return "Código";
		if(id.equals("unidade")) return "Unidade";
		if(id.equals("num_sala")) return "Número da Sala";
		if(id.equals("andar")) return "Andar";
		if(id.equals("tipo")) return "Tipo";
		if(id.equals("capacidade")) return "Capacidade";
		return null;
	}
	
	@Override
	public int getWidth(String id) {
		if(id.equals("codigo")) return 100;
		if(id.equals("unidade")) return 150;
		if(id.equals("tipo")) return 100;
		return 0;
	}
	
}
