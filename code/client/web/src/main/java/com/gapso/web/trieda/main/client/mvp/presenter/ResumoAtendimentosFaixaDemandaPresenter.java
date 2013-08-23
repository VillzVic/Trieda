package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.view.ResumoAtendimentosFaixaDemandaFormView;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaDemandaDTO;
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

public class ResumoAtendimentosFaixaDemandaPresenter 
	implements Presenter
{
		public interface Display
		extends ITriedaI18nGateway
	{
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		Button getEditButton();
		Grid< AtendimentoFaixaDemandaDTO > getGrid();
		ListStore< AtendimentoFaixaDemandaDTO > getStore();
		Component getComponent();
		CampusComboBox getCampusComboBox();
	}
	
	private Display display;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenarioDTO;
	
	public ResumoAtendimentosFaixaDemandaPresenter(
			InstituicaoEnsinoDTO instituicaoEnsinoDTO,
			CenarioDTO cenarioDTO, Display display )
	{
			this.display = display;
			this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
			this.cenarioDTO = cenarioDTO;
	
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
				Services.alunosDemanda().getResumoFaixaDemandaList( cenarioDTO, se.getSelectedItem(), null,
				new AbstractAsyncCallbackWithDefaultOnFailure< List < AtendimentoFaixaDemandaDTO > >( display )
				{
					@Override
					public void onSuccess(
						List< AtendimentoFaixaDemandaDTO > list )
						{
							display.getStore().removeAll();
							display.getStore().add( list );
							display.getGrid().unmask();
							display.getEditButton().setEnabled(true);
						}
				});
			}
		});
		
		display.getEditButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new ResumoAtendimentosFaixaDemandaFormPresenter( cenarioDTO, new ResumoAtendimentosFaixaDemandaFormView(),
						display.getGrid(), display.getCampusComboBox().getValue());

				presenter.go( null );
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
					ExcelInformationType.ATENDIMENTOS_FAIXA_DEMANDA, instituicaoEnsinoDTO, fileExtension );

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
					ExcelInformationType.ATENDIMENTOS_FAIXA_DEMANDA, instituicaoEnsinoDTO, fileExtension );

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
	
	@Override
	public void go(Widget widget)
	{
		GTab tab = (GTab)widget;
		tab.add( (GTabItem) this.display.getComponent() );
	}
}

