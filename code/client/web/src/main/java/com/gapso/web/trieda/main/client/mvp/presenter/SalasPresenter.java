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
import com.gapso.web.trieda.main.client.mvp.view.AssociarDisciplinasView;
import com.gapso.web.trieda.main.client.mvp.view.DisciplinaAssociarSalasView;
import com.gapso.web.trieda.main.client.mvp.view.DisponibilidadesFormView;
import com.gapso.web.trieda.main.client.mvp.view.HorarioDisponivelSalaFormView;
import com.gapso.web.trieda.main.client.mvp.view.SalaFormView;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisponibilidadeDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.TipoSalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.SalasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.gapso.web.trieda.shared.util.view.OperadorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.TipoSalaComboBox;
import com.gapso.web.trieda.shared.util.view.UnidadeComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class SalasPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		Button getDisciplinasAssociadasButton();
		Button getAssociarDisciplinasButton();
		UnidadeComboBox getUnidadeCB();
		CampusComboBox getCampusCB();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid< SalaDTO > getGrid();
		GTabItem getGTabItem();
		Button getDisponibilidadeButton();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< SalaDTO > > proxy );
		 OperadorComboBox getOperadorCapacidadeInstaladaCB();
		 OperadorComboBox getOperadorCapacidadeMaximaCB();
		 OperadorComboBox getOperadorCustoOperacaoCB();
		 NumberField getCapacidadeInstalada();
		 NumberField  getCapacidadeMaxima();
		 NumberField  getCustoOperacao();
		 TextField<String> getNumeroTF();
		 TextField<String> getDescricaoTF(); 
		 TextField<String> getAndarTF();
		 TextField<String> getCodigoAmbienteTF();
		 TipoSalaComboBox getTipoSalaCB();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display;
	private CenarioDTO cenario;
	private GTab gTab;

	public SalasPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.display = display;
		this.cenario = cenario;

		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{
		final SalasServiceAsync service = Services.salas();

		RpcProxy< PagingLoadResult< SalaDTO > > proxy
			= new RpcProxy<PagingLoadResult< SalaDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< SalaDTO > > callback )
			{
				CampusDTO campusDTO = display.getCampusCB().getValue();
				UnidadeDTO unidadeDTO = display.getUnidadeCB().getValue();
				TipoSalaDTO tipoSalaDTO = display.getTipoSalaCB().getValue();
				String operadorCapacidadeInstalada = (display.getOperadorCapacidadeInstaladaCB().getValue()==null)?null: display.getOperadorCapacidadeInstaladaCB().getValue().getValue().getOperadorSQL();
				Integer capacidadeInstalada = display.getCapacidadeInstalada().getValue() == null?null:display.getCapacidadeInstalada().getValue().intValue();
				String operadorCapacidadeMaxima = (display.getOperadorCapacidadeMaximaCB().getValue()==null)?null: display.getOperadorCapacidadeMaximaCB().getValue().getValue().getOperadorSQL();
				Integer capacidadeMaxima = display.getCapacidadeMaxima().getValue() == null?null:display.getCapacidadeMaxima().getValue().intValue();
				
				String operadorCustoOperacao = (display.getOperadorCustoOperacaoCB().getValue()==null)?null: display.getOperadorCustoOperacaoCB().getValue().getValue().getOperadorSQL();
				Double custoOperacao = display.getCustoOperacao().getValue() == null?null:display.getCustoOperacao().getValue().doubleValue();
				
				String codigo =  display.getCodigoAmbienteTF().getValue();
				String numero =  display.getNumeroTF().getValue();
				String descricao = display.getDescricaoTF().getValue();
				String andar = display.getAndarTF().getValue();
				

				service.getList( cenario,  campusDTO, unidadeDTO,
						tipoSalaDTO, operadorCapacidadeInstalada, capacidadeInstalada,
						operadorCapacidadeMaxima, capacidadeMaxima, operadorCustoOperacao, custoOperacao,
						numero, descricao, andar, codigo,
					(PagingLoadConfig) loadConfig, callback );
			}
		};

		this.display.setProxy( proxy );
	}

	private void setListeners()
	{
		this.display.getNewButton().addSelectionListener(
			new SelectionListener<ButtonEvent>()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new SalaFormPresenter(
					instituicaoEnsinoDTO, new SalaFormView( cenario ), display.getGrid() );

				presenter.go( null );
			}
		});

		this.display.getEditButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final SalaDTO salaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				final FutureResult< CampusDTO > futureCampusDTO = new FutureResult<CampusDTO>();
				final FutureResult<UnidadeDTO> futureUnidadeDTO = new FutureResult<UnidadeDTO>();
				final FutureResult<TipoSalaDTO> futureSalaDTO = new FutureResult<TipoSalaDTO>();

				Services.campi().getCampus( salaDTO.getCampusId(), futureCampusDTO );
				Services.unidades().getUnidade( salaDTO.getUnidadeId(), futureUnidadeDTO );
				Services.salas().getTipoSala( salaDTO.getTipoId(), futureSalaDTO );

				FutureSynchronizer synch = new FutureSynchronizer(
					futureCampusDTO, futureUnidadeDTO, futureSalaDTO );

				synch.addCallback( new AsyncCallback< Boolean >()
				{
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!",
							"Não foi possível editar o ambiente", null );
					}

					public void onSuccess( Boolean result )
					{
						CampusDTO campusDTO = futureCampusDTO.result();
						UnidadeDTO unidadeDTO = futureUnidadeDTO.result();
						TipoSalaDTO tipoSalaDTO = futureSalaDTO.result();

						Presenter presenter = new SalaFormPresenter( instituicaoEnsinoDTO,
							new SalaFormView( salaDTO, campusDTO, unidadeDTO,
							tipoSalaDTO, cenario ), display.getGrid() );

						presenter.go( null );
					}
				});
			}
		});

		this.display.getRemoveButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final SalasServiceAsync service = Services.salas();

				final List< SalaDTO > list
					= display.getGrid().getGrid().getSelectionModel().getSelectedItems();

				service.remove( list,
					new AbstractAsyncCallbackWithDefaultOnFailure< Void >( display )
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!",
							"Não foi possível remover o(s) ambiente(s)", null );
					}

					@Override
					public void onSuccess( Void result )
					{
						display.getGrid().updateList();

						Info.display( display.getI18nConstants().informacao(),
							display.getI18nMessages().sucessoRemoverDoBD( list.toString() ) );
					}
				});
			}
		});

		this.display.getImportExcelButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.SALAS, instituicaoEnsinoDTO, cenario );

				ImportExcelFormView importExcelFormView
					= new ImportExcelFormView( parametros, display.getGrid() );

				importExcelFormView.show();
			}
		});

		this.display.getExportXlsExcelButton().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				String fileExtension = "xls";
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.SALAS, instituicaoEnsinoDTO, cenario, fileExtension );
						
				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );

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
						ExcelInformationType.SALAS, instituicaoEnsinoDTO, cenario, fileExtension );
							
					ExportExcelFormSubmit e = new ExportExcelFormSubmit(
						parametros, display.getI18nConstants(), display.getI18nMessages() );

					e.submit();
					new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
				}
			});

		this.display.getDisponibilidadeButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final SalaDTO salaDTO
					= display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				
				Presenter presenter = new DisponibilidadesFormPresenter(
					instituicaoEnsinoDTO, cenario, salaDTO.getId(), 
					new DisponibilidadesFormView( cenario, salaDTO.getId(), DisponibilidadeDTO.SALA, salaDTO.getCodigo() ) );

				presenter.go( null );
