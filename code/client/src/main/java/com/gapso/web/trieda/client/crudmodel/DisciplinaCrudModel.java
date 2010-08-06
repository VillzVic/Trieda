package com.gapso.web.trieda.client.crudmodel;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.services.Services;


public class DisciplinaCrudModel extends AbstractCrudModel {

	@Override
	public String getServiceId() {
		return Services.DISCIPLINAS;
	}
	
	@Override
	public List<String> getIds() {
		List<String> ids = new ArrayList<String>();
		ids.add("codigo");
		ids.add("nome");
		ids.add("cred_teorico");
		ids.add("cred_pratico");
		ids.add("laboratorio");
		ids.add("tipo");
		ids.add("dificuldade");
		ids.add("max_alun_teorico");
		ids.add("max_alun_pratico");
		return ids;
	}
	
	@Override
	public Field<?> getField(String id) {
		ComboBox<ModelData> cb = new ComboBox<ModelData>();
		cb.setStore(new ListStore<ModelData>());
		if(id.equals("tipo")) return cb;
		if(id.equals("dificuldade")) return cb;
		if(id.equals("cred_teorico")) return new NumberField();
		if(id.equals("cred_pratico")) return new NumberField();
		if(id.equals("max_alun_teorico")) return new NumberField();
		if(id.equals("max_alun_pratico")) return new NumberField();
		if(id.equals("laboratorio")) return new CheckBox();
		return new TextField<String>();
	}
	
	@Override
	public boolean isIdFilter(String id) {
		if(id.equals("codigo")) return true;
		if(id.equals("nome")) return true;
		if(id.equals("laboratorio")) return true;
		if(id.equals("tipo")) return true;
		if(id.equals("dificuldade")) return true;
		return false;
	}
		
	@Override
	public String getHeader(String id) {
		if(id.equals("codigo")) return "Código";
		if(id.equals("nome")) return "Nome";
		if(id.equals("tipo")) return "Tipo";
		if(id.equals("cred_teorico")) return "Créditos Teóricos";
		if(id.equals("cred_pratico")) return "Créditos Práticos";
		if(id.equals("laboratorio")) return "Usa laboratório?";
		if(id.equals("dificuldade")) return "Nível de dificuldade";
		if(id.equals("max_alun_teorico")) return "Máximo de Alunos - Teórico";
		if(id.equals("max_alun_pratico")) return "Máximo de Alunos - Prático";
		return null;
	}
	
	@Override
	public int getWidth(String id) {
		if(id.equals("codigo")) return 100;
		if(id.equals("nome")) return 150;
		return super.getWidth(id);
	}
	
}
