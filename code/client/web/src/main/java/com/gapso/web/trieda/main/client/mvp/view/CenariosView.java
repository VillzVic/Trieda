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
import com.extjs.gxt.ui.client.widget.grid.CheckColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.main.client.mvp.presenter.CenariosPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class CenariosView
	extends MyComposite
	implements CenariosPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid<CenarioDTO> gridPanel;
	private Button abrirCenarioBT;
	private Button clonarCenarioBT;
	private SimpleFilter filter;
	private TextField< Integer > anoBuscaTextField;
	private TextField< Integer > semestreBuscaTextField;
	private ContentPanel panel;
	private GTabItem tabItem;

	public CenariosView()
	{
		initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeading( "Master Data » Cenários" );

		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( this.tabItem );
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			"Cenário", Resources.DEFAULTS.cenario16() );

		this.tabItem.setContent( this.panel );
	}
	
	private void createToolBar()
	{
		this.toolBar = new SimpleToolBar( this );
		this.toolBar.add( new SeparatorToolItem() );

		this.clonarCenarioBT = toolBar.createButton(
			"Clonar cenário", Resources.DEFAULTS.cenarioClonar16() );

		this.toolBar.add( this.clonarCenarioBT );

		this.abrirCenarioBT = toolBar.createButton(
			"Adicionar na árvore de cenários",
			Resources.DEFAULTS.cenarioAbrir16() );

		this.toolBar.add( this.abrirCenarioBT );
		this.panel.setTopComponent( this.toolBar );
	}
	
	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );

	    this.gridPanel = new SimpleGrid< CenarioDTO >( getColumnList(), this );
	    this.panel.add( this.gridPanel, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new CheckColumnConfig( CenarioDTO.PROPERTY_OFICIAL, "Oficial", 40 ) );
		list.add( new ColumnConfig( CenarioDTO.PROPERTY_NOME, "Nome", 100 ) );
		list.add( new ColumnConfig( CenarioDTO.PROPERTY_ANO, "Ano", 100 ) );
		list.add( new ColumnConfig( CenarioDTO.PROPERTY_SEMESTRE, "Semestre", 100 ) );

		return list;
	}

	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST );
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		this.filter = new SimpleFilter();
		this.anoBuscaTextField = new TextField< Integer >();
		this.anoBuscaTextField.setFieldLabel( "Ano" );
		this.semestreBuscaTextField = new TextField< Integer >();
		this.semestreBuscaTextField.setFieldLabel( "Semestre" );
		this.filter.addField( this.anoBuscaTextField );
		this.filter.addField( this.semestreBuscaTextField ); 

		this.panel.add( this.filter, bld );
	}

	@Override
	public Button getNewButton()
	{
		return this.toolBar.getNewButton();
	}

	@Override
	public Button getEditButton()
	{
		return this.toolBar.getEditButton();
	}

	@Override
	public Button getRemoveButton()
	{
		return this.toolBar.getRemoveButton();
	}

	@Override
	public SimpleGrid< CenarioDTO > getGrid()
	{
		return this.gridPanel;
	}

	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< CenarioDTO > > proxy )
	{
		this.gridPanel.setProxy( proxy );
	}

	@Override
	public TextField< Integer > getAnoBuscaTextField()
	{
		return this.anoBuscaTextField;
	}

	@Override
	public TextField< Integer > getSemestreBuscaTextField()
	{
		return this.semestreBuscaTextField;
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

	@Override
	public Button getAbrirCenarioButton()
	{
		return this.abrirCenarioBT;
	}

	@Override
	public Button getClonarCenarioButton()
	{
		return this.clonarCenarioBT;
	}
}
