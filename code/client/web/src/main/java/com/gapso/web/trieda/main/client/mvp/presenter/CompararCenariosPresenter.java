package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.BaseTreeModel;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.TreeStore;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.extjs.gxt.ui.client.widget.treegrid.TreeGrid;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.ui.Widget;

public class CompararCenariosPresenter 
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		void setStore( TreeStore< BaseTreeModel > store );
		TreeStore< BaseTreeModel > getStore();
		List<CenarioDTO> getCenarios();
		TreeGrid< BaseTreeModel > getTree();  
		Component getComponent();
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
	}


	private Display display;
	
	public CompararCenariosPresenter( Display display )
	{
		this.display = display;
		setListeners();
		load();
	}

	private void load() {
		display.getTree().mask( display.getI18nMessages().loading() );
		Services.campi().getCenariosComparados(display.getCenarios(),
			new AbstractAsyncCallbackWithDefaultOnFailure< List< BaseTreeModel > >( display )
			{
				@Override
				public void onSuccess( List< BaseTreeModel > list )
				{
					display.getStore().removeAll();
					display.getStore().add( list, true );
					display.getTree().unmask();
				}
			});
	}

	@Override
	public void go( Widget widget )
	{
		GTab tab = (GTab)widget;
		tab.add( (GTabItem) this.display.getComponent() );
	}
	
	private void setListeners()
	{
		this.display.getExportXlsExcelButton().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected( MenuEvent ce ) {
				String fileExtension = "xls";
				ExcelParametros parametros = new ExcelParametros(ExcelInformationType.RESUMO_CAMPI,null, null, fileExtension);
				ExportExcelFormSubmit e = new ExportExcelFormSubmit(parametros,display.getI18nConstants(),display.getI18nMessages());
				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
		
		this.display.getExportXlsxExcelButton().addSelectionListener(new SelectionListener<MenuEvent>() {
			@Override
			public void componentSelected( MenuEvent ce ) {
				String fileExtension = "xlsx";
				ExcelParametros parametros = new ExcelParametros(ExcelInformationType.RESUMO_CAMPI,null, null, fileExtension);
				ExportExcelFormSubmit e = new ExportExcelFormSubmit(parametros,display.getI18nConstants(),display.getI18nMessages());
				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});	
	}
}
