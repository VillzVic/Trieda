package com.gapso.web.trieda.client.crudmodel;

import java.util.ArrayList;
import java.util.List;

import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.simplecrud.AbstractCrudModel;


public class AreaTitulacaoCrudModel extends AbstractCrudModel {

	@Override
	public String getServiceId() {
		return Services.AREASTITULACAO;
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
	public int getWidth(String id) {
		if(id.equals("descricao")) return 300;
		return super.getWidth(id);
	}
	
//	@Override
//	public List<Button> getExtrasButtons() {
//		List<Button> buttons = new ArrayList<Button>();
//		
//		Button bt1 = new Button("", AbstractImagePrototype.create(Resources.DEFAULTS.professores16()));
//		bt1.setToolTip("Professores");
//		buttons.add(bt1);
//		
//		return buttons;
//	}

}
