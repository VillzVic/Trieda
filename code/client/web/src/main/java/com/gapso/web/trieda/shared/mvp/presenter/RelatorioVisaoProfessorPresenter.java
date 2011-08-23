package com.gapso.web.trieda.shared.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.GradeHorariaProfessorGrid;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.ProfessorVirtualComboBox;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.ui.Widget;

public class RelatorioVisaoProfessorPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Button getSubmitBuscaButton();
		CampusComboBox getCampusComboBox();
		TurnoComboBox getTurnoComboBox();
		ProfessorComboBox getProfessorComboBox();
		GradeHorariaProfessorGrid getGrid();
		Component getComponent();
		ProfessorVirtualComboBox getProfessorVirtualComboBox();
		Button getExportExcelButton(); 
	}

	private Display display;
	private UsuarioDTO usuario;
	private boolean isVisaoProfessor;

	public RelatorioVisaoProfessorPresenter( CenarioDTO cenario,
		UsuarioDTO usuario, Display display, boolean isVisaoProfessor )
	{
		this.usuario = usuario;
		this.display = display;
		this.isVisaoProfessor = isVisaoProfessor;

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
					if ( usuario.isAdministrador() )
					{
						display.getGrid().setProfessorDTO(
							display.getProfessorComboBox().getValue() );

						display.getGrid().setProfessorVirtualDTO(
							display.getProfessorVirtualComboBox().getValue() );
					}
					else
					{
						ProfessorDTO professor = new ProfessorDTO();
						professor.setId( usuario.getProfessorId() );
						display.getGrid().setProfessorDTO( professor );
					}

					display.getGrid().setTurnoDTO(
						display.getTurnoComboBox().getValue() );

					display.getGrid().requestAtendimentos();
				}
			});

		display.getProfessorComboBox().addSelectionChangedListener(
			new SelectionChangedListener< ProfessorDTO >()
			{
				@Override
				public void selectionChanged( SelectionChangedEvent< ProfessorDTO > se )
				{
					display.getProfessorVirtualComboBox().setValue( null );
					display.getProfessorComboBox().setValue( se.getSelectedItem() );
				}
			});

		display.getProfessorVirtualComboBox().addSelectionChangedListener(
			new SelectionChangedListener< ProfessorVirtualDTO >()
			{
				@Override
				public void selectionChanged( SelectionChangedEvent< ProfessorVirtualDTO > se )
				{
					display.getProfessorComboBox().setValue( null );
					display.getProfessorVirtualComboBox().setValue( se.getSelectedItem() );
				}
			});

		display.getExportExcelButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
				{
					@Override
					public void componentSelected( ButtonEvent ce )
					{
						ExportExcelFormSubmit e = new ExportExcelFormSubmit(
							ExcelInformationType.RELATORIO_VISAO_PROFESSOR,
							display.getI18nConstants(), display.getI18nMessages() );

						CampusDTO campusDTO = display.getCampusComboBox().getValue();
						TurnoDTO turnoDTO = display.getTurnoComboBox().getValue();
						ProfessorDTO professorDTO = display.getProfessorComboBox().getValue();
						ProfessorVirtualDTO professorVirtualDTO = display.getProfessorVirtualComboBox().getValue();

						e.addParameter( "campusId", campusDTO.getId().toString() );
						e.addParameter( "turnoId", turnoDTO.getId().toString() );

						if ( professorDTO == null )
						{
							e.addParameter( "professorId", "-1" );
							e.addParameter( "professorVirtualId", professorVirtualDTO.getId().toString() );
						}
						else if ( professorVirtualDTO == null )
						{
							e.addParameter( "professorId", professorDTO.getId().toString() );
							e.addParameter( "professorVirtualId", "-1" );
						}

						e.submit();
					}
				});
	}

	public boolean isVisaoProfessor()
	{
		return this.isVisaoProfessor;
	}

	public void setVisaoProfessor( boolean isVisaoProfessor )
	{
		this.isVisaoProfessor = isVisaoProfessor;
	}

	@Override
	public void go( Widget widget )
	{
		GTab tab = (GTab)widget;
		tab.add( (GTabItem) display.getComponent() );
	}
}
