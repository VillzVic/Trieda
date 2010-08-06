package com.gapso.web.trieda.client.crudmodel;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.services.Services;


public class MatrizCurricularCrudModel extends AbstractCrudModel {

	@Override
	public String getServiceId() {
		return Services.MATRIZESCURRICULARES;
	}
	
	@Override
	public List<String> getIds() {
		List<String> ids = new ArrayList<String>();
		ids.add("codigo");
		ids.add("descricao");
		ids.add("curso");
		ids.add("num_periodos");
		return ids;
	}

	@Override
	public Field<?> getField(String id) {
		ComboBox<ModelData> cb = new ComboBox<ModelData>();
		cb.setStore(new ListStore<ModelData>());
		if(id.equals("curso")) return cb;
		return new TextField<String>();
	}
	
	@Override
	public boolean isIdForm(String id) {
		if(id.equals("num_periodos")) return false;
		return true;
	}
	
	@Override
	public boolean isIdFilter(String id) {
		if(id.equals("num_periodos")) return false;
		return true;
	}
	
	@Override
	public String getHeader(String id) {
		if(id.equals("codigo")) return "Código";
		if(id.equals("descricao")) return "Descrição";
		if(id.equals("curso")) return "Curso";
		if(id.equals("num_periodos")) return "Número de Períodos";
		return null;
	}
	
	@Override
	public int getWidth(String id) {
		if(id.equals("codigo")) return 100;
		if(id.equals("descricao")) return 300;
		if(id.equals("curso")) return 100;
		if(id.equals("num_periodos")) return 50;
		return super.getWidth(id);
	}
	
}
