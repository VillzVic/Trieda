package com.gapso.web.trieda.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.gapso.web.trieda.client.mvp.model.HorarioAulaDTO;
import com.gapso.web.trieda.client.mvp.presenter.HorariosAulaPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.SemanaLetivaComboBox;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleFilter;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleToolBar;
import com.gapso.web.trieda.client.util.view.TurnoComboBox;
import com.google.gwt.i18n.client.DateTimeFormat;

public class HorariosAulaView extends MyComposite implements HorariosAulaPresenter.Display {

	private SimpleToolBar toolBar;
	private SimpleGrid<HorarioAulaDTO> gridPanel;
	private SimpleFilter filter;
	private TextField<String> horarioBuscaTextField;
	private SemanaLetivaComboBox semanaLetivaBuscaComboBox;
	private TurnoComboBox turnoBuscaComboBox;
	private ContentPanel panel;
	private GTabItem tabItem;
	
	private DateTimeFormat dateTimeFormat = DateTimeFormat.getFormat("HH:mm");
	
	public HorariosAulaView() {
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Horário de Aula");
		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent(tabItem);
	}
	
	private void createTabItem() {
		tabItem = new GTabItem("Horário de Aula", Resources.DEFAULTS.horarioAula16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		toolBar = new SimpleToolBar(this);
		panel.setTopComponent(toolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    gridPanel = new SimpleGrid<HorarioAulaDTO>(getColumnList());
	    panel.add(gridPanel, bld);
	}

	public List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		list.add(new ColumnConfig("semanaLetivaString", "Semana Letiva", 100));
		list.add(new ColumnConfig("turnoString", "Turno", 100));
		ColumnConfig inicioColumn = new ColumnConfig("inicio", "Horário Início", 100);
		inicioColumn.setDateTimeFormat(dateTimeFormat);
		list.add(inicioColumn);
		ColumnConfig fimColumn = new ColumnConfig("fim", "Horário Fim", 100);
		fimColumn.setDateTimeFormat(dateTimeFormat);
		list.add(fimColumn);
		return list;
	}

	private void createFilter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.EAST);
		bld.setMargins(new Margins(0, 0, 0, 5));
		bld.setCollapsible(true);
		
		filter = new SimpleFilter();
		semanaLetivaBuscaComboBox = new SemanaLetivaComboBox();
		semanaLetivaBuscaComboBox.setFieldLabel("Semana Letiva");
		turnoBuscaComboBox = new TurnoComboBox();
		turnoBuscaComboBox.setFieldLabel("Turno");
		horarioBuscaTextField = new TextField<String>();
		horarioBuscaTextField.setFieldLabel("Horário início");
		
		filter.addField(semanaLetivaBuscaComboBox);
		filter.addField(turnoBuscaComboBox);
		filter.addField(horarioBuscaTextField);
		
		panel.add(filter, bld);
	}
	
	@Override
	public Button getNewButton() {
		return toolBar.getNewButton();
	}

	@Override
	public Button getEditButton() {
		return toolBar.getEditButton();
	}

	@Override
	public Button getRemoveButton() {
		return toolBar.getRemoveButton();
	}

	@Override
	public Button getImportExcelButton() {
		return toolBar.getImportExcelButton();
	}

	@Override
	public Button getExportExcelButton() {
		return toolBar.getExportExcelButton();
	}
	
	@Override
	public SimpleGrid<HorarioAulaDTO> getGrid() {
		return gridPanel;
	}

	@Override
	public void setProxy(RpcProxy<PagingLoadResult<HorarioAulaDTO>> proxy) {
		gridPanel.setProxy(proxy);
	}

	@Override
	public TextField<String> getHorarioBuscaTextField() {
		return horarioBuscaTextField;
	}

	@Override
	public SemanaLetivaComboBox getSemanaLetivaBuscaComboBox() {
		return semanaLetivaBuscaComboBox;
	}

	@Override
	public TurnoComboBox getTurnoBuscaComboBox() {
		return turnoBuscaComboBox;
	}

	@Override
	public Button getSubmitBuscaButton() {
		return filter.getSubmitButton();
	}

	@Override
	public Button getResetBuscaButton() {
		return filter.getResetButton();
	}
	
}
