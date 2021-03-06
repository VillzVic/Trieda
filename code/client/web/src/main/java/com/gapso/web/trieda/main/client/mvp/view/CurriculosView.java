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
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.main.client.mvp.presenter.CurriculosPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SemanaLetivaComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class CurriculosView extends MyComposite
	implements CurriculosPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< CurriculoDTO > gridPanel;
	private SimpleFilter filter;
	private TextField< String > codigoBuscaTextField;
	private TextField< String > descricaoBuscaTextField;
	private TextField< String > periodoTextField;
	private SemanaLetivaComboBox semanaLetivaComboBox;
	private CursoComboBox cursoBuscaComboBox;
	private ContentPanel panel;
	private GTabItem tabItem;
	private Button associarDisciplinasBT;

	private CursoDTO cursoDTO;
	private CenarioDTO cenarioDTO;

	public CurriculosView( CenarioDTO cenarioDTO )
	{
		this( cenarioDTO, null );
	}

	public CurriculosView( CenarioDTO cenarioDTO, CursoDTO cursoDTO )
	{
		this.cenarioDTO	= cenarioDTO;
		this.cursoDTO = cursoDTO;
		initUI();
	}

	private void initUI()
	{
		panel = new ContentPanel( new BorderLayout() );
		panel.setHeadingHtml( cenarioDTO.getNome() + " » Matrizes Curriculares" );

		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( tabItem );
	}

	private void createTabItem()
	{
		tabItem = new GTabItem( "Matrizes Curriculares",
			Resources.DEFAULTS.matrizCurricular16() );

		tabItem.setContent( panel );
	}

	private void createToolBar()
	{
		toolBar = new SimpleToolBar( this );
		toolBar.add( new SeparatorToolItem() );
		associarDisciplinasBT = toolBar.createButton(
			"Associar Disciplinas", Resources.DEFAULTS.disciplina16() );
		toolBar.add( associarDisciplinasBT );
		panel.setTopComponent( toolBar );
	}

	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
		bld.setMargins( new Margins( 5, 5, 5, 5 ) );

		gridPanel = new SimpleGrid< CurriculoDTO >( getColumnList(), this, this.toolBar );
		panel.add( gridPanel, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( CurriculoDTO.PROPERTY_CURSO_STRING, getI18nConstants().codigoCurso(), 250 ) );
		list.add( new ColumnConfig( CurriculoDTO.PROPERTY_CODIGO, getI18nConstants().codigoCurriculo(), 120 ) );
		list.add( new ColumnConfig( CurriculoDTO.PROPERTY_DESCRICAO, getI18nConstants().descricaoCurriculo(), 150 ) );
		list.add( createColumnConfig( CurriculoDTO.PROPERTY_PERIODOS, getI18nConstants().periodos(), 110, false ) );
		list.add( new ColumnConfig( CurriculoDTO.PROPERTY_SEMANA_LETIVA_STRING, getI18nConstants().semanaLetiva(), 110 ) );

		return list;
	}
	
	private ColumnConfig createColumnConfig(
			String id, String text, int width, boolean sortable )
	{
		ColumnConfig cc = new ColumnConfig( id, text, width );
		cc.setSortable( sortable );

		return cc;
	}

	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST, 350 );
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		filter = new SimpleFilter();
		filter.setLabelWidth(125);
		cursoBuscaComboBox = new CursoComboBox( cenarioDTO );
		cursoBuscaComboBox.setFieldLabel( "Curso" );
		cursoBuscaComboBox.setValue( cursoDTO );
		codigoBuscaTextField = new TextField< String >();
		codigoBuscaTextField.setFieldLabel( "Código" );
		descricaoBuscaTextField = new TextField< String >();
		descricaoBuscaTextField.setFieldLabel( "Descricao" );
		periodoTextField = new TextField< String >();
		periodoTextField.setFieldLabel( "Período" );
		semanaLetivaComboBox = new SemanaLetivaComboBox(cenarioDTO);
		semanaLetivaComboBox.setFieldLabel(getI18nConstants().semanaLetiva());
		

		filter.addField( cursoBuscaComboBox );
		filter.addField( codigoBuscaTextField );
		filter.addField( descricaoBuscaTextField );
		filter.addField( periodoTextField );
		filter.addField( semanaLetivaComboBox );

		panel.add( filter, bld );
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
	public SimpleGrid< CurriculoDTO > getGrid()
	{
		return gridPanel;
	}

	@Override
	public void setProxy( RpcProxy< PagingLoadResult< CurriculoDTO > > proxy )
	{
		gridPanel.setProxy( proxy );
	}

	@Override
	public TextField< String > getDescricaoBuscaTextField()
	{
		return descricaoBuscaTextField;
	}

	@Override
	public TextField< String > getCodigoBuscaTextField()
	{
		return codigoBuscaTextField;
	}

	@Override
	public CursoComboBox getCursoBuscaComboBox()
	{
		return cursoBuscaComboBox;
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

	@Override
	public Button getAssociarDisciplinasButton()
	{
		return associarDisciplinasBT;
	}

	@Override
	public SemanaLetivaComboBox getSemanaLetivaComboBox() {
		return semanaLetivaComboBox;
	}
	
	@Override
	public TextField<String> getPeriodoTextField() {
		return periodoTextField;
	}
}
