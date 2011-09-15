package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "alunos" )
public interface AlunosService
	extends RemoteService
{
	AlunoDTO getAluno( Long id );
	PagingLoadResult< AlunoDTO > getAlunosList( String nome, String cpf );
	void saveAluno( AlunoDTO alunoDTO );
	void removeAlunos( List< AlunoDTO > list );
}
