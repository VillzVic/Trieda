package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.client.mvp.view.TurnoFormView;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.google.gwt.user.client.ui.Widget;

public class TurnosPresenter implements Presenter {

	public interface Display {
		Button getNewButton();
		Button getEditButton();
		Button getDeleteButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		Button getGrid();
		GTabItem getGTabItem();
		Component getComponent();
	}
	private Display display; 
	
	public TurnosPresenter(Display display) {
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new TurnoFormPresenter(new TurnoFormView());
				presenter.go(null);
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		GTab tab = (GTab)widget;
		tab.add((GTabItem)display.getComponent());
	}

}
