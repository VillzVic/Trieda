package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface AlunosServiceAsync
{
	void getAluno( Long id, AsyncCallback< AlunoDTO > callback );
	void getBuscaList(CenarioDTO cenario, String nome, String matricula, PagingLoadConfig config, AsyncCallback<PagingLoadResult<AlunoDTO>> callback);
	void getAlunosList( String nome, String cpf, AsyncCallback< PagingLoadResult< AlunoDTO > > callback );
	void saveAluno( AlunoDTO alunoDTO, AsyncCallback< Void > callback );
	void removeAlunos( List< AlunoDTO > list, AsyncCallback< Void > callback );
}
