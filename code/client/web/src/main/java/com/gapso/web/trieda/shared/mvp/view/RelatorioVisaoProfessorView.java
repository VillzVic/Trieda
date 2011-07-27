package com.gapso.web.trieda.shared.mvp.view;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.RelatorioVisaoProfessorPresenter;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.GradeHorariaProfessorGrid;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.ProfessorVirtualComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class RelatorioVisaoProfessorView extends MyComposite
	implements RelatorioVisaoProfessorPresenter.Display
{
	private SimpleToolBar toolBar;
	private GradeHorariaProfessorGrid grid;
	private Button submitBt;
	private CampusComboBox campusCB;
	private TurnoComboBox turnoCB;
	private ProfessorComboBox professorCB;
	private ProfessorVirtualComboBox professorVirtualCB;
	private ContentPanel panel;
	private GTabItem tabItem;
	private UsuarioDTO usuario;

	public RelatorioVisaoProfessorView( UsuarioDTO usuario )
	{
		this.usuario = usuario;
		initUI();
	}

	private void createToolBar()
	{
		// Exibe apenas o botão 'exportExcel'
		toolBar = new SimpleToolBar( false, false, false, false, true, this );
		panel.setTopComponent( toolBar );
	}

	private void initUI()
	{
		panel = new ContentPanel( new BorderLayout() );
		panel.setHeading( "Master Data » Grade Horária Visão Professor" );
		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( tabItem );
	}

	private void createTabItem()
	{
		tabItem = new GTabItem( "Grade Horária Visão Professor",
			Resources.DEFAULTS.saidaProfessor16() );
		tabItem.setContent( panel );
	}

	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );
	    grid = new GradeHorariaProfessorGrid();
	    panel.add( grid, bld );
	}

	private void createFilter()
	{
		FormData formData = new FormData("100%");
		FormPanel panel = new FormPanel();
		panel.setHeaderVisible(true);
		panel.setHeading("Filtro");

		LayoutContainer main = new LayoutContainer(new ColumnLayout());

		LayoutContainer left = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));
		LayoutContainer right = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));
		
		if(usuario.isAdministrador()) {
			campusCB = new CampusComboBox();
			left.add(campusCB, formData);
		}
		
		turnoCB = new TurnoComboBox(campusCB, usuario.isProfessor());
		left.add(turnoCB, formData);
		
		if(usuario.isAdministrador()) {
			professorCB = new ProfessorComboBox(campusCB);
			right.add(professorCB, formData);
			professorVirtualCB = new ProfessorVirtualComboBox(campusCB);
			right.add(professorVirtualCB, formData);
		}
		
		submitBt = new Button("Filtrar", AbstractImagePrototype.create(Resources.DEFAULTS.filter16()));
		panel.addButton(submitBt);
		
		main.add(left, new ColumnData(.5));
		main.add(right, new ColumnData(.5));
		
		panel.add(main, new FormData("100%"));
		
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.NORTH);
		bld.setMargins(new Margins(5, 5, 0, 5));
		bld.setCollapsible(true);
		if(usuario.isAdministrador()) {
			bld.setSize(130);
		} else {
			bld.setSize(103);
		}
		
		this.panel.add(panel, bld);
	}
	
	@Override
	public GradeHorariaProfessorGrid getGrid() {
		return grid;
	}

	@Override
	public Button getSubmitBuscaButton() {
		return submitBt;
	}

	@Override
	public CampusComboBox getCampusComboBox() {
		return campusCB;
	}
	
	@Override
	public TurnoComboBox getTurnoComboBox() {
		return turnoCB;
	}

	@Override
	public ProfessorComboBox getProfessorComboBox() {
		return professorCB;
	}

	@Override
	public ProfessorVirtualComboBox getProfessorVirtualComboBox() {
		return professorVirtualCB;
	}

	@Override
	public Button getExportExcelButton()
	{
		return toolBar.getExportExcelButton();
	}
}
