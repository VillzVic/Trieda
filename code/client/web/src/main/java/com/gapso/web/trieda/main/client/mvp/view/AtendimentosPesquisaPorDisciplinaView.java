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
import com.gapso.web.trieda.main.client.mvp.presenter.AtendimentosPesquisaPorDisciplinaPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.PesquisaPorDisciplinaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class AtendimentosPesquisaPorDisciplinaView extends MyComposite
	implements AtendimentosPesquisaPorDisciplinaPresenter.Display
{

	private SimpleToolBar toolBar;
	private SimpleGrid< PesquisaPorDisciplinaDTO > gridPanel;
	private SimpleFilter filter;
	private TextField< String > codigoBuscaTextField;
	private CampusComboBox campusBuscaComboBox;
	private TextField< String > turmaBuscaTextField;
	private TextField< String > nomeBuscaTextField;
	private ContentPanel panel;
	private GTabItem tabItem;
	private CenarioDTO cenarioDTO;

	public AtendimentosPesquisaPorDisciplinaView( CenarioDTO cenarioDTO )
	{
		this.cenarioDTO = cenarioDTO;
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
		panel.setHeadingHtml( cenarioDTO.getNome() + " » Pesquisa por Disciplina" );
	}

	@Override
	protected void beforeRender()
	{
		super.beforeRender();
	}
	
	private void createTabItem()
	{
		tabItem = new GTabItem(
			"Pesquisa por Disciplina", Resources.DEFAULTS.resumoDisciplinas16() );

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
		
		this.nomeBuscaTextField = new TextField<String>();
		this.nomeBuscaTextField.setFieldLabel("Nome");
		this.campusBuscaComboBox = new CampusComboBox(cenarioDTO);
		this.campusBuscaComboBox.setFieldLabel("Campus");
		this.codigoBuscaTextField = new TextField<String>();
		this.codigoBuscaTextField.setFieldLabel("Codigo da Disciplina");
		this.turmaBuscaTextField = new TextField<String>();
		this.turmaBuscaTextField.setFieldLabel("Turma");
		
		this.filter.addField( this.campusBuscaComboBox );
		this.filter.addField( this.nomeBuscaTextField );
		this.filter.addField( this.codigoBuscaTextField );
		this.filter.addField( this.turmaBuscaTextField );

		this.panel.add( this.filter, bld );
	}
	
	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );

	    this.gridPanel = new SimpleGrid< PesquisaPorDisciplinaDTO >( getColumnList(), this, this.toolBar );
	    this.panel.add( this.gridPanel, bld );
	}
	
	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();
		
		list.add( new ColumnConfig( PesquisaPorDisciplinaDTO.PROPERTY_CURSO_STRING, getI18nConstants().codigoCurso(), 100 ) );
		list.add( new ColumnConfig( PesquisaPorDisciplinaDTO.PROPERTY_DISCIPLINA_CODIGO, getI18nConstants().codigoDisciplina(), 100 ) );
		list.add( new ColumnConfig( PesquisaPorDisciplinaDTO.PROPERTY_DISCIPLINA_NOME, getI18nConstants().nomeDisciplina(), 120 ) );
		list.add( new ColumnConfig( PesquisaPorDisciplinaDTO.PROPERTY_TURMA_STRING, getI18nConstants().turma(), 120 ) );
		list.add( new ColumnConfig( PesquisaPorDisciplinaDTO.PROPERTY_DIA_SEMANA_STRING, getI18nConstants().dia(), 100 ) );
		list.add( new ColumnConfig( PesquisaPorDisciplinaDTO.PROPERTY_HORARIO_INICIAL, "Hora Inicial",80 ) );
		list.add( new ColumnConfig( PesquisaPorDisciplinaDTO.PROPERTY_HORARIO_FINAL, "Hora Final",80 ) );
		list.add( new ColumnConfig( PesquisaPorDisciplinaDTO.PROPERTY_QUANTIDADE_ALUNOS_INT, "Quantidade de Alunos", 100 ) );
		list.add( new ColumnConfig( PesquisaPorDisciplinaDTO.PROPERTY_SALA_STRING, "Ambiente",80 ) );
		list.add( new ColumnConfig( PesquisaPorDisciplinaDTO.PROPERTY_PROFESSOR_STRING, "Professor",130 ) );

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
	public TextField<String> getTurmaBuscaTextField() {
		return this.turmaBuscaTextField;
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
	public SimpleGrid<PesquisaPorDisciplinaDTO> getGrid() {
		return this.gridPanel;
	}

	@Override
	public void setProxy(
			RpcProxy<PagingLoadResult<PesquisaPorDisciplinaDTO>> proxy) {
		this.gridPanel.setProxy( proxy );
		
	}

	@Override
	public TextField< String > getNomeBuscaTextField()
	{
		return this.nomeBuscaTextField;
	}

}
