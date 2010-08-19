package com.gapso.web.trieda.client.crudmodel;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.simplecrud.AbstractCrudModel;
import com.google.gwt.user.client.ui.AbstractImagePrototype;


public class GrupoSalaCrudModel extends AbstractCrudModel {
	
	@Override
	public String getServiceId() {
		return Services.GRUPOSSALAS;
	}
	
	@Override
	public List<String> getIds() {
		List<String> ids = new ArrayList<String>();
		ids.add("codigo");
		ids.add("descricao");
		return ids;
	}
		
	@Override
	public String getHeader(String id) {
		if(id.equals("codigo")) return "Código";
		if(id.equals("descricao")) return "Descrição";
		return null;
	}
		
	@Override
	public List<Button> getExtrasButtons() {
		List<Button> buttons = new ArrayList<Button>();
		
		Button bt1 = new Button("", AbstractImagePrototype.create(Resources.DEFAULTS.sala16()));
		bt1.setData("event", AppEvents.GrupoSalaAssociadasView);
		bt1.setToolTip("Associar Salas");
		buttons.add(bt1);
		
		return buttons;
	}
	
}
