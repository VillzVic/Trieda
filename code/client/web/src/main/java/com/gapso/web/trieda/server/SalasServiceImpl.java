package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.TipoSala;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.TipoSalaDTO;
import com.gapso.web.trieda.client.services.SalasService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class SalasServiceImpl extends RemoteServiceServlet implements SalasService {

	private static final long serialVersionUID = -5850050305078103981L;

	@Override
	public SalaDTO getSala(Long id) {
		return ConvertBeans.toSalaDTO(Sala.find(id));
	}
	
	@Override
	public TipoSalaDTO getTipoSala(Long id) {
		return ConvertBeans.toTipoSalaDTO(TipoSala.find(id));
	}
	
	@Override
	public PagingLoadResult<SalaDTO> getList(PagingLoadConfig config) {
		List<SalaDTO> list = new ArrayList<SalaDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(Sala sala : Sala.find(config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toSalaDTO(sala));
		}
		BasePagingLoadResult<SalaDTO> result = new BasePagingLoadResult<SalaDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Sala.count());
		return result;
	}
	
	@Override
	public ListLoadResult<SalaDTO> getAndaresList() {
		List<SalaDTO> list = new ArrayList<SalaDTO>();
		for(Sala sala : Sala.findAndaresAll()) {
			list.add(ConvertBeans.toSalaDTO(sala));
		}
		return new BaseListLoadResult<SalaDTO>(list);
	}
	
	@Override
	public ListLoadResult<SalaDTO> getSalasDoAndareList(List<String> andares) {
		List<SalaDTO> list = new ArrayList<SalaDTO>();
		for(Sala sala : Sala.findSalasDoAndarAll(andares)) {
			list.add(ConvertBeans.toSalaDTO(sala));
		}
		return new BaseListLoadResult<SalaDTO>(list);
	}
	
	@Override
	public ListLoadResult<SalaDTO> getList() {
		List<SalaDTO> list = new ArrayList<SalaDTO>();
		for(Sala sala : Sala.findAll()) {
			list.add(ConvertBeans.toSalaDTO(sala));
		}
		return new BaseListLoadResult<SalaDTO>(list);
	}
	
	@Override
	public void save(SalaDTO salaDTO) {
		Sala sala = ConvertBeans.toSala(salaDTO);
		if(sala.getId() != null && sala.getId() > 0) {
			sala.merge();
		} else {
			sala.persist();
		}
	}
	
	@Override
	public void remove(List<SalaDTO> salaDTOList) {
		for(SalaDTO salaDTO : salaDTOList) {
			ConvertBeans.toSala(salaDTO).remove();
		}
	}

	@Override
	public ListLoadResult<TipoSalaDTO> getTipoSalaList() {
		// TODO REMOVER AS LINHAS DE BAIXO
		if(TipoSala.count() == 0) {
			TipoSala tipo1 = new TipoSala();
			tipo1.setNome("Sala de Aula");
			tipo1.setDescricao("Sala de Aula");
			tipo1.persist();
			
			TipoSala tipo2 = new TipoSala();
			tipo2.setNome("Laboratório");
			tipo2.setDescricao("Laboratório");
			tipo2.persist();
			
			TipoSala tipo3 = new TipoSala();
			tipo3.setNome("Auditório");
			tipo3.setDescricao("Auditório");
			tipo3.persist();
		}
		
		
		List<TipoSalaDTO> list = new ArrayList<TipoSalaDTO>();
		for(TipoSala tipo : TipoSala.findAll()) {
			list.add(ConvertBeans.toTipoSalaDTO(tipo));
		}
		return new BaseListLoadResult<TipoSalaDTO>(list);
	}
	
}
