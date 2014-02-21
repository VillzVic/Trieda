package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.widget.Component;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.SalaUtilizadaDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.SalasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.RelatorioSalaFiltro;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class SalasUtilizadasPresenter implements Presenter {

	public interface Display extends ITriedaI18nGateway
	{
		Component getComponent();
		SimpleGrid< SalaUtilizadaDTO > getGrid();
		void setProxy(RpcProxy< PagingLoadResult< SalaUtilizadaDTO > > proxy );
	}
	
	private CenarioDTO cenario;
	private RelatorioSalaFiltro salaFiltro;
	private Long campusDTO;
	private Display display;
	private GTab gTab;

	public SalasUtilizadasPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO, Long campusDTO, RelatorioSalaFiltro salaFiltro, Display display )
	{
		this.cenario = cenarioDTO;
		this.display = display;
		this.salaFiltro = salaFiltro;
		this.campusDTO = campusDTO;

		configureProxy();
	}
	
	private void configureProxy()
	{
		final SalasServiceAsync service = Services.salas();

		RpcProxy< PagingLoadResult< SalaUtilizadaDTO > > proxy
			= new RpcProxy<PagingLoadResult< SalaUtilizadaDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< SalaUtilizadaDTO > > callback )
			{
				service.getSalasUtilizadas( cenario, campusDTO, salaFiltro, (BasePagingLoadConfig) loadConfig, callback );
			}
		};

		this.display.setProxy( proxy );
	}

	@Override
	public void go(Widget widget) {
		this.gTab = (GTab) widget;
		this.gTab.add( (GTabItem) this.display.getComponent() );
	}}
