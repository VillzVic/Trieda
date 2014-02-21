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
import com.gapso.web.trieda.main.client.mvp.view.HorarioDisponivelCenarioFormView;
import com.gapso.web.trieda.main.client.mvp.view.SemanaLetivaFormView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.SemanasLetivaServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class SemanasLetivaPresenter
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		Button getDiasDeAulaButton();
		TextField< String > getCodigoBuscaTextField();
		TextField< String > getDescricaoBuscaTextField();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid< SemanaLetivaDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< SemanaLetivaDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display; 
	private CenarioDTO cenario;

	public SemanasLetivaPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.display = display;
		this.cenario = cenario;

		this.configureProxy();
		this.setListeners();
	}

	private void configureProxy()
	{
		final SemanasLetivaServiceAsync service = Services.semanasLetiva();

		RpcProxy< PagingLoadResult< SemanaLetivaDTO > > proxy
			= new RpcProxy< PagingLoadResult< SemanaLetivaDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< SemanaLetivaDTO > > callback )
			{
				String codigo = display.getCodigoBuscaTextField().getValue();
				String descricao = display.getDescricaoBuscaTextField().getValue();

				service.getBuscaList( cenario, codigo, descricao, (PagingLoadConfig) loadConfig, callback );
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
				Presenter presenter = new SemanaLetivaFormPresenter( instituicaoEnsinoDTO,
					cenario, new SemanaLetivaFormView( cenario ), display.getGrid() );

				presenter.go( null );
			}
		});

		this.display.getEditButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				SemanaLetivaDTO dto = display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				Presenter presenter = new SemanaLetivaFormPresenter( instituicaoEnsinoDTO,
					cenario, new SemanaLetivaFormView( dto, cenario ), display.getGrid() );

				presenter.go( null );
			}
		});

		this.display.getRemoveButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final SemanasLetivaServiceAsync service = Services.semanasLetiva();
				List< SemanaLetivaDTO > list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();

				service.remove( list, new AsyncCallback< Void >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!",
							"Não foi possível remover a(s) semana(s) letiva(s)", null );
					}

					@Override
					public void onSuccess( Void result )
					{
						display.getGrid().updateList();
						Info.display( "Removido", "Ite(s) removido(s) com sucesso!" );
					}
				});
			}
		});
		
		this.display.getImportExcelButton().addSelectionListener(new SelectionListener< ButtonEvent >() {
			@Override
			public void componentSelected( ButtonEvent ce ) {
				ExcelParametros parametros = new ExcelParametros(ExcelInformationType.SEMANA_LETIVA,instituicaoEnsinoDTO, cenario);
				ImportExcelFormView importExcelFormView = new ImportExcelFormView(parametros,display.getGrid());
				importExcelFormView.show();
			}
		});
		
		this.display.getExportXlsExcelButton().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected( MenuEvent ce ) {
				String fileExtension = "xls";
				ExcelParametros parametros = new ExcelParametros(ExcelInformationType.SEMANA_LETIVA, instituicaoEnsinoDTO, cenario, fileExtension);
				ExportExcelFormSubmit e = new ExportExcelFormSubmit(parametros,display.getI18nConstants(),display.getI18nMessages());
				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
		
		this.display.getExportXlsxExcelButton().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected( MenuEvent ce ) {
				String fileExtension = "xlsx";
				ExcelParametros parametros = new ExcelParametros(ExcelInformationType.SEMANA_LETIVA,instituicaoEnsinoDTO, cenario, fileExtension);
				ExportExcelFormSubmit e = new ExportExcelFormSubmit(parametros,display.getI18nConstants(),display.getI18nMessages());
				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});

		this.display.getDiasDeAulaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				SemanaLetivaDTO dto = display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				Presenter presenter = new HorarioDisponivelCenarioFormPresenter(
					instituicaoEnsinoDTO, cenario, dto, new HorarioDisponivelCenarioFormView(cenario, dto ) );

				presenter.go( null );
			}
		});

		this.display.getResetBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				display.getCodigoBuscaTextField().setValue( null );
				display.getDescricaoBuscaTextField().setValue( null );
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
		GTab tab = (GTab) widget;
		tab.add( (GTabItem) this.display.getComponent() );
	}
}
