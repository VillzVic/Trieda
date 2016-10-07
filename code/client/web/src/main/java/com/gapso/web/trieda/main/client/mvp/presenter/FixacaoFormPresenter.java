package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.dtos.FixacaoDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.OtimizacaoCampusComboBox;
import com.gapso.web.trieda.shared.util.view.OtimizacaoDisciplinasComboBox;
import com.gapso.web.trieda.shared.util.view.OtimizacaoTurmaComboBox;
import com.gapso.web.trieda.shared.util.view.TurmaComboBox;
import com.gapso.web.trieda.shared.util.view.OtimizacaoProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.OtimizacaoSalasComboBox;
import com.gapso.web.trieda.shared.util.view.SemanaLetivaDoCenarioGrid;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.OtimizacaoUnidadeComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class FixacaoFormPresenter
	implements Presenter
{
	public interface Display
	{
		Button getSalvarButton();
		TextField< String > getCodigoTextField();
		TextField< String > getDescricaoTextField();
		OtimizacaoDisciplinasComboBox getDisciplinaComboBox();
		OtimizacaoTurmaComboBox getTurmaComboBox();
		OtimizacaoCampusComboBox getCampusComboBox();
		OtimizacaoUnidadeComboBox getUnidadeComboBox();
		OtimizacaoSalasComboBox getSalaComboBox();
		CheckBox getDiasEHorarios();
		CheckBox getAmbiente();
		FixacaoDTO getFixacaoDTO();
		SemanaLetivaDoCenarioGrid< HorarioDisponivelCenarioDTO > getGrid();
		boolean isValid();
		SimpleModal getSimpleModal();
		OtimizacaoProfessorComboBox getProfessorComboBox();
	}

	private CenarioDTO cenario;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private SimpleGrid< FixacaoDTO > gridPanel;
	private Display display;

	public FixacaoFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display, SimpleGrid< FixacaoDTO > gridPanel )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenario = cenario;
		this.gridPanel = gridPanel;
		this.display = display;

		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{
		RpcProxy< PagingLoadResult< HorarioDisponivelCenarioDTO > > proxy =
			new RpcProxy< PagingLoadResult< HorarioDisponivelCenarioDTO > >()
		{
			@Override
			protected void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< HorarioDisponivelCenarioDTO > > callback )
			{
				DisciplinaDTO disciplinaDTO = display.getDisciplinaComboBox().getValue();
				SalaDTO salaDTO = display.getSalaComboBox().getValue();
				ProfessorDTO professorDTO = display.getProfessorComboBox().getValue();

				Services.fixacoes().getHorariosDisponiveis( professorDTO, disciplinaDTO, salaDTO, callback );
			}
		};

		this.display.getGrid().setProxy( proxy );
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
					List< HorarioDisponivelCenarioDTO > hdcDTOList
						= display.getGrid().getStore().getModels();

					Services.fixacoes().save( getDTO(), hdcDTOList, new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Deu falha na conexão", null );
						}

						@Override
						public void onSuccess( Void result )
						{
							display.getSimpleModal().hide();
							gridPanel.updateList();

							Info.display( "Salvo", "Item salvo com sucesso!" );
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

		this.display.getProfessorComboBox().addSelectionChangedListener(
			new SelectionChangedListener< ProfessorDTO >()
		{
			@Override
			public void selectionChanged( SelectionChangedEvent< ProfessorDTO > se )
			{
				display.getGrid().updateList();
			}
		});

		this.display.getDisciplinaComboBox().addSelectionChangedListener(
			new SelectionChangedListener< DisciplinaDTO >()
		{
			@Override
			public void selectionChanged( SelectionChangedEvent< DisciplinaDTO > se )
			{
				display.getGrid().updateList();
			}
		});

		this.display.getSalaComboBox().addSelectionChangedListener(
			new SelectionChangedListener< SalaDTO >()
		{
			@Override
			public void selectionChanged( SelectionChangedEvent< SalaDTO > se )
			{
				display.getGrid().updateList();
			}
		});
		
	}

	private boolean isValid()
	{
		return this.display.isValid();
	}

	private FixacaoDTO getDTO()
	{
		FixacaoDTO fixacaoDTO = this.display.getFixacaoDTO();

		fixacaoDTO.setInstituicaoEnsinoId( this.instituicaoEnsinoDTO.getId() );
		fixacaoDTO.setCodigo( " " );
		fixacaoDTO.setDescricao( this.display.getDescricaoTextField().getValue() );

		ProfessorDTO professor = this.display.getProfessorComboBox().getValue();

		if ( professor != null )
		{
			fixacaoDTO.setProfessorId( professor.getId() );
			fixacaoDTO.setProfessorString( professor.getNome() );
		}

		DisciplinaDTO disciplina = this.display.getDisciplinaComboBox().getValue();

		if ( disciplina != null )
		{
			fixacaoDTO.setDisciplinaId( disciplina.getId() );
			fixacaoDTO.setDisciplinaString( disciplina.getCodigo() );
		}
		
		AtendimentoOperacionalDTO turma = this.display.getTurmaComboBox().getValue();
		
		if( turma != null )
		{
			fixacaoDTO.setTurmaString( turma.getTurma() );			
		}

		CampusDTO campus = this.display.getCampusComboBox().getValue();

		if ( campus != null )
		{
			fixacaoDTO.setCampusId( campus.getId() );
			fixacaoDTO.setCampusString( campus.getCodigo() );
		}

		UnidadeDTO unidade = this.display.getUnidadeComboBox().getValue();

		if ( unidade != null )
		{
			fixacaoDTO.setUnidadeId( unidade.getId() );
			fixacaoDTO.setUnidadeString( unidade.getCodigo() );
		}

		SalaDTO sala = this.display.getSalaComboBox().getValue();

		if ( sala != null )
		{
			fixacaoDTO.setSalaId( sala.getId() );
			fixacaoDTO.setSalaString( sala.getCodigo() );
		}
		
		fixacaoDTO.setFixaDiaEHorario(this.display.getDiasEHorarios().getValue());
		fixacaoDTO.setFixaAmbiente(this.display.getAmbiente().getValue());

		return fixacaoDTO;
	}
	
	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
