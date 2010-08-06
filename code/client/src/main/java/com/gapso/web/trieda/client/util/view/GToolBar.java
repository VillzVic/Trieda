package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.Style.ButtonArrowAlign;
import com.extjs.gxt.ui.client.Style.ButtonScale;
import com.extjs.gxt.ui.client.Style.IconAlign;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.mvc.Dispatcher;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.button.ButtonGroup;
import com.extjs.gxt.ui.client.widget.button.SplitButton;
import com.extjs.gxt.ui.client.widget.layout.TableData;
import com.extjs.gxt.ui.client.widget.menu.Menu;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class GToolBar extends ToolBar {

	public GToolBar() {
		createGroups();
	}
	
	private void createGroups() {
		createGroupUnidades();
		createGroupUnidades();
		createGroupUnidades();
		createGroupUnidades();
	}
	
	private void createGroupUnidades() {
		TableData data = new TableData();
		data.setRowspan(2);
		
		ButtonGroup group = new ButtonGroup(2);
		group.setHeading("Campi");

		Button campiBt = new Button("Campi");
		campiBt.addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				MessageBox.alert("Desenvolvimento", "Esta sessão está em desenvolvimento", null);
			}
		});
		campiBt.setScale(ButtonScale.MEDIUM);
		campiBt.setIconAlign(IconAlign.TOP);
		campiBt.setArrowAlign(ButtonArrowAlign.BOTTOM);
		campiBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.campi24()));

		group.add(campiBt, data);
		
		Button unidadesBt = new Button("Unidades");
		unidadesBt.addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Dispatcher.forwardEvent(AppEvents.UnidadeList);
			}
		});
		unidadesBt.setIconAlign(IconAlign.LEFT);
		unidadesBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.unidade16()));
		group.add(unidadesBt);
		
		SplitButton salasBt = new SplitButton("Outros");
		salasBt.setIconAlign(IconAlign.LEFT);
		salasBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.sala16()));
		Menu menuSalasBt = new Menu();
		
		MenuItem outros1 = new MenuItem("Salas");
		outros1.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.sala16()));
		menuSalasBt.add(outros1);
		
		MenuItem outros2 = new MenuItem("Cursos");
		outros2.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.sala16()));
		menuSalasBt.add(outros2);
		
		MenuItem outros3 = new MenuItem("Disciplinas");
		outros3.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.sala16()));
		menuSalasBt.add(outros3);
		
		MenuItem outros4 = new MenuItem("Matrizes Curriculares");
		outros4.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.sala16()));
		menuSalasBt.add(outros4);
		
		salasBt.setMenu(menuSalasBt);
		group.add(salasBt);
		
		add(group);
	}
	
}
