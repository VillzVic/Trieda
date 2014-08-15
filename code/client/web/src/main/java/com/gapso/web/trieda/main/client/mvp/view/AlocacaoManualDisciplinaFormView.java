package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextArea;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.CheckColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.gapso.web.trieda.main.client.mvp.presenter.AlocacaoManualDisciplinaFormPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ResumoMatriculaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class AlocacaoManualDisciplinaFormView 
	extends MyComposite
	implements AlocacaoManualDisciplinaFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextArea motivos;
	private CenarioDTO cenarioDTO;
	private Grid< ResumoMatriculaDTO > gridPanel;
	private SimpleFilter filter;
	private TextField< String > codigoBuscaTextField;
	private CampusComboBox campusBuscaComboBox;
	private CursoComboBox cursoBuscaComboBox;
	private ListStore< ResumoMatriculaDTO > store = new ListStore< ResumoMatriculaDTO >();
	private ContentPanel panel;
	
	public AlocacaoManualDisciplinaFormView( CenarioDTO cenarioDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.initUI();
	}
	
	private void initUI()
	{
		String title = cenarioDTO.getNome() + " »  Selecione a disciplina para alocação manual";
		this.simpleModal = new SimpleModal( title,
			Resources.DEFAULTS.alunoCurriculo16() );
	
		this.simpleModal.setHeight( 600 );
		this.simpleModal.setWidth( 1200 );
		createForm();
		this.simpleModal.setContent( this.panel );
	}
	
	private void createForm()
	{
		this.formPanel = new FormPanel();
		this.formPanel.setHeaderVisible( false );
		
		BorderLayoutData bldTop = new BorderLayoutData( LayoutRegion.NORTH );
		bldTop.setMargins( new Margins( 5, 5, 5, 5 ) );
		
		panel = new ContentPanel( new BorderLayout() );
		panel.getHeader().hide();
		LayoutContainer top = new LayoutContainer();
		
		top.addText("Selecione uma linha da tabela abaixo e acione o botão Selecionar:");
		top.setStyleAttribute("margin-top", "5px");
		top.setStyleAttribute("margin-left", "10px");
		top.setStyleAttribute("font-size", "13px");
		panel.add(top);
		
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST );
		bld.setMargins( new Margins( 25, 5, 5, 0 ) );
		bld.setCollapsible( true );
		
		createGrid();
		
		this.filter = new SimpleFilter();
		this.codigoBuscaTextField = new TextField<String>();
		this.codigoBuscaTextField.setFieldLabel("Código da Disciplina");
		this.cursoBuscaComboBox = new CursoComboBox(cenarioDTO);
		this.cursoBuscaComboBox.setFieldLabel("Curso");
		this.campusBuscaComboBox = new CampusComboBox(cenarioDTO);
		this.campusBuscaComboBox.setFieldLabel("Campus");
		
		this.filter.addField( this.codigoBuscaTextField );
		this.filter.addField( this.cursoBuscaComboBox );
		this.filter.addField( this.campusBuscaComboBox );
	
		this.panel.add( this.filter, bld );
		this.formPanel.add(panel);
		
		this.simpleModal.getSalvarBt().setText("Selecionar");
		
		this.simpleModal.setFocusWidget( this.motivos );
	}
	
	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
		bld.setMargins( new Margins( 25, 5, 5, 0 ) );
		
	    this.gridPanel = new Grid<ResumoMatriculaDTO>( this.getStore(),  getColumnList() );
	    gridPanel.setBorders(true);
	    gridPanel.setHeight(100);
	    gridPanel.setWidth(100);
	    this.panel.add( this.gridPanel, bld );
	}
	
	public ColumnModel getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();
		
		list.add( new ColumnConfig( ResumoMatriculaDTO.PROPERTY_CAMPUS_STRING, getI18nConstants().campus(), 120 ) );
		list.add( new ColumnConfig( ResumoMatriculaDTO.PROPERTY_CODIGO_DISCIPLINA, getI18nConstants().codigoDisciplina(), 100 ) );
		list.add( new ColumnConfig( ResumoMatriculaDTO.PROPERTY_NOME_DISCIPLINA, getI18nConstants().nomeDisciplina(), 100 ) );
		list.add( new ColumnConfig( ResumoMatriculaDTO.PROPERTY_DISCIPLINA_DEMANDA_P1, "Demanda P1",80 ) );
		list.add( new ColumnConfig( ResumoMatriculaDTO.PROPERTY_DISCIPLINA_ATENDIDOS_P1, "Atendidos P1", 80 ) );
		list.add( new ColumnConfig( ResumoMatriculaDTO.PROPERTY_DISCIPLINA_ATENDIDOS_P2, "Atendidos P2", 80 ) );
		list.add( new ColumnConfig( ResumoMatriculaDTO.PROPERTY_DISCIPLINA_ATENDIDOS_SOMA, "Atendidos P1 + P2", 100 ) );
		list.add( new ColumnConfig( ResumoMatriculaDTO.PROPERTY_DISCIPLINA_CRED_PRATICO, getI18nConstants().creditosPraticos(), 100 ) );
		list.add( new ColumnConfig( ResumoMatriculaDTO.PROPERTY_DISCIPLINA_CRED_TEORICO, getI18nConstants().creditosTeoricos(), 100 ) );
		list.add( new CheckColumnConfig( ResumoMatriculaDTO.PROPERTY_DISCIPLINA_EXIGE_LAB, getI18nConstants().exigeLaboratorio(), 100 ) );
		
		ColumnModel cm = new ColumnModel(list);

		return cm;
	}
	
	public boolean isValid()
	{
		return this.formPanel.isValid();
	}
	
	@Override
	public ListStore< ResumoMatriculaDTO > getStore()
	{
		return this.store;
	}
	
	@Override
	public Grid< ResumoMatriculaDTO > getGrid()
	{
		return this.gridPanel;
	}
	
	@Override
	public SimpleModal getSimpleModal()
	{
		return this.simpleModal;
	}
	
	@Override
	public TextField<String> getCodigoBuscaTextField() {
		return this.codigoBuscaTextField;
	}

	@Override
	public CampusComboBox getCampusBuscaComboBox() {
		return this.campusBuscaComboBox;
	}

	@Override
	public CursoComboBox getCursoBuscaComboBox() {
		return this.cursoBuscaComboBox;
	}
	
	@Override
	public Button getSalvarButton()
	{
		return this.simpleModal.getSalvarBt();
	}
	
	@Override
	public SimpleFilter getFiltro()
	{
		return this.filter;
	}
}
