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
import com.gapso.web.trieda.main.client.mvp.view.CurriculoDisciplinaFormView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CurriculosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CurriculoDisciplinasPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Button getNewButton();
		Button getRemoveButton();
		CurriculoDTO getCurriculoDTO();
		Grid< CurriculoDisciplinaDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< ListLoadResult< CurriculoDisciplinaDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display; 
	private GTab gTab;
	private CenarioDTO cenarioDTO;

	public CurriculoDisciplinasPresenter(
		CenarioDTO cenarioDTO, InstituicaoEnsinoDTO instituicaoEnsinoDTO, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.display = display;
		this.cenarioDTO = cenarioDTO;

		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{
		final CurriculosServiceAsync service = Services.curriculos();

		RpcProxy< ListLoadResult< CurriculoDisciplinaDTO > > proxy =
			new RpcProxy< ListLoadResult< CurriculoDisciplinaDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< ListLoadResult< CurriculoDisciplinaDTO > > callback )
			{
				CurriculoDTO curriculoDTO = display.getCurriculoDTO();
				service.getDisciplinasList( curriculoDTO, callback );
			}
		};

		display.setProxy( proxy );
	}

	private void setListeners()
	{
		display.getNewButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new CurriculoDisciplinaFormPresenter( instituicaoEnsinoDTO,
					new CurriculoDisciplinaFormView( cenarioDTO, new CurriculoDisciplinaDTO(), display.getCurriculoDTO() ), display.getGrid() );

				presenter.go( null );
			}
		});

		display.getRemoveButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final CurriculosServiceAsync service = Services.curriculos();
				List< CurriculoDisciplinaDTO > list
					= display.getGrid().getSelectionModel().getSelectedItems();

				service.removeDisciplina( list,
					new AbstractAsyncCallbackWithDefaultOnFailure< Void >( display )
				{
					@Override
					public void onSuccess( Void result )
					{
						display.getGrid().getStore().getLoader().load();
						Info.display( "Removido", "Item removido com sucesso!" );
					}
				});
			}
		});
	}

	@Override
	public void go( Widget widget )
	{
		gTab = (GTab) widget;
		gTab.add( (GTabItem) display.getComponent() );
	}
}
