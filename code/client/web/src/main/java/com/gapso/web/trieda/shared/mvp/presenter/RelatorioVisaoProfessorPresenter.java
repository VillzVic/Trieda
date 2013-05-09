package com.gapso.web.trieda.shared.mvp.presenter;

import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaProfessorGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoPresenter;
import com.gapso.web.trieda.shared.util.view.ProfessorVirtualComboBox;

public class RelatorioVisaoProfessorPresenter extends RelatorioVisaoPresenter{
	public interface Display extends RelatorioVisaoPresenter.Display{
		ProfessorVirtualComboBox getProfessorVirtualComboBox();
		GradeHorariaProfessorGrid getGrid();
		TextField<String> getCpfTextField();
		TextField<String> getProfessorTextField();
	}

	private boolean isVisaoProfessor;

	public RelatorioVisaoProfessorPresenter(InstituicaoEnsinoDTO instituicaoEnsinoDTO, CenarioDTO cenario, UsuarioDTO usuario,
		Display display, boolean isVisaoProfessor)
	{
		super(instituicaoEnsinoDTO, display);
		this.isVisaoProfessor = isVisaoProfessor;
	}

	protected void setListeners(){
		super.setListeners();
		
		final Display display = (Display) this.getDisplay();
		
		if (display.getProfessorVirtualComboBox() != null) {
			display.getProfessorVirtualComboBox().addSelectionChangedListener(new SelectionChangedListener<ProfessorVirtualDTO>(){
				@Override
				public void selectionChanged(SelectionChangedEvent<ProfessorVirtualDTO> se){
					display.getProfessorTextField().setValue(null);
					display.getCpfTextField().setValue(null);
				}
			});
		}
	}

	public boolean isVisaoProfessor(){
		return this.isVisaoProfessor;
	}

	public void setVisaoProfessor(boolean isVisaoProfessor){
		this.isVisaoProfessor = isVisaoProfessor;
	}

}
