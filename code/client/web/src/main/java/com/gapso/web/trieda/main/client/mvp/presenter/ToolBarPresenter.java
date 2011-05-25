package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.main.client.mvp.view.AreasTitulacaoView;
import com.gapso.web.trieda.main.client.mvp.view.CampiDeslocamentoView;
import com.gapso.web.trieda.main.client.mvp.view.CampiView;
import com.gapso.web.trieda.main.client.mvp.view.CampusFormView;
import com.gapso.web.trieda.main.client.mvp.view.CompatibilidadesView;
import com.gapso.web.trieda.main.client.mvp.view.CurriculosView;
import com.gapso.web.trieda.main.client.mvp.view.CursoFormView;
import com.gapso.web.trieda.main.client.mvp.view.CursosView;
import com.gapso.web.trieda.main.client.mvp.view.DemandasView;
import com.gapso.web.trieda.main.client.mvp.view.DisciplinaFormView;
import com.gapso.web.trieda.main.client.mvp.view.DisciplinasAssociarSalaView;
import com.gapso.web.trieda.main.client.mvp.view.DisciplinasView;
import com.gapso.web.trieda.main.client.mvp.view.DivisoesCreditosView;
import com.gapso.web.trieda.main.client.mvp.view.EquivalenciasView;
import com.gapso.web.trieda.main.client.mvp.view.FixacoesView;
import com.gapso.web.trieda.main.client.mvp.view.GruposSalasView;
import com.gapso.web.trieda.main.client.mvp.view.HorariosAulaView;
import com.gapso.web.trieda.main.client.mvp.view.OfertasView;
import com.gapso.web.trieda.main.client.mvp.view.ParametrosView;
import com.gapso.web.trieda.main.client.mvp.view.ProfessorFormView;
import com.gapso.web.trieda.main.client.mvp.view.ProfessoresView;
import com.gapso.web.trieda.main.client.mvp.view.RelatorioVisaoCursoView;
import com.gapso.web.trieda.main.client.mvp.view.RelatorioVisaoSalaView;
import com.gapso.web.trieda.main.client.mvp.view.ResumoCampiView;
import com.gapso.web.trieda.main.client.mvp.view.ResumoCenarioView;
import com.gapso.web.trieda.main.client.mvp.view.ResumoCursosView;
import com.gapso.web.trieda.main.client.mvp.view.ResumoDisciplinaView;
import com.gapso.web.trieda.main.client.mvp.view.SalaFormView;
import com.gapso.web.trieda.main.client.mvp.view.SalasView;
import com.gapso.web.trieda.main.client.mvp.view.SemanasLetivaView;
import com.gapso.web.trieda.main.client.mvp.view.TiposCursosView;
import com.gapso.web.trieda.main.client.mvp.view.TurnosView;
import com.gapso.web.trieda.main.client.mvp.view.UnidadeFormView;
import com.gapso.web.trieda.main.client.mvp.view.UnidadesDeslocamentoView;
import com.gapso.web.trieda.main.client.mvp.view.UnidadesView;
import com.gapso.web.trieda.main.client.mvp.view.UsuariosView;
import com.gapso.web.trieda.main.client.mvp.view.VincularAreasTitulacaoView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoCampusDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.CampusProfessoresPresenter;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.mvp.presenter.ProfessoresDisciplinaPresenter;
import com.gapso.web.trieda.shared.mvp.presenter.RelatorioVisaoProfessorPresenter;
import com.gapso.web.trieda.shared.mvp.view.CampusProfessoresView;
import com.gapso.web.trieda.shared.mvp.view.ProfessoresDisciplinaView;
import com.gapso.web.trieda.shared.mvp.view.RelatorioVisaoProfessorView;
import com.gapso.web.trieda.shared.services.CampiServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.CenarioPanel;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class ToolBarPresenter implements Presenter {

	public interface Display extends ITriedaI18nGateway {
		Component getComponent();
		
//		MenuItem getCenariosListMenuItem();
		
		Button getCampiNovoCampiButton();
		Button getCampiListCampiButton();
		Button getCampusDeslocamentoListCampiButton();
		Button getSemanasLetivaListCampiButton();
		Button getHorariosAulaListCampiButton();
		Button getTurnosListCampiButton();
		Button getOfertasListCampiButton();
		
		Button getUnidadesNovoUnidadesButton();
		Button getUnidadesListUnidadesButton();
		Button getUnidadeDeslocamentoListUnidadesButton();
		
		Button getSalasNovoSalasButton();
		Button getSalasListSalasButton();
		Button getGruposSalasListSalasButton();
		Button getAssociarDisciplinasSalasListSalasButton();
		
		Button getCursosNovoCursosButton();
		Button getCurriculosListCursosButton();
		Button getCursosListCursosButton();
		Button getAreasTitulacaoListCursosButton();
		Button getVincularAreasTitulacaoListCursosButton();
		Button getTiposCursosListCursosButton();
		Button getOfertasListCursosButton();
		
		Button getDisciplinasNovoDisciplinasButton();
		Button getDisciplinasListDisciplinasButton();
		Button getDemandasDisciplinasButton();
		Button getAssociarDisciplinasSalasListDisciplinasButton();
		Button getCurriculosListDisciplinasButton();
		Button getDivisaoCreditosListDisciplinasButton();
		Button getEquivalenciasListDisciplinasButton();
		Button getCompatibilidadesListDisciplinasButton();
		
		Button getProfessoresNovoProfessoresButton();
		Button getProfessoresListProfessoresButton();
		Button getProfessoresDisciplinaListProfessoresButton();
		Button getProfessoresCampusListprofessoresBt();
		
		Button getRelatorioVisaoSalaButton();
		Button getRelatorioVisaoCursoButton();
		Button getRelatorioVisaoProfessorButton();
		Button getResumoCenarioButton();
		Button getResumoCampiButton();
		Button getResumoCursosButton();
		Button getResumoDisciplinasButton();
		
		Button getFixacoesListButton();
		Button getParametrosButton();

		Button getUsuariosListButton();
		Button getImportarButton();
		Button getExportarButton();
	}
	
	private CenarioDTO masterData;
	private UsuarioDTO usuario;
	private Display toolBar;
	private GTab gTab;
	
	public ToolBarPresenter(CenarioDTO masterData, UsuarioDTO usuario, CenarioPanel cenarioPanel, Display toolBar) {
		this.masterData = masterData;
		this.usuario = usuario;
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
				Presenter presenter = new SemanasLetivaPresenter(masterData, new SemanasLetivaView());
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
		toolBar.getCampiNovoCampiButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new CampusFormPresenter(masterData, new CampusFormView(masterData));
				presenter.go(null);
			}
		});
		toolBar.getCampiListCampiButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new CampiPresenter(masterData, new CampiView());
				presenter.go(gTab);
			}
		});
		toolBar.getUnidadesNovoUnidadesButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new UnidadeFormPresenter(new UnidadeFormView(masterData));
				presenter.go(null);
			}
		});
		toolBar.getUnidadesListUnidadesButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new UnidadesPresenter(masterData, new UnidadesView());
				presenter.go(gTab);
			}
		});
		toolBar.getSalasNovoSalasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new SalaFormPresenter(new SalaFormView(masterData));
				presenter.go(null);
			}
		});
		toolBar.getSalasListSalasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new SalasPresenter(masterData, new SalasView());
				presenter.go(gTab);
			}
		});
		toolBar.getGruposSalasListSalasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new GruposSalasPresenter(masterData, new GruposSalasView());
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
						Presenter presenter = new CampiDeslocamentoPresenter(masterData, new CampiDeslocamentoView(result));
						presenter.go(gTab);	
					}
				});
			}
		});
		toolBar.getCursosNovoCursosButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new CursoFormPresenter(masterData, new CursoFormView(masterData));
				presenter.go(null);
			}
		});
		toolBar.getCursosListCursosButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new CursosPresenter(masterData, new CursosView());
				presenter.go(gTab);
			}
		});
		toolBar.getDisciplinasNovoDisciplinasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new DisciplinaFormPresenter(masterData, new DisciplinaFormView(masterData));
				presenter.go(null);
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
		toolBar.getCompatibilidadesListDisciplinasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new CompatibilidadesPresenter(masterData, new CompatibilidadesView());
				presenter.go(gTab);
			}
		});
		toolBar.getProfessoresNovoProfessoresButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new ProfessorFormPresenter(masterData, new ProfessorFormView(masterData));
				presenter.go(null);
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
				Presenter presenter = new ProfessoresDisciplinaPresenter(masterData, usuario, new ProfessoresDisciplinaView(usuario));
				presenter.go(gTab);
			}
		});
		toolBar.getProfessoresCampusListprofessoresBt().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new CampusProfessoresPresenter(masterData, usuario, new CampusProfessoresView(usuario));
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
		toolBar.getUsuariosListButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new UsuariosPresenter(masterData, new UsuariosView());
				presenter.go(gTab);
			}
		});
		toolBar.getImportarButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				ImportExcelFormView importExcelFormView = new ImportExcelFormView(ExcelInformationType.TUDO, null);
				importExcelFormView.show();
			}
		});
		toolBar.getExportarButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				ExportExcelFormSubmit e = new ExportExcelFormSubmit(ExcelInformationType.TUDO, toolBar.getI18nConstants(),toolBar.getI18nMessages());
				e.submit();
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
				// TODO Temporariamente vai aparecer o modal de selecionar os campi
//				Presenter presenter = new SelecionarCampiPresenter(masterData, new SelecionarCampiView());
//				presenter.go(gTab);
				
				// TODO ESTE CODIGO NÂO PERTENCE AQUI, DEVE FICAR NO PRESENTATION DE PARAMETROS
				Services.otimizar().getParametro(masterData, new AsyncCallback<ParametroDTO>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(ParametroDTO parametroDTO) {
						Presenter presenter = new ParametrosPresenter(masterData, new ParametrosView(parametroDTO));
						presenter.go(gTab);
					}
				});
			}
		});
		toolBar.getRelatorioVisaoCursoButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new RelatorioVisaoCursoPresenter(masterData, new RelatorioVisaoCursoView());
				presenter.go(gTab);
			}
		});
		toolBar.getRelatorioVisaoProfessorButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new RelatorioVisaoProfessorPresenter(masterData, usuario, new RelatorioVisaoProfessorView(usuario));
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
		toolBar.getResumoCursosButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new ResumoCursosPresenter(masterData, new ResumoCursosView(masterData));
				presenter.go(gTab);
			}
		});
		toolBar.getResumoDisciplinasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new ResumoDisciplinaPresenter(masterData, new ResumoDisciplinaView(masterData));
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
