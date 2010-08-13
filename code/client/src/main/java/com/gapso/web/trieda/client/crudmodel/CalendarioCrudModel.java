package com.gapso.web.trieda.client.crudmodel;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.simplecrud.AbstractCrudModel;
import com.google.gwt.user.client.ui.AbstractImagePrototype;


public class CalendarioCrudModel extends AbstractCrudModel {

	@Override
	public String getServiceId() {
		return Services.CALENDARIOS;
	}
	
	@Override
	public List<String> getIds() {
		List<String> ids = new ArrayList<String>();
		ids.add("codigo");
		ids.add("descricao");
		ids.add("duracao_aula");
		return ids;
	}
	
	@Override
	public Field<?> getField(String id) {
		if(id.equals("duracao_aula")) return new NumberField();
		return super.getField(id);
	}

	@Override
	public String getHeader(String id) {
		if(id.equals("codigo")) return "Código";
		if(id.equals("descricao")) return "Descrição";
		if(id.equals("duracao_aula")) return "Duração da Aula";
		return null;
	}

	@Override
	public int getWidth(String id) {
		if(id.equals("descricao")) return 300;
		return super.getWidth(id);
	}
	
	@Override
	public List<Button> getExtrasButtons() {
		List<Button> buttons = new ArrayList<Button>();
		
		Button bt1 = new Button("", AbstractImagePrototype.create(Resources.DEFAULTS.periodoDeAula16()));
		bt1.setToolTip("Dias de Aulas");
		buttons.add(bt1);
		
		return buttons;
	}

}
