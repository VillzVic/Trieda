package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.GridEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.Record;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.mvp.presenter.ErrorsWarningsNewAulaPresenter.TipoModal;
import com.gapso.web.trieda.main.client.mvp.view.AlocacaoManualDisciplinaFormView;
import com.gapso.web.trieda.main.client.mvp.view.AulaFormView;
import com.gapso.web.trieda.main.client.mvp.view.ErrorsWarningsNewAulaView;
import com.gapso.web.trieda.main.client.mvp.view.NovaTurmaFormView;
import com.gapso.web.trieda.shared.dtos.AlunoStatusDTO;
import com.gapso.web.trieda.shared.dtos.AulaDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorStatusDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SalaStatusDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaStatusDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AtendimentosServiceAsync;
import com.gapso.web.trieda.shared.services.SalasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaSalaGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaVisao.LinhaDeCredito;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoSalaFiltro;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SalaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.SimpleUnpagedGrid;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class AlocacaoManualPresenter 
	implements Presenter
{

	private Display display;
	private GTab gTab;
	private GTabItem gTabItem;
	
	public interface Display extends ITriedaI18nGateway {
		Component getComponent();
		SimpleUnpagedGrid< TurmaStatusDTO > getGrid();
		SimpleUnpagedGrid< AlunoStatusDTO > getAlunosGrid();
		SimpleUnpagedGrid<SalaStatusDTO> getAmbientesGrid();
		void setProxy( RpcProxy< ListLoadResult< TurmaStatusDTO > > proxy );
		DemandaDTO getDemanda();
		CampusDTO getCampusDTO();
		DisciplinaDTO getDisciplinaDTO();
		Button getTurmasRemoveButton();
		Button getTurmasEditarButton();
		Button getTurmasNewButton();
		Button getTurmasNewButton2();
		TextField<String> getNovaTurmaNomeTextField();
		void refreshDemandasPanel(int planejado, int naoPlanejado, int naoAtendido);
		void refreshTurmaSelecionadaPanel();
		void setTurmaSelecionada(TurmaDTO turmaDTO, List<AulaDTO> aulas, String status);
		Button getSalvarTurmaButton();
		Button getEditarTurmaButton();
		Button getRemoverTurmaButton();
		TurmaDTO getTurmaSelecionada();
		String getTurmaSelecionadaStatus();
		SalaAutoCompleteBox getSalaComboBox();
		Button getProxAmbienteButton();
		Button getAntAmbienteButton();
		Button getFiltrarButton();
		GradeHorariaSalaGrid getSalaGridPanel();
		RelatorioVisaoSalaFiltro getSalaFiltro();
		List<Button> getMostrarGradeBts();
		List<Button> getEditarAulaBts();
		List<Button> getRemoverAulaBts();
		List<AulaDTO> getAulasSelecionadas();
		void setAmbientesProxy( RpcProxy<ListLoadResult<SalaStatusDTO>> ambientesProxy );
		void setAlunosProxy( RpcProxy< ListLoadResult< AlunoStatusDTO > > proxy );
		void setProfessoresProxy( RpcProxy< ListLoadResult< ProfessorStatusDTO > > proxy );
		Button getCancelarMarcacoesAlunosButton();
		Button getMarcarTodosAlunosButton();
		Button getDesmarcarTodosAlunosButton();
		Button getCancelarMarcacoesProfessoresButton();
		void setAulaNaGrade(AulaDTO aulaDTO);
		AulaDTO getAulaNaGrade();
		SimpleUnpagedGrid< ProfessorStatusDTO > getProfessoresGrid();
		Button getNovaAulaButton();
		Button getSalvarMarcacoesAlunosButton();
		Button getSalvarMarcacoesProfessoresButton();
		Button getConfirmarTurmaButton();
		Button getDesconfirmarTurmaButton();
		Button getAlocacoesAtalhoButton();
		public void atualizaNumAlunosMarcados();
		ContentPanel getTurmaSelecionadaPanel();
	}
	
	private CenarioDTO cenarioDTO;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	
	public AlocacaoManualPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO, Display display )
	{
		this.display = display;
		this.cenarioDTO = cenarioDTO;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		configureProxy();
		setListeners();
	}
	
 	private void setListeners()
 	{ 		
		this.getDisplay().getTurmasRemoveButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					final List< TurmaStatusDTO > list = getDisplay().getGrid().getGrid().getSelectionModel().getSelectedItems();
					final AtendimentosServiceAsync service = Services.atendimentos();
					getDisplay().getTurmaSelecionadaPanel().mask(getDisplay().getI18nMessages().loading(), "loading");
					service.deleteTurmasStatus(cenarioDTO, getDisplay().getDemanda(), list, new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Não foi possível remover a(s) Turma(s)", null );
						}

						@Override
						public void onSuccess( Void result )
						{
							getDisplay().getGrid().updateList();
							int planejadas = 0;
							int naoPlanejadas = 0;
							int naoAtendidas = 0;
							for (TurmaStatusDTO turmas : list)
							{
								if (turmas.getStatus().equals("Planejada"))
								{
									planejadas -= turmas.getQtdeTotal();
									naoAtendidas += turmas.getQtdeTotal();
								}
								else if (turmas.getStatus().equals("Não Planejada"))
								{
									naoPlanejadas -= turmas.getQtdeTotal();
									naoAtendidas += turmas.getQtdeTotal();
								}
							}
							getDisplay().refreshDemandasPanel(planejadas, naoPlanejadas, naoAtendidas);
							getDisplay().getGrid().updateList();
							getDisplay().setTurmaSelecionada(null, null, "");
							getDisplay().refreshTurmaSelecionadaPanel();
							getDisplay().getTurmaSelecionadaPanel().unmask();
							getDisplay().getAlunosGrid().updateList();
							if (getDisplay().getProfessoresGrid().isRendered())
								getDisplay().getProfessoresGrid().updateList();
							if (getDisplay().getAmbientesGrid().isRendered())
								getDisplay().getAmbientesGrid().updateList();
							Info.display( "Removido", "Turma(s) removida(s) com sucesso!" );
						}
					});

				}
			});
		
		this.getDisplay().getRemoverTurmaButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					getDisplay().getTurmaSelecionadaPanel().mask(getDisplay().getI18nMessages().loading(), "loading");
					final AtendimentosServiceAsync service = Services.atendimentos();
					service.deleteTurmaSelecionada(cenarioDTO, getDisplay().getDemanda(), getDisplay().getTurmaSelecionada(), new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Não foi possível remover a(s) Turma(s)", null );
						}

						@Override
						public void onSuccess( Void result )
						{
							getDisplay().getGrid().updateList();
							int planejadas = 0;
							int naoPlanejadas = 0;
							if (getDisplay().getTurmaSelecionadaStatus().equals("Planejada"))
								planejadas -= getDisplay().getTurmaSelecionada().getNoAlunos();
							else if (getDisplay().getTurmaSelecionadaStatus().equals("Não Planejada"))
								naoPlanejadas -= getDisplay().getTurmaSelecionada().getNoAlunos();
							getDisplay().refreshDemandasPanel(planejadas, naoPlanejadas, getDisplay().getTurmaSelecionada().getNoAlunos());
							getDisplay().getGrid().updateList();
							getDisplay().setTurmaSelecionada(null, new ArrayList<AulaDTO>(), null);
							getDisplay().refreshTurmaSelecionadaPanel();
							getDisplay().getTurmaSelecionadaPanel().unmask();
							getDisplay().getAlunosGrid().updateList();
							if (getDisplay().getProfessoresGrid().isRendered())
								getDisplay().getProfessoresGrid().updateList();
							if (getDisplay().getAmbientesGrid().isRendered())
								getDisplay().getAmbientesGrid().updateList();
							Info.display( "Removido", "Turma(s) removida(s) com sucesso!" );
						}
					});

				}
			});
		
