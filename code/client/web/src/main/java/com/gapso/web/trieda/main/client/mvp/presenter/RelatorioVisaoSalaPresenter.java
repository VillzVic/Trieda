package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.GradeHorariaSalaGrid;
import com.gapso.web.trieda.shared.util.view.SalaComboBox;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.gapso.web.trieda.shared.util.view.UnidadeComboBox;
import com.google.gwt.user.client.ui.Widget;

public class RelatorioVisaoSalaPresenter implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		Button getSubmitBuscaButton();
		CampusComboBox getCampusComboBox();
		UnidadeComboBox getUnidadeComboBox();
		SalaComboBox getSalaComboBox();
		TurnoComboBox getTurnoComboBox();
		TextField< String > getCapacidadeTextField();
		TextField< String > getTipoTextField();
		GradeHorariaSalaGrid getGrid();
		Component getComponent();
		Button getExportExcelButton();
	}

	private Display display; 

	public RelatorioVisaoSalaPresenter( CenarioDTO cenario, Display display )
	{
		this.display = display;
		setListeners();
	}

	private void setListeners()
	{
		display.getSubmitBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					display.getGrid().setSalaDTO( display.getSalaComboBox().getValue() );
					display.getGrid().setTurnoDTO( display.getTurnoComboBox().getValue() );
					display.getGrid().requestAtendimentos();
				}
			});
		display.getSalaComboBox().addSelectionChangedListener(
			new SelectionChangedListener< SalaDTO >()
			{
				@Override
				public void selectionChanged(SelectionChangedEvent< SalaDTO > se )
				{
					final SalaDTO salaDTO = se.getSelectedItem();
					if ( salaDTO == null )
					{
						display.getCapacidadeTextField().setValue( "" );
						display.getTipoTextField().setValue( "" );
					}
					else
					{
						display.getCapacidadeTextField().setValue( salaDTO.getCapacidade().toString() );
						display.getTipoTextField().setValue( salaDTO.getTipoString() );
					}
				}
			});

		display.getExportExcelButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
				{
					@Override
					public void componentSelected( ButtonEvent ce )
					{
						ExportExcelFormSubmit e = new ExportExcelFormSubmit(
							ExcelInformationType.RELATORIO_VISAO_SALA,
							display.getI18nConstants(), display.getI18nMessages() );

						CampusDTO campusDTO = display.getCampusComboBox().getValue();
						UnidadeDTO unidadeDTO = display.getUnidadeComboBox().getValue();
						SalaDTO salaDTO = display.getSalaComboBox().getValue();
						TurnoDTO turnoDTO = display.getTurnoComboBox().getValue();

						e.addParameter( "campusId", campusDTO.getId().toString() );
						e.addParameter( "unidadeId", unidadeDTO.getId().toString() );
						e.addParameter( "salaId", salaDTO.getId().toString() );
						e.addParameter( "turnoId", turnoDTO.getId().toString() );

						e.submit();
					}
				});
	}
	
	@Override
	public void go( Widget widget )
	{
		GTab tab = (GTab)widget;
		tab.add( (GTabItem)display.getComponent() );
	}
}
