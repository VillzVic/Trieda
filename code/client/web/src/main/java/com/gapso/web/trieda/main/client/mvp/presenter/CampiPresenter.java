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
import com.gapso.web.trieda.main.client.mvp.view.CampusFormView;
import com.gapso.web.trieda.main.client.mvp.view.HorarioDisponivelCampusFormView;
import com.gapso.web.trieda.main.client.mvp.view.UnidadesDeslocamentoView;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoUnidadeDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CampiServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.services.UnidadesServiceAsync;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.EstadoComboBox;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CampiPresenter
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
		Button getUnidadeDeslocamentosButton();
		Button getDisponibilidadeButton();
		TextField< String > getCodigoBuscaTextField();
		TextField< String > getNomeBuscaTextField();
		EstadoComboBox getEstadoBuscaComboBox();
		TextField< String > getMunicipioBuscaTextField();
		TextField< String > getBairroBuscaTextField();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid< CampusDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< CampusDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenario;
	private Display display;
	private GTab gTab;

	public CampiPresenter(
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
		final CampiServiceAsync service = Services.campi();

		RpcProxy< PagingLoadResult< CampusDTO > > proxy =
			new RpcProxy< PagingLoadResult< CampusDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< CampusDTO > > callback )
			{
				String nome = display.getNomeBuscaTextField().getValue();
				String codigo = display.getCodigoBuscaTextField().getValue();
				String estado = null;
				String municipio = display.getMunicipioBuscaTextField().getValue();
				String bairro = display.getBairroBuscaTextField().getValue();

				service.getBuscaList( cenario, nome, codigo, estado, municipio,
					bairro, (PagingLoadConfig) loadConfig, callback );
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
					Presenter presenter = new CampusFormPresenter( instituicaoEnsinoDTO,
						cenario, new CampusFormView( cenario, new CampusDTO() ), display.getGrid() );

					presenter.go( null );
				}
		});

		this.display.getEditButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					CampusDTO campusDTO = display.getGrid().getGrid()
						.getSelectionModel().getSelectedItem();

					Presenter presenter = new CampusFormPresenter( instituicaoEnsinoDTO, cenario,
						new CampusFormView( cenario, campusDTO ), display.getGrid() );

					presenter.go( null );
				}
		});

		this.display.getRemoveButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final CampiServiceAsync service = Services.campi();

				List< CampusDTO > list
					= display.getGrid().getGrid().getSelectionModel().getSelectedItems();

				service.remove( list,
					new AbstractAsyncCallbackWithDefaultOnFailure< Void >( display )
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!",
							"Não foi possível remover o(s) campus/campi", null );
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

		this.display.getImportExcelButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.CAMPI, instituicaoEnsinoDTO );

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
					ExcelInformationType.CAMPI, instituicaoEnsinoDTO, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );

				e.submit();
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
					ExcelInformationType.CAMPI, instituicaoEnsinoDTO, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );

				e.submit();
			}
		});

		this.display.getUnidadeDeslocamentosButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				UnidadesServiceAsync service = Services.unidades();
				final CampusDTO campusDTO
					= display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				service.getDeslocamento( campusDTO,
					new AbstractAsyncCallbackWithDefaultOnFailure< List< DeslocamentoUnidadeDTO > >( display )
					{
						@Override
						public void onSuccess(
							List<DeslocamentoUnidadeDTO> result )
						{
							Presenter presenter = new UnidadesDeslocamentoPresenter(
								new UnidadesDeslocamentoView( campusDTO, result ) );

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
					final CampusDTO campusDTO
						= display.getGrid().getGrid().getSelectionModel().getSelectedItem();

					Services.campi().getHorariosDisponiveis( campusDTO,
						new AsyncCallback< PagingLoadResult< HorarioDisponivelCenarioDTO > >()
						{
							@Override
							public void onFailure( Throwable caught )
							{
								MessageBox.alert( "ERRO!",
									"Não foi posssível exibir as tela de disponiblidade", null );
							}

							@Override
							public void onSuccess(
								PagingLoadResult< HorarioDisponivelCenarioDTO > result )
							{
								Presenter presenter = new HorarioDisponivelCampusFormPresenter(
									instituicaoEnsinoDTO, cenario,
									new HorarioDisponivelCampusFormView( campusDTO, result.getData() ) );

								presenter.go( null );
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
					display.getCodigoBuscaTextField().setValue( null );
					display.getEstadoBuscaComboBox().setValueField( null );
					display.getMunicipioBuscaTextField().setValue( null );
					display.getBairroBuscaTextField().setValue( null );

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
