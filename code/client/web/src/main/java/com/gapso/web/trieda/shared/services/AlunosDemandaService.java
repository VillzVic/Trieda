package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaDemandaDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.ResumoMatriculaDTO;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "alunosDemanda" )
public interface AlunosDemandaService
	extends RemoteService
{
	AlunoDemandaDTO getAlunoDemanda( Long id );
	ListLoadResult< AlunoDemandaDTO > getAlunosDemandaList( DemandaDTO demandaDTO );
	void saveAlunoDemanda( DemandaDTO demandaDTO, AlunoDemandaDTO alunoDemandaDTO ) throws TriedaException;
	void removeAlunosDemanda( List< AlunoDemandaDTO > list );
	PagingLoadResult<ResumoMatriculaDTO> getResumoMatriculasList( String aluno, String matricula, CampusDTO campusDTO, CursoDTO cursoDTO,
			PagingLoadConfig loadConfig );
	PagingLoadResult<ResumoMatriculaDTO> getResumoAtendimentosDisciplinaList( String codigo, CampusDTO campusDTO, CursoDTO cursoDTO,
			PagingLoadConfig loadConfig );
	List<AtendimentoFaixaDemandaDTO> getResumoFaixaDemandaList(CampusDTO campusDTO, List<ParDTO<Integer, Integer>> faixas);
}
