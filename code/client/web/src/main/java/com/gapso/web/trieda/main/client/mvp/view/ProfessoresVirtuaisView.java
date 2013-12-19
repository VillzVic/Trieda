package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessoresVirtuaisPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.TitulacaoComboBox;

public class ProfessoresVirtuaisView extends MyComposite
	implements ProfessoresVirtuaisPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< ProfessorVirtualDTO > gridPanel;
	private SimpleFilter filter;
	private TitulacaoComboBox titulacaoBuscaCB;
	private ContentPanel panel;
	private GTabItem tabItem;
	private CenarioDTO cenarioDTO;
	
	public ProfessoresVirtuaisView( CenarioDTO cenarioDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.initUI();
	}
	
	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeadingHtml( cenarioDTO.getNome() + " » Professores Virtuais" );
	
		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( this.tabItem );
	}
	
	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			"Professores", Resources.DEFAULTS.professor16() );
	
		this.tabItem.setContent( this.panel );
	}
	
	private void createToolBar()
	{
	}
	
	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
		bld.setMargins( new Margins( 5, 5, 5, 5 ) );
	
		this.gridPanel = new SimpleGrid< ProfessorVirtualDTO >( getColumnList(), this, this.toolBar );
		this.panel.add( this.gridPanel, bld );
	}
	
	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST );
	
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );
	
		this.filter = new SimpleFilter();
	
		this.titulacaoBuscaCB = new TitulacaoComboBox( cenarioDTO );
	
		this.filter.addField( this.titulacaoBuscaCB );
	
		this.panel.add( this.filter, bld );
	}
	
	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();
	
		list.add( new ColumnConfig(
			ProfessorVirtualDTO.PROPERTY_NOME, getI18nConstants().professorVirtual(), 100 ) );
	
		list.add( new ColumnConfig( ProfessorVirtualDTO.PROPERTY_TITULACAO_STRING,
			getI18nConstants().titulacao(), 115 ) );
	
		return list;
	}
	
	@Override
	public SimpleGrid< ProfessorVirtualDTO > getGrid()
	{
		return this.gridPanel;
	}
	
	@Override
	public void setProxy( RpcProxy< PagingLoadResult< ProfessorVirtualDTO > > proxy )
	{
		this.gridPanel.setProxy( proxy );
	}
	
	@Override
	public TitulacaoComboBox getTitulacaoBuscaComboBox()
	{
		return this.titulacaoBuscaCB;
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
