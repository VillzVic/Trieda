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
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.DeslocamentoUnidadeDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.gapso.web.trieda.client.services.UnidadesService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
@Transactional
public class UnidadesServiceImpl extends RemoteServiceServlet implements UnidadesService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public UnidadeDTO getUnidade(Long id) {
		return ConvertBeans.toUnidadeDTO(Unidade.find(id));
	}
	
	@Override
	public PagingLoadResult<UnidadeDTO> getList(PagingLoadConfig config) {
		List<UnidadeDTO> list = new ArrayList<UnidadeDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(Unidade unidade : Unidade.find(config.getOffset(), config.getLimit(), orderBy)) {
			UnidadeDTO unidadeDTO = ConvertBeans.toUnidadeDTO(unidade);
			unidadeDTO.setCapSalas(Unidade.getCapacidadeMedia(unidade));
			list.add(unidadeDTO);
		}
		BasePagingLoadResult<UnidadeDTO> result = new BasePagingLoadResult<UnidadeDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Unidade.count());
		return result;
	}

	@Override
	public ListLoadResult<UnidadeDTO> getList(BasePagingLoadConfig loadConfig) {
		return getBuscaList(null, null, loadConfig.get("query").toString(), loadConfig);
	}
	
	@Override
	public PagingLoadResult<UnidadeDTO> getBuscaList(CampusDTO campusDTO, String nome, String codigo, PagingLoadConfig config) {
		List<UnidadeDTO> list = new ArrayList<UnidadeDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		Campus campus = null;
		if(campusDTO != null) {
			campus = ConvertBeans.toCampus(campusDTO);
		}
		for(Unidade unidade : Unidade.findByCampusCodigoLikeAndNomeLikeAndCodigoLike(campus, nome, codigo, config.getOffset(), config.getLimit(), orderBy)) {
			UnidadeDTO unidadeDTO = ConvertBeans.toUnidadeDTO(unidade);
			unidadeDTO.setCapSalas(Unidade.getCapacidadeMedia(unidade));
			list.add(unidadeDTO);
		}
		BasePagingLoadResult<UnidadeDTO> result = new BasePagingLoadResult<UnidadeDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Turno.count());
		return result;
	}
	
	@Override
	public ListLoadResult<UnidadeDTO> getList() {
		List<UnidadeDTO> list = new ArrayList<UnidadeDTO>();
		for(Unidade unidade : Unidade.findAll()) {
			list.add(ConvertBeans.toUnidadeDTO(unidade));
		}
		return new BaseListLoadResult<UnidadeDTO>(list);
	}
	
	@Override
	public void save(UnidadeDTO unidadeDTO) {
		Unidade unidade = ConvertBeans.toUnidade(unidadeDTO);
		if(unidade.getId() != null && unidade.getId() > 0) {
			unidade.merge();
		} else {
			unidade.persist();
		}
	}
	
	@Override
	public void remove(List<UnidadeDTO> unidadeDTOList) {
		for(UnidadeDTO unidadeDTO : unidadeDTOList) {
			ConvertBeans.toUnidade(unidadeDTO).remove();
		}
	}
	
	@Override
	public List<DeslocamentoUnidadeDTO> getDeslocamento(CampusDTO campusDTO) {
		List<DeslocamentoUnidadeDTO> list = new ArrayList<DeslocamentoUnidadeDTO>();
		if(campusDTO != null) {
			Campus campus = Campus.find(campusDTO.getId());
			for(Unidade unidade : campus.getUnidades()) {
				list.add(ConvertBeans.toDeslocamentoUnidadeDTO(unidade));
			}
		}
		return list;
	}
	
	@Override
	public void saveDeslocamento(List<DeslocamentoUnidadeDTO> list) {
		for(DeslocamentoUnidadeDTO deslocamentoUnidadeDTO : list) {
			Unidade unidade = ConvertBeans.toDeslocamentoUnidade(deslocamentoUnidadeDTO);
			unidade.merge();
		}
	}
}
