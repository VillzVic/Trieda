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
import com.gapso.web.trieda.main.client.mvp.view.DivisaoCreditosAssociarDisciplinaFormView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.DivisaoCreditoDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.DivisoesCreditosServiceAsync;
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

public class DivisoesCreditoDisciplinaPresenter
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
		SimpleGrid< DivisaoCreditoDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< DivisaoCreditoDTO > > proxy );
	}
	
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenario;
	private Display display; 
	
	public DivisoesCreditoDisciplinaPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenario = cenario;
		this.display = display;
	
		this.configureProxy();
		this.setListeners();
	}
	
	private void configureProxy()
	{
		final DivisoesCreditosServiceAsync service = Services.divisaoCreditos();
	
		RpcProxy< PagingLoadResult< DivisaoCreditoDTO > > proxy
			= new RpcProxy< PagingLoadResult< DivisaoCreditoDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< DivisaoCreditoDTO > > callback )
			{
				service.getListComDisciplinas( cenario, (PagingLoadConfig) loadConfig, callback );
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
				Presenter presenter = new DivisaoCreditosAssociarDisciplinaFormPresenter( instituicaoEnsinoDTO,
					cenario, new DivisaoCreditosAssociarDisciplinaFormView( cenario, new DivisaoCreditoDTO(), null ), display.getGrid() );
	
				presenter.go( null );
			}
		});
	
		this.display.getEditButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final DivisaoCreditoDTO divisaoCreditoDTO
					= display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Services.disciplinas().getDisciplina(divisaoCreditoDTO.getDisciplinaId(), new AsyncCallback<DisciplinaDTO>()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!",
							"Não foi possível obter a disciplina a ser editada.", null );
					}
	
					@Override
					public void onSuccess( DisciplinaDTO result )
					{
						Presenter presenter = new DivisaoCreditosAssociarDisciplinaFormPresenter( instituicaoEnsinoDTO,
								cenario, new DivisaoCreditosAssociarDisciplinaFormView( cenario, divisaoCreditoDTO, result ), display.getGrid() );
				
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
				final DivisoesCreditosServiceAsync service = Services.divisaoCreditos();
	
				List< DivisaoCreditoDTO > list
					= display.getGrid().getGrid().getSelectionModel().getSelectedItems();
	
				service.removeWithDisciplina( list, new AsyncCallback< Void >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!",
							"Não foi possível remover o(s) item(ns) selecionado(s).", null );
					}
	
					@Override
					public void onSuccess( Void result )
					{
						display.getGrid().updateList();
	
						Info.display( "Removido",
							"Item removido com sucesso!" );
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
							ExcelInformationType.DIVISOES_CREDITO_DISCIPLINA, instituicaoEnsinoDTO, cenario );
	
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
						ExcelInformationType.DIVISOES_CREDITO_DISCIPLINA, instituicaoEnsinoDTO, cenario, fileExtension );
	
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
						ExcelInformationType.DIVISOES_CREDITO_DISCIPLINA, instituicaoEnsinoDTO, cenario, fileExtension );
	
					ExportExcelFormSubmit e = new ExportExcelFormSubmit(
						parametros,	display.getI18nConstants(), display.getI18nMessages() );
	
					e.submit();
					new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
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