/*		this.getDisplay().getTurmasNewButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					Presenter presenter = new NovaTurmaFormPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new NovaTurmaFormView( cenarioDTO, getDisplay().getCampusDTO(), getDisplay().getDisciplinaDTO(), new TurmaDTO(), false), getDisplay().getGrid(), AlocacaoManualPresenter.this );

					presenter.go( null );
				}
			});
		*/
		this.getDisplay().getEditarTurmaButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					Presenter presenter = new NovaTurmaFormPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new NovaTurmaFormView( cenarioDTO, getDisplay().getCampusDTO(), getDisplay().getDisciplinaDTO(), getDisplay().getTurmaSelecionada(), true ), getDisplay().getGrid(), AlocacaoManualPresenter.this );

					presenter.go( null );
				}
			});
		
		this.getDisplay().getTurmasEditarButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					TurmaStatusDTO turmaStatusDTO = getDisplay().getGrid().getSelectionModel().getSelectedItem();
					TurmaDTO turmaDTO = new TurmaDTO();
					turmaDTO.setId(turmaStatusDTO.getId());
					turmaDTO.setInstituicaoEnsinoId(turmaStatusDTO.getInstituicaoEnsinoId());
					turmaDTO.setDisciplinaId(turmaStatusDTO.getDisciplinaId());
					turmaDTO.setCenarioId(turmaStatusDTO.getCenarioId());
					turmaDTO.setNome(turmaStatusDTO.getNome());
					
					Presenter presenter = new NovaTurmaFormPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new NovaTurmaFormView( cenarioDTO, getDisplay().getCampusDTO(), getDisplay().getDisciplinaDTO(), turmaDTO, true ), getDisplay().getGrid(), AlocacaoManualPresenter.this );

					presenter.go( null );
				}
			});
		
		this.getDisplay().getTurmasNewButton2().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					getDisplay().getTurmaSelecionadaPanel().mask(getDisplay().getI18nMessages().loading(), "loading");
					final AtendimentosServiceAsync service = Services.atendimentos();
					if (getDisplay().getNovaTurmaNomeTextField().getValue() != null)
					{
						final TurmaDTO turmaDTO = new TurmaDTO();
						turmaDTO.setNome(getDisplay().getNovaTurmaNomeTextField().getValue());
						turmaDTO.setParcial(true);
						turmaDTO.setDisciplinaId(getDisplay().getDisciplinaDTO().getId());
						turmaDTO.setCampusId(getDisplay().getCampusDTO().getId());
						turmaDTO.setCenarioId(cenarioDTO.getId());
						turmaDTO.setInstituicaoEnsinoId(instituicaoEnsinoDTO.getId());
						turmaDTO.setNoAlunos(0);
						turmaDTO.setCredAlocados(0);
						
						service.saveTurma(turmaDTO, new AsyncCallback< TurmaDTO >()
						{
							@Override
							public void onFailure( Throwable caught )
							{
								if (caught instanceof TriedaException)
									MessageBox.alert("ERRO!", caught.getMessage(), null);
								else
									MessageBox.alert( "ERRO!", "Não foi possível criar a Turma", null );
							}

							@Override
							public void onSuccess( TurmaDTO result )
							{
								getDisplay().getNovaTurmaNomeTextField().setValue(null);
								getDisplay().getGrid().updateList();
								getDisplay().setTurmaSelecionada(result, new ArrayList<AulaDTO>(), "Parcial");
								getDisplay().refreshTurmaSelecionadaPanel();
								addAulasButtonsListeners();
								getDisplay().getTurmaSelecionadaPanel().unmask();
								getDisplay().getAlunosGrid().updateList();
								if (getDisplay().getProfessoresGrid().isRendered())
									getDisplay().getProfessoresGrid().updateList();
								if (getDisplay().getAmbientesGrid().isRendered())
									getDisplay().getAmbientesGrid().updateList();
								Info.display( "Salvo!", "Turma criada com sucesso!" );
							}
						});


						
					}
					else
					{
						MessageBox.alert( "ERRO!", "Nome da turma está em branco", null );
					}
				}
			});
		
		this.getDisplay().getGrid().addGridSelectionChangedListener("selecionarTurma", new SelectionChangedListener<TurmaStatusDTO>()
		{
			@Override
		    public void selectionChanged(SelectionChangedEvent<TurmaStatusDTO> se)
			{
		        if(display.getGrid().getSelectionModel().getSelectedItems().size() == 1)
		        {
					final TurmaStatusDTO turmaSelecionada = se.getSelectedItem();
					final AtendimentosServiceAsync service = Services.atendimentos();
					getDisplay().getTurmaSelecionadaPanel().mask(getDisplay().getI18nMessages().loading(), "loading");
					service.selecionarTurma(turmaSelecionada, cenarioDTO, getDisplay().getDemanda(), new AsyncCallback< ParDTO<TurmaDTO, List<AulaDTO>> >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Não foi possível selecionar a turma", null );
						}

						@Override
						public void onSuccess( ParDTO<TurmaDTO, List<AulaDTO>> result )
						{
							getDisplay().setTurmaSelecionada(result.getPrimeiro(), result.getSegundo(), turmaSelecionada.getStatus());
							getDisplay().refreshTurmaSelecionadaPanel();
							addAulasButtonsListeners();
							getDisplay().getTurmaSelecionadaPanel().unmask();
							getDisplay().getAlunosGrid().updateList();
							if (getDisplay().getProfessoresGrid().isRendered())
								getDisplay().getProfessoresGrid().updateList();
							if (getDisplay().getAmbientesGrid().isRendered())
								getDisplay().getAmbientesGrid().updateList();
						}
					});
				}
			}
		});
		
		this.getDisplay().getFiltrarButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					if (getDisplay().getSalaComboBox().getValue() != null)
					{
						getDisplay().getSalaGridPanel().getFiltro().setSalaCodigo(getDisplay().getSalaComboBox().getValue().getCodigo());
						getDisplay().getSalaGridPanel().requestAtendimentos();
						getDisplay().getProxAmbienteButton().enable();
						getDisplay().getAntAmbienteButton().enable();
						getDisplay().getSalaGridPanel().addGridListener(Events.CellDoubleClick, new Listener<GridEvent<LinhaDeCredito>>()
						{
				            @Override
				            public void handleEvent(final GridEvent<LinhaDeCredito> be)
				            {
								if (getDisplay().getTurmaSelecionada() != null)
								{
					            	Integer semana = be.getColIndex() == 7 ? 1 : be.getColIndex() + 1;
					            	Services.horariosAula().findHorarioDisponivelCenario(be.getModel().getDisplay().substring(0, 5), semana , display.getDemanda() , new AsyncCallback< TrioDTO<HorarioDisponivelCenarioDTO, String, SemanaLetivaDTO> >()
									{
										@Override
										public void onFailure( Throwable caught )
										{
											if (caught instanceof TriedaException)
											{
												MessageBox.alert( "ERRO!", caught.getMessage(), null );
											}
											else
												MessageBox.alert( "ERRO!", "Não foi possível obter o horario da grade", null );
										}
			
										@Override
										public void onSuccess( TrioDTO<HorarioDisponivelCenarioDTO, String, SemanaLetivaDTO> result )
										{
											int creditosAlocadosPratico = 0;
											int creditosAlocadosTeorico = 0;
											for (AulaDTO aulaDTO : getDisplay().getAulasSelecionadas())
											{
												creditosAlocadosPratico += aulaDTO.getCreditosPraticos();
												creditosAlocadosTeorico += aulaDTO.getCreditosTeoricos();
											}
											AulaDTO aulaDTO = new AulaDTO();
											aulaDTO.setSemanaString(result.getSegundo());
											Presenter presenter = new AulaFormPresenter( instituicaoEnsinoDTO,
												cenarioDTO, creditosAlocadosPratico, creditosAlocadosTeorico, 
												new AulaFormView( cenarioDTO, getDisplay().getCampusDTO(), getDisplay().getDisciplinaDTO(),
												getDisplay().getSalaComboBox().getValue(), result.getTerceiro(), result.getPrimeiro(), getDisplay().getTurmaSelecionada(), aulaDTO, getDisplay().getAulasSelecionadas() ), AlocacaoManualPresenter.this);
			
											presenter.go( null );
										}
									});
								}
				            }
						});
					}
					else
					{
						getDisplay().getProxAmbienteButton().disable();
						getDisplay().getAntAmbienteButton().disable();
					}
				}
			});
		
		this.getDisplay().getSalaComboBox().addSelectionChangedListener(
			new SelectionChangedListener< SalaDTO >()
			{
				@Override
				public void selectionChanged(
					SelectionChangedEvent< SalaDTO > se )
				{
					final SalaDTO salaDTO = se.getSelectedItem();
					if ( salaDTO == null )
					{
						getDisplay().getProxAmbienteButton().disable();
						getDisplay().getAntAmbienteButton().disable();
					}
				}
			});
		
		this.getDisplay().getProxAmbienteButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					if (getDisplay().getSalaComboBox().getValue() != null)
					{
						SalasServiceAsync service = Services.salas();

						service.getProxSala(cenarioDTO, getDisplay().getSalaComboBox().getValue(), new AsyncCallback< SalaDTO >()
						{
							@Override
							public void onFailure(Throwable caught) {
								MessageBox.alert( "ERRO!", "Não foi possível obter o próximo ambiente", null );
								
							}
							@Override
							public void onSuccess(SalaDTO result) {
								getDisplay().getSalaComboBox().setValue(result);
								if (getDisplay().getSalaComboBox().getValue() != null)
								{
									getDisplay().getSalaGridPanel().getFiltro().setSalaCodigo(getDisplay().getSalaComboBox().getValue().getCodigo());
									getDisplay().getSalaGridPanel().requestAtendimentos();
									getDisplay().getProxAmbienteButton().enable();
									getDisplay().getAntAmbienteButton().enable();
								}
								else
								{
									getDisplay().getProxAmbienteButton().disable();
									getDisplay().getAntAmbienteButton().disable();
								}
							}
						});
					}
				}
			});

		this.getDisplay().getAntAmbienteButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					if (getDisplay().getSalaComboBox().getValue() != null)
					{
						SalasServiceAsync service = Services.salas();

						service.getAntSala(cenarioDTO, getDisplay().getSalaComboBox().getValue(), new AsyncCallback< SalaDTO >()
						{
							@Override
							public void onFailure(Throwable caught) {
								MessageBox.alert( "ERRO!", "Não foi possível obter o próximo ambiente", null );
								
							}
							@Override
							public void onSuccess(SalaDTO result) {
								getDisplay().getSalaComboBox().setValue(result);
								if (getDisplay().getSalaComboBox().getValue() != null)
								{
									getDisplay().getSalaGridPanel().getFiltro().setSalaCodigo(getDisplay().getSalaComboBox().getValue().getCodigo());
									getDisplay().getSalaGridPanel().requestAtendimentos();
									getDisplay().getProxAmbienteButton().enable();
									getDisplay().getAntAmbienteButton().enable();
								}
								else
								{
									getDisplay().getProxAmbienteButton().disable();
									getDisplay().getAntAmbienteButton().disable();
								}
							}
						});
					}
				}
			});
		
			getDisplay().getCancelarMarcacoesAlunosButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce) {
					getDisplay().getAlunosGrid().getGrid().getStore().rejectChanges();
					Info.display("Cancelado", "Alterações canceladas com sucesso");
				}
			});
			getDisplay().getMarcarTodosAlunosButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce) {
					for (AlunoStatusDTO model : getDisplay().getAlunosGrid().getGrid().getStore().getModels())
					{
						getDisplay().getAlunosGrid().getGrid().getStore().getRecord(model).set(AlunoStatusDTO.PROPERTY_MARCADO, true);
					}
				}
			});
			getDisplay().getDesmarcarTodosAlunosButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce) {
					for (AlunoStatusDTO model : getDisplay().getAlunosGrid().getGrid().getStore().getModels())
					{
						getDisplay().getAlunosGrid().getGrid().getStore().getRecord(model).set(AlunoStatusDTO.PROPERTY_MARCADO, false);
					}
				}
			});
			getDisplay().getCancelarMarcacoesProfessoresButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce) {
					getDisplay().getProfessoresGrid().getGrid().getStore().rejectChanges();
					Info.display("Cancelado", "Alterações canceladas com sucesso");
				}
			});
			getDisplay().getSalvarMarcacoesAlunosButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce) {
					final AtendimentosServiceAsync service = Services.atendimentos();
					final int noAlunosAtuais = getDisplay().getTurmaSelecionada().getNoAlunos();
					service.verificaViabilidadeAulasNovosAlunos(cenarioDTO, display.getTurmaSelecionada(), display.getAulasSelecionadas(), getAlunosStatusMarcados(), new AsyncCallback< TrioDTO<Boolean, List<String>, List<String>> >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Não foi possível verificar viabilidade da nova aula", null );
						}

						@Override
						public void onSuccess( TrioDTO<Boolean, List<String>, List<String>> result )
						{
							if (!result.getSegundo().isEmpty() || !result.getTerceiro().isEmpty())
							{
								Presenter presenter = new ErrorsWarningsNewAulaPresenter( result.getTerceiro(),
										result.getSegundo(), new ErrorsWarningsNewAulaView(), cenarioDTO, null, null,
										AlocacaoManualPresenter.this, TipoModal.ALUNO);

								presenter.go( null );
							}
							else
							{
								getDisplay().getTurmaSelecionadaPanel().mask(getDisplay().getI18nMessages().loading(), "loading");
								service.alocaAlunosTurma(cenarioDTO, getDisplay().getDemanda(), display.getTurmaSelecionada(), getAlunosStatusModificados(), new AsyncCallback< Void >()
								{
									@Override
									public void onFailure( Throwable caught )
									{
										MessageBox.alert( "ERRO!", "Não foi possível alocar o(s) aluno(s)", null );
									}
			
									@Override
									public void onSuccess( Void result )
									{
										final TurmaStatusDTO turmaSelecionada = new TurmaStatusDTO();
										turmaSelecionada.setCenarioId(cenarioDTO.getId());
										turmaSelecionada.setDisciplinaId(display.getTurmaSelecionada().getDisciplinaId());
										turmaSelecionada.setId(display.getTurmaSelecionada().getId());
										turmaSelecionada.setInstituicaoEnsinoId(display.getTurmaSelecionada().getInstituicaoEnsinoId());
										turmaSelecionada.setNome(display.getTurmaSelecionada().getNome());
										turmaSelecionada.setTurma(display.getTurmaSelecionada().getId() == null ? display.getTurmaSelecionada().getNome() : null);
										turmaSelecionada.setStatus(getDisplay().getTurmaSelecionadaStatus());
										
										service.selecionarTurma(turmaSelecionada, cenarioDTO, getDisplay().getDemanda(), new AsyncCallback< ParDTO<TurmaDTO, List<AulaDTO>> >()
										{
											@Override
											public void onFailure( Throwable caught )
											{
												MessageBox.alert( "ERRO!", "Não foi possível selecionar a turma", null );
											}
			
											@Override
											public void onSuccess( ParDTO<TurmaDTO, List<AulaDTO>> result )
											{
												getDisplay().getAlunosGrid().updateList();
												getDisplay().getGrid().updateList();
												int planejadas = 0;
												int naoPlanejadas = 0;
												if (turmaSelecionada.getStatus().equals("Planejada"))
												{
													planejadas = result.getPrimeiro().getNoAlunos() - noAlunosAtuais;
												}
												else if (turmaSelecionada.getStatus().equals("Não Planejada"))
												{
													naoPlanejadas = result.getPrimeiro().getNoAlunos() - noAlunosAtuais;
												}
												getDisplay().refreshDemandasPanel(planejadas, naoPlanejadas, 0);
												if (getDisplay().getAulaNaGrade() != null)
												{
													getDisplay().getSalaGridPanel().requestAtendimentos();
												}
												getDisplay().setTurmaSelecionada(result.getPrimeiro(), result.getSegundo(), getDisplay().getTurmaSelecionadaStatus());
												getDisplay().refreshTurmaSelecionadaPanel();
												addAulasButtonsListeners();
												getDisplay().getTurmaSelecionadaPanel().unmask();
												Info.display( "Alocados", "Aluno(s) alocado(s) a turma com sucesso!" );
											}
										});
									}
								});
							}
						}
					});
				}
			});
			
			getDisplay().getSalvarMarcacoesProfessoresButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce) {
					List<Record> records = getDisplay().getProfessoresGrid().getGrid().getStore().getModifiedRecords();
					final ProfessorStatusDTO professorStatusDTO = (ProfessorStatusDTO) records.get(0).getModel();
					final AtendimentosServiceAsync service = Services.atendimentos();
					service.verificaViabilidadeAulasNovoProfessor(cenarioDTO, display.getTurmaSelecionada(), display.getAulaNaGrade(), professorStatusDTO, new AsyncCallback< TrioDTO<Boolean, List<String>, List<String>> >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Não foi possível verificar viabilidade da nova aula", null );
						}

						@Override
						public void onSuccess( TrioDTO<Boolean, List<String>, List<String>> result )
						{
							if (!result.getSegundo().isEmpty() || !result.getTerceiro().isEmpty())
							{
								Presenter presenter = new ErrorsWarningsNewAulaPresenter( result.getTerceiro(),
										result.getSegundo(), new ErrorsWarningsNewAulaView(), cenarioDTO, null, null,
										AlocacaoManualPresenter.this, TipoModal.PROFESSOR);

								presenter.go( null );
							}
							else
							{
								getDisplay().getTurmaSelecionadaPanel().mask(getDisplay().getI18nMessages().loading(), "loading");
								service.alocaProfessoresAula(cenarioDTO, getDisplay().getDemanda(), display.getTurmaSelecionada(), display.getAulaNaGrade(), professorStatusDTO, new AsyncCallback< Void >()
								{
									@Override
									public void onFailure( Throwable caught )
									{
										MessageBox.alert( "ERRO!", "Não foi possível alocar o professor", null );
									}
			
									@Override
									public void onSuccess( Void result )
									{
										TurmaStatusDTO turmaSelecionada = new TurmaStatusDTO();
										turmaSelecionada.setCenarioId(cenarioDTO.getId());
										turmaSelecionada.setDisciplinaId(display.getTurmaSelecionada().getDisciplinaId());
										turmaSelecionada.setId(display.getTurmaSelecionada().getId());
										turmaSelecionada.setInstituicaoEnsinoId(display.getTurmaSelecionada().getInstituicaoEnsinoId());
										turmaSelecionada.setNome(display.getTurmaSelecionada().getNome());
										turmaSelecionada.setTurma(display.getTurmaSelecionada().getId() == null ? display.getTurmaSelecionada().getNome() : null);
										turmaSelecionada.setStatus(getDisplay().getTurmaSelecionadaStatus());
										
										service.selecionarTurma(turmaSelecionada, cenarioDTO, getDisplay().getDemanda(), new AsyncCallback< ParDTO<TurmaDTO, List<AulaDTO>> >()
										{
											@Override
											public void onFailure( Throwable caught )
											{
												MessageBox.alert( "ERRO!", "Não foi possível selecionar a turma", null );
											}
			
											@Override
											public void onSuccess( ParDTO<TurmaDTO, List<AulaDTO>> result )
											{
												getDisplay().setTurmaSelecionada(result.getPrimeiro(), result.getSegundo(), getDisplay().getTurmaSelecionadaStatus());
												if (professorStatusDTO.getProfessorId() != null)
												{
													display.getAulaNaGrade().setProfessorId(professorStatusDTO.getProfessorId());
												}
												else
												{
													display.getAulaNaGrade().setProfessorVirtualId(professorStatusDTO.getProfessorVirtualId());
												}
												getDisplay().refreshTurmaSelecionadaPanel();
												addAulasButtonsListeners();
												getDisplay().getTurmaSelecionadaPanel().unmask();
												getDisplay().getProfessoresGrid().updateList();
												getDisplay().getGrid().updateList();
												Info.display( "Alocados", "Professor alocado a aula com sucesso!" );
											}
										});
									}
								});
							}
						}
					});
				}
			});
			
			getDisplay().getSalvarTurmaButton().addSelectionListener(new SelectionListener<ButtonEvent>()
			{
				@Override
				public void componentSelected(ButtonEvent ce)
				{
					final AtendimentosServiceAsync service = Services.atendimentos();
					service.verificaViabilidadeSalvarTurma(cenarioDTO, display.getTurmaSelecionada(), display.getAulasSelecionadas(), new AsyncCallback< TrioDTO<Boolean, List<String>, List<String>> >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Não foi possível verificar viabilidade ao salvar a turma", null );
						}

						@Override
						public void onSuccess( TrioDTO<Boolean, List<String>, List<String>> result )
						{
							if (!result.getSegundo().isEmpty() || !result.getTerceiro().isEmpty())
							{
								Presenter presenter = new ErrorsWarningsNewAulaPresenter( result.getTerceiro(),
										result.getSegundo(), new ErrorsWarningsNewAulaView(), cenarioDTO, null, null,
										AlocacaoManualPresenter.this, TipoModal.TURMA);

								presenter.go( null );
							}
							else
							{
								getDisplay().getTurmaSelecionadaPanel().mask(getDisplay().getI18nMessages().loading(), "loading");
								service.salvarTurma(display.getTurmaSelecionada(), new AsyncCallback< Void >()
								{
									@Override
									public void onFailure( Throwable caught )
									{
										MessageBox.alert( "ERRO!", "Não foi possível salvar a turma", null );
									}
			
									@Override
									public void onSuccess( Void result )
									{
										TurmaStatusDTO turmaSelecionada = new TurmaStatusDTO();
										turmaSelecionada.setCenarioId(cenarioDTO.getId());
										turmaSelecionada.setDisciplinaId(display.getTurmaSelecionada().getDisciplinaId());
										turmaSelecionada.setId(display.getTurmaSelecionada().getId());
										turmaSelecionada.setInstituicaoEnsinoId(display.getTurmaSelecionada().getInstituicaoEnsinoId());
										turmaSelecionada.setNome(display.getTurmaSelecionada().getNome());
										turmaSelecionada.setTurma(display.getTurmaSelecionada().getNome());
										turmaSelecionada.setStatus(getDisplay().getTurmaSelecionadaStatus());
										
										service.selecionarTurma(turmaSelecionada, cenarioDTO, getDisplay().getDemanda(), new AsyncCallback< ParDTO<TurmaDTO, List<AulaDTO>> >()
										{
											@Override
											public void onFailure( Throwable caught )
											{
												MessageBox.alert( "ERRO!", "Não foi possível selecionar a turma", null );
											}
			
											@Override
											public void onSuccess( ParDTO<TurmaDTO, List<AulaDTO>> result )
											{
												getDisplay().getAlunosGrid().updateList();
												getDisplay().getGrid().updateList();
												getDisplay().setTurmaSelecionada(result.getPrimeiro(), result.getSegundo(), "Não Planejada");
												getDisplay().refreshDemandasPanel(0, result.getPrimeiro().getNoAlunos(), -result.getPrimeiro().getNoAlunos());
												getDisplay().refreshTurmaSelecionadaPanel();
												addAulasButtonsListeners();
												getDisplay().getTurmaSelecionadaPanel().unmask();
												Info.display( "Salvo", "Turma salva com sucesso!" );
											}
										});
									}
								});
							}
						}
					});
				}
			});
			
			getDisplay().getConfirmarTurmaButton().addSelectionListener(new SelectionListener<ButtonEvent>()
			{
				@Override
				public void componentSelected(ButtonEvent ce)
				{
					getDisplay().getTurmaSelecionadaPanel().mask(getDisplay().getI18nMessages().loading(), "loading");
					final AtendimentosServiceAsync service = Services.atendimentos();
					service.confirmarTurmaSelecionada(display.getDemanda(), display.getTurmaSelecionada(), new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Não foi possível salvar a turma", null );
						}

						@Override
						public void onSuccess( Void result )
						{
							TurmaStatusDTO turmaSelecionada = new TurmaStatusDTO();
							turmaSelecionada.setCenarioId(cenarioDTO.getId());
							turmaSelecionada.setDisciplinaId(display.getTurmaSelecionada().getDisciplinaId());
							turmaSelecionada.setId(display.getTurmaSelecionada().getId());
							turmaSelecionada.setInstituicaoEnsinoId(display.getTurmaSelecionada().getInstituicaoEnsinoId());
							turmaSelecionada.setNome(display.getTurmaSelecionada().getNome());
							turmaSelecionada.setTurma(display.getTurmaSelecionada().getNome());
							turmaSelecionada.setStatus("Planejada");
							
							service.selecionarTurma(turmaSelecionada, cenarioDTO, getDisplay().getDemanda(), new AsyncCallback< ParDTO<TurmaDTO, List<AulaDTO>> >()
							{
								@Override
								public void onFailure( Throwable caught )
								{
									MessageBox.alert( "ERRO!", "Não foi possível selecionar a turma", null );
								}

								@Override
								public void onSuccess( ParDTO<TurmaDTO, List<AulaDTO>> result )
								{
									getDisplay().getAlunosGrid().updateList();
									getDisplay().getGrid().updateList();
									getDisplay().setTurmaSelecionada(result.getPrimeiro(), result.getSegundo(), "Planejada");
									getDisplay().refreshDemandasPanel(result.getPrimeiro().getNoAlunos(), -result.getPrimeiro().getNoAlunos(), 0);
									getDisplay().refreshTurmaSelecionadaPanel();
									addAulasButtonsListeners();
									getDisplay().getTurmaSelecionadaPanel().unmask();
									Info.display( "Salvo", "Turma confirmada com sucesso!" );
								}
							});
						}
					});
				}
			});
			
			getDisplay().getDesconfirmarTurmaButton().addSelectionListener(new SelectionListener<ButtonEvent>()
			{
				@Override
				public void componentSelected(ButtonEvent ce)
				{
					getDisplay().getTurmaSelecionadaPanel().mask(getDisplay().getI18nMessages().loading(), "loading");
					final AtendimentosServiceAsync service = Services.atendimentos();
					service.desconfirmarTurmaSelecionada(display.getDemanda(), display.getTurmaSelecionada(), new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Não foi possível desconfirmar a turma", null );
						}

						@Override
						public void onSuccess( Void result )
						{
							TurmaStatusDTO turmaSelecionada = new TurmaStatusDTO();
							turmaSelecionada.setCenarioId(cenarioDTO.getId());
							turmaSelecionada.setDisciplinaId(display.getTurmaSelecionada().getDisciplinaId());
							turmaSelecionada.setId(display.getTurmaSelecionada().getId());
							turmaSelecionada.setInstituicaoEnsinoId(display.getTurmaSelecionada().getInstituicaoEnsinoId());
							turmaSelecionada.setNome(display.getTurmaSelecionada().getNome());
							turmaSelecionada.setTurma(display.getTurmaSelecionada().getNome());
							turmaSelecionada.setStatus("Não Planejada");
							
							service.selecionarTurma(turmaSelecionada, cenarioDTO, getDisplay().getDemanda(), new AsyncCallback< ParDTO<TurmaDTO, List<AulaDTO>> >()
							{
								@Override
								public void onFailure( Throwable caught )
								{
									MessageBox.alert( "ERRO!", "Não foi possível selecionar a turma", null );
								}

								@Override
								public void onSuccess( ParDTO<TurmaDTO, List<AulaDTO>> result )
								{
									getDisplay().getAlunosGrid().updateList();
									getDisplay().getGrid().updateList();
									getDisplay().setTurmaSelecionada(result.getPrimeiro(), result.getSegundo(), "Não Planejada");
									getDisplay().refreshDemandasPanel(-result.getPrimeiro().getNoAlunos(), result.getPrimeiro().getNoAlunos(), 0);
									getDisplay().refreshTurmaSelecionadaPanel();
									getDisplay().getTurmaSelecionadaPanel().unmask();
									addAulasButtonsListeners();
									Info.display( "Salvo", "Turma desconfirmada com sucesso!" );
								}
							});
						}
					});
				}
			});
			
			this.display.getAlocacoesAtalhoButton().addSelectionListener(
					new SelectionListener< ButtonEvent >()
				{
					@Override
					public void componentSelected( ButtonEvent ce )
					{
						Presenter presenter = new AlocacaoManualDisciplinaFormPresenter(instituicaoEnsinoDTO,
								cenarioDTO, new AlocacaoManualDisciplinaFormView( cenarioDTO ), gTab, gTabItem );

						presenter.go( gTab );
					}
				});
	}

	private void configureProxy()
 	{
 		final AtendimentosServiceAsync service = Services.atendimentos();

 		RpcProxy< ListLoadResult< TurmaStatusDTO > > proxy =
 			new RpcProxy< ListLoadResult< TurmaStatusDTO > >()
 		{
 			@Override
 			public void load( Object loadConfig,
 				AsyncCallback< ListLoadResult< TurmaStatusDTO > > callback)
 			{
 				service.getTurmasStatus(cenarioDTO, getDisplay().getDemanda(), callback);
 			}
 		};
 		
 		RpcProxy< ListLoadResult< SalaStatusDTO > > ambientesProxy =
 			new RpcProxy< ListLoadResult< SalaStatusDTO > >()
 		{
 			@Override
 			public void load( Object loadConfig,
 				AsyncCallback< ListLoadResult< SalaStatusDTO > > callback)
 			{
 				if (getDisplay().getTurmaSelecionada() == null)
 				{
 					getDisplay().getAmbientesGrid().getGrid().getView().setEmptyText("Ainda não é possível exibir os ambientes associados a disciplina da turma, pois não há uma turma selecionada.");
 				}
 				else
 				{
 	 				getDisplay().getAmbientesGrid().getGrid().getView().setEmptyText("Não existem ambientes relacionados a esta disciplina");
 				}
 				service.getAmbientesTurma(cenarioDTO, getDisplay().getTurmaSelecionada(), getDisplay().getAulasSelecionadas(), getDisplay().getCampusDTO(), callback);
 			}
 		};
 		
 		RpcProxy< ListLoadResult< AlunoStatusDTO > > alunosProxy =
 			new RpcProxy< ListLoadResult< AlunoStatusDTO > >()
 		{
 			@Override
 			public void load( Object loadConfig,
 				AsyncCallback< ListLoadResult< AlunoStatusDTO > > callback)
 			{
 				if (getDisplay().getTurmaSelecionada() == null)
 				{
 					getDisplay().getAlunosGrid().getGrid().getView().setEmptyText("Ainda não é possível associar alunos a uma turma, pois, não há uma turma selecionada.");
 				}
 				else if (getDisplay().getTurmaSelecionada().getCredAlocados() < getDisplay().getDisciplinaDTO().getTotalCreditos())
 				{
 					getDisplay().getAlunosGrid().getGrid().getView().setEmptyText("Ainda não é possível associar alunos à turma," +
 							" pois, não foi definida uma quantidade de aulas suficiente para completar o total de créditos da disciplina.");
 				}
 				else
 				{
 					getDisplay().getAlunosGrid().getGrid().getView().setEmptyText("Não foram encontrados alunos com demanda para esta disciplina. Ou" +
 							"todos os alunos já foram atendidos por outra turma");
 				}
 				
 				service.getAlunosStatus(cenarioDTO, getDisplay().getDemanda(), getDisplay().getTurmaSelecionada(), getDisplay().getAulasSelecionadas(), callback);
 			}
 		};
 		
 		RpcProxy< ListLoadResult< ProfessorStatusDTO > > professoresProxy =
 	 			new RpcProxy< ListLoadResult< ProfessorStatusDTO > >()
 	 		{
 	 			@Override
 	 			public void load( Object loadConfig,
 	 				AsyncCallback< ListLoadResult< ProfessorStatusDTO > > callback)
 	 			{
 	 				if (getDisplay().getTurmaSelecionada() == null)
 	 				{
 	 					getDisplay().getProfessoresGrid().getGrid().getView().setEmptyText("Ainda não é possível associar professores às aulas de uma turma, pois, não há uma turma selecionada.");
 	 				}
 	 				else if (getDisplay().getAulasSelecionadas().size() < 1)
 	 				{
 	 					getDisplay().getProfessoresGrid().getGrid().getView().setEmptyText("Ainda não é possível associar professores às aulas da turma," +
 	 							" pois, não há aulas associadas à turma selecionada.");
 	 				}
 	 				else if (getDisplay().getAulaNaGrade() == null)
 	 				{
 	 					getDisplay().getProfessoresGrid().getGrid().getView().setEmptyText("Selecionar uma das aulas da turma para que o professor alocado e" +
 	 							" os professores candidatos sejam disponibilizados.");
 	 				}
 	 				service.getProfessoresStatus(cenarioDTO, getDisplay().getDemanda(), getDisplay().getTurmaSelecionada(), getDisplay().getAulaNaGrade(), callback);
 	 			}
 	 		};

 	 	getDisplay().setAmbientesProxy( ambientesProxy );
 		getDisplay().setAlunosProxy(alunosProxy);
 		getDisplay().setProfessoresProxy(professoresProxy);
 		getDisplay().setProxy( proxy );
 	}
	
	public void addAulasButtonsListeners()
	{
		getDisplay().getNovaAulaButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					int creditosAlocadosPratico = 0;
					int creditosAlocadosTeorico = 0;
					for (AulaDTO aulaDTO : getDisplay().getAulasSelecionadas())
					{
						creditosAlocadosPratico += aulaDTO.getCreditosPraticos();
						creditosAlocadosTeorico += aulaDTO.getCreditosTeoricos();
					}
					Presenter presenter = new AulaFormPresenter( instituicaoEnsinoDTO,
						cenarioDTO, creditosAlocadosPratico, creditosAlocadosTeorico, 
						new AulaFormView( cenarioDTO, getDisplay().getCampusDTO(), getDisplay().getDisciplinaDTO(),
						null, null, null, getDisplay().getTurmaSelecionada(), new AulaDTO(), getDisplay().getAulasSelecionadas() ), AlocacaoManualPresenter.this);

					presenter.go( null );
				}
			});

		for (int i = 0; i < getDisplay().getAulasSelecionadas().size(); i++)
		{
			final AulaDTO aulaDTO = getDisplay().getAulasSelecionadas().get(i);
			getDisplay().getMostrarGradeBts().get(i).addSelectionListener(
				new SelectionListener< ButtonEvent >()
				{
					@Override
					public void componentSelected( ButtonEvent ce )
					{
						SalasServiceAsync service = Services.salas();
						service.getSala(aulaDTO.getSalaId(), new AsyncCallback< SalaDTO >()
						{
							@Override
							public void onFailure( Throwable caught )
							{
								MessageBox.alert( "ERRO!", "Não foi possível obter a grade horária do ambiente", null );
							}

							@Override
							public void onSuccess( SalaDTO result )
							{
								final SalaDTO salaDTO = result;
								getDisplay().getSalaGridPanel().addGridListener(Events.CellDoubleClick, new Listener<GridEvent<LinhaDeCredito>>() {
						            @Override
						            public void handleEvent(final GridEvent<LinhaDeCredito> be) {
						            	Integer semana = be.getColIndex() == 7 ? 1 : be.getColIndex() + 1;
						            	Services.horariosAula().findHorarioDisponivelCenario(be.getModel().getDisplay().substring(0, 5), semana , display.getDemanda() , new AsyncCallback< TrioDTO<HorarioDisponivelCenarioDTO, String, SemanaLetivaDTO> >()
											{
											@Override
											public void onFailure( Throwable caught )
											{
												if (caught instanceof TriedaException)
												{
													MessageBox.alert( "ERRO!", caught.getMessage(), null );
												}
												else
													MessageBox.alert( "ERRO!", "Não foi possível obter o horario da grade", null );
											}

											@Override
											public void onSuccess( TrioDTO<HorarioDisponivelCenarioDTO, String, SemanaLetivaDTO> result )
											{
												int creditosAlocadosPratico = 0;
												int creditosAlocadosTeorico = 0;
												for (AulaDTO aulaDTO : getDisplay().getAulasSelecionadas())
												{
													creditosAlocadosPratico += aulaDTO.getCreditosPraticos();
													creditosAlocadosTeorico += aulaDTO.getCreditosTeoricos();
												}
												AulaDTO aulaDTO = new AulaDTO();
												aulaDTO.setSemanaString(result.getSegundo());
												Presenter presenter = new AulaFormPresenter( instituicaoEnsinoDTO,
													cenarioDTO, creditosAlocadosPratico, creditosAlocadosTeorico, 
													new AulaFormView( cenarioDTO, getDisplay().getCampusDTO(), getDisplay().getDisciplinaDTO(),
													salaDTO, result.getTerceiro(), result.getPrimeiro(), getDisplay().getTurmaSelecionada(), aulaDTO, getDisplay().getAulasSelecionadas() ), AlocacaoManualPresenter.this);

												presenter.go( null );
											}
										});
						            }
						            
						        });
								getDisplay().getSalaComboBox().setValue(salaDTO);
								getDisplay().getSalaGridPanel().getFiltro().setSalaCodigo(result.getCodigo());
								getDisplay().getSalaGridPanel().setAulaDestaque(aulaDTO);
								getDisplay().setAulaNaGrade(aulaDTO);
								getDisplay().getSalaGridPanel().requestAtendimentos();
								if (getDisplay().getProfessoresGrid().isRendered())
									getDisplay().getProfessoresGrid().updateList();
								if (getDisplay().getAmbientesGrid().isRendered())
									getDisplay().getAmbientesGrid().updateList();
							}
						});
					}
				});
			
			getDisplay().getEditarAulaBts().get(i).addSelectionListener(
					new SelectionListener< ButtonEvent >()
					{
						@Override
						public void componentSelected( ButtonEvent ce )
						{
							final FutureResult<SalaDTO> futureSalaDTO = new FutureResult<SalaDTO>();
							final FutureResult<ParDTO<HorarioDisponivelCenarioDTO, SemanaLetivaDTO>> futureHdcDTO =
									new FutureResult<ParDTO<HorarioDisponivelCenarioDTO, SemanaLetivaDTO>>();
							
							Services.salas().getSala(aulaDTO.getSalaId(), futureSalaDTO);
							Services.horariosAula().getHorarioDisponivelCenario(aulaDTO.getHorarioDisponivelCenarioId(), futureHdcDTO);
							
							FutureSynchronizer synch = new FutureSynchronizer(futureSalaDTO,futureHdcDTO);
							synch.addCallback(new AbstractAsyncCallbackWithDefaultOnFailure<Boolean>(display) {
								@Override
								public void onSuccess(Boolean result)
								{
									int creditosAlocadosPratico = 0;
									int creditosAlocadosTeorico = 0;
									for (AulaDTO aulaDTO : getDisplay().getAulasSelecionadas())
									{
										creditosAlocadosPratico += aulaDTO.getCreditosPraticos();
										creditosAlocadosTeorico += aulaDTO.getCreditosTeoricos();
									}
									Presenter presenter = new AulaFormPresenter( instituicaoEnsinoDTO,
										cenarioDTO, creditosAlocadosPratico, creditosAlocadosTeorico, 
										new AulaFormView( cenarioDTO, getDisplay().getCampusDTO(), getDisplay().getDisciplinaDTO(),
										futureSalaDTO.result(), futureHdcDTO.result().getSegundo(), futureHdcDTO.result().getPrimeiro(),
										getDisplay().getTurmaSelecionada(), aulaDTO, getDisplay().getAulasSelecionadas() ), AlocacaoManualPresenter.this);

									presenter.go( null );
								}
							});
						}
					});
			
			getDisplay().getRemoverAulaBts().get(i).addSelectionListener(
			new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					getDisplay().getTurmaSelecionadaPanel().mask(getDisplay().getI18nMessages().loading(), "loading");
					final AtendimentosServiceAsync service = Services.atendimentos();
					service.removeAula(display.getTurmaSelecionada(), aulaDTO, new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Não foi possível remover a aula", null );
						}

						@Override
						public void onSuccess( Void result )
						{
							if (getDisplay().getAulaNaGrade() != null 
									&& getDisplay().getAulaNaGrade().getNaturalKey().equals(aulaDTO.getNaturalKey()))
							{
								getDisplay().setAulaNaGrade(null);
								getDisplay().getSalaGridPanel().setAulaDestaque(null);
								getDisplay().getSalaGridPanel().requestAtendimentos();
							}
							getDisplay().getTurmaSelecionada().setCredAlocados(getDisplay().getTurmaSelecionada().getCredAlocados()
									- (aulaDTO.getCreditosPraticos() + aulaDTO.getCreditosTeoricos()));
							getDisplay().getAulasSelecionadas().remove(aulaDTO);
							getDisplay().refreshTurmaSelecionadaPanel();
							addAulasButtonsListeners();
							getDisplay().getTurmaSelecionadaPanel().unmask();
							getDisplay().getAlunosGrid().updateList();
							if (getDisplay().getProfessoresGrid().isRendered())
								getDisplay().getProfessoresGrid().updateList();
							if (getDisplay().getAmbientesGrid().isRendered())
								getDisplay().getAmbientesGrid().updateList();
						}
					});
				}
			});
		}
	}
	
	@Override
	public void go( Widget widget )
	{
		gTab = (GTab) widget;
		gTabItem = (GTabItem) this.getDisplay().getComponent();
		gTab.add( gTabItem );
	}

	public Display getDisplay() {
		return display;
	}
	
	public List<AlunoStatusDTO> getAlunosStatusMarcados()
	{
		final List<AlunoStatusDTO> result = new ArrayList<AlunoStatusDTO>();
		for(AlunoStatusDTO model : getDisplay().getAlunosGrid().getGrid().getStore().getModels())
		{
			if (model.getMarcado())
				result.add(model);
		}
		
		return result;
	}
	
	public List<AlunoStatusDTO> getAlunosStatusModificados()
	{
		List<Record> records = getDisplay().getAlunosGrid().getGrid().getStore().getModifiedRecords();
		final List<AlunoStatusDTO> result = new ArrayList<AlunoStatusDTO>();
		for(Record record : records)
		{
			result.add((AlunoStatusDTO) record.getModel());
		}
		
		return result;
	}
}
