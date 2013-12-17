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
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.view.AlunoDemandaView;
import com.gapso.web.trieda.main.client.mvp.view.DemandaFormView;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CampiServiceAsync;
import com.gapso.web.trieda.shared.services.CurriculosServiceAsync;
import com.gapso.web.trieda.shared.services.CursosServiceAsync;
import com.gapso.web.trieda.shared.services.DemandasServiceAsync;
import com.gapso.web.trieda.shared.services.DisciplinasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.services.TurnosServiceAsync;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.DisciplinaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class DemandasPresenter
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
		Button getAssociarAlunosDemanda();
		CampusComboBox getCampusBuscaComboBox();
		CursoComboBox getCursoBuscaComboBox();
		CurriculoComboBox getCurriculoBuscaComboBox();
		TurnoComboBox getTurnoBuscaComboBox();
		DisciplinaAutoCompleteBox getDisciplinaBuscaComboBox();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid< DemandaDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< DemandaDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display; 
	private GTab gTab;
	private CenarioDTO cenarioDTO;

	public DemandasPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.display = display;
		this.cenarioDTO = cenarioDTO;
		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{
		final DemandasServiceAsync service = Services.demandas();

		RpcProxy< PagingLoadResult< DemandaDTO > > proxy =
			new RpcProxy< PagingLoadResult< DemandaDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< DemandaDTO > > callback )
			{
				CampusDTO campus = display.getCampusBuscaComboBox().getValue();
				CursoDTO curso = display.getCursoBuscaComboBox().getValue();
				CurriculoDTO curriculo = display.getCurriculoBuscaComboBox().getValue();
				TurnoDTO turno = display.getTurnoBuscaComboBox().getValue();
				DisciplinaDTO disciplina = display.getDisciplinaBuscaComboBox().getValue();

				service.getBuscaList( cenarioDTO, campus, curso, curriculo, turno,
					disciplina, (PagingLoadConfig) loadConfig, callback );
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
				Presenter presenter = new DemandaFormPresenter( instituicaoEnsinoDTO,
					new DemandaFormView( cenarioDTO, new DemandaDTO(), null, null, null, null, null ), display.getGrid() );

				presenter.go( null );
			}
		});

		this.display.getEditButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final DemandaDTO demandaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				final CampiServiceAsync campiService = Services.campi();
				final CursosServiceAsync cursosService = Services.cursos();
				final CurriculosServiceAsync curriculosService = Services.curriculos();
				final TurnosServiceAsync turnosService = Services.turnos();
				final DisciplinasServiceAsync disciplinasService = Services.disciplinas();

				final FutureResult< CampusDTO > futureCampusDTO = new FutureResult< CampusDTO >();
				final FutureResult< CursoDTO > futureCursoDTO = new FutureResult< CursoDTO >();
				final FutureResult< CurriculoDTO > futureCurriculoDTO = new FutureResult< CurriculoDTO >();
				final FutureResult< TurnoDTO > futureTurnoDTO = new FutureResult< TurnoDTO >();
				final FutureResult< DisciplinaDTO > futureDisciplinaDTO = new FutureResult< DisciplinaDTO >();

				campiService.getCampus( demandaDTO.getCampusId(), futureCampusDTO );
				cursosService.getCurso( demandaDTO.getCursoId(), futureCursoDTO );
				curriculosService.getCurriculo( demandaDTO.getCurriculoId(), futureCurriculoDTO );
				turnosService.getTurno( demandaDTO.getTurnoId(), futureTurnoDTO );
				disciplinasService.getDisciplina( demandaDTO.getDisciplinaId(), futureDisciplinaDTO );

				FutureSynchronizer synch = new FutureSynchronizer( futureCampusDTO, futureCursoDTO,
					futureCurriculoDTO, futureTurnoDTO, futureDisciplinaDTO );

				synch.addCallback( new AbstractAsyncCallbackWithDefaultOnFailure< Boolean >( display )
				{
					@Override
					public void onSuccess( Boolean result )
					{
						CampusDTO campusDTO = futureCampusDTO.result();
						CursoDTO cursoDTO = futureCursoDTO.result();
						CurriculoDTO curriculoDTO = futureCurriculoDTO.result();
						TurnoDTO turnoDTO = futureTurnoDTO.result();
						DisciplinaDTO disciplinaDTO = futureDisciplinaDTO.result();

						Presenter presenter = new DemandaFormPresenter( instituicaoEnsinoDTO,
							new DemandaFormView( cenarioDTO, demandaDTO, campusDTO, cursoDTO, curriculoDTO,
									turnoDTO, disciplinaDTO ), display.getGrid() );

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
				List< DemandaDTO > list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final DemandasServiceAsync service = Services.demandas();

				service.remove( list, new AbstractAsyncCallbackWithDefaultOnFailure< Void >( display )
				{
					@Override
					public void onSuccess( Void result )
					{
						display.getGrid().updateList();
						Info.display( "Removido", "Item removido com sucesso!" );
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
						ExcelInformationType.DEMANDAS, instituicaoEnsinoDTO, cenarioDTO );

				ImportExcelFormView importExcelFormView
					= new ImportExcelFormView( parametros,display.getGrid() );

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
					ExcelInformationType.DEMANDAS, instituicaoEnsinoDTO, cenarioDTO, fileExtension );

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
					ExcelInformationType.DEMANDAS, instituicaoEnsinoDTO, cenarioDTO, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );

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
				display.getCampusBuscaComboBox().setValue( null );
				display.getCursoBuscaComboBox().setValue( null );
				display.getCurriculoBuscaComboBox().setValue( null );
				display.getTurnoBuscaComboBox().setValue( null );
				display.getDisciplinaBuscaComboBox().setValue( null );

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

		this.display.getAssociarAlunosDemanda().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final DemandaDTO demandaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				
				final DemandasServiceAsync service = Services.demandas();
				service.findPeriodo(demandaDTO,new AbstractAsyncCallbackWithDefaultOnFailure<Integer>(display) {
					@Override
					public void onSuccess(Integer result) {
						demandaDTO.setPeriodo(result);
						Presenter presenter = new AlunosDemandaPresenter(instituicaoEnsinoDTO, cenarioDTO, new AlunoDemandaView(demandaDTO));
						presenter.go( gTab );
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
