package com.gapso.web.trieda.client.util.view.simplecrud;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.client.crudmodel.AbstractCrudModel;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class SimpleCrudToolBar extends ToolBar {

	private AbstractCrudModel crudModel;
	
	public SimpleCrudToolBar(AbstractCrudModel crudModel) {
		createButtons();
		this.crudModel = crudModel;
	}
	
	private void createButtons() {
		createDefaultsButtons();
		createExtrasButtons();
	}
	
	private void createDefaultsButtons() {
		// BOTÃO ADICIONAR
		addButton("Adicionar", Resources.SIMPLE_CRUD.add16(), new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				SimpleCrudModal modalCrud = new SimpleCrudModal("BorderLayout Dialog");
				modalCrud.add(new SimpleCrudForm(crudModel));
				modalCrud.show();
			}
		});
		// BOTÃO EDITAR
		addButton("Editar", Resources.SIMPLE_CRUD.edit16(), new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				MessageBox.alert("Desenvolvimento", "Esta sessão está em desenvolvimento", null);				
			}
		});
		// BOTÃO REMOVER
		addButton("Remover", Resources.SIMPLE_CRUD.del16(), new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				MessageBox.alert("Desenvolvimento", "Esta sessão está em desenvolvimento", null);				
			}
		});
		// SEPARADOR
		add(new SeparatorToolItem());
		// EXPORTAR PARA EXCEL
		addButton("Exportar para Excel", Resources.SIMPLE_CRUD.excelExport16(), new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				MessageBox.alert("Desenvolvimento", "Esta sessão está em desenvolvimento", null);				
			}
		});
		// IMPORTAR PARA EXCEL
		addButton("Importar para Excel", Resources.SIMPLE_CRUD.excelImport16(), new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				MessageBox.alert("Desenvolvimento", "Esta sessão está em desenvolvimento", null);				
			}
		});
		
	}
	
	private void createExtrasButtons() {
		add(new SeparatorToolItem());
	}
	
	private void addButton(String toolTip, ImageResource img, SelectionListener<ButtonEvent> sl) {
		Button bt = new Button();
		bt.setIcon(AbstractImagePrototype.create(img));
		bt.setToolTip(toolTip);
		bt.addSelectionListener(sl);
		add(bt);
	}
	
}
