package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class SimpleToolBar extends ToolBar {

	private Button newButton;
	private boolean showNewButton = true;
	private Button editButton;
	private boolean showEditButton = true;
	private Button removeButton;
	private boolean showRemoveButton = true;
	private Button importExcelButton;
	private boolean showImportExcelButton = true;
	private Button exportExcelButton;
	private boolean showExportExcelButton = true;
	
	private ITriedaI18nGateway i18nGateway;
	
	public SimpleToolBar(ITriedaI18nGateway i18nGateway) {
		super();
		this.i18nGateway = i18nGateway;
		initUI();
	}
	
	public SimpleToolBar(boolean showNewButton, boolean showEditButton, boolean showRemoveButton, boolean showImportExcelButton, boolean showExportExcelButton, ITriedaI18nGateway i18nGateway) {
		super();
		this.showNewButton = showNewButton;
		this.showEditButton = showEditButton;
		this.showRemoveButton = showRemoveButton;
		this.showImportExcelButton = showImportExcelButton;
		this.showExportExcelButton = showExportExcelButton;
		this.i18nGateway = i18nGateway;
		initUI();
	}

	private void initUI() {
		if(showNewButton) {
			newButton = createButton(i18nGateway.getI18nConstants().adicionar(), Resources.DEFAULTS.add16());
			add(newButton);
		}
		
		if(showEditButton) {
			editButton = createButton(i18nGateway.getI18nConstants().editar(), Resources.DEFAULTS.edit16());
			add(editButton);
		}
		
		if(showRemoveButton) {
			removeButton = new ConfirmationButton(i18nGateway);
			removeButton.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.del16()));
			removeButton.setToolTip(i18nGateway.getI18nConstants().remover());
			add(removeButton);
		}
		
		if(showImportExcelButton || showExportExcelButton) {
			add(new SeparatorToolItem());
		}
		
		if(showImportExcelButton) {
			importExcelButton = createButton(i18nGateway.getI18nConstants().importarExcel(), Resources.DEFAULTS.importar16());
			add(importExcelButton);
		}
		
		if(showExportExcelButton) {
			exportExcelButton = createButton(i18nGateway.getI18nConstants().exportarExcel(), Resources.DEFAULTS.exportar16());
			add(exportExcelButton);
		}

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

	public Button createButton(String toolTip, ImageResource img) {
		Button bt = new Button();
		bt.setIcon(AbstractImagePrototype.create(img));
		bt.setToolTip(toolTip);
		return bt;
	}

}
