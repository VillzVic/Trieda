package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.Record;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.mvp.presenter.ErrorsWarningsNewAulaPresenter.TipoModal;
import com.gapso.web.trieda.main.client.mvp.view.AulaFormView;
import com.gapso.web.trieda.main.client.mvp.view.ErrorsWarningsNewAulaView;
import com.gapso.web.trieda.main.client.mvp.view.NovaTurmaFormView;
import com.gapso.web.trieda.shared.dtos.AlunoStatusDTO;
import com.gapso.web.trieda.shared.dtos.AulaDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorStatusDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaStatusDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AtendimentosServiceAsync;
import com.gapso.web.trieda.shared.services.SalasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaSalaGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoSalaFiltro;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SalaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.SimpleUnpagedGrid;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class AlocacaoManualPresenter 
	implements Presenter
{

	private Display display;
	
	public interface Display extends ITriedaI18nGateway {
		Component getComponent();
		SimpleUnpagedGrid< TurmaStatusDTO > getGrid();
		SimpleUnpagedGrid< AlunoStatusDTO > getAlunosGrid();
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
		Button getSelecionarTurmaButton();
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
									planejadas -= turmas.getQtdeTotal();
								else if (turmas.getStatus().equals("Não Planejada"))
									naoPlanejadas -= turmas.getQtdeTotal();
								naoAtendidas += turmas.getQtdeTotal();
							}
							getDisplay().refreshDemandasPanel(planejadas, naoPlanejadas, naoAtendidas);
							getDisplay().getGrid().updateList();
							getDisplay().setTurmaSelecionada(null, null, "");
							getDisplay().refreshTurmaSelecionadaPanel();
							getDisplay().getAlunosGrid().updateList();
							if (getDisplay().getProfessoresGrid().isRendered())
								getDisplay().getProfessoresGrid().updateList();
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
							getDisplay().getAlunosGrid().updateList();
							if (getDisplay().getProfessoresGrid().isRendered())
								getDisplay().getProfessoresGrid().updateList();
							Info.display( "Removido", "Turma(s) removida(s) com sucesso!" );
						}
					});

				}
			});
		
		this.getDisplay().getTurmasNewButton().addSelectionListener(
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
								getDisplay().getAlunosGrid().updateList();
								if (getDisplay().getProfessoresGrid().isRendered())
									getDisplay().getProfessoresGrid().updateList();
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
		
		this.getDisplay().getSelecionarTurmaButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					final TurmaStatusDTO turmaSelecionada = getDisplay().getGrid().getGrid().getSelectionModel().getSelectedItem();
					final AtendimentosServiceAsync service = Services.atendimentos();
					
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
							getDisplay().getAlunosGrid().updateList();
							if (getDisplay().getProfessoresGrid().isRendered())
								getDisplay().getProfessoresGrid().updateList();
						}
					});
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
					if ( salaDTO != null )
					{
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
					List<Record> records = getDisplay().getAlunosGrid().getGrid().getStore().getModifiedRecords();
					final List<AlunoStatusDTO> list = new ArrayList<AlunoStatusDTO>();
					for(Record record : records)
					{
						list.add((AlunoStatusDTO) record.getModel());
					}
					final AtendimentosServiceAsync service = Services.atendimentos();
					service.verificaViabilidadeAulasNovosAlunos(cenarioDTO, display.getTurmaSelecionada(), display.getAulasSelecionadas(), list, new AsyncCallback< TrioDTO<Boolean, List<String>, List<String>> >()
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
								service.alocaAlunosTurma(cenarioDTO, getDisplay().getDemanda(), display.getTurmaSelecionada(), list, new AsyncCallback< Void >()
								{
									@Override
									public void onFailure( Throwable caught )
									{
										MessageBox.alert( "ERRO!", "Não foi possível alocar o(s) aluno(s)", null );
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
												getDisplay().getAlunosGrid().updateList();
												getDisplay().getGrid().updateList();
												getDisplay().setTurmaSelecionada(result.getPrimeiro(), result.getSegundo(), getDisplay().getTurmaSelecionadaStatus());
												getDisplay().refreshTurmaSelecionadaPanel();
												addAulasButtonsListeners();
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
						null, null, null, getDisplay().getTurmaSelecionada(), new AulaDTO() ), AlocacaoManualPresenter.this);

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
								getDisplay().getSalaGridPanel().getFiltro().setSalaCodigo(result.getCodigo());
								getDisplay().getSalaGridPanel().setAulaDestaque(aulaDTO);
								getDisplay().setAulaNaGrade(aulaDTO);
								getDisplay().getSalaGridPanel().requestAtendimentos();
								if (getDisplay().getProfessoresGrid().isRendered())
									getDisplay().getProfessoresGrid().updateList();
							}
						});
					}
				});
		}
		
	}
	
	@Override
	public void go( Widget widget )
	{
		GTab tab = (GTab) widget;
		tab.add( (GTabItem) this.getDisplay().getComponent() );
	}

	public Display getDisplay() {
		return display;
	}
}
