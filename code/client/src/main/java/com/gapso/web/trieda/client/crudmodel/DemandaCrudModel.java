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
import com.gapso.web.trieda.client.util.view.simplecrud.AbstractCrudModel;


public class DemandaCrudModel extends AbstractCrudModel {

	@Override
	public String getServiceId() {
		return Services.DEMANDAS;
	}
	
	@Override
	public List<String> getIds() {
		List<String> ids = new ArrayList<String>();
		ids.add("campus");
		ids.add("turno");
		ids.add("curso");
		ids.add("disciplina");
		ids.add("demanda");
		return ids;
	}
	
	@Override
	public Field<?> getField(String id) {
		ComboBox<ModelData> cb = new ComboBox<ModelData>();
		cb.setStore(new ListStore<ModelData>());
		if(id.equals("campus")) return cb; 
		if(id.equals("turno")) return cb; 
		if(id.equals("curso")) return cb; 
		if(id.equals("disciplina")) return cb; 
		
		if(id.equals("demanda")) return new NumberField();
		
		return new TextField<String>();
	}
	
	@Override
	public boolean isIdFilter(String id) {
		if(id.equals("demanda")) return false;
		return true;
	}
	
	@Override
	public String getHeader(String id) {
		if(id.equals("campus")) return "Campus";
		if(id.equals("turno")) return "Turno";
		if(id.equals("curso")) return "Curso";
		if(id.equals("disciplina")) return "Disciplina";
		if(id.equals("demanda")) return "Demanda (em alunos)";
		return null;
	}
	
//	@Override
//	public int getWidth(String id) {
//		if(id.equals("nome")) return 150;
//		if(id.equals("tipo")) return 100;
//		return super.getWidth(id);
//	}
	
//	@Override
//	public List<Button> getExtrasButtons() {
//		List<Button> buttons = new ArrayList<Button>();
//		
//		Button bt1 = new Button("", AbstractImagePrototype.create(Resources.DEFAULTS.matrizCurricular16()));
//		bt1.setToolTip("Matrizes Curriculares");
//		buttons.add(bt1);
//		
//		return buttons;
//	}

}
