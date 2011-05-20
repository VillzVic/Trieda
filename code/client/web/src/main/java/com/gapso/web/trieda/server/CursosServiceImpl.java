package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.TipoCurso;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.ResumoCursoDTO;
import com.gapso.web.trieda.shared.dtos.TipoCursoDTO;
import com.gapso.web.trieda.shared.services.CursosService;
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
	public ListLoadResult<CursoDTO> getListByCampus(CampusDTO campusDTO, List<CursoDTO> retirarCursosDTO) {
		List<CursoDTO> list = new ArrayList<CursoDTO>();
		
		Campus campus = (campusDTO == null)? null : ConvertBeans.toCampus(campusDTO);
		
		for(Curso curso : Curso.findByCampus(campus)) {
			list.add(ConvertBeans.toCursoDTO(curso));
		}
		
		list.removeAll(retirarCursosDTO);
		
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

	@Override
	public List<ResumoCursoDTO> getResumos(CenarioDTO cenarioDTO, CampusDTO campusDTO) {
		Cenario cenario = Cenario.find(cenarioDTO.getId());
		Campus campus = Campus.findByCenario(cenario).get(0);
	
		List<Oferta> ofertas = new ArrayList<Oferta>(campus.getOfertas());
		Collections.sort(ofertas);
		
		Map<String, AtendimentoTatico> atendimentoTaticoMap = new HashMap<String, AtendimentoTatico>();
		
		Map<String, ResumoCursoDTO> nivel1Map = new HashMap<String, ResumoCursoDTO>();
		Map<String, Map<String, ResumoCursoDTO>> nivel2Map = new HashMap<String, Map<String, ResumoCursoDTO>>();
		Map<String, Map<String, Map<String, ResumoCursoDTO>>> nivel3Map = new HashMap<String, Map<String, Map<String, ResumoCursoDTO>>>();
		
		for (Oferta oferta : ofertas) {
			Curso curso = oferta.getCurriculo().getCurso();
			Turno turno = oferta.getTurno();
			
			List<AtendimentoTatico> atendimentoTaticoList = AtendimentoTatico.findAllBy(oferta);
			for(AtendimentoTatico atendimentoTatico : atendimentoTaticoList) {
				
				String key = atendimentoTatico.getNaturalKey();
				if(!atendimentoTaticoMap.containsKey(key)) {
					atendimentoTaticoMap.put(key, atendimentoTatico);
				}
				
				Curriculo curriculo = oferta.getCurriculo();
				Disciplina disciplina = atendimentoTatico.getDisciplina();
				
				ResumoCursoDTO resumoDTO = new ResumoCursoDTO();
				resumoDTO.setOfertaId(oferta.getId());
				resumoDTO.setCampusId(campus.getId());
				resumoDTO.setCampusString(campus.getNome());
				resumoDTO.setTurnoId(turno.getId());
				resumoDTO.setTurnoString(turno.getNome());
				resumoDTO.setCursoId(curso.getId());
				resumoDTO.setCursoString(curso.getNome());
				resumoDTO.setMatrizCurricularId(curriculo.getId());
				resumoDTO.setMatrizCurricularString(curriculo.getCodigo());
				resumoDTO.setPeriodo(oferta.getCurriculo().getPeriodo(atendimentoTatico.getDisciplina()));
				resumoDTO.setQuantidadeAlunos(atendimentoTatico.getQuantidadeAlunos());
				resumoDTO.setDisciplinaId(disciplina.getId());
				resumoDTO.setDisciplinaString(disciplina.getNome());
				resumoDTO.setTurma(atendimentoTatico.getTurma());
				resumoDTO.setTipoCreditoTeorico(atendimentoTatico.getCreditosTeorico() > 0);
				resumoDTO.setCreditos(atendimentoTatico.getTotalCreditos());
				
				createResumoNivel1(nivel1Map, nivel2Map, nivel3Map, resumoDTO);
				createResumoNivel2(nivel2Map, nivel3Map, resumoDTO);
				createResumoNivel3(nivel3Map, resumoDTO);
			}
			
		}
		calculaResumo3(nivel3Map, atendimentoTaticoMap);
		calculaResumo1e2(nivel1Map, nivel2Map, nivel3Map);
		return createResumoEstrutura(nivel1Map, nivel2Map, nivel3Map);
	}
	
	private List<ResumoCursoDTO> createResumoEstrutura(Map<String, ResumoCursoDTO> map1, Map<String, Map<String, ResumoCursoDTO>> map2, Map<String, Map<String, Map<String, ResumoCursoDTO>>> map3) {
		List<ResumoCursoDTO> list = new ArrayList<ResumoCursoDTO>();
		for(String key1 : map1.keySet()) {
			ResumoCursoDTO rc1DTO = map1.get(key1);
			list.add(rc1DTO);
			for(String key2 : map2.get(key1).keySet()) {
				ResumoCursoDTO rc2DTO = map2.get(key1).get(key2);
				rc1DTO.add(rc2DTO);
				for(String key3 : map3.get(key1).get(key2).keySet()) {
					ResumoCursoDTO rc3DTO = map3.get(key1).get(key2).get(key3);
					rc2DTO.add(rc3DTO);
				}
			}
		}
		return list;
	}
	
	private void calculaResumo3(Map<String, Map<String, Map<String, ResumoCursoDTO>>> map3, Map<String, AtendimentoTatico> atendimentoTaticoMap) {
		for(String key1 : map3.keySet()) {
			for(String key2 : map3.get(key1).keySet()) {
				for(String key3 : map3.get(key1).get(key2).keySet()) {
					ResumoCursoDTO resumo3DTO = map3.get(key1).get(key2).get(key3);
					AtendimentoTatico atendimentoTatico = atendimentoTaticoMap.get(key3);
					Campus campus = atendimentoTatico.getOferta().getCampus();
					Double rateio = 100.0/100; // TODO Arrumar este número
					Double docente = campus.getValorCredito();
					Double receita = atendimentoTatico.getOferta().getReceita();
					int qtdAlunos = atendimentoTatico.getQuantidadeAlunos();
					int creditos = atendimentoTatico.getTotalCreditos();
					resumo3DTO.setRateio(rateio);
					resumo3DTO.setCustoDocente(creditos * docente * rateio * 4.5 * 6);
					resumo3DTO.setReceita(receita * creditos * qtdAlunos * 4.5 * 6);
					resumo3DTO.setMargem(resumo3DTO.getReceita() - resumo3DTO.getCustoDocente());
					resumo3DTO.setMargemPercente(resumo3DTO.getMargem() / resumo3DTO.getReceita());
				}
			}
		}
	}
	
	private void calculaResumo1e2(Map<String, ResumoCursoDTO> map1, Map<String, Map<String, ResumoCursoDTO>> map2, Map<String, Map<String, Map<String, ResumoCursoDTO>>> map3) {
		for(String key1 : map3.keySet()) {
			ResumoCursoDTO rc1 = map1.get(key1);
			rc1.setCustoDocente(0.0);
			rc1.setReceita(0.0);
			rc1.setMargem(0.0);
			rc1.setMargemPercente(0.0);
			for(String key2 : map3.get(key1).keySet()) {
				ResumoCursoDTO rc2 = map2.get(key1).get(key2);
				rc2.setCustoDocente(0.0);
				rc2.setReceita(0.0);
				rc2.setMargem(0.0);
				rc2.setMargemPercente(0.0);
				for(String key3 : map3.get(key1).get(key2).keySet()) {
					ResumoCursoDTO rc3 = map3.get(key1).get(key2).get(key3);
					rc1.setCustoDocente(rc1.getCustoDocente() + rc3.getCustoDocente());
					rc2.setCustoDocente(rc2.getCustoDocente() + rc3.getCustoDocente());
					
					rc1.setReceita(rc1.getReceita() + rc3.getReceita());
					rc2.setReceita(rc2.getReceita() + rc3.getReceita());
					
					rc1.setMargem(rc1.getMargem() + rc3.getMargem());
					rc2.setMargem(rc2.getMargem() + rc3.getMargem());
					
					rc1.setMargemPercente(rc1.getMargem() / rc1.getReceita());
					rc2.setMargemPercente(rc2.getMargem() / rc2.getReceita());
				}
			}
		}
	}
	
	private void createResumoNivel1(Map<String, ResumoCursoDTO> map1, Map<String, Map<String, ResumoCursoDTO>> map2, Map<String, Map<String, Map<String, ResumoCursoDTO>>> map3, ResumoCursoDTO resumoCursoDTO) {
		String key1 = resumoCursoDTO.getCampusId() +"-"+ resumoCursoDTO.getTurnoId() +"-"+ resumoCursoDTO.getCursoId(); 
		if(!map1.containsKey(key1)) {
			ResumoCursoDTO resumoCursoDTONew = new ResumoCursoDTO();
			resumoCursoDTONew.setCampusId(resumoCursoDTO.getCampusId());
			resumoCursoDTONew.setCampusString(resumoCursoDTO.getCampusString());
			resumoCursoDTONew.setTurnoId(resumoCursoDTO.getTurnoId());
			resumoCursoDTONew.setTurnoString(resumoCursoDTO.getTurnoString());
			resumoCursoDTONew.setCursoId(resumoCursoDTO.getCursoId());
			resumoCursoDTONew.setCursoString(resumoCursoDTO.getCursoString());
			map1.put(key1, resumoCursoDTONew);
			map2.put(key1, new HashMap<String, ResumoCursoDTO>());
			map3.put(key1, new HashMap<String, Map<String, ResumoCursoDTO>>());
			
		}
	}
	private void createResumoNivel2(Map<String, Map<String, ResumoCursoDTO>> map2, Map<String, Map<String, Map<String, ResumoCursoDTO>>> map3, ResumoCursoDTO resumoCursoDTO) {
		String key1 = resumoCursoDTO.getCampusId() +"-"+ resumoCursoDTO.getTurnoId() +"-"+ resumoCursoDTO.getCursoId();
		String key2 = key1 +"-"+ resumoCursoDTO.getMatrizCurricularId() +"-"+ resumoCursoDTO.getPeriodo();
		if(!map2.get(key1).containsKey(key2)) {
			ResumoCursoDTO resumoCursoDTONew = new ResumoCursoDTO();
			resumoCursoDTONew.setCampusId(resumoCursoDTO.getCampusId());
			resumoCursoDTONew.setCampusString(resumoCursoDTO.getCampusString());
			resumoCursoDTONew.setTurnoId(resumoCursoDTO.getTurnoId());
			resumoCursoDTONew.setTurnoString(resumoCursoDTO.getTurnoString());
			resumoCursoDTONew.setCursoId(resumoCursoDTO.getCursoId());
			resumoCursoDTONew.setCursoString(resumoCursoDTO.getCursoString());
			resumoCursoDTONew.setMatrizCurricularId(resumoCursoDTO.getMatrizCurricularId());
			resumoCursoDTONew.setMatrizCurricularString(resumoCursoDTO.getMatrizCurricularString());
			resumoCursoDTONew.setPeriodo(resumoCursoDTO.getPeriodo());
			map2.get(key1).put(key2, resumoCursoDTONew);
			map3.get(key1).put(key2, new HashMap<String, ResumoCursoDTO>());
		}
	}
	private void createResumoNivel3(Map<String, Map<String, Map<String, ResumoCursoDTO>>> map, ResumoCursoDTO resumoCursoDTO) {
		String key1 = resumoCursoDTO.getCampusId() +"-"+ resumoCursoDTO.getTurnoId() +"-"+ resumoCursoDTO.getCursoId();
		String key2 = key1 +"-"+ resumoCursoDTO.getMatrizCurricularId() +"-"+ resumoCursoDTO.getPeriodo();
		String key3 = key2 +"-"+ resumoCursoDTO.getDisciplinaId() +"-"+ resumoCursoDTO.getTurma() +"-"+ resumoCursoDTO.getTipoCreditoTeorico();
		if(!map.get(key1).get(key2).containsKey(key3)) {
			ResumoCursoDTO resumoCursoDTONew = new ResumoCursoDTO();
			resumoCursoDTONew.setCampusId(resumoCursoDTO.getCampusId());
			resumoCursoDTONew.setCampusString(resumoCursoDTO.getCampusString());
			resumoCursoDTONew.setTurnoId(resumoCursoDTO.getTurnoId());
			resumoCursoDTONew.setTurnoString(resumoCursoDTO.getTurnoString());
			resumoCursoDTONew.setCursoId(resumoCursoDTO.getCursoId());
			resumoCursoDTONew.setCursoString(resumoCursoDTO.getCursoString());
			resumoCursoDTONew.setMatrizCurricularId(resumoCursoDTO.getMatrizCurricularId());
			resumoCursoDTONew.setMatrizCurricularString(resumoCursoDTO.getMatrizCurricularString());
			resumoCursoDTONew.setPeriodo(resumoCursoDTO.getPeriodo());
			resumoCursoDTONew.setDisciplinaId(resumoCursoDTO.getDisciplinaId());
			resumoCursoDTONew.setDisciplinaString(resumoCursoDTO.getDisciplinaString());
			resumoCursoDTONew.setQuantidadeAlunos(resumoCursoDTO.getQuantidadeAlunos());
			resumoCursoDTONew.setTurma(resumoCursoDTO.getTurma());
			resumoCursoDTONew.setTipoCreditoTeorico(resumoCursoDTO.getTipoCreditoTeorico());
			resumoCursoDTONew.setCreditos(resumoCursoDTO.getCreditos());
			map.get(key1).get(key2).put(key3, resumoCursoDTONew);
		}
	}
	
}
