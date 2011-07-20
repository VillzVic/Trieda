package com.gapso.web.trieda.main.client.mvp.view;

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
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessoresPresenter;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.AreaTitulacaoComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.TipoContratoComboBox;
import com.gapso.web.trieda.shared.util.view.TitulacaoComboBox;

public class ProfessoresView extends MyComposite implements
		ProfessoresPresenter.Display {

	private SimpleToolBar toolBar;
	private SimpleGrid<ProfessorDTO> gridPanel;
	private SimpleFilter filter;
	private TextField<String> cpfBuscaTF;
	private TipoContratoComboBox tipoContratoBuscaCB;
	private TitulacaoComboBox titulacaoBuscaCB;
	private AreaTitulacaoComboBox areaTitulacaoBuscaCB;
	private Button disponibilidadeBT;
	private ContentPanel panel;
	private GTabItem tabItem;

	public ProfessoresView() {
		initUI();
	}

	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Professores");
		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent(tabItem);
	}

	private void createTabItem() {
		tabItem = new GTabItem("Professores", Resources.DEFAULTS.professor16());
		tabItem.setContent(panel);
	}

	private void createToolBar() {
		toolBar = new SimpleToolBar(this);
		toolBar.add(new SeparatorToolItem());
		disponibilidadeBT = toolBar.createButton(
				"Disponibilidade do Professor",
				Resources.DEFAULTS.disponibilidade16());
		toolBar.add(disponibilidadeBT);
		panel.setTopComponent(toolBar);
	}

	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
		bld.setMargins(new Margins(5, 5, 5, 5));

		gridPanel = new SimpleGrid<ProfessorDTO>(getColumnList(), this);
		panel.add(gridPanel, bld);
	}

	public List<ColumnConfig> getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_CPF, getI18nConstants().cpf(), 100 ) );
		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_NOME, getI18nConstants().nomeProfessor()	, 100 ) );
		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_TIPO_CONTRATO_STRING, getI18nConstants().tipoContrato(), 100 ) );
		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_CARGA_HORARIA_MIN, getI18nConstants().cargaHorariaMin(), 100 ) );
		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_CARGA_HORARIA_MAX, getI18nConstants().cargaHorariaMax(), 100 ) );
		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_TITULACAO_STRING, getI18nConstants().titulacao(), 100 ) );
		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_AREA_TITULACAO_STRING, getI18nConstants().areaTitulacao(), 100 ) );
		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_NOTA_DESEMPENHO, getI18nConstants().notaDesempenho(), 100 ) );
		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_CREDITO_ANTERIOR, getI18nConstants().cargaHorariaAnterior(), 100 ) );
		list.add( new ColumnConfig( ProfessorDTO.PROPERTY_VALOR_CREDITO, getI18nConstants().valorCredito(), 100 ) );

		return list;
	}

	private void createFilter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.EAST);
		bld.setMargins(new Margins(5, 5, 5, 0));
		bld.setCollapsible(true);

		filter = new SimpleFilter();

		cpfBuscaTF = new TextField<String>();
		cpfBuscaTF.setFieldLabel("CPF");
		tipoContratoBuscaCB = new TipoContratoComboBox();
		titulacaoBuscaCB = new TitulacaoComboBox();
		areaTitulacaoBuscaCB = new AreaTitulacaoComboBox();

		filter.addField(cpfBuscaTF);
		filter.addField(tipoContratoBuscaCB);
		filter.addField(titulacaoBuscaCB);
		filter.addField(areaTitulacaoBuscaCB);

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
	public SimpleGrid<ProfessorDTO> getGrid() {
		return gridPanel;
	}

	@Override
	public void setProxy(RpcProxy<PagingLoadResult<ProfessorDTO>> proxy) {
		gridPanel.setProxy(proxy);
	}

	@Override
	public TextField<String> getCpfBuscaTextField() {
		return cpfBuscaTF;
	}

	@Override
	public TipoContratoComboBox getTipoContratoBuscaComboBox() {
		return tipoContratoBuscaCB;
	}

	@Override
	public TitulacaoComboBox getTitulacaoBuscaComboBox() {
		return titulacaoBuscaCB;
	}

	@Override
	public AreaTitulacaoComboBox getAreaTitulacaoBuscaComboBox() {
		return areaTitulacaoBuscaCB;
	}

	@Override
	public Button getSubmitBuscaButton() {
		return filter.getSubmitButton();
	}

	@Override
	public Button getResetBuscaButton() {
		return filter.getResetButton();
	}

	@Override
	public Button getDisponibilidadeButton() {
		return disponibilidadeBT;
	}

}
