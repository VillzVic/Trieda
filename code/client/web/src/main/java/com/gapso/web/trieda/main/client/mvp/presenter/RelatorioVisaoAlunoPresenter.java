package com.gapso.web.trieda.main.client.mvp.presenter;

import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoAlunoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoPresenter;

public class RelatorioVisaoAlunoPresenter extends RelatorioVisaoPresenter{
	public interface Display extends RelatorioVisaoPresenter.Display{
		RelatorioVisaoAlunoFiltro getFiltro();
	}

	public RelatorioVisaoAlunoPresenter(InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display)
	{
		super(instituicaoEnsinoDTO, display);
	}
	
}
