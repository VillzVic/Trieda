package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.TipoCurso;
import com.gapso.web.trieda.client.mvp.model.TipoCursoDTO;
import com.gapso.web.trieda.client.services.TiposCursosService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class TiposCursosServiceImpl extends RemoteServiceServlet implements TiposCursosService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public PagingLoadResult<TipoCursoDTO> getBuscaList(String nome, String descricao, PagingLoadConfig config) {
		List<TipoCursoDTO> list = new ArrayList<TipoCursoDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(TipoCurso tipoCurso : TipoCurso.findByCodigoLikeAndDescricaoLike(nome, descricao, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toTipoCursoDTO(tipoCurso));
		}
		BasePagingLoadResult<TipoCursoDTO> result = new BasePagingLoadResult<TipoCursoDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(TipoCurso.count());
		return result;
	}

	@Override
	public void save(TipoCursoDTO tipoCursoDTO) {
		TipoCurso tipoCurso = ConvertBeans.toTipoCurso(tipoCursoDTO);
		if(tipoCurso.getId() != null && tipoCurso.getId() > 0) {
			tipoCurso.merge();
		} else {
			tipoCurso.persist();
		}
	}
	
	@Override
	public void remove(List<TipoCursoDTO> tipoCursoDTOList) {
		for(TipoCursoDTO tipoCursoDTO : tipoCursoDTOList) {
			ConvertBeans.toTipoCurso(tipoCursoDTO).remove();
		}
	}

}
