package com.gapso.web.trieda.shared.mvp.presenter;

import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaProfessorGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoPresenter;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.ProfessorVirtualComboBox;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;

public class RelatorioVisaoProfessorPresenter extends RelatorioVisaoPresenter{
	public interface Display extends RelatorioVisaoPresenter.Display{
		CampusComboBox getCampusComboBox();
		TurnoComboBox getTurnoComboBox();
		ProfessorComboBox getProfessorComboBox();
		ProfessorVirtualComboBox getProfessorVirtualComboBox();
		GradeHorariaProfessorGrid getGrid();
	}

	private boolean isVisaoProfessor;

	public RelatorioVisaoProfessorPresenter(InstituicaoEnsinoDTO instituicaoEnsinoDTO, CenarioDTO cenario, UsuarioDTO usuario,
		Display display, boolean isVisaoProfessor)
	{
		super(instituicaoEnsinoDTO, display);
		this.isVisaoProfessor = isVisaoProfessor;

		this.setListeners();
	}

	protected void setListeners(){
		super.setListeners();
		
		final Display display = (Display) this.getDisplay();
		
		display.getProfessorComboBox().addSelectionChangedListener(new SelectionChangedListener<ProfessorDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<ProfessorDTO> se){
				display.getProfessorVirtualComboBox().setValue(null);
			}
		});

		display.getProfessorVirtualComboBox().addSelectionChangedListener(new SelectionChangedListener<ProfessorVirtualDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<ProfessorVirtualDTO> se){
				display.getProfessorComboBox().setValue(null);
			}
		});
	}

	public boolean isVisaoProfessor(){
		return this.isVisaoProfessor;
	}

	public void setVisaoProfessor(boolean isVisaoProfessor){
		this.isVisaoProfessor = isVisaoProfessor;
	}

}
