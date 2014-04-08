package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaStatusDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AtendimentosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.SimpleUnpagedGrid;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class NovaTurmaFormPresenter
implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		Button getSalvarButton();
		TextField< String > getNomeTextField();
		TurmaDTO getTurmaDTO();
		DisciplinaDTO getDisciplinaDTO();
		CampusDTO getCampusDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
		boolean getEdit();
	}
	
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenario;
	private SimpleUnpagedGrid< TurmaStatusDTO > gridPanel;
	private Display display;
	private AlocacaoManualPresenter alocacaoManualPresenter;
	
	public NovaTurmaFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display,
		SimpleUnpagedGrid< TurmaStatusDTO > gridPanel,
		AlocacaoManualPresenter alocacaoManualPresenter)
	{
		this.gridPanel = gridPanel;
		this.display = display;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenario = cenario;
		this.alocacaoManualPresenter = alocacaoManualPresenter;
		setListeners();
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
					final AtendimentosServiceAsync service = Services.atendimentos();
					if (!display.getEdit())
					{
						service.saveTurma(getDTO(), new AsyncCallback< TurmaDTO >()
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
								display.getSimpleModal().hide();
								gridPanel.updateList();
								Info.display( "Salvo", "Item salvo com sucesso!" );
							}
						});
					}
					else
					{
						final String turmaEditadaNome = display.getTurmaDTO().getNome();
						service.editTurma(getDTO(), new AsyncCallback< Void >()
						{
							@Override
							public void onFailure( Throwable caught )
							{
								MessageBox.alert("ERRO!", caught.getMessage(), null);
							}
	
							@Override
							public void onSuccess( Void result )
							{
								if (alocacaoManualPresenter.getDisplay().getTurmaSelecionada() != null
										&& alocacaoManualPresenter.getDisplay().getTurmaSelecionada().getNome().equals(turmaEditadaNome))
								{
									alocacaoManualPresenter.getDisplay().getTurmaSelecionada().setNome(getDTO().getNome());
									alocacaoManualPresenter.getDisplay().refreshTurmaSelecionadaPanel();
									alocacaoManualPresenter.addAulasButtonsListeners();
								}
								display.getSimpleModal().hide();
								gridPanel.updateList();
								Info.display( "Salvo", "Item salvo com sucesso!" );
							}
						});
					}
				}
				else
				{
					MessageBox.alert( "ERRO!",
						"Verifique os campos digitados", null );
				}
			}
		});
	}
	
	private boolean isValid()
	{
		return this.display.isValid();
	}
	
	private TurmaDTO getDTO()
	{
		TurmaDTO turmaDTO = this.display.getTurmaDTO();
	
		turmaDTO.setInstituicaoEnsinoId( this.instituicaoEnsinoDTO.getId() );
		turmaDTO.setCenarioId( this.cenario.getId() );
		turmaDTO.setNome( this.display.getNomeTextField().getValue() );
		turmaDTO.setDisciplinaId(display.getDisciplinaDTO().getId());
		turmaDTO.setCampusId(display.getCampusDTO().getId());
		turmaDTO.setParcial( display.getTurmaDTO().getParcial() == null ? true : display.getTurmaDTO().getParcial());
		
	
		return turmaDTO;
	}
	
	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
