package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.ComponentEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.TabItem;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.extjs.gxt.ui.client.widget.TabPanel;
import com.gapso.web.trieda.main.client.command.util.CommandFactory;
import com.gapso.web.trieda.main.client.command.util.CommandSelectionListener;
import com.gapso.web.trieda.main.client.mvp.view.AlocacaoManualDisciplinaFormView;
import com.gapso.web.trieda.main.client.mvp.view.AlterarSenhaFormView;
import com.gapso.web.trieda.main.client.mvp.view.AlunosDisciplinasCursadasView;
import com.gapso.web.trieda.main.client.mvp.view.AlunosFormView;
import com.gapso.web.trieda.main.client.mvp.view.AlunosView;
import com.gapso.web.trieda.main.client.mvp.view.AmbientesFaixaUtilizacaoHorariosView;
import com.gapso.web.trieda.main.client.mvp.view.AmbientesFaixaOcupacaoCapacidadeView;
import com.gapso.web.trieda.main.client.mvp.view.AreasTitulacaoView;
import com.gapso.web.trieda.main.client.mvp.view.AtendimentosFaixaCreditoView;
import com.gapso.web.trieda.main.client.mvp.view.AtendimentosFaixaDisciplinaView;
import com.gapso.web.trieda.main.client.mvp.view.CampiDeslocamentoView;
import com.gapso.web.trieda.main.client.mvp.view.CampiView;
import com.gapso.web.trieda.main.client.mvp.view.CampusFormView;
import com.gapso.web.trieda.main.client.mvp.view.CarregarSolucaoView;
import com.gapso.web.trieda.main.client.mvp.view.CenariosView;
import com.gapso.web.trieda.main.client.mvp.view.CompararCenariosFormView;
import com.gapso.web.trieda.main.client.mvp.view.CompatibilidadesView;
import com.gapso.web.trieda.main.client.mvp.view.ConfiguracoesView;
import com.gapso.web.trieda.main.client.mvp.view.ConfirmacaoTurmasView;
import com.gapso.web.trieda.main.client.mvp.view.CurriculoFormView;
import com.gapso.web.trieda.main.client.mvp.view.CurriculosView;
import com.gapso.web.trieda.main.client.mvp.view.CursoFormView;
import com.gapso.web.trieda.main.client.mvp.view.CursosView;
import com.gapso.web.trieda.main.client.mvp.view.DemandasPorAlunoView;
import com.gapso.web.trieda.main.client.mvp.view.DemandasView;
import com.gapso.web.trieda.main.client.mvp.view.DisciplinaFormView;
import com.gapso.web.trieda.main.client.mvp.view.DisciplinasCoRequisitosView;
import com.gapso.web.trieda.main.client.mvp.view.DisciplinasPreRequisitosView;
import com.gapso.web.trieda.main.client.mvp.view.DisciplinasView;
import com.gapso.web.trieda.main.client.mvp.view.DivisoesCreditosView;
import com.gapso.web.trieda.main.client.mvp.view.EquivalenciasView;
import com.gapso.web.trieda.main.client.mvp.view.ExportExcelFormView;
import com.gapso.web.trieda.main.client.mvp.view.FixacoesView;
import com.gapso.web.trieda.main.client.mvp.view.GruposSalasAssociarDisciplinaView;
import com.gapso.web.trieda.main.client.mvp.view.GruposSalasView;
import com.gapso.web.trieda.main.client.mvp.view.OfertasView;
import com.gapso.web.trieda.main.client.mvp.view.ParametrosGeracaoDemandaView;
import com.gapso.web.trieda.main.client.mvp.view.ParametrosView;
import com.gapso.web.trieda.main.client.mvp.view.ProfessorFormView;
import com.gapso.web.trieda.main.client.mvp.view.ProfessoresAreasConhecimentoView;
import com.gapso.web.trieda.main.client.mvp.view.ProfessoresDisciplinasHabilitadasView;
import com.gapso.web.trieda.main.client.mvp.view.ProfessoresDisciplinasLecionadasView;
import com.gapso.web.trieda.main.client.mvp.view.ProfessoresJanelasGradeView;
import com.gapso.web.trieda.main.client.mvp.view.ProfessoresTitulacoesView;
import com.gapso.web.trieda.main.client.mvp.view.ProfessoresView;
import com.gapso.web.trieda.main.client.mvp.view.ProfessoresVirtuaisView;
import com.gapso.web.trieda.main.client.mvp.view.RelatorioAlunosView;
import com.gapso.web.trieda.main.client.mvp.view.RelatorioProfessoresView;
import com.gapso.web.trieda.main.client.mvp.view.RelatorioSalasView;
import com.gapso.web.trieda.main.client.mvp.view.RelatorioVisaoAlunoView;
import com.gapso.web.trieda.main.client.mvp.view.RelatorioVisaoCursoView;
import com.gapso.web.trieda.main.client.mvp.view.RelatorioVisaoSalaView;
import com.gapso.web.trieda.main.client.mvp.view.ResumoAtendimentosDisciplinaView;
import com.gapso.web.trieda.main.client.mvp.view.ResumoAtendimentosFaixaDemandaView;
import com.gapso.web.trieda.main.client.mvp.view.ResumoCampiView;
import com.gapso.web.trieda.main.client.mvp.view.ResumoCenarioView;
import com.gapso.web.trieda.main.client.mvp.view.ResumoCursosView;
import com.gapso.web.trieda.main.client.mvp.view.ResumoDisciplinaView;
import com.gapso.web.trieda.main.client.mvp.view.ResumoMatriculasView;
import com.gapso.web.trieda.main.client.mvp.view.ResumoPercentMestresDoutoresView;
import com.gapso.web.trieda.main.client.mvp.view.SalaFormView;
import com.gapso.web.trieda.main.client.mvp.view.SalasAssociarDisciplinaView;
import com.gapso.web.trieda.main.client.mvp.view.SalasView;
import com.gapso.web.trieda.main.client.mvp.view.SemanasLetivaView;
import com.gapso.web.trieda.main.client.mvp.view.TiposCursosView;
import com.gapso.web.trieda.main.client.mvp.view.TurnosView;
import com.gapso.web.trieda.main.client.mvp.view.UnidadeFormView;
import com.gapso.web.trieda.main.client.mvp.view.UnidadesDeslocamentoView;
import com.gapso.web.trieda.main.client.mvp.view.UnidadesView;
import com.gapso.web.trieda.main.client.mvp.view.UsuariosView;
import com.gapso.web.trieda.main.client.mvp.view.VincularAreasTitulacaoView;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoCampusDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ParametroConfiguracaoDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.dtos.ParametroGeracaoDemandaDTO;
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
import com.gapso.web.trieda.shared.services.CenariosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.gapso.web.trieda.shared.util.view.CenarioPanel;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class ToolBarPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Component getComponent();
		TabItem getNomeContextoTabItem();
		Button getCarregarMasterDataButton();
		MenuItem getGerenciarCenariosButton();
		MenuItem getCompararCenariosButton();
		MenuItem getGerenciarRequisicoesCenariosButton();
		Button getUsuariosButton();
		MenuItem getListarUsuariosButton();
		MenuItem getConfiguracoesButton();
		MenuItem getUsuariosAlterarSenhaButton();
		MenuItem getUsuariosSairButton();
		MenuItem getUsuariosNomeButton();
		ContentPanel getMasterDataPanel();
		
		Button getImportarButton();
		Button getImportarOfertasButton();
		Button getCarregarSolucaoButton();
		
		Button getCampiNovoCampiButton();
		Button getCampiListCampiButton();
		Button getCampusDeslocamentoListCampiButton();

		Button getUnidadesNovoUnidadesButton();
		Button getUnidadesListUnidadesButton();
		Button getUnidadeDeslocamentoListUnidadesButton();

		Button getSalasNovoSalasButton();
		Button getSalasListSalasButton();
		Button getGruposSalasListSalasButton();
		Button getAssociarDisciplinasSalasListSalasButton();
		Button getAssociarDisciplinasGruposSalasListSalasButton();

		Button getCursosNovoCursosButton();
		Button getCursosListCursosButton();
		Button getAreasTitulacaoListCursosButton();
		Button getVincularAreasTitulacaoListCursosButton();
		Button getTiposCursosListCursosButton();
		
		Button getMatrizesNovoMatrizesButton();
		Button getMatrizesListMatrizesButton();
		Button getCurriculosDisciplinasPreRequisitosMatrizesButton();
		Button getCurriculosDisciplinasCoRequisitosMatrizesButton();

		Button getDisciplinasNovoDisciplinasButton();
		Button getDisciplinasListDisciplinasButton();
		Button getAssociarDisciplinasSalasListDisciplinasButton();
		Button getAssociarDisciplinasGruposSalasListDisciplinasButton();
		Button getDivisaoCreditosListDisciplinasButton();
		Button getEquivalenciasListDisciplinasButton();
		Button getCompatibilidadesListDisciplinasButton();

		Button getAlunosNovoAlunoButton();
		Button getAlunosListAlunosButton();
		Button getAlunosDisciplinasCursadasButton();

		Button getProfessoresNovoProfessoresButton();
		Button getProfessoresListProfessoresButton();
		Button getProfessoresDisciplinaListProfessoresButton();
		Button getProfessoresCampusListprofessoresBt();
		Button getProfessoresListProfessoresVirtuaisBt();

		MenuItem getRelatorioVisaoSalaMenuItem();
		MenuItem getRelatorioVisaoCursoMenuItem();
		MenuItem getRelatorioVisaoProfessorMenuItem();
		MenuItem getRelatorioVisaoAlunoMenuItem();
		MenuItem getResumoCenarioMenuItem();
		MenuItem getResumoCampiMenuItem();
		MenuItem getResumoCursosMenuItem();
		MenuItem getResumoDisciplinasMenuItem();
		MenuItem getRelatorioAlunosMenuItem();
		MenuItem getRelatorioProfessoresMenuItem();
		MenuItem getRelatorioSalasMenuItem();
		MenuItem getResumoMatriculasMenuItem();
		MenuItem getResumoAtendimentosDisciplinaMenuItem();
		//MenuItem getResumoAtendimentosFaixaTurmaMenuItem();
		MenuItem getResumoAtendimentosFaixaDemandaMenuItem();
		MenuItem getAtendimentosFaixaCreditoMenuItem();
		MenuItem getAtendimentosFaixaDisciplinaMenuItem();
		MenuItem getAmbientesFaixaOcupacaoHorariosMenuItem();
		MenuItem getAmbientesFaixaUtilizacaoCapacidadeMenuItem();
		MenuItem getResumoPercentMestresDoutoresMenuItem();
		MenuItem getProfessoresJanelasGradeMenuItem();
		MenuItem getProfessoresDisciplinasHabilitadasMenuItem();
		MenuItem getProfessoresDisciplinasLecionadasMenuItem();
		MenuItem getProfessoresTitulacoesMenuItem();
		MenuItem getProfessoresAreasConhecimentoMenuItem();

		Button getFixacoesListButton();
		Button getParametrosButton();
		Button getConsultaRequisicoesOtimizacaoBt();
		Button getConfirmacaoTurmasButton();
		Button getAlocacaoManualButton();

		Button getExportarButton();
		Button getExportarGradesERPButton();
		Button getExportarReqButton();
		
		Button getCurriculosListDemandasButton();
		Button getCurriculosDisciplinasPreRequisitosDemandasButton();
		Button getCurriculosDisciplinasCoRequisitosDemandasButton();
		Button getAlunosDisciplinasCursadasDemandasButton();
		Button getParametrosGeracaoDemandaButton();
		Button getDemandasDemandasButton();
		Button getDemandasPorAlunoDemandasButton();
		Button getOfertasListDemandasButton();

		Button getSemanasLetivaListCampiButton();
		Button getTurnosListCampiButton();
		
		TabItem getOfertasDemandasTabItem();
		TabItem getRelatoriosTabItem();
		TabItem getPlanejamentoTabItem();
		TabItem getCalendarioTabItem();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenarioDTO;
	private UsuarioDTO usuarioDTO;
	private Display toolBar;
	private GTab gTab;
	private ContentPanel panel;

	public ToolBarPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO masterData, UsuarioDTO usuario,
		CenarioPanel cenarioPanel, ContentPanel mainPanel, Display toolBar )
	{
		this.cenarioDTO = masterData;
		this.usuarioDTO = usuario;
		this.toolBar = toolBar;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.panel = mainPanel;

		addListeners();
	}

	private void addListeners()
	{
		this.toolBar.getAlunosNovoAlunoButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new AlunosFormPresenter( instituicaoEnsinoDTO,
					cenarioDTO, new AlunosFormView( cenarioDTO, new AlunoDTO() ) );

				presenter.go( null );
			}
		});

		this.toolBar.getAlunosListAlunosButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new AlunosPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new AlunosView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getAlunosDisciplinasCursadasButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					Presenter presenter = new AlunosDisciplinasCursadasPresenter(
						instituicaoEnsinoDTO, cenarioDTO, new AlunosDisciplinasCursadasView(cenarioDTO) );

					presenter.go( gTab );
				}
			});

		this.toolBar.getTurnosListCampiButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new TurnosPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new TurnosView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getSemanasLetivaListCampiButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new SemanasLetivaPresenter(
					instituicaoEnsinoDTO,  cenarioDTO, new SemanasLetivaView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getCampiNovoCampiButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new CampusFormPresenter( instituicaoEnsinoDTO,
					cenarioDTO, new CampusFormView( cenarioDTO ) );

				presenter.go( null );
			}
		});

		this.toolBar.getCampiListCampiButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new CampiPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new CampiView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getUnidadesNovoUnidadesButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new UnidadeFormPresenter(
					instituicaoEnsinoDTO, new UnidadeFormView( cenarioDTO ) );

				presenter.go( null );
			}
		});

		this.toolBar.getUnidadesListUnidadesButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new UnidadesPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new UnidadesView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getSalasNovoSalasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new SalaFormPresenter(
					instituicaoEnsinoDTO, new SalaFormView( cenarioDTO ) );

				presenter.go( null );
			}
		});

		this.toolBar.getSalasListSalasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new SalasPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new SalasView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getGruposSalasListSalasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new GruposSalasPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new GruposSalasView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getTiposCursosListCursosButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new TiposCursosPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new TiposCursosView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getAreasTitulacaoListCursosButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new AreasTitulacaoPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new AreasTitulacaoView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getVincularAreasTitulacaoListCursosButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new VincularAreasTitulacaoPresenter(
						instituicaoEnsinoDTO, cenarioDTO, new VincularAreasTitulacaoView(cenarioDTO) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getUnidadeDeslocamentoListUnidadesButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new UnidadesDeslocamentoPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new UnidadesDeslocamentoView( cenarioDTO, null, null ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getCampusDeslocamentoListCampiButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				CampiServiceAsync service = Services.campi();

				service.getDeslocamentos( cenarioDTO, new AsyncCallback< List< DeslocamentoCampusDTO > >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!",
							"Não foi possível listar os deslocamentos.", null );
					}

					@Override
					public void onSuccess( List< DeslocamentoCampusDTO > result )
					{
						Presenter presenter = new CampiDeslocamentoPresenter(
							instituicaoEnsinoDTO, cenarioDTO, new CampiDeslocamentoView( cenarioDTO, result ) );

						presenter.go( gTab );	
					}
				});
			}
		});

		this.toolBar.getCursosNovoCursosButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new CursoFormPresenter( instituicaoEnsinoDTO,
					cenarioDTO, new CursoFormView( cenarioDTO ) );

				presenter.go( null );
			}
		});

		this.toolBar.getCursosListCursosButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					Presenter presenter = new CursosPresenter(
						instituicaoEnsinoDTO,  cenarioDTO, new CursosView( cenarioDTO ) );

					presenter.go( gTab );
				}
		});

		this.toolBar.getDisciplinasNovoDisciplinasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new DisciplinaFormPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new DisciplinaFormView( cenarioDTO ) );

				presenter.go( null );
			}
		});

		this.toolBar.getDisciplinasListDisciplinasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new DisciplinasPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new DisciplinasView(cenarioDTO) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getMatrizesNovoMatrizesButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new CurriculoFormPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new CurriculoFormView(new CurriculoDTO(), null, cenarioDTO), null );

				presenter.go( gTab );
			}
		});

		this.toolBar.getMatrizesListMatrizesButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new CurriculosPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new CurriculosView(cenarioDTO) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getCurriculosDisciplinasPreRequisitosMatrizesButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new DisciplinasPreRequisitosPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new DisciplinasPreRequisitosView(cenarioDTO) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getCurriculosDisciplinasCoRequisitosMatrizesButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					Presenter presenter = new DisciplinasCoRequisitosPresenter(
						instituicaoEnsinoDTO, cenarioDTO, new DisciplinasCoRequisitosView(cenarioDTO) );

					presenter.go( gTab );
				}
			});

		this.toolBar.getDivisaoCreditosListDisciplinasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new DivisoesCreditosPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new DivisoesCreditosView( cenarioDTO ) );

				presenter.go(gTab);
			}
		});

		this.toolBar.getEquivalenciasListDisciplinasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new EquivalenciasPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new EquivalenciasView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getFixacoesListButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new FixacoesPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new FixacoesView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getAlocacaoManualButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					Presenter presenter = new AlocacaoManualDisciplinaFormPresenter(instituicaoEnsinoDTO,
							cenarioDTO, new AlocacaoManualDisciplinaFormView( cenarioDTO ), gTab, null );

						presenter.go( gTab );
				}
			});
		
		this.toolBar.getConfirmacaoTurmasButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					Presenter presenter = new ConfirmacaoTurmasPresenter(
						instituicaoEnsinoDTO, cenarioDTO, new ConfirmacaoTurmasView( cenarioDTO ) );

					presenter.go( gTab );
				}
			});

		this.toolBar.getCompatibilidadesListDisciplinasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new CompatibilidadesPresenter(
						cenarioDTO, new CompatibilidadesView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getProfessoresNovoProfessoresButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new ProfessorFormPresenter( instituicaoEnsinoDTO,
					cenarioDTO, new ProfessorFormView( cenarioDTO ) );

				presenter.go( null );
			}
		});

		this.toolBar.getProfessoresListProfessoresButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new ProfessoresPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new ProfessoresView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getProfessoresDisciplinaListProfessoresButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new ProfessoresDisciplinaPresenter(
					instituicaoEnsinoDTO, cenarioDTO, usuarioDTO,
					new ProfessoresDisciplinaView( cenarioDTO, usuarioDTO ), false );

				presenter.go( gTab );
			}
		});

		this.toolBar.getProfessoresCampusListprofessoresBt().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new CampusProfessoresPresenter(
					instituicaoEnsinoDTO, cenarioDTO, usuarioDTO,
					new CampusProfessoresView( cenarioDTO, usuarioDTO ), false );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getProfessoresListProfessoresVirtuaisBt().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					Presenter presenter = new ProfessoresVirtuaisPresenter(
						instituicaoEnsinoDTO, cenarioDTO, null,
						new ProfessoresVirtuaisView( cenarioDTO ) );

					presenter.go( gTab );
				}
			});

		this.toolBar.getOfertasListDemandasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new OfertasPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new OfertasView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getImportarButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			// Fazendo com que importe todos dados do
			// masterdata. Lembrando que uma importação
			// nunca exclui dados, apenas modifica e adiciona

			@Override
			public void componentSelected( ButtonEvent ce )
			{
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.TUDO, instituicaoEnsinoDTO, cenarioDTO );

				ImportExcelFormView importExcelFormView
					= new ImportExcelFormView( parametros, null );

				importExcelFormView.show();
			}
		});

		this.toolBar.getExportarButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			// Fazendo com que exporte todos dados do masterdata.
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new ExportExcelFormPresenter( instituicaoEnsinoDTO, cenarioDTO, new ExportExcelFormView() );

				presenter.go( null );
			}
		});
		
		this.toolBar.getCarregarSolucaoButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				CarregarSolucaoView view = new CarregarSolucaoView(cenarioDTO);
				view.show();
			}
		});

		this.toolBar.getAssociarDisciplinasSalasListSalasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new SalasAssociarDisciplinaPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new SalasAssociarDisciplinaView( cenarioDTO, new DisciplinaDTO() ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getAssociarDisciplinasGruposSalasListSalasButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new GruposSalasAssociarDisciplinaPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new GruposSalasAssociarDisciplinaView( cenarioDTO, new DisciplinaDTO() ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getAssociarDisciplinasSalasListDisciplinasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new SalasAssociarDisciplinaPresenter(
						instituicaoEnsinoDTO, cenarioDTO, new SalasAssociarDisciplinaView( cenarioDTO, new DisciplinaDTO() ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getAssociarDisciplinasGruposSalasListDisciplinasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new GruposSalasAssociarDisciplinaPresenter(
						instituicaoEnsinoDTO, cenarioDTO, new GruposSalasAssociarDisciplinaView( cenarioDTO, new DisciplinaDTO() ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getRelatorioVisaoSalaMenuItem().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new RelatorioVisaoSalaPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new RelatorioVisaoSalaView(cenarioDTO) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getParametrosButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Services.otimizar().getParametrosDaRequisicaoDeOtimizacao(
					cenarioDTO, new AsyncCallback< ParametroDTO >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!",
							"Não foi possível abrir a tela de parâmetros", null );
					}

					@Override
					public void onSuccess(ParametroDTO parametroDTO) {
						ParametrosView view = new ParametrosView(cenarioDTO, parametroDTO);
						Presenter presenter = new ParametrosPresenter(cenarioDTO,view,view);
						presenter.go(gTab);
					}
				});
			}
		});
		
		this.toolBar.getConsultaRequisicoesOtimizacaoBt().addSelectionListener(CommandSelectionListener.<ButtonEvent>create(
			CommandFactory.createConsultarRequisicoesOtimizacaoCommand(true)
		));

		this.toolBar.getRelatorioVisaoCursoMenuItem().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new RelatorioVisaoCursoPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new RelatorioVisaoCursoView(cenarioDTO) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getRelatorioVisaoProfessorMenuItem().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new RelatorioVisaoProfessorPresenter(
					instituicaoEnsinoDTO, cenarioDTO, usuarioDTO,
					new RelatorioVisaoProfessorView( cenarioDTO, usuarioDTO, false ), false );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getRelatorioVisaoAlunoMenuItem().addSelectionListener(
			new SelectionListener<MenuEvent>(){
				@Override
				public void componentSelected(MenuEvent ce){
					Presenter presenter = new RelatorioVisaoAlunoPresenter(
						instituicaoEnsinoDTO, cenarioDTO, new RelatorioVisaoAlunoView(cenarioDTO)
					);

					presenter.go(gTab);
				}
			}
		);

		this.toolBar.getResumoCenarioMenuItem().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new ResumoCenarioPresenter( instituicaoEnsinoDTO,
					cenarioDTO, new ResumoCenarioView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getResumoCampiMenuItem().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new ResumoCampiPresenter( instituicaoEnsinoDTO,
					cenarioDTO, new ResumoCampiView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getResumoCursosMenuItem().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new ResumoCursosPresenter( instituicaoEnsinoDTO,
					cenarioDTO, new ResumoCursosView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getResumoDisciplinasMenuItem().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new ResumoDisciplinaPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new ResumoDisciplinaView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getRelatorioAlunosMenuItem().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new RelatorioAlunosPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new RelatorioAlunosView( instituicaoEnsinoDTO, cenarioDTO, gTab ) );
					presenter.go( gTab );

			}
		});
		
		this.toolBar.getRelatorioProfessoresMenuItem().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new RelatorioProfessoresPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new RelatorioProfessoresView( instituicaoEnsinoDTO, cenarioDTO, gTab ) );
					presenter.go( gTab );

				}
		});
		
		this.toolBar.getRelatorioSalasMenuItem().addSelectionListener(
				new SelectionListener< MenuEvent >()
			{
				@Override
				public void componentSelected( MenuEvent ce )
				{
					Presenter presenter = new RelatorioSalasPresenter( instituicaoEnsinoDTO,
							cenarioDTO, new RelatorioSalasView( instituicaoEnsinoDTO, cenarioDTO, gTab ) );
						presenter.go( gTab );

					}
			});
		
		this.toolBar.getResumoMatriculasMenuItem().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new ResumoMatriculasPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new ResumoMatriculasView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getResumoAtendimentosDisciplinaMenuItem().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new ResumoAtendimentosDisciplinaPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new ResumoAtendimentosDisciplinaView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
