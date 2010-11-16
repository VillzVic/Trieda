package com.gapso.web.trieda.client.util.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;

public class SalaComboBox extends ComboBox<SalaDTO> {

	private ListStore<SalaDTO> store;
	
	public SalaComboBox() {
		store = new ListStore<SalaDTO>();
		store.add(new ArrayList<SalaDTO>());
		
		setDisplayField("codigo");
		setStore(store);
		setSimpleTemplate("{codigo}");
	}
	
	public void updateList(List<SalaDTO> list) {
		store.removeAll();
		store.add(list);
		setEnabled(!list.isEmpty());
	}

}
