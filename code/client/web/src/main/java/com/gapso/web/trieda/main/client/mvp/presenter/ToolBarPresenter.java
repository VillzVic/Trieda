package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.command.util.CommandFactory;
import com.gapso.web.trieda.main.client.command.util.CommandSelectionListener;
import com.gapso.web.trieda.main.client.mvp.view.AlunosFormView;
import com.gapso.web.trieda.main.client.mvp.view.AlunosView;
import com.gapso.web.trieda.main.client.mvp.view.AreasTitulacaoView;
import com.gapso.web.trieda.main.client.mvp.view.CampiDeslocamentoView;
import com.gapso.web.trieda.main.client.mvp.view.CampiView;
import com.gapso.web.trieda.main.client.mvp.view.CampusFormView;
import com.gapso.web.trieda.main.client.mvp.view.CarregarSolucaoView;
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
import com.gapso.web.trieda.main.client.mvp.view.OfertasView;
import com.gapso.web.trieda.main.client.mvp.view.ParametrosView;
import com.gapso.web.trieda.main.client.mvp.view.ProfessorFormView;
import com.gapso.web.trieda.main.client.mvp.view.ProfessoresView;
import com.gapso.web.trieda.main.client.mvp.view.RelatorioVisaoAlunoView;
import com.gapso.web.trieda.main.client.mvp.view.RelatorioVisaoCursoView;
import com.gapso.web.trieda.main.client.mvp.view.RelatorioVisaoSalaView;
import com.gapso.web.trieda.main.client.mvp.view.ResumoAtendimentosDisciplinaView;
import com.gapso.web.trieda.main.client.mvp.view.ResumoCampiView;
import com.gapso.web.trieda.main.client.mvp.view.ResumoCenarioView;
import com.gapso.web.trieda.main.client.mvp.view.ResumoCursosView;
import com.gapso.web.trieda.main.client.mvp.view.ResumoDisciplinaView;
import com.gapso.web.trieda.main.client.mvp.view.ResumoMatriculasView;
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
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoCampusDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
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
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.CenarioPanel;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class ToolBarPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Component getComponent();

		Button getCampiNovoCampiButton();
		Button getCampiListCampiButton();
		Button getCampusDeslocamentoListCampiButton();
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

		Button getAlunosNovoAlunoButton();
		Button getAlunosListAlunosButton();

		Button getProfessoresNovoProfessoresButton();
		Button getProfessoresListProfessoresButton();
		Button getProfessoresDisciplinaListProfessoresButton();
		Button getProfessoresCampusListprofessoresBt();

		Button getRelatorioVisaoSalaButton();
		Button getRelatorioVisaoCursoButton();
		Button getRelatorioVisaoProfessorButton();
		Button getRelatorioVisaoAlunoButton();
		Button getResumoCenarioButton();
		Button getResumoCampiButton();
		Button getResumoCursosButton();
		Button getResumoDisciplinasButton();
		Button getResumoMatriculasButton();
		Button getResumoAtendimentosDisciplinaButton();

		Button getFixacoesListButton();
		Button getParametrosButton();
		Button getConsultaRequisicoesOtimizacaoBt();

		Button getUsuariosListButton();
		Button getImportarButton();
		MenuItem getExportarXlsButton();
		MenuItem getExportarXlsxButton();
		MenuItem getExportarXlsTabelasButton();
		MenuItem getExportarXlsxTabelasButton();
		MenuItem getExportarXlsGradesHorariasButton();
		MenuItem getExportarXlsxGradesHorariasButton();
		MenuItem getExportarXlsVisaoAlunoButton();
		MenuItem getExportarXlsxVisaoAlunoButton();
		Button getCarregarSolucaoButton();

		Button getSemanasLetivaListCampiButton();
		Button getTurnosListCampiButton();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenarioDTO;
	private UsuarioDTO usuarioDTO;
	private Display toolBar;
	private GTab gTab;

	public ToolBarPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO masterData, UsuarioDTO usuario,
		CenarioPanel cenarioPanel, Display toolBar )
	{
		this.cenarioDTO = masterData;
		this.usuarioDTO = usuario;
		this.toolBar = toolBar;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;

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
					instituicaoEnsinoDTO, cenarioDTO, new AlunosView() );

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
					instituicaoEnsinoDTO, cenarioDTO, new TurnosView() );

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
					instituicaoEnsinoDTO,  cenarioDTO, new SemanasLetivaView() );

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
					instituicaoEnsinoDTO, cenarioDTO, new CampiView() );

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
					instituicaoEnsinoDTO, cenarioDTO, new UnidadesView() );

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
					instituicaoEnsinoDTO, cenarioDTO, new SalasView() );

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
					instituicaoEnsinoDTO, cenarioDTO, new GruposSalasView() );

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
					instituicaoEnsinoDTO, new TiposCursosView() );

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
					instituicaoEnsinoDTO, new AreasTitulacaoView() );

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
						instituicaoEnsinoDTO, new VincularAreasTitulacaoView() );

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
					new UnidadesDeslocamentoView( null, null ) );

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

				service.getDeslocamentos( new AsyncCallback< List< DeslocamentoCampusDTO > >()
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
							cenarioDTO, new CampiDeslocamentoView( result ) );

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
						instituicaoEnsinoDTO,  cenarioDTO, new CursosView() );

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
					instituicaoEnsinoDTO, cenarioDTO, new DisciplinasView() );

				presenter.go( gTab );
			}
		});

		this.toolBar.getCurriculosListCursosButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new CurriculosPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new CurriculosView() );

				presenter.go( gTab );
			}
		});

		this.toolBar.getCurriculosListDisciplinasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new CurriculosPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new CurriculosView() );

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
					instituicaoEnsinoDTO, cenarioDTO, new DivisoesCreditosView() );

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
					instituicaoEnsinoDTO, cenarioDTO, new EquivalenciasView() );

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
					instituicaoEnsinoDTO, cenarioDTO, new FixacoesView() );

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
						cenarioDTO, new CompatibilidadesView() );

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
					instituicaoEnsinoDTO, cenarioDTO, new ProfessoresView() );

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
					new ProfessoresDisciplinaView( usuarioDTO ), false );

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
					new CampusProfessoresView( usuarioDTO ), false );

				presenter.go( gTab );
			}
		});

		this.toolBar.getOfertasListCampiButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new OfertasPresenter(
					instituicaoEnsinoDTO, new OfertasView() );

				presenter.go( gTab );
			}
		});

		this.toolBar.getUsuariosListButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new UsuariosPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new UsuariosView() );

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
					ExcelInformationType.TUDO, instituicaoEnsinoDTO );

				ImportExcelFormView importExcelFormView
					= new ImportExcelFormView( parametros, null );

				importExcelFormView.show();
			}
		});

		this.toolBar.getExportarXlsButton().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			// Fazendo com que exporte todos dados do masterdata.
			@Override
			public void componentSelected( MenuEvent ce )
			{
				String fileExtension = "xls";
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.TUDO, instituicaoEnsinoDTO, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, toolBar.getI18nConstants(), toolBar.getI18nMessages() );

				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
		
		this.toolBar.getExportarXlsxButton().addSelectionListener(
				new SelectionListener< MenuEvent >()
			{
				// Fazendo com que exporte todos dados do masterdata.
				@Override
				public void componentSelected( MenuEvent ce )
				{
					String fileExtension = "xlsx";
					
					ExcelParametros parametros = new ExcelParametros(
						ExcelInformationType.TUDO, instituicaoEnsinoDTO, fileExtension );

					ExportExcelFormSubmit e = new ExportExcelFormSubmit(
						parametros, toolBar.getI18nConstants(), toolBar.getI18nMessages() );

					e.submit();
					new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
				}
			});
		
		this.toolBar.getExportarXlsTabelasButton().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			// Fazendo com que exporte todos dados do masterdata.
			@Override
			public void componentSelected( MenuEvent ce )
			{
				String fileExtension = "xls";
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.TODAS_TABELAS, instituicaoEnsinoDTO, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, toolBar.getI18nConstants(), toolBar.getI18nMessages() );

				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
		
		this.toolBar.getExportarXlsxTabelasButton().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			// Fazendo com que exporte todos dados do masterdata.
			@Override
			public void componentSelected( MenuEvent ce )
			{
				String fileExtension = "xlsx";
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.TODAS_TABELAS, instituicaoEnsinoDTO, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, toolBar.getI18nConstants(), toolBar.getI18nMessages() );

				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
		
		this.toolBar.getExportarXlsGradesHorariasButton().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			// Fazendo com que exporte todos dados do masterdata.
			@Override
			public void componentSelected( MenuEvent ce )
			{
				String fileExtension = "xls";
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.TODAS_GRADES_HORARIAS, instituicaoEnsinoDTO, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, toolBar.getI18nConstants(), toolBar.getI18nMessages() );

				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
		
		this.toolBar.getExportarXlsxGradesHorariasButton().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			// Fazendo com que exporte todos dados do masterdata.
			@Override
			public void componentSelected( MenuEvent ce )
			{
				String fileExtension = "xlsx";
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.TODAS_GRADES_HORARIAS, instituicaoEnsinoDTO, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, toolBar.getI18nConstants(), toolBar.getI18nMessages() );

				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
		
		this.toolBar.getExportarXlsVisaoAlunoButton().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			// Fazendo com que exporte todos dados do masterdata.
			@Override
			public void componentSelected( MenuEvent ce )
			{
				String fileExtension = "xls";
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.TODAS_VISAO_ALUNO, instituicaoEnsinoDTO, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, toolBar.getI18nConstants(), toolBar.getI18nMessages() );

				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
		
		this.toolBar.getExportarXlsxVisaoAlunoButton().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			// Fazendo com que exporte todos dados do masterdata.
			@Override
			public void componentSelected( MenuEvent ce )
			{
				String fileExtension = "xlsx";
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.TODAS_VISAO_ALUNO, instituicaoEnsinoDTO, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, toolBar.getI18nConstants(), toolBar.getI18nMessages() );

				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
		
		this.toolBar.getCarregarSolucaoButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				CarregarSolucaoView view = new CarregarSolucaoView(cenarioDTO);
				view.show();
			}
		});

		this.toolBar.getOfertasListCursosButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new OfertasPresenter(
					instituicaoEnsinoDTO, new OfertasView() );

				presenter.go( gTab );
			}
		});

		this.toolBar.getAssociarDisciplinasSalasListSalasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new DisciplinasAssociarSalaPresenter(
					instituicaoEnsinoDTO, new DisciplinasAssociarSalaView() );

				presenter.go( gTab );
			}
		});

		this.toolBar.getAssociarDisciplinasSalasListDisciplinasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new DisciplinasAssociarSalaPresenter(
					instituicaoEnsinoDTO, new DisciplinasAssociarSalaView() );

				presenter.go( gTab );
			}
		});

		this.toolBar.getDemandasDisciplinasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new DemandasPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new DemandasView() );

				presenter.go( gTab );
			}
		});

		this.toolBar.getRelatorioVisaoSalaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new RelatorioVisaoSalaPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new RelatorioVisaoSalaView() );

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
						ParametrosView view = new ParametrosView(parametroDTO);
						Presenter presenter = new ParametrosPresenter(cenarioDTO,view,view);
						presenter.go(gTab);
					}
				});
			}
		});
		
		this.toolBar.getConsultaRequisicoesOtimizacaoBt().addSelectionListener(CommandSelectionListener.<ButtonEvent>create(
			CommandFactory.createConsultarRequisicoesOtimizacaoCommand(true)
		));

		this.toolBar.getRelatorioVisaoCursoButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new RelatorioVisaoCursoPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new RelatorioVisaoCursoView() );

				presenter.go( gTab );
			}
		});

		this.toolBar.getRelatorioVisaoProfessorButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new RelatorioVisaoProfessorPresenter(
					instituicaoEnsinoDTO, cenarioDTO, usuarioDTO,
					new RelatorioVisaoProfessorView( usuarioDTO, false ), false );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getRelatorioVisaoAlunoButton().addSelectionListener(
			new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce){
					Presenter presenter = new RelatorioVisaoAlunoPresenter(
						instituicaoEnsinoDTO, cenarioDTO, new RelatorioVisaoAlunoView()
					);

					presenter.go(gTab);
				}
			}
		);

		this.toolBar.getResumoCenarioButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new ResumoCenarioPresenter(
					cenarioDTO, new ResumoCenarioView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getResumoCampiButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new ResumoCampiPresenter(
					cenarioDTO, new ResumoCampiView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getResumoCursosButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new ResumoCursosPresenter( instituicaoEnsinoDTO,
					cenarioDTO, new ResumoCursosView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});

		this.toolBar.getResumoDisciplinasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new ResumoDisciplinaPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new ResumoDisciplinaView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getResumoMatriculasButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new ResumoMatriculasPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new ResumoMatriculasView() );

				presenter.go( gTab );
			}
		});
		
		this.toolBar.getResumoAtendimentosDisciplinaButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new ResumoAtendimentosDisciplinaPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new ResumoAtendimentosDisciplinaView() );

				presenter.go( gTab );
			}
		});
	}

	@Override
	public void go(Widget widget) {
		AppPresenter.Display container = (AppPresenter.Display)widget;
		this.gTab = container.getGTab();
		container.getPanel().setTopComponent(this.toolBar.getComponent());
		
		CommandFactory.createConsultarRequisicoesOtimizacaoCommand(false).execute(); 
	}
}