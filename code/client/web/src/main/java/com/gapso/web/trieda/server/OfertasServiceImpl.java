package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.services.OfertasService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
@Transactional
public class OfertasServiceImpl extends RemoteServiceServlet implements OfertasService {

	private static final long serialVersionUID = -3010939181486905949L;

	@Override
	public OfertaDTO getOferta(Long id) {
		return ConvertBeans.toOfertaDTO(Oferta.find(id));
	}

	@Override
	public PagingLoadResult<OfertaDTO> getBuscaList(TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO, CurriculoDTO curriculoDTO, PagingLoadConfig config) {
		List<OfertaDTO> list = new ArrayList<OfertaDTO>();
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

		for(Oferta oferta : Oferta.findBy(turno, campus, curso, curriculo, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toOfertaDTO(oferta));
		}
		BasePagingLoadResult<OfertaDTO> result = new BasePagingLoadResult<OfertaDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Oferta.count(turno, campus, curso, curriculo));
		return result;
	}

	@Override
	public ListLoadResult<TreeNodeDTO> getListByCampusAndTurno(CampusDTO campusDTO, TurnoDTO turnoDTO) {
		List<TreeNodeDTO> treeNodesList = new ArrayList<TreeNodeDTO>();
		
		Campus campus = (campusDTO == null)? null : ConvertBeans.toCampus(campusDTO);
		Turno turno = (turnoDTO == null)? null : ConvertBeans.toTurno(turnoDTO);
		
		for(Oferta oferta : Oferta.findByCampusAndTurno(campus, turno)) {
			OfertaDTO ofertaDTO = ConvertBeans.toOfertaDTO(oferta);
			treeNodesList.add(new TreeNodeDTO(ofertaDTO));
		}
		
		Collections.sort(treeNodesList);
		
		ListLoadResult<TreeNodeDTO> result = new BaseListLoadResult<TreeNodeDTO>(treeNodesList);
		return result;
	}
	
	@Override
	public void save(OfertaDTO ofertaDTO) {
		Oferta oferta = ConvertBeans.toOferta(ofertaDTO);
		if(oferta.getId() != null && oferta.getId() > 0) {
			oferta.merge();
		} else {
			oferta.persist();
		}
	}

	@Override
	public void remove(List<OfertaDTO> ofertaDTOList) {
		for(OfertaDTO ofertaDTO : ofertaDTOList) {
			ConvertBeans.toOferta(ofertaDTO).remove();
		}
	}

}
