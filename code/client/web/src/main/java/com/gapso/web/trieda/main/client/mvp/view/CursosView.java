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
import com.gapso.web.trieda.main.client.mvp.presenter.CursosPresenter;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.TipoCursoComboBox;

public class CursosView
	extends MyComposite
	implements CursosPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< CursoDTO > gridPanel;
	private SimpleFilter filter;
	private TextField< String > nomeBuscaTextField;
	private TextField< String > codigoBuscaTextField;
	private TipoCursoComboBox tipoCursoBuscaComboBox;
	private Button curriculosBT;
	private ContentPanel panel;
	private GTabItem tabItem;

	public CursosView()
	{
		this.initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeading( "Master Data » Cursos" );

		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( this.tabItem );
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			"Cursos", Resources.DEFAULTS.curso16() );

		this.tabItem.setContent( this.panel );
	}
	
	private void createToolBar()
	{
		this.toolBar = new SimpleToolBar( this );
		this.toolBar.add( new SeparatorToolItem() );

		this.curriculosBT = toolBar.createButton(
			"Matrizes Curriculares",
			Resources.DEFAULTS.matrizCurricular16() );

		this.toolBar.add( this.curriculosBT );
		this.panel.setTopComponent( this.toolBar );
	}

	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );

	    this.gridPanel = new SimpleGrid< CursoDTO >( getColumnList(), this );
	    this.panel.add( this.gridPanel, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( CursoDTO.PROPERTY_NOME, getI18nConstants().nomeCurso(), 100 ) );
		list.add( new ColumnConfig( CursoDTO.PROPERTY_CODIGO, getI18nConstants().codigoCurso(), 100 ) );
		list.add( new ColumnConfig( CursoDTO.PROPERTY_TIPO_STRING, getI18nConstants().tipoCurso(), 100 ) );
		list.add( new ColumnConfig( CursoDTO.PROPERTY_NUM_MIN_DOUTORES, getI18nConstants().minPercentualDoutor(), 100 ) );
		list.add( new ColumnConfig( CursoDTO.PROPERTY_NUM_MIN_MESTRES, getI18nConstants().minPercentualMestre(), 100 ) );
		list.add( new ColumnConfig( CursoDTO.PROPERTY_MIN_TEMPO_INTEGRAL_PARCIAL, getI18nConstants().minTempoIntegralParcial(), 100 ) );
		list.add( new ColumnConfig( CursoDTO.PROPERTY_MIN_TEMPO_INTEGRAL, getI18nConstants().minTempoIntegral(), 100 ) );
		list.add( new ColumnConfig( CursoDTO.PROPERTY_MAX_DISCIPLINAS_PELO_PROFESSOR, getI18nConstants().maxDisciplinasProfessor(), 110 ) );
		list.add( new CheckColumnConfig( CursoDTO.PROPERTY_ADM_MAIS_DE_UMA_DISCIPLINA, getI18nConstants().maisDeUmaDisciplinaProfessor(), 200 ) );

		return list;
	}

	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST );
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		this.filter = new SimpleFilter();
		this.tipoCursoBuscaComboBox = new TipoCursoComboBox();
		this.tipoCursoBuscaComboBox.setFieldLabel( "Tipo" );
		this.nomeBuscaTextField = new TextField< String >();
		this.nomeBuscaTextField.setFieldLabel( "Nome" );
		this.codigoBuscaTextField = new TextField< String >();
		this.codigoBuscaTextField.setFieldLabel( "Código" );

		this.filter.addField( this.tipoCursoBuscaComboBox );
		this.filter.addField( this.nomeBuscaTextField );
		this.filter.addField( this.codigoBuscaTextField );

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
	public SimpleGrid< CursoDTO > getGrid()
	{
		return this.gridPanel;
	}
	
	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< CursoDTO > > proxy )
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
	public TipoCursoComboBox getTipoCursoBuscaComboBox()
	{
		return this.tipoCursoBuscaComboBox;
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
	public Button getCurriculosButton()
	{
		return this.curriculosBT;
	}
}
