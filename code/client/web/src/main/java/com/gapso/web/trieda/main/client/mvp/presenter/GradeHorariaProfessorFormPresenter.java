package com.gapso.web.trieda.main.client.mvp.presenter;

import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaProfessorGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.Widget;

public class GradeHorariaProfessorFormPresenter implements Presenter {

	public interface Display
	{
		SimpleModal getSimpleModal();
		ProfessorDTO getProfessorDTO();
		GradeHorariaProfessorGrid getPanel();
	}

	private Display display;
	
	public GradeHorariaProfessorFormPresenter( Display display )
	{
		this.display = display;
	}

	@Override
	public void go(Widget widget) {
		this.display.getSimpleModal().show();
	}
	
}
