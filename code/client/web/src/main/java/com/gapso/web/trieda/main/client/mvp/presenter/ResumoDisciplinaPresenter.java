package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.TreeStore;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.extjs.gxt.ui.client.widget.treegrid.TreeGrid;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ResumoDisciplinaDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.ui.Widget;

public class ResumoDisciplinaPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Component getComponent();
		CampusComboBox getCampusComboBox();
		TreeStore< ResumoDisciplinaDTO > getStore();
		TreeGrid< ResumoDisciplinaDTO > getTree();
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenario;
	private Display display;

	public ResumoDisciplinaPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display )
	{
		this.cenario = cenario;
		this.display = display;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;

		this.setListeners();
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

					display.getTree().mask( display.getI18nMessages().loading() );
					Services.disciplinas().getResumos(
						cenario, se.getSelectedItem(),
						new AbstractAsyncCallbackWithDefaultOnFailure< List< ResumoDisciplinaDTO > >( display )
						{
							@Override
							public void onSuccess(
								List< ResumoDisciplinaDTO > list )
							{
								display.getStore().removeAll();
								display.getStore().add( list, true );
								display.getTree().unmask();
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
						ExcelInformationType.RESUMO_DISCIPLINA, instituicaoEnsinoDTO, fileExtension ); 

					ExportExcelFormSubmit e = new ExportExcelFormSubmit(
						parametros,	display.getI18nConstants(), display.getI18nMessages() );

					if ( display.getCampusComboBox() == null
						|| display.getCampusComboBox().getValue() == null
						|| display.getCampusComboBox().getValue().getId() == null )
					{
						return;
					}

					String campusId = display.getCampusComboBox().getValue().getId().toString();
					e.addParameter( "campusId", campusId );

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
						ExcelInformationType.RESUMO_DISCIPLINA, instituicaoEnsinoDTO, fileExtension ); 

					ExportExcelFormSubmit e = new ExportExcelFormSubmit(
						parametros,	display.getI18nConstants(), display.getI18nMessages() );

					if ( display.getCampusComboBox() == null
						|| display.getCampusComboBox().getValue() == null
						|| display.getCampusComboBox().getValue().getId() == null )
					{
						return;
					}

					String campusId = display.getCampusComboBox().getValue().getId().toString();
					e.addParameter( "campusId", campusId );

					e.submit();
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
