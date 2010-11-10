package com.gapso.web.trieda.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.client.mvp.model.DeslocamentoCampusDTO;
import com.gapso.web.trieda.client.mvp.view.AreasTitulacaoView;
import com.gapso.web.trieda.client.mvp.view.CampiDeslocamentoView;
import com.gapso.web.trieda.client.mvp.view.CampiView;
import com.gapso.web.trieda.client.mvp.view.CenariosView;
import com.gapso.web.trieda.client.mvp.view.CurriculosView;
import com.gapso.web.trieda.client.mvp.view.CursosView;
import com.gapso.web.trieda.client.mvp.view.DisciplinasAssociarSalaView;
import com.gapso.web.trieda.client.mvp.view.DisciplinasView;
import com.gapso.web.trieda.client.mvp.view.GruposSalasView;
import com.gapso.web.trieda.client.mvp.view.HorariosAulaView;
import com.gapso.web.trieda.client.mvp.view.OfertasView;
import com.gapso.web.trieda.client.mvp.view.SalasView;
import com.gapso.web.trieda.client.mvp.view.SemanasLetivaView;
import com.gapso.web.trieda.client.mvp.view.TiposCursosView;
import com.gapso.web.trieda.client.mvp.view.TurnosView;
import com.gapso.web.trieda.client.mvp.view.UnidadesDeslocamentoView;
import com.gapso.web.trieda.client.mvp.view.UnidadesView;
import com.gapso.web.trieda.client.services.CampiServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.GTab;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class ToolBarPresenter implements Presenter {

	public interface Display {
		Button getCampusListButton();
		Button getCampusNewButton();
		
		Component getComponent();
		
		MenuItem getCenariosListMenuItem();
		MenuItem getTurnosListMenuItem();
		MenuItem getSemanasLetivaListMenuItem();
		MenuItem getHorariosAulaListMenuItem();
		MenuItem getCampiListMenuItem();
		MenuItem getUnidadesListMenuItem();
		MenuItem getSalasListMenuItem();
		MenuItem getGruposSalasListMenuItem();
		MenuItem getTiposCursosListMenuItem();
		MenuItem getAreasTitulacaoListMenuItem();
		MenuItem getUnidadeDeslocamentoListMenuItem();
		MenuItem getCampusDeslocamentoListMenuItem();
		MenuItem getCursosListMenuItem();
		MenuItem getDisciplinasListMenuItem();
		MenuItem getCurriculosListMenuItem();
		MenuItem getOfertasListMenuItem();
		MenuItem getAssociarDisciplinasSalasListMenuItem();
	}
	
	private Display toolBar;
	private GTab gTab;
	
	public ToolBarPresenter(Display toolBar) {
		this.toolBar = toolBar;
		addListeners();
	}

	private void addListeners() {
		toolBar.getCampusListButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				MessageBox.alert("Desenvolvimento", "Esta sessão está em desenvolvimento", null);
			}
		});
		toolBar.getCampusNewButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				MessageBox.alert("Desenvolvimento", "Esta sessão está em desenvolvimento", null);
			}
		});
		toolBar.getCenariosListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new CenariosPresenter(new CenariosView());
				presenter.go(gTab);
			}
		});
		toolBar.getTurnosListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new TurnosPresenter(new TurnosView());
				presenter.go(gTab);
			}
		});
		toolBar.getSemanasLetivaListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new SemanasLetivaPresenter(new SemanasLetivaView());
				presenter.go(gTab);
			}
		});
		toolBar.getHorariosAulaListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new HorariosAulaPresenter(new HorariosAulaView());
				presenter.go(gTab);
			}
		});
		toolBar.getCampiListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new CampiPresenter(new CampiView());
				presenter.go(gTab);
			}
		});
		toolBar.getUnidadesListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new UnidadesPresenter(new UnidadesView());
				presenter.go(gTab);
			}
		});
		toolBar.getSalasListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new SalasPresenter(new SalasView());
				presenter.go(gTab);
			}
		});
		toolBar.getGruposSalasListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new GruposSalasPresenter(new GruposSalasView());
				presenter.go(gTab);
			}
		});
		toolBar.getTiposCursosListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new TiposCursosPresenter(new TiposCursosView());
				presenter.go(gTab);
			}
		});
		toolBar.getAreasTitulacaoListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new AreasTitulacaoPresenter(new AreasTitulacaoView());
				presenter.go(gTab);
			}
		});
		toolBar.getUnidadeDeslocamentoListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new UnidadesDeslocamentoPresenter(new UnidadesDeslocamentoView(null, null));
				presenter.go(gTab);
			}
		});
		toolBar.getCampusDeslocamentoListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				// TODO ESTE CODIGO CODIGO NÂO PERTENCE AQUI, DEVE FICAR NO CAMPI DESLOCAMENTO
				// QUANDO EU COLOCO LA, ELE BUGA O HEADER DA TABELA
				CampiServiceAsync service = Services.campi();
				service.getDeslocamento(null, null, null, null, null, new AsyncCallback<List<DeslocamentoCampusDTO>>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(List<DeslocamentoCampusDTO> result) {
						Presenter presenter = new CampiDeslocamentoPresenter(new CampiDeslocamentoView(result));
						presenter.go(gTab);	
					}
				});
			}
		});
		toolBar.getCursosListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new CursosPresenter(new CursosView());
				presenter.go(gTab);
			}
		});
		toolBar.getDisciplinasListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new DisciplinasPresenter(new DisciplinasView());
				presenter.go(gTab);
			}
		});
		toolBar.getCurriculosListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new CurriculosPresenter(new CurriculosView());
				presenter.go(gTab);
			}
		});
		toolBar.getOfertasListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new OfertasPresenter(new OfertasView());
				presenter.go(gTab);
			}
		});
		toolBar.getAssociarDisciplinasSalasListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new DisciplinasAssociarSalaPresenter(new DisciplinasAssociarSalaView());
				presenter.go(gTab);
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		AppPresenter.Display container = (AppPresenter.Display) widget;
		this.gTab = container.getGTab();
		container.getPanel().setTopComponent(toolBar.getComponent());
	}

}
