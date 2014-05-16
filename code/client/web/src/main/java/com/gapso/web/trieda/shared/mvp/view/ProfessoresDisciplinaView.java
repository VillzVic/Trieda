package com.gapso.web.trieda.shared.mvp.view;

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
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.ProfessoresDisciplinaPresenter;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.DisciplinaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.OperadorComboBox;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class ProfessoresDisciplinaView extends MyComposite
	implements ProfessoresDisciplinaPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< ProfessorDisciplinaDTO > gridPanel;
	private SimpleFilter filter;
	private TextField< String > cpfBuscaTF;
	private TextField< String > nomeBuscaTF;
	private NumberField preferenciaField;
	private OperadorComboBox preferenciaOperadorCB;
	private NumberField notaDesempenhoBuscaField;
	private OperadorComboBox notaDesempenhoOperadorCB;
	private ProfessorComboBox professorBuscaCB;
	private DisciplinaAutoCompleteBox disciplinaBuscaCB;
	private ContentPanel panel;
	private GTabItem tabItem;
	private UsuarioDTO usuario;
	private CenarioDTO cenarioDTO;
	private Button associarMassaBt;

	public ProfessoresDisciplinaView( CenarioDTO cenarioDTO, UsuarioDTO usuario )
	{
		this.usuario = usuario;
		this.cenarioDTO = cenarioDTO;
		initUI();
	}

	private void initUI()
	{
		panel = new ContentPanel( new BorderLayout() );
		String title = null;

		if ( usuario.isProfessor() )
		{
			title = "Professor » Habilitação do Professor";
		}
		else
		{
			title = cenarioDTO.getNome() + " » Habilitação dos Professores";
		}

		panel.setHeadingHtml( title );
		createToolBar();
		createGrid();

		if ( usuario.isAdministrador() )
		{
			createFilter();
		}

		createTabItem();
		initComponent( tabItem );
	}

	private void createTabItem()
	{
		tabItem = new GTabItem(
			"Habilitação dos Professores",
			Resources.DEFAULTS.habilitacaoProfessor16() );

		tabItem.setContent( panel );
	}

	private void createToolBar()
	{
		if ( usuario.isProfessor() )
		{
			toolBar = new SimpleToolBar(
				false, true, false, false, false, this );
		}
		else
		{
			toolBar = new SimpleToolBar( this );
			associarMassaBt = toolBar.createButton("Associar em Massa", Resources.DEFAULTS.add216());
			toolBar.insert(associarMassaBt, 1);
		}

		panel.setTopComponent( toolBar );
	}

	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );

	    gridPanel = new SimpleGrid< ProfessorDisciplinaDTO >( getColumnList(), this, this.toolBar );
	    panel.add( gridPanel, bld );
	}

	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( ProfessorDisciplinaDTO.PROPERTY_PROFESSOR_CPF, "CPF", 100 ) );
		list.add( new ColumnConfig( ProfessorDisciplinaDTO.PROPERTY_PROFESSOR_STRING, "Nome", 300 ) );
		list.add( new ColumnConfig( ProfessorDisciplinaDTO.PROPERTY_DISCIPLINA_STRING, "Disciplina", 150 ) );
		list.add( new ColumnConfig( ProfessorDisciplinaDTO.PROPERTY_PREFERENCIA, "Preferência", 100 ) );
		list.add( new ColumnConfig( ProfessorDisciplinaDTO.PROPERTY_NOTA_DESEMPENHO, "Nota Desempenho", 100 ) );

		return list;
	}

	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST, 350 );
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		filter = new SimpleFilter();
		filter.setLabelWidth(150);
		
		this.cpfBuscaTF = new TextField< String >();
		this.cpfBuscaTF.setFieldLabel( "CPF" );
		this.nomeBuscaTF = new TextField< String >();
		this.nomeBuscaTF.setFieldLabel( "Nome" );
		professorBuscaCB = new ProfessorComboBox( cenarioDTO );
		professorBuscaCB.setAllowBlank( false );
		disciplinaBuscaCB = new DisciplinaAutoCompleteBox( cenarioDTO );
		disciplinaBuscaCB.setAllowBlank( false );
		
		this.preferenciaOperadorCB = new OperadorComboBox();
		this.preferenciaOperadorCB.setFieldLabel(getI18nConstants().preferencia());
		this.preferenciaOperadorCB.setWidth(100);
		
		this.preferenciaField = new NumberField();
		this.preferenciaField.setWidth( "75" );
		
		this.notaDesempenhoOperadorCB = new OperadorComboBox();
		this.notaDesempenhoOperadorCB.setFieldLabel(getI18nConstants().notaDesempenho());
		this.notaDesempenhoOperadorCB.setWidth(100);
		
		this.notaDesempenhoBuscaField = new NumberField();
		this.notaDesempenhoBuscaField.setWidth( "75" );

		this.filter.addField( this.cpfBuscaTF );
		this.filter.addField( this.nomeBuscaTF );
		filter.addField( professorBuscaCB );
		filter.addField( disciplinaBuscaCB );
		this.filter.addMultiField(this.preferenciaOperadorCB, this.preferenciaField);
		this.filter.addMultiField(this.notaDesempenhoOperadorCB, this.notaDesempenhoBuscaField);

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
	public SimpleGrid< ProfessorDisciplinaDTO > getGrid()
	{
		return gridPanel;
	}

	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< ProfessorDisciplinaDTO > > proxy )
	{
		gridPanel.setProxy( proxy );
	}

	@Override
	public ProfessorComboBox getProfessorBuscaComboBox()
	{
		return professorBuscaCB;
	}

	@Override
	public DisciplinaAutoCompleteBox getDisciplinaBuscaComboBox()
	{
		return disciplinaBuscaCB;
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
	public Button getAssociarMassaButton()
	{
		return associarMassaBt;
	}
	
	@Override
	public TextField< String > getCpfBuscaTextField()
	{
		return this.cpfBuscaTF;
	}

	@Override
	public TextField<String> getNomeBuscaTF() {
		return nomeBuscaTF;
	}

	@Override
	public NumberField getPreferenciaField() {
		return preferenciaField;
	}

	@Override
	public OperadorComboBox getPreferenciaOperadorCB() {
		return preferenciaOperadorCB;
	}

	@Override
	public NumberField getNotaDesempenhoBuscaField() {
		return notaDesempenhoBuscaField;
	}

	@Override
	public OperadorComboBox getNotaDesempenhoOperadorCB() {
		return notaDesempenhoOperadorCB;
	}
	
	
}
