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
import com.gapso.web.trieda.shared.dtos.RelatorioQuantidadeDTO;
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

public class ProfessoresTitulacoesPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		Grid< RelatorioQuantidadeDTO > getGrid();
		ListStore< RelatorioQuantidadeDTO > getStore();
		Component getComponent();
		CampusComboBox getCampusComboBox();
	}
	
	private Display display;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenarioDTO;
	private Long campusDTO;
	private boolean somenteAlocados;
	
	public ProfessoresTitulacoesPresenter(
			InstituicaoEnsinoDTO instituicaoEnsinoDTO,
			CenarioDTO cenarioDTO, Display display )
	{
		this (instituicaoEnsinoDTO, cenarioDTO, null, false, display);
	}
	
	public ProfessoresTitulacoesPresenter(
			InstituicaoEnsinoDTO instituicaoEnsinoDTO,
			CenarioDTO cenarioDTO, Long campusDTO, boolean somenteAlocados, Display display )
	{
			this.display = display;
			this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
			this.cenarioDTO = cenarioDTO;
			this.somenteAlocados = somenteAlocados;
			this.campusDTO = campusDTO;
	
			setListeners();
			setCampus();
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
				Services.professores().getProfessoresTitulacoes( cenarioDTO, se.getSelectedItem(), somenteAlocados,
					new AbstractAsyncCallbackWithDefaultOnFailure< List < RelatorioQuantidadeDTO > >( display )
				{
					@Override
					public void onSuccess(
						List< RelatorioQuantidadeDTO > list )
						{
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
				String fileExtension = "xls";
	
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.PROFESSORES_TITULACOES, instituicaoEnsinoDTO, cenarioDTO, fileExtension );
	
				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );
	
				if ( display.getCampusComboBox() == null
						|| display.getCampusComboBox().getValue() == null
						|| display.getCampusComboBox().getValue().getId() == null )
				{
					return;
				}
	
				String campusId = display.getCampusComboBox().getValue().getId().toString();
				e.addParameter( "campusId", campusId );
	
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
					ExcelInformationType.PROFESSORES_TITULACOES, instituicaoEnsinoDTO, cenarioDTO, fileExtension );
	
				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );
				
				if ( display.getCampusComboBox() == null
						|| display.getCampusComboBox().getValue() == null
						|| display.getCampusComboBox().getValue().getId() == null )
				{
					return;
				}
	
				String campusId = display.getCampusComboBox().getValue().getId().toString();
				e.addParameter( "campusId", campusId );
	
				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
	}
	
	public void setCampus()
	{
		if (campusDTO != null)
		{
			Services.campi().getCampus(campusDTO, new AbstractAsyncCallbackWithDefaultOnFailure< CampusDTO >( display )
			{
				@Override
				public void onSuccess(
						CampusDTO campusDTO )
					{
					display.getCampusComboBox().setValue(campusDTO);
					}
			});
		}
	}
	
	@Override
	public void go(Widget widget)
	{
		GTab tab = (GTab)widget;
		tab.add( (GTabItem) this.display.getComponent() );
	}
}