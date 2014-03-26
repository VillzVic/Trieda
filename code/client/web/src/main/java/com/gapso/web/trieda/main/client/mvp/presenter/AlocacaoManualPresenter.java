package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.mvp.view.NovaTurmaFormView;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaStatusDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AtendimentosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
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
		void setProxy( RpcProxy< ListLoadResult< TurmaStatusDTO > > proxy );
		DemandaDTO getDemanda();
		CampusDTO getCampusDTO();
		DisciplinaDTO getDisciplinaDTO();
		Button getTurmasRemoveButton();
		Button getTurmasNewButton();
		Button getTurmasNewButton2();
		TextField<String> getNovaTurmaNomeTextField();
		void refreshDemandasPanel(int planejado, int naoPlanejado, int naoAtendido);
		void refreshTurmaSelecionadaPanel();
		void setTurmaSelecionada(TurmaDTO turmaDTO, String status);
		Button getSalvarTurmaButton();
		Button getEditarTurmaButton();
		Button getRemoverTurmaButton();
		TurmaDTO getTurmaSelecionada();
		String getTurmaSelecionadaStatus();
		Button getSelecionarTurmaButton();
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
		this.display.getTurmasRemoveButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					final List< TurmaStatusDTO > list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
					final AtendimentosServiceAsync service = Services.atendimentos();
					service.deleteTurmasStatus(cenarioDTO, display.getDemanda(), list, new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Não foi possível remover a(s) Turma(s)", null );
						}

						@Override
						public void onSuccess( Void result )
						{
							display.getGrid().updateList();
							int planejadas = 0;
							int naoPlanejadas = 0;
							for (TurmaStatusDTO turmas : list)
							{
								if (turmas.getStatus().equals("Planejada"))
									planejadas -= turmas.getQtdeTotal();
								else if (turmas.getStatus().equals("Não Planejada"))
									naoPlanejadas -= turmas.getQtdeTotal();
							}
							display.refreshDemandasPanel(planejadas, naoPlanejadas, 0);
							display.getGrid().updateList();
							Info.display( "Removido", "Turma(s) removida(s) com sucesso!" );
						}
					});

				}
			});
		
		this.display.getRemoverTurmaButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					final List< TurmaStatusDTO > list = new ArrayList<TurmaStatusDTO>();
					TurmaStatusDTO turmaStatusDTO = new TurmaStatusDTO();
					turmaStatusDTO.setNome(display.getTurmaSelecionada().getNome());
					turmaStatusDTO.setDisciplinaId(display.getTurmaSelecionada().getDisciplinaId());
					turmaStatusDTO.setStatus(display.getTurmaSelecionadaStatus());

					list.add(turmaStatusDTO);
					final AtendimentosServiceAsync service = Services.atendimentos();
					service.deleteTurmasStatus(cenarioDTO, display.getDemanda(), list, new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Não foi possível remover a(s) Turma(s)", null );
						}

						@Override
						public void onSuccess( Void result )
						{
							display.getGrid().updateList();
							int planejadas = 0;
							int naoPlanejadas = 0;
							for (TurmaStatusDTO turmas : list)
							{
								if (turmas.getStatus().equals("Planejada"))
									planejadas -= turmas.getQtdeTotal();
								else if (turmas.getStatus().equals("Não Planejada"))
									naoPlanejadas -= turmas.getQtdeTotal();
							}
							display.refreshDemandasPanel(planejadas, naoPlanejadas, 0);
							display.getGrid().updateList();
							display.setTurmaSelecionada(null, null);
							display.refreshTurmaSelecionadaPanel();
							Info.display( "Removido", "Turma(s) removida(s) com sucesso!" );
						}
					});

				}
			});
		
		this.display.getTurmasNewButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					Presenter presenter = new NovaTurmaFormPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new NovaTurmaFormView( cenarioDTO, display.getCampusDTO(), display.getDisciplinaDTO(), new TurmaDTO() ), display.getGrid() );

					presenter.go( null );
				}
			});
		
		this.display.getEditarTurmaButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					Presenter presenter = new NovaTurmaFormPresenter( instituicaoEnsinoDTO,
						cenarioDTO, new NovaTurmaFormView( cenarioDTO, display.getCampusDTO(), display.getDisciplinaDTO(), display.getTurmaSelecionada() ), display.getGrid() );

					presenter.go( null );
				}
			});
		
		this.display.getTurmasNewButton2().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					final AtendimentosServiceAsync service = Services.atendimentos();
					if (display.getNovaTurmaNomeTextField().getValue() != null)
					{
						final TurmaDTO turmaDTO = new TurmaDTO();
						turmaDTO.setNome(display.getNovaTurmaNomeTextField().getValue());
						turmaDTO.setParcial(true);
						turmaDTO.setDisciplinaId(display.getDisciplinaDTO().getId());
						turmaDTO.setCampusId(display.getCampusDTO().getId());
						turmaDTO.setCenarioId(cenarioDTO.getId());
						turmaDTO.setInstituicaoEnsinoId(instituicaoEnsinoDTO.getId());
						turmaDTO.setNoAlunos(0);
						turmaDTO.setCredAlocados(0);
						
						service.saveTurma(turmaDTO, new AsyncCallback< Void >()
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
							public void onSuccess( Void result )
							{
								display.getNovaTurmaNomeTextField().setValue(null);
								display.getGrid().updateList();
								display.setTurmaSelecionada(turmaDTO, "Parcial");
								display.refreshTurmaSelecionadaPanel();
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
		
		this.display.getSelecionarTurmaButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					final TurmaStatusDTO turmaSelecionada = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
					final AtendimentosServiceAsync service = Services.atendimentos();
					
					service.selecionarTurma(turmaSelecionada, cenarioDTO, display.getDemanda(), new AsyncCallback< TurmaDTO >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Não foi possível selecionar a turma", null );
						}

						@Override
						public void onSuccess( TurmaDTO result )
						{
							display.setTurmaSelecionada(result, turmaSelecionada.getStatus());
							display.refreshTurmaSelecionadaPanel();
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
 				service.getTurmasStatus(cenarioDTO, display.getDemanda(), callback);
 			}
 		};

 		display.setProxy( proxy );
 	}
	
	@Override
	public void go( Widget widget )
	{
		GTab tab = (GTab) widget;
		tab.add( (GTabItem) this.display.getComponent() );
	}
}
