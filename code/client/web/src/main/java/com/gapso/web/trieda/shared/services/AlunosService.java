package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "alunos" )
public interface AlunosService
	extends RemoteService
{
	AlunoDTO getAluno( Long id );
	PagingLoadResult<AlunoDTO> getBuscaList(CenarioDTO cenario, String nome, String matricula, PagingLoadConfig config);
	PagingLoadResult< AlunoDTO > getAlunosList( String nome, String cpf );
	PagingLoadResult< AlunoDTO > getAlunosListByCampus(CampusDTO campusDTO);
	void saveAluno( AlunoDTO alunoDTO );
	void removeAlunos( List< AlunoDTO > list );
	ListLoadResult<AlunoDTO> getAutoCompleteList(CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig, String tipoComboBox);
}
