package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.MessageBoxEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.mvp.view.CenarioClonarFormView;
import com.gapso.web.trieda.main.client.mvp.view.CenarioEditarFormView;
import com.gapso.web.trieda.main.client.mvp.view.SituacaoCadastrosView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CenariosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CenariosPresenter
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getClonarCenarioButton();
		Button getAbrirCenarioButton();
		Button getLimparSolucaoButton();
		Button getProgressBarButton();
		TextField< Integer > getAnoBuscaTextField();
		TextField< Integer > getSemestreBuscaTextField();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid< CenarioDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< CenarioDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display; 
	private GTab gTab;
	private ToolBarPresenter presenter;

	public CenariosPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		ToolBarPresenter presenter, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.display = display;
		this.presenter = presenter;

		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{
		final CenariosServiceAsync service = Services.cenarios();

		RpcProxy< PagingLoadResult< CenarioDTO > > proxy =
			new RpcProxy< PagingLoadResult< CenarioDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< CenarioDTO > > callback )
			{
				Integer ano = display.getAnoBuscaTextField().getValue();
				Integer semestre = display.getSemestreBuscaTextField().getValue();

				service.getBuscaList( ano, semestre,
					(PagingLoadConfig) loadConfig, callback );
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
				Presenter presenter = new CenarioEditarFormPresenter( instituicaoEnsinoDTO,
					new CenarioEditarFormView( new CenarioDTO() ), display.getGrid() );

				presenter.go( null );
			}
		});

		this.display.getEditButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				CenarioDTO cenarioDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				Presenter presenter = new CenarioEditarFormPresenter( instituicaoEnsinoDTO,
					new CenarioEditarFormView( cenarioDTO ), display.getGrid() );

				presenter.go( null );
			}
		});

		this.display.getRemoveButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final CenariosServiceAsync service = Services.cenarios();
				List< CenarioDTO > list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				display.getGrid().mask(display.getI18nMessages().loading(), "loading");

				service.remove( list, new AsyncCallback< Void >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!",
							"Deu falha na conexão", null );
					}

					@Override
					public void onSuccess( Void result )
					{
						display.getGrid().updateList();
						display.getGrid().unmask();
						Info.display( "Removido",
							"Item removido com sucesso!" );
						CenariosServiceAsync service = Services.cenarios();
						service.getMasterData( new AsyncCallback<CenarioDTO>(){

							@Override
							public void onFailure(Throwable caught) {
								MessageBox.alert( "ERRO!",
										"Deu falha na conexão", null );
								display.getGrid().unmask();
							}

							@Override
							public void onSuccess(CenarioDTO result) {
								presenter.changeCenario(result);
								Services.cenarios().setCurrentCenario(result.getId(), new AsyncCallback<Void>(){

									@Override
									public void onFailure(Throwable caught) {
										MessageBox.alert( "ERRO!",
												"Deu falha na conexão", null );
										display.getGrid().unmask();
										
									}

									@Override
									public void onSuccess(Void result) {
										MessageBox.alert( "Contexto modificado!",
												"Contexto alterado para Master Data", null );
										display.getGrid().unmask();
									}
									
								});
							}
							
						});
					}
				});
			}
		});
		
		this.display.getClonarCenarioButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new CenarioClonarFormPresenter( instituicaoEnsinoDTO,
					new CenarioClonarFormView( display.getGrid().getSelectionModel().getSelectedItem() ), display.getGrid() );

				presenter.go( null );
			}
		});

		this.display.getResetBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				display.getAnoBuscaTextField().setValue( null );
				display.getSemestreBuscaTextField().setValue( null );

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

		this.display.getAbrirCenarioButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				CenariosServiceAsync service = Services.cenarios();
				
				CenarioDTO cenarioDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				display.getGrid().mask(display.getI18nMessages().loading(), "loading");
				service.setCurrentCenario(cenarioDTO.getId(), new AsyncCallback<Void>(){

					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert( "ERRO!",
								"Deu falha na conexão", null );
					}

					@Override
					public void onSuccess(Void result) {
						Services.cenarios().getCurrentCenario(new AsyncCallback<CenarioDTO>(){

							@Override
							public void onFailure(Throwable caught) {
								MessageBox.alert( "ERRO!",
										"Deu falha na conexão", null );
								display.getGrid().unmask();
								
							}

							@Override
							public void onSuccess(CenarioDTO result) {
								presenter.changeCenario(result);
								MessageBox.alert( "Contexto modificado!",
										"Contexto alterado para " + result.getNome(), null );
								display.getGrid().unmask();
							}
							
						});
					}
					
				});
			}
		});
		
		this.display.getLimparSolucaoButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					
					MessageBox.confirm("Confirmação","Tem certeza que deseja limpar as soluções do cenário selecionado.", new Listener<MessageBoxEvent>() {
						@Override
						public void handleEvent(MessageBoxEvent be) {
							if(be.getButtonClicked().getHtml().equalsIgnoreCase("yes") ||
									be.getButtonClicked().getHtml().equalsIgnoreCase("sim")) {
								
								CenariosServiceAsync service = Services.cenarios();
								
								CenarioDTO cenarioDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
								display.getGrid().mask();
								service.limpaSolucoesCenario(cenarioDTO, new AsyncCallback<Void>(){

									@Override
									public void onFailure(Throwable caught) {
										MessageBox.alert( "ERRO!",
												"Deu falha na conexão", null );
									}

									@Override
									public void onSuccess(Void result) {
										display.getGrid().unmask();
										Info.display( "Removido",
											"Soluções Limpas com Sucesso!" );
									}
									
								});
								
							}
						}
					});
					
					

				}
			});
		
		this.display.getProgressBarButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				CenarioDTO cenarioDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
	
				Presenter presenter = new SituacaoCadastrosPresenter( instituicaoEnsinoDTO, cenarioDTO,
						new SituacaoCadastrosView( cenarioDTO ) );
				
				presenter.go(gTab);
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
