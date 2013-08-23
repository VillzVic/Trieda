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
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.view.HorarioDisponivelUnidadeFormView;
import com.gapso.web.trieda.main.client.mvp.view.SalasView;
import com.gapso.web.trieda.main.client.mvp.view.UnidadeFormView;
import com.gapso.web.trieda.main.client.mvp.view.UnidadesDeslocamentoView;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CampiServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.services.UnidadesServiceAsync;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class UnidadesPresenter
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
		Button getDeslocamentoUnidadesButton();
		Button getSalasButton();
		Button getDisponibilidadeButton();
		TextField< String > getNomeBuscaTextField();
		TextField< String > getCodigoBuscaTextField();
		CampusComboBox getCampusBuscaComboBox();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid< UnidadeDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< UnidadeDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display; 
	private CenarioDTO cenario; 
	private GTab gTab;
	
	public UnidadesPresenter( InstituicaoEnsinoDTO instituicaoEnsinoDTO,
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
		final UnidadesServiceAsync service = Services.unidades();

		RpcProxy< PagingLoadResult< UnidadeDTO > > proxy
			= new RpcProxy< PagingLoadResult< UnidadeDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< UnidadeDTO > > callback )
			{
				String nome = display.getNomeBuscaTextField().getValue();
				String codigo = display.getCodigoBuscaTextField().getValue();
				CampusDTO campusDTO = display.getCampusBuscaComboBox().getValue();
			
				service.getBuscaList( cenario, campusDTO, nome, codigo, (PagingLoadConfig)loadConfig, callback );
			}
		};

		display.setProxy( proxy );
	}
	
	private void setListeners()
	{
		this.display.getNewButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new UnidadeFormPresenter(
					instituicaoEnsinoDTO, new UnidadeFormView( cenario ), display.getGrid() );

				presenter.go( null );
			}
		});

		this.display.getEditButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final UnidadeDTO unidadeDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				final CampiServiceAsync campus = Services.campi();

				campus.getCampus( unidadeDTO.getCampusId(), new AsyncCallback< CampusDTO >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						caught.printStackTrace();
					}

					@Override
					public void onSuccess( CampusDTO campusDTO )
					{
						Presenter presenter = new UnidadeFormPresenter( instituicaoEnsinoDTO,
							new UnidadeFormView( unidadeDTO, campusDTO, cenario ), display.getGrid() );

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
				final UnidadesServiceAsync service = Services.unidades();

				final List< UnidadeDTO > list
					= display.getGrid().getGrid().getSelectionModel().getSelectedItems();

				service.remove( list, new AbstractAsyncCallbackWithDefaultOnFailure< Void >( display )
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!", "Não foi possível remover a(s) unidade(s)", null );
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
						ExcelInformationType.UNIDADES, instituicaoEnsinoDTO );

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
					ExcelInformationType.UNIDADES, instituicaoEnsinoDTO, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros,	display.getI18nConstants(), display.getI18nMessages() );

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
					ExcelInformationType.UNIDADES, instituicaoEnsinoDTO, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros,	display.getI18nConstants(), display.getI18nMessages() );

				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});

		this.display.getResetBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				display.getNomeBuscaTextField().setValue( null );
				display.getCodigoBuscaTextField().setValue( null );
				display.getCampusBuscaComboBox().setValue( null );

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

		this.display.getSalasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final UnidadeDTO unidadeDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				final CampiServiceAsync campus = Services.campi();

				campus.getCampus( unidadeDTO.getCampusId(), new AsyncCallback< CampusDTO >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						caught.printStackTrace();
					}

					@Override
					public void onSuccess( CampusDTO campusDTO )
					{
						Presenter presenter = new SalasPresenter(
							instituicaoEnsinoDTO, cenario,
							new SalasView( cenario, campusDTO, unidadeDTO ) );

						presenter.go( gTab );
					}
				});
			}
		});

		this.display.getDisponibilidadeButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final UnidadeDTO unidadeDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				final FutureResult< CampusDTO > futureCampusDTO = new FutureResult< CampusDTO >();
				final FutureResult< PagingLoadResult< HorarioDisponivelCenarioDTO > > futureHorarioDisponivelCenarioDTOList
					= new FutureResult< PagingLoadResult< HorarioDisponivelCenarioDTO > >();

				Services.campi().getCampus( unidadeDTO.getCampusId(), futureCampusDTO );
				Services.unidades().getHorariosDisponiveis( unidadeDTO,
					futureHorarioDisponivelCenarioDTOList );

				FutureSynchronizer synch = new FutureSynchronizer(
					futureCampusDTO, futureHorarioDisponivelCenarioDTOList );

				synch.addCallback( new AsyncCallback< Boolean >()
				{
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!",
							"Não foi posssível exibir as tela de disponiblidade", null );
					}

					public void onSuccess( Boolean result )
					{
						CampusDTO campusDTO = futureCampusDTO.result();
						List< HorarioDisponivelCenarioDTO > horarioDisponivelCenarioDTOList
							= futureHorarioDisponivelCenarioDTOList.result().getData();

						Presenter presenter = new HorarioDisponivelUnidadeFormPresenter(
							instituicaoEnsinoDTO, campusDTO, 
							new HorarioDisponivelUnidadeFormView(
								unidadeDTO, horarioDisponivelCenarioDTOList ) );

						presenter.go( null );
					}
				});
			}
		});

		this.display.getDeslocamentoUnidadesButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new UnidadesDeslocamentoPresenter(
					new UnidadesDeslocamentoView( cenario, null, null ) );

				presenter.go( gTab );
			}
		});
	}

	@Override
	public void go( Widget widget )
	{
		this.gTab = (GTab)widget;
		this.gTab.add( (GTabItem) this.display.getComponent() );
	}
}
