package com.gapso.web.trieda.client.crudmodel;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.simplecrud.AbstractCrudModel;
import com.google.gwt.user.client.ui.AbstractImagePrototype;


public class CursoCrudModel extends AbstractCrudModel {

	@Override
	public String getServiceId() {
		return Services.CURSOS;
	}
	
	@Override
	public List<String> getIds() {
		List<String> ids = new ArrayList<String>();
		ids.add("codigo");
		ids.add("nome");
		ids.add("tipo");
		ids.add("min_doutores");
		ids.add("min_mestres");
		ids.add("max_dis_professor");
		ids.add("multi_dis_professor");
		return ids;
	}
	
	@Override
	public Field<?> getField(String id) {
		ComboBox<ModelData> cb = new ComboBox<ModelData>();
		cb.setStore(new ListStore<ModelData>());
		if(id.equals("tipo")) return cb;
		if(id.equals("min_doutores")) return new NumberField();
		if(id.equals("min_mestres")) return new NumberField();
		if(id.equals("max_dis_professor")) return new NumberField();
		if(id.equals("multi_dis_professor")) return new CheckBox();
		return new TextField<String>();
	}
	
	@Override
	public boolean isIdFilter(String id) {
		if(id.equals("codigo")) return true;
		if(id.equals("nome")) return true;
		if(id.equals("tipo")) return true;
		return false;
	}
	
	@Override
	public String getHeader(String id) {
		if(id.equals("codigo")) return "Código";
		if(id.equals("nome")) return "Nome";
		if(id.equals("tipo")) return "Tipo";
		if(id.equals("min_doutores")) return "% mínimo de doutores";
		if(id.equals("min_mestres")) return "% mínimo de mestres";
		if(id.equals("max_dis_professor")) return "Número máximo de disciplinas que um professor pode ministra";
		if(id.equals("multi_dis_professor")) return "Um professor pode ministrar mais de uma disciplina por período?";
		return null;
	}
	
	@Override
	public String getLabel(String id) {
		return getHeader(id);
	}
	
	@Override
	public int getWidth(String id) {
		if(id.equals("codigo")) return 100;
		if(id.equals("nome")) return 150;
		if(id.equals("tipo")) return 100;
		return super.getWidth(id);
	}
	
	@Override
	public List<Button> getExtrasButtons() {
		List<Button> buttons = new ArrayList<Button>();
		
		Button bt1 = new Button("", AbstractImagePrototype.create(Resources.DEFAULTS.matrizCurricular16()));
		bt1.setToolTip("Matrizes Curriculares");
		buttons.add(bt1);
		
		Button bt2 = new Button("", AbstractImagePrototype.create(Resources.DEFAULTS.areasDeTitulacao16()));
		bt2.setToolTip("Vincular Áreas de Titulação");
		buttons.add(bt2);
		
		return buttons;
	}

}
