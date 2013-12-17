package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoAlunoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoPresenter;
import com.gapso.web.trieda.shared.util.view.AlunosComboBox;
import com.gapso.web.trieda.shared.util.view.AlunosMatriculaComboBox;

public class RelatorioVisaoAlunoPresenter extends RelatorioVisaoPresenter{
	public interface Display extends RelatorioVisaoPresenter.Display{
		RelatorioVisaoAlunoFiltro getFiltro();
		AlunosComboBox getAlunoTextField();
		AlunosMatriculaComboBox getAlunoMatriculaTextField();
	}

	public RelatorioVisaoAlunoPresenter(InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display)
	{
		super(instituicaoEnsinoDTO, cenario, display);
	}
	
	protected void setListeners(){
		super.setListeners();
		
		final Display display = (Display) this.getDisplay();
		
		if (display.getAlunoTextField() != null) {
			display.getAlunoTextField().addSelectionChangedListener(new SelectionChangedListener<AlunoDTO>(){
				@Override
				public void selectionChanged(SelectionChangedEvent<AlunoDTO> se){
					if (display.getAlunoTextField().getValue() != null){
						display.getAlunoMatriculaTextField().setValue(null);
					}
				}
			});
		}
		
		if (display.getAlunoMatriculaTextField() != null) {
			display.getAlunoMatriculaTextField().addSelectionChangedListener(new SelectionChangedListener<AlunoDTO>(){
				@Override
				public void selectionChanged(SelectionChangedEvent<AlunoDTO> se){
					if (display.getAlunoMatriculaTextField().getValue() != null){
						display.getAlunoTextField().setValue(null);
					}
				}
			});
		}
	}
}
