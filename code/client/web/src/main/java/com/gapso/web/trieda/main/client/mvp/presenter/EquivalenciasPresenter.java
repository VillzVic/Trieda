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
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.view.EquivalenciaFormView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.EquivalenciaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.DisciplinaAutoCompleteBox;
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

public class EquivalenciasPresenter
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
		CampusComboBox getCampusComboBox();
		DisciplinaAutoCompleteBox getDisciplinaComboBox();
		CursoComboBox getCursoComboBox();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid<EquivalenciaDTO> getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< EquivalenciaDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display; 
	private CenarioDTO cenarioDTO;
	
	public EquivalenciasPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.display = display;
		this.cenarioDTO = cenarioDTO;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		RpcProxy<PagingLoadResult<EquivalenciaDTO>> proxy = new RpcProxy<PagingLoadResult<EquivalenciaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<EquivalenciaDTO>> callback) {
				DisciplinaDTO disciplina = display.getDisciplinaComboBox().getValue();
				CursoDTO  curso = display.getCursoComboBox().getValue();
				Services.equivalencias().getBuscaList(cenarioDTO, disciplina, curso, (PagingLoadConfig)loadConfig, callback);
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
				Presenter presenter = new EquivalenciaFormPresenter( instituicaoEnsinoDTO, cenarioDTO,
					new EquivalenciaFormView( cenarioDTO, new EquivalenciaDTO() ), display.getGrid() );

				presenter.go( null );
			}
		});
		
		this.display.getEditButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final EquivalenciaDTO equivalenciaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				final FutureResult< List< CursoDTO > > futureCursosDTO = new FutureResult< List< CursoDTO > >();
				final FutureResult< DisciplinaDTO > futureDisciplinaCursouDTO = new FutureResult< DisciplinaDTO >();
				final FutureResult< DisciplinaDTO > futureDisciplinaEliminaDTO = new FutureResult< DisciplinaDTO >();

				Services.equivalencias().getCursosEquivalencia( equivalenciaDTO, futureCursosDTO );
				Services.disciplinas().getDisciplina( equivalenciaDTO.getCursouId(), futureDisciplinaCursouDTO );
				Services.disciplinas().getDisciplina( equivalenciaDTO.getEliminaId(), futureDisciplinaEliminaDTO );

				FutureSynchronizer synch = new FutureSynchronizer(
					futureCursosDTO, futureDisciplinaCursouDTO, futureDisciplinaEliminaDTO );

				synch.addCallback( new AsyncCallback< Boolean >()
				{
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!",
							"Não foi possível editar a equivalencia", null );
					}

					public void onSuccess( Boolean result )
					{
						List< CursoDTO > cursosDTO = futureCursosDTO.result();
						DisciplinaDTO disciplinaCursouDTO = futureDisciplinaCursouDTO.result();
						DisciplinaDTO disciplinaEliminaDTO = futureDisciplinaEliminaDTO.result();

						Presenter presenter = new EquivalenciaFormPresenter( instituicaoEnsinoDTO, cenarioDTO,
							new EquivalenciaFormView( cenarioDTO, equivalenciaDTO, cursosDTO, disciplinaCursouDTO,
								disciplinaEliminaDTO), display.getGrid() );

						presenter.go( null );
					}
				});
			}
		});

		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<EquivalenciaDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				Services.equivalencias().remove(list, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
					@Override
					public void onSuccess(Void result) {
						display.getGrid().updateList();
						Info.display("Removido", "Item removido com sucesso!");
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
					ExcelInformationType.EQUIVALENCIAS, instituicaoEnsinoDTO );

				ImportExcelFormView importExcelFormView
					= new ImportExcelFormView( parametros,display.getGrid() );

				importExcelFormView.show();
			}
		});
		display.getExportXlsExcelButton().addSelectionListener(new SelectionListener<MenuEvent>(){
			@Override
			public void componentSelected( MenuEvent ce )
			{
				String fileExtension = "xls";
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.EQUIVALENCIAS, instituicaoEnsinoDTO, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );

				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
		display.getExportXlsxExcelButton().addSelectionListener(new SelectionListener<MenuEvent>(){
			@Override
			public void componentSelected( MenuEvent ce )
			{
				String fileExtension = "xlsx";
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.EQUIVALENCIAS, instituicaoEnsinoDTO, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );

				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
		display.getResetBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getCampusComboBox().setValue(null);
				display.getDisciplinaComboBox().setValue(null);
				display.getCursoComboBox().setValue(null);
				display.getGrid().updateList();
			}
		});
		display.getSubmitBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().updateList();
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		GTab tab = (GTab)widget;
		tab.add((GTabItem)display.getComponent());
	}

}
