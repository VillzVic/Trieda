package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.FixacaoDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "fixacoes" )
public interface FixacoesService
	extends RemoteService
{
	FixacaoDTO getFixacao( Long id );
	PagingLoadResult< FixacaoDTO > getBuscaList(CenarioDTO cenarioDTO, String descricao, PagingLoadConfig config );
	void remove( List< FixacaoDTO > fixacaoDTOList );
	void save( FixacaoDTO fixacaoDTO, List< HorarioDisponivelCenarioDTO > hdcDTOList );
	List< HorarioDisponivelCenarioDTO > getHorariosSelecionados( FixacaoDTO fixacaoDTO );
	//PagingLoadResult<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(ProfessorDTO professorDTO, DisciplinaDTO disciplinaDTO, SalaDTO salaDTO, String string);
	PagingLoadResult<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(DisciplinaDTO disciplinaDTO, String turma);
}
