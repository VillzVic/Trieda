package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.gapso.web.trieda.main.client.mvp.view.AlunosDemandaFormView;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AlunosDemandaServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class AlunosDemandaPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Button getNewButton();
		Button getRemoveButton();
		DemandaDTO getDemandaDTO();
		Grid< AlunoDemandaDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< ListLoadResult< AlunoDemandaDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display; 
	private GTab gTab;
	private CenarioDTO cenarioDTO;

	public AlunosDemandaPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO, CenarioDTO cenarioDTO, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.display = display;
		this.cenarioDTO = cenarioDTO;

		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{
		final AlunosDemandaServiceAsync service = Services.alunosDemanda();

		RpcProxy< ListLoadResult< AlunoDemandaDTO > > proxy =
			new RpcProxy< ListLoadResult< AlunoDemandaDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< ListLoadResult< AlunoDemandaDTO > > callback )
			{
				DemandaDTO demandaDTO = display.getDemandaDTO();
				service.getAlunosDemandaList( demandaDTO, callback );
			}
		};

		this.display.setProxy( proxy );
	}

	private void setListeners()
	{
		this.display.getNewButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new AlunosDemandaFormPresenter( instituicaoEnsinoDTO,
					new AlunosDemandaFormView( cenarioDTO, new AlunoDemandaDTO(), 
						display.getDemandaDTO() ), display.getGrid() );

				presenter.go( null );
			}
		});

		this.display.getRemoveButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final AlunosDemandaServiceAsync service = Services.alunosDemanda();

				List< AlunoDemandaDTO > list
					= display.getGrid().getSelectionModel().getSelectedItems();

				service.removeAlunosDemanda( list,
					new AbstractAsyncCallbackWithDefaultOnFailure< Void >( display )
				{
					@Override
					public void onSuccess( Void result )
					{
						display.getGrid().getStore().getLoader().load();
						Info.display( "Removido", "Item(ns) removido com sucesso!" );
					}
				});
			}
		});
	}

	@Override
	public void go( Widget widget )
	{
		this.gTab = (GTab) widget;
		this.gTab.add( (GTabItem) this.display.getComponent() );
	}
}
