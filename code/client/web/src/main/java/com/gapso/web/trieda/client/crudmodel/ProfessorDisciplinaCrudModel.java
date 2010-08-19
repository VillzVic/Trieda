package com.gapso.web.trieda.client.crudmodel;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.simplecrud.AbstractCrudModel;
import com.google.gwt.user.client.ui.AbstractImagePrototype;


public class ProfessorDisciplinaCrudModel extends AbstractCrudModel {

	@Override
	public String getServiceId() {
		return Services.PROFESSORESDISCIPLINAS;
	}
	
	@Override
	public List<String> getIds() {
		List<String> ids = new ArrayList<String>();
		ids.add("cpf");
		ids.add("nome");
		ids.add("disciplina");
		ids.add("preferencia");
		ids.add("nota");
		return ids;
	}
	
	@Override
	public Field<?> getField(String id) {
		ComboBox<ModelData> cb = new ComboBox<ModelData>();
		cb.setStore(new ListStore<ModelData>());
		if(id.equals("disciplina")) return cb; 
		if(id.equals("nota")) return cb; 
				
		if(id.equals("preferencia")) return new NumberField(); 
		
		return new TextField<String>();
	}
	
	@Override
	public String getHeader(String id) {
		if(id.equals("cpf")) return "CPF";
		if(id.equals("nome")) return "Nome";
		if(id.equals("disciplina")) return "Disciplina";
		if(id.equals("preferencia")) return "Ranking de Preferência";
		if(id.equals("nota")) return "Nota Avaliação de Desempenho";
		return null;
	}
	
	@Override
	public List<Button> getExtrasButtons() {
		List<Button> buttons = new ArrayList<Button>();
		
		Button bt1 = new Button("", AbstractImagePrototype.create(Resources.DEFAULTS.disciplina16()));
		bt1.setData("event", AppEvents.ProfessorDisciplinaView);
		bt1.setToolTip("Disciplinas Capaz de Lecionar");
		buttons.add(bt1);
		
		return buttons;
	}

}
