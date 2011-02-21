package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class DeslocamentoToolBar extends ToolBar {

	private Button saveBT;
	private Button cancelBT;
	private Button simetriaBT;
	private Button importExcelButton;
	private Button exportExcelButton;
	
	public DeslocamentoToolBar() {
		initUI();
	}
	
	private void initUI() {
		saveBT = createButton("Salvar", Resources.DEFAULTS.save16());
		cancelBT = createButton("Cancelar", Resources.DEFAULTS.cancel16());
		simetriaBT = createButton("Matriz simétrica", Resources.DEFAULTS.deslocamento16());
		importExcelButton = createButton("Exportar para Excel", Resources.DEFAULTS.exportar16());
		exportExcelButton = createButton("Importar de Excel", Resources.DEFAULTS.importar16());

		add(saveBT);
		add(cancelBT);
		add(simetriaBT);
		add(new SeparatorToolItem());
		add(importExcelButton);
		add(exportExcelButton);
	}
	
	public Button getSaveButton() {
		return saveBT;
	}

	public Button getCancelButton() {
		return cancelBT;
	}

	public Button getSimetriaButton() {
		return simetriaBT;
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
