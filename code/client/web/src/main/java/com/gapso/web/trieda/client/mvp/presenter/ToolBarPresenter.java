package com.gapso.web.trieda.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.MessageBoxEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.DeslocamentoCampusDTO;
import com.gapso.web.trieda.client.mvp.view.AreasTitulacaoView;
import com.gapso.web.trieda.client.mvp.view.CampiDeslocamentoView;
import com.gapso.web.trieda.client.mvp.view.CampiView;
import com.gapso.web.trieda.client.mvp.view.CenariosView;
import com.gapso.web.trieda.client.mvp.view.CurriculosView;
import com.gapso.web.trieda.client.mvp.view.CursosView;
import com.gapso.web.trieda.client.mvp.view.DemandasView;
import com.gapso.web.trieda.client.mvp.view.DisciplinasAssociarSalaView;
import com.gapso.web.trieda.client.mvp.view.DisciplinasView;
import com.gapso.web.trieda.client.mvp.view.GruposSalasView;
import com.gapso.web.trieda.client.mvp.view.HorariosAulaView;
import com.gapso.web.trieda.client.mvp.view.OfertasView;
import com.gapso.web.trieda.client.mvp.view.RelatorioVisaoCursoView;
import com.gapso.web.trieda.client.mvp.view.RelatorioVisaoSalaView;
import com.gapso.web.trieda.client.mvp.view.SalasView;
import com.gapso.web.trieda.client.mvp.view.SemanasLetivaView;
import com.gapso.web.trieda.client.mvp.view.TiposCursosView;
import com.gapso.web.trieda.client.mvp.view.TurnosView;
import com.gapso.web.trieda.client.mvp.view.UnidadesDeslocamentoView;
import com.gapso.web.trieda.client.mvp.view.UnidadesView;
import com.gapso.web.trieda.client.services.CampiServiceAsync;
import com.gapso.web.trieda.client.services.OtimizarServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CenarioPanel;
import com.gapso.web.trieda.client.util.view.GTab;
import com.google.gwt.user.client.Timer;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.AbstractImagePrototype;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class ToolBarPresenter implements Presenter {

	public interface Display {
		Button getCampusListButton();
		Button getCampusNewButton();
		
		Component getComponent();
		
		Button getOtimizatButton();
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
		MenuItem getDemandasMenuItem();
		MenuItem getRelatorioVisaoSalaMenuItem();
		MenuItem getRelatorioVisaoCursoMenuItem();
	}
	
	private CenarioDTO masterData;
	private Display toolBar;
	private GTab gTab;
	private CenarioPanel cenarioPanel;
	
	public ToolBarPresenter(CenarioDTO masterData, CenarioPanel cenarioPanel, Display toolBar) {
		this.masterData = masterData;
		this.cenarioPanel = cenarioPanel;
		this.toolBar = toolBar;
		addListeners();
	}

	private void addListeners() {
		toolBar.getOtimizatButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				MessageBox.confirm("Otimizar?", "Deseja otimizar o cenário?", new Listener<MessageBoxEvent>() {
					@Override
					public void handleEvent(MessageBoxEvent be) {
						if(be.getButtonClicked().getText().equalsIgnoreCase("yes")) {
							OtimizarServiceAsync service = Services.otimizar();
							service.input(masterData, new AsyncCallback<Long>() {
								@Override
								public void onFailure(Throwable caught) {
									MessageBox.alert("ERRO!", "Deu falha na conexão", null);
								}
								@Override
								public void onSuccess(final Long round) {
									Info.display("Otimizando", "Otimizando com sucesso!");
									toolBar.getOtimizatButton().setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.ajax24()));
									toolBar.getOtimizatButton().disable();
									checkSolver(round);
								}
							});
						}
					}
				});
			}
		});
		toolBar.getCenariosListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new CenariosPresenter(cenarioPanel, new CenariosView());
				presenter.go(gTab);
			}
		});
		toolBar.getTurnosListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new TurnosPresenter(masterData, new TurnosView());
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
				Presenter presenter = new CampiPresenter(masterData, new CampiView());
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
				Presenter presenter = new CursosPresenter(masterData, new CursosView());
				presenter.go(gTab);
			}
		});
		toolBar.getDisciplinasListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new DisciplinasPresenter(masterData, new DisciplinasView());
				presenter.go(gTab);
			}
		});
		toolBar.getCurriculosListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new CurriculosPresenter(masterData, new CurriculosView());
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
		toolBar.getDemandasMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new DemandasPresenter(masterData, new DemandasView());
				presenter.go(gTab);
			}
		});
		toolBar.getRelatorioVisaoSalaMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new RelatorioVisaoSalaPresenter(masterData, new RelatorioVisaoSalaView());
				presenter.go(gTab);
			}
		});
		toolBar.getRelatorioVisaoCursoMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected(MenuEvent ce) {
				Presenter presenter = new RelatorioVisaoCursoPresenter(masterData, new RelatorioVisaoCursoView());
				presenter.go(gTab);
			}
		});
	}
	
	private void checkSolver(final Long round) {
		final Timer t = new Timer() {
			@Override
			public void run() {
				final OtimizarServiceAsync otimizarService = Services.otimizar();
				final FutureResult<Boolean> futureBoolean = new FutureResult<Boolean>();
				otimizarService.isOptimizing(round, futureBoolean);
				FutureSynchronizer synch = new FutureSynchronizer(futureBoolean);
				synch.addCallback(new AsyncCallback<Boolean>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Impossível de verificar, servidor fora do ar", null);
						toolBar.getOtimizatButton().enable();
					}
					@Override
					public void onSuccess(Boolean result) {
						if(futureBoolean.result()) {
							checkSolver(round);
						} else {
							toolBar.getOtimizatButton().setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.otimizar24()));
							Info.display("OTIMIZADO", "Otimização finalizada!");
							atualizaSaida(round);
						}
					}
				});
			}
		};
		t.schedule(5 * 1000);
	}
	
	private void atualizaSaida(final Long round) {
		final OtimizarServiceAsync otimizarService = Services.otimizar();
		final FutureResult<Boolean> futureBoolean = new FutureResult<Boolean>();
		otimizarService.saveContent(masterData, round, futureBoolean);
		FutureSynchronizer synch = new FutureSynchronizer(futureBoolean);
		synch.addCallback(new AsyncCallback<Boolean>() {
			@Override
			public void onFailure(Throwable caught) {
				MessageBox.alert("ERRO!", "Erro ao pegar a saída", null);
				toolBar.getOtimizatButton().enable();
			}
			@Override
			public void onSuccess(Boolean result) {
				if(futureBoolean.result()) {
					MessageBox.alert("OTIMIZADO", "Saída salva com sucesso", null);
				} else {
					MessageBox.alert("ERRO!", "Erro ao salvar a saída", null);
				}
				toolBar.getOtimizatButton().enable();
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
