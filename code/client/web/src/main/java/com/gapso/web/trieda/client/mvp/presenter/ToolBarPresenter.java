package com.gapso.web.trieda.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.DeslocamentoCampusDTO;
import com.gapso.web.trieda.client.mvp.view.AreasTitulacaoView;
import com.gapso.web.trieda.client.mvp.view.CampiDeslocamentoView;
import com.gapso.web.trieda.client.mvp.view.CampiView;
import com.gapso.web.trieda.client.mvp.view.CampusProfessoresView;
import com.gapso.web.trieda.client.mvp.view.CurriculosView;
import com.gapso.web.trieda.client.mvp.view.CursosView;
import com.gapso.web.trieda.client.mvp.view.DemandasView;
import com.gapso.web.trieda.client.mvp.view.DisciplinasAssociarSalaView;
import com.gapso.web.trieda.client.mvp.view.DisciplinasView;
import com.gapso.web.trieda.client.mvp.view.DivisoesCreditosView;
import com.gapso.web.trieda.client.mvp.view.EquivalenciasView;
import com.gapso.web.trieda.client.mvp.view.FixacoesView;
import com.gapso.web.trieda.client.mvp.view.GruposSalasView;
import com.gapso.web.trieda.client.mvp.view.HorariosAulaView;
import com.gapso.web.trieda.client.mvp.view.IncompatibilidadesView;
import com.gapso.web.trieda.client.mvp.view.OfertasView;
import com.gapso.web.trieda.client.mvp.view.ParametrosView;
import com.gapso.web.trieda.client.mvp.view.ProfessoresDisciplinaView;
import com.gapso.web.trieda.client.mvp.view.ProfessoresView;
import com.gapso.web.trieda.client.mvp.view.RelatorioVisaoCursoView;
import com.gapso.web.trieda.client.mvp.view.RelatorioVisaoSalaView;
import com.gapso.web.trieda.client.mvp.view.ResumoCampiView;
import com.gapso.web.trieda.client.mvp.view.ResumoCenarioView;
import com.gapso.web.trieda.client.mvp.view.SalasView;
import com.gapso.web.trieda.client.mvp.view.SemanasLetivaView;
import com.gapso.web.trieda.client.mvp.view.TiposCursosView;
import com.gapso.web.trieda.client.mvp.view.TurnosView;
import com.gapso.web.trieda.client.mvp.view.UnidadesDeslocamentoView;
import com.gapso.web.trieda.client.mvp.view.UnidadesView;
import com.gapso.web.trieda.client.mvp.view.VincularAreasTitulacaoView;
import com.gapso.web.trieda.client.services.CampiServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.CenarioPanel;
import com.gapso.web.trieda.client.util.view.GTab;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class ToolBarPresenter implements Presenter {

	public interface Display {
		Component getComponent();
		
//		MenuItem getCenariosListMenuItem();
		
		Button getCampiListCampiButton();
		Button getCampusDeslocamentoListCampiButton();
		Button getSemanasLetivaListCampiButton();
		Button getHorariosAulaListCampiButton();
		Button getTurnosListCampiButton();
		Button getOfertasListCampiButton();
		
		Button getUnidadesListUnidadesButton();
		Button getUnidadeDeslocamentoListUnidadesButton();
		
		Button getSalasListSalasButton();
		Button getGruposSalasListSalasButton();
		Button getAssociarDisciplinasSalasListSalasButton();
		
		Button getCurriculosListCursosButton();
		Button getCursosListCursosButton();
		Button getAreasTitulacaoListCursosButton();
		Button getVincularAreasTitulacaoListCursosButton();
		Button getTiposCursosListCursosButton();
		Button getOfertasListCursosButton();
		
		Button getDisciplinasListDisciplinasButton();
		Button getDemandasDisciplinasButton();
		Button getAssociarDisciplinasSalasListDisciplinasButton();
		Button getCurriculosListDisciplinasButton();
		Button getDivisaoCreditosListDisciplinasButton();
		Button getEquivalenciasListDisciplinasButton();
		Button getIncompatibilidadesListDisciplinasButton();
		
		Button getProfessoresListProfessoresButton();
		Button getProfessoresDisciplinaListProfessoresButton();
		Button getProfessoresCampusListprofessoresBt();
		
		Button getRelatorioVisaoSalaButton();
		Button getRelatorioVisaoCursoButton();
		Button getResumoCenarioButton();
		Button getResumoCampiButton();
		
		Button getFixacoesListButton();
		Button getParametrosButton();



	}
	
	private CenarioDTO masterData;
	private Display toolBar;
	private GTab gTab;
	
	public ToolBarPresenter(CenarioDTO masterData, CenarioPanel cenarioPanel, Display toolBar) {
		this.masterData = masterData;
		this.toolBar = toolBar;
		addListeners();
	}

	private void addListeners() {
//		toolBar.getCenariosListMenuItem().addSelectionListener(new SelectionListener<MenuEvent>() {
//			@Override
//			public void componentSelected(MenuEvent ce) {
//				Presenter presenter = new CenariosPresenter(cenarioPanel, new CenariosView());
//				presenter.go(gTab);
//			}
//		});
		toolBar.getTurnosListCampiButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new TurnosPresenter(masterData, new TurnosView());
				presenter.go(gTab);
			}
		});
		toolBar.getSemanasLetivaListCampiButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new SemanasLetivaPresenter(new SemanasLetivaView());
				presenter.go(gTab);
			}
		});
		toolBar.getHorariosAulaListCampiButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new HorariosAulaPresenter(new HorariosAulaView());
				presenter.go(gTab);
			}
		});
		toolBar.getCampiListCampiButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new CampiPresenter(masterData, new CampiView());
				presenter.go(gTab);
			}
		});
		toolBar.getUnidadesListUnidadesButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new UnidadesPresenter(masterData, new UnidadesView());
				presenter.go(gTab);
			}
		});
		toolBar.getSalasListSalasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new SalasPresenter(new SalasView());
				presenter.go(gTab);
			}
		});
		toolBar.getGruposSalasListSalasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new GruposSalasPresenter(new GruposSalasView());
				presenter.go(gTab);
			}
		});
		toolBar.getTiposCursosListCursosButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new TiposCursosPresenter(new TiposCursosView());
				presenter.go(gTab);
			}
		});
		toolBar.getAreasTitulacaoListCursosButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new AreasTitulacaoPresenter(new AreasTitulacaoView());
				presenter.go(gTab);
			}
		});
		toolBar.getVincularAreasTitulacaoListCursosButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new VincularAreasTitulacaoPresenter(new VincularAreasTitulacaoView());
				presenter.go(gTab);
			}
		});
		toolBar.getUnidadeDeslocamentoListUnidadesButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new UnidadesDeslocamentoPresenter(new UnidadesDeslocamentoView(null, null));
				presenter.go(gTab);
			}
		});
		toolBar.getCampusDeslocamentoListCampiButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				// TODO ESTE CODIGO CODIGO NÂO PERTENCE AQUI, DEVE FICAR NO CAMPI DESLOCAMENTO
				// QUANDO EU COLOCO LA, ELE BUGA O HEADER DA TABELA
				CampiServiceAsync service = Services.campi();
				service.getDeslocamentos(new AsyncCallback<List<DeslocamentoCampusDTO>>() {
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
		toolBar.getCursosListCursosButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new CursosPresenter(masterData, new CursosView());
				presenter.go(gTab);
			}
		});
		toolBar.getDisciplinasListDisciplinasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new DisciplinasPresenter(masterData, new DisciplinasView());
				presenter.go(gTab);
			}
		});
		toolBar.getCurriculosListCursosButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new CurriculosPresenter(masterData, new CurriculosView());
				presenter.go(gTab);
			}
		});
		toolBar.getCurriculosListDisciplinasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new CurriculosPresenter(masterData, new CurriculosView());
				presenter.go(gTab);
			}
		});
		toolBar.getDivisaoCreditosListDisciplinasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new DivisoesCreditosPresenter(masterData, new DivisoesCreditosView());
				presenter.go(gTab);
			}
		});
		toolBar.getEquivalenciasListDisciplinasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new EquivalenciasPresenter(masterData, new EquivalenciasView());
				presenter.go(gTab);
			}
		});
		toolBar.getFixacoesListButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new FixacoesPresenter(masterData, new FixacoesView());
				presenter.go(gTab);
			}
		});
		toolBar.getIncompatibilidadesListDisciplinasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new IncompatibilidadesPresenter(masterData, new IncompatibilidadesView());
				presenter.go(gTab);
			}
		});
		toolBar.getProfessoresListProfessoresButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new ProfessoresPresenter(masterData, new ProfessoresView());
				presenter.go(gTab);
			}
		});
		toolBar.getProfessoresDisciplinaListProfessoresButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new ProfessoresDisciplinaPresenter(masterData, new ProfessoresDisciplinaView());
				presenter.go(gTab);
			}
		});
		toolBar.getProfessoresCampusListprofessoresBt().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new CampusProfessoresPresenter(masterData, new CampusProfessoresView());
				presenter.go(gTab);
			}
		});
		toolBar.getOfertasListCampiButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new OfertasPresenter(new OfertasView());
				presenter.go(gTab);
			}
		});
		toolBar.getOfertasListCursosButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new OfertasPresenter(new OfertasView());
				presenter.go(gTab);
			}
		});
		toolBar.getAssociarDisciplinasSalasListSalasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new DisciplinasAssociarSalaPresenter(new DisciplinasAssociarSalaView());
				presenter.go(gTab);
			}
		});
		toolBar.getAssociarDisciplinasSalasListDisciplinasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new DisciplinasAssociarSalaPresenter(new DisciplinasAssociarSalaView());
				presenter.go(gTab);
			}
		});
		toolBar.getDemandasDisciplinasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new DemandasPresenter(masterData, new DemandasView());
				presenter.go(gTab);
			}
		});
		toolBar.getRelatorioVisaoSalaButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new RelatorioVisaoSalaPresenter(masterData, new RelatorioVisaoSalaView());
				presenter.go(gTab);
			}
		});
		toolBar.getParametrosButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new ParametrosPresenter(masterData, new ParametrosView());
				presenter.go(gTab);
			}
		});
		toolBar.getRelatorioVisaoCursoButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new RelatorioVisaoCursoPresenter(masterData, new RelatorioVisaoCursoView());
				presenter.go(gTab);
			}
		});
		toolBar.getResumoCenarioButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new ResumoCenarioPresenter(masterData, new ResumoCenarioView(masterData));
				presenter.go(gTab);
			}
		});
		toolBar.getResumoCampiButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new ResumoCampiPresenter(masterData, new ResumoCampiView(masterData));
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
