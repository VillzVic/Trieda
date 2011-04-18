package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.ButtonArrowAlign;
import com.extjs.gxt.ui.client.Style.ButtonScale;
import com.extjs.gxt.ui.client.Style.IconAlign;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.TabItem;
import com.extjs.gxt.ui.client.widget.TabPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayoutData;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.main.client.mvp.presenter.ToolBarPresenter;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
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
	private ToolBar relatoriosToolBar;
	
	private ToolBar cenarioToolBar;
	private ToolBar planejamentoToolBar;
		
	// Campi
	private Button campiNovoCampiBt;
	private Button campiListCampiBt;
	private Button campusDeslocamentoListCampiBt;
	private Button semanasLetivaListCampiBt;
	private Button horariosAulaListCampiBt;
	private Button turnosListCampiBt;
	private Button ofertasListCampiBt;
	private Button usuariosListBt;
	
	// Unidades
	private Button unidadesNovoUnidadesBt;
	private Button unidadesListUnidadesBt;
	private Button unidadeDeslocamentoListUnidadesBt;
	
	// Salas
	private Button salasNovoSalasBt;
	private Button salasListSalasBt;
	private Button gruposSalasListSalasBt;
	private Button associarDisciplinasSalasListSalasBt;
	
	// Cursos
	private Button cursosNovoCursosBt;
	private Button cursosListCursosBt;
	private Button tiposCursosListCursosBt;
	private Button areasTitulacaoListCursosBt;
	private Button vincularAreasTitulacaoListCursosBt;
	private Button curriculosListCursosBt;
	private Button ofertasListCursosBt;
	
	// Disciplinas
	private Button disciplinasNovoDisciplinasBt;
	private Button disciplinasListDisciplinasBt;
	private Button demandasDisciplinasBt;
	private Button curriculosListDisciplinasBt;
	private Button associarDisciplinasSalasListDisciplinasBt;
	private Button divisaoCreditosListDisciplinasBt;
	private Button equivalenciasListDisciplinasBt;
	private Button compatibilidadesListDisciplinasBt;
	
	// Professores
	private Button professoresNovoProfessoresBt;
	private Button professoresListprofessoresBt;
	private Button professoresDisciplinaListprofessoresBt;
	private Button professoresCampusListprofessoresBt;
	
	// Relatórios
	private Button relatorioVisaoSalaBt;
	private Button relatorioVisaoCursoBt;
	private Button relatorioVisaoProfessorBt;
	private Button resumoCenarioBt;
	private Button resumoCampiBt;
	
	// Cenário
	private Button fixacoesListBt;
	private Button parametrosBt;
	
	public ToolBarView() {
		initUI();
	}
	
	private void initUI() {
		container = new LayoutContainer(new HBoxLayout());
		
		ContentPanel cenarioPanel = new ContentPanel();
		cenarioPanel.setHeading("Cenários");
		cenarioPanel.setWidth(85);
		
		ContentPanel planejamentoPanel = new ContentPanel();
		planejamentoPanel.setHeading("Planejamento");
		planejamentoPanel.setWidth(150);
		
		ContentPanel masterDataPanel = new ContentPanel();
		masterDataPanel.setHeaderVisible(false);
		
		cenarioPanel.setBodyBorder(false);
		planejamentoPanel.setBodyBorder(false);
		masterDataPanel.setBodyBorder(false);
		
		HBoxLayoutData flex = new HBoxLayoutData(new Margins(0));
		flex.setFlex(1);  
		container.add(cenarioPanel, new HBoxLayoutData(new Margins(0)));
		container.add(planejamentoPanel, new HBoxLayoutData(new Margins(0)));
		container.add(masterDataPanel, flex);
		
		TabItem campiTabItem = new TabItem("Campi");
		TabItem unidadesTabItem = new TabItem("Unidades");
		TabItem salasTabItem = new TabItem("Salas");
		TabItem cursosTabItem = new TabItem("Cursos");
		TabItem disciplinasTabItem = new TabItem("Disciplinas");
		TabItem professoresTabItem = new TabItem("Professores");
		TabItem relatoriosTabItem = new TabItem("Relatórios");
		
		cenarioToolBar = new ToolBar();
		
		planejamentoToolBar = new ToolBar();
		
		campiToolBar = new ToolBar();
		campiToolBar.setLayoutData(new FitLayout());
		unidadesToolBar = new ToolBar();
		salasToolBar = new ToolBar();
		cursosToolBar = new ToolBar();
		disciplinasToolBar = new ToolBar();
		professoresToolBar = new ToolBar();
		relatoriosToolBar = new ToolBar();

		campiTabItem.add(campiToolBar);
		unidadesTabItem.add(unidadesToolBar);
		salasTabItem.add(salasToolBar);
		cursosTabItem.add(cursosToolBar);
		disciplinasTabItem.add(disciplinasToolBar);
		professoresTabItem.add(professoresToolBar);
		relatoriosTabItem.add(relatoriosToolBar);
		
		createGroups();
		
		int height = 92;
		
		TabPanel masterDataTab = new TabPanel();
		masterDataTab.addStyleName("tabPanelMasterData");
		masterDataTab.setHeight(height);
		cenarioToolBar.setHeight(height - 26);
		planejamentoToolBar.setHeight(height - 26);

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
		masterDataTab.add(relatoriosTabItem);
		
		cenarioPanel.add(cenarioToolBar);
		planejamentoPanel.add(planejamentoToolBar);
		masterDataPanel.setTopComponent(masterDataTab);
		
		container.setHeight(height);
		
		initComponent(container);
	}
	
	
	private void createGroups() {
		createCampi();
		createUnidades();
		createSalas();
		createCursos();
		createDisciplinas();
		createProfessores();
		createRelatorios();
		
		createCenario();
		createPlanejamento();
	}

	/********************************
	 * CRIAÇÃO DOS BOTÕES
	 */
	
	private void createCampi() {
		// TODO Implementar o cadastro de campus pelo menu
		campiNovoCampiBt = createButton("Novo", "Adicionar um campus", Resources.DEFAULTS.campusNovo24());
		campiToolBar.add(campiNovoCampiBt);
		
		campiListCampiBt = createButton("Listar", "Listar Campi", Resources.DEFAULTS.campusListar24());
		campiToolBar.add(campiListCampiBt);
		
		campiToolBar.add(new SeparatorToolItem());
		
		campusDeslocamentoListCampiBt = createButton("Deslocamento<br />entre Campi", "Deslocamento entre Campi", Resources.DEFAULTS.deslocamentoCampi24());
		campiToolBar.add(campusDeslocamentoListCampiBt);
		
		semanasLetivaListCampiBt = createButton("Semanas<br />Letivas", "Semanas Letivas", Resources.DEFAULTS.semanaLetiva24());
		campiToolBar.add(semanasLetivaListCampiBt);
		
		horariosAulaListCampiBt = createButton("Horários<br />de Aula", "Horários de Aula", Resources.DEFAULTS.horarioAula24());
		campiToolBar.add(horariosAulaListCampiBt);
		
		turnosListCampiBt = createButton("Turnos", "Turnos", Resources.DEFAULTS.turno24());
		campiToolBar.add(turnosListCampiBt);
		
		ofertasListCampiBt = createButton("Oferta de Cursos<br />em Campi", "Oferta de Cursos em Campi", Resources.DEFAULTS.ofertaCurso24());
		campiToolBar.add(ofertasListCampiBt);
		
		usuariosListBt = createButton("Usuários", "Usuários", Resources.DEFAULTS.turno24()); // TODO Ajustar o icone de usuários
		campiToolBar.add(usuariosListBt);
	}
	
	private void createUnidades() {
		// TODO Implementar o cadastro de unidade pelo menu
		unidadesNovoUnidadesBt = createButton("Nova", "Adicionar uma Unidade", Resources.DEFAULTS.unidadeNovo24()); 
		unidadesToolBar.add(unidadesNovoUnidadesBt);
		
		unidadesListUnidadesBt = createButton("Listar", "Listar Unidades", Resources.DEFAULTS.unidadeListar24());
		unidadesToolBar.add(unidadesListUnidadesBt);
		
		unidadesToolBar.add(new SeparatorToolItem());
		
		unidadeDeslocamentoListUnidadesBt = createButton("Deslocamento<br />entre unidades", "Deslocamento de Unidades", Resources.DEFAULTS.deslocamentoUnidade24());
		unidadesToolBar.add(unidadeDeslocamentoListUnidadesBt);
	}
	
	private void createSalas() {
		salasNovoSalasBt = createButton("Nova", "Adicionar uma Sala", Resources.DEFAULTS.salaNovo24());
		salasToolBar.add(salasNovoSalasBt);
		
		salasListSalasBt = createButton("Listar", "Listar Salas", Resources.DEFAULTS.salaListar24());
		salasToolBar.add(salasListSalasBt);
		
		salasToolBar.add(new SeparatorToolItem());
		
		gruposSalasListSalasBt = createButton("Grupos de<br />Salas", "Grupos de Salas", Resources.DEFAULTS.grupoSala24());
		salasToolBar.add(gruposSalasListSalasBt);
		
		associarDisciplinasSalasListSalasBt = createButton("Associação de<br />Disciplinas à Salas", "Associação de Disciplinas à Salas", Resources.DEFAULTS.associacaoDisciplinaSala24());
		salasToolBar.add(associarDisciplinasSalasListSalasBt);
	}
	
	private void createCursos() {
		cursosNovoCursosBt = createButton("Novo", "Adicionar um Curso", Resources.DEFAULTS.cursoNovo24());
		cursosToolBar.add(cursosNovoCursosBt);
		
		cursosListCursosBt = createButton("Listar", "Listar Cursos", Resources.DEFAULTS.cursoListar24());
		cursosToolBar.add(cursosListCursosBt);
		
		cursosToolBar.add(new SeparatorToolItem());
		
		ofertasListCursosBt = createButton("Oferta de Cursos<br />em Campi", "Oferta de Cursos em Campi", Resources.DEFAULTS.ofertaCurso24());
		cursosToolBar.add(ofertasListCursosBt);
		
		areasTitulacaoListCursosBt = createButton("Áreas de<br />Titulação", "Áreas de Titulação", Resources.DEFAULTS.areaTitulacao());
		cursosToolBar.add(areasTitulacaoListCursosBt);
		
		vincularAreasTitulacaoListCursosBt = createButton("Vincular Áreas<br />de Titulação", "Vincular Áreas de Titulação", Resources.DEFAULTS.vincularAreaTitulacao24());
		cursosToolBar.add(vincularAreasTitulacaoListCursosBt);
		
		curriculosListCursosBt = createButton("Matrizes<br />Curriculares", "Matrizes Curriculares", Resources.DEFAULTS.matrizCurricular24());
		cursosToolBar.add(curriculosListCursosBt);
		
		tiposCursosListCursosBt = createButton("Tipos de<br />Curso", "Tipos de Curso", Resources.DEFAULTS.tipoCurso24());
		cursosToolBar.add(tiposCursosListCursosBt);
	}
	
	private void createDisciplinas() {
		disciplinasNovoDisciplinasBt = createButton("Novo", "Adicionar um Curso", Resources.DEFAULTS.disciplinaNovo24());
		disciplinasToolBar.add(disciplinasNovoDisciplinasBt);

		disciplinasListDisciplinasBt = createButton("Listar", "Listar Disciplinas", Resources.DEFAULTS.disciplinaListar24());
		disciplinasToolBar.add(disciplinasListDisciplinasBt);
		
		disciplinasToolBar.add(new SeparatorToolItem());

		associarDisciplinasSalasListDisciplinasBt = createButton("Associação de<br />Disciplinas à Salas", "Associação de Disciplinas à Salas", Resources.DEFAULTS.associacaoDisciplinaSala24());
		disciplinasToolBar.add(associarDisciplinasSalasListDisciplinasBt);
		
		curriculosListDisciplinasBt = createButton("Matrizes<br />Curriculares", "Matrizes Curriculares", Resources.DEFAULTS.matrizCurricular24());
		disciplinasToolBar.add(curriculosListDisciplinasBt);
		
		demandasDisciplinasBt = createButton("Previsão de<br />demanda", "Previsão de demanda", Resources.DEFAULTS.demanda24());
		disciplinasToolBar.add(demandasDisciplinasBt);
		
		divisaoCreditosListDisciplinasBt = createButton("Divisão de<br />Créditos", "Divisão de Créditos", Resources.DEFAULTS.divisaoCredito24());
		disciplinasToolBar.add(divisaoCreditosListDisciplinasBt);
		
		equivalenciasListDisciplinasBt = createButton("Equivalências", "Equivalências", Resources.DEFAULTS.equivalencia24());
		disciplinasToolBar.add(equivalenciasListDisciplinasBt);
		
		compatibilidadesListDisciplinasBt = createButton("Compatibilidade<br />entre disciplinas", "Compatibilidade entre disciplinas", Resources.DEFAULTS.compatibilidade24());
		disciplinasToolBar.add(compatibilidadesListDisciplinasBt);
	}
	
	private void createProfessores() {
		professoresNovoProfessoresBt = createButton("Novo", "Adicionar novo Professor", Resources.DEFAULTS.professorNovo24());
		professoresToolBar.add(professoresNovoProfessoresBt);
		
		professoresListprofessoresBt = createButton("Listar", "Listar Professores", Resources.DEFAULTS.professorListar24());
		professoresToolBar.add(professoresListprofessoresBt);
		
		professoresToolBar.add(new SeparatorToolItem());
		
		professoresDisciplinaListprofessoresBt = createButton("Habilitação<br />dos Professores", "Habilitação dos Professores", Resources.DEFAULTS.habilitacaoProfessor24());
		professoresToolBar.add(professoresDisciplinaListprofessoresBt);
		
		professoresCampusListprofessoresBt = createButton("Campi de<br />Trabalho", "Campi de Trabalho", Resources.DEFAULTS.campiTrabalho24());
		professoresToolBar.add(professoresCampusListprofessoresBt);
	}
	
	private void createRelatorios() {
		resumoCenarioBt = createButton("Resumo do<br />Cenário", "Resumo do Cenário", Resources.DEFAULTS.resumoCenario24());
		relatoriosToolBar.add(resumoCenarioBt);
		
		resumoCampiBt = createButton("Resumo por<br />Campus", "Resumo por Campus", Resources.DEFAULTS.resumoCampi24());
		relatoriosToolBar.add(resumoCampiBt);
		
		relatoriosToolBar.add(new SeparatorToolItem());
		
		relatorioVisaoSalaBt = createButton("Grade Horária<br />Visão Sala", "Grade Horária Visão Sala", Resources.DEFAULTS.saidaSala24());
		relatoriosToolBar.add(relatorioVisaoSalaBt);
		
		relatorioVisaoCursoBt = createButton("Grade Horária<br />Visão Curso", "Grade Horária Visão Curso", Resources.DEFAULTS.saidaCurso24());
		relatoriosToolBar.add(relatorioVisaoCursoBt);		
		
		relatorioVisaoProfessorBt = createButton("Grade Horária<br />Visão Professor", "Grade Horária Visão Professor", Resources.DEFAULTS.saidaProfessor24());
		relatoriosToolBar.add(relatorioVisaoProfessorBt);		
	}
	
	private void createPlanejamento() {
		fixacoesListBt = createButton("Fixações", "Fixações", Resources.DEFAULTS.fixacao24());
		planejamentoToolBar.add(fixacoesListBt);
		
		parametrosBt = createButton("Parâmetros de<br />Planejamento", "Parâmetros de Planejamento", Resources.DEFAULTS.parametroPlanejamento24());
		planejamentoToolBar.add(parametrosBt);
	}
	
	private void createCenario() {
		cenarioToolBar.add(createButton("Novo", "Adicionar novo Cenário", Resources.DEFAULTS.cenarioAbrir24()));
		cenarioToolBar.add(createButton("Listar", "Listar cenários", Resources.DEFAULTS.cenarioListar24()));
//		cenarioToolBar.add(createButton("Clonar", "Clonar um cenário", Resources.DEFAULTS.cenarioClonar24()));
//		cenarioToolBar.add(createButton("Abrir", "Abrir um cenário", Resources.DEFAULTS.cenarioNovo24()));
	}
	
	private Button createButton(String text, String toolTip, ImageResource icon) {
		Button bt = new Button(text, AbstractImagePrototype.create(icon));
		bt.setToolTip(toolTip);
		bt.setScale(ButtonScale.MEDIUM);
		bt.setIconAlign(IconAlign.TOP);
		bt.setArrowAlign(ButtonArrowAlign.BOTTOM);
		bt.setHeight(60);
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
	public Button getSalasNovoSalasButton() {
		return salasNovoSalasBt;
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
		return areasTitulacaoListCursosBt;
	}
	@Override
	public Button getVincularAreasTitulacaoListCursosButton() {
		return vincularAreasTitulacaoListCursosBt;
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
	public Button getCursosNovoCursosButton() {
		return cursosNovoCursosBt;
	}
	@Override
	public Button getCursosListCursosButton() {
		return cursosListCursosBt;
	}
	@Override
	public Button getDisciplinasNovoDisciplinasButton() {
		return disciplinasNovoDisciplinasBt;
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
	public Button getUsuariosListButton() {
		return usuariosListBt;
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
	public Button getFixacoesListButton() {
		return fixacoesListBt;
	}
	@Override
	public Button getCompatibilidadesListDisciplinasButton() {
		return compatibilidadesListDisciplinasBt;
	}
	@Override
	public Button getProfessoresNovoProfessoresButton() {
		return professoresNovoProfessoresBt;
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
	public Button getProfessoresCampusListprofessoresBt() {
		return professoresCampusListprofessoresBt;
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
	public Button getRelatorioVisaoProfessorButton() {
		return relatorioVisaoProfessorBt;
	}
	@Override
	public Button getResumoCenarioButton() {
		return resumoCenarioBt;
	}
	@Override
	public Button getResumoCampiButton() {
		return resumoCampiBt;
	}

	@Override
	public Button getTurnosListCampiButton() {
		return turnosListCampiBt;
	}
	
	@Override
	public Button getCampiNovoCampiButton() {
		return campiNovoCampiBt;
	}

	@Override
	public Button getCampiListCampiButton() {
		return campiListCampiBt;
	}
	
	@Override
	public Button getUnidadesNovoUnidadesButton() {
		return unidadesNovoUnidadesBt;
	}

	@Override
	public Button getUnidadesListUnidadesButton() {
		return unidadesListUnidadesBt;
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