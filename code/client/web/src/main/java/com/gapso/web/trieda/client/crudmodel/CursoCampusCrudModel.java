package com.gapso.web.trieda.client.crudmodel;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.simplecrud.AbstractCrudModel;
import com.google.gwt.user.client.ui.AbstractImagePrototype;


public class CursoCampusCrudModel extends AbstractCrudModel {

	@Override
	public String getServiceId() {
		return Services.CURSOSCAMPI;
	}
	
	@Override
	public List<String> getIds() {
		List<String> ids = new ArrayList<String>();
		ids.add("campus");
		ids.add("turno");
		ids.add("curso");
		ids.add("matrizcurricular");
		return ids;
	}
	
	@Override
	public Field<?> getField(String id) {
		ComboBox<ModelData> cb = new ComboBox<ModelData>();
		cb.setStore(new ListStore<ModelData>());
		if(id.equals("turno")) return cb;
		if(id.equals("campus")) return cb;
		if(id.equals("curso")) return cb;
		if(id.equals("matrizcurricular")) return cb;
		return new TextField<String>();
	}
		
	@Override
	public String getHeader(String id) {
		if(id.equals("turno")) return "Turno";
		if(id.equals("campus")) return "Campus";
		if(id.equals("curso")) return "Curso";
		if(id.equals("matrizcurricular")) return "Matriz Curricular";
		return null;
	}
	
	@Override
	public List<Button> getExtrasButtons() {
		List<Button> buttons = new ArrayList<Button>();
		
		Button bt1 = new Button("", AbstractImagePrototype.create(Resources.DEFAULTS.demanda16()));
		bt1.setToolTip("Demanda de Alunos");
		buttons.add(bt1);
		
		return buttons;
	}

}
