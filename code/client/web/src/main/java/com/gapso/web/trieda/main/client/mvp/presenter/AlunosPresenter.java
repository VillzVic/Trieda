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
import com.gapso.web.trieda.main.client.mvp.presenter.AlocacaoManualDisciplinaFormPresenter.Display;
import com.gapso.web.trieda.main.client.mvp.view.AlunosFormView;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AlunosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.ComboBoxBoolean;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;

public class AlunosPresenter
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		Button getRemoveAllButton();
		TextField< String > getNomeBuscaTextField();
		TextField< String > getMatriculaBuscaTextField();
		ComboBoxBoolean getFormando();
		ComboBoxBoolean getVirtual();
		NumberField getPeriodoField();
		ComboBoxBoolean getCriadoPeloTrieda();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid< AlunoDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< AlunoDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display;
	private CenarioDTO cenarioDTO;
	
	
	
	public AlunosPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO, Display display )
	{
		this.cenarioDTO = cenarioDTO;
		this.display = display;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{
		final AlunosServiceAsync service = Services.alunos();

		RpcProxy< PagingLoadResult< AlunoDTO > > proxy =
			new RpcProxy< PagingLoadResult< AlunoDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< AlunoDTO > > callback )
			{
				String nome = display.getNomeBuscaTextField().getValue();
				String matricula = display.getMatriculaBuscaTextField().getValue();
				Boolean formando = (display.getFormando().getValue()==null)?null:display.getFormando().getValue().getValue().getValue();
				Boolean virtual = (display.getVirtual().getValue()==null)?null:display.getVirtual().getValue().getValue().getValue();
				Boolean criadoPeloTrieda= (display.getCriadoPeloTrieda().getValue()==null)?null:display.getCriadoPeloTrieda().getValue().getValue().getValue();
				Integer periodo = ( display.getPeriodoField().getValue() == null ) ? null : display.getPeriodoField().getValue().intValue();

				service.getBuscaList(cenarioDTO,nome,matricula,formando,periodo,virtual,criadoPeloTrieda,(PagingLoadConfig) loadConfig,callback);
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
				Presenter presenter = new AlunosFormPresenter( instituicaoEnsinoDTO,
					cenarioDTO, new AlunosFormView( cenarioDTO, new AlunoDTO() ), display.getGrid() );

				presenter.go( null );
			}
		});

		this.display.getEditButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				AlunoDTO alunoDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Presenter presenter = new AlunosFormPresenter( instituicaoEnsinoDTO, cenarioDTO,
					new AlunosFormView( cenarioDTO, alunoDTO ), display.getGrid() );

				presenter.go( null );
			}
		});

		this.display.getRemoveButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				List< AlunoDTO > list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final AlunosServiceAsync service = Services.alunos();

				service.removeAlunos( list, new AsyncCallback< Void >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!", "Não foi possível remover o(s) aluno(s)", null );
					}

					@Override
					public void onSuccess( Void result )
					{
						display.getGrid().updateList();
						Info.display( "Removido", "Aluno(s) removido(s) com sucesso!" );
					}
				});
			}
		});
		
		
		this.display.getImportExcelButton().addSelectionListener(new SelectionListener< ButtonEvent >() {
			@Override
			public void componentSelected( ButtonEvent ce ) {
				ExcelParametros parametros = new ExcelParametros(ExcelInformationType.ALUNOS,instituicaoEnsinoDTO, cenarioDTO);
				ImportExcelFormView importExcelFormView = new ImportExcelFormView(parametros,display.getGrid());
				importExcelFormView.show();
			}
		});

		this.display.getExportXlsExcelButton().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected( MenuEvent ce ) {
				String fileExtension = "xls";
				ExcelParametros parametros = new ExcelParametros(ExcelInformationType.ALUNOS,instituicaoEnsinoDTO, cenarioDTO, fileExtension);
				ExportExcelFormSubmit e = new ExportExcelFormSubmit(parametros,display.getI18nConstants(),display.getI18nMessages());
				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
		
		this.display.getExportXlsxExcelButton().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected( MenuEvent ce ) {
				String fileExtension = "xlsx";
				ExcelParametros parametros = new ExcelParametros(ExcelInformationType.ALUNOS,instituicaoEnsinoDTO, cenarioDTO, fileExtension);
				ExportExcelFormSubmit e = new ExportExcelFormSubmit(parametros,display.getI18nConstants(),display.getI18nMessages());
				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
		
		//issue6
				this.display.getRemoveAllButton().addSelectionListener (
								new SelectionListener< ButtonEvent >() 
							{
								@Override
								public void componentSelected( ButtonEvent ce )
								{									 
								final AlunosServiceAsync service = Services.alunos();
								
									service.removeAllAlunos(cenarioDTO, new AsyncCallback< Void >()
									{
										@Override
										public void onFailure( Throwable caught )
										{
											MessageBox.alert( "ERRO!", "Não foi possível remover o(s) aluno(s)", null );
									}

										@Override
									public void onSuccess( Void result )
										{
											display.getGrid().updateList();
											Info.display( "Removido", "Aluno(s) removido(s) com sucesso!" );
										}
									});
								}
							});

		this.display.getResetBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				display.getNomeBuscaTextField().setValue( null );
				display.getMatriculaBuscaTextField().setValue( null );
				display.getFormando().setValue( null );
				display.getVirtual().setValue( null );
				display.getPeriodoField().setValue( null );
				display.getCriadoPeloTrieda().setValue( null );
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
