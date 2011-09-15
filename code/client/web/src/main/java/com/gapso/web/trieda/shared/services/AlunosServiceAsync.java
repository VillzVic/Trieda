package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface AlunosServiceAsync
{
	void getAluno( Long id, AsyncCallback< AlunoDTO > callback );
	void getAlunosList( String nome, String cpf, AsyncCallback< PagingLoadResult< AlunoDTO > > callback );
	void saveAluno( AlunoDTO alunoDTO, AsyncCallback< Void > callback );
	void removeAlunos( List< AlunoDTO > list, AsyncCallback< Void > callback );
}
