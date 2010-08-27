package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class SimpleToolBar extends ToolBar {

	private Button newButton;
	private Button editButton;
	private Button removeButton;
	private Button importExcelButton;
	private Button exportExcelButton;
	
	public SimpleToolBar() {
		initUI();
	}
	
	private void initUI() {
		newButton = createButton("Adicionar", Resources.SIMPLE_CRUD.add16());
		editButton = createButton("Editar", Resources.SIMPLE_CRUD.edit16());
		removeButton = createButton("Remover", Resources.SIMPLE_CRUD.del16());
		importExcelButton = createButton("Exportar para Excel", Resources.SIMPLE_CRUD.excelExport16());
		exportExcelButton = createButton("Importar para Excel", Resources.SIMPLE_CRUD.excelImport16());

		add(newButton);
		add(editButton);
		add(removeButton);
		add(new SeparatorToolItem());
		add(importExcelButton);
		add(exportExcelButton);
	}
	
	public void activateEmptyState() {
		editButton.setEnabled(false);
		removeButton.setEnabled(false);
	}
	
	public void enableMultiState() {
		editButton.setEnabled(false);
		removeButton.setEnabled(true);
	}
	
	public void enableSimpleState() {
		editButton.setEnabled(true);
		removeButton.setEnabled(true);
	}
	
	public Button getNewButton() {
		return newButton;
	}

	public Button getEditButton() {
		return editButton;
	}

	public Button getRemoveButton() {
		return removeButton;
	}

	public Button getImportExcelButton() {
		return importExcelButton;
	}

	public Button getExportExcelButton() {
		return exportExcelButton;
	}

	private Button createButton(String toolTip, ImageResource img) {
		Button bt = new Button();
		bt.setIcon(AbstractImagePrototype.create(img));
		bt.setToolTip(toolTip);
		return bt;
	}
	
}
