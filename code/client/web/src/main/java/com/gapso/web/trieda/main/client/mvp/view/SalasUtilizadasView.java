package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaUtilizadaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.main.client.mvp.presenter.SalasUtilizadasPresenter;

public class SalasUtilizadasView extends MyComposite
	implements SalasUtilizadasPresenter.Display{

	private CenarioDTO cenarioDTO;
	private ContentPanel panel;
	private SimpleGrid< SalaUtilizadaDTO > gridPanel;
	private GTabItem tabItem;
	private SimpleToolBar toolBar;

	public SalasUtilizadasView( CenarioDTO cenarioDTO)
	{
		this.cenarioDTO = cenarioDTO;
		this.initUI();
	}
	
	private void initUI()
	{

		createPanel();
		createToolBar();
		createGrid();
		createTabItem();
		initComponent( this.tabItem );
	}
	
	protected void createPanel() {
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeadingHtml( cenarioDTO.getNome() + " » Ambientes Utilizados" );
	}
	
	
	private void createToolBar()
	{
		this.toolBar = new SimpleToolBar( false, false, false, false, false, this );
		this.toolBar.add( new SeparatorToolItem() );
	
		this.panel.setTopComponent( this.toolBar );
		
		this.toolBar.activateEmptyState();
	}
	
	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
		bld.setMargins( new Margins( 5, 5, 5, 5 ) );

		this.gridPanel = new SimpleGrid< SalaUtilizadaDTO >( getColumnList(), this, this.toolBar );
		this.panel.add( this.gridPanel, bld );
	}
	
	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			"Ambientes Utilizados", Resources.DEFAULTS.sala16() );

		this.tabItem.setContent( this.panel );
	}
	
	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( SalaUtilizadaDTO.PROPERTY_CODIGO,
			getI18nConstants().codigoSala(), 90 ) );

		list.add( new ColumnConfig( SalaUtilizadaDTO.PROPERTY_CAMPUS,
			getI18nConstants().campus(), 60 ) );

		list.add( new ColumnConfig( SalaUtilizadaDTO.PROPERTY_UNIDADE,
			getI18nConstants().unidade(), 70 ) );

		list.add( new ColumnConfig( SalaUtilizadaDTO.PROPERTY_ANDAR,
			getI18nConstants().andar(), 60 ) );

		list.add( new ColumnConfig( SalaUtilizadaDTO.PROPERTY_NUMERO,
			getI18nConstants().numero(), 60 ) );

		list.add( new ColumnConfig( SalaUtilizadaDTO.PROPERTY_CARGA_HORARIA,
			"Carga Horaria Semanal Alocada", 170 ) );

		list.add( new ColumnConfig( SalaUtilizadaDTO.PROPERTY_UTILZIACAO_HORARIO,
			"Utilização Média dos Horários", 170 ) );

		list.add( new ColumnConfig( SalaUtilizadaDTO.PROPERTY_OCUPACAO_CAPACIDADE,
			"Ocupação Média da Capacidade", 170 ) );

		return list;
	}
	
	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< SalaUtilizadaDTO > > proxy )
	{
		gridPanel.setProxy( proxy );
	}
	
	@Override
	public SimpleGrid< SalaUtilizadaDTO > getGrid()
	{
		return this.gridPanel;
	}
}
