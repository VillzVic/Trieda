package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.TipoCurso;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.services.CursosService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.TipoCursoDTO;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
@Transactional
public class CursosServiceImpl extends RemoteServiceServlet implements CursosService {

	private static final long serialVersionUID = 5250776996542788849L;
	
	@Override
	public CursoDTO getCurso(Long id) {
		return ConvertBeans.toCursoDTO(Curso.find(id));
	}
	
	@Override
	public ListLoadResult<CursoDTO> getListAll() {
		List<CursoDTO> list = new ArrayList<CursoDTO>();
		List<Curso> cursos = Curso.findAll();
		for(Curso curso : cursos) {
			CursoDTO cursoDTO = ConvertBeans.toCursoDTO(curso);
			list.add(cursoDTO);
		}
		return new BasePagingLoadResult<CursoDTO>(list);
	}
	
	@Override
	public ListLoadResult<CursoDTO> getList(BasePagingLoadConfig loadConfig) {
		return getBuscaList(null, loadConfig.get("query").toString(), null, loadConfig);
	}
	
	@Override
	public PagingLoadResult<CursoDTO> getBuscaList(String nome, String codigo, TipoCursoDTO tipoCursoDTO, PagingLoadConfig config) {
		List<CursoDTO> list = new ArrayList<CursoDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		TipoCurso tipoCurso = null;
		if(tipoCursoDTO != null) {
			tipoCurso = ConvertBeans.toTipoCurso(tipoCursoDTO);
		}
		for(Curso curso : Curso.findBy(codigo, nome, tipoCurso, config.getOffset(), config.getLimit(), orderBy)) {
			CursoDTO cursoDTO = ConvertBeans.toCursoDTO(curso);
			list.add(cursoDTO);
		}
		BasePagingLoadResult<CursoDTO> result = new BasePagingLoadResult<CursoDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Curso.count(codigo, nome, tipoCurso));
		return result;
	}
	
	@Override
	public ListLoadResult<CursoDTO> getListByCampus(CampusDTO campusDTO) {
		List<CursoDTO> list = new ArrayList<CursoDTO>();
		
		Campus campus = (campusDTO == null)? null : ConvertBeans.toCampus(campusDTO);
		
		for(Curso curso : Curso.findByCampus(campus)) {
			list.add(ConvertBeans.toCursoDTO(curso));
		}
		
		ListLoadResult<CursoDTO> result = new BaseListLoadResult<CursoDTO>(list);
		return result;
	}
	
	@Override
	public void save(CursoDTO cursoDTO) {
		Curso curso = ConvertBeans.toCurso(cursoDTO);
		if(curso.getId() != null && curso.getId() > 0) {
			curso.merge();
		} else {
			curso.persist();
		}
	}
	
	@Override
	public void remove(List<CursoDTO> cursoDTOList) {
		for(CursoDTO cursoDTO : cursoDTOList) {
			ConvertBeans.toCurso(cursoDTO).remove();
		}
	}

}
