package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.LoadEvent;
import com.extjs.gxt.ui.client.data.Loader;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.ComponentEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.MessageBoxEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.mvp.presenter.HorarioDisponivelCenarioFormPresenter.Display;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisponibilidadeDTO;
import com.gapso.web.trieda.shared.dtos.HorarioAulaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.SemanasLetivaServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.DisponibilidadesGrid;
import com.gapso.web.trieda.shared.util.view.SemanaLetivaGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.i18n.client.DateTimeFormat;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class DisponibilidadesFormPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Button getSalvarButton();
		Long getEntidadeId();
		String getEntidadeTipo();
		SimpleModal getSimpleModal();
		void setProxy(
			RpcProxy< PagingLoadResult< DisponibilidadeDTO > > proxy );
		ListStore< DisponibilidadeDTO > getStore();
		TextField< String > getHorarioInicioTF();
		TextField< String > getHorarioFimTF();
		Button getAdicionarHorarioBT();
		DisponibilidadesGrid<DisponibilidadeDTO> getGrid();
	}
	
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display;
	private Long entidadeId;
	private CenarioDTO cenarioDTO;

	public DisponibilidadesFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO,
		Long entidadeId, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenarioDTO = cenarioDTO;
		this.entidadeId = entidadeId;
		this.display = display;

		configureProxy();
		setListeners();
	}
	
	private void configureProxy()
	{
		RpcProxy< PagingLoadResult< DisponibilidadeDTO > > proxy
			= new RpcProxy< PagingLoadResult< DisponibilidadeDTO > >()
		{
			@Override
			protected void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< DisponibilidadeDTO > > callback)
			{
				Services.disponibilidades().getDisponibilidades(cenarioDTO, entidadeId, display.getEntidadeTipo(), callback);
			}
		};

		this.display.setProxy( proxy );
		display.getGrid().addListener(Events.Render, new Listener<ComponentEvent>(){
			public void handleEvent(ComponentEvent be){
				display.getStore().getLoader().addListener(Loader.Load, new Listener<LoadEvent>(){
					public void handleEvent(LoadEvent be){
						display.getGrid().enable();
					}
				});
			}
		});
	}
	
	private void setListeners()
	{
		this.display.getSalvarButton().addSelectionListener(
			new SelectionListener< ButtonEvent >(){
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					display.getGrid().disable();
					display.getStore().commitChanges();
					final List<DisponibilidadeDTO> hdcDTOList = display.getStore().getModels();
	
					Services.disponibilidades().saveDisponibilidadesDias( cenarioDTO, hdcDTOList, display.getEntidadeTipo(),
						new AbstractAsyncCallbackWithDefaultOnFailure< Void >( display ){
							@Override
							public void onSuccess(Void result){
								Info.display( "Atualizado", "Horários atualizados com sucesso!" );
		
								display.getSimpleModal().hide();
							}
						}
					);
				}
			}
		);

		this.display.getAdicionarHorarioBT().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Services.disponibilidades().saveDisponibilidade( cenarioDTO, getDisponibilidadeDTO(), display.getEntidadeTipo(),
					new AbstractAsyncCallbackWithDefaultOnFailure< Void >( display )
				{
					@Override
					public void onSuccess( Void result )
					{
						display.getStore().getLoader().load();

						Info.display( "Adicionado",
							"Disponibilidade adicionada com sucesso!" );
					}
				});
			}
		});
	}

	private DisponibilidadeDTO getDisponibilidadeDTO()
	{
		DisponibilidadeDTO dto = new DisponibilidadeDTO();
		
		dto.setInstituicaoEnsinoId( this.instituicaoEnsinoDTO.getId() );
		dto.setHorarioInicioString( this.display.getHorarioInicioTF().getValue() );
		dto.setHorarioFimString( this.display.getHorarioFimTF().getValue() );
		dto.setEntidadeId(entidadeId);


		return dto;
	}

	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}

}
