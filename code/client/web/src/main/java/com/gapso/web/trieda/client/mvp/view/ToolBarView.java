package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.Style.ButtonArrowAlign;
import com.extjs.gxt.ui.client.Style.ButtonScale;
import com.extjs.gxt.ui.client.Style.IconAlign;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.TabItem;
import com.extjs.gxt.ui.client.widget.TabPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.client.mvp.presenter.ToolBarPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class ToolBarView extends MyComposite implements ToolBarPresenter.Display {

	private LayoutContainer container;
	private ToolBar campiToolBar;
	private ToolBar unidadesToolBar;
	private ToolBar salasToolBar;
	private ToolBar cursosToolBar;
	private ToolBar disciplinasToolBar;
	private ToolBar professoresToolBar;
	
	private ToolBar cenarioToolBar;
	private ToolBar planejamentoToolBar;
		
	// Campi
	private Button campiListCampiBt;
	private Button campusDeslocamentoListCampiBt;
	private Button semanasLetivaListCampiBt;
	private Button horariosAulaListCampiBt;
	private Button turnosListCampiBt;
	private Button ofertasListCampiBt;
	
	// Unidades
	private Button unidadesListUnidadesBt;
	private Button unidadeDeslocamentoListUnidadesBt;
	private Button semanasLetivaListUnidadesBt;
	private Button horariosAulaListUnidadesBt;
	
	// Salas
	private Button salasListSalasBt;
	private Button gruposSalasListSalasBt;
	private Button associarDisciplinasSalasListSalasBt;
	
	// Cursos
	private Button tiposCursosListCursosBt;
	private Button areasTitulacaoListCursosBtI;
	private Button cursosListCursosBt;
	private Button curriculosListCursosBt;
	private Button ofertasListCursosBt;
	
	// Disciplinas
	private Button disciplinasListDisciplinasBt;
	private Button demandasDisciplinasBt;
	private Button curriculosListDisciplinasBt;
	private Button associarDisciplinasSalasListDisciplinasBt;
	private Button divisaoCreditosListDisciplinasBt;
	private Button equivalenciasListDisciplinasBt;
	private Button fixacoesListDisciplinasBt;
	
	// Professores
	private Button professoresListprofessoresBt;
	private Button professoresDisciplinaListprofessoresBt;
	
	// Cenário
	private Button parametrosBt;
	private Button relatorioVisaoSalaBt;
	private Button relatorioVisaoCursoBt;
	
	public ToolBarView() {
		initUI();
	}
	
	private void initUI() {
		container = new LayoutContainer(new ColumnLayout());
		
		ContentPanel cenarioPanel = new ContentPanel();
		cenarioPanel.setHeading("Cenário");
		
		ContentPanel planejamentoPanel = new ContentPanel();
		planejamentoPanel.setHeading("Planejamento");
		
		ContentPanel masterDataPanel = new ContentPanel();
		masterDataPanel.setHeaderVisible(false);
		
		cenarioPanel.setBodyBorder(false);
		planejamentoPanel.setBodyBorder(false);
		masterDataPanel.setBodyBorder(false);
		
		
		container.add(cenarioPanel, new ColumnData(.2));
		container.add(planejamentoPanel, new ColumnData(.2));
		container.add(masterDataPanel, new ColumnData(.6));
		
		TabItem campiTabItem = new TabItem("Campi");
		TabItem unidadesTabItem = new TabItem("Unidades");
		TabItem salasTabItem = new TabItem("Salas");
		TabItem cursosTabItem = new TabItem("Cursos");
		TabItem disciplinasTabItem = new TabItem("Disciplinas");
		TabItem professoresTabItem = new TabItem("Professores");
		
		cenarioToolBar = new ToolBar();
		
		planejamentoToolBar = new ToolBar();
		
		campiToolBar = new ToolBar();
		campiToolBar.setLayoutData(new FitLayout());
		unidadesToolBar = new ToolBar();
		salasToolBar = new ToolBar();
		cursosToolBar = new ToolBar();
		disciplinasToolBar = new ToolBar();
		professoresToolBar = new ToolBar();

		campiTabItem.add(campiToolBar);
		
		unidadesTabItem.add(unidadesToolBar);
		salasTabItem.add(salasToolBar);
		cursosTabItem.add(cursosToolBar);
		disciplinasTabItem.add(disciplinasToolBar);
		professoresTabItem.add(professoresToolBar);
		
		createGroups();
		
		TabPanel masterDataTab = new TabPanel();
//		masterDataTab.setBodyBorder(false);
		masterDataTab.addStyleName("tabPanelMasterData");
		masterDataTab.setHeight(78);
		cenarioToolBar.setHeight(52);
		planejamentoToolBar.setHeight(52);

		TabItem masterDataItem = new TabItem("Master Data");
		masterDataItem.disable();
		masterDataTab.add(masterDataItem);
		masterDataTab.add(campiTabItem);
		masterDataTab.setSelection(campiTabItem);
		masterDataTab.add(unidadesTabItem);
		masterDataTab.add(salasTabItem);
		masterDataTab.add(cursosTabItem);
		masterDataTab.add(disciplinasTabItem);
		masterDataTab.add(professoresTabItem);
		
		cenarioPanel.add(cenarioToolBar);
		planejamentoPanel.add(planejamentoToolBar);
		masterDataPanel.setTopComponent(masterDataTab);
		
		container.setHeight(78);
		
		initComponent(container);
	}
	
	
	private void createGroups() {
		createCampi();
		createUnidades();
		createSalas();
		createCursos();
		createDisciplinas();
		createProfessores();
		
		createCenario();
		createPlanejamento();
	}

	/********************************
	 * CRIAÇÃO DOS BOTÕES
	 */
	
	private void createCampi() {
		// TODO Implementar o cadastro de campus pelo menu
		campiToolBar.add(createButton("Novo", "Adicionar um campus", Resources.DEFAULTS.campi24()));
		
		campiListCampiBt = createButton("Listar", "Listar Campi", Resources.DEFAULTS.campi24());
		campiToolBar.add(campiListCampiBt);
		
		campiToolBar.add(new SeparatorToolItem());
		
		campusDeslocamentoListCampiBt = createButton("Deslocamento", "Deslocamento entre Campi", Resources.DEFAULTS.campi24());
		campiToolBar.add(campusDeslocamentoListCampiBt);
		
		semanasLetivaListCampiBt = createButton("Semanas Letivas", "Semanas Letivas", Resources.DEFAULTS.campi24());
		campiToolBar.add(semanasLetivaListCampiBt);
		
		horariosAulaListCampiBt = createButton("Horários Aula", "Horários de Aula", Resources.DEFAULTS.campi24());
		campiToolBar.add(horariosAulaListCampiBt);
		
		turnosListCampiBt = createButton("Turnos", "Turnos", Resources.DEFAULTS.campi24());
		campiToolBar.add(turnosListCampiBt);
		
		ofertasListCampiBt = createButton("Ofertas", "Oferta de Cursos em Campi", Resources.DEFAULTS.campi24());
		campiToolBar.add(ofertasListCampiBt);
	}
	
	private void createUnidades() {
		// TODO Implementar o cadastro de unidade pelo menu
		unidadesToolBar.add(createButton("Nova", "Adicionar uma Unidade", Resources.DEFAULTS.campi24()));
		
		unidadesListUnidadesBt = createButton("Listar", "Listar Unidades", Resources.DEFAULTS.campi24());
		unidadesToolBar.add(unidadesListUnidadesBt);
		
		unidadesToolBar.add(new SeparatorToolItem());
		
		unidadeDeslocamentoListUnidadesBt = createButton("Desloc.", "Deslocamento de Unidades", Resources.DEFAULTS.campi24());
		unidadesToolBar.add(unidadeDeslocamentoListUnidadesBt);
		
		semanasLetivaListUnidadesBt = createButton("Sem. Let.", "Semanas Letivas", Resources.DEFAULTS.campi24());
		unidadesToolBar.add(semanasLetivaListUnidadesBt);
		
		horariosAulaListUnidadesBt = createButton("Horários", "Horários de Aula", Resources.DEFAULTS.campi24());
		unidadesToolBar.add(horariosAulaListUnidadesBt);
	}
	
	private void createSalas() {
		// TODO Implementar o cadastro de sala pelo menu
		salasToolBar.add(createButton("Nova", "Adicionar uma Sala", Resources.DEFAULTS.campi24()));
		
		salasListSalasBt = createButton("Listar", "Listar Salas", Resources.DEFAULTS.campi24());
		salasToolBar.add(salasListSalasBt);
		
		salasToolBar.add(new SeparatorToolItem());
		
		gruposSalasListSalasBt = createButton("Grupos", "Grupos de Salas", Resources.DEFAULTS.campi24());
		salasToolBar.add(gruposSalasListSalasBt);
		
		associarDisciplinasSalasListSalasBt = createButton("Assoc. Salas", "Associação de Disciplinas à Salas", Resources.DEFAULTS.campi24());
		salasToolBar.add(associarDisciplinasSalasListSalasBt);
	}
	
	private void createCursos() {
		// TODO Implementar o cadastro de curso pelo menu
		cursosToolBar.add(createButton("Novo", "Adicionar um Curso", Resources.DEFAULTS.campi24()));
		
		cursosListCursosBt = createButton("Listar", "Listar Cursos", Resources.DEFAULTS.campi24());
		cursosToolBar.add(cursosListCursosBt);
		
		cursosToolBar.add(new SeparatorToolItem());
		
		ofertasListCursosBt = createButton("Ofertas", "Oferta de Cursos em Campi", Resources.DEFAULTS.campi24());
		cursosToolBar.add(ofertasListCursosBt);
		
		areasTitulacaoListCursosBtI = createButton("Áreas", "Áreas de Titulação", Resources.DEFAULTS.campi24());
		cursosToolBar.add(areasTitulacaoListCursosBtI);
		
		cursosToolBar.add(createButton("Vinc. Áreas", "Vincular Áreas de Titulação", Resources.DEFAULTS.campi24()));
		
		curriculosListCursosBt = createButton("Matr. Curr.", "Matrizes Curriculares", Resources.DEFAULTS.campi24());
		cursosToolBar.add(curriculosListCursosBt);
		
		tiposCursosListCursosBt = createButton("Tipos", "Tipos de Curso", Resources.DEFAULTS.campi24());
		cursosToolBar.add(tiposCursosListCursosBt);
	}
	
	private void createDisciplinas() {
		// TODO Implementar o cadastro de disciplina pelo menu
		disciplinasToolBar.add(createButton("Novo", "Adicionar um Curso", Resources.DEFAULTS.campi24()));

		disciplinasListDisciplinasBt = createButton("Listar", "Listar Disciplinas", Resources.DEFAULTS.campi24());
		disciplinasToolBar.add(disciplinasListDisciplinasBt);
		
		disciplinasToolBar.add(new SeparatorToolItem());

		associarDisciplinasSalasListDisciplinasBt = createButton("Assoc. Salas", "Associação de Disciplinas à Salas", Resources.DEFAULTS.campi24());
		disciplinasToolBar.add(associarDisciplinasSalasListDisciplinasBt);
		
		curriculosListDisciplinasBt = createButton("Matr. Curr.", "Matrizes Curriculares", Resources.DEFAULTS.campi24());
		disciplinasToolBar.add(curriculosListDisciplinasBt);
		
		demandasDisciplinasBt = createButton("Demanda", "Previsão de demanda", Resources.DEFAULTS.campi24());
		disciplinasToolBar.add(demandasDisciplinasBt);
		
		divisaoCreditosListDisciplinasBt = createButton("Divisão de Créditos", "Divisão de Créditos", Resources.DEFAULTS.campi24());
		disciplinasToolBar.add(divisaoCreditosListDisciplinasBt);
		
		equivalenciasListDisciplinasBt = createButton("Equivalências", "Equivalências", Resources.DEFAULTS.campi24());
		disciplinasToolBar.add(equivalenciasListDisciplinasBt);
		
		fixacoesListDisciplinasBt = createButton("Fixações", "Fixações", Resources.DEFAULTS.campi24());
		disciplinasToolBar.add(fixacoesListDisciplinasBt);
	}
	
	
	private void createProfessores() {
		professoresToolBar.add(createButton("Novo", "Adicionar novo Professor", Resources.DEFAULTS.campi24()));
		
		professoresListprofessoresBt = createButton("Listar", "Listar Professores", Resources.DEFAULTS.campi24());
		professoresToolBar.add(professoresListprofessoresBt);
		
		professoresDisciplinaListprofessoresBt = createButton("Habilitação dos Professores", "Habilitação dos Professores", Resources.DEFAULTS.campi24());
		professoresToolBar.add(professoresDisciplinaListprofessoresBt);
		
		professoresToolBar.add(new SeparatorToolItem());
	}
	
	private void createPlanejamento() {
		parametrosBt = createButton("Gerar Grade", "Gerar Grade", Resources.DEFAULTS.otimizar24());
		planejamentoToolBar.add(parametrosBt);
		
		relatorioVisaoSalaBt = createButton("Saida: Sala", "Grade Horária Visão Sala", Resources.DEFAULTS.otimizar24());
		planejamentoToolBar.add(relatorioVisaoSalaBt);
		
		relatorioVisaoCursoBt = createButton("Saida: Curso", "Grade Horária Visão Curso", Resources.DEFAULTS.otimizar24());
		planejamentoToolBar.add(relatorioVisaoCursoBt);
	}
	
	private void createCenario() {
		cenarioToolBar.add(createButton("Novo", "Adicionar novo Cenário", Resources.DEFAULTS.cenario24()));
		cenarioToolBar.add(createButton("Clonar", "Clonar um cenário", Resources.DEFAULTS.cenario24()));
		cenarioToolBar.add(createButton("Abrir", "Abrir um cenário", Resources.DEFAULTS.cenario24()));
	}
	
	private Button createButton(String text, String toolTip, ImageResource icon) {
		Button bt = new Button(text, AbstractImagePrototype.create(icon));
		bt.setToolTip(toolTip);
		bt.setScale(ButtonScale.MEDIUM);
		bt.setIconAlign(IconAlign.TOP);
		bt.setArrowAlign(ButtonArrowAlign.BOTTOM);
		return bt;
	}
	
	/********************************
	 * GET BUTTONS
	 */
	
	@Override
	public Button getParametrosButton() {
		return parametrosBt;
	}
	@Override
	public Button getSemanasLetivaListCampiButton() {
		return semanasLetivaListCampiBt;
	}
	@Override
	public Button getHorariosAulaListCampiButton() {
		return horariosAulaListCampiBt;
	}
	@Override
	public Button getSalasListSalasButton() {
		return salasListSalasBt;
	}
	@Override
	public Button getGruposSalasListSalasButton() {
		return gruposSalasListSalasBt;
	}
	@Override
	public Button getTiposCursosListCursosButton() {
		return tiposCursosListCursosBt;
	}
	@Override
	public Button getAreasTitulacaoListCursosButton() {
		return areasTitulacaoListCursosBtI;
	}
	@Override
	public Button getUnidadeDeslocamentoListUnidadesButton() {
		return unidadeDeslocamentoListUnidadesBt;
	}
	@Override
	public Button getCampusDeslocamentoListCampiButton() {
		return campusDeslocamentoListCampiBt;
	}
	@Override
	public Button getCursosListCursosButton() {
		return cursosListCursosBt;
	}
	@Override
	public Button getDisciplinasListDisciplinasButton() {
		return disciplinasListDisciplinasBt;
	}
	@Override
	public Button getCurriculosListCursosButton() {
		return curriculosListCursosBt;
	}
	@Override
	public Button getOfertasListCampiButton() {
		return ofertasListCampiBt;
	}
	@Override
	public Button getAssociarDisciplinasSalasListSalasButton() {
		return associarDisciplinasSalasListSalasBt;
	}
	@Override
	public Button getDemandasDisciplinasButton() {
		return demandasDisciplinasBt;
	}
	@Override
	public Button getDivisaoCreditosListDisciplinasButton() {
		return divisaoCreditosListDisciplinasBt;
	}
	@Override
	public Button getEquivalenciasListDisciplinasButton() {
		return equivalenciasListDisciplinasBt;
	}
	@Override
	public Button getFixacoesListDisciplinasButton() {
		return fixacoesListDisciplinasBt;
	}
	@Override
	public Button getProfessoresListProfessoresButton() {
		return professoresListprofessoresBt;
	}
	@Override
	public Button getProfessoresDisciplinaListProfessoresButton() {
		return professoresDisciplinaListprofessoresBt;
	}
	@Override
	public Button getRelatorioVisaoSalaButton() {
		return relatorioVisaoSalaBt;
	}
	@Override
	public Button getRelatorioVisaoCursoButton() {
		return relatorioVisaoCursoBt;
	}

	@Override
	public Button getTurnosListCampiButton() {
		return turnosListCampiBt;
	}

	@Override
	public Button getCampiListCampiButton() {
		return campiListCampiBt;
	}

	@Override
	public Button getUnidadesListUnidadesButton() {
		return unidadesListUnidadesBt;
	}

	@Override
	public Button getSemanasLetivaListUnidadesButton() {
		return semanasLetivaListUnidadesBt;
	}

	@Override
	public Button getHorariosAulaListUnidadesButton() {
		return horariosAulaListUnidadesBt;
	}

	@Override
	public Button getOfertasListCursosButton() {
		return ofertasListCursosBt;
	}

	@Override
	public Button getAssociarDisciplinasSalasListDisciplinasButton() {
		return associarDisciplinasSalasListDisciplinasBt;
	}

	@Override
	public Button getCurriculosListDisciplinasButton() {
		return curriculosListDisciplinasBt;
	}
	
}
