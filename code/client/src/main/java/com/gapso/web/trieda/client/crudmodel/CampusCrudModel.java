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


public class CampusCrudModel extends AbstractCrudModel {

	@Override
	public String getServiceId() {
		return Services.CAMPI;
	}
	
	@Override
	public List<String> getIds() {
		List<String> ids = new ArrayList<String>();
		ids.add("codigo");
		ids.add("nome");
		return ids;
	}

	@Override
	public String getHeader(String id) {
		if(id.equals("codigo")) return "Código";
		if(id.equals("nome")) return "Nome";
		return null;
	}
	
	@Override
	public List<Button> getExtrasButtons() {
		List<Button> buttons = new ArrayList<Button>();
		
		Button bt1 = new Button("", AbstractImagePrototype.create(Resources.DEFAULTS.professores16()));
		bt1.setToolTip("Professores");
		buttons.add(bt1);
		
		return buttons;
	}

}
