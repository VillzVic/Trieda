package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.Style.ButtonArrowAlign;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.menu.Menu;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.shared.util.resources.Resources;
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
		simetriaBT = createButton("Matriz simétrica", Resources.DEFAULTS.matrizSimetrica16());
		importExcelButton = createButton("Importar para Excel", Resources.DEFAULTS.importar16());
		String[] menus = {"Exportar como xls","Exportar como xlsx"};
		exportExcelButton = createMenuButton( "Exportar para Excel",
			Resources.DEFAULTS.exportar16(), menus );

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
	
	public Button createMenuButton ( String toolTip, ImageResource img, String menus[] )
	{
		Button bt = new Button();
		
		bt.setIcon( AbstractImagePrototype.create( img ) );
		bt.setToolTip( toolTip );
		bt.setArrowAlign( ButtonArrowAlign.BOTTOM );

		Menu menu = new Menu();
		
		for(String s : menus) {
			menu.add(new MenuItem(s));
		}
		bt.setMenu(menu);

		return bt;
	}
}
