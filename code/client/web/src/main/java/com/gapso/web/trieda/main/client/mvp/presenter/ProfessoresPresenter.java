package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.view.GradeHorariaProfessorView;
import com.gapso.web.trieda.main.client.mvp.view.ProfessorFormView;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.HorarioDisponivelProfessorFormPresenter;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.mvp.view.HorarioDisponivelProfessorFormView;
import com.gapso.web.trieda.shared.services.AreasTitulacaoServiceAsync;
import com.gapso.web.trieda.shared.services.ProfessoresServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.AreaTitulacaoComboBox;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.gapso.web.trieda.shared.util.view.OperadorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.TipoContratoComboBox;
import com.gapso.web.trieda.shared.util.view.TitulacaoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class ProfessoresPresenter implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		TextField< String > getCpfBuscaTextField();
		TextField< String > getNomeBuscaTextField();
		TipoContratoComboBox getTipoContratoBuscaComboBox();
		TitulacaoComboBox getTitulacaoBuscaComboBox();
		AreaTitulacaoComboBox getAreaTitulacaoBuscaComboBox();
		NumberField getCargaHorariaMinBuscaField();
		OperadorComboBox getCargaHorariaMinOperadorCB();
		NumberField getCargaHorariaMaxBuscaField();
		OperadorComboBox getCargaHorariaMaxOperadorCB();
		NumberField getNotaDesempenhoBuscaField();
		OperadorComboBox getNotaDesempenhoBuscaOperadorCB();
		NumberField getCargaHorariaAnteriorBuscaField();
		OperadorComboBox getCargaHorariaAnteriorOperadorCB();
		NumberField getCustoCreditoSemanalBuscaField();
		OperadorComboBox getCustoCreditoSemanalBuscaOperadorCB();
		NumberField getMaxDiasSemanaBuscaField();
		OperadorComboBox getMaxDiasSemanaOperadorCB();
		NumberField getMinCreditosSemanaisBuscaField();
		OperadorComboBox getMinCreditosSemanaisOperadorCB();
		NumberField getTotalCreditosSemanaisBuscaField();
		OperadorComboBox getTotalCreditosSemanaisBuscaOperadorCB();
		NumberField getCargaHorariaSemanalBuscaField();
		OperadorComboBox getCargaHorariaSemanalOperadorCB();

		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		Button getDisponibilidadeButton();
		Button getGradeHorariaButton();
		SimpleGrid< ProfessorDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< ProfessorDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenario;
	private Display display;
	private GTab gTab;

	public ProfessoresPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO, Display display )
	{
		this.instituicaoEnsinoDTO  = instituicaoEnsinoDTO;
		this.cenario = cenarioDTO;
		this.display = display;

		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{
		final ProfessoresServiceAsync service = Services.professores();

		RpcProxy< PagingLoadResult< ProfessorDTO > > proxy =
			new RpcProxy< PagingLoadResult< ProfessorDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< ProfessorDTO > > callback)
			{
				String cpf = display.getCpfBuscaTextField().getValue();
				String nome = display.getNomeBuscaTextField().getValue();
				TipoContratoDTO tipoContratoDTO = display.getTipoContratoBuscaComboBox().getValue();
				TitulacaoDTO titulacaoDTO = display.getTitulacaoBuscaComboBox().getValue();
				String operadorCargaHorariaMin = (display.getCargaHorariaMinOperadorCB().getValue()==null)?null: display.getCargaHorariaMinOperadorCB().getValue().getValue().getOperadorSQL();
				Integer cargaHorariaMin = display.getCargaHorariaMinBuscaField().getValue() == null?null:display.getCargaHorariaMinBuscaField().getValue().intValue();
				String operadorCargaHorariaMax = (display.getCargaHorariaMaxOperadorCB().getValue()==null)?null: display.getCargaHorariaMaxOperadorCB().getValue().getValue().getOperadorSQL();
				Integer cargaHorariaMax = display.getCargaHorariaMaxBuscaField().getValue() == null?null:display.getCargaHorariaMaxBuscaField().getValue().intValue();
				String operadorNotaDesempenho = (display.getNotaDesempenhoBuscaOperadorCB().getValue()==null)?null: display.getNotaDesempenhoBuscaOperadorCB().getValue().getValue().getOperadorSQL();
				Double notaDesempenho = display.getNotaDesempenhoBuscaField().getValue() == null?null:display.getNotaDesempenhoBuscaField().getValue().doubleValue();
				String operadorCargaHorariaAnterior = (display.getCargaHorariaAnteriorOperadorCB().getValue()==null)?null: display.getCargaHorariaAnteriorOperadorCB().getValue().getValue().getOperadorSQL();
				Integer cargaHorariaAnterior = display.getCargaHorariaAnteriorBuscaField().getValue() == null?null:display.getCargaHorariaAnteriorBuscaField().getValue().intValue();
				String operadorCustoCreditoSemanal = (display.getCustoCreditoSemanalBuscaOperadorCB().getValue()==null)?null: display.getCustoCreditoSemanalBuscaOperadorCB().getValue().getValue().getOperadorSQL();
				Double custoCreditoSemanal = display.getCustoCreditoSemanalBuscaField().getValue() == null?null:display.getCustoCreditoSemanalBuscaField().getValue().doubleValue();
				String operadorMaxDiasSemana = (display.getMaxDiasSemanaOperadorCB().getValue()==null)?null: display.getMaxDiasSemanaOperadorCB().getValue().getValue().getOperadorSQL();
				Integer maxDiasSemana = display.getMaxDiasSemanaBuscaField().getValue() == null?null:display.getMaxDiasSemanaBuscaField().getValue().intValue();
				String operadorMinCreditosSemanais = (display.getMinCreditosSemanaisOperadorCB().getValue()==null)?null: display.getMinCreditosSemanaisOperadorCB().getValue().getValue().getOperadorSQL();
				Integer minCreditosSemanais = display.getMinCreditosSemanaisBuscaField().getValue() == null?null:display.getMinCreditosSemanaisBuscaField().getValue().intValue();
				String operadorTotalCreditosSemanais = (display.getTotalCreditosSemanaisBuscaOperadorCB().getValue()==null)?null: display.getTotalCreditosSemanaisBuscaOperadorCB().getValue().getValue().getOperadorSQL();
				Long totalCreditosSemanais = display.getTotalCreditosSemanaisBuscaField().getValue() == null?null:display.getTotalCreditosSemanaisBuscaField().getValue().longValue();
				String operadorCargaHorariaSemanal = (display.getCargaHorariaSemanalOperadorCB().getValue()==null)?null: display.getCargaHorariaSemanalOperadorCB().getValue().getValue().getOperadorSQL();
				Long cargaHorariaSemanal = display.getCargaHorariaSemanalBuscaField().getValue() == null?null:display.getCargaHorariaSemanalBuscaField().getValue().longValue();
				
				AreaTitulacaoDTO areaTitulacaoDTO = display.getAreaTitulacaoBuscaComboBox().getValue();

				service.getBuscaList( cenario, cpf,	nome, tipoContratoDTO, titulacaoDTO, areaTitulacaoDTO,
						operadorCargaHorariaMin,cargaHorariaMin,operadorCargaHorariaMax,cargaHorariaMax,operadorNotaDesempenho,notaDesempenho,
						operadorCargaHorariaAnterior,cargaHorariaAnterior,operadorCustoCreditoSemanal,custoCreditoSemanal,
						operadorMaxDiasSemana,maxDiasSemana,operadorMinCreditosSemanais,minCreditosSemanais,
						operadorTotalCreditosSemanais,totalCreditosSemanais,operadorCargaHorariaSemanal,cargaHorariaSemanal,
					 (PagingLoadConfig) loadConfig, callback );
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
				Presenter presenter = new ProfessorFormPresenter( instituicaoEnsinoDTO, cenario,
					new ProfessorFormView( new ProfessorDTO(), null, null, null, cenario ), display.getGrid() );

				presenter.go( null );
			}
		});

		display.getEditButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final ProfessorDTO professorDTO
					= display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				final ProfessoresServiceAsync professoresService = Services.professores();
				final AreasTitulacaoServiceAsync areasTitulacaoService = Services.areasTitulacao();

				final FutureResult< TipoContratoDTO > futureTipoContratoDTO = new FutureResult< TipoContratoDTO >();
				final FutureResult< TitulacaoDTO > futureTitulacaoDTO = new FutureResult< TitulacaoDTO >();
				final FutureResult< AreaTitulacaoDTO > futureAreaTitulacaoDTO = new FutureResult< AreaTitulacaoDTO >();

				professoresService.getTipoContrato(
					professorDTO.getTipoContratoId(), futureTipoContratoDTO );

				professoresService.getTitulacao(
					professorDTO.getTitulacaoId(), futureTitulacaoDTO );

				areasTitulacaoService.getAreaTitulacao(
					professorDTO.getAreaTitulacaoId(), futureAreaTitulacaoDTO );

				FutureSynchronizer synch = new FutureSynchronizer(
					futureTipoContratoDTO, futureTitulacaoDTO, futureAreaTitulacaoDTO );

				synch.addCallback( new AsyncCallback< Boolean >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!", "Deu falha na conexão", null );
					}

					@Override
					public void onSuccess( Boolean result )
					{
						TipoContratoDTO tipoContratoDTO = futureTipoContratoDTO.result();
						TitulacaoDTO titulacaoDTO = futureTitulacaoDTO.result();
						AreaTitulacaoDTO areaTitulacaoDTO = futureAreaTitulacaoDTO.result();

						Presenter presenter = new ProfessorFormPresenter(
							instituicaoEnsinoDTO, cenario,
							new ProfessorFormView( professorDTO, tipoContratoDTO,
							titulacaoDTO, areaTitulacaoDTO, cenario ), display.getGrid() );

						presenter.go( null );
					}
				});
			}
		});

		display.getDisponibilidadeButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final ProfessorDTO professorDTO
					= display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				Services.professores().getHorariosDisponiveis( professorDTO,
					new AsyncCallback< List< HorarioDisponivelCenarioDTO > >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!", "Não foi posssível exibir as tela de disponiblidade", null );
					}

					@Override
					public void onSuccess(
						List< HorarioDisponivelCenarioDTO > result )
					{
						Presenter presenter = new HorarioDisponivelProfessorFormPresenter(
							instituicaoEnsinoDTO, cenario, 
							new HorarioDisponivelProfessorFormView( professorDTO, result ) );

						presenter.go( null );
					}
				});
			}
		});
		
		display.getGradeHorariaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final ProfessorDTO professorDTO
					= display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				Presenter presenter = new GradeHorariaProfessorFormPresenter(
					new GradeHorariaProfessorView( cenario, professorDTO ) );

				presenter.go( null );
			}
		});

		display.getRemoveButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final ProfessoresServiceAsync service = Services.professores();

				List< ProfessorDTO > list
					= display.getGrid().getGrid().getSelectionModel().getSelectedItems();

				service.remove( list, new AsyncCallback< Void >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!", "Não foi possível remover o(s) professor(es)", null );
					}

					@Override
					public void onSuccess( Void result )
					{
						display.getGrid().updateList();

						Info.display( "Removido", "Item removido com sucesso!" );
					}
				});
			}
		});

		display.getImportExcelButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.PROFESSORES, instituicaoEnsinoDTO, cenario );

				ImportExcelFormView importExcelFormView
					= new ImportExcelFormView( parametros, display.getGrid() );

				importExcelFormView.show();
			}
		});

		display.getExportXlsExcelButton().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				String fileExtension = "xls";
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.PROFESSORES, instituicaoEnsinoDTO, cenario, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );

				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
		
		display.getExportXlsxExcelButton().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				String fileExtension = "xlsx";
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.PROFESSORES, instituicaoEnsinoDTO, cenario, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );

				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});

		display.getResetBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				display.getCpfBuscaTextField().setValue( null );
				display.getTipoContratoBuscaComboBox().setValue( null );
				display.getTitulacaoBuscaComboBox().setValue( null );
				display.getAreaTitulacaoBuscaComboBox().setValue( null );
				display.getCargaHorariaMinBuscaField().setValue( null );
				display.getCargaHorariaMinOperadorCB().setValue( null );
				display.getCargaHorariaMaxBuscaField().setValue( null );
				display.getCargaHorariaMaxOperadorCB().setValue( null );
				display.getNotaDesempenhoBuscaField().setValue( null );
				display.getNotaDesempenhoBuscaOperadorCB().setValue( null );
				display.getCargaHorariaAnteriorBuscaField().setValue( null );
				display.getCargaHorariaAnteriorOperadorCB().setValue( null );
				display.getCustoCreditoSemanalBuscaField().setValue( null );
				display.getCustoCreditoSemanalBuscaOperadorCB().setValue( null );
				display.getMaxDiasSemanaBuscaField().setValue( null );
				display.getMaxDiasSemanaOperadorCB().setValue( null );
				display.getMinCreditosSemanaisBuscaField().setValue( null );
				display.getMinCreditosSemanaisOperadorCB().setValue( null );
				display.getTotalCreditosSemanaisBuscaField().setValue( null );
				display.getTotalCreditosSemanaisBuscaOperadorCB().setValue( null );
				display.getCargaHorariaSemanalBuscaField().setValue( null );
				display.getCargaHorariaSemanalOperadorCB().setValue( null );
				display.getGrid().updateList();
			}
		});

		display.getSubmitBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				display.getGrid().updateList();
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
