package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
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
	}

	@Override
	public void go(Widget widget) {
		GTab tab = (GTab)widget;
		tab.add((GTabItem)display.getComponent());
	}

}
