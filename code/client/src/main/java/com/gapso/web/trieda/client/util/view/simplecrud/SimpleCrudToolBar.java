package com.gapso.web.trieda.client.util.view.simplecrud;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class SimpleCrudToolBar<M extends ModelData> extends ToolBar {

	private AbstractCrudModel crudModel;
	private SimpleCrudGrid<M> grid;
	private List<Button> oneItemButtons;
	private List<Button> multiItemsButtons;
	
	public SimpleCrudToolBar(AbstractCrudModel crudModel) {
		this.crudModel = crudModel;
		createButtons();
	}
	
	private void createButtons() {
		createDefaultsButtons();
		createExtrasButtons();
	}
	
	private void createDefaultsButtons() {
		oneItemButtons = new ArrayList<Button>();
		multiItemsButtons = new ArrayList<Button>();
		
		// BOTÃO ADICIONAR
		addButton("Adicionar", Resources.SIMPLE_CRUD.add16(), null, new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				SimpleCrudModal modalCrud = new SimpleCrudModal("BorderLayout Dialog");
				modalCrud.add(new SimpleCrudForm(crudModel));
				modalCrud.show();
			}
		});
		// BOTÃO EDITAR
		addButton("Editar", Resources.SIMPLE_CRUD.edit16(), oneItemButtons, new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				MessageBox.alert("Desenvolvimento", "Esta sessão está em desenvolvimento", null);
			}
		});
		// BOTÃO REMOVER
		addButton("Remover", Resources.SIMPLE_CRUD.del16(), multiItemsButtons, new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				MessageBox.alert("Desenvolvimento", "Esta sessão está em desenvolvimento", null);
			}
		});
		// SEPARADOR
		add(new SeparatorToolItem());
		// EXPORTAR PARA EXCEL
		addButton("Exportar para Excel", Resources.SIMPLE_CRUD.excelExport16(), null, new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				MessageBox.alert("Desenvolvimento", "Esta sessão está em desenvolvimento", null);
			}
		});
		// IMPORTAR PARA EXCEL
		addButton("Importar para Excel", Resources.SIMPLE_CRUD.excelImport16(), null, new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				MessageBox.alert("Desenvolvimento", "Esta sessão está em desenvolvimento", null);
			}
		});
		
	}
	
	private void createExtrasButtons() {
		List<Button> buttons = crudModel.getExtrasButtons();
		if(buttons != null && !buttons.isEmpty()) {
			add(new SeparatorToolItem());
			for(Button button : buttons) {
				button.addSelectionListener(new SelectionListener<ButtonEvent>() {
					@Override
					public void componentSelected(ButtonEvent arg0) {
						MessageBox.alert("Desenvolvimento", "Esta sessão está em desenvolvimento", null);						
					}
				});
				add(button);
				oneItemButtons.add(button);
			}
		}
	}
	
	private void addButton(String toolTip, ImageResource img, List<Button> list, SelectionListener<ButtonEvent> sl) {
		Button bt = new Button();
		bt.setIcon(AbstractImagePrototype.create(img));
		bt.setToolTip(toolTip);
		bt.addSelectionListener(sl);
		add(bt);
		if(list != null) list.add(bt);
	}
	
	public void setGrid(SimpleCrudGrid<M> grid) {
		this.grid = grid;
		setGridListeners();
		updateButtons(0);
	}
	
	private void setGridListeners() {
		grid.getSelectionModel().addListener(Events.SelectionChange, new Listener<SelectionChangedEvent<M>>() {
			public void handleEvent(SelectionChangedEvent<M> be) {
				updateButtons(be.getSelection().size());
			}
		});
	}
	
	private void updateButtons(int size) {
		switch (size) {
			case 0:
				for(Button bt : oneItemButtons) 	bt.setEnabled(false);
				for(Button bt : multiItemsButtons) 	bt.setEnabled(false);
			break;
			case 1:
				for(Button bt : oneItemButtons) 	bt.setEnabled(true);
				for(Button bt : multiItemsButtons) 	bt.setEnabled(true);
			break;
			default:
				for(Button bt : oneItemButtons) 	bt.setEnabled(false);
				for(Button bt : multiItemsButtons) 	bt.setEnabled(true);
			break;
		}
	}
	
}
