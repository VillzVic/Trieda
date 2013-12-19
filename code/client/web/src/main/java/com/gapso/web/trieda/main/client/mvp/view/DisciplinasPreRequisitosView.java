package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.main.client.mvp.presenter.DisciplinasPreRequisitosPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaRequisitoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.DisciplinaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class DisciplinasPreRequisitosView
	extends MyComposite
	implements DisciplinasPreRequisitosPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< DisciplinaRequisitoDTO > gridPanel;
	private SimpleFilter filter;
	private NumberField periodoBuscaTextField;
	private DisciplinaAutoCompleteBox disciplinaBuscaComboBox;
	private CurriculoComboBox curriculoBuscaComboBox;
	private ContentPanel panel;
	private GTabItem tabItem;
	private CenarioDTO cenarioDTO;
	
	public DisciplinasPreRequisitosView( CenarioDTO cenarioDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.initUI();
	}
	
	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeadingHtml( cenarioDTO.getNome() + " » Disciplinas Pré-Requisitos" );
	
		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( this.tabItem );
	}
	
	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			"Disciplinas Pré-Requisitos", Resources.DEFAULTS.disciplinaCurriculo16() );
	
		this.tabItem.setContent( this.panel );
	}
	
	private void createToolBar()
	{
		this.toolBar = new SimpleToolBar( true, false, true, true, true, this );
		this.toolBar.add( new SeparatorToolItem() );
	
		this.panel.setTopComponent( this.toolBar );
		
		this.toolBar.activateEmptyState();
	}
	
	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );
	
	    this.gridPanel = new SimpleGrid< DisciplinaRequisitoDTO >( getColumnList(), this, this.toolBar );
	    this.panel.add( this.gridPanel, bld );
	}
	
	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();
	
		list.add( new ColumnConfig( DisciplinaRequisitoDTO.PROPERTY_CURRICULO_STRING, getI18nConstants().codigoCurriculo(), 200 ) );
		list.add( new ColumnConfig( DisciplinaRequisitoDTO.PROPERTY_PERIODO, getI18nConstants().periodo(), 100 ) );
		list.add( new ColumnConfig( DisciplinaRequisitoDTO.PROPERTY_DISCIPLINA_STRING, getI18nConstants().codigoDisciplina(), 100 ) );
		list.add( new ColumnConfig( DisciplinaRequisitoDTO.PROPERTY_DISCIPLINA_REQUISITO_STRING, getI18nConstants().codigoDisciplinaPreRequisito(), 200 ) );
	
		return list;
	}
	
	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST );
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );
	
		this.filter = new SimpleFilter();
		this.periodoBuscaTextField = new NumberField();
		this.periodoBuscaTextField.setFieldLabel("Periodo");
		this.disciplinaBuscaComboBox = new DisciplinaAutoCompleteBox( cenarioDTO );
		this.disciplinaBuscaComboBox.setFieldLabel("Disciplina");
		this.curriculoBuscaComboBox = new CurriculoComboBox( cenarioDTO );
		this.curriculoBuscaComboBox.setFieldLabel("Currículo");
		
		this.filter.addField( this.periodoBuscaTextField );
		this.filter.addField( this.disciplinaBuscaComboBox );
		this.filter.addField( this.curriculoBuscaComboBox );
	
		this.panel.add( this.filter, bld );
	}
	
	@Override
	public Button getNewButton()
	{
		return this.toolBar.getNewButton();
	}
	
	@Override
	public Button getRemoveButton()
	{
		return this.toolBar.getRemoveButton();
	}
	
	@Override
	public Button getImportExcelButton()
	{
		return this.toolBar.getImportExcelButton();
	}
	
	@Override
	public MenuItem getExportXlsExcelButton()
	{
		return (MenuItem) this.toolBar.getExportExcelButton().getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getExportXlsxExcelButton()
	{
		return (MenuItem) this.toolBar.getExportExcelButton().getMenu().getItem(1);
	}
	
	@Override
	public SimpleGrid< DisciplinaRequisitoDTO > getGrid()
	{
		return this.gridPanel;
	}
	
	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< DisciplinaRequisitoDTO > > proxy )
	{
		this.gridPanel.setProxy( proxy );
	}
	
	@Override
	public NumberField getPeriodoBuscaTextField()
	{
		return this.periodoBuscaTextField;
	}
	
	@Override
	public DisciplinaAutoCompleteBox getDisciplinaBuscaComboBox()
	{
		return this.disciplinaBuscaComboBox;
	}
	
	@Override
	public CurriculoComboBox getCurriculoBuscaComboBox()
	{
		return this.curriculoBuscaComboBox;
	}
	
	@Override
	public Button getSubmitBuscaButton()
	{
		return this.filter.getSubmitButton();
	}
	
	@Override
	public Button getResetBuscaButton()
	{
		return this.filter.getResetButton();
	}
}

