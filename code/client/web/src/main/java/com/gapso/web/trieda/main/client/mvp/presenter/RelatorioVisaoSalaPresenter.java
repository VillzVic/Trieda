package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaSalaGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoPresenter;

public class RelatorioVisaoSalaPresenter extends RelatorioVisaoPresenter{
	public interface Display extends RelatorioVisaoPresenter.Display{
		TextField<String> getSalaTextField();
		GradeHorariaSalaGrid getGrid();
	}

	public RelatorioVisaoSalaPresenter(InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display)
	{
		super(instituicaoEnsinoDTO, display);
	}

}
