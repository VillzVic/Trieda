package com.gapso.web.trieda.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.client.mvp.model.CalendarioDTO;
import com.gapso.web.trieda.client.mvp.view.CalendarioFormView;
import com.gapso.web.trieda.client.services.CalendariosServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CalendariosPresenter implements Presenter {

	public interface Display {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		SimpleGrid<CalendarioDTO> getGrid();
		GTabItem getGTabItem();
		Component getComponent();
	}
	private Display display; 
	
	public CalendariosPresenter(Display display) {
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new CalendarioFormPresenter(new CalendarioFormView(new CalendarioDTO()), display.getGrid());
				presenter.go(null);
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				CalendarioDTO dto = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Presenter presenter = new CalendarioFormPresenter(new CalendarioFormView(dto), display.getGrid());
				presenter.go(null);
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<CalendarioDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final CalendariosServiceAsync service = Services.calendarios();
				service.remove(list, new AsyncCallback<Void>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(Void result) {
						display.getGrid().updateList();
						Info.display("Removido", "Item removido com sucesso!");
					}
				});
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		GTab tab = (GTab)widget;
		tab.add((GTabItem)display.getComponent());
	}

}
