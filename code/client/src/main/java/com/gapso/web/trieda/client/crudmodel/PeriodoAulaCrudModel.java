package com.gapso.web.trieda.client.crudmodel;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.form.TimeField;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.simplecrud.AbstractCrudModel;


public class PeriodoAulaCrudModel extends AbstractCrudModel {

	@Override
	public String getServiceId() {
		return Services.PERIODOSAULA;
	}
	
	@Override
	public List<String> getIds() {
		List<String> ids = new ArrayList<String>();
		ids.add("calendario");
		ids.add("turno");
		ids.add("inicio");
		ids.add("fim");
		return ids;
	}
	
	@Override
	public Field<?> getField(String id) {
		ComboBox<ModelData> cb = new ComboBox<ModelData>();
		cb.setStore(new ListStore<ModelData>());
		if(id.equals("calendario")) return cb;
		if(id.equals("turno")) return cb;
		if(id.equals("inicio")) return new TimeField();
		if(id.equals("fim")) return new TimeField();
		return new TextField<String>();
	}
	
	@Override
	public String getHeader(String id) {
		if(id.equals("calendario")) return "Calendário";
		if(id.equals("turno")) return "Turno";
		if(id.equals("inicio")) return "Horário Início";
		if(id.equals("fim")) return "Horário Fim";
		return null;
	}
	
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
