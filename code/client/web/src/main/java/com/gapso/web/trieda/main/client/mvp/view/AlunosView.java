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
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.CheckColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.presenter.AlunosPresenter;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.ComboBoxBoolean;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class AlunosView
	extends MyComposite
	implements AlunosPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< AlunoDTO > gridPanel;
	private SimpleFilter filter;
	private TextField< String > nomeBuscaTextField;
	private TextField< String > matriculaBuscaTextField;
	private ComboBoxBoolean formando;
	private ComboBoxBoolean virtual;
	private NumberField periodoField;
	private ComboBoxBoolean criadoPeloTrieda;
	private ContentPanel panel;
	private GTabItem tabItem;
	private CenarioDTO cenarioDTO;

	public AlunosView( CenarioDTO cenarioDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeadingHtml( cenarioDTO.getNome() + " » Alunos" );

		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( this.tabItem );
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			"Alunos", Resources.DEFAULTS.professor16() );

		this.tabItem.setContent( this.panel );
	}

	private void createToolBar()
	{
		this.toolBar = new SimpleToolBar(
			true, true, true, true, true, true, this, "Alunos" );//issue6

		this.panel.setTopComponent( this.toolBar );
	}

	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );

	    this.gridPanel = new SimpleGrid< AlunoDTO >( getColumnList(), this, this.toolBar );

	    this.panel.add( this.gridPanel, bld );
	}

	private List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list
			= new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( AlunoDTO.PROPERTY_ALUNO_NOME, getI18nConstants().nome(), 400 ) );
		list.add( new ColumnConfig( AlunoDTO.PROPERTY_ALUNO_MATRICULA, getI18nConstants().matriculaAluno() , 150 ) );
		list.add( new CheckColumnConfig( AlunoDTO.PROPERTY_ALUNO_FORMANDO, getI18nConstants().formando() , 100 ) );
		list.add( new ColumnConfig( AlunoDTO.PROPERTY_ALUNO_PERIODO, getI18nConstants().periodo() , 100 ) );
		list.add( new ColumnConfig( AlunoDTO.PROPERTY_ALUNO_PRIORIDADE, getI18nConstants().prioridade() , 100 ) );
		CheckColumnConfig column = new CheckColumnConfig( AlunoDTO.PROPERTY_ALUNO_VIRTUAL, getI18nConstants().virtual() , 60 );
		column.setToolTip("Alunos virtuais são aqueles ainda inexistentes, porém, que a Instituição de Ensino" +
				" deseja simular a sua presenção, por exemplo, alunos entrantes. Além disso, estes alunos virtuais" +
				" podem ser criados automaticamente pelo Trieda através do Módulo de Geração de Demanda da aba de" +
				" Ofertas e Demandas. Mais especificamente, ao informar uma quantidade de alunos através da tela de" +
				" Ofertas e Demandas o Trieda irá criar, automaticamente, a quantidade informada de alunos." +
				" Nestes casos, além do aluno ser considerado virtual o mesmo terá o campo Criado por Trieda? como marcado.");
		list.add( column );
		CheckColumnConfig column2 = new CheckColumnConfig( AlunoDTO.PROPERTY_ALUNO_CRIADO_TRIEDA, getI18nConstants().criadoPeloTrieda() , 100 );
		column2.setToolTip("Alunos virtuais são aqueles ainda inexistentes, porém, que a Instituição de Ensino" +
				" deseja simular a sua presenção, por exemplo, alunos entrantes. Além disso, estes alunos virtuais" +
				" podem ser criados automaticamente pelo Trieda através do Módulo de Geração de Demanda da aba de" +
				" Ofertas e Demandas. Mais especificamente, ao informar uma quantidade de alunos através da tela de" +
				" Ofertas e Demandas o Trieda irá criar, automaticamente, a quantidade informada de alunos." +
				" Nestes casos, além do aluno ser considerado virtual o mesmo terá o campo Criado por Trieda? como marcado.");
		list.add( column2 );

		return list;
	}

	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST, 350 );
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		this.filter = new SimpleFilter();
		this.filter.setLabelWidth(150);
		

		this.nomeBuscaTextField = new TextField< String >();
		this.nomeBuscaTextField.setFieldLabel( "Nome" );
		this.filter.addField( this.nomeBuscaTextField );

		this.matriculaBuscaTextField = new TextField< String >();
		this.matriculaBuscaTextField.setFieldLabel( "Matricula" );
		this.filter.addField( this.matriculaBuscaTextField );
		
		this.formando = new ComboBoxBoolean();
		this.formando.setFieldLabel(getI18nConstants().formando());
		
		this.periodoField = new NumberField();
		this.periodoField.setFieldLabel(getI18nConstants().periodo());
		
		this.virtual = new ComboBoxBoolean();
		this.virtual.setFieldLabel(getI18nConstants().virtual());
		
		this.criadoPeloTrieda = new ComboBoxBoolean();
		this.criadoPeloTrieda.setFieldLabel(getI18nConstants().criadoPeloTrieda());
		
		this.matriculaBuscaTextField = new TextField< String >();
		this.matriculaBuscaTextField.setFieldLabel( "Matricula" );
		this.filter.addField( this.matriculaBuscaTextField );
		this.filter.addField( this.formando );
		this.filter.addField( this.periodoField );
		this.filter.addField( this.virtual );
		this.filter.addField( this.criadoPeloTrieda );

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
	public MenuItem getExportXlsExcelButton()
	{
		return (MenuItem) this.toolBar.getExportExcelButton().getMenu().getItem(0);
	}
	
	//issue6
		@Override
		public Button getRemoveAllButton()
		{
			return this.toolBar.getRemoveAllButton();
		}
			
	@Override
	public MenuItem getExportXlsxExcelButton()
	{
		return (MenuItem) this.toolBar.getExportExcelButton().getMenu().getItem(1);
	}

	@Override
	public SimpleGrid< AlunoDTO > getGrid()
	{
		return this.gridPanel;
	}

	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< AlunoDTO > > proxy )
	{
		this.gridPanel.setProxy( proxy );
	}

	@Override
	public TextField< String > getNomeBuscaTextField()
	{
		return this.nomeBuscaTextField;
	}

	@Override
	public TextField< String > getMatriculaBuscaTextField()
	{
		return this.matriculaBuscaTextField;
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
	public ComboBoxBoolean getFormando() {
		return formando;
	}

	@Override
	public ComboBoxBoolean getVirtual() {
		return virtual;
	}

	@Override
	public NumberField getPeriodoField() {
		return periodoField;
	}

	@Override
	public ComboBoxBoolean getCriadoPeloTrieda() {
		return criadoPeloTrieda;
	}
}