/*
				final FutureResult< UnidadeDTO > futureUnidadeDTO = new FutureResult< UnidadeDTO >();
				final FutureResult< List< HorarioDisponivelCenarioDTO > > futureHorarioDisponivelCenarioDTOList
					= new FutureResult< List< HorarioDisponivelCenarioDTO > >();

				Services.unidades().getUnidade( salaDTO.getUnidadeId(), futureUnidadeDTO );
				Services.salas().getHorariosDisponiveis( salaDTO, futureHorarioDisponivelCenarioDTOList );

				FutureSynchronizer synch = new FutureSynchronizer(
					futureUnidadeDTO, futureHorarioDisponivelCenarioDTOList );

				synch.addCallback( new AsyncCallback< Boolean >()
				{
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!",
							"Não foi posssível exibir as tela de disponiblidade", null );
					}

					public void onSuccess( Boolean result )
					{
						UnidadeDTO unidadeDTO = futureUnidadeDTO.result();

						List< HorarioDisponivelCenarioDTO > horarioDisponivelCenarioDTOList
							= futureHorarioDisponivelCenarioDTOList.result();

						Presenter presenter = new HorarioDisponivelSalaFormPresenter(
							instituicaoEnsinoDTO, unidadeDTO, 
							new HorarioDisponivelSalaFormView( salaDTO, horarioDisponivelCenarioDTOList ) );

						presenter.go( null );
					}
				});*/
			}
		});
		
		this.display.getAssociarDisciplinasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {

			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new AssociarDisciplinasPresenter(
						instituicaoEnsinoDTO, cenario,new AssociarDisciplinasView( cenario ), display.getGrid() );

				presenter.go( null );
				
			}
			
			
		});
		
		this.display.getDisciplinasAssociadasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {

			@Override
			public void componentSelected(ButtonEvent ce) {
				final SalaDTO salaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				
				Presenter presenter = new DisciplinaAssociarSalasPresenter(
						instituicaoEnsinoDTO, cenario,new DisciplinaAssociarSalasView( cenario, salaDTO ) );

				presenter.go( gTab );
				
			}
			
			
		});

		this.display.getResetBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				display.getUnidadeCB().setValue( null );
				display.getCampusCB().setValue( null );
				display.getTipoSalaCB().clearSelections();
				 display.getOperadorCapacidadeInstaladaCB().clearSelections();
				 display.getOperadorCapacidadeMaximaCB().clearSelections();
				 display.getOperadorCustoOperacaoCB().clearSelections();
				 display.getCapacidadeInstalada().setValue(null);
				 display.getCapacidadeMaxima().setValue(null);
				 display.getCustoOperacao().setValue(null);
				 display.getNumeroTF().setValue(null);
				 display.getDescricaoTF().setValue(null);
				 display.getCodigoAmbienteTF().setValue(null);
				 display.getAndarTF().setValue(null);

				display.getGrid().updateList();
			}
		});

		this.display.getSubmitBuscaButton().addSelectionListener(
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