/*		this.toolBar.getResumoAtendimentosFaixaTurmaMenuItem().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new AtendimentosFaixaTurmaPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new AtendimentosFaixaTurmaView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});*/

		this.toolBar.getResumoAtendimentosFaixaDemandaMenuItem().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new ResumoAtendimentosFaixaDemandaPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new ResumoAtendimentosFaixaDemandaView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getAtendimentosFaixaCreditoMenuItem().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new AtendimentosFaixaCreditoPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new AtendimentosFaixaCreditoView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getAtendimentosFaixaDisciplinaMenuItem().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new AtendimentosFaixaDisciplinaPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new AtendimentosFaixaDisciplinaView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getAmbientesFaixaOcupacaoHorariosMenuItem().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new AmbientesFaixaUtilizacaoHorariosPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new AmbientesFaixaUtilizacaoHorariosView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getAmbientesFaixaUtilizacaoCapacidadeMenuItem().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new AmbientesFaixaOcupacaoCapacidadePresenter( instituicaoEnsinoDTO,
						cenarioDTO, new AmbientesFaixaOcupacaoCapacidadeView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getResumoPercentMestresDoutoresMenuItem().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new ResumoPercentMestresDoutoresPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new ResumoPercentMestresDoutoresView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getProfessoresJanelasGradeMenuItem().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new ProfessoresJanelasGradePresenter( instituicaoEnsinoDTO,
						cenarioDTO, new ProfessoresJanelasGradeView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getProfessoresDisciplinasHabilitadasMenuItem().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new ProfessoresDisciplinasHabilitadasPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new ProfessoresDisciplinasHabilitadasView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getProfessoresDisciplinasLecionadasMenuItem().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new ProfessoresDisciplinasLecionadasPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new ProfessoresDisciplinasLecionadasView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getProfessoresTitulacoesMenuItem().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new ProfessoresTitulacoesPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new ProfessoresTitulacoesView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getProfessoresAreasConhecimentoMenuItem().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				Presenter presenter = new ProfessoresAreasConhecimentoPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new ProfessoresAreasConhecimentoView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getCurriculosListDemandasButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new CurriculosPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new CurriculosView(cenarioDTO) );

				presenter.go( gTab );
			}
		});
			
		this.toolBar.getCurriculosDisciplinasPreRequisitosDemandasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new DisciplinasPreRequisitosPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new DisciplinasPreRequisitosView(cenarioDTO) );

				presenter.go( gTab );
			}
		});
			
		this.toolBar.getCurriculosDisciplinasCoRequisitosDemandasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new DisciplinasCoRequisitosPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new DisciplinasCoRequisitosView(cenarioDTO) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getAlunosDisciplinasCursadasDemandasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new AlunosDisciplinasCursadasPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new AlunosDisciplinasCursadasView(cenarioDTO) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getParametrosGeracaoDemandaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Services.demandas().getParametroGeracaoDemanda(
						cenarioDTO, new AsyncCallback< ParametroGeracaoDemandaDTO >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!",
								"Não foi possível abrir a tela de parâmetros", null );
						}

						@Override
						public void onSuccess(ParametroGeracaoDemandaDTO parametroGeracaoDemandaDTO) {
							Presenter presenter = new ParametrosGeracaoDemandaPresenter(
									cenarioDTO, instituicaoEnsinoDTO,
									new ParametrosGeracaoDemandaView(cenarioDTO, parametroGeracaoDemandaDTO) );

								presenter.go( gTab );
						}
					});
			}
		});
		
		this.toolBar.getDemandasDemandasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new DemandasPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new DemandasView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getDemandasPorAlunoDemandasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new DemandasPorAlunoPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new DemandasPorAlunoView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getCarregarMasterDataButton().addSelectionListener( new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				CenariosServiceAsync service = Services.cenarios();
				service.getMasterData( new AsyncCallback<CenarioDTO>(){

					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert( "ERRO!",
								"Deu falha na conexão", null );
					}

					@Override
					public void onSuccess(CenarioDTO result) {
						changeCenario(result);
						Services.cenarios().setCurrentCenario(result.getId(), new AsyncCallback<Void>(){

							@Override
							public void onFailure(Throwable caught) {
								MessageBox.alert( "ERRO!",
										"Deu falha na conexão", null );
								
							}

							@Override
							public void onSuccess(Void result) {
								((TabPanel) toolBar.getMasterDataPanel().getTopComponent()).setSelection(toolBar.getCalendarioTabItem());
								MessageBox.alert( "Contexto modificado!",
										"Contexto alterado para Master Data", null );
							}
							
						});
					}
					
				});
			}
		});
		
		this.toolBar.getUsuariosNomeButton().setText(usuarioDTO.getNome());
		this.toolBar.getUsuariosButton().setText(usuarioDTO.getUsername());
		
		this.toolBar.getListarUsuariosButton().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected( MenuEvent ce ) {
			Presenter presenter = new UsuariosPresenter( instituicaoEnsinoDTO,
					cenarioDTO, new UsuariosView( cenarioDTO ) );

			presenter.go( gTab );
			}
		});
		
		this.toolBar.getConfiguracoesButton().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected( MenuEvent ce ) {
				Services.instituicoesEnsino().getConfiguracoes(new AsyncCallback< ParametroConfiguracaoDTO >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!",
							"Não foi possível abrir a tela de parâmetros", null );
					}
	
					@Override
					public void onSuccess(ParametroConfiguracaoDTO parametroConfiguracaoDTO) {
						Presenter presenter = new ConfiguracoesPresenter( instituicaoEnsinoDTO,
								cenarioDTO, new ConfiguracoesView( parametroConfiguracaoDTO ) );
	
						presenter.go( gTab );
					}
				});
			}
		});
		
		this.toolBar.getUsuariosAlterarSenhaButton().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected( MenuEvent ce ) {
			Presenter presenter = new AlterarSenhaFormPresenter( instituicaoEnsinoDTO,
					new AlterarSenhaFormView(usuarioDTO) );

			presenter.go( gTab );
			}
		});
		
		this.toolBar.getUsuariosSairButton().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected( MenuEvent ce ) {
				Window.open("../resources/j_spring_security_logout"+TriedaUtil.paramsDebug(), "_self", "");
			}
		});
		
		this.toolBar.getGerenciarCenariosButton().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected( MenuEvent ce ) {
			Presenter presenter = new CenariosPresenter( instituicaoEnsinoDTO, ToolBarPresenter.this, new CenariosView() );

			presenter.go( gTab );
			}
		});
		
		this.toolBar.getCompararCenariosButton().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected( MenuEvent ce ) {
			Presenter presenter = new CompararCenariosFormPresenter( instituicaoEnsinoDTO, cenarioDTO, new CompararCenariosFormView(), gTab );

			presenter.go( gTab );
			}
		});
		
		this.toolBar.getGerenciarRequisicoesCenariosButton().addSelectionListener(CommandSelectionListener.<MenuEvent>create(
				CommandFactory.createConsultarRequisicoesOtimizacaoCommand(true)
		));
		
		this.toolBar.getMasterDataPanel().addListener(Events.Collapse, new Listener<ComponentEvent>() {
		@Override
			public void handleEvent(ComponentEvent fe) {
				panel.syncSize();
		    }
		});
		
		this.toolBar.getMasterDataPanel().addListener(Events.Expand, new Listener<ComponentEvent>() {
		@Override
			public void handleEvent(ComponentEvent fe) {
				panel.syncSize();
		    }
		});
	}
	
	public void changeCenario( CenarioDTO cenarioDTO )
	{
		this.cenarioDTO = cenarioDTO;
/*		toolBar.getNomeContextoTabItem().removeAll();
		toolBar.getNomeContextoTabItem().addText(cenarioDTO.getNome());
		if (cenarioDTO.getNome().length() > 20)
		{
			toolBar.getNomeContextoTabItem().addStyleName("cenarioNameSmall");
			toolBar.getNomeContextoTabItem().removeStyleName("cenarioName");
		}
		else
		{
			toolBar.getNomeContextoTabItem().addStyleName("cenarioName");
			toolBar.getNomeContextoTabItem().removeStyleName("cenarioNameSmall");
		}
		if (!cenarioDTO.getMasterData())
		{
			toolBar.getNomeContextoTabItem().setStyleAttribute("margin-top", "5px");
			toolBar.getNomeContextoTabItem().add(toolBar.getCarregarMasterDataButton());
		}
		else
		{
			toolBar.getNomeContextoTabItem().setStyleAttribute("margin-top", "14px");
		}*/
		toolBar.getMasterDataPanel().setHeadingHtml("Contexto: " + cenarioDTO.getNome());
		escondeBarrasMasterData( cenarioDTO );
		toolBar.getMasterDataPanel().layout();
	}
	
	private void escondeBarrasMasterData( CenarioDTO cenarioDTO )
	{
		if (cenarioDTO.getMasterData())
		{
			toolBar.getCarregarMasterDataButton().hide();
			toolBar.getImportarOfertasButton().hide();
			toolBar.getExportarGradesERPButton().hide();
			toolBar.getExportarReqButton().hide();
			toolBar.getOfertasDemandasTabItem().getHeader().hide();
			toolBar.getProfessoresListProfessoresVirtuaisBt().hide();
			toolBar.getPlanejamentoTabItem().getHeader().hide();
			toolBar.getRelatoriosTabItem().getHeader().hide();
		}
		else
		{
			toolBar.getCarregarMasterDataButton().show();
			toolBar.getImportarOfertasButton().show();
			toolBar.getExportarGradesERPButton().show();
			toolBar.getExportarReqButton().show();
			toolBar.getOfertasDemandasTabItem().getHeader().show();
			toolBar.getProfessoresListProfessoresVirtuaisBt().show();
			toolBar.getPlanejamentoTabItem().getHeader().show();
			toolBar.getRelatoriosTabItem().getHeader().show();
		}
	}

	@Override
	public void go(Widget widget) {
		AppPresenter.Display container = (AppPresenter.Display)widget;
		this.gTab = container.getGTab();
		container.getPanel().setTopComponent(this.toolBar.getComponent());
		
		CommandFactory.createConsultarRequisicoesOtimizacaoCommand(false).execute(); 
	}
}