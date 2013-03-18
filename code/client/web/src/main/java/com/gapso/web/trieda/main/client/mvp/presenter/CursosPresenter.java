
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
import com.gapso.web.trieda.main.client.mvp.view.CurriculosView;
import com.gapso.web.trieda.main.client.mvp.view.CursoFormView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.TipoCursoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CursosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.services.TiposCursosServiceAsync;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.TipoCursoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CursosPresenter
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
		Button getCurriculosButton();
		TextField< String > getNomeBuscaTextField();
		TextField< String > getCodigoBuscaTextField();
		TipoCursoComboBox getTipoCursoBuscaComboBox();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid< CursoDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< CursoDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenario;
	private Display display; 
	private GTab gTab;

	public CursosPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenario = cenario;
		this.display = display;

		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{
		final CursosServiceAsync service = Services.cursos();

		RpcProxy< PagingLoadResult< CursoDTO > > proxy =
			new RpcProxy< PagingLoadResult< CursoDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< CursoDTO > > callback )
			{
				String nome = display.getNomeBuscaTextField().getValue();
				String codigo = display.getCodigoBuscaTextField().getValue();
				TipoCursoDTO tipoCursoDTO = display.getTipoCursoBuscaComboBox().getValue();

				service.getBuscaList( nome, codigo, tipoCursoDTO, (PagingLoadConfig) loadConfig, callback );
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
				Presenter presenter = new CursoFormPresenter( instituicaoEnsinoDTO,
					cenario, new CursoFormView( cenario ), display.getGrid() );

				presenter.go( null );
			}
		});

		this.display.getEditButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final CursoDTO cursoDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				final TiposCursosServiceAsync service = Services.tiposCursos();

				service.getTipoCurso( cursoDTO.getTipoId(), new AsyncCallback< TipoCursoDTO >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						caught.printStackTrace();
					}

					@Override
					public void onSuccess( TipoCursoDTO tipoCursoDTO )
					{
						Presenter presenter = new CursoFormPresenter( instituicaoEnsinoDTO, cenario,
							new CursoFormView( cursoDTO, tipoCursoDTO, cenario ), display.getGrid() );

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
				final CursosServiceAsync service = Services.cursos();
				List< CursoDTO > list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();

				service.remove( list, new AsyncCallback< Void >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!", "Não foi possível remover o(s) curso(s)", null );
					}

					@Override
					public void onSuccess( Void result )
					{
						display.getGrid().updateList();
						Info.display( "Removido", "Item(s) removido(s) com sucesso!" );
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
					ExcelInformationType.CURSOS, instituicaoEnsinoDTO );

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
					ExcelInformationType.CURSOS, instituicaoEnsinoDTO, fileExtension );

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
					ExcelInformationType.CURSOS, instituicaoEnsinoDTO, fileExtension );

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
				display.getNomeBuscaTextField().setValue( null );
				display.getCodigoBuscaTextField().setValue( null );
				display.getTipoCursoBuscaComboBox().setValue( null );

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

		this.display.getCurriculosButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final CursoDTO cursoDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				Presenter presenter = new CurriculosPresenter(
					instituicaoEnsinoDTO, cenario, new CurriculosView( cursoDTO ) );

				presenter.go( gTab );
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
