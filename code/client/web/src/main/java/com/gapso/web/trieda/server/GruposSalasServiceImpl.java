package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.GrupoSala;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.gapso.web.trieda.client.services.GruposSalasService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class GruposSalasServiceImpl extends RemoteServiceServlet implements GruposSalasService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public GrupoSalaDTO getGrupoSala(Long id) {
		return ConvertBeans.toGrupoSalaDTO(GrupoSala.find(id));
	}
	
	@Override
	public void save(GrupoSalaDTO grupoSalaDTO) {
		GrupoSala grupoSala = ConvertBeans.toGrupoSala(grupoSalaDTO);
		if(grupoSala.getId() != null && grupoSala.getId() > 0) {
			grupoSala.merge();
		} else {
			grupoSala.persist();
		}
	}
	
	@Override
	public void remove(List<GrupoSalaDTO> grupoSalaDTOList) {
		for(GrupoSalaDTO grupoSalaDTO : grupoSalaDTOList) {
			ConvertBeans.toGrupoSala(grupoSalaDTO).remove();
		}
	}

	@Override
	public PagingLoadResult<GrupoSalaDTO> getBuscaList(String nome, String codigo, UnidadeDTO unidadeDTO, PagingLoadConfig config) {
		List<GrupoSalaDTO> list = new ArrayList<GrupoSalaDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		Unidade unidade = null;
		if(unidadeDTO != null) {
			unidade = ConvertBeans.toUnidade(unidadeDTO);
		}
		for(GrupoSala grupoSala : GrupoSala.findByNomeLikeAndCodigoLikeAndUnidade(nome, codigo, unidade, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toGrupoSalaDTO(grupoSala));
		}
		BasePagingLoadResult<GrupoSalaDTO> result = new BasePagingLoadResult<GrupoSalaDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(GrupoSala.count());
		return result;
	}

	@Override
	public ListLoadResult<GrupoSalaDTO> getList(BasePagingLoadConfig loadConfig) {
		return getBuscaList(null, loadConfig.get("query").toString(), null, loadConfig);
	}

}
