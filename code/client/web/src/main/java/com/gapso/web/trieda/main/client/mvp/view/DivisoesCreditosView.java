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
import com.gapso.web.trieda.main.client.mvp.presenter.DivisoesCreditosPresenter;
import com.gapso.web.trieda.shared.dtos.DivisaoCreditoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class DivisoesCreditosView
	extends MyComposite
	implements DivisoesCreditosPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< DivisaoCreditoDTO > gridPanel;
	private ContentPanel panel;
	private GTabItem tabItem;

	public DivisoesCreditosView()
	{
		this.initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeadingHtml( "Master Data » Divisões de Créditos" );

		this.createToolBar();
		this.createGrid();
		this.createTabItem();
		this.initComponent( this.tabItem );
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem( "Divisões de Créditos",
			Resources.DEFAULTS.divisaoDeCreditos16() );

		this.tabItem.setContent( this.panel );
	}

	private void createToolBar()
	{
		this.toolBar = new SimpleToolBar( this );
		this.panel.setTopComponent( this.toolBar );
	}

	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );

	    this.gridPanel = new SimpleGrid< DivisaoCreditoDTO >( getColumnList(), this );
	    this.panel.add( this.gridPanel, bld );
	}

	private List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( createColumnConfig( DivisaoCreditoDTO.PROPERTY_TOTAL_CREDITOS, "Créditos", 50 ) );
		list.add( createColumnConfig( DivisaoCreditoDTO.PROPERTY_DIA1, "Dia 1", 50 ) );
		list.add( createColumnConfig( DivisaoCreditoDTO.PROPERTY_DIA2, "Dia 2", 50 ) );
		list.add( createColumnConfig( DivisaoCreditoDTO.PROPERTY_DIA3, "Dia 3", 50 ) );
		list.add( createColumnConfig( DivisaoCreditoDTO.PROPERTY_DIA4, "Dia 4", 50 ) );
		list.add( createColumnConfig( DivisaoCreditoDTO.PROPERTY_DIA5, "Dia 5", 50 ) );
		list.add( createColumnConfig( DivisaoCreditoDTO.PROPERTY_DIA6, "Dia 6", 50 ) );
		list.add( createColumnConfig( DivisaoCreditoDTO.PROPERTY_DIA7, "Dia 7", 50 ) );

		return list;
	}

	private ColumnConfig createColumnConfig(
		String id, String text, int width )
	{
		ColumnConfig cc = new ColumnConfig( id, text, width );
		cc.setSortable( false );
		cc.setMenuDisabled( true );

		return cc;
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
	public Button getImportExcelButton()
	{
		return this.toolBar.getImportExcelButton();
	}

	@Override
	public Button getExportExcelButton()
	{
		return this.toolBar.getExportExcelButton();
	}

	@Override
	public SimpleGrid< DivisaoCreditoDTO > getGrid()
	{
		return this.gridPanel;
	}

	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< DivisaoCreditoDTO > > proxy )
	{
		this.gridPanel.setProxy( proxy );
	}
}
