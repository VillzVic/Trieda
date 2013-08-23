package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaDemandaDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.ResumoMatriculaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface AlunosDemandaServiceAsync
{
	void getAlunoDemanda( Long id, AsyncCallback< AlunoDemandaDTO > callback );
	void getAlunosDemandaList(DemandaDTO demandaDTO, AsyncCallback< ListLoadResult< AlunoDemandaDTO > > callback );
	void saveAlunoDemanda( DemandaDTO demandaDTO, AlunoDemandaDTO alunoDemandaDTO, AsyncCallback< Void > callback );
	void removeAlunosDemanda( List< AlunoDemandaDTO > list, AsyncCallback< Void > callback );
	void getResumoMatriculasList( CenarioDTO cenarioDTO, String aluno, String matricula, CampusDTO campusDTO, CursoDTO cursoDTO, PagingLoadConfig loadConfig,
			AsyncCallback<PagingLoadResult<ResumoMatriculaDTO>> callback );
	void getResumoAtendimentosDisciplinaList(CenarioDTO cenarioDTO, String codigo,	CampusDTO campusDTO, CursoDTO cursoDTO,
			PagingLoadConfig loadConfig, AsyncCallback<PagingLoadResult<ResumoMatriculaDTO>> callback );
	void getResumoFaixaDemandaList( CenarioDTO cenarioDTO, CampusDTO campusDTO, List<ParDTO<Integer, Integer>> faixas,
			AsyncCallback<List<AtendimentoFaixaDemandaDTO>> callback );
}
