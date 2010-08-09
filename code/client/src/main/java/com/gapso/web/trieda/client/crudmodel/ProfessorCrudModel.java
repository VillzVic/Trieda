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


public class ProfessorCrudModel extends AbstractCrudModel {

	@Override
	public String getServiceId() {
		return Services.PROFESSORES;
	}
	
	@Override
	public List<String> getIds() {
		List<String> ids = new ArrayList<String>();
		ids.add("cpf");
		ids.add("nome");
		ids.add("tipo");
		ids.add("hor_min");
		ids.add("hor_max");
		ids.add("titulacao");
		ids.add("area_titulacao");
		ids.add("nota");
		ids.add("cred_anterior");
		ids.add("valor_credito");
		return ids;
	}
	
	@Override
	public Field<?> getField(String id) {
		ComboBox<ModelData> cb = new ComboBox<ModelData>();
		cb.setStore(new ListStore<ModelData>());
		if(id.equals("tipo")) return cb; 
		if(id.equals("titulacao")) return cb; 
		if(id.equals("area_titulacao")) return cb; 
		if(id.equals("nota")) return cb; 
		
		if(id.equals("hor_min")) return new NumberField();
		if(id.equals("hor_max")) return new NumberField();
		if(id.equals("cred_anterior")) return new NumberField();
		if(id.equals("valor_credito")) return new NumberField();
		
		return new TextField<String>();
	}
	
	@Override
	public boolean isIdFilter(String id) {
		if(id.equals("cpf")) return true;
		if(id.equals("nome")) return true;
		if(id.equals("tipo")) return true;
		if(id.equals("titulacao")) return true;
		if(id.equals("area_titulacao")) return true;
		if(id.equals("nota")) return true;
		return false;
	}
	
	@Override
	public String getHeader(String id) {
		if(id.equals("cpf")) return "CPF";
		if(id.equals("nome")) return "Nome";
		if(id.equals("tipo")) return "Tipo";
		if(id.equals("hor_min")) return "Carga Horária Mínima";
		if(id.equals("hor_max")) return "Carga Horária Máxima";
		if(id.equals("titulacao")) return "Titulação";
		if(id.equals("area_titulacao")) return "Área de Titulação";
		if(id.equals("nota")) return "Nota Avaliação de Desempenho";
		if(id.equals("cred_anterior")) return "Créditos Alocados no Período Anterior";
		if(id.equals("valor_credito")) return "Valor do Crédito Pago";
		return null;
	}
	
	@Override
	public int getWidth(String id) {
		if(id.equals("nome")) return 150;
		if(id.equals("tipo")) return 100;
		return super.getWidth(id);
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
