package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.CampusCurriculo;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.client.mvp.model.CampusCurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.services.CampiCurriculosService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
@Transactional
public class CampiCurriculosServiceImpl extends RemoteServiceServlet implements CampiCurriculosService {

	private static final long serialVersionUID = -3010939181486905949L;

	@Override
	public CampusCurriculoDTO getCampusCurriculo(Long id) {
		return ConvertBeans.toCampusCurriculoDTO(CampusCurriculo.find(id));
	}

	@Override
	public PagingLoadResult<CampusCurriculoDTO> getBuscaList(TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO, CurriculoDTO curriculoDTO, PagingLoadConfig config) {
		List<CampusCurriculoDTO> list = new ArrayList<CampusCurriculoDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		Turno turno = (turnoDTO != null)? ConvertBeans.toTurno(turnoDTO) : null;
		Campus campus = (campusDTO != null)? ConvertBeans.toCampus(campusDTO) : null;
		Curso curso = (cursoDTO != null)? ConvertBeans.toCurso(cursoDTO) : null;
		Curriculo curriculo = (curriculoDTO != null)? ConvertBeans.toCurriculo(curriculoDTO) : null;

		for(CampusCurriculo campusCurriculo : CampusCurriculo.findByCursoAndCodigoLikeAndDescricaoLike(turno, campus, curso, curriculo, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toCampusCurriculoDTO(campusCurriculo));
		}
		BasePagingLoadResult<CampusCurriculoDTO> result = new BasePagingLoadResult<CampusCurriculoDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(CampusCurriculo.count());
		return result;
	}

	@Override
	public void save(CampusCurriculoDTO campusCurriculoDTO) {
		CampusCurriculo campusCurriculo = ConvertBeans.toCampusCurriculo(campusCurriculoDTO);
		if(campusCurriculo.getId() != null && campusCurriculo.getId() > 0) {
			campusCurriculo.merge();
		} else {
			campusCurriculo.persist();
		}
	}

	@Override
	public void remove(List<CampusCurriculoDTO> campusCurriculoDTOList) {
		for(CampusCurriculoDTO campusCurriculoDTO : campusCurriculoDTOList) {
			ConvertBeans.toCampusCurriculo(campusCurriculoDTO).remove();
		}
	}

}
