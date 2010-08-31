package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.client.mvp.view.CalendariosView;
import com.gapso.web.trieda.client.mvp.view.PeriodosDeAulaView;
import com.gapso.web.trieda.client.mvp.view.TurnosView;
import com.gapso.web.trieda.client.util.view.GTab;
import com.google.gwt.user.client.ui.Widget;

public class ToolBarPresenter implements Presenter {

	public interface Display {
		Button getCampusListButton();
		Button getCampusNewButton();
		
		MenuItem getTurnosListMenuItem();
		MenuItem getCalendariosListMenuItem();
		MenuItem getPeriodosDeAulaListMenuItem();
		
		Component getComponent();
	}
	
	private Display toolBar;
	private GTab gTab;
	
	public ToolBarPresenter(Display toolBar) {
		this.toolBar = toolBar;
		addListeners();
	}

	private void addListeners() {
		toolBar.getCampusListButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				MessageBox.alert("Desenvolvimento", "Esta sessão está em desenvolvimento", null);
			}
		});
		toolBar.getCampusNewButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				MessageBox.alert("Desenvolvimento", "Esta sessão está em desenvolvimento", null);
			}
		});
		toolBar.getTurnosListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new TurnosPresenter(new TurnosView());
				presenter.go(gTab);
			}
		});
		toolBar.getCalendariosListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new CalendariosPresenter(new CalendariosView());
				presenter.go(gTab);
			}
		});
		toolBar.getPeriodosDeAulaListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new PeriodosDeAulaPresenter(new PeriodosDeAulaView());
				presenter.go(gTab);
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		AppPresenter.Display container = (AppPresenter.Display) widget;
		this.gTab = container.getGTab();
		container.getPanel().setTopComponent(toolBar.getComponent());
	}

}
