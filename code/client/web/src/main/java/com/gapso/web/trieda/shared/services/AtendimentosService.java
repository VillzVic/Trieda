package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.QuintetoDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SextetoDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "atendimentos" )
public interface AtendimentosService extends RemoteService {

	/**
	 * 
	 * @param curriculoDTO
	 * @param periodo
	 * @param turnoDTO
	 * @param campusDTO
	 * @param cursoDTO
	 * @return
	 */
	SextetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>,List<String>> getAtendimentosParaGradeHorariaVisaoCurso(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO);
	
	/**
	 * 
	 * @param sala
	 * @param turno
	 * @return
	 */
	QuintetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<String>> getAtendimentosParaGradeHorariaVisaoSala(SalaDTO sala, TurnoDTO turno);
	
	QuintetoDTO<Integer, Integer, Integer, List<AtendimentoRelatorioDTO>, List<String>> getAtendimentosParaGradeHorariaVisaoProfessor(ProfessorDTO professorDTO, 
		ProfessorVirtualDTO professorVirtualDTO, TurnoDTO turnoDTO, boolean isVisaoProfessor);
	
	PagingLoadResult< AtendimentoTaticoDTO > getList();

//	ParDTO< List< AtendimentoRelatorioDTO >, List< Integer > > getBusca(
//		CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO );

	ListLoadResult< ProfessorVirtualDTO > getProfessoresVirtuais( CampusDTO campusDTO );

}
