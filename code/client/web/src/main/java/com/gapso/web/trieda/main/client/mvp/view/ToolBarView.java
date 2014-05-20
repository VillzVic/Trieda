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
import com.extjs.gxt.ui.client.widget.layout.HBoxLayoutData;
import com.extjs.gxt.ui.client.widget.menu.Menu;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.main.client.mvp.presenter.ToolBarPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class ToolBarView
	extends MyComposite
		implements ToolBarPresenter.Display
{
	private CenarioDTO cenarioDTO;
	private LayoutContainer container;
	private TabItem nomeContexto;
	private ContentPanel masterDataPanel;
	private Button carregarMasterDataBt;
	private ToolBar importacoesToolBar;
	private ToolBar campiToolBar;
	private ToolBar unidadesToolBar;
	private ToolBar salasToolBar;
	private ToolBar cursosToolBar;
	private ToolBar matrizesToolBar;
	private ToolBar disciplinasToolBar;
	private ToolBar alunosToolBar;
	private ToolBar professoresToolBar;
	private ToolBar relatoriosToolBar;
	private ToolBar exportacoesToolBar;
	private ToolBar ofertasDemandasToolBar;
	private ToolBar calendarioToolBar;
	private ToolBar planejamentoToolBar;
	private Button cenariosBt;
	private Button administracaoBt;
	private Button usuariosBt;

	// Importacoes
	private Button importarBt;
	private Button importarBaseIntBt;
	private Button importarOfertasIntBt;
	private Button requisicoesIntBt;
	
	// Campi
	private Button campiNovoCampiBt;
	private Button campiListCampiBt;
	private Button campusDeslocamentoListCampiBt;

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
	
	// Matrizes Curriculares
	private Button matrizesNovoMatrizesBt;
	private Button matrizesListMatrizesBt;
	private Button curriculosDisciplinasPreRequisitosListMatrizesBt;
	private Button curriculosDisciplinasCoRequisitosListMatrizesBt;
	

	// Disciplinas
	private Button disciplinasNovoDisciplinasBt;
	private Button disciplinasListDisciplinasBt;
	private Button associarDisciplinasSalasListDisciplinasBt;
	private Button associarDisciplinasGruposSalasListDisciplinasBt;
	private Button divisaoCreditosListDisciplinasBt;
	private Button divisaoCreditosDisciplinaListDisciplinasBt;
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
	private Button professoresListProfessoresVirtuaisBt;

	// Relatórios
	private Button gradeHorariaDropDownBt;
	private Button resumosDropDownBt;
	private Button atendimentosDropDownBt;
	private Button docentesDropDownBt;

	// Exportacoes
	private Button exportarBt;
	private Button exportarGradesERPBt;
	private Button exportarReqBt;
	private Button carregarSolucaoBt;
	
	// Ofertas e Demandas
	private Button ofertasListDemandasBt;
	private Button curriculosListDemandasBt;
	private Button curriculosDisciplinasPreRequisitosListDemandasBt;
	private Button curriculosDisciplinasCoRequisitosListDemandasBt;
	private Button alunosDisciplinasCursadasDemandasBt;
	private Button parametrosGeracaoDemandaBt;
	private Button demandasDemandasBt;
	private Button demandasPorAlunoDemandasBt;

	// Calendário
	private Button semanasLetivaListCampiBt;
	private Button turnosListCampiBt;

	// Planejamento
	private Button fixacoesListBt;
	private Button parametrosBt;
	private Button consultaRequisicoesOtimizacaoBt;
	private Button alocacaoManualBt;
	private Button confirmacaoTurmasBt;
	
	TabItem ofertasDemandasTabItem;
	TabItem planejamentoTabItem;
	TabItem relatoriosTabItem;
	TabItem calendarioTabItem;

	public ToolBarView( CenarioDTO cenarioDTO)
	{
		this.cenarioDTO = cenarioDTO;
		initUI();
	}

	private void initUI()
	{
		container = new LayoutContainer();

		carregarMasterDataBt = new Button();
		carregarMasterDataBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.voltar16()));
		carregarMasterDataBt.setToolTip("Carregar Master Data");
		carregarMasterDataBt.setScale(ButtonScale.SMALL);
		carregarMasterDataBt.setIconAlign(IconAlign.LEFT);
		carregarMasterDataBt.setArrowAlign(ButtonArrowAlign.BOTTOM);
		carregarMasterDataBt.setHeight(24);
		if (cenarioDTO.getMasterData())
		{
			carregarMasterDataBt.hide();
		}

		masterDataPanel = new ContentPanel()
		{
			@Override
			protected void afterRender()
			{
				super.afterRender();
				getCollapseBtn().setStyleAttribute("margin-top", "4px");
				getCollapseBtn().setStyleAttribute("margin-left", "15px");

			}
		};
		masterDataPanel.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.logo()));
		masterDataPanel.getHeader().addTool(carregarMasterDataBt);
		masterDataPanel.getHeader().getTool(0).setStyleAttribute("margin-right", "10px");
		masterDataPanel.getHeader().addTool(createCenariosButton());
		masterDataPanel.getHeader().getTool(1).setStyleAttribute("margin-right", "10px");
		masterDataPanel.getHeader().addTool(createAdministracaoButton());
		masterDataPanel.getHeader().getTool(2).setStyleAttribute("margin-right", "10px");
		masterDataPanel.getHeader().addTool(createUsuariosButton());
		masterDataPanel.setHeadingHtml("Contexto: " + cenarioDTO.getNome());
		masterDataPanel.getHeader().setStyleAttribute("line-height", "23px");
		masterDataPanel.getHeader().setStyleAttribute("font-size", "14px");
		masterDataPanel.setCollapsible(true);
		
		masterDataPanel.setBodyBorder( false );

		HBoxLayoutData flex = new HBoxLayoutData( new Margins( 0 ) );
		flex.setFlex( 1 );  
		//container.add( planejamentoPanel,
		//	new HBoxLayoutData( new Margins( 0 ) ) );
		container.add( masterDataPanel, flex );

		TabItem importacoesTabItem = new TabItem( "Importações" );
		TabItem campiTabItem = new TabItem( "Campi" );
		TabItem unidadesTabItem = new TabItem( "Unidades" );
		TabItem salasTabItem = new TabItem( "Ambientes" );
		TabItem cursosTabItem = new TabItem( "Cursos" );
		TabItem disciplinasTabItem = new TabItem( "Disciplinas" );
		TabItem matrizesTabItem = new TabItem( "Matrizes Curriculares" );
		TabItem alunosTabItem = new TabItem( "Alunos" );
		TabItem professoresTabItem = new TabItem( "Professores" );
		planejamentoTabItem = new TabItem( "Planejamento" );
		relatoriosTabItem = new TabItem( "Relatórios" );
		TabItem exportacoesTabItem = new TabItem( "Exportações" );
		calendarioTabItem = new TabItem( "Calendário" );
		ofertasDemandasTabItem = new TabItem( "Ofertas e Demandas" );

		//planejamentoToolBar = new ToolBar();

		importacoesToolBar = new ToolBar();
		campiToolBar = new ToolBar();
		campiToolBar.setLayoutData( new FitLayout() );
		unidadesToolBar = new ToolBar();
		salasToolBar = new ToolBar();
		cursosToolBar = new ToolBar();
		matrizesToolBar = new ToolBar();
		disciplinasToolBar = new ToolBar();
		alunosToolBar = new ToolBar();
		professoresToolBar = new ToolBar();
		relatoriosToolBar = new ToolBar();
		planejamentoToolBar = new ToolBar();
		exportacoesToolBar = new ToolBar();
		ofertasDemandasToolBar = new ToolBar();
		calendarioToolBar = new ToolBar();

		importacoesTabItem.add( importacoesToolBar );
		campiTabItem.add( campiToolBar );
		unidadesTabItem.add( unidadesToolBar );
		salasTabItem.add( salasToolBar );
		cursosTabItem.add( cursosToolBar );
		disciplinasTabItem.add( disciplinasToolBar );
		matrizesTabItem.add( matrizesToolBar );
		alunosTabItem.add( alunosToolBar );
		professoresTabItem.add( professoresToolBar );
		planejamentoTabItem.add( planejamentoToolBar );
		relatoriosTabItem.add( relatoriosToolBar );
		calendarioTabItem.add( calendarioToolBar );
		exportacoesTabItem.add( exportacoesToolBar );
		ofertasDemandasTabItem.add( ofertasDemandasToolBar );

		createGroups();

		int height = 92;

		TabPanel masterDataTab = new TabPanel();
		masterDataTab.setTabScroll(true);
		masterDataTab.addStyleName( "tabPanelMasterData" );
		masterDataTab.setHeight( height );

		masterDataTab.add( importacoesTabItem );
		masterDataTab.add( calendarioTabItem );
		masterDataTab.add( campiTabItem );
		masterDataTab.add( unidadesTabItem );
		masterDataTab.add( salasTabItem );
		masterDataTab.add( cursosTabItem );
		masterDataTab.add( disciplinasTabItem );
		masterDataTab.add( matrizesTabItem );
		masterDataTab.add( alunosTabItem );
		masterDataTab.add( ofertasDemandasTabItem );
		masterDataTab.add( professoresTabItem );
		masterDataTab.add( planejamentoTabItem );
		masterDataTab.add( relatoriosTabItem );
		masterDataTab.add( exportacoesTabItem );

		masterDataTab.setSelection( calendarioTabItem );

		masterDataPanel.setTopComponent( masterDataTab );

		container.setHeight( height );
		
