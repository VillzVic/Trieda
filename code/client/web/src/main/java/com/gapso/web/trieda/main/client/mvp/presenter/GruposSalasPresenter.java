package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.main.client.mvp.view.GrupoSalaAssociarSalaView;
import com.gapso.web.trieda.main.client.mvp.view.GrupoSalaFormView;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.GruposSalasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class GruposSalasPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		Button getAssociarSalasButton();
		SimpleGrid< GrupoSalaDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< GrupoSalaDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display; 
	private CenarioDTO cenario;

	public GruposSalasPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.display = display;
		this.cenario = cenario;

		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final GruposSalasServiceAsync service = Services.gruposSalas();
		RpcProxy<PagingLoadResult<GrupoSalaDTO>> proxy = new RpcProxy<PagingLoadResult<GrupoSalaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<GrupoSalaDTO>> callback) {
				service.getBuscaList(cenario, null, null, null, (PagingLoadConfig)loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners()
	{
		display.getNewButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new GrupoSalaFormPresenter( instituicaoEnsinoDTO,
					new GrupoSalaFormView( new GrupoSalaDTO(), null, null, cenario ), display.getGrid() );

				presenter.go( null );
			}
		});

		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final GrupoSalaDTO grupoSalaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				final FutureResult< CampusDTO > futureCampusDTO = new FutureResult< CampusDTO >();
				final FutureResult< UnidadeDTO > futureUnidadeDTO = new FutureResult< UnidadeDTO >();

				Services.campi().getCampus( grupoSalaDTO.getCampusId(), futureCampusDTO );
				Services.unidades().getUnidade( grupoSalaDTO.getUnidadeId(), futureUnidadeDTO );

				FutureSynchronizer synch = new FutureSynchronizer( futureCampusDTO, futureUnidadeDTO );

				synch.addCallback( new AbstractAsyncCallbackWithDefaultOnFailure< Boolean >( display )
				{
					@Override
					public void onSuccess( Boolean result )
					{
						CampusDTO campusDTO = futureCampusDTO.result();
						UnidadeDTO unidadeDTO = futureUnidadeDTO.result();

						Presenter presenter = new GrupoSalaFormPresenter( instituicaoEnsinoDTO,
							new GrupoSalaFormView( grupoSalaDTO, campusDTO, unidadeDTO, cenario ), display.getGrid() );

						presenter.go( null );
					}
				});
				
			}
		});

		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<GrupoSalaDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				Services.gruposSalas().remove(list, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
					@Override
					public void onSuccess(Void result) {
						display.getGrid().updateList();
						Info.display("Removido", "Item removido com sucesso!");
					}
				});
			}
		});
		display.getAssociarSalasButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				GrupoSalaDTO grupoSalaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Presenter presenter = new GrupoSalaAssociarSalaPresenter(new GrupoSalaAssociarSalaView(grupoSalaDTO), display.getGrid());
				presenter.go(null);
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		GTab tab = (GTab)widget;
		tab.add((GTabItem)display.getComponent());
	}

}
