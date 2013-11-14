package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioDTO;
import com.gapso.web.trieda.shared.util.view.RelatorioAlunoFiltro;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface AlunosServiceAsync
{
	void getAluno( Long id, AsyncCallback< AlunoDTO > callback );
	void getBuscaList(CenarioDTO cenario, String nome, String matricula, PagingLoadConfig config, AsyncCallback<PagingLoadResult<AlunoDTO>> callback);
	void getAlunosList( String nome, String cpf, AsyncCallback< PagingLoadResult< AlunoDTO > > callback );
	void getAlunosListByCampus(CampusDTO campusDTO, AsyncCallback<PagingLoadResult<AlunoDTO>> callback);
	void saveAluno( AlunoDTO alunoDTO, AsyncCallback< Void > callback );
	void removeAlunos( List< AlunoDTO > list, AsyncCallback< Void > callback );
	void getAutoCompleteList(CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig, String tipoComboBox,
			AsyncCallback<ListLoadResult<AlunoDTO>> callback);
	void getRelatorio(CenarioDTO cenarioDTO,	RelatorioAlunoFiltro alunoFiltro, RelatorioDTO currentNode,
			AsyncCallback<List<RelatorioDTO>> callback);
}
