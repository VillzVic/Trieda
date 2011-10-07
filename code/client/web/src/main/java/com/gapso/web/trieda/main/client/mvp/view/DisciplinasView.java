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
import com.gapso.web.trieda.main.client.mvp.presenter.DisciplinasPresenter;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.TipoDisciplinaComboBox;

public class DisciplinasView
	extends MyComposite
	implements DisciplinasPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< DisciplinaDTO > gridPanel;
	private SimpleFilter filter;
	private TextField< String > nomeBuscaTextField;
	private TextField< String > codigoBuscaTextField;
	private TipoDisciplinaComboBox tipoDisciplinaBuscaComboBox;
	private Button disponibilidadeBT;
	private Button divisaoCreditoBT;
	private ContentPanel panel;
	private GTabItem tabItem;

	public DisciplinasView()
	{
		this.initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeading( "Master Data » Disciplinas" );

		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( this.tabItem );
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			"Disciplinas", Resources.DEFAULTS.disciplina16() );

		this.tabItem.setContent( this.panel );
	}

	private void createToolBar()
	{
		this.toolBar = new SimpleToolBar( this );
		this.toolBar.add( new SeparatorToolItem() );

		this.divisaoCreditoBT = this.toolBar.createButton(
			"Divisão de Créditos da disciplina",
			Resources.DEFAULTS.divisaoDeCreditos16() );

		this.toolBar.add( this.divisaoCreditoBT );

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

	    this.gridPanel = new SimpleGrid< DisciplinaDTO >( getColumnList(), this );
	    this.panel.add( this.gridPanel, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( DisciplinaDTO.PROPERTY_NOME, getI18nConstants().nomeDisciplina(), 250 ) );
		list.add( new ColumnConfig( DisciplinaDTO.PROPERTY_CODIGO, getI18nConstants().codigoDisciplina(), 100 ) );
		list.add( new ColumnConfig( DisciplinaDTO.PROPERTY_CREDITOS_TEORICO, getI18nConstants().creditosTeoricos(), 100 ) );
		list.add( new ColumnConfig( DisciplinaDTO.PROPERTY_CREDITOS_PRATICO, getI18nConstants().creditosPraticos(), 100 ) );
		list.add( new CheckColumnConfig( DisciplinaDTO.PROPERTY_LABORATORIO, getI18nConstants().usaLaboratorio(), 100 ) );
		list.add( new ColumnConfig( DisciplinaDTO.PROPERTY_TIPO_STRING, getI18nConstants().tipoDisciplina(), 100 ) );
		list.add( new ColumnConfig( DisciplinaDTO.PROPERTY_DIFICULDADE, getI18nConstants().nivelDificuldade(), 100 ) );
		list.add( new ColumnConfig( DisciplinaDTO.PROPERTY_MAX_ALUNOS_TEORICO, getI18nConstants().maxAlunosTeorico(), 120 ) );
		list.add( new ColumnConfig( DisciplinaDTO.PROPERTY_MAX_ALUNOS_PRATICO, getI18nConstants().maxAlunosPratico(), 120 ) );

		return list;
	}

	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST );
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		this.filter = new SimpleFilter();
		this.nomeBuscaTextField = new TextField<String>();
		this.nomeBuscaTextField.setFieldLabel("Nome");
		this.codigoBuscaTextField = new TextField<String>();
		this.codigoBuscaTextField.setFieldLabel("Código");
		this.tipoDisciplinaBuscaComboBox = new TipoDisciplinaComboBox();
		this.tipoDisciplinaBuscaComboBox.setFieldLabel("Tipo de disciplina");
		
		this.filter.addField( this.nomeBuscaTextField );
		this.filter.addField( this.codigoBuscaTextField );
		this.filter.addField( this.tipoDisciplinaBuscaComboBox );

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
	public SimpleGrid< DisciplinaDTO > getGrid()
	{
		return this.gridPanel;
	}

	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< DisciplinaDTO > > proxy )
	{
		this.gridPanel.setProxy( proxy );
	}

	@Override
	public TextField< String > getNomeBuscaTextField()
	{
		return this.nomeBuscaTextField;
	}

	@Override
	public TextField< String > getCodigoBuscaTextField()
	{
		return this.codigoBuscaTextField;
	}

	@Override
	public TipoDisciplinaComboBox getTipoDisciplinaBuscaComboBox()
	{
		return this.tipoDisciplinaBuscaComboBox;
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
	public Button getDisponibilidadeButton()
	{
		return this.disponibilidadeBT;
	}

	@Override
	public Button getDivisaoCreditoButton()
	{
		return this.divisaoCreditoBT;
	}
}
