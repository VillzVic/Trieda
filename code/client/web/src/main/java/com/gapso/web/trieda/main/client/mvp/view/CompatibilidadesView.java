package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.Orientation;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.RowData;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.main.client.mvp.presenter.CompatibilidadesPresenter;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.IncompatibilidadeGrid;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class CompatibilidadesView extends MyComposite implements CompatibilidadesPresenter.Display {

	private ContentPanel panel;
	private IncompatibilidadeGrid grid;
	private ToolBar toolBar;
	
	private CursoComboBox cursoCB;
	private CurriculoComboBox curriculoCB;
	private SimpleComboBox<Integer> periodoCB;
	private Button salvarBt;
	private Button cancelarBt;
	
	private GTabItem tabItem;
	
	public CompatibilidadesView() {
		initUI();
		createTabItem();
		createToolBar();
		createContent();
		initComponent(tabItem);
	}

	private void initUI() {
		panel = new ContentPanel(new RowLayout(Orientation.VERTICAL));
		panel.setHeading("Master Data » Compatibilidade entre disciplinas");
		panel.setBodyStyle("background-color: transparent;");
	}

	private void createToolBar() {
		this.toolBar = new ToolBar();
		salvarBt = new Button();
		salvarBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.save16()));
		salvarBt.setToolTip("Salvar");
		
		cancelarBt = new Button();
		cancelarBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.cancel16()));
		cancelarBt.setToolTip("Cancelar");
		
		this.toolBar.add(salvarBt);
		this.toolBar.add(cancelarBt);
		
		panel.setTopComponent(this.toolBar);
	}
	
	private void createTabItem() {
		tabItem = new GTabItem("Compatibilidade entre disciplinas", Resources.DEFAULTS.compatibilidade16());
		tabItem.setContent(panel);
	}
	
	private void createContent() {
		FormData formData = new FormData("100%");
		FormPanel formPanel = new FormPanel();
		formPanel.setBodyBorder(false);
		formPanel.setLabelWidth(100);
		formPanel.setLabelAlign(LabelAlign.RIGHT);
		formPanel.setHeaderVisible(false);
		formPanel.setAutoHeight(true);
		
		cursoCB = new CursoComboBox();
		formPanel.add(cursoCB, formData);
		
		curriculoCB = new CurriculoComboBox(cursoCB);
		formPanel.add(curriculoCB, formData);
		
		periodoCB = new SimpleComboBox<Integer>();
		periodoCB.setFieldLabel("Periodo");
		periodoCB.setEnabled(false);
		formPanel.add(periodoCB, formData);
		
		panel.add(formPanel, new RowData(1, -1, new Margins(4)));
		
		grid = new IncompatibilidadeGrid();
		grid.setBodyBorder(false);
		
		panel.add(grid, new RowData(1, 1, new Margins(0, 5, 5, 5)));
	}
	
	@Override
	public Button getSalvarButton() {
		return salvarBt;
	}
	
	@Override
	public Button getCancelarButton() {
		return cancelarBt;
	}
	
	@Override
	public CursoComboBox getCursoComboBox() {
		return cursoCB;
	}

	@Override
	public IncompatibilidadeGrid getGrid() {
		return grid;
	}

	@Override
	public CurriculoComboBox getCurriculoComboBox() {
		return curriculoCB;
	}

	@Override
	public SimpleComboBox<Integer> getPeriodoComboBox() {
		return periodoCB;
	}
	
}
