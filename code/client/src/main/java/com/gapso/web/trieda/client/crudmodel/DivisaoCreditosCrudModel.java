package com.gapso.web.trieda.client.crudmodel;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.simplecrud.AbstractCrudModel;


public class DivisaoCreditosCrudModel extends AbstractCrudModel {

	@Override
	public String getServiceId() {
		return Services.DIVISOESCREDITOS;
	}
	
	@Override
	public List<String> getIds() {
		List<String> ids = new ArrayList<String>();
		ids.add("creditos");
		ids.add("dia1");
		ids.add("dia2");
		ids.add("dia3");
		ids.add("dia4");
		ids.add("dia5");
		ids.add("dia6");
		ids.add("dia7");
		return ids;
	}
	
	@Override
	public Field<?> getField(String id) {
		return new NumberField();
	}
	
	@Override
	public boolean isIdFilter(String id) {
		if(id.equals("creditos")) return true;
		return false;
	}

	@Override
	public String getHeader(String id) {
		if(id.equals("creditos")) return "Créditos";
		if(id.equals("dia1")) return "Dia 1";
		if(id.equals("dia2")) return "Dia 2";
		if(id.equals("dia3")) return "Dia 3";
		if(id.equals("dia4")) return "Dia 4";
		if(id.equals("dia5")) return "Dia 5";
		if(id.equals("dia6")) return "Dia 6";
		if(id.equals("dia7")) return "Dia 7";
		return null;
	}
	
//	@Override
//	public List<Button> getExtrasButtons() {
//		List<Button> buttons = new ArrayList<Button>();
//		
//		Button bt1 = new Button("", AbstractImagePrototype.create(Resources.DEFAULTS.demanda16()));
//		bt1.setToolTip("Demanda de Alunos");
//		buttons.add(bt1);
//		
//		return buttons;
//	}

}
