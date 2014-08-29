package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.mvp.view.InstituicaoEnsinoFormView;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.InstituicaoEnsinoServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class InstituicoesEnsinoPresenter
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		TextField< String > getNomeBuscaTextField();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid< InstituicaoEnsinoDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< InstituicaoEnsinoDTO > > proxy );
	}
	
	private Display display; 
	
	public InstituicoesEnsinoPresenter( Display display )
	{
		this.display = display;
	
		configureProxy();
		setListeners();
	}
	
	private void configureProxy()
	{
		final InstituicaoEnsinoServiceAsync service = Services.instituicoesEnsino();
	
		RpcProxy< PagingLoadResult< InstituicaoEnsinoDTO > > proxy
			= new RpcProxy< PagingLoadResult< InstituicaoEnsinoDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< InstituicaoEnsinoDTO > > callback )
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
				Presenter presenter = new InstituicaoEnsinoFormPresenter( 
						new InstituicaoEnsinoFormView( new InstituicaoEnsinoDTO() ), display.getGrid() );
	
				presenter.go( null );
			}
		});
	
		this.display.getEditButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				InstituicaoEnsinoDTO instituicaoEnsinoDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Presenter presenter = new InstituicaoEnsinoFormPresenter( 
						new InstituicaoEnsinoFormView( instituicaoEnsinoDTO ), display.getGrid() );
	
				presenter.go( null );
			}
		});
	
		this.display.getRemoveButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
			    List< InstituicaoEnsinoDTO > list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final InstituicaoEnsinoServiceAsync service = Services.instituicoesEnsino();
	
				service.remove(list, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display.getI18nMessages().erroAoRemover("Instituicao de Ensino"),display)
				{
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
