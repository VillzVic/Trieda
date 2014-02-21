package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.services.ProfessoresServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class ProfessoresMalAlocadosListPresenter
	extends ProfessoresListPresenter 
{

	public ProfessoresMalAlocadosListPresenter(
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

				service.getBuscaListMalAlocados(cenario, cpf, campusDTO, display.getProfessorFiltro(), (PagingLoadConfig) loadConfig, callback);
			}
		};

		display.setProxy( proxy );
	}
	
	protected void setListeners()
	{
		super.setListeners();
		
		this.display.getExportXlsExcelButton().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				String fileExtension = "xls";

				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.PROFESSORES_MAL_ALOCADOS, instituicaoEnsinoDTO, cenario, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );

				String campusId = campusDTO.toString();
				String cursoId = (display.getProfessorFiltro().getCurso() == null ? null : display.getProfessorFiltro().getCurso().getId().toString());
				String turnoId = (display.getProfessorFiltro().getTurno() == null ? null : display.getProfessorFiltro().getTurno().getId().toString());
				String titulacaoId = (display.getProfessorFiltro().getTitulacao() == null ? null : display.getProfessorFiltro().getTitulacao().getId().toString());
				String areaTitulacaoId = (display.getProfessorFiltro().getAreaTitulacao() == null ? null : display.getProfessorFiltro().getAreaTitulacao().getId().toString());
				String tipoContratoId  = (display.getProfessorFiltro().getTipoContrato() == null ? null : display.getProfessorFiltro().getTipoContrato().getId().toString());
				
				e.addParameter( "campusId", campusId );
				e.addParameter( "cursoId", cursoId );
				e.addParameter( "turnoId", turnoId );
				e.addParameter( "titulacaoId", titulacaoId );
				e.addParameter( "areaTitulacaoId", areaTitulacaoId );
				e.addParameter( "tipoContratoId", tipoContratoId );

				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
					
		this.display.getExportXlsxExcelButton().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				String fileExtension = "xlsx";
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.PROFESSORES_MAL_ALOCADOS, instituicaoEnsinoDTO, cenario, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );
				
				String campusId = campusDTO.toString();
				String cursoId = (display.getProfessorFiltro().getCurso() == null ? null : display.getProfessorFiltro().getCurso().getId().toString());
				String turnoId = (display.getProfessorFiltro().getTurno() == null ? null : display.getProfessorFiltro().getTurno().getId().toString());
				String titulacaoId = (display.getProfessorFiltro().getTitulacao() == null ? null : display.getProfessorFiltro().getTitulacao().getId().toString());
				String areaTitulacaoId = (display.getProfessorFiltro().getAreaTitulacao() == null ? null : display.getProfessorFiltro().getAreaTitulacao().getId().toString());
				String tipoContratoId  = (display.getProfessorFiltro().getTipoContrato() == null ? null : display.getProfessorFiltro().getTipoContrato().getId().toString());
				
				e.addParameter( "campusId", campusId );
				e.addParameter( "cursoId", cursoId );
				e.addParameter( "turnoId", turnoId );
				e.addParameter( "titulacaoId", titulacaoId );
				e.addParameter( "areaTitulacaoId", areaTitulacaoId );
				e.addParameter( "tipoContratoId", tipoContratoId );

				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
	}

}