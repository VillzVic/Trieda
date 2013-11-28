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
import com.extjs.gxt.ui.client.widget.menu.Menu;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.main.client.mvp.presenter.ToolBarPresenter;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class ToolBarView
	extends MyComposite
		implements ToolBarPresenter.Display
{
	private LayoutContainer container;
	private ToolBar campiToolBar;
	private ToolBar unidadesToolBar;
	private ToolBar salasToolBar;
	private ToolBar cursosToolBar;
	private ToolBar disciplinasToolBar;
	private ToolBar alunosToolBar;
	private ToolBar professoresToolBar;
	private ToolBar relatoriosToolBar;
	private ToolBar administracaoToolBar;
	private ToolBar geracaoDemandaToolBar;
	private ToolBar calendarioToolBar;
	private ToolBar planejamentoToolBar;

	// Campi
	private Button campiNovoCampiBt;
	private Button campiListCampiBt;
	private Button campusDeslocamentoListCampiBt;
	private Button ofertasListCampiBt;

	// Unidades
	private Button unidadesNovoUnidadesBt;
	private Button unidadesListUnidadesBt;
	private Button unidadeDeslocamentoListUnidadesBt;

	// Salas
	private Button salasNovoSalasBt;
	private Button salasListSalasBt;
	private Button gruposSalasListSalasBt;
	private Button associarDisciplinasSalasListSalasBt;
	private Button associarDisciplinasGruposSalasListSalasBt;

	// Cursos
	private Button cursosNovoCursosBt;
	private Button cursosListCursosBt;
	private Button tiposCursosListCursosBt;
	private Button areasTitulacaoListCursosBt;
	private Button vincularAreasTitulacaoListCursosBt;
	private Button curriculosListCursosBt;
	private Button curriculosDisciplinasPreRequisitosListCursosBt;
	private Button curriculosDisciplinasCoRequisitosListCursosBt;
	private Button ofertasListCursosBt;

	// Disciplinas
	private Button disciplinasNovoDisciplinasBt;
	private Button disciplinasListDisciplinasBt;
	private Button demandasDisciplinasBt;
	private Button curriculosListDisciplinasBt;
	private Button curriculosDisciplinasPreRequisitosListDisciplinasBt;
	private Button curriculosDisciplinasCoRequisitosListDisciplinasBt;
	private Button associarDisciplinasSalasListDisciplinasBt;
	private Button associarDisciplinasGruposSalasListDisciplinasBt;
	private Button divisaoCreditosListDisciplinasBt;
	private Button equivalenciasListDisciplinasBt;
	private Button compatibilidadesListDisciplinasBt;

	// Alunos
	private Button alunosNovoAlunoBt;
	private Button alunosListAlunosBt;
	private Button alunosDisciplinasCursadasBt;

	// Professores
	private Button professoresNovoProfessoresBt;
	private Button professoresListprofessoresBt;
	private Button professoresDisciplinaListprofessoresBt;
	private Button professoresCampusListprofessoresBt;

	// Relatórios
	private Button gradeHorariaDropDownBt;
	private Button resumosDropDownBt;
	private Button atendimentosDropDownBt;
	private Button docentesDropDownBt;

	// Administracao
	private Button usuariosListBt;
	private Button importarBt;
	private Button exportarBt;
	private Button carregarSolucaoBt;
	
	// Geracao de Demanda
	private Button curriculosListDemandasBt;
	private Button curriculosDisciplinasPreRequisitosListDemandasBt;
	private Button curriculosDisciplinasCoRequisitosListDemandasBt;
	private Button alunosDisciplinasCursadasDemandasBt;
	private Button parametrosGeracaoDemandaBt;

	// Calendário
	private Button semanasLetivaListCampiBt;
	private Button turnosListCampiBt;

	// Cenário
	private Button fixacoesListBt;
	private Button parametrosBt;
	private Button consultaRequisicoesOtimizacaoBt;

	public ToolBarView()
	{
		initUI();
	}

	private void initUI()
	{
		container = new LayoutContainer( new HBoxLayout() );

		ContentPanel planejamentoPanel = new ContentPanel();
		planejamentoPanel.setHeadingHtml( "Planejamento" );
		planejamentoPanel.setWidth( 270 );

		ContentPanel masterDataPanel = new ContentPanel();
		masterDataPanel.setHeaderVisible( false );

		planejamentoPanel.setBodyBorder( false );
		masterDataPanel.setBodyBorder( false );

		HBoxLayoutData flex = new HBoxLayoutData( new Margins( 0 ) );
		flex.setFlex( 1 );  
		container.add( planejamentoPanel,
			new HBoxLayoutData( new Margins( 0 ) ) );
		container.add( masterDataPanel, flex );

		TabItem campiTabItem = new TabItem( "Campi" );
		TabItem unidadesTabItem = new TabItem( "Unidades" );
		TabItem salasTabItem = new TabItem( "Salas" );
		TabItem cursosTabItem = new TabItem( "Cursos" );
		TabItem disciplinasTabItem = new TabItem( "Disciplinas" );
		TabItem alunosTabItem = new TabItem( "Alunos" );
		TabItem professoresTabItem = new TabItem( "Professores" );
		TabItem relatoriosTabItem = new TabItem( "Relatórios" );
		TabItem administracaoTabItem = new TabItem( "Administração" );
		TabItem calendarioTabItem = new TabItem( "Calendário" );
		TabItem geracaoDemandaTabItem = new TabItem( "Geração de Demanda" );

		planejamentoToolBar = new ToolBar();

		campiToolBar = new ToolBar();
		campiToolBar.setLayoutData( new FitLayout() );
		unidadesToolBar = new ToolBar();
		salasToolBar = new ToolBar();
		cursosToolBar = new ToolBar();
		disciplinasToolBar = new ToolBar();
		alunosToolBar = new ToolBar();
		professoresToolBar = new ToolBar();
		relatoriosToolBar = new ToolBar();
		administracaoToolBar = new ToolBar();
		geracaoDemandaToolBar = new ToolBar();
		calendarioToolBar = new ToolBar();

		campiTabItem.add( campiToolBar );
		unidadesTabItem.add( unidadesToolBar );
		salasTabItem.add( salasToolBar );
		cursosTabItem.add( cursosToolBar );
		disciplinasTabItem.add( disciplinasToolBar );
		alunosTabItem.add( alunosToolBar );
		professoresTabItem.add( professoresToolBar );
		relatoriosTabItem.add( relatoriosToolBar );
		calendarioTabItem.add( calendarioToolBar );
		administracaoTabItem.add( administracaoToolBar );
		geracaoDemandaTabItem.add( geracaoDemandaToolBar );

		createGroups();

		int height = 92;

		TabPanel masterDataTab = new TabPanel();
		masterDataTab.addStyleName( "tabPanelMasterData" );
		masterDataTab.setHeight( height );
		planejamentoToolBar.setHeight( height - 26 );

		TabItem masterDataItem = new TabItem( "Master Data" );
		masterDataItem.disable();
		masterDataTab.add( masterDataItem );
		masterDataTab.add( calendarioTabItem );
		masterDataTab.add( campiTabItem );
		masterDataTab.add( unidadesTabItem );
		masterDataTab.add( salasTabItem );
		masterDataTab.add( cursosTabItem );
		masterDataTab.add( disciplinasTabItem );
		masterDataTab.add( alunosTabItem );
		masterDataTab.add( professoresTabItem );
		masterDataTab.add( relatoriosTabItem );
		masterDataTab.add( administracaoTabItem );
		masterDataTab.add( geracaoDemandaTabItem );

		masterDataTab.setSelection( calendarioTabItem );

		planejamentoPanel.add( planejamentoToolBar );
		masterDataPanel.setTopComponent( masterDataTab );

		container.setHeight( height );

		initComponent( container );
	}

	private void createGroups()
	{
		createCampi();
		createUnidades();
		createSalas();
		createCursos();
		createDisciplinas();
		createAlunos();
		createProfessores();
		createRelatorios();
		createAdministracao();
		createGeracaoDemanda();
		createCalendario();
		createPlanejamento();
	}

	private void createCampi()
	{
		campiNovoCampiBt = createButton( "Novo",
			"Adicionar um campus", Resources.DEFAULTS.campusNovo24() );
		campiToolBar.add( campiNovoCampiBt );

		campiListCampiBt = createButton( "Listar",
			"Listar Campi", Resources.DEFAULTS.campusListar24() );
		campiToolBar.add( campiListCampiBt );

		campiToolBar.add( new SeparatorToolItem() );

		campusDeslocamentoListCampiBt = createButton( "Deslocamento<br />entre Campi",
			"Deslocamento entre Campi", Resources.DEFAULTS.deslocamentoCampi24() );
		campiToolBar.add( campusDeslocamentoListCampiBt );
		
		ofertasListCampiBt = createButton( "Oferta de Cursos<br />em Campi",
			"Oferta de Cursos em Campi", Resources.DEFAULTS.ofertaCurso24() );
		campiToolBar.add( ofertasListCampiBt );
	}
	
	private void createUnidades()
	{
		unidadesNovoUnidadesBt = createButton( "Nova",
			"Adicionar uma Unidade", Resources.DEFAULTS.unidadeNovo24() ); 
		unidadesToolBar.add( unidadesNovoUnidadesBt );

		unidadesListUnidadesBt = createButton("Listar",
			"Listar Unidades", Resources.DEFAULTS.unidadeListar24() );
		unidadesToolBar.add( unidadesListUnidadesBt );

		unidadesToolBar.add( new SeparatorToolItem() );

		unidadeDeslocamentoListUnidadesBt = createButton(
			"Deslocamento<br />entre unidades",
			"Deslocamento de Unidades", Resources.DEFAULTS.deslocamentoUnidade24() );
		unidadesToolBar.add( unidadeDeslocamentoListUnidadesBt );
	}
	
	private void createSalas()
	{
		salasNovoSalasBt = createButton( "Nova",
			"Adicionar uma Sala", Resources.DEFAULTS.salaNovo24() );
		salasToolBar.add( salasNovoSalasBt );

		salasListSalasBt = createButton( "Listar",
			"Listar Salas", Resources.DEFAULTS.salaListar24() );
		salasToolBar.add( salasListSalasBt );

		salasToolBar.add( new SeparatorToolItem() );

		gruposSalasListSalasBt = createButton( "Grupos de<br />Salas",
			"Grupos de Salas", Resources.DEFAULTS.grupoSala24() );
		salasToolBar.add( gruposSalasListSalasBt );

		associarDisciplinasSalasListSalasBt = createButton(
			"Associação de<br />Disciplinas à Salas",
			"Associação de Disciplinas à Salas", Resources.DEFAULTS.associacaoDisciplinaSala24() );
		salasToolBar.add( associarDisciplinasSalasListSalasBt );
		
		associarDisciplinasGruposSalasListSalasBt = createButton(
				"Associação de Disciplinas<br />à Grupos de Salas",
				"Associação de Disciplinas à Grupos Salas", Resources.DEFAULTS.grupoSala24() );
			salasToolBar.add( associarDisciplinasGruposSalasListSalasBt );
	}

	private void createCursos()
	{
		cursosNovoCursosBt = createButton( "Novo",
			"Adicionar um Curso", Resources.DEFAULTS.cursoNovo24() );
		cursosToolBar.add( cursosNovoCursosBt );

		cursosListCursosBt = createButton( "Listar",
			"Listar Cursos", Resources.DEFAULTS.cursoListar24() );
		cursosToolBar.add( cursosListCursosBt );

		cursosToolBar.add( new SeparatorToolItem() );

		ofertasListCursosBt = createButton( "Oferta de Cursos<br />em Campi",
			"Oferta de Cursos em Campi", Resources.DEFAULTS.ofertaCurso24() );
		cursosToolBar.add( ofertasListCursosBt );

		areasTitulacaoListCursosBt = createButton( "Áreas de<br />Conhecimento",
			"Áreas de Conhecimento", Resources.DEFAULTS.areaTitulacao() );
		cursosToolBar.add( areasTitulacaoListCursosBt );

		vincularAreasTitulacaoListCursosBt = createButton( "Vincular Áreas<br />de Conhecimento",
			"Vincular Áreas de Conhecimento", Resources.DEFAULTS.vincularAreaTitulacao24() );
		cursosToolBar.add( vincularAreasTitulacaoListCursosBt );

		curriculosListCursosBt = createButton( "Matrizes<br />Curriculares",
			"Matrizes Curriculares", Resources.DEFAULTS.matrizCurricular24() );
		cursosToolBar.add( curriculosListCursosBt );
		
		curriculosDisciplinasPreRequisitosListCursosBt = createButton( "Disciplinas<br />Pré-Requisitos",
			"Disciplinas Pré-Requisitos", Resources.DEFAULTS.disciplinaCurriculo24() );
		cursosToolBar.add( curriculosDisciplinasPreRequisitosListCursosBt );
		
		curriculosDisciplinasCoRequisitosListCursosBt = createButton( "Disciplinas<br />Co-Requisitos",
				"Disciplinas Co-Requisitos", Resources.DEFAULTS.disciplinaCurriculo24() );
			cursosToolBar.add( curriculosDisciplinasCoRequisitosListCursosBt );

		tiposCursosListCursosBt = createButton( "Tipos de<br />Curso",
			"Tipos de Curso", Resources.DEFAULTS.tipoCurso24() );
		cursosToolBar.add( tiposCursosListCursosBt );
	}

	private void createDisciplinas()
	{
		disciplinasNovoDisciplinasBt = createButton( "Novo",
			"Adicionar um Curso", Resources.DEFAULTS.disciplinaNovo24() );
		disciplinasToolBar.add( disciplinasNovoDisciplinasBt );

		disciplinasListDisciplinasBt = createButton( "Listar",
			"Listar Disciplinas", Resources.DEFAULTS.disciplinaListar24() );
		disciplinasToolBar.add( disciplinasListDisciplinasBt );

		disciplinasToolBar.add( new SeparatorToolItem() );

		associarDisciplinasSalasListDisciplinasBt = createButton(
			"Associação de<br />Disciplinas à Salas",
			"Associação de Disciplinas à Salas", Resources.DEFAULTS.associacaoDisciplinaSala24() );
		disciplinasToolBar.add( associarDisciplinasSalasListDisciplinasBt );
		
		associarDisciplinasGruposSalasListDisciplinasBt = createButton(
			"Associação de Disciplinas<br />à Grupos de Salas",
			"Associação de Disciplinas à Grupos Salas", Resources.DEFAULTS.grupoSala24() );
		disciplinasToolBar.add( associarDisciplinasGruposSalasListDisciplinasBt );

		curriculosListDisciplinasBt = createButton( "Matrizes<br />Curriculares",
			"Matrizes Curriculares", Resources.DEFAULTS.matrizCurricular24() );
		disciplinasToolBar.add( curriculosListDisciplinasBt );
		
		curriculosDisciplinasPreRequisitosListDisciplinasBt = createButton( "Disciplinas<br />Pré-Requisitos",
			"Disciplinas Pré-Requisitos", Resources.DEFAULTS.disciplinaCurriculo24() );
		disciplinasToolBar.add( curriculosDisciplinasPreRequisitosListDisciplinasBt );
		
		curriculosDisciplinasCoRequisitosListDisciplinasBt = createButton( "Disciplinas<br />Co-Requisitos",
				"Disciplinas Co-Requisitos", Resources.DEFAULTS.disciplinaCurriculo24() );
			disciplinasToolBar.add( curriculosDisciplinasCoRequisitosListDisciplinasBt );

		demandasDisciplinasBt = createButton( "Previsão de<br />demanda",
			"Previsão de demanda", Resources.DEFAULTS.demanda24() );
		disciplinasToolBar.add( demandasDisciplinasBt );

		divisaoCreditosListDisciplinasBt = createButton( "Regras de Divisão<br />de Créditos",
			"Regras de Divisão de Créditos", Resources.DEFAULTS.divisaoCredito24() );
		disciplinasToolBar.add( divisaoCreditosListDisciplinasBt );

		equivalenciasListDisciplinasBt = createButton( "Equivalências",
			"Equivalências", Resources.DEFAULTS.equivalencia24() );
		disciplinasToolBar.add( equivalenciasListDisciplinasBt );

		compatibilidadesListDisciplinasBt = createButton(
			"Compatibilidade<br />entre disciplinas",
			"Compatibilidade entre disciplinas", Resources.DEFAULTS.compatibilidade24() );
		disciplinasToolBar.add( compatibilidadesListDisciplinasBt );
	}

	private void createAlunos()
	{
		alunosNovoAlunoBt = createButton( "Novo",
			"Adicionar Novo Aluno", Resources.DEFAULTS.professorNovo24() );
		alunosToolBar.add( alunosNovoAlunoBt );

		alunosListAlunosBt = createButton( "Listar",
			"Listar Alunos", Resources.DEFAULTS.professorListar24() );
		alunosToolBar.add( alunosListAlunosBt );
		
		alunosDisciplinasCursadasBt = createButton( "Disciplinas<br /> Cursadas",
				"Disciplinas Cursadas", Resources.DEFAULTS.alunoCurriculo24() );
			alunosToolBar.add( alunosDisciplinasCursadasBt );
	}

	private void createProfessores()
	{
		professoresNovoProfessoresBt = createButton( "Novo",
			"Adicionar Novo Professor", Resources.DEFAULTS.professorNovo24() );
		professoresToolBar.add( professoresNovoProfessoresBt );

		professoresListprofessoresBt = createButton( "Listar",
			"Listar Professores", Resources.DEFAULTS.professorListar24() );
		professoresToolBar.add( professoresListprofessoresBt );

		professoresToolBar.add( new SeparatorToolItem() );

		professoresDisciplinaListprofessoresBt = createButton(
			"Habilitação<br />dos Professores", "Habilitação dos Professores",
			Resources.DEFAULTS.habilitacaoProfessor24() );
		professoresToolBar.add( professoresDisciplinaListprofessoresBt );

		professoresCampusListprofessoresBt = createButton( "Campi de<br />Trabalho",
			"Campi de Trabalho", Resources.DEFAULTS.campiTrabalho24() );
		professoresToolBar.add( professoresCampusListprofessoresBt );
	}

	private void createRelatorios()
	{
		resumosDropDownBt = createButton( "Resumos",
				"Resumos", Resources.DEFAULTS.resumoCenario24() );
		Menu menuResumos = new Menu();
		menuResumos.add( createMenuItem("Resumo do Cenário", Resources.DEFAULTS.resumoCenario16()) );
		menuResumos.add( createMenuItem("Resumo por Campus", Resources.DEFAULTS.resumoCampi16()) );
		menuResumos.add( createMenuItem("Resumo por Curso", Resources.DEFAULTS.resumoCursos16()) );
		menuResumos.add( createMenuItem("Resumo por Disciplina", Resources.DEFAULTS.resumoDisciplinas16()) );
		menuResumos.add( createMenuItem("Relatório de Alunos", Resources.DEFAULTS.professor16()) );
		menuResumos.add( createMenuItem("Relatório de Professores", Resources.DEFAULTS.professor16()) );
		menuResumos.add( createMenuItem("Relatório de Ambientes", Resources.DEFAULTS.sala16()) );
		resumosDropDownBt.setMenu(menuResumos);
		relatoriosToolBar.add(resumosDropDownBt);
		
		atendimentosDropDownBt = createButton( "Relatórios<br />Atendimentos",
				"Relatórios Atendimentos", Resources.DEFAULTS.resumoFaixaDemanda24() );
		Menu menuAtendimentos = new Menu();
		menuAtendimentos.add( createMenuItem("Atendimentos por Matrícula", Resources.DEFAULTS.resumoMatricula16()) );
		menuAtendimentos.add( createMenuItem("Atendimentos por Disciplina", Resources.DEFAULTS.resumoDisciplinas16()) );
		//menuAtendimentos.add( createMenuItem("Atendimentos por Faixa de Turma", Resources.DEFAULTS.disciplinaCurriculo16()) );
		menuAtendimentos.add( createMenuItem("Atendimentos por Faixa de Demanda", Resources.DEFAULTS.resumoFaixaDemanda16()) );
		menuAtendimentos.add( createMenuItem("Atendimentos por Faixa de Crédito", Resources.DEFAULTS.divisaoDeCreditos16()) );
		menuAtendimentos.add( createMenuItem("Atendimentos por Faixa de Disciplina", Resources.DEFAULTS.resumoDisciplinas16()) );
		menuAtendimentos.add( createMenuItem("Ambientes por Faixa de Ocupação dos Horários", Resources.DEFAULTS.sala16()) );
		menuAtendimentos.add( createMenuItem("Ambientes por Faixa de Utilização Média da Capacidade", Resources.DEFAULTS.sala16()) );
		atendimentosDropDownBt.setMenu(menuAtendimentos);
		relatoriosToolBar.add(atendimentosDropDownBt);
		
		docentesDropDownBt = createButton( "Relatórios<br />de Docentes",
				"Relatórios de Docentes", Resources.DEFAULTS.resumoMatricula24() );
		Menu menuDocentes = new Menu();
		menuDocentes.add( createMenuItem("Porcentagem de Mestres e Doutores", Resources.DEFAULTS.resumoMatricula16()) );
		menuDocentes.add( createMenuItem("Professores por Quantidade de Janelas", Resources.DEFAULTS.resumoMatricula16()) );
		menuDocentes.add( createMenuItem("Professores por Quantidade de Disciplinas Habilitadas", Resources.DEFAULTS.resumoDisciplinas16()) );
		menuDocentes.add( createMenuItem("Professores por Quantidade de Disciplinas Lecionadas", Resources.DEFAULTS.resumoDisciplinas16()) );
		menuDocentes.add( createMenuItem("Professores por Titulações", Resources.DEFAULTS.tipoCurso16()) );
		menuDocentes.add( createMenuItem("Professores por Areas de Conhecimento", Resources.DEFAULTS.habilitacaoProfessor16()) );
		docentesDropDownBt.setMenu(menuDocentes);
		relatoriosToolBar.add(docentesDropDownBt);

		relatoriosToolBar.add( new SeparatorToolItem() );
		
		gradeHorariaDropDownBt = createButton( "Relatórios<br /> Grade Horária",
				"Relatórios Grade Horária", Resources.DEFAULTS.saidaCurso24() );
		Menu menuGradeHoraria = new Menu();
		menuGradeHoraria.add( createMenuItem("Grade Horária Visão Sala", Resources.DEFAULTS.saidaSala16()) );
		menuGradeHoraria.add( createMenuItem("Grade Horária Visão Curso", Resources.DEFAULTS.saidaCurso16()) );
		menuGradeHoraria.add( createMenuItem("Grade Horária Visão Professor", Resources.DEFAULTS.saidaProfessor16()) );
		menuGradeHoraria.add( createMenuItem("Grade Horária Visão Aluno", Resources.DEFAULTS.saidaProfessor16()) );
		gradeHorariaDropDownBt.setMenu(menuGradeHoraria);
		relatoriosToolBar.add(gradeHorariaDropDownBt);
	}

	private void createAdministracao() {
		
		usuariosListBt = createButton("Usuários","Usuários",Resources.DEFAULTS.turno24());
		administracaoToolBar.add(usuariosListBt);

		importarBt = createButton("Importar<br />Tudo","Importar Tudo",Resources.DEFAULTS.importar24());
		administracaoToolBar.add(importarBt);

		exportarBt = createButton("Exportar<br />para Excel","Exportar para Excel",Resources.DEFAULTS.exportar24());
		administracaoToolBar.add(exportarBt);
		
		carregarSolucaoBt = createButton("Carregar<br />Solução","Carregar Solução",Resources.DEFAULTS.trieda24());
		administracaoToolBar.add(carregarSolucaoBt);
	}
	
	private void createGeracaoDemanda() {
		
		curriculosListDemandasBt = createButton( "Matrizes<br />Curriculares",
				"Matrizes Curriculares", Resources.DEFAULTS.matrizCurricular24() );
		geracaoDemandaToolBar.add( getCurriculosListDemandasButton() );
			
		curriculosDisciplinasPreRequisitosListDemandasBt = createButton( "Disciplinas<br />Pré-Requisitos",
			"Disciplinas Pré-Requisitos", Resources.DEFAULTS.disciplinaCurriculo24() );
		geracaoDemandaToolBar.add( getCurriculosDisciplinasPreRequisitosDemandasButton() );
			
		curriculosDisciplinasCoRequisitosListDemandasBt = createButton( "Disciplinas<br />Co-Requisitos",
			"Disciplinas Co-Requisitos", Resources.DEFAULTS.disciplinaCurriculo24() );
		geracaoDemandaToolBar.add( getCurriculosDisciplinasCoRequisitosDemandasButton() );
		
		alunosDisciplinasCursadasDemandasBt = createButton( "Disciplinas<br /> Cursadas",
			"Disciplinas Cursadas", Resources.DEFAULTS.alunoCurriculo24() );
		geracaoDemandaToolBar.add( alunosDisciplinasCursadasDemandasBt );
		
		parametrosGeracaoDemandaBt = createButton( "Parâmetros para<br /> Geração de Demanda",
			"Parâmetros para Geração de Demanda", Resources.DEFAULTS.parametroPlanejamento24() );
		geracaoDemandaToolBar.add( parametrosGeracaoDemandaBt );
		
	}

	private void createCalendario()
	{
		turnosListCampiBt = createButton("Turnos", "Turnos", Resources.DEFAULTS.turno24());
		calendarioToolBar.add(turnosListCampiBt);
		
		semanasLetivaListCampiBt = createButton( "Semanas<br />Letivas",
			"Semanas Letivas", Resources.DEFAULTS.semanaLetiva24() );
		calendarioToolBar.add( semanasLetivaListCampiBt );
	}

	private void createPlanejamento() {
		fixacoesListBt = createButton("Fixações","Fixações",Resources.DEFAULTS.fixacao24());
		planejamentoToolBar.add(fixacoesListBt);

		parametrosBt = createButton("Parâmetros de<br />Planejamento","Parâmetros de Planejamento",Resources.DEFAULTS.parametroPlanejamento24());
		planejamentoToolBar.add(parametrosBt);
		
		consultaRequisicoesOtimizacaoBt = createButton("Consulta Requisições<br />de Otimização","Consulta Requisições de Otimização",Resources.DEFAULTS.gerarGradeConsultaRequisicao24());
		planejamentoToolBar.add(consultaRequisicoesOtimizacaoBt);
	}

	private Button createButton( String text, String toolTip, ImageResource icon )
	{
		Button bt = new Button( text,
			AbstractImagePrototype.create( icon ) );

		bt.setToolTip( toolTip );
		bt.setScale( ButtonScale.MEDIUM );
		bt.setIconAlign( IconAlign.TOP );
		bt.setArrowAlign( ButtonArrowAlign.BOTTOM );
		bt.setHeight( 60 );

		return bt;
	}
	
	private MenuItem createMenuItem( String text, ImageResource icon )
	{
		MenuItem menuItem = new MenuItem( text, AbstractImagePrototype.create( icon ) );
		
		return menuItem;
	}
	
	@Override
	public Button getConsultaRequisicoesOtimizacaoBt() {
		return consultaRequisicoesOtimizacaoBt;
	}

	@Override
	public Button getParametrosButton()
	{
		return parametrosBt;
	}

	@Override
	public Button getSemanasLetivaListCampiButton()
	{
		return semanasLetivaListCampiBt;
	}

	@Override
	public Button getSalasNovoSalasButton()
	{
		return salasNovoSalasBt;
	}

	@Override
	public Button getSalasListSalasButton()
	{
		return salasListSalasBt;
	}

	@Override
	public Button getGruposSalasListSalasButton()
	{
		return gruposSalasListSalasBt;
	}

	@Override
	public Button getTiposCursosListCursosButton()
	{
		return tiposCursosListCursosBt;
	}

	@Override
	public Button getAreasTitulacaoListCursosButton()
	{
		return areasTitulacaoListCursosBt;
	}

	@Override
	public Button getVincularAreasTitulacaoListCursosButton()
	{
		return vincularAreasTitulacaoListCursosBt;
	}

	@Override
	public Button getUnidadeDeslocamentoListUnidadesButton()
	{
		return unidadeDeslocamentoListUnidadesBt;
	}

	@Override
	public Button getCampusDeslocamentoListCampiButton()
	{
		return campusDeslocamentoListCampiBt;
	}

	@Override
	public Button getCursosNovoCursosButton()
	{
		return cursosNovoCursosBt;
	}

	@Override
	public Button getCursosListCursosButton()
	{
		return cursosListCursosBt;
	}

	@Override
	public Button getDisciplinasNovoDisciplinasButton()
	{
		return disciplinasNovoDisciplinasBt;
	}

	@Override
	public Button getDisciplinasListDisciplinasButton()
	{
		return disciplinasListDisciplinasBt;
	}

	@Override
	public Button getCurriculosListCursosButton()
	{
		return curriculosListCursosBt;
	}

	@Override
	public Button getOfertasListCampiButton()
	{
		return ofertasListCampiBt;
	}

	@Override
	public Button getUsuariosListButton()
	{
		return usuariosListBt;
	}

	@Override
	public Button getImportarButton()
	{
		return importarBt;
	}

	@Override
	public Button getExportarButton()
	{
		return exportarBt;
	}
	
	@Override
	public Button getCarregarSolucaoButton() {
		return carregarSolucaoBt;
	}

	@Override
	public Button getAssociarDisciplinasSalasListSalasButton()
	{
		return associarDisciplinasSalasListSalasBt;
	}
	
	@Override
	public Button getAssociarDisciplinasGruposSalasListSalasButton()
	{
		return associarDisciplinasGruposSalasListSalasBt;
	}

	@Override
	public Button getDemandasDisciplinasButton()
	{
		return demandasDisciplinasBt;
	}

	@Override
	public Button getDivisaoCreditosListDisciplinasButton()
	{
		return divisaoCreditosListDisciplinasBt;
	}

	@Override
	public Button getEquivalenciasListDisciplinasButton()
	{
		return equivalenciasListDisciplinasBt;
	}

	@Override
	public Button getFixacoesListButton()
	{
		return fixacoesListBt;
	}

	@Override
	public Button getCompatibilidadesListDisciplinasButton()
	{
		return compatibilidadesListDisciplinasBt;
	}

	@Override
	public Button getProfessoresNovoProfessoresButton()
	{
		return professoresNovoProfessoresBt;
	}

	@Override
	public Button getProfessoresListProfessoresButton()
	{
		return professoresListprofessoresBt;
	}

	@Override
	public Button getProfessoresDisciplinaListProfessoresButton()
	{
		return professoresDisciplinaListprofessoresBt;
	}

	@Override
	public Button getProfessoresCampusListprofessoresBt()
	{
		return professoresCampusListprofessoresBt;
	}

	@Override
	public MenuItem getRelatorioVisaoSalaMenuItem()
	{
		return (MenuItem) gradeHorariaDropDownBt.getMenu().getItem(0);
	}

	@Override
	public MenuItem getRelatorioVisaoCursoMenuItem()
	{
		return (MenuItem) gradeHorariaDropDownBt.getMenu().getItem(1);
	}

	@Override
	public MenuItem getRelatorioVisaoProfessorMenuItem()
	{
		return (MenuItem) gradeHorariaDropDownBt.getMenu().getItem(2);
	}

	@Override
	public MenuItem getRelatorioVisaoAlunoMenuItem()
	{
		return (MenuItem) gradeHorariaDropDownBt.getMenu().getItem(3);
	}
	
	@Override
	public MenuItem getResumoCenarioMenuItem()
	{
		return (MenuItem) resumosDropDownBt.getMenu().getItem(0);
	}

	@Override
	public MenuItem getResumoCampiMenuItem()
	{
		return (MenuItem) resumosDropDownBt.getMenu().getItem(1);
	}

	@Override
	public MenuItem getResumoCursosMenuItem()
	{
		return (MenuItem) resumosDropDownBt.getMenu().getItem(2);
	}

	@Override
	public MenuItem getResumoDisciplinasMenuItem()
	{
		return (MenuItem) resumosDropDownBt.getMenu().getItem(3);
	}
	
	@Override
	public MenuItem getRelatorioAlunosMenuItem()
	{
		return (MenuItem) resumosDropDownBt.getMenu().getItem(4);
	}
	
	@Override
	public MenuItem getRelatorioProfessoresMenuItem()
	{
		return (MenuItem) resumosDropDownBt.getMenu().getItem(5);
	}
	
	@Override
	public MenuItem getRelatorioSalasMenuItem()
	{
		return (MenuItem) resumosDropDownBt.getMenu().getItem(6);
	}
	
	@Override
	public MenuItem getResumoMatriculasMenuItem()
	{
		return (MenuItem) atendimentosDropDownBt.getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getResumoAtendimentosDisciplinaMenuItem()
	{
		return (MenuItem) atendimentosDropDownBt.getMenu().getItem(1);
	}
	
/*	@Override
	public MenuItem getResumoAtendimentosFaixaTurmaMenuItem()
	{
		return (MenuItem) atendimentosDropDownBt.getMenu().getItem(2);
	}*/

	@Override
	public MenuItem getResumoAtendimentosFaixaDemandaMenuItem()
	{
		return (MenuItem) atendimentosDropDownBt.getMenu().getItem(2);
	}
	
	@Override
	public MenuItem getAtendimentosFaixaCreditoMenuItem()
	{
		return (MenuItem) atendimentosDropDownBt.getMenu().getItem(3);
	}
	
	@Override
	public MenuItem getAtendimentosFaixaDisciplinaMenuItem()
	{
		return (MenuItem) atendimentosDropDownBt.getMenu().getItem(4);
	}
	
	@Override
	public MenuItem getAmbientesFaixaOcupacaoHorariosMenuItem()
	{
		return (MenuItem) atendimentosDropDownBt.getMenu().getItem(5);
	}
	
	@Override
	public MenuItem getAmbientesFaixaUtilizacaoCapacidadeMenuItem()
	{
		return (MenuItem) atendimentosDropDownBt.getMenu().getItem(6);
	}
	
	@Override
	public MenuItem getResumoPercentMestresDoutoresMenuItem()
	{
		return (MenuItem) docentesDropDownBt.getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getProfessoresJanelasGradeMenuItem()
	{
		return (MenuItem) docentesDropDownBt.getMenu().getItem(1);
	}
	
	@Override
	public MenuItem getProfessoresDisciplinasHabilitadasMenuItem()
	{
		return (MenuItem) docentesDropDownBt.getMenu().getItem(2);
	}
	
	@Override
	public MenuItem getProfessoresDisciplinasLecionadasMenuItem()
	{
		return (MenuItem) docentesDropDownBt.getMenu().getItem(3);
	}
	
	@Override
	public MenuItem getProfessoresTitulacoesMenuItem()
	{
		return (MenuItem) docentesDropDownBt.getMenu().getItem(4);
	}
	
	@Override
	public MenuItem getProfessoresAreasConhecimentoMenuItem()
	{
		return (MenuItem) docentesDropDownBt.getMenu().getItem(5);
	}

	@Override
	public Button getTurnosListCampiButton()
	{
		return turnosListCampiBt;
	}

	@Override
	public Button getCampiNovoCampiButton()
	{
		return campiNovoCampiBt;
	}

	@Override
	public Button getCampiListCampiButton()
	{
		return campiListCampiBt;
	}

	@Override
	public Button getUnidadesNovoUnidadesButton()
	{
		return unidadesNovoUnidadesBt;
	}

	@Override
	public Button getUnidadesListUnidadesButton()
	{
		return unidadesListUnidadesBt;
	}

	@Override
	public Button getOfertasListCursosButton()
	{
		return ofertasListCursosBt;
	}

	@Override
	public Button getAssociarDisciplinasSalasListDisciplinasButton()
	{
		return associarDisciplinasSalasListDisciplinasBt;
	}
	
	@Override
	public Button getAssociarDisciplinasGruposSalasListDisciplinasButton()
	{
		return associarDisciplinasGruposSalasListDisciplinasBt;
	}

	@Override
	public Button getCurriculosListDisciplinasButton()
	{
		return curriculosListDisciplinasBt;
	}

	@Override
	public Button getAlunosNovoAlunoButton()
	{
		return alunosNovoAlunoBt;
	}

	@Override
	public Button getAlunosListAlunosButton()
	{
		return alunosListAlunosBt;
	}

	@Override
	public Button getCurriculosDisciplinasPreRequisitosCursosButton() {
		return curriculosDisciplinasPreRequisitosListCursosBt;
	}

	@Override
	public Button getCurriculosDisciplinasPreRequisitosDisciplinasButton() {
		return curriculosDisciplinasPreRequisitosListDisciplinasBt;
	}
	
	@Override
	public Button getCurriculosDisciplinasCoRequisitosCursosButton() {
		return curriculosDisciplinasCoRequisitosListCursosBt;
	}

	@Override
	public Button getCurriculosDisciplinasCoRequisitosDisciplinasButton() {
		return curriculosDisciplinasCoRequisitosListDisciplinasBt;
	}
	
	@Override
	public Button getAlunosDisciplinasCursadasButton() {
		return alunosDisciplinasCursadasBt;
	}

	@Override
	public Button getCurriculosListDemandasButton() {
		return curriculosListDemandasBt;
	}

	@Override
	public Button getCurriculosDisciplinasPreRequisitosDemandasButton() {
		return curriculosDisciplinasPreRequisitosListDemandasBt;
	}

	@Override
	public Button getCurriculosDisciplinasCoRequisitosDemandasButton() {
		return curriculosDisciplinasCoRequisitosListDemandasBt;
	}
	
	@Override
	public Button getAlunosDisciplinasCursadasDemandasButton() {
		return alunosDisciplinasCursadasDemandasBt;
	}
	
	@Override
	public Button getParametrosGeracaoDemandaButton() {
		return parametrosGeracaoDemandaBt;
	}

}
