package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.HorarioAulaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.i18n.client.DateTimeFormat;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class HorarioDisponivelCenarioFormPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Button getSalvarButton();
		SemanaLetivaDTO getSemanaLetivaDTO();
		SimpleModal getSimpleModal();
		void setProxy( RpcProxy< PagingLoadResult< HorarioDisponivelCenarioDTO > > proxy );
		ListStore< HorarioDisponivelCenarioDTO > getStore();
		Button getAdicionarHorarioBT();
		Button getRemoverHorarioBT();
		TurnoComboBox getTurnoCB();
		TextField< String > getHorarioInicioTF();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display;
	private SemanaLetivaDTO semanaLetiva;

	public HorarioDisponivelCenarioFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		SemanaLetivaDTO semanaLetiva, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.semanaLetiva = semanaLetiva;
		this.display = display;

		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{
		RpcProxy< PagingLoadResult< HorarioDisponivelCenarioDTO > > proxy
			= new RpcProxy< PagingLoadResult< HorarioDisponivelCenarioDTO > >()
		{
			@Override
			protected void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< HorarioDisponivelCenarioDTO > > callback)
			{
				Services.semanasLetiva().getHorariosDisponiveisCenario( getDTO(), callback );
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
				List< HorarioDisponivelCenarioDTO > hdcDTOList = display.getStore().getModels();

				Services.semanasLetiva().saveHorariosDisponiveisCenario( getDTO(), hdcDTOList,
					new AbstractAsyncCallbackWithDefaultOnFailure< Void >( display )
				{
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

		this.display.getAdicionarHorarioBT().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Services.horariosAula().save( getHorarioAulaDTO(),
					new AbstractAsyncCallbackWithDefaultOnFailure< Void >( display )
				{
					@Override
					public void onSuccess( Void result )
					{
						display.getStore().getLoader().load();

						Info.display( "Adicionado",
							"Horário adicionado com sucesso!" );
					}
				});
			}
		});

		this.display.getRemoverHorarioBT().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Services.horariosAula().removeWithHorario( getHorarioAulaDTO(),
					new AbstractAsyncCallbackWithDefaultOnFailure< Void >( display )
				{
					@Override
					public void onSuccess( Void result )
					{
						display.getStore().getLoader().load();

						Info.display( "Removido",
							"Horário removido com sucesso!" );
					}
				});
			}
		});
	}

	private SemanaLetivaDTO getDTO()
	{
		SemanaLetivaDTO dto = this.display.getSemanaLetivaDTO();
		dto.setInstituicaoEnsinoId( this.semanaLetiva.getInstituicaoEnsinoId() );
		return dto;
	}

	private HorarioAulaDTO getHorarioAulaDTO()
	{
		HorarioAulaDTO dto = new HorarioAulaDTO();

		dto.setInstituicaoEnsinoId( this.instituicaoEnsinoDTO.getId() );
		dto.setSemanaLetivaId( this.semanaLetiva.getId() );
		dto.setTurnoId( this.display.getTurnoCB().getSelection().get( 0 ).getId() );

		DateTimeFormat df = DateTimeFormat.getFormat( "HH:mm" );
		dto.setInicio( df.parse( this.display.getHorarioInicioTF().getValue() ) );

		return dto;
	}

	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
