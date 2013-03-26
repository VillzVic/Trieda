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
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.presenter.ResumoAtendimentosDisciplinaPresenter;
import com.gapso.web.trieda.shared.dtos.ResumoMatriculaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class ResumoAtendimentosDisciplinaView extends MyComposite
	implements ResumoAtendimentosDisciplinaPresenter.Display
{

	private SimpleToolBar toolBar;
	private SimpleGrid< ResumoMatriculaDTO > gridPanel;
	private SimpleFilter filter;
	private TextField< String > codigoBuscaTextField;
	private CampusComboBox campusBuscaComboBox;
	private CursoComboBox cursoBuscaComboBox;
	private ContentPanel panel;
	private GTabItem tabItem;

	public ResumoAtendimentosDisciplinaView()
	{
			initUI();
			createToolBar();
			createGrid();
			createFilter();
			createTabItem();
			initComponent( this.tabItem );
	}
	
	private void initUI()
	{
		panel = new ContentPanel( new BorderLayout() );
		panel.setHeading( "Master Data » Resumo de Atendimentos por Disciplina" );
	}

	@Override
	protected void beforeRender()
	{
		super.beforeRender();
	}
	
	private void createTabItem()
	{
		tabItem = new GTabItem(
			"Atendimentos por Disciplina", Resources.DEFAULTS.resumoDisciplinas16() );

		tabItem.setContent( panel );
	}
	
	private void createToolBar()
	{
		// Exibe apenas o botão 'exportExcel'
		this.toolBar = new SimpleToolBar(
			false, false, false, false, true, this );

		this.panel.setTopComponent( this.toolBar );
	}
	
	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST );
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		this.filter = new SimpleFilter();
		this.codigoBuscaTextField = new TextField<String>();
		this.codigoBuscaTextField.setFieldLabel("Codigo da Disciplina");
		this.cursoBuscaComboBox = new CursoComboBox();
		this.cursoBuscaComboBox.setFieldLabel("Curso");
		this.campusBuscaComboBox = new CampusComboBox();
		this.campusBuscaComboBox.setFieldLabel("Campus");
		
		this.filter.addField( this.codigoBuscaTextField );
		this.filter.addField( this.cursoBuscaComboBox );
		this.filter.addField( this.campusBuscaComboBox );

		this.panel.add( this.filter, bld );
	}
	
	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );

	    this.gridPanel = new SimpleGrid< ResumoMatriculaDTO >( getColumnList(), this );
	    this.panel.add( this.gridPanel, bld );
	}
	
	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();
		
		list.add( new ColumnConfig( ResumoMatriculaDTO.PROPERTY_CAMPUS_STRING, getI18nConstants().campus(), 120 ) );
		list.add( new ColumnConfig( ResumoMatriculaDTO.PROPERTY_CODIGO_DISCIPLINA, getI18nConstants().codigoDisciplina(), 100 ) );
		list.add( new ColumnConfig( ResumoMatriculaDTO.PROPERTY_DISCIPLINA_DEMANDA_P1, "Demanda P1",80 ) );
		list.add( new ColumnConfig( ResumoMatriculaDTO.PROPERTY_DISCIPLINA_ATENDIDOS_P1, "Atendidos P1", 80 ) );
		list.add( new ColumnConfig( ResumoMatriculaDTO.PROPERTY_DISCIPLINA_NAO_ATENDIDOS_P1, "Nao Atendidos P1", 100 ) );
		list.add( new ColumnConfig( ResumoMatriculaDTO.PROPERTY_DISCIPLINA_ATENDIDOS_P2, "Atendidos P2", 80 ) );
		list.add( new ColumnConfig( ResumoMatriculaDTO.PROPERTY_DISCIPLINA_ATENDIDOS_SOMA, "Atendidos P1 + P2", 100 ) );
		list.add( new ColumnConfig( ResumoMatriculaDTO.PROPERTY_DISCIPLINA_DEMANDA_NAO_ATENDIDA, "Demanda P1 - Atendidos", 130 ) );

		return list;
	}

	@Override
	public MenuItem getExportXlsExcelButton() {
		return (MenuItem) this.toolBar.getExportExcelButton().getMenu().getItem(0);
	}

	@Override
	public MenuItem getExportXlsxExcelButton() {
		return (MenuItem) this.toolBar.getExportExcelButton().getMenu().getItem(1);
	}

	@Override
	public TextField<String> getCodigoBuscaTextField() {
		return this.codigoBuscaTextField;
	}

	@Override
	public CampusComboBox getCampusBuscaComboBox() {
		return this.campusBuscaComboBox;
	}

	@Override
	public CursoComboBox getCursoBuscaComboBox() {
		return this.cursoBuscaComboBox;
	}

	@Override
	public Button getSubmitBuscaButton() {
		return this.filter.getSubmitButton();
	}

	@Override
	public Button getResetBuscaButton() {
		return this.filter.getResetButton();
	}

	@Override
	public SimpleGrid<ResumoMatriculaDTO> getGrid() {
		return this.gridPanel;
	}

	@Override
	public void setProxy(RpcProxy<PagingLoadResult<ResumoMatriculaDTO>> proxy) {
		this.gridPanel.setProxy(proxy);
		
	}
}
