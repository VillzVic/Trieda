package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.services.ProfessoresServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class ProfessoresDeslocamentoCampiListPresenter
	extends ProfessoresListPresenter 
{

	public ProfessoresDeslocamentoCampiListPresenter(
			InstituicaoEnsinoDTO instituicaoEnsinoDTO, Long campusDTO,
			CenarioDTO cenarioDTO, Display display) {
		super(instituicaoEnsinoDTO, cenarioDTO, campusDTO, display);
	}

	@Override
	protected void configureProxy() {
		final ProfessoresServiceAsync service = Services.professores();

		RpcProxy< PagingLoadResult< ProfessorDTO > > proxy =
			new RpcProxy< PagingLoadResult< ProfessorDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< ProfessorDTO > > callback)
			{
				String cpf = display.getCpfBuscaTextField().getValue();

				display.getProfessorFiltro().setTipoContrato(display.getTipoContratoBuscaComboBox().getValue());
				display.getProfessorFiltro().setTitulacao(display.getTitulacaoBuscaComboBox().getValue());
				display.getProfessorFiltro().setAreaTitulacao(display.getAreaTitulacaoBuscaComboBox().getValue());

				service.getBuscaListDeslocamentoCampi(cenario, cpf, campusDTO, display.getProfessorFiltro(),
						(PagingLoadConfig) loadConfig, callback);
			}
		};

		display.setProxy( proxy );
	}

}