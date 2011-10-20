package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class HorarioDisponivelDisciplinaFormPresenter
	implements Presenter
{
	public interface Display
	{
		Button getSalvarButton();
		SimpleModal getSimpleModal();
		DisciplinaDTO getDisciplinaDTO();
		void setProxy( RpcProxy< PagingLoadResult< HorarioDisponivelCenarioDTO > > proxy );
		ListStore< HorarioDisponivelCenarioDTO > getStore();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display;

	public HorarioDisponivelDisciplinaFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO, CenarioDTO cenario,
		SemanaLetivaDTO semanaLetivaDTO, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
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
				Services.semanasLetiva().getAllHorariosDisponiveisCenario( callback );
			}
		};

		this.display.setProxy( proxy );
	}

	private void setListeners()
	{
		this.display.getSalvarButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				display.getStore().commitChanges();
				List< HorarioDisponivelCenarioDTO > hdcDTOList
					= display.getStore().getModels();

				Services.disciplinas().saveHorariosDisponiveis( getDTO(),
					hdcDTOList, new AsyncCallback< Void >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						caught.printStackTrace();
					}

					@Override
					public void onSuccess( Void result )
					{
						Info.display( "Atualizado",
							"Horários atualizados com sucesso!" );

						display.getSimpleModal().hide();
					}
				});
			}
		});
	}

	private DisciplinaDTO getDTO()
	{
		DisciplinaDTO dto = this.display.getDisciplinaDTO();
		dto.setInstituicaoEnsinoId( this.instituicaoEnsinoDTO.getId() );
		return dto;
	}

	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
