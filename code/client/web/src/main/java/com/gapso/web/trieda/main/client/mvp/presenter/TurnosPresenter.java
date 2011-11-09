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
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.mvp.view.TurnoFormView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.services.TurnosServiceAsync;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class TurnosPresenter
	implements Presenter
{
	public interface Display
	{
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		TextField< String > getNomeBuscaTextField();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid< TurnoDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< TurnoDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenario;
	private Display display; 

	public TurnosPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display )
	{
		this.cenario = cenario;
		this.display = display;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;

		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{
		final TurnosServiceAsync service = Services.turnos();

		RpcProxy< PagingLoadResult< TurnoDTO > > proxy
			= new RpcProxy< PagingLoadResult< TurnoDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< TurnoDTO > > callback )
			{
				String nome = display.getNomeBuscaTextField().getValue();
				service.getBuscaList( nome, (PagingLoadConfig) loadConfig, callback );
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
				Presenter presenter = new TurnoFormPresenter( instituicaoEnsinoDTO, cenario,
					new TurnoFormView( new TurnoDTO() ), display.getGrid() );

				presenter.go( null );
			}
		});

		this.display.getEditButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				TurnoDTO turnoDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Presenter presenter = new TurnoFormPresenter( instituicaoEnsinoDTO,
					cenario, new TurnoFormView( turnoDTO ), display.getGrid() );

				presenter.go( null );
			}
		});

		this.display.getRemoveButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				List< TurnoDTO > list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final TurnosServiceAsync service = Services.turnos();

				service.remove(list, new AsyncCallback< Void >()
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

						Info.display( "Removido",
							"Item removido com sucesso!" );
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
