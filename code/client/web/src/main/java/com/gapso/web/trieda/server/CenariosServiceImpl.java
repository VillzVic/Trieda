package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.web.trieda.server.util.CenarioUtil;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.gapso.web.trieda.shared.services.CenariosService;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
@Transactional
public class CenariosServiceImpl extends RemoteServiceServlet implements CenariosService {

	private static final long serialVersionUID = -5951529933566541220L;

	@Override
	public CenarioDTO getMasterData() {
		Cenario cenario = Cenario.findMasterData();
		if(cenario == null) {
			cenario = new Cenario();
			cenario.setOficial(false);
			cenario.setMasterData(true);
			cenario.setNome("MASTER DATA");
			cenario.setAno(1);
			cenario.setSemestre(1);
			cenario.setComentario("MASTER DATA");
			cenario.persist();
			cenario = Cenario.findMasterData();
		}
		return ConvertBeans.toCenarioDTO(cenario);
	}

	@Override
	public CenarioDTO getCenario(Long id) {
		return ConvertBeans.toCenarioDTO(Cenario.find(id));
	}
	
	@Override
	public PagingLoadResult<CenarioDTO> getList(PagingLoadConfig config) {
		List<CenarioDTO> list = new ArrayList<CenarioDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(Cenario cenario : Cenario.find(config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toCenarioDTO(cenario));
		}
		BasePagingLoadResult<CenarioDTO> result = new BasePagingLoadResult<CenarioDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Cenario.count());
		return result;
	}

	@Override
	public PagingLoadResult<CenarioDTO> getBuscaList(Integer ano, Integer semestre, PagingLoadConfig config) {
		List<CenarioDTO> list = new ArrayList<CenarioDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(Cenario cenario : Cenario.findByAnoAndSemestre(ano, semestre, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toCenarioDTO(cenario));
		}
		BasePagingLoadResult<CenarioDTO> result = new BasePagingLoadResult<CenarioDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Cenario.count());
		return result;
	}
		
	@Override
	@Transactional
	public void editar(CenarioDTO cenarioDTO) {
		Cenario cenario = ConvertBeans.toCenario(cenarioDTO);
		cenario.merge();
	}
	
	@Override
	@Transactional
	public void criar(CenarioDTO cenarioDTO, SemanaLetivaDTO semanaLetivaDTO, Set<CampusDTO> campiDTO) {
		Cenario cenario = ConvertBeans.toCenario(cenarioDTO);
		SemanaLetiva semanaLetiva = ConvertBeans.toSemanaLetiva(semanaLetivaDTO);
		Set<Campus> campi = new HashSet<Campus>();
		for(CampusDTO dto : campiDTO) {
			campi.add(ConvertBeans.toCampus(dto));
		}
			
		CenarioUtil cenarioUtil = new CenarioUtil();
		cenarioUtil.criarCenario(cenario, semanaLetiva, campi);
	}
	
	@Override
	@Transactional
	public void clonar(CenarioDTO cenarioDTO) {
		Cenario cenario = ConvertBeans.toCenario(cenarioDTO);
		CenarioUtil cenarioUtil = new CenarioUtil();
		cenarioUtil.clonarCenario(cenario);
	}
	
	@Override
	public void remove(List<CenarioDTO> cenarioDTOList) {
		for(CenarioDTO cenarioDTO : cenarioDTOList) {
			ConvertBeans.toCenario(cenarioDTO).remove();
		}
	}
	
	@Override
	public List<TreeNodeDTO> getResumos(CenarioDTO cenarioDTO) {
		Cenario cenario = Cenario.find(cenarioDTO.getId());
		List<TreeNodeDTO> list = new ArrayList<TreeNodeDTO>();
		list.add(new TreeNodeDTO("Total de Campi: <b>"+Campus.count(cenario)+"</b>"));
		list.add(new TreeNodeDTO("Total de Cursos: <b>"+Curso.count(cenario)+"</b>"));
		list.add(new TreeNodeDTO("Total de Matrizes Curriculares: <b>"+Curriculo.count(cenario)+"</b>"));
		list.add(new TreeNodeDTO("Total de Disciplinas: <b>"+Disciplina.count(cenario)+"</b>"));
		list.add(new TreeNodeDTO("Demanda Total: <b>"+Demanda.sumDemanda(cenario)+" Alunos</b>"));
		list.add(new TreeNodeDTO("Total de Salas de Aula: <b>"+Sala.countSalaDeAula(cenario)+"</b>"));
		list.add(new TreeNodeDTO("Total de Salas de Laborat&oacute;rio: <b>"+Sala.countLaboratorio(cenario)+"</b>"));
		
		return list;
	}

}