/*		if (cenarioDTO.getMasterData())
		{
			getCarregarMasterDataButton().hide();
			getImportarOfertasButton().hide();
			getExportarGradesERPButton().hide();
			getExportarReqButton().hide();
			getOfertasDemandasTabItem().getHeader().hide();
			getProfessoresListProfessoresVirtuaisBt().hide();
			getPlanejamentoTabItem().getHeader().hide();
			getRelatoriosTabItem().getHeader().hide();
		}
		else
		{
			getCarregarMasterDataButton().show();
			getExportarGradesERPButton().show();
			getExportarReqButton().show();
			getImportarOfertasButton().show();
			getOfertasDemandasTabItem().getHeader().show();
			getProfessoresListProfessoresVirtuaisBt().show();
			getPlanejamentoTabItem().getHeader().show();
			getRelatoriosTabItem().getHeader().show();
		}*/

		initComponent( masterDataPanel );
	}

	private void createGroups()
	{
		createImportacoes();
		createCampi();
		createUnidades();
		createSalas();
		createCursos();
		createMatrizesCurriculares();
		createDisciplinas();
		createAlunos();
		createProfessores();
		createRelatorios();
		createExportacoes();
		createOfertasDemandas();
		createCalendario();
		createPlanejamento();
	}

	private void createImportacoes()
	{
		importarBt = createButton("Importar<br />(Excel)","Importar (Excel)",Resources.DEFAULTS.importar24());
		importacoesToolBar.add(importarBt);
		
		importarBaseIntBt = createButton("Importar Dados<br />Base (Integração)",
				"Importar Dados Base (Integração)",Resources.DEFAULTS.integracao24());
		importacoesToolBar.add(importarBaseIntBt);
		importarBaseIntBt.disable();
		
		importarOfertasIntBt = createButton("Importar Ofertas/Demandas<br />(Integração)",
				"Importar Ofertas/Demandas (Integração)",Resources.DEFAULTS.integracao24());
		importacoesToolBar.add(importarOfertasIntBt);
		importarOfertasIntBt.disable();
		
		requisicoesIntBt = createButton("Requisições de<br />Integração",
				"Requisições de Integração",Resources.DEFAULTS.reqIntegracao24());
		importacoesToolBar.add(requisicoesIntBt);
		requisicoesIntBt.disable();
		
		importacoesToolBar.add( new SeparatorToolItem() );
				
		carregarSolucaoBt = createButton("Carregar<br />Solução","Carregar Solução",Resources.DEFAULTS.trieda24());
		importacoesToolBar.add(carregarSolucaoBt);
	}
	
	private void createCampi()
	{
	
		campiNovoCampiBt = createButton( "Novo",
			"Adicionar um campus", Resources.DEFAULTS.campusNovo24() );
		campiToolBar.add( campiNovoCampiBt );
		campiNovoCampiBt.hide();

		campiListCampiBt = createButton( "Campi",
			"Listar Campi", Resources.DEFAULTS.campusListar24() );
		campiToolBar.add( campiListCampiBt );

		campiToolBar.add( new SeparatorToolItem() );

		campusDeslocamentoListCampiBt = createButton( "Deslocamento<br />entre Campi",
			"Deslocamento entre Campi", Resources.DEFAULTS.deslocamentoCampi24() );
		campiToolBar.add( campusDeslocamentoListCampiBt );
	}
	
	private void createUnidades()
	{
		unidadesNovoUnidadesBt = createButton( "Nova",
			"Adicionar uma Unidade", Resources.DEFAULTS.unidadeNovo24() ); 
		unidadesToolBar.add( unidadesNovoUnidadesBt );
		unidadesNovoUnidadesBt.hide();

		unidadesListUnidadesBt = createButton("Unidades",
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
		salasNovoSalasBt = createButton( "Novo",
			"Adicionar um Ambiente", Resources.DEFAULTS.salaNovo24() );
		salasToolBar.add( salasNovoSalasBt );
		salasNovoSalasBt.hide();

		salasListSalasBt = createButton( "Ambientes",
			"Listar Ambientes", Resources.DEFAULTS.salaListar24() );
		salasToolBar.add( salasListSalasBt );

		salasToolBar.add( new SeparatorToolItem() );
		
		associarDisciplinasSalasListSalasBt = createButton(
			"Associação de<br />Disciplinas à Ambientes",
			"Associação de Disciplinas à Ambientes", Resources.DEFAULTS.associacaoDisciplinaSala24() );
		salasToolBar.add( associarDisciplinasSalasListSalasBt );

		gruposSalasListSalasBt = createButton( "Grupos de<br />Ambientes",
			"Grupos de Ambientes", Resources.DEFAULTS.grupoSala24() );
		salasToolBar.add( gruposSalasListSalasBt );
		
		associarDisciplinasGruposSalasListSalasBt = createButton(
				"Associação de Disciplinas<br />à Grupos de Ambientes",
				"Associação de Disciplinas à Grupos de Ambientes", Resources.DEFAULTS.grupoSala24() );
			salasToolBar.add( associarDisciplinasGruposSalasListSalasBt );
	}

	private void createCursos()
	{
		cursosNovoCursosBt = createButton( "Novo",
			"Adicionar um Curso", Resources.DEFAULTS.cursoNovo24() );
		cursosToolBar.add( cursosNovoCursosBt );
		cursosNovoCursosBt.hide();

		cursosListCursosBt = createButton( "Cursos",
			"Listar Cursos", Resources.DEFAULTS.cursoListar24() );
		cursosToolBar.add( cursosListCursosBt );

		cursosToolBar.add( new SeparatorToolItem() );
		
		tiposCursosListCursosBt = createButton( "Tipos de<br />Curso",
			"Tipos de Curso", Resources.DEFAULTS.tipoCurso24() );
		cursosToolBar.add( tiposCursosListCursosBt );
		
		areasTitulacaoListCursosBt = createButton( "Áreas de<br />Conhecimento",
			"Áreas de Conhecimento", Resources.DEFAULTS.resumoFaixaDemanda24() );
		cursosToolBar.add( areasTitulacaoListCursosBt );

		vincularAreasTitulacaoListCursosBt = createButton( "Cursos e Áreas<br />de Conhecimento",
			"Cursos e Áreas de Conhecimento", Resources.DEFAULTS.resumoFaixaDemanda24() );
		cursosToolBar.add( vincularAreasTitulacaoListCursosBt );
	}
	
	private void createMatrizesCurriculares()
	{
		matrizesNovoMatrizesBt = createButton( "Novo",
			"Adicionar uma Matriz Curricular", Resources.DEFAULTS.matrizCurricular24() );
		matrizesToolBar.add( matrizesNovoMatrizesBt );
		matrizesNovoMatrizesBt.hide();

		matrizesListMatrizesBt = createButton( "Matrizes Curriculares",
			"Listar Matrizes Curriculares", Resources.DEFAULTS.matrizCurricular24() );
		matrizesToolBar.add( matrizesListMatrizesBt );
		
		matrizesToolBar.add( new SeparatorToolItem() );
		
		curriculosDisciplinasPreRequisitosListMatrizesBt = createButton( "Pré-Requisitos",
			"Disciplinas Pré-Requisitos", Resources.DEFAULTS.disciplinaCurriculo24() );
		matrizesToolBar.add( curriculosDisciplinasPreRequisitosListMatrizesBt );
		
		curriculosDisciplinasCoRequisitosListMatrizesBt = createButton( "Co-Requisitos",
			"Disciplinas Co-Requisitos", Resources.DEFAULTS.disciplinaCurriculo24() );
		matrizesToolBar.add( curriculosDisciplinasCoRequisitosListMatrizesBt );
	}

	private void createDisciplinas()
	{
		disciplinasNovoDisciplinasBt = createButton( "Novo",
			"Adicionar um Curso", Resources.DEFAULTS.disciplinaNovo24() );
		disciplinasToolBar.add( disciplinasNovoDisciplinasBt );
		disciplinasNovoDisciplinasBt.hide();

		disciplinasListDisciplinasBt = createButton( "Disciplinas",
			"Listar Disciplinas", Resources.DEFAULTS.disciplinaListar24() );
		disciplinasToolBar.add( disciplinasListDisciplinasBt );

		disciplinasToolBar.add( new SeparatorToolItem() );
		
		divisaoCreditosListDisciplinasBt = createButton( "Regras de Divisão<br />de Créditos",
			"Regras de Divisão de Créditos", Resources.DEFAULTS.divisaoCredito24() );
		disciplinasToolBar.add( divisaoCreditosListDisciplinasBt );

		divisaoCreditosDisciplinaListDisciplinasBt = createButton( "Divisão de Créditos<br />das Disciplinas",
				"Regras de Divisão de Créditos", Resources.DEFAULTS.divisaoCreditoDisciplina24() );
			disciplinasToolBar.add( divisaoCreditosDisciplinaListDisciplinasBt );
		
		equivalenciasListDisciplinasBt = createButton( "Equivalências",
			"Equivalências", Resources.DEFAULTS.equivalencia24() );
		disciplinasToolBar.add( equivalenciasListDisciplinasBt );

		associarDisciplinasSalasListDisciplinasBt = createButton(
			"Associação de<br />Disciplinas à Ambientes",
			"Associação de Disciplinas à Ambientes", Resources.DEFAULTS.associacaoDisciplinaSala24() );
		disciplinasToolBar.add( associarDisciplinasSalasListDisciplinasBt );
		
		associarDisciplinasGruposSalasListDisciplinasBt = createButton(
			"Associação de Disciplinas<br />à Grupos de Ambientes",
			"Associação de Disciplinas à Grupos de Ambientes", Resources.DEFAULTS.grupoSala24() );
		disciplinasToolBar.add( associarDisciplinasGruposSalasListDisciplinasBt );

		compatibilidadesListDisciplinasBt = createButton(
			"Compatibilidade<br />entre disciplinas",
			"Compatibilidade entre disciplinas", Resources.DEFAULTS.compatibilidade24() );
		disciplinasToolBar.add( compatibilidadesListDisciplinasBt );
		compatibilidadesListDisciplinasBt.hide();
	}

	private void createAlunos()
	{
		alunosNovoAlunoBt = createButton( "Novo",
			"Adicionar Novo Aluno", Resources.DEFAULTS.professorNovo24() );
		alunosToolBar.add( alunosNovoAlunoBt );
		alunosNovoAlunoBt.hide();

		alunosListAlunosBt = createButton( "Alunos",
			"Listar Alunos", Resources.DEFAULTS.professorListar24() );
		alunosToolBar.add( alunosListAlunosBt );
		
		alunosToolBar.add( new SeparatorToolItem() );
		
		alunosDisciplinasCursadasBt = createButton( "Disciplinas<br /> Cursadas",
				"Disciplinas Cursadas", Resources.DEFAULTS.alunoCurriculo24() );
			alunosToolBar.add( alunosDisciplinasCursadasBt );
	}

	private void createProfessores()
	{
		professoresNovoProfessoresBt = createButton( "Novo",
			"Adicionar Novo Professor", Resources.DEFAULTS.professorNovo24() );
		professoresToolBar.add( professoresNovoProfessoresBt );
		professoresNovoProfessoresBt.hide();

		professoresListprofessoresBt = createButton( "Professores",
			"Listar Professores", Resources.DEFAULTS.professorListar24() );
		professoresToolBar.add( professoresListprofessoresBt );

		professoresToolBar.add( new SeparatorToolItem() );

		professoresDisciplinaListprofessoresBt = createButton(
			"Habilitações", "Habilitação dos Professores",
			Resources.DEFAULTS.habilitacaoProfessor24() );
		professoresToolBar.add( professoresDisciplinaListprofessoresBt );

		professoresCampusListprofessoresBt = createButton( "Campi de<br />Trabalho",
			"Campi de Trabalho", Resources.DEFAULTS.campiTrabalho24() );
		professoresToolBar.add( professoresCampusListprofessoresBt );
		
		professoresListProfessoresVirtuaisBt = createButton( "Professores<br />Virtuais",
				"Professores Virtuais", Resources.DEFAULTS.professorVirtual24() );
			professoresToolBar.add( professoresListProfessoresVirtuaisBt );
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
		
		atendimentosDropDownBt = createButton( "Atendimentos",
				"Relatórios Atendimentos", Resources.DEFAULTS.resumoFaixaDemanda24() );
		Menu menuAtendimentos = new Menu();
		menuAtendimentos.add( createMenuItem("Atendimentos por Matrícula", Resources.DEFAULTS.resumoMatricula16()) );
		menuAtendimentos.add( createMenuItem("Atendimentos por Disciplina", Resources.DEFAULTS.resumoDisciplinas16()) );
		//menuAtendimentos.add( createMenuItem("Atendimentos por Faixa de Turma", Resources.DEFAULTS.disciplinaCurriculo16()) );
		menuAtendimentos.add( createMenuItem("Atendimentos por Faixa de Demanda", Resources.DEFAULTS.resumoFaixaDemanda16()) );
		menuAtendimentos.add( createMenuItem("Atendimentos por Faixa de Crédito", Resources.DEFAULTS.divisaoDeCreditos16()) );
		menuAtendimentos.add( createMenuItem("Atendimentos por Faixa de Disciplina", Resources.DEFAULTS.resumoDisciplinas16()) );
		menuAtendimentos.add( createMenuItem("Ambientes por Faixa de Utilização dos Horários", Resources.DEFAULTS.sala16()) );
		menuAtendimentos.add( createMenuItem("Ambientes por Faixa de Ocupação Média da Capacidade", Resources.DEFAULTS.sala16()) );
		atendimentosDropDownBt.setMenu(menuAtendimentos);
		relatoriosToolBar.add(atendimentosDropDownBt);
		
		docentesDropDownBt = createButton( "Docentes",
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
		
		gradeHorariaDropDownBt = createButton( "Grades Horárias",
				"Relatórios Grade Horária", Resources.DEFAULTS.saidaCurso24() );
		Menu menuGradeHoraria = new Menu();
		menuGradeHoraria.add( createMenuItem("Grade Horária Visão Ambiente", Resources.DEFAULTS.saidaSala16()) );
		menuGradeHoraria.add( createMenuItem("Grade Horária Visão Curso", Resources.DEFAULTS.saidaCurso16()) );
		menuGradeHoraria.add( createMenuItem("Grade Horária Visão Professor", Resources.DEFAULTS.saidaProfessor16()) );
		menuGradeHoraria.add( createMenuItem("Grade Horária Visão Aluno", Resources.DEFAULTS.saidaProfessor16()) );
		gradeHorariaDropDownBt.setMenu(menuGradeHoraria);
		relatoriosToolBar.add(gradeHorariaDropDownBt);
	}

	private void createExportacoes() {
		
		exportarBt = createButton("Exportar<br />para Excel","Exportar para Excel",Resources.DEFAULTS.exportar24());
		exportacoesToolBar.add(exportarBt);
		
		exportarGradesERPBt = createButton("Exportar Grades<br />Horárias (ERP)",
				"Exportar Grades Horárias (ERP)",Resources.DEFAULTS.integracao24());
		exportacoesToolBar.add(exportarGradesERPBt);
		exportarGradesERPBt.disable();
		
		exportarReqBt = createButton("Requisições de<br />Integração",
				"Requisições de Integração",Resources.DEFAULTS.reqIntegracao24());
		exportacoesToolBar.add(exportarReqBt);
		exportarReqBt.disable();
	}
	
	private void createOfertasDemandas() {
		ofertasListDemandasBt = createButton( "Oferta de Cursos<br />em Campi",
			"Oferta de Cursos em Campi", Resources.DEFAULTS.ofertaCurso24() );
		ofertasDemandasToolBar.add( ofertasListDemandasBt );
		
		demandasDemandasBt = createButton( "Ofertas e<br />Demandas",
			"Previsão de demanda", Resources.DEFAULTS.demanda24() );
		ofertasDemandasToolBar.add( demandasDemandasBt );
		
		demandasPorAlunoDemandasBt = createButton( "Demandas por<br />Aluno",
				"Previsão de demanda", Resources.DEFAULTS.demandaAluno24() );
		ofertasDemandasToolBar.add( demandasPorAlunoDemandasBt );
		
		ofertasDemandasToolBar.add( new SeparatorToolItem() );
		
		parametrosGeracaoDemandaBt = createButton( "Parâmetros<br />Geração de Demanda",
			"Parâmetros para Geração de Demanda", Resources.DEFAULTS.parametroPlanejamento24() );
		ofertasDemandasToolBar.add( parametrosGeracaoDemandaBt );
		
		curriculosListDemandasBt = createButton( "Matrizes<br />Curriculares",
				"Matrizes Curriculares", Resources.DEFAULTS.matrizCurricular24() );
		ofertasDemandasToolBar.add( getCurriculosListDemandasButton() );
			
		curriculosDisciplinasPreRequisitosListDemandasBt = createButton( "Pré-Requisitos",
			"Disciplinas Pré-Requisitos", Resources.DEFAULTS.disciplinaCurriculo24() );
		ofertasDemandasToolBar.add( getCurriculosDisciplinasPreRequisitosDemandasButton() );
			
		curriculosDisciplinasCoRequisitosListDemandasBt = createButton( "Co-Requisitos",
			"Disciplinas Co-Requisitos", Resources.DEFAULTS.disciplinaCurriculo24() );
		ofertasDemandasToolBar.add( getCurriculosDisciplinasCoRequisitosDemandasButton() );
		
		alunosDisciplinasCursadasDemandasBt = createButton( "Disciplinas<br /> Cursadas",
			"Disciplinas Cursadas", Resources.DEFAULTS.alunoCurriculo24() );
		ofertasDemandasToolBar.add( alunosDisciplinasCursadasDemandasBt );
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
		parametrosBt = createButton("Parâmetros de<br />Planejamento","Parâmetros de Planejamento",Resources.DEFAULTS.parametroPlanejamento24());
		planejamentoToolBar.add(parametrosBt);
		
		consultaRequisicoesOtimizacaoBt = createButton("Requisições<br />de Otimização","Requisições de Otimização",Resources.DEFAULTS.gerarGradeConsultaRequisicao24());
		planejamentoToolBar.add(consultaRequisicoesOtimizacaoBt);
		
		planejamentoToolBar.add( new SeparatorToolItem() );
		
		alocacaoManualBt = createButton("Alocação<br />Manual","Alocação Manual",Resources.DEFAULTS.alocacaoManual24());
		planejamentoToolBar.add(alocacaoManualBt);
		
		planejamentoToolBar.add( new SeparatorToolItem() );
		
		confirmacaoTurmasBt = createButton("Confirmação<br />de Turmas","Confirmação de Turmas",Resources.DEFAULTS.confirmacao24());
		planejamentoToolBar.add(confirmacaoTurmasBt);
		
		fixacoesListBt = createButton("Fixações","Fixações",Resources.DEFAULTS.fixacao24());
		fixacoesListBt.hide();
		planejamentoToolBar.add(fixacoesListBt);
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
	
	public Button createCenariosButton() {
		cenariosBt = new Button();
		cenariosBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.cenario16()));
		cenariosBt.setText("Cenários");
		cenariosBt.setToolTip("Cenários");
		cenariosBt.setScale(ButtonScale.MEDIUM);
		cenariosBt.setIconAlign(IconAlign.LEFT);
		cenariosBt.setArrowAlign(ButtonArrowAlign.BOTTOM);
		cenariosBt.setHeight(24);
		Menu menu = new Menu();
		menu.add(new MenuItem("Gerenciar Cenários", AbstractImagePrototype.create(Resources.DEFAULTS.cenario16()) ));
		menu.add(new MenuItem("Comparar Cenários", AbstractImagePrototype.create(Resources.DEFAULTS.compararCenarios16()) ));
		menu.add(new MenuItem("Gerenciar Requisições de Otimização", AbstractImagePrototype.create(Resources.DEFAULTS.gerarGradeConsultaRequisicao16()) ));
		cenariosBt.setMenu(menu);
		return cenariosBt;
	}
	
	public Button createUsuariosButton() {
		usuariosBt = new Button();
		usuariosBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.professor16()));
		usuariosBt.setText("Usuários");
		usuariosBt.setToolTip("Menu do Usuário");
		usuariosBt.setScale(ButtonScale.MEDIUM);
		usuariosBt.setIconAlign(IconAlign.LEFT);
		usuariosBt.setArrowAlign(ButtonArrowAlign.BOTTOM);
		usuariosBt.setHeight(24);
		
		Menu menu = new Menu();
		MenuItem menuItem = new MenuItem("Nome");
		menuItem.disable();
		menuItem.addStyleName("disabled-color");
		menu.add(menuItem);
		menu.add(new MenuItem("Alterar Senha", AbstractImagePrototype.create(Resources.DEFAULTS.senha16()) ));
		menu.add(new MenuItem("Sair", AbstractImagePrototype.create(Resources.DEFAULTS.logout16()) ));
		usuariosBt.setMenu(menu);
		return usuariosBt;
	}

	public Button createAdministracaoButton() {
		administracaoBt = new Button();
		administracaoBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.chave16()));
		administracaoBt.setText("Administração");
		administracaoBt.setToolTip("Administração");
		administracaoBt.setScale(ButtonScale.MEDIUM);
		administracaoBt.setIconAlign(IconAlign.LEFT);
		administracaoBt.setArrowAlign(ButtonArrowAlign.BOTTOM);
		administracaoBt.setHeight(24);
		Menu menu = new Menu();
		menu.add(new MenuItem("Usuários", AbstractImagePrototype.create(Resources.DEFAULTS.usuarios16()) ));
		menu.add(new MenuItem("Configurações", AbstractImagePrototype.create(Resources.DEFAULTS.gerarGradeConsultaRequisicao16()) ));
		administracaoBt.setMenu(menu);
		return administracaoBt;
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
	public Button getOfertasListDemandasButton()
	{
		return ofertasListDemandasBt;
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
	public Button getDivisaoCreditosListDisciplinasButton()
	{
		return divisaoCreditosListDisciplinasBt;
	}
	
	@Override
	public Button getDivisaoCreditosDisciplinaListDisciplinasButton()
	{
		return divisaoCreditosDisciplinaListDisciplinasBt;
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
	public Button getAlocacaoManualButton()
	{
		return alocacaoManualBt;
	}
	
	@Override
	public Button getConfirmacaoTurmasButton()
	{
		return confirmacaoTurmasBt;
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
	public Button getProfessoresListProfessoresVirtuaisBt()
	{
		return professoresListProfessoresVirtuaisBt;
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
	public Button getMatrizesNovoMatrizesButton() {
		return matrizesNovoMatrizesBt;
	}
	
	@Override
	public Button getMatrizesListMatrizesButton() {
		return matrizesListMatrizesBt;
	}

	@Override
	public Button getCurriculosDisciplinasPreRequisitosMatrizesButton() {
		return curriculosDisciplinasPreRequisitosListMatrizesBt;
	}

	
	@Override
	public Button getCurriculosDisciplinasCoRequisitosMatrizesButton() {
		return curriculosDisciplinasCoRequisitosListMatrizesBt;
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

	@Override
	public Button getDemandasDemandasButton()
	{
		return demandasDemandasBt;
	}
	
	@Override
	public Button getDemandasPorAlunoDemandasButton()
	{
		return demandasPorAlunoDemandasBt;
	}
	
	@Override
	public TabItem getNomeContextoTabItem()
	{
		return nomeContexto;
	}
	
	@Override
	public Button getCarregarMasterDataButton()
	{
		return carregarMasterDataBt;
	}
	
	@Override
	public MenuItem getGerenciarCenariosButton() {
		return (MenuItem) cenariosBt.getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getCompararCenariosButton() {
		return (MenuItem) cenariosBt.getMenu().getItem(1);
	}
	
	@Override
	public MenuItem getGerenciarRequisicoesCenariosButton() {
		return (MenuItem) cenariosBt.getMenu().getItem(2);
	}
		
	@Override
	public MenuItem getListarUsuariosButton() {
		return (MenuItem) administracaoBt.getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getConfiguracoesButton() {
		return (MenuItem) administracaoBt.getMenu().getItem(1);
	}
	
	
	@Override
	public Button getUsuariosButton()
	{
		return usuariosBt;
	}
	
	@Override
	public MenuItem getUsuariosNomeButton() {
		return (MenuItem) usuariosBt.getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getUsuariosAlterarSenhaButton() {
		return (MenuItem) usuariosBt.getMenu().getItem(1);
	}
	
	@Override
	public MenuItem getUsuariosSairButton() {
		return (MenuItem) usuariosBt.getMenu().getItem(2);
	}
	
	@Override
	public Button getImportarOfertasButton() {
		return importarOfertasIntBt;
	}
	
	@Override
	public Button getExportarGradesERPButton() {
		return exportarGradesERPBt;
	}
	
	@Override
	public Button getExportarReqButton() {
		return exportarReqBt;
	}
	
	@Override 
	public TabItem getOfertasDemandasTabItem() {
		return ofertasDemandasTabItem;
	}
	
	@Override 
	public TabItem getRelatoriosTabItem() {
		return relatoriosTabItem;
	}
	
	@Override 
	public TabItem getPlanejamentoTabItem() {
		return planejamentoTabItem;
	}
	
	@Override 
	public TabItem getCalendarioTabItem() {
		return calendarioTabItem;
	}
	
	@Override
	public ContentPanel getMasterDataPanel() {
		return masterDataPanel;
	}
}
