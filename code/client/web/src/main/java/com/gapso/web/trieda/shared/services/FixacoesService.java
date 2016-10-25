package com.gapso.web.trieda.shared.services;

import java.util.Collection;
import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.FixacaoDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "fixacoes" )
public interface FixacoesService
	extends RemoteService
{
	FixacaoDTO getFixacao( Long id );
	PagingLoadResult< FixacaoDTO > getBuscaList(CenarioDTO cenarioDTO, String descricao, PagingLoadConfig config );
	void remove( List< FixacaoDTO > fixacaoDTOList );
	void save(FixacaoDTO fixacaoDTO, List<HorarioDisponivelCenarioDTO> hdcDTOList, List<ProfessorDTO> professorDTOList, List<UnidadeDTO> unidadeDTOList, List<SalaDTO> salaDTOList);
	
	List< HorarioDisponivelCenarioDTO > getHorariosSelecionados( FixacaoDTO fixacaoDTO );
	PagingLoadResult<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(DisciplinaDTO disciplinaDTO, AtendimentoOperacionalDTO turma);
	List<ProfessorDTO> getProfessoresFixados(FixacaoDTO fixacaoDTO);
	List<UnidadeDTO> getUnidadesFixadas(FixacaoDTO fixacaoDTO);
	Collection<SalaDTO> getSalasFixadas(FixacaoDTO fixacaoDTO);
	
}
