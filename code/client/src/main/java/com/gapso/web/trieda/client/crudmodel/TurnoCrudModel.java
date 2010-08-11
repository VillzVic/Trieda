package com.gapso.web.trieda.client.crudmodel;

import java.util.ArrayList;
import java.util.List;

import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.simplecrud.AbstractCrudModel;

public class TurnoCrudModel extends AbstractCrudModel {

	@Override
	public String getServiceId() {
		return Services.TURNOS;
	}

	@Override
	public List<String> getIds() {
		List<String> ids = new ArrayList<String>();
		ids.add("nome");
		return ids;
	}

	@Override
	public String getHeader(String id) {
		if (id.equals("nome"))
			return "Nome";
		return null;
	}

	// @Override
	// public List<Button> getExtrasButtons() {
	// List<Button> buttons = new ArrayList<Button>();
	//
	// Button bt1 = new Button("",
	// AbstractImagePrototype.create(Resources.DEFAULTS.professores16()));
	// bt1.setToolTip("Professores");
	// buttons.add(bt1);
	//
	// return buttons;
	// }

}
