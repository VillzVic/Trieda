package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.DemandaDTO;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.services.DemandasService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class DemandasServiceImpl extends RemoteServiceServlet implements DemandasService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public PagingLoadResult<DemandaDTO> getBuscaList(CampusDTO campusDTO, CursoDTO cursoDTO, CurriculoDTO curriculoDTO, TurnoDTO turnoDTO, DisciplinaDTO disciplinaDTO, PagingLoadConfig config) {
		List<DemandaDTO> list = new ArrayList<DemandaDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		Campus campus 			= (campusDTO == null)? null 	: ConvertBeans.toCampus(campusDTO);
		Curso curso 			= (cursoDTO == null)? null 		: ConvertBeans.toCurso(cursoDTO);
		Curriculo curriculo 	= (curriculoDTO == null)? null 	: ConvertBeans.toCurriculo(curriculoDTO);
		Turno turno 			= (turnoDTO == null)? null 		: ConvertBeans.toTurno(turnoDTO);
		Disciplina disciplina	= (disciplinaDTO == null)? null : ConvertBeans.toDisciplina(disciplinaDTO);
		
		for(Demanda demanda : Demanda.findBy(campus, curso, curriculo, turno, disciplina, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toDemandaDTO(demanda));
		}
		BasePagingLoadResult<DemandaDTO> result = new BasePagingLoadResult<DemandaDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Demanda.count(campus, curso, curriculo, turno, disciplina));
		return result;
	}

	@Override
	public void save(DemandaDTO demandaDTO) {
		Demanda d = ConvertBeans.toDemanda(demandaDTO);
		if(d.getId() != null && d.getId() > 0) {
			d.merge();
		} else {
			List<Demanda> demandas = Demanda.findBy(d.getOferta().getCampus(), d.getOferta().getCurriculo().getCurso(), d.getOferta().getCurriculo(), d.getOferta().getTurno(), d.getDisciplina(), 0, 1, null);
			if(!demandas.isEmpty()) {
				Integer qtd = d.getQuantidade();
				d = demandas.get(0);
				d.setQuantidade(qtd);
				d.merge();
			} else {
				d.persist();
			}
		}
	}
	
	@Override
	public void remove(List<DemandaDTO> demandaDTOList) {
		for(DemandaDTO demandaDTO : demandaDTOList) {
			ConvertBeans.toDemanda(demandaDTO).remove();
		}
	}
	
}
