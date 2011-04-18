package com.gapso.web.trieda.shared.mvp.view;

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
import com.gapso.web.trieda.shared.dtos.ProfessorDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.ProfessoresDisciplinaPresenter;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class ProfessoresDisciplinaView extends MyComposite implements ProfessoresDisciplinaPresenter.Display {

	private SimpleToolBar toolBar;
	private SimpleGrid<ProfessorDisciplinaDTO> gridPanel;
	private SimpleFilter filter;
	private ProfessorComboBox professorBuscaCB;
	private DisciplinaComboBox disciplinaBuscaCB;
	private ContentPanel panel;
	private GTabItem tabItem;
	private UsuarioDTO usuario;
	
	public ProfessoresDisciplinaView(UsuarioDTO usuario) {
		this.usuario = usuario;
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		String title = null;
		if(usuario.isProfessor()) {
			title = "Professor » Habilitação do Professor";
		} else {
			title = "Master Data » Habilitação dos Professores";
		}
		panel.setHeading(title);
		createToolBar();
		createGrid();
		if(usuario.isAdministrador()) {
			createFilter();
		}
		createTabItem();
		initComponent(tabItem);
	}
	
	private void createTabItem() {
		tabItem = new GTabItem("Habilitação dos Professores", Resources.DEFAULTS.habilitacaoProfessor16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		if(usuario.isProfessor()) {
			toolBar = new SimpleToolBar(false, true, false, false, false, this);
		} else {
			toolBar = new SimpleToolBar(this);
		}
		panel.setTopComponent(toolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    gridPanel = new SimpleGrid<ProfessorDisciplinaDTO>(getColumnList(), this);
	    panel.add(gridPanel, bld);
	}

	public List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		list.add(new ColumnConfig(ProfessorDisciplinaDTO.PROPERTY_PROFESSOR_CPF, "CPF", 100));
		list.add(new ColumnConfig(ProfessorDisciplinaDTO.PROPERTY_PROFESSOR_STRING, "Nome", 100));
		list.add(new ColumnConfig(ProfessorDisciplinaDTO.PROPERTY_DISCIPLINA_STRING, "Disciplina", 100));
		list.add(new ColumnConfig(ProfessorDisciplinaDTO.PROPERTY_PREFERENCIA, "Preferência", 100));
		list.add(new ColumnConfig(ProfessorDisciplinaDTO.PROPERTY_NOTA_DESEMPENHO, "Nota Desempenho", 100));
		return list;
	}

	private void createFilter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.EAST);
		bld.setMargins(new Margins(5, 5, 5, 0));
		bld.setCollapsible(true);
		
		filter = new SimpleFilter();
		professorBuscaCB = new ProfessorComboBox();
		professorBuscaCB.setAllowBlank(false);
		disciplinaBuscaCB = new DisciplinaComboBox();
		disciplinaBuscaCB.setAllowBlank(false);
		
		filter.addField(professorBuscaCB);
		filter.addField(disciplinaBuscaCB);
		
		panel.add(filter, bld);
	}
	
	@Override
	public Button getNewButton() {
		return toolBar.getNewButton();
	}

	@Override
	public Button getEditButton() {
		return toolBar.getEditButton();
	}

	@Override
	public Button getRemoveButton() {
		return toolBar.getRemoveButton();
	}

	@Override
	public Button getImportExcelButton() {
		return toolBar.getImportExcelButton();
	}

	@Override
	public Button getExportExcelButton() {
		return toolBar.getExportExcelButton();
	}
	
	@Override
	public SimpleGrid<ProfessorDisciplinaDTO> getGrid() {
		return gridPanel;
	}
	
	@Override
	public void setProxy(RpcProxy<PagingLoadResult<ProfessorDisciplinaDTO>> proxy) {
		gridPanel.setProxy(proxy);
	}

	@Override
	public ProfessorComboBox getProfessorBuscaComboBox() {
		return professorBuscaCB;
	}
	
	@Override
	public DisciplinaComboBox getDisciplinaBuscaComboBox() {
		return disciplinaBuscaCB;
	}

	@Override
	public Button getSubmitBuscaButton() {
		return filter.getSubmitButton();
	}

	@Override
	public Button getResetBuscaButton() {
		return filter.getResetButton();
	}

}