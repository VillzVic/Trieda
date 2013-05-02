package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.PercentMestresDoutoresDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.ui.Widget;

public class ResumoPercentMestresDoutoresPresenter 
	implements Presenter {

	public interface Display extends ITriedaI18nGateway
	{
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		Grid< PercentMestresDoutoresDTO > getGrid();
		ListStore< PercentMestresDoutoresDTO > getStore();
		Component getComponent();
		CampusComboBox getCampusComboBox();
	}
	
	private Display display;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	
	public ResumoPercentMestresDoutoresPresenter(
			InstituicaoEnsinoDTO instituicaoEnsinoDTO,
			CenarioDTO cenario, Display display )
	{
			this.display = display;
			this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
	
			setListeners();
	}
	
	private void setListeners()
	{
		this.display.getCampusComboBox().addSelectionChangedListener(
				new SelectionChangedListener< CampusDTO >()
		{
			@Override
			public void selectionChanged(
				SelectionChangedEvent< CampusDTO > se )
			{
				if ( se.getSelectedItem() == null )
				{
					return;
				}
				display.getGrid().mask( display.getI18nMessages().loading() );
				Services.atendimentos().getPercentMestresDoutoresList(se.getSelectedItem(),
						new AbstractAsyncCallbackWithDefaultOnFailure< List < PercentMestresDoutoresDTO > >( display )
				{
					@Override
					public void onSuccess(
						List< PercentMestresDoutoresDTO > list )
						{
							display.getGrid().getView().setEmptyText("Nao há informações de docentes para o(s) campus(i)" +
									"selecionado(s) pois o modo de otimização utilizado foi o tático");
							display.getStore().removeAll();
							display.getStore().add( list );
							display.getGrid().unmask();
						}
				});
			}
		});
		
		this.display.getExportXlsExcelButton().addSelectionListener(
				new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{

			}
		});
					
		this.display.getExportXlsxExcelButton().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{

			}
		});
	}
	
	@Override
	public void go(Widget widget)
	{
		GTab tab = (GTab)widget;
		tab.add( (GTabItem) this.display.getComponent() );
	}
}
