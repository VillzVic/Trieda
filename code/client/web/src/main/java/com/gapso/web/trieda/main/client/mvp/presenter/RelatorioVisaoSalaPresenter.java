package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaSalaGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoPresenter;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.SalaComboBox;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.gapso.web.trieda.shared.util.view.UnidadeComboBox;

public class RelatorioVisaoSalaPresenter extends RelatorioVisaoPresenter{
	public interface Display extends RelatorioVisaoPresenter.Display{
		CampusComboBox getCampusComboBox();
		UnidadeComboBox getUnidadeComboBox();
		SalaComboBox getSalaComboBox();
		TurnoComboBox getTurnoComboBox();
		TextField<String> getCapacidadeTextField();
		TextField<String> getTipoTextField();
		GradeHorariaSalaGrid getGrid();
	}

	public RelatorioVisaoSalaPresenter(InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display)
	{
		super(instituicaoEnsinoDTO, display);
	}

	protected void setListeners(){
		super.setListeners();
		
		final Display display = (Display) getDisplay();
		
		display.getSalaComboBox().addSelectionChangedListener(new SelectionChangedListener<SalaDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<SalaDTO> se){
				final SalaDTO salaDTO = se.getSelectedItem();
				
				boolean isSalaNull = salaDTO == null; 

				display.getCapacidadeTextField().setValue(isSalaNull ? "" : salaDTO.getCapacidade().toString());
				display.getTipoTextField().setValue(isSalaNull ? "" : salaDTO.getTipoString());
			}
		});

	}

}
