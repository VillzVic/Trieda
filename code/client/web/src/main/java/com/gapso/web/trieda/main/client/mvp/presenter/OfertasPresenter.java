package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.main.client.mvp.view.OfertaFormView;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CampiServiceAsync;
import com.gapso.web.trieda.shared.services.CurriculosServiceAsync;
import com.gapso.web.trieda.shared.services.OfertasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.services.TurnosServiceAsync;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class OfertasPresenter
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
		TurnoComboBox getTurnoBuscaComboBox();
		CampusComboBox getCampusBuscaComboBox();
		CursoComboBox getCursoBuscaComboBox();
		CurriculoComboBox getCurriculoBuscaComboBox();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid< OfertaDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< OfertaDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display; 

	public OfertasPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.display = display;

		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{
		final OfertasServiceAsync service = Services.ofertas();

		RpcProxy< PagingLoadResult< OfertaDTO > > proxy =
			new RpcProxy< PagingLoadResult< OfertaDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< OfertaDTO > > callback )
			{
				TurnoDTO turnoDTO = display.getTurnoBuscaComboBox().getValue();
				CampusDTO campusDTO = display.getCampusBuscaComboBox().getValue();
				CursoDTO cursoDTO = display.getCursoBuscaComboBox().getValue();
				CurriculoDTO curriculoDTO = display.getCurriculoBuscaComboBox().getValue();

				service.getBuscaList( turnoDTO, campusDTO, cursoDTO,
					curriculoDTO, (PagingLoadConfig) loadConfig, callback );
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
				Presenter presenter = new OfertaFormPresenter( instituicaoEnsinoDTO, 
					new OfertaFormView(null, null, null), display.getGrid() );

				presenter.go( null );
			}
		});

		display.getEditButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final OfertaDTO ofertaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				final TurnosServiceAsync turnosService = Services.turnos();
				final CampiServiceAsync campiService = Services.campi();
				final CurriculosServiceAsync curriculosService = Services.curriculos();

				final FutureResult< TurnoDTO > futureTurnoDTO = new FutureResult< TurnoDTO >();
				final FutureResult< CampusDTO > futureCampusDTO = new FutureResult< CampusDTO >();
				final FutureResult< CurriculoDTO > futureCurriculoDTO = new FutureResult< CurriculoDTO >();

				turnosService.getTurno( ofertaDTO.getTurnoId(), futureTurnoDTO );
				campiService.getCampus( ofertaDTO.getCampusId(), futureCampusDTO );
				curriculosService.getCurriculo( ofertaDTO.getMatrizCurricularId(), futureCurriculoDTO );
				
				FutureSynchronizer synch = new FutureSynchronizer(
					futureTurnoDTO, futureCampusDTO, futureCurriculoDTO );

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
						TurnoDTO turnoDTO = futureTurnoDTO.result();
						CampusDTO campusDTO = futureCampusDTO.result();
						CurriculoDTO curriculoDTO = futureCurriculoDTO.result();

						Presenter presenter = new OfertaFormPresenter( instituicaoEnsinoDTO,
							new OfertaFormView( ofertaDTO, turnoDTO, campusDTO, curriculoDTO ), display.getGrid() );

						presenter.go( null );	
					}
				});
			}
		});

		display.getRemoveButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				List< OfertaDTO > list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final OfertasServiceAsync service = Services.ofertas();

				service.remove( list, new AsyncCallback< Void >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!", "Deu falha na conexão", null );
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
				// ImportExcelFormView importExcelFormView = new ImportExcelFormView(
				// 	ExcelInformationType.DEMANDAS,display.getGrid() );
				// importExcelFormView.show();
			}
		});

		display.getExportExcelButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.DEMANDAS, instituicaoEnsinoDTO );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );

				e.submit();
			}
		});

		display.getResetBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				display.getTurnoBuscaComboBox().setValue( null );
				display.getCampusBuscaComboBox().setValue( null );
				display.getCursoBuscaComboBox().setValue( null );
				display.getCurriculoBuscaComboBox().setValue( null );
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
		GTab gTab = (GTab) widget;
		gTab.add( (GTabItem) display.getComponent() );
	}

}
