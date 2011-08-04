package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CurriculosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.GradeHorariaCursoGrid;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class RelatorioVisaoCursoPresenter
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		Button getSubmitBuscaButton();
		CampusComboBox getCampusComboBox();
		CursoComboBox getCursoComboBox();
		CurriculoComboBox getCurriculoComboBox();
		TurnoComboBox getTurnoComboBox();
		SimpleComboBox< Integer > getPeriodoComboBox();
		GradeHorariaCursoGrid getGrid();
		Component getComponent();
		Button getExportExcelButton();
	}

	private Display display; 

	public RelatorioVisaoCursoPresenter(
		CenarioDTO cenario, Display display )
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
					display.getGrid().setCurriculoDTO( display.getCurriculoComboBox().getValue() );
					display.getGrid().setPeriodo( display.getPeriodoComboBox().getValue().getValue() );
					display.getGrid().setTurnoDTO( display.getTurnoComboBox().getValue() );
					display.getGrid().setCampusDTO( display.getCampusComboBox().getValue() );
					display.getGrid().setCursoDTO( display.getCursoComboBox().getValue() );
					display.getGrid().requestAtendimentos();
				}
		});

		display.getCurriculoComboBox().addSelectionChangedListener(
			new SelectionChangedListener< CurriculoDTO >()
		{
			@Override
			public void selectionChanged( SelectionChangedEvent< CurriculoDTO > se )
			{
				final CurriculoDTO curriculoDTO = se.getSelectedItem();

				display.getPeriodoComboBox().getStore().removeAll();
				display.getPeriodoComboBox().setValue( null );
				display.getPeriodoComboBox().setEnabled( curriculoDTO != null );

				if ( curriculoDTO != null )
				{
					CurriculosServiceAsync service = Services.curriculos();

					service.getPeriodos( curriculoDTO, new AsyncCallback< List< Integer > >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "Erro",
								"Erro no servidor ao pegar os períodos da Matriz Curricular", null );
						}

						@Override
						public void onSuccess( List< Integer > result )
						{
							display.getPeriodoComboBox().add( result );
						}
					});
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
						ExcelInformationType.RELATORIO_VISAO_CURSO,
						display.getI18nConstants(), display.getI18nMessages() );

					CursoDTO cursoDTO = display.getCursoComboBox().getValue();
					CurriculoDTO curriculoDTO = display.getCurriculoComboBox().getValue();
					CampusDTO campusDTO = display.getCampusComboBox().getValue();
					Integer periodo = display.getPeriodoComboBox().getValue().getValue();
					TurnoDTO turnoDTO = display.getTurnoComboBox().getValue();

					e.addParameter( "cursoId", cursoDTO.getId().toString() );
					e.addParameter( "curriculoId", curriculoDTO.getId().toString() );
					e.addParameter( "campusId", campusDTO.getId().toString() );
					e.addParameter( "periodoId", periodo.toString() );
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
