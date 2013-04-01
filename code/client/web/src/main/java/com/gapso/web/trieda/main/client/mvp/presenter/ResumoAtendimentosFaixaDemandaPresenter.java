package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ResumoFaixaDemandaDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.ui.Widget;

public class ResumoAtendimentosFaixaDemandaPresenter 
	implements Presenter
{
		public interface Display
		extends ITriedaI18nGateway
	{
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		Grid< ResumoFaixaDemandaDTO > getGrid();
		ListStore< ResumoFaixaDemandaDTO > getStore();
		Component getComponent();
		CampusComboBox getCampusComboBox();
	}
	
	private Display display;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	
	public ResumoAtendimentosFaixaDemandaPresenter(
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
						Services.alunosDemanda().getResumoFaixaDemandaList(se.getSelectedItem(),
							new AbstractAsyncCallbackWithDefaultOnFailure< List < ResumoFaixaDemandaDTO > >( display )
							{
								@Override
								public void onSuccess(
										List< ResumoFaixaDemandaDTO > list )
								{
									display.getStore().removeAll();
									display.getStore().add( list );
									display.getGrid().unmask();
								}
							});
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

