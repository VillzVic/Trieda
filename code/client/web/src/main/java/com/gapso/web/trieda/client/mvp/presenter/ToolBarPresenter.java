package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.mvc.Dispatcher;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.client.AppEvents;
import com.google.gwt.user.client.ui.Widget;

public class ToolBarPresenter implements Presenter {

	public interface Display {
		Button getCampusListBt();
		Button getCampusNewBt();
		
		Component asComponent();
		ToolBar getToolBar();
	}
	
	private Display toolBar;
	
	public ToolBarPresenter(Display toolBar) {
		this.toolBar = toolBar;
		addListeners();
	}

	private void addListeners() {
		toolBar.getCampusListBt().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Dispatcher.forwardEvent(AppEvents.CampusList);
			}
		});
		toolBar.getCampusNewBt().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				MessageBox.alert("Desenvolvimento", "Esta sessão está em desenvolvimento", null);
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		ContentPanel container = (ContentPanel) widget;
		container.setTopComponent(toolBar.getToolBar());
	}

}
