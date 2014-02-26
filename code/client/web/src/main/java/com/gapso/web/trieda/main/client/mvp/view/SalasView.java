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
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.main.client.mvp.presenter.SalasPresenter;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.UnidadeComboBox;
import com.google.gwt.i18n.client.NumberFormat;

public class SalasView
	extends MyComposite
	implements SalasPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< SalaDTO > gridPanel;
	private SimpleFilter filter;
	private ContentPanel panel;
	private GTabItem tabItem;
	private Button disciplinasAssociadasBT;
	private Button disponibilidadeBT;
	private UnidadeComboBox unidadeCB;
	private CampusComboBox campusCB;

	private CampusDTO campusDTO;
	private UnidadeDTO unidadeDTO;
	private CenarioDTO cenarioDTO;

	public SalasView( CenarioDTO cenarioDTO )
	{
		this( cenarioDTO, null, null );
	}

	public SalasView( CenarioDTO cenarioDTO,
		CampusDTO campusDTO, UnidadeDTO unidadeDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.campusDTO = campusDTO;
		this.unidadeDTO = unidadeDTO;

		initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeadingHtml( cenarioDTO.getNome() + " » Ambientes" );

		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( this.tabItem );
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			getI18nConstants().salas(), Resources.DEFAULTS.sala16() );

		this.tabItem.setContent( this.panel );
	}

	private void createToolBar()
	{
		this.toolBar = new SimpleToolBar( this );
		this.toolBar.add( new SeparatorToolItem() );

		this.disciplinasAssociadasBT = this.toolBar.createButton(
			getI18nConstants() .disciplinasAssociadas(),
			Resources.DEFAULTS.disciplina16() );

		this.toolBar.add( this.disciplinasAssociadasBT );
		this.disponibilidadeBT = this.toolBar.createButton(
			getI18nConstants().disponibilidadesSemanaLetiva(),
			Resources.DEFAULTS.disponibilidade16() );

		this.toolBar.add( this.disponibilidadeBT );
		this.panel.setTopComponent( this.toolBar );
	}

	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
		bld.setMargins( new Margins( 5, 5, 5, 5 ) );

		this.gridPanel = new SimpleGrid< SalaDTO >( getColumnList(), this, this.toolBar );
		this.panel.add( this.gridPanel, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( SalaDTO.PROPERTY_CODIGO,
			getI18nConstants().codigoSala(), 100 ) );

		list.add( new ColumnConfig( SalaDTO.PROPERTY_TIPO_STRING,
			getI18nConstants().tipo(), 100 ) );
		list.add( new ColumnConfig( SalaDTO.PROPERTY_UNIDADE_STRING,
			getI18nConstants().codigoUnidade(), 100 ) );
		list.add( new ColumnConfig( SalaDTO.PROPERTY_NUMERO,
			getI18nConstants().numero(), 100 ) );
		list.add( new ColumnConfig( SalaDTO.PROPERTY_DESCRICAO,
				getI18nConstants().descricao(), 100 ) );
		list.add( new ColumnConfig( SalaDTO.PROPERTY_ANDAR,
			getI18nConstants().andar(), 100 ) );
		list.add( createIntegerColumnConfig( SalaDTO.PROPERTY_CAPACIDADE_INSTALADA,
			getI18nConstants().capacidadeInstaladaAlunos(), 160, "É a capacidade do ambiente em número de alunos, em seu uso normal." ) );
		list.add( createIntegerColumnConfig( SalaDTO.PROPERTY_CAPACIDADE_MAX,
				getI18nConstants().capacidadeMaxAlunos(), 160, "É a capacidade máxima do ambiente, em número de alunos, em situações extremas." ) );
		list.add( createDecimalColumnConfig( SalaDTO.PROPERTY_CUSTO_OPERACAO_CRED,
				getI18nConstants().custoOperacaoCred(), 185, "Custo de uso do ambiente por tempo de aula. Pode considerar custo de aluguel, limpeza, energia elétrica, móveis e equipamentos instalados, etc." ) );

		return list;
	}

	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST );
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		this.filter = new SimpleFilter();

		this.campusCB = new CampusComboBox(cenarioDTO);
		this.campusCB.setValue( this.campusDTO );
		this.unidadeCB = new UnidadeComboBox( this.campusCB );
		this.unidadeCB.setValue( this.unidadeDTO );

		this.filter.addField( this.campusCB );
		this.filter.addField( this.unidadeCB );

		this.panel.add( this.filter, bld );
	}
	
	private ColumnConfig createIntegerColumnConfig(
			String id, String text, int width, String toolTip )
	{
		String pattern = "#,###";
		ColumnConfig cc = new ColumnConfig( id, text, width );
		cc.setNumberFormat(NumberFormat.getFormat(pattern));
		cc.setToolTip(toolTip);

		return cc;
	}
	
	private ColumnConfig createDecimalColumnConfig(
			String id, String text, int width, String toolTip)
	{
		String pattern = "#,##0.00";
		ColumnConfig cc = new ColumnConfig( id, text, width );
		cc.setNumberFormat(NumberFormat.getFormat(pattern));
		cc.setToolTip(toolTip);

		return cc;
	}

	@Override
	public Button getNewButton()
	{
		return toolBar.getNewButton();
	}

	@Override
	public Button getEditButton()
	{
		return toolBar.getEditButton();
	}

	@Override
	public Button getRemoveButton()
	{
		return toolBar.getRemoveButton();
	}

	@Override
	public Button getImportExcelButton()
	{
		return toolBar.getImportExcelButton();
	}

	@Override
	public MenuItem getExportXlsExcelButton()
	{
		return (MenuItem) toolBar.getExportExcelButton().getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getExportXlsxExcelButton()
	{
		return (MenuItem) toolBar.getExportExcelButton().getMenu().getItem(1);
	}

	@Override
	public GTabItem getGTabItem()
	{
		return tabItem;
	}

	@Override
	public SimpleGrid< SalaDTO > getGrid()
	{
		return gridPanel;
	}

	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< SalaDTO > > proxy )
	{
		gridPanel.setProxy( proxy );
	}

	@Override
	public Button getDisciplinasAssociadasButton()
	{
		return disciplinasAssociadasBT;
	}

	@Override
	public Button getDisponibilidadeButton()
	{
		return disponibilidadeBT;
	}

	@Override
	public UnidadeComboBox getUnidadeCB()
	{
		return unidadeCB;
	}

	@Override
	public CampusComboBox getCampusCB()
	{
		return campusCB;
	}

	@Override
	public Button getSubmitBuscaButton()
	{
		return filter.getSubmitButton();
	}

	@Override
	public Button getResetBuscaButton()
	{
		return filter.getResetButton();
	}
}
