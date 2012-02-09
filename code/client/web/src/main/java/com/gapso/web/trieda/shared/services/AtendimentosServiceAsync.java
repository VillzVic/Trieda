package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface AtendimentosServiceAsync {
	void getList( AsyncCallback< PagingLoadResult< AtendimentoTaticoDTO > > callback );

	void getBusca( SalaDTO sala, TurnoDTO turno, SemanaLetivaDTO semanaLetivaDTO,
		AsyncCallback< List< AtendimentoRelatorioDTO > > callback );

	void getAtendimentosOperacional( ProfessorDTO professorDTO, ProfessorVirtualDTO professorVirtualDTO,
		TurnoDTO turnoDTO, boolean isVisaoProfessor, SemanaLetivaDTO semanaLetivaDTO,
		AsyncCallback< List< AtendimentoOperacionalDTO > > callback );

	void getBusca( CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO,
		AsyncCallback< ParDTO< List< AtendimentoRelatorioDTO >, List< Integer > > > callback );

	void getProfessoresVirtuais( CampusDTO campusDTO,
		AsyncCallback< ListLoadResult< ProfessorVirtualDTO > > callback );

	/**
	 * 
	 * @param curriculoDTO
	 * @param periodo
	 * @param turnoDTO
	 * @param campusDTO
	 * @param cursoDTO
	 * @param callback
	 */
	void getAtendimentosParaGradeHorariaVisaoCurso(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO, AsyncCallback<ParDTO<List<AtendimentoRelatorioDTO>,List<Integer>>> callback);
}