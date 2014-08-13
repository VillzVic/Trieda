package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.Record;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.gapso.web.trieda.shared.dtos.AulaDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorStatusDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaStatusDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AtendimentosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class ErrorsWarningsNewAulaPresenter	
	implements Presenter
{
	public enum TipoModal
	{
		AULA,
		ALUNO,
		PROFESSOR,
		TURMA
	}
	
	public interface Display
	{
		ContentPanel getMessagesWarningPanel();
		ContentPanel getMessagesErrorPanel();
		SimpleModal getSimpleModal();
	}

	private List< String > warnings;
	private List< String > errors;
	private Display display;
	private CenarioDTO cenarioDTO;
	private AulaDTO aulaDTO;
	private TurmaDTO turmaDTO;
	private TipoModal tipoModal;
	private AlocacaoManualPresenter alocacaoManualPresenter;

	public ErrorsWarningsNewAulaPresenter(
		List< String > warnings,
		List< String > errors, Display display,
		CenarioDTO cenarioDTO, TurmaDTO turmaDTO, AulaDTO aulaDTO,
		AlocacaoManualPresenter alocacaoManualPresenter,
		TipoModal tipoModal)
	{
		this.display = display;
		this.warnings = warnings;
		this.errors = errors;
		this.cenarioDTO = cenarioDTO;
		this.turmaDTO = turmaDTO;
		this.aulaDTO = aulaDTO;
		this.alocacaoManualPresenter = alocacaoManualPresenter;
		this.tipoModal = tipoModal;

		setListeners();
		populationMessages();
	}

	private void setListeners() {
		if (!errors.isEmpty())
		{
			display.getSimpleModal().getSalvarBt().disable();
		}
		
		display.getSimpleModal().getSalvarBt().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				final AtendimentosServiceAsync service = Services.atendimentos();
				if (tipoModal.equals(TipoModal.AULA))
				{
					service.saveAula(alocacaoManualPresenter.getDisplay().getDisciplinaDTO(), 
							alocacaoManualPresenter.getDisplay().getCampusDTO(), turmaDTO, aulaDTO, new AsyncCallback< TurmaDTO >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Não foi possível criar a aula", null );
						}
	
						@Override
						public void onSuccess( TurmaDTO result )
						{
							final TurmaStatusDTO turmaSelecionada = new TurmaStatusDTO();
							alocacaoManualPresenter.getDisplay().getTurmaSelecionada().setId(result.getId());
							turmaSelecionada.setCenarioId(cenarioDTO.getId());
							turmaSelecionada.setDisciplinaId(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getDisciplinaId());
							turmaSelecionada.setId(result.getId());
							turmaSelecionada.setInstituicaoEnsinoId(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getInstituicaoEnsinoId());
							turmaSelecionada.setNome(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getNome());
							turmaSelecionada.setTurma(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getId() == null ? alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getNome() : null);
							turmaSelecionada.setStatus(result.getParcial() ? "Parcial" : alocacaoManualPresenter.getDisplay().getTurmaSelecionadaStatus());
							
							service.selecionarTurma(turmaSelecionada, cenarioDTO, alocacaoManualPresenter.getDisplay().getDemanda(), new AsyncCallback< ParDTO<TurmaDTO, List<AulaDTO>> >()
							{
								@Override
								public void onFailure( Throwable caught )
								{
									MessageBox.alert( "ERRO!", "Não foi possível selecionar a turma", null );
								}

								@Override
								public void onSuccess( ParDTO<TurmaDTO, List<AulaDTO>> result )
								{
									AulaDTO aulaDestaque = aulaDTO;
									for (AulaDTO aula : result.getSegundo())
									{
										if (aula.getHorarioAulaId().equals(aulaDTO.getHorarioAulaId()) && aula.getSemana().equals(aulaDTO.getSemana()))
											aulaDestaque = aula;
									}
									alocacaoManualPresenter.getDisplay().setTurmaSelecionada(result.getPrimeiro(), result.getSegundo(), turmaSelecionada.getStatus());
									alocacaoManualPresenter.getDisplay().setAulaNaGrade(aulaDestaque);
									alocacaoManualPresenter.getDisplay().refreshTurmaSelecionadaPanel();
									alocacaoManualPresenter.getDisplay().getAlunosGrid().updateList();
									alocacaoManualPresenter.getDisplay().getSalaGridPanel().getFiltro().setSalaCodigo(aulaDTO.getSalaString());
									alocacaoManualPresenter.getDisplay().getSalaGridPanel().setAulaDestaque(aulaDestaque);
									alocacaoManualPresenter.getDisplay().getSalaGridPanel().requestAtendimentos();
									alocacaoManualPresenter.addAulasButtonsListeners();
									if (alocacaoManualPresenter.getDisplay().getProfessoresGrid().isRendered())
										alocacaoManualPresenter.getDisplay().getProfessoresGrid().updateList();
								}
							});
							display.getSimpleModal().hide();
							Info.display( "Salvo", "Item salvo com sucesso!" );
						}
					});
				}
				else if(tipoModal.equals(TipoModal.ALUNO))
				{
					final int noAlunosAtuais = alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getNoAlunos();
					service.alocaAlunosTurma(cenarioDTO, alocacaoManualPresenter.getDisplay().getDemanda(), alocacaoManualPresenter.getDisplay().getTurmaSelecionada(),
							alocacaoManualPresenter.getAlunosStatusModificados(), new AsyncCallback< Void >()
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
							turmaSelecionada.setDisciplinaId(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getDisciplinaId());
							turmaSelecionada.setId(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getId());
							turmaSelecionada.setInstituicaoEnsinoId(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getInstituicaoEnsinoId());
							turmaSelecionada.setNome(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getNome());
							turmaSelecionada.setTurma(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getId() == null ? alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getNome() : null);
							turmaSelecionada.setStatus(alocacaoManualPresenter.getDisplay().getTurmaSelecionadaStatus());
							
							service.selecionarTurma(turmaSelecionada, cenarioDTO, alocacaoManualPresenter.getDisplay().getDemanda(), new AsyncCallback< ParDTO<TurmaDTO, List<AulaDTO>> >()
							{
								@Override
								public void onFailure( Throwable caught )
								{
									MessageBox.alert( "ERRO!", "Não foi possível selecionar a turma", null );
								}

								@Override
								public void onSuccess( ParDTO<TurmaDTO, List<AulaDTO>> result )
								{
									alocacaoManualPresenter.getDisplay().getAlunosGrid().updateList();
									alocacaoManualPresenter.getDisplay().getGrid().updateList();
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
									alocacaoManualPresenter.getDisplay().refreshDemandasPanel(planejadas, naoPlanejadas, 0);
									if (alocacaoManualPresenter.getDisplay().getAulaNaGrade() != null)
									{
										alocacaoManualPresenter.getDisplay().getSalaGridPanel().requestAtendimentos();
									}
									alocacaoManualPresenter.getDisplay().setTurmaSelecionada(result.getPrimeiro(), result.getSegundo(), alocacaoManualPresenter.getDisplay().getTurmaSelecionadaStatus());
									alocacaoManualPresenter.getDisplay().refreshTurmaSelecionadaPanel();
									alocacaoManualPresenter.addAulasButtonsListeners();
									Info.display( "Alocados", "Aluno(s) alocado(s) a turma com sucesso!" );
								}
							});
						}
					});
				}
				else if(tipoModal.equals(TipoModal.PROFESSOR))
				{
					List<Record> records = alocacaoManualPresenter.getDisplay().getProfessoresGrid().getGrid().getStore().getModifiedRecords();
					final ProfessorStatusDTO professorStatusDTO = (ProfessorStatusDTO) records.get(0).getModel();
					service.alocaProfessoresAula(cenarioDTO, alocacaoManualPresenter.getDisplay().getDemanda(), alocacaoManualPresenter.getDisplay().getTurmaSelecionada(),
							alocacaoManualPresenter.getDisplay().getAulaNaGrade(), professorStatusDTO, new AsyncCallback< Void >()
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
							turmaSelecionada.setDisciplinaId(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getDisciplinaId());
							turmaSelecionada.setId(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getId());
							turmaSelecionada.setInstituicaoEnsinoId(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getInstituicaoEnsinoId());
							turmaSelecionada.setNome(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getNome());
							turmaSelecionada.setTurma(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getId() == null ? alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getNome() : null);
							turmaSelecionada.setStatus(alocacaoManualPresenter.getDisplay().getTurmaSelecionadaStatus());
							
							service.selecionarTurma(turmaSelecionada, cenarioDTO, alocacaoManualPresenter.getDisplay().getDemanda(), new AsyncCallback< ParDTO<TurmaDTO, List<AulaDTO>> >()
							{
								@Override
								public void onFailure( Throwable caught )
								{
									MessageBox.alert( "ERRO!", "Não foi possível selecionar a turma", null );
								}

								@Override
								public void onSuccess( ParDTO<TurmaDTO, List<AulaDTO>> result )
								{
									alocacaoManualPresenter.getDisplay().setTurmaSelecionada(result.getPrimeiro(), result.getSegundo(), alocacaoManualPresenter.getDisplay().getTurmaSelecionadaStatus());
									if (professorStatusDTO.getProfessorId() != null)
									{
										alocacaoManualPresenter.getDisplay().getAulaNaGrade().setProfessorId(professorStatusDTO.getProfessorId());
									}
									else
									{
										alocacaoManualPresenter.getDisplay().getAulaNaGrade().setProfessorVirtualId(professorStatusDTO.getProfessorVirtualId());
									}
									alocacaoManualPresenter.getDisplay().refreshTurmaSelecionadaPanel();
									alocacaoManualPresenter.addAulasButtonsListeners();
									alocacaoManualPresenter.getDisplay().getProfessoresGrid().updateList();
									alocacaoManualPresenter.getDisplay().getGrid().updateList();
									Info.display( "Alocados", "Professor alocado a aula com sucesso!" );
								}
							});
						}
					});
				}
				else if(tipoModal.equals(TipoModal.TURMA))
				{
					service.salvarTurma(alocacaoManualPresenter.getDisplay().getTurmaSelecionada(), new AsyncCallback< Void >()
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
							turmaSelecionada.setDisciplinaId(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getDisciplinaId());
							turmaSelecionada.setId(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getId());
							turmaSelecionada.setInstituicaoEnsinoId(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getInstituicaoEnsinoId());
							turmaSelecionada.setNome(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getNome());
							turmaSelecionada.setTurma(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getNome());
							turmaSelecionada.setStatus(alocacaoManualPresenter.getDisplay().getTurmaSelecionadaStatus());
							
							service.selecionarTurma(turmaSelecionada, cenarioDTO, alocacaoManualPresenter.getDisplay().getDemanda(), new AsyncCallback< ParDTO<TurmaDTO, List<AulaDTO>> >()
							{
								@Override
								public void onFailure( Throwable caught )
								{
									MessageBox.alert( "ERRO!", "Não foi possível selecionar a turma", null );
								}

								@Override
								public void onSuccess( ParDTO<TurmaDTO, List<AulaDTO>> result )
								{
									alocacaoManualPresenter.getDisplay().getProfessoresGrid().updateList();
									alocacaoManualPresenter.getDisplay().getGrid().updateList();
									alocacaoManualPresenter.getDisplay().setTurmaSelecionada(result.getPrimeiro(), result.getSegundo(), "Não Planejada");
									alocacaoManualPresenter.getDisplay().refreshDemandasPanel(0, -result.getPrimeiro().getNoAlunos(), result.getPrimeiro().getNoAlunos());
									alocacaoManualPresenter.getDisplay().refreshTurmaSelecionadaPanel();
									alocacaoManualPresenter.addAulasButtonsListeners();
									Info.display( "Salvo", "Turma salva com sucesso!" );
								}
							});
						}
					});
				}
			}
		});
	}

	private void populationMessages()
	{
		for ( String msg : this.warnings )
		{
			this.display.getMessagesWarningPanel().addText( "• " + msg );
		}

		for ( String msg : this.errors )
		{
			this.display.getMessagesErrorPanel().addText( "• " + msg );
		}

		this.display.getMessagesWarningPanel().setVisible( !this.warnings.isEmpty() );
		this.display.getMessagesErrorPanel().setVisible( !this.errors.isEmpty() );
	}

	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
