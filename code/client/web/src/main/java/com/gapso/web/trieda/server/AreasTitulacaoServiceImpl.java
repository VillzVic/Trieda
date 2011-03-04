package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Set;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.Curso;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.services.AreasTitulacaoService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class AreasTitulacaoServiceImpl extends RemoteServiceServlet implements AreasTitulacaoService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public AreaTitulacaoDTO getAreaTitulacao(Long id) {
		return ConvertBeans.toAreaTitulacaoDTO(AreaTitulacao.find(id));
	}
	
	@Override
	public PagingLoadResult<AreaTitulacaoDTO> getBuscaList(String nome, String descricao, PagingLoadConfig config) {
		List<AreaTitulacaoDTO> list = new ArrayList<AreaTitulacaoDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(AreaTitulacao areaTitulacao : AreaTitulacao.findBy(nome, descricao, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toAreaTitulacaoDTO(areaTitulacao));
		}
		BasePagingLoadResult<AreaTitulacaoDTO> result = new BasePagingLoadResult<AreaTitulacaoDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(AreaTitulacao.count(nome, descricao));
		return result;
	}

	@Override
	public ListLoadResult<AreaTitulacaoDTO> getListAll() {
		List<AreaTitulacaoDTO> listDTO = new ArrayList<AreaTitulacaoDTO>();
		List<AreaTitulacao> list = AreaTitulacao.findAll();
		for(AreaTitulacao areaTitulacao : list) {
			listDTO.add(ConvertBeans.toAreaTitulacaoDTO(areaTitulacao));
		}
		return new BaseListLoadResult<AreaTitulacaoDTO>(listDTO);
	}
	
	@Override
	public void save(AreaTitulacaoDTO areaTitulacaoDTO) {
		AreaTitulacao areaTitulacao = ConvertBeans.toAreaTitulacao(areaTitulacaoDTO);
		if(areaTitulacao.getId() != null && areaTitulacao.getId() > 0) {
			areaTitulacao.merge();
		} else {
			areaTitulacao.persist();
		}
	}
	
	@Override
	public void remove(List<AreaTitulacaoDTO> areaTitulacaoDTOList) {
		for(AreaTitulacaoDTO areaTitulacaoDTO : areaTitulacaoDTOList) {
			ConvertBeans.toAreaTitulacao(areaTitulacaoDTO).remove();
		}
	}

	@Override
	public List<AreaTitulacaoDTO> getListVinculadas(CursoDTO cursoDTO) {
		if(cursoDTO == null) return Collections.<AreaTitulacaoDTO>emptyList();
		
		Curso curso = Curso.find(cursoDTO.getId());
		Set<AreaTitulacao> areaTitulacaoList = curso.getAreasTitulacao();
		List<AreaTitulacaoDTO> areaTitulacaoDTOList = new ArrayList<AreaTitulacaoDTO>(areaTitulacaoList.size());
		for(AreaTitulacao areaTitulacao : areaTitulacaoList) {
			areaTitulacaoDTOList.add(ConvertBeans.toAreaTitulacaoDTO(areaTitulacao));
		}
		return areaTitulacaoDTOList;
	}
	
	@Override
	public List<AreaTitulacaoDTO> getListNaoVinculadas(CursoDTO cursoDTO) {
		if(cursoDTO == null) return Collections.<AreaTitulacaoDTO>emptyList();
		
		Curso curso = Curso.find(cursoDTO.getId());
		Set<AreaTitulacao> areaTitulacaoList = curso.getAreasTitulacao();
		List<AreaTitulacao> naoAssociadasList = AreaTitulacao.findAll();
		naoAssociadasList.removeAll(areaTitulacaoList);
		
		List<AreaTitulacaoDTO> areaTitulacaoDTOList = new ArrayList<AreaTitulacaoDTO>(naoAssociadasList.size());
		for(AreaTitulacao areaTitulacao : naoAssociadasList) {
			areaTitulacaoDTOList.add(ConvertBeans.toAreaTitulacaoDTO(areaTitulacao));
		}
		
		return areaTitulacaoDTOList;
	}
	
	@Override
	public void vincula(CursoDTO cursoDTO, List<AreaTitulacaoDTO> areasTitulacaoDTO) {
		Curso curso = Curso.find(cursoDTO.getId());
		for(AreaTitulacaoDTO areaTitulacaoDTO : areasTitulacaoDTO) {
			AreaTitulacao area = AreaTitulacao.find(areaTitulacaoDTO.getId());
			area.getCursos().add(curso);
			area.merge();
		}
	}
	
	@Override
	public void desvincula(CursoDTO cursoDTO, List<AreaTitulacaoDTO> areasTitulacaoDTO) {
		Curso curso = Curso.find(cursoDTO.getId());
		for(AreaTitulacaoDTO areaTitulacaoDTO : areasTitulacaoDTO) {
			AreaTitulacao area = AreaTitulacao.find(areaTitulacaoDTO.getId());
			area.getCursos().remove(curso);
			area.merge();
		}
	}
	
}
