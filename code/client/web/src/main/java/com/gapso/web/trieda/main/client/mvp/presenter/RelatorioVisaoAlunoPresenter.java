package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.util.view.AlunosComboBox;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.GradeHorariaAlunoGrid;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.ui.Widget;

public class RelatorioVisaoAlunoPresenter implements Presenter{
	public interface Display extends ITriedaI18nGateway{
		Button getSubmitBuscaButton();
		CampusComboBox getCampusComboBox();
		TurnoComboBox getTurnoComboBox();
		AlunosComboBox getAlunosComboBox();
		GradeHorariaAlunoGrid getGrid();
		Component getComponent();
		Button getExportExcelButton(); 
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display;

	public RelatorioVisaoAlunoPresenter(InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display)
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.display = display;

		this.setListeners();
	}

	private void setListeners(){
		this.display.getSubmitBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce){
				display.getGrid().setAlunoDTO(display.getAlunosComboBox().getValue());
				display.getGrid().setCampusDTO(display.getCampusComboBox().getValue());
				display.getGrid().setTurnoDTO(display.getTurnoComboBox().getValue());
				display.getGrid().requestAtendimentos();
			}
		});

		this.display.getAlunosComboBox().addSelectionChangedListener(new SelectionChangedListener<AlunoDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<AlunoDTO> se){
				display.getAlunosComboBox().setValue(se.getSelectedItem());
			}
		});

		this.display.getExportExcelButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce){
				/*
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.RELATORIO_VISAO_PROFESSOR, instituicaoEnsinoDTO
				);

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages()
				);

				CampusDTO campusDTO = display.getCampusComboBox().getValue();
				TurnoDTO turnoDTO = display.getTurnoComboBox().getValue();
				AlunoDTO alunoDTO = display.getAlunosComboBox().getValue();

				e.addParameter( "campusId", campusDTO.getId().toString() );
				e.addParameter( "turnoId", turnoDTO.getId().toString() );
				e.addParameter( "instituicaoEnsinoId", turnoDTO.getInstituicaoEnsinoId().toString() );

				if(alunoDTO != null) e.addParameter( "alunoId", alunoDTO.getId().toString());

				e.submit();
				*/
			}
		});
	}

	@Override
	public void go(Widget widget){
		GTab tab = (GTab)widget;
		tab.add((GTabItem) display.getComponent());
	}
	
}
