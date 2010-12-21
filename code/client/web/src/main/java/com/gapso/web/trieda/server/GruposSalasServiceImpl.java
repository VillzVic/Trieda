package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.GrupoSala;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.gapso.web.trieda.client.services.GruposSalasService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
@Transactional
public class GruposSalasServiceImpl extends RemoteServiceServlet implements GruposSalasService {

	private static final long serialVersionUID = 5250776996542788849L;
	
	@Override
	public ListLoadResult<GrupoSalaDTO> getListByUnidade(UnidadeDTO unidadeDTO) {
		Unidade unidade = Unidade.find(unidadeDTO.getId());
		Set<GrupoSala> gruposSalas = unidade.getGruposSalas();
		List<GrupoSalaDTO> gruposSalasDTO = new ArrayList<GrupoSalaDTO>(gruposSalas.size());
		for(GrupoSala gs : gruposSalas) {
			gruposSalasDTO.add(ConvertBeans.toGrupoSalaDTO(gs));
		}
		return new BaseListLoadResult<GrupoSalaDTO>(gruposSalasDTO);
	}

	@Override
	public ListLoadResult<GrupoSalaDTO> getList(BasePagingLoadConfig loadConfig) {
		return getBuscaList(null, loadConfig.get("query").toString(), null, loadConfig);
	}
	
	@Override
	public GrupoSalaDTO getGrupoSala(Long id) {
		return ConvertBeans.toGrupoSalaDTO(GrupoSala.find(id));
	}
	
	@Override
	public void save(GrupoSalaDTO grupoSalaDTO) {
		GrupoSala grupoSala = GrupoSala.find(grupoSalaDTO.getId());
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
	public ListLoadResult<SalaDTO> getSalas(GrupoSalaDTO grupoSalaDTO) {
		GrupoSala grupoSala = GrupoSala.find(grupoSalaDTO.getId());
		
		List<SalaDTO> list = new ArrayList<SalaDTO>();
		for(Sala sala : grupoSala.getSalas()) {
			list.add(ConvertBeans.toSalaDTO(sala));
		}
		
		return new BaseListLoadResult<SalaDTO>(list);
	}
	
	@Override
	public void saveSalas(List<SalaDTO> salaDTOList, GrupoSalaDTO grupoSalaDTO) {
		GrupoSala grupoSala = ConvertBeans.toGrupoSala(grupoSalaDTO);
		
		List<Sala> removerSalas = new ArrayList<Sala>();
		List<Sala> adicionarSalas = new ArrayList<Sala>();
		
		Set<Sala> salas = grupoSala.getSalas();
		
		for(SalaDTO sala2DTO : salaDTOList) {
			Sala sala2 = ConvertBeans.toSala(sala2DTO);
			boolean naoContem = true;
			for(Sala sala1 : salas) {
				if(sala2.getId().equals(sala1.getId())) {
					naoContem = false;
					break;
				}
			}
			if(naoContem) {
				adicionarSalas.add(sala2);
			} else {
				removerSalas.remove(sala2);
			}
		}
		
		salas.removeAll(removerSalas);
		salas.addAll(adicionarSalas);
		
		grupoSala.merge();
		
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
		List<GrupoSala> listGruposSalas = GrupoSala.findByNomeLikeAndCodigoLikeAndUnidade(nome, codigo, unidade, config.getOffset(), config.getLimit(), orderBy);
		for(GrupoSala grupoSala : listGruposSalas) {
			String salasString = "";
			Set<Sala> salas = grupoSala.getSalas();
			for(Sala sala : salas) {
				salasString += sala.getCodigo() + " (Num. "+sala.getNumero()+" | Cap."+sala.getCapacidade()+"), ";
			}
			GrupoSalaDTO gsDTO = ConvertBeans.toGrupoSalaDTO(grupoSala);
			gsDTO.setSalasString(salasString);
			gsDTO.setCampusString(grupoSala.getUnidade().getCampus().getCodigo());
			gsDTO.setName(gsDTO.getCodigo());
			gsDTO.setPath(gsDTO.getCodigo() + "/");
			list.add(gsDTO);
		}
		BasePagingLoadResult<GrupoSalaDTO> result = new BasePagingLoadResult<GrupoSalaDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(GrupoSala.count());
		return result;
	}

}
