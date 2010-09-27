package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.Style.ButtonArrowAlign;
import com.extjs.gxt.ui.client.Style.ButtonScale;
import com.extjs.gxt.ui.client.Style.IconAlign;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.mvc.Dispatcher;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.button.ButtonGroup;
import com.extjs.gxt.ui.client.widget.button.SplitButton;
import com.extjs.gxt.ui.client.widget.layout.TableData;
import com.extjs.gxt.ui.client.widget.menu.Menu;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvp.presenter.ToolBarPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class ToolBarView extends MyComposite implements ToolBarPresenter.Display {

	private ToolBar toolBar;
	
	// Cenário
	private Button cenariosBt;
	private Button parametrosBt;
	private MenuItem turnosListMI;
	private MenuItem semanasLetivaListMI;
	private MenuItem horariosAulaListMI;
	private MenuItem campiListMI;
	private MenuItem unidadesListMI;
	private MenuItem salasListMI;
	private MenuItem gruposSalasListMI;
	private MenuItem tiposCursosListMI;
	private MenuItem areasTitulacaoListMI;
	private MenuItem unidadeDeslocamentoListMI;
	private MenuItem campusDeslocamentoListMI;
	private MenuItem cursosListMI;
	private MenuItem disciplinasListMI;
	
	// Campi
	private Button campiBt;
	private Button novoCampusBt;
	private Button deslocamentoCampiBt;

	// Unidades
	private Button unidadesBt;
	private Button novoUnidadeBt;
	private Button deslocamentoUnidadeBt;
	
	// Professores
	private Button professoresBt;
	private Button novoProfessorBt;
	private Button deslocamentoProfessorBt;
	
	// Cursos
	private Button cursosBt;
	private Button novoCursoBt;
	private Button deslocamentoCursoBt;
	
	// Disciplinas
	private Button disciplinasBt;
	private Button novoDisciplinaBt;
	private Button deslocamentoDisciplinaBt;
	
	// Salas
	private Button salasBt;
	private Button novoSalaBt;
	private Button deslocamentoSalaBt;
	
	public ToolBarView() {
		initUI();
	}
	
	private void initUI() {
		toolBar = new ToolBar();
		createGroups();
		initComponent(toolBar);
	}
	
	
	private void createGroups() {
		createGroupCenarios();
		createGroupCampi();
		createGroupUnidades();
		createGroupProfessores();
		createGroupDisciplinas();
		createGroupSalas();
		createGroupCursos();
		
		createGroupPrototipo();
	}
	
	/********************************
	 * CRIAÇÃO DOS BOTÕES
	 */
	
	private void createGroupCenarios() {
		ButtonGroup group = new ButtonGroup(2);
		group.setHeading("Cenários");
		
		TableData data = new TableData();
		data.setRowspan(2);
		
		cenariosBt = createButtonList(Resources.DEFAULTS.cenario24());
		group.add(cenariosBt, data);
		
		parametrosBt = createButton("Parâmetros", Resources.DEFAULTS.parametros16());
		group.add(parametrosBt);
		
		SplitButton outrosUnidadesSBt = new SplitButton("Outros");
		outrosUnidadesSBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.outros16()));
		outrosUnidadesSBt.setIconAlign(IconAlign.LEFT);
		Menu outros = new Menu();
		
		turnosListMI = createMenuItem("Turnos", Resources.DEFAULTS.turnos16());
		outros.add(turnosListMI);
		semanasLetivaListMI = createMenuItem("Semanas Letiva", Resources.DEFAULTS.semanaLetiva16());
		outros.add(semanasLetivaListMI);
		horariosAulaListMI = createMenuItem("Horários de Aula", Resources.DEFAULTS.horarioAula16());
		outros.add(horariosAulaListMI);
		campiListMI = createMenuItem("Campi", Resources.DEFAULTS.campi16());
		outros.add(campiListMI);
		unidadesListMI = createMenuItem("Unidades", Resources.DEFAULTS.unidades16());
		outros.add(unidadesListMI);
		salasListMI = createMenuItem("Salas", Resources.DEFAULTS.sala16());
		outros.add(salasListMI);
		gruposSalasListMI = createMenuItem("Grupos de Salas", Resources.DEFAULTS.sala16());
		outros.add(gruposSalasListMI);
		tiposCursosListMI = createMenuItem("Tipos de Curso", Resources.DEFAULTS.curso16());
		outros.add(tiposCursosListMI);
		areasTitulacaoListMI = createMenuItem("Áreas de Titulação", Resources.DEFAULTS.areasDeTitulacao16());
		outros.add(areasTitulacaoListMI);
		unidadeDeslocamentoListMI = createMenuItem("Deslocamento de Unidades", Resources.DEFAULTS.deslocamento16());
		outros.add(unidadeDeslocamentoListMI);
		campusDeslocamentoListMI = createMenuItem("Deslocamento de Campi", Resources.DEFAULTS.deslocamento16());
		outros.add(campusDeslocamentoListMI);
		cursosListMI = createMenuItem("Cursos", Resources.DEFAULTS.cursos16());
		outros.add(cursosListMI);
		disciplinasListMI = createMenuItem("Disciplinas", Resources.DEFAULTS.disciplina16());
		outros.add(disciplinasListMI);
		
		outrosUnidadesSBt.setMenu(outros);
		group.add(outrosUnidadesSBt);
		
		toolBar.add(group);
	}

	private void createGroupCampi() {
		ButtonGroup group = new ButtonGroup(2);
		group.setHeading("Campi");
		
		TableData data = new TableData();
		data.setRowspan(2);
		
		campiBt = createButtonList(Resources.DEFAULTS.campi24());
		group.add(campiBt, data);

		novoCampusBt = createButton("Novo", Resources.DEFAULTS.campus16());
		group.add(novoCampusBt);
		
		deslocamentoCampiBt = createButton("Desloc.", Resources.DEFAULTS.deslocamento16());
		group.add(deslocamentoCampiBt);
		
		toolBar.add(group);
	}

	private void createGroupUnidades() {
		ButtonGroup group = new ButtonGroup(2);
		group.setHeading("Unidade");
		
		TableData data = new TableData();
		data.setRowspan(2);
		
		unidadesBt = createButtonList(Resources.DEFAULTS.unidades24());
		group.add(unidadesBt, data);

		novoUnidadeBt = createButton("Novo", Resources.DEFAULTS.unidade16());
		group.add(novoUnidadeBt);
		
		deslocamentoUnidadeBt = createButton("Desloc.", Resources.DEFAULTS.deslocamento16());
		group.add(deslocamentoUnidadeBt);
		
		toolBar.add(group);
	}
	
	private void createGroupProfessores() {
		ButtonGroup group = new ButtonGroup(2);
		group.setHeading("Professores");
		
		TableData data = new TableData();
		data.setRowspan(2);
		
		professoresBt = createButtonList(Resources.DEFAULTS.professores24());
		group.add(professoresBt, data);

		novoProfessorBt = createButton("Novo", Resources.DEFAULTS.professor16());
		group.add(novoProfessorBt);
		
		deslocamentoProfessorBt = createButton("Desloc.", Resources.DEFAULTS.deslocamento16());
		group.add(deslocamentoProfessorBt);
		
		toolBar.add(group);
	}
	
	private void createGroupCursos() {
		ButtonGroup group = new ButtonGroup(2);
		group.setHeading("Cursos");
		
		TableData data = new TableData();
		data.setRowspan(2);
		
		cursosBt = createButtonList(Resources.DEFAULTS.cursos24());
		group.add(cursosBt, data);
		
		novoCursoBt = createButton("Novo", Resources.DEFAULTS.curso16());
		group.add(novoCursoBt);
		
		deslocamentoCursoBt = createButton("Desloc.", Resources.DEFAULTS.deslocamento16());
		group.add(deslocamentoCursoBt);
		
		toolBar.add(group);
	}
	
	private void createGroupDisciplinas() {
		ButtonGroup group = new ButtonGroup(2);
		group.setHeading("Disciplinas");
		
		TableData data = new TableData();
		data.setRowspan(2);
		
		disciplinasBt = createButtonList(Resources.DEFAULTS.professores24());
		group.add(disciplinasBt, data);

		novoDisciplinaBt = createButton("Novo", Resources.DEFAULTS.professor16());
		group.add(novoDisciplinaBt);
		
		deslocamentoDisciplinaBt = createButton("Desloc.", Resources.DEFAULTS.deslocamento16());
		group.add(deslocamentoDisciplinaBt);
		
		toolBar.add(group);
	}
	
	private void createGroupSalas() {
		ButtonGroup group = new ButtonGroup(2);
		group.setHeading("Salas");
		
		TableData data = new TableData();
		data.setRowspan(2);
		
		salasBt = createButtonList(Resources.DEFAULTS.professores24());
		group.add(salasBt, data);

		novoSalaBt = createButton("Novo", Resources.DEFAULTS.professor16());
		group.add(novoSalaBt);
		
		deslocamentoSalaBt = createButton("Desloc.", Resources.DEFAULTS.deslocamento16());
		group.add(deslocamentoSalaBt);
		
		toolBar.add(group);
	}
	
	private Button createButtonList(ImageResource icon) {
		Button bt = createButton("Listar", icon);
		bt.setScale(ButtonScale.MEDIUM);
		bt.setIconAlign(IconAlign.TOP);
		bt.setArrowAlign(ButtonArrowAlign.BOTTOM);
		
		return bt;
	}
	
	private Button createButton(String text, ImageResource icon) {
		Button bt = new Button(text, AbstractImagePrototype.create(icon));
		bt.setIconAlign(IconAlign.LEFT);
		return bt;
	}
	
	private MenuItem createMenuItem(String text, ImageResource icon) {
		MenuItem mi = new MenuItem(text, AbstractImagePrototype.create(icon));
		return mi;
	}
	
	private void createGroupPrototipo() {
		TableData data = new TableData();
		data.setRowspan(2);
		
		ButtonGroup group = new ButtonGroup(2);
		group.setHeading("Protótipo");

		Button campiBt = new Button("Campi");
		campiBt.setScale(ButtonScale.MEDIUM);
		campiBt.setIconAlign(IconAlign.TOP);
		campiBt.setArrowAlign(ButtonArrowAlign.BOTTOM);
		campiBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.campi24()));

		group.add(campiBt, data);
		
		Button unidadesBt = new Button("Unidades");
		unidadesBt.setIconAlign(IconAlign.LEFT);
		unidadesBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.unidade16()));
		group.add(unidadesBt);
		
		SplitButton salasBt = new SplitButton("Outros");
		salasBt.setIconAlign(IconAlign.LEFT);
		salasBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.sala16()));
		Menu menuSalasBt = new Menu();
		
		MenuItem outros2 = new MenuItem("Cursos");
		outros2.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.curso16()));
		outros2.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.CursoList);
			}
		});
		menuSalasBt.add(outros2);
		
		MenuItem outros3 = new MenuItem("Disciplinas");
		outros3.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.disciplina16()));
		outros3.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.DisciplinaList);
			}
		});
		menuSalasBt.add(outros3);
		
		MenuItem outros4 = new MenuItem("Matrizes Curriculares");
		outros4.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.matrizCurricular16()));
		outros4.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.MatrizCurricularList);
			}
		});
		menuSalasBt.add(outros4);

		MenuItem outros5 = new MenuItem("Oferta de Cursos em Campi");
		outros5.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.curso16()));
		outros5.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.CursoCampusList);
			}
		});
		menuSalasBt.add(outros5);
		
		MenuItem outros6 = new MenuItem("Professores");
		outros6.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.professores16()));
		outros6.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.ProfessorList);
			}
		});
		menuSalasBt.add(outros6);
		
		MenuItem outros7 = new MenuItem("Previssão de Demandas");
		outros7.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.demanda16()));
		outros7.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.DemandaList);
			}
		});
		menuSalasBt.add(outros7);
		
		MenuItem outros8 = new MenuItem("Preferência de Divisão de Créditos");
		outros8.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.divisaoDeCreditos16()));
		outros8.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.DivisaoCreditosList);
			}
		});
		menuSalasBt.add(outros8);
		
		MenuItem outros12 = new MenuItem("Vincular Áreas de Titulação");
		outros12.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.areasDeTitulacao16()));
		outros12.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.CursoAreaTitulacaoList);
			}
		});
		menuSalasBt.add(outros12);
		
		MenuItem outros15 = new MenuItem("Professores/Disciplinas");
		outros15.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.professores16()));
		outros15.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.ProfessorDisciplinaList);
			}
		});
		menuSalasBt.add(outros15);
		
		MenuItem outros16 = new MenuItem("Matriz Curricular/Disciplinas");
		outros16.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.disciplina16()));
		outros16.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.MatrizCurricularDisciplinaList);
			}
		});
		menuSalasBt.add(outros16);
		
		MenuItem outros17 = new MenuItem("Associação de Disciplinas à Salas");
		outros17.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.disciplina16()));
		outros17.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.SalaDisciplinaList);
			}
		});
		menuSalasBt.add(outros17);
		
		MenuItem outros18 = new MenuItem("Equivalência entre Disciplinas");
		outros18.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.disciplina16()));
		outros18.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.EquivalenciaList);
			}
		});
		menuSalasBt.add(outros18);
		
		MenuItem outros19 = new MenuItem("Campi/Professores");
		outros19.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.professores16()));
		outros19.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.CampusProfessorList);
			}
		});
		menuSalasBt.add(outros19);
		
		salasBt.setMenu(menuSalasBt);
		group.add(salasBt);
		toolBar.add(group);
	}
	
	/********************************
	 * GET BUTTONS
	 */
	
	@Override
	public Button getCampusListButton() {
		return campiBt;
	}

	@Override
	public Button getCampusNewButton() {
		return novoCampusBt;
	}
	
	@Override
	public MenuItem getTurnosListMenuItem() {
		return turnosListMI;
	}
	@Override
	public MenuItem getSemanasLetivaListMenuItem() {
		return semanasLetivaListMI;
	}
	@Override
	public MenuItem getHorariosAulaListMenuItem() {
		return horariosAulaListMI;
	}
	@Override
	public MenuItem getCampiListMenuItem() {
		return campiListMI;
	}
	@Override
	public MenuItem getUnidadesListMenuItem() {
		return unidadesListMI;
	}
	@Override
	public MenuItem getSalasListMenuItem() {
		return salasListMI;
	}
	@Override
	public MenuItem getGruposSalasListMenuItem() {
		return gruposSalasListMI;
	}
	@Override
	public MenuItem getTiposCursosListMenuItem() {
		return tiposCursosListMI;
	}
	@Override
	public MenuItem getAreasTitulacaoListMenuItem() {
		return areasTitulacaoListMI;
	}
	@Override
	public MenuItem getUnidadeDeslocamentoListMenuItem() {
		return unidadeDeslocamentoListMI;
	}
	@Override
	public MenuItem getCampusDeslocamentoListMenuItem() {
		return campusDeslocamentoListMI;
	}
	@Override
	public MenuItem getCursosListMenuItem() {
		return cursosListMI;
	}
	@Override
	public MenuItem getDisciplinasListMenuItem() {
		return disciplinasListMI;
	}
	
}
