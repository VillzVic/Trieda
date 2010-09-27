package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.TipoDisciplina;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.TipoDisciplinaDTO;
import com.gapso.web.trieda.client.services.DisciplinasService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
@Transactional
public class DisciplinasServiceImpl extends RemoteServiceServlet implements DisciplinasService {

	private static final long serialVersionUID = -4850774141421616870L;

	@Override
	public PagingLoadResult<DisciplinaDTO> getBuscaList(String nome, String codigo, TipoDisciplinaDTO tipoDisciplinaDTO, PagingLoadConfig config) {
		List<DisciplinaDTO> list = new ArrayList<DisciplinaDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		TipoDisciplina tipoDisciplina = null;
		if(tipoDisciplinaDTO != null) {
			tipoDisciplina = ConvertBeans.toTipoDisciplina(tipoDisciplinaDTO);
		}
		for(Disciplina disciplina : Disciplina.findByCodigoLikeAndNomeLikeAndTipo(nome, codigo, tipoDisciplina, config.getOffset(), config.getLimit(), orderBy)) {
			DisciplinaDTO disciplinaDTO = ConvertBeans.toDisciplinaDTO(disciplina);
			list.add(disciplinaDTO);
		}
		BasePagingLoadResult<DisciplinaDTO> result = new BasePagingLoadResult<DisciplinaDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Disciplina.count());
		return result;
	}
	
	@Override
	public void save(DisciplinaDTO disciplinaDTO) {
		Disciplina disciplina = ConvertBeans.toDisciplina(disciplinaDTO);
		if(disciplina.getId() != null && disciplina.getId() > 0) {
			disciplina.merge();
		} else {
			disciplina.persist();
		}
	}
	
	@Override
	public void remove(List<DisciplinaDTO> disciplinaDTOList) {
		for(DisciplinaDTO disciplinaDTO : disciplinaDTOList) {
			ConvertBeans.toDisciplina(disciplinaDTO).remove();
		}
	}
	
	
	@Override
	public TipoDisciplinaDTO getTipoDisciplina(Long id) {
		return ConvertBeans.toTipoDisciplinaDTO(TipoDisciplina.find(id));
	}
	
	@Override
	public ListLoadResult<TipoDisciplinaDTO> getTipoDisciplinaList() {
		// TODO REMOVER AS LINHAS DE BAIXO
		if(TipoDisciplina.count() == 0) {
			TipoDisciplina tipo1 = new TipoDisciplina();
			tipo1.setNome("Presencial");
			tipo1.persist();
			
			TipoDisciplina tipo2 = new TipoDisciplina();
			tipo2.setNome("Telepresencial");
			tipo2.persist();
			
			TipoDisciplina tipo3 = new TipoDisciplina();
			tipo3.setNome("Online");
			tipo3.persist();
		}
		
		List<TipoDisciplinaDTO> list = new ArrayList<TipoDisciplinaDTO>();
		for(TipoDisciplina tipo : TipoDisciplina.findAll()) {
			list.add(ConvertBeans.toTipoDisciplinaDTO(tipo));
		}
		return new BaseListLoadResult<TipoDisciplinaDTO>(list);
	}
}
