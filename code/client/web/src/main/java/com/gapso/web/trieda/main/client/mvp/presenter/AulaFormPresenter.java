package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.BaseEvent;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.RadioGroup;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.gapso.web.trieda.main.client.mvp.presenter.ErrorsWarningsNewAulaPresenter.TipoModal;
import com.gapso.web.trieda.main.client.mvp.view.ErrorsWarningsNewAulaView;
import com.gapso.web.trieda.shared.dtos.AulaDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaStatusDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AtendimentosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.HorarioComboBox;
import com.gapso.web.trieda.shared.util.view.SalaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.SemanaLetivaComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class AulaFormPresenter
	implements Presenter
{

	public interface Display extends ITriedaI18nGateway
	{
		Button getSalvarButton();
		TurmaDTO getTurmaDTO();
		AulaDTO getAulaDTO();
		DisciplinaDTO getDisciplinaDTO();
		CampusDTO getCampusDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
		SalaAutoCompleteBox getSalaComboBox();
		RadioGroup getDiaSemanaRadioGroup();
		SemanaLetivaComboBox getSemanaLetivaComboBox();
		HorarioComboBox getHorarioComboBox();
		RadioGroup getTipoCreditoRadioGroup();
		SimpleComboBox<Integer> getQtdeCreditosComboBox();
	}
	
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenario;
	private Display display;
	private AlocacaoManualPresenter alocacaoManualPresenter;
	private Integer creditosAlocadosPratico;
	private Integer creditosAlocadosTeorico;
	
	public AulaFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Integer creditosAlocadosPratico,
		Integer creditosAlocadosTeorico, Display display, AlocacaoManualPresenter alocacaoManualPresenter)
	{
		this.display = display;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenario = cenario;
		this.creditosAlocadosPratico = creditosAlocadosPratico;
		this.creditosAlocadosTeorico = creditosAlocadosTeorico;
		this.alocacaoManualPresenter = alocacaoManualPresenter;
		setListeners();
		preencheCreditosComboBox();
	}
	
	private void preencheCreditosComboBox()
	{
		if (display.getTipoCreditoRadioGroup().getValue() != null && 
				display.getTipoCreditoRadioGroup().getValue().getBoxLabel().equals("Teórico"))
		{
			for (int i = 1; i <= (display.getDisciplinaDTO().getCreditosTeorico() - creditosAlocadosTeorico + display.getAulaDTO().getCreditosTeoricos()); i++)
			{
				display.getQtdeCreditosComboBox().add(i);
			}
			display.getQtdeCreditosComboBox().setSimpleValue(display.getAulaDTO().getCreditosTeoricos());
		}
		else if (display.getTipoCreditoRadioGroup().getValue() != null && 
				display.getTipoCreditoRadioGroup().getValue().getBoxLabel().equals("Prático"))
		{
			for (int i = 1; i <= (display.getDisciplinaDTO().getCreditosPratico() - creditosAlocadosPratico + display.getAulaDTO().getCreditosPraticos()); i++)
			{
				display.getQtdeCreditosComboBox().add(i);
			}
			display.getQtdeCreditosComboBox().setSimpleValue(display.getAulaDTO().getCreditosPraticos());
		}
	}

	private void setListeners()
	{
		this.display.getSalvarButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				if ( isValid() )
				{
					Services.atendimentos().verificaViabilidadeAula(cenario, display.getTurmaDTO(), getDTO(), new AsyncCallback< TrioDTO<Boolean, List<String>, List<String>> >()
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
										result.getSegundo(), new ErrorsWarningsNewAulaView(), cenario, display.getTurmaDTO(), getDTO(), alocacaoManualPresenter,
										TipoModal.AULA);

								presenter.go( null );
							}
							else
							{
								final AtendimentosServiceAsync service = Services.atendimentos();
								service.saveAula(display.getDisciplinaDTO(), display.getCampusDTO(), display.getTurmaDTO(), getDTO(), new AsyncCallback< TurmaDTO >()
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
										turmaSelecionada.setCenarioId(cenario.getId());
										turmaSelecionada.setDisciplinaId(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getDisciplinaId());
										turmaSelecionada.setId(result.getId());
										turmaSelecionada.setInstituicaoEnsinoId(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getInstituicaoEnsinoId());
										turmaSelecionada.setNome(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getNome());
										turmaSelecionada.setTurma(alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getId() == null ? alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getNome() : null);
										turmaSelecionada.setStatus(result.getParcial() ? "Parcial" : alocacaoManualPresenter.getDisplay().getTurmaSelecionadaStatus());
										service.selecionarTurma(turmaSelecionada, cenario, alocacaoManualPresenter.getDisplay().getDemanda(), new AsyncCallback< ParDTO<TurmaDTO, List<AulaDTO>> >()
										{
											@Override
											public void onFailure( Throwable caught )
											{
												MessageBox.alert( "ERRO!", "Não foi possível selecionar a turma", null );
											}

											@Override
											public void onSuccess( ParDTO<TurmaDTO, List<AulaDTO>> result )
											{
												alocacaoManualPresenter.getDisplay().setTurmaSelecionada(result.getPrimeiro(), result.getSegundo(), turmaSelecionada.getStatus());
												alocacaoManualPresenter.getDisplay().setAulaNaGrade(getDTO());
												alocacaoManualPresenter.getDisplay().refreshTurmaSelecionadaPanel();
												alocacaoManualPresenter.getDisplay().getAlunosGrid().updateList();
												alocacaoManualPresenter.getDisplay().getSalaGridPanel().getFiltro().setSalaCodigo(getDTO().getSalaString());
												alocacaoManualPresenter.getDisplay().getSalaGridPanel().setAulaDestaque(getDTO());
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
						}
					});
				}
				else
				{
					MessageBox.alert( "ERRO!",
						"Verifique os campos digitados", null );
				}
			}
		});
		
		display.getSalaComboBox().addSelectionChangedListener(new SelectionChangedListener<SalaDTO>() {
			@Override
			public void selectionChanged(SelectionChangedEvent<SalaDTO> se)
			{
				if (se.getSelectedItem() != null &&display.getDiaSemanaRadioGroup().getValue() != null && display.getSemanaLetivaComboBox().getValue() != null)
				{
					display.getHorarioComboBox().changeValues(se.getSelectedItem(),
							display.getSemanaLetivaComboBox().getValue(), display.getDiaSemanaRadioGroup().getValue().getBoxLabel());
					display.getHorarioComboBox().enable();
					display.getHorarioComboBox().setValue(null);
				}
				else
				{
					display.getHorarioComboBox().disable();
				}
			}
		});
		
		display.getSemanaLetivaComboBox().addSelectionChangedListener(new SelectionChangedListener<SemanaLetivaDTO>() {
			@Override
			public void selectionChanged(SelectionChangedEvent<SemanaLetivaDTO> se)
			{
				if (se.getSelectedItem() != null &&display.getDiaSemanaRadioGroup().getValue() != null && display.getSalaComboBox().getValue() != null)
				{
					display.getHorarioComboBox().changeValues(display.getSalaComboBox().getValue(),
							se.getSelectedItem(), display.getDiaSemanaRadioGroup().getValue().getBoxLabel());
					display.getHorarioComboBox().enable();
					display.getHorarioComboBox().setValue(null);
				}
				else
				{
					display.getHorarioComboBox().disable();
				}
			}
		});
		
		display.getDiaSemanaRadioGroup().addListener(Events.Change,new Listener<BaseEvent>() {
			@Override
			public void handleEvent(BaseEvent be)
			{
				if (display.getSemanaLetivaComboBox().getValue() != null && display.getDiaSemanaRadioGroup().getValue() != null && display.getSalaComboBox().getValue() != null)
				{
					display.getHorarioComboBox().changeValues(display.getSalaComboBox().getValue(),
							display.getSemanaLetivaComboBox().getValue(), display.getDiaSemanaRadioGroup().getValue().getBoxLabel());
					display.getHorarioComboBox().enable();
					display.getHorarioComboBox().setValue(null);
				}
				else
				{
					display.getHorarioComboBox().disable();
				}
			}
		});
		
		display.getTipoCreditoRadioGroup().addListener(Events.Change,new Listener<BaseEvent>() {
			@Override
			public void handleEvent(BaseEvent be)
			{
				if (display.getTipoCreditoRadioGroup().getValue() != null)
				{
					display.getQtdeCreditosComboBox().enable();
					if (display.getTipoCreditoRadioGroup().getValue().getBoxLabel().equals("Teórico"))
					{
						display.getQtdeCreditosComboBox().setValue(null);
						display.getQtdeCreditosComboBox().removeAll();
						for (int i = 1; i <= display.getDisciplinaDTO().getCreditosTeorico() - creditosAlocadosTeorico; i++)
						{
							display.getQtdeCreditosComboBox().add(i);
						}
					}
					else
					{
						display.getQtdeCreditosComboBox().setValue(null);
						display.getQtdeCreditosComboBox().removeAll();
						for (int i = 1; i <= display.getDisciplinaDTO().getCreditosPratico() - creditosAlocadosPratico; i++)
						{
							display.getQtdeCreditosComboBox().add(i);
						}
					}
				}
				else
				{
					display.getQtdeCreditosComboBox().disable();
				}
			}
		});
	}
	
	private boolean isValid()
	{
		return this.display.isValid();
	}
	
	private AulaDTO getDTO()
	{
		AulaDTO aulaDTO = new AulaDTO();
		
		aulaDTO.setAtendimentosIds(display.getAulaDTO().getAtendimentosIds());
		aulaDTO.setInstituicaoEnsinoId(instituicaoEnsinoDTO.getId());
		aulaDTO.setCenarioId(cenario.getId());
		aulaDTO.setId(display.getAulaDTO().getId());
		aulaDTO.setHorarioAulaId(display.getHorarioComboBox().getValue().getHorarioDeAulaId());
		aulaDTO.setSemanaString(display.getDiaSemanaRadioGroup().getValue().getBoxLabel());
		aulaDTO.setSalaId(display.getSalaComboBox().getValue().getId());
		aulaDTO.setSalaString(display.getSalaComboBox().getValue().getCodigo());
		aulaDTO.setSemana(Integer.valueOf(display.getDiaSemanaRadioGroup().getValue().getItemId()));
		if (display.getTipoCreditoRadioGroup().getValue().getBoxLabel().equals("Teórico"))
		{
			aulaDTO.setCreditosTeoricos(display.getQtdeCreditosComboBox().getSimpleValue());
			aulaDTO.setCreditosPraticos(0);
		}
		else
		{
			aulaDTO.setCreditosTeoricos(0);
			aulaDTO.setCreditosPraticos(display.getQtdeCreditosComboBox().getSimpleValue());
		}
	
		return aulaDTO;
	}
	
	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
