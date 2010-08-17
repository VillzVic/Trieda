package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.Style.ButtonArrowAlign;
import com.extjs.gxt.ui.client.Style.ButtonScale;
import com.extjs.gxt.ui.client.Style.IconAlign;
import com.extjs.gxt.ui.client.event.ButtonEvent;
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
import com.gapso.web.trieda.client.util.resources.Resources;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class GToolBar extends ToolBar {

	public GToolBar() {
		createGroups();
	}
	
	private void createGroups() {
		createGroupUnidades();
		createGroupUnidades();
		createGroupUnidades();
		createGroupUnidades();
	}
	
	private void createGroupUnidades() {
		TableData data = new TableData();
		data.setRowspan(2);
		
		ButtonGroup group = new ButtonGroup(2);
		group.setHeading("Campi");

		Button campiBt = new Button("Campi");
		campiBt.setScale(ButtonScale.MEDIUM);
		campiBt.setIconAlign(IconAlign.TOP);
		campiBt.setArrowAlign(ButtonArrowAlign.BOTTOM);
		campiBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.campi24()));
		campiBt.addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Dispatcher.forwardEvent(AppEvents.CampusList);
			}
		});

		group.add(campiBt, data);
		
		Button unidadesBt = new Button("Unidades");
		unidadesBt.addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Dispatcher.forwardEvent(AppEvents.UnidadeList);
			}
		});
		unidadesBt.setIconAlign(IconAlign.LEFT);
		unidadesBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.unidade16()));
		group.add(unidadesBt);
		
		SplitButton salasBt = new SplitButton("Outros");
		salasBt.setIconAlign(IconAlign.LEFT);
		salasBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.sala16()));
		Menu menuSalasBt = new Menu();
		
		MenuItem outros1 = new MenuItem("Salas");
		outros1.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.SalaList);
			}
		});
		outros1.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.sala16()));
		menuSalasBt.add(outros1);
		
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
		
		MenuItem outros9 = new MenuItem("Períodos de Aula");
		outros9.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.periodoDeAula16()));
		outros9.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.PeriodoAulaList);
			}
		});
		menuSalasBt.add(outros9);
		
		MenuItem outros10 = new MenuItem("Turnos");
		outros10.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.turnos16()));
		outros10.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.TurnoList);
			}
		});
		menuSalasBt.add(outros10);
		
		MenuItem outros11 = new MenuItem("Áreas de Titulação");
		outros11.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.areasDeTitulacao16()));
		outros11.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.AreaTitulacaoList);
			}
		});
		menuSalasBt.add(outros11);
		
		MenuItem outros12 = new MenuItem("Vincular Áreas de Titulação");
		outros12.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.areasDeTitulacao16()));
		outros12.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.CursoAreaTitulacaoList);
			}
		});
		menuSalasBt.add(outros12);
		
		MenuItem outros13 = new MenuItem("Calendarios");
		outros13.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.calendario16()));
		outros13.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.CalendarioList);
			}
		});
		menuSalasBt.add(outros13);
		
		MenuItem outros14 = new MenuItem("Grupo de Salas");
		outros14.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.sala16()));
		outros14.addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Dispatcher.forwardEvent(AppEvents.GrupoSalaList);
			}
		});
		menuSalasBt.add(outros14);
		
		salasBt.setMenu(menuSalasBt);
		group.add(salasBt);
		
		add(group);
	}
	
}
