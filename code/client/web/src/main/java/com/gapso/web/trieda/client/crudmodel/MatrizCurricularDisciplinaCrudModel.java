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


public class MatrizCurricularDisciplinaCrudModel extends AbstractCrudModel {

	@Override
	public String getServiceId() {
		return Services.MATRIZCURRIULARDISCIPLINAS;
	}
	
	@Override
	public List<String> getIds() {
		List<String> ids = new ArrayList<String>();
		ids.add("matriz_curricular");
		ids.add("curso");
		ids.add("periodo");
		ids.add("disciplina_nome");
		ids.add("cred_teoricos");
		ids.add("cred_pratico");
		ids.add("total_creditos");
		return ids;
	}
	
	@Override
	public String groupBy() {
		return "periodo";
	}

	@Override
	public boolean isIdTable(String id) {
		if(id.equals("matriz_curricular")) return false;
		if(id.equals("curso")) return false;
		return true;
	}

	@Override
	public Field<?> getField(String id) {
		ComboBox<ModelData> cb = new ComboBox<ModelData>();
		cb.setStore(new ListStore<ModelData>());
		if(id.equals("disciplina_nome")) return cb;
		if(id.equals("periodo")) return new NumberField();
		if(id.equals("cred_teoricos")) return new NumberField();
		if(id.equals("cred_pratico")) return new NumberField();
		if(id.equals("total_creditos")) return new NumberField();
		return new TextField<String>();
	}
	
	@Override
	public boolean isIdForm(String id) {
		if(id.equals("disciplina_nome")) return true;
		if(id.equals("periodo")) return true;
		return false;
	}

	@Override
	public boolean isIdFilter(String id) {
		if(id.equals("matriz_curricular")) return true;
		if(id.equals("curso")) return true;
		return false;
	}
		
	@Override
	public String getHeader(String id) {
		if(id.equals("matriz_curricular")) return "Matriz Curricular";
		if(id.equals("curso")) return "Curso";
		if(id.equals("periodo")) return "Período";
		if(id.equals("disciplina_nome")) return "Displina";
		if(id.equals("cred_teoricos")) return "Créditos Teóricos";
		if(id.equals("cred_pratico")) return "Créditos Práticos";
		if(id.equals("total_creditos")) return "Total de Créditos";
		return null;
	}
	
//	@Override
//	public int getWidth(String id) {
//		if(id.equals("codigo")) return 100;
//		if(id.equals("nome")) return 150;
//		return super.getWidth(id);
//	}
	
//	public List<Button> getExtrasButtons() {
//		List<Button> buttons = new ArrayList<Button>();
//		
//		Button bt1 = new Button("", AbstractImagePrototype.create(Resources.DEFAULTS.professores16()));
//		bt1.setToolTip("Professores Capazes de Lecionar");
//		buttons.add(bt1);
//		
//		return buttons;
//	}
}
