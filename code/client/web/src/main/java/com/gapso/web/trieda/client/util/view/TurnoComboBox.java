package com.gapso.web.trieda.client.util.view;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.services.TurnosServiceAsync;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TurnoComboBox extends ComboBox<TurnoDTO> {

	final TurnosServiceAsync service;
	private CampusDTO campusDTO;
	
	public TurnoComboBox() {
		service = Services.turnos();
		update();
		setStore(new ListStore<TurnoDTO>());
		
		setFieldLabel("Turno");
		setDisplayField("nome");
		setTemplate(getTemplateCB());
		setEditable(false);
	}

	private void update() {
		if(getCampusDTO() == null) {
			updateWithoutCampus();
		} else {
			updateWithCampus();
		}
	}
	
	private void updateWithCampus() {
		service.getListByCampus(getCampusDTO(), new AsyncCallback<ListLoadResult<TurnoDTO>>() {
			@Override
			public void onFailure(Throwable caught) {
				caught.printStackTrace();
			}
			@Override
			public void onSuccess(ListLoadResult<TurnoDTO> result) {
				populate(result.getData());
			}
		});
	}
	
	private void updateWithoutCampus() {
		service.getList(new AsyncCallback<ListLoadResult<TurnoDTO>>() {
			@Override
			public void onFailure(Throwable caught) {
				caught.printStackTrace();
			}
			@Override
			public void onSuccess(ListLoadResult<TurnoDTO> result) {
				populate(result.getData());
			}
		});
	}
	
	private void populate(List<TurnoDTO> list) {
		getStore().removeAll();
		getStore().add(list);
	}
	
	public CampusDTO getCampusDTO() {
		return campusDTO;
	}
	public void setCampusDTO(CampusDTO campusDTO) {
		this.campusDTO = campusDTO;
		update();
	}

	private native String getTemplateCB() /*-{
		return  [
			'<tpl for=".">',
			'<div class="x-combo-list-item">{nome} ({tempo}min)</div>',
			'</tpl>'
		].join("");
	}-*/;
}
