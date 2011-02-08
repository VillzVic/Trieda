package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.HashSet;
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
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.GrupoSala;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Incompatibilidade;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoDisciplina;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.DisciplinaIncompativelDTO;
import com.gapso.web.trieda.client.mvp.model.FileModel;
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.client.mvp.model.OfertaDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
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
	public DisciplinaDTO getDisciplina(Long id) {
		if(id == null) return null;
		return ConvertBeans.toDisciplinaDTO(Disciplina.find(id));
	}
	
	@Override
	public List<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(DisciplinaDTO disciplinaDTO) {
		List<HorarioDisponivelCenario> list = new ArrayList<HorarioDisponivelCenario>(Disciplina.find(disciplinaDTO.getId()).getHorarios());
		List<HorarioDisponivelCenarioDTO> listDTO = ConvertBeans.toHorarioDisponivelCenarioDTO(list);
		
		return listDTO;
	}
	
	@Override
	public void saveHorariosDisponiveis(DisciplinaDTO disciplinaDTO, List<HorarioDisponivelCenarioDTO> listDTO) {
		List<HorarioDisponivelCenario> listSelecionados = ConvertBeans.toHorarioDisponivelCenario(listDTO);
		Disciplina disciplina = Disciplina.find(disciplinaDTO.getId());
		List<HorarioDisponivelCenario> adicionarList = new ArrayList<HorarioDisponivelCenario> (listSelecionados);
		adicionarList.removeAll(disciplina.getHorarios());
		List<HorarioDisponivelCenario> removerList = new ArrayList<HorarioDisponivelCenario> (disciplina.getHorarios());
		removerList.removeAll(listSelecionados);
		for(HorarioDisponivelCenario o : removerList) {
			o.getDisciplinas().remove(disciplina);
			o.merge();
		}
		for(HorarioDisponivelCenario o : adicionarList) {
			o.getDisciplinas().add(disciplina);
			o.merge();
		}
	}
	
	@Override
	public ListLoadResult<DisciplinaDTO> getListByCursos(List<CursoDTO> cursosDTO) {
		List<DisciplinaDTO> list = new ArrayList<DisciplinaDTO>();
		List<Curso> cursos = new ArrayList<Curso>();
		for(CursoDTO cursoDTO : cursosDTO) {
			cursos.add(ConvertBeans.toCurso(cursoDTO));
		}
		List<Disciplina> disciplinas = Disciplina.findByCursos(cursos);
		for(Disciplina disciplina : disciplinas) {
			list.add(ConvertBeans.toDisciplinaDTO(disciplina));
		}
		return new BaseListLoadResult<DisciplinaDTO>(list);
	}
	
	public ListLoadResult<DisciplinaDTO> getListBySalas(List<SalaDTO> salasDTO) {
		List<DisciplinaDTO> list = new ArrayList<DisciplinaDTO>();
		List<Sala> salas = new ArrayList<Sala>();
		for(SalaDTO salaDTO : salasDTO) {
			salas.add(ConvertBeans.toSala(salaDTO));
		}
		List<Disciplina> disciplinas = Disciplina.findBySalas(salas);
		for(Disciplina disciplina : disciplinas) {
			list.add(ConvertBeans.toDisciplinaDTO(disciplina));
		}
		return new BaseListLoadResult<DisciplinaDTO>(list);
	}
	
	@Override
	public ListLoadResult<DisciplinaDTO> getList(BasePagingLoadConfig loadConfig) {
		return getBuscaList(null, loadConfig.get("query").toString(), null, loadConfig);
	}
	
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
		List<Disciplina> disciplinas = Disciplina.findByCodigoLikeAndNomeLikeAndTipo(codigo, nome, tipoDisciplina, config.getOffset(), config.getLimit(), orderBy);
		for(Disciplina disciplina : disciplinas) {
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
			// TODO Pegar a semana letiva do cenario da disciplina
			Set<HorarioAula> horariosAula = SemanaLetiva.findAll().get(0).getHorariosAula();
			for(HorarioAula horarioAula : horariosAula) {
				Set<HorarioDisponivelCenario> horariosDisponiveis = horarioAula.getHorariosDisponiveisCenario();
				for(HorarioDisponivelCenario horarioDisponivel : horariosDisponiveis) {
					horarioDisponivel.getDisciplinas().add(disciplina);
					horarioDisponivel.merge();
				}
			}
		}
	}
	
	@Override
	public void remove(List<DisciplinaDTO> disciplinaDTOList) {
		for(DisciplinaDTO disciplinaDTO : disciplinaDTOList) {
			Disciplina.find(disciplinaDTO.getId()).remove();
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

	@Override
	@Transactional
	public List<FileModel> getFolderChildren(FileModel model) {
		List<FileModel> listDTO = new ArrayList<FileModel>();
		if(model != null) {
			if(model instanceof OfertaDTO) {
				Curriculo curriculo = Curriculo.find(((OfertaDTO) model).getMatrizCurricularId());
				Set<CurriculoDisciplina> disciplinas = curriculo.getDisciplinas();
				Set<Integer> periodos = new HashSet<Integer>();
				for(CurriculoDisciplina cd : disciplinas) {
					CurriculoDisciplinaDTO curriculoDisciplinaDTO = ConvertBeans.toCurriculoDisciplinaDTO(cd);
					curriculoDisciplinaDTO.setName("Período "+curriculoDisciplinaDTO.getPeriodo().toString());
					curriculoDisciplinaDTO.setPath(model.getPath() + curriculoDisciplinaDTO.getPeriodo().toString() + "/");
					if(periodos.add(curriculoDisciplinaDTO.getPeriodo())) {
						listDTO.add(curriculoDisciplinaDTO);
					}
				}
			} else if(model instanceof CurriculoDisciplinaDTO) {
				Integer periodo = ((CurriculoDisciplinaDTO) model).getPeriodo();
				Curriculo curriculo = Curriculo.find(((CurriculoDisciplinaDTO) model).getCurriculoId());
				Set<CurriculoDisciplina> disciplinas = curriculo.getDisciplinas();
				List<CurriculoDisciplina> list = new ArrayList<CurriculoDisciplina>(disciplinas);
				for(CurriculoDisciplina cd : list) {
					CurriculoDisciplinaDTO curriculoDisciplinaDTO = ConvertBeans.toCurriculoDisciplinaDTO(cd);
					curriculoDisciplinaDTO.setName(curriculoDisciplinaDTO.getDisciplinaString());
					curriculoDisciplinaDTO.setPath(model.getPath() + curriculoDisciplinaDTO.getDisciplinaString() + "/");
					curriculoDisciplinaDTO.setFolha(true);
					if(cd.getPeriodo().equals(periodo)) {
						listDTO.add(curriculoDisciplinaDTO);
					}
				}
			}
		}
		return listDTO;
	}
	
	public List<FileModel> getOfertasByTreeSalas(SalaDTO salaDTO) {
		List<FileModel> list = new ArrayList<FileModel>();
		Sala sala = Sala.find(salaDTO.getId());
		List<Oferta> ofertas = Oferta.findAllBy(sala);
		for(Oferta oferta : ofertas) {
			OfertaDTO dto = ConvertBeans.toOfertaDTO(oferta);
			dto.setName(dto.getMatrizCurricularString() + " (" + dto.getCursoString() + ")");
			dto.setPath(salaDTO.getPath() + dto.getMatrizCurricularString() + "/");
			list.add(dto);
		}
		return list;
	}

	public List<FileModel> getPeriodosByTreeSalas(SalaDTO salaDTO, OfertaDTO ofertaDTO) {
		List<FileModel> list = new ArrayList<FileModel>();
		
		Sala sala = Sala.find(salaDTO.getId());
		Oferta oferta = Oferta.find(ofertaDTO.getId());
		List<CurriculoDisciplina> curriculoDisciplinas = CurriculoDisciplina.findAllPeriodosBy(sala, oferta);
		for(CurriculoDisciplina CurriculoDisciplina : curriculoDisciplinas) {
			CurriculoDisciplinaDTO dto = ConvertBeans.toCurriculoDisciplinaDTO(CurriculoDisciplina); 
			dto.setName("Periodo " + dto.getPeriodo());
			dto.setPath(ofertaDTO.getPath() + dto.getPeriodo() + "/");
			list.add(dto);
		}
		return list;
	}
	
	@Override
	public List<FileModel> getDisciplinasByTreeSalas(SalaDTO salaDTO, OfertaDTO ofertaDTO, CurriculoDisciplinaDTO curriculoDisciplinaDTO) {
		if(ofertaDTO == null)				return getOfertasByTreeSalas(salaDTO);
		if(curriculoDisciplinaDTO == null) 	return getPeriodosByTreeSalas(salaDTO, ofertaDTO);
		
		List<FileModel> list = new ArrayList<FileModel>();
		
		Sala sala = Sala.find(salaDTO.getId());
		Oferta oferta = Oferta.find(ofertaDTO.getId());
		List<CurriculoDisciplina> curriculoDisciplinas = CurriculoDisciplina.findBy(sala, oferta, curriculoDisciplinaDTO.getPeriodo());
		for(CurriculoDisciplina curriculoDisciplina : curriculoDisciplinas) {
			CurriculoDisciplinaDTO dto = ConvertBeans.toCurriculoDisciplinaDTO(curriculoDisciplina); 
			dto.setName(dto.getDisciplinaString());
			dto.setPath(curriculoDisciplinaDTO.getPath() + dto.getDisciplinaString() + "/");
			dto.setFolha(true);
			list.add(dto);
		}
		return list;
	}
	
	public List<FileModel> getOfertasByTreeSalas(GrupoSalaDTO grupoSalaDTO) {
		List<FileModel> list = new ArrayList<FileModel>();
		GrupoSala grupoSala = GrupoSala.find(grupoSalaDTO.getId());
		List<Oferta> ofertas = Oferta.findAllBy(grupoSala);
		for(Oferta oferta : ofertas) {
			OfertaDTO dto = ConvertBeans.toOfertaDTO(oferta);
			dto.setName(dto.getMatrizCurricularString() + " (" + dto.getCursoString() + ")");
			dto.setPath(grupoSalaDTO.getPath() + dto.getMatrizCurricularString() + "/");
			list.add(dto);
		}
		return list;
	}
	
	public List<FileModel> getPeriodosByTreeSalas(GrupoSalaDTO grupoSalaDTO, OfertaDTO ofertaDTO) {
		List<FileModel> list = new ArrayList<FileModel>();
		
		GrupoSala grupoSala = GrupoSala.find(grupoSalaDTO.getId());
		Oferta oferta = Oferta.find(ofertaDTO.getId());
		List<CurriculoDisciplina> curriculoDisciplinas = CurriculoDisciplina.findAllPeriodosBy(grupoSala, oferta);
		for(CurriculoDisciplina CurriculoDisciplina : curriculoDisciplinas) {
			CurriculoDisciplinaDTO dto = ConvertBeans.toCurriculoDisciplinaDTO(CurriculoDisciplina); 
			dto.setName("Periodo " + dto.getPeriodo());
			dto.setPath(ofertaDTO.getPath() + dto.getPeriodo() + "/");
			list.add(dto);
		}
		return list;
	}
	
	@Override
	public List<FileModel> getDisciplinasByTreeSalas(GrupoSalaDTO grupoSalaDTO, OfertaDTO ofertaDTO, CurriculoDisciplinaDTO curriculoDisciplinaDTO) {
		if(ofertaDTO == null)				return getOfertasByTreeSalas(grupoSalaDTO);
		if(curriculoDisciplinaDTO == null) 	return getPeriodosByTreeSalas(grupoSalaDTO, ofertaDTO);
		
		List<FileModel> list = new ArrayList<FileModel>();
		
		GrupoSala grupoSala = GrupoSala.find(grupoSalaDTO.getId());
		Oferta oferta = Oferta.find(ofertaDTO.getId());
		List<CurriculoDisciplina> curriculoDisciplinas = CurriculoDisciplina.findBy(grupoSala, oferta, curriculoDisciplinaDTO.getPeriodo());
		for(CurriculoDisciplina curriculoDisciplina : curriculoDisciplinas) {
			CurriculoDisciplinaDTO dto = ConvertBeans.toCurriculoDisciplinaDTO(curriculoDisciplina); 
			dto.setName(dto.getDisciplinaString());
			dto.setPath(curriculoDisciplinaDTO.getPath() + dto.getDisciplinaString() + "/");
			dto.setFolha(true);
			list.add(dto);
		}
		return list;
	}
	
	
	@Override
	public void saveDisciplinaToSala(OfertaDTO ofertaDTO, Integer periodo, CurriculoDisciplinaDTO cdDTO, SalaDTO salaDTO) {
		Sala sala = Sala.find(salaDTO.getId());
		Set<CurriculoDisciplina> list = new HashSet<CurriculoDisciplina>();
		
		if(ofertaDTO != null && periodo == null && cdDTO == null) {
			Oferta oferta = Oferta.find(ofertaDTO.getId());
			list.addAll(oferta.getCurriculo().getDisciplinas());
		} else if(ofertaDTO != null && periodo != null && cdDTO == null) {
			// Informou o periodo inteiro somente
			Oferta oferta = Oferta.find(ofertaDTO.getId());
			list.addAll(CurriculoDisciplina.findAllByCurriculoAndPeriodo(oferta.getCurriculo(), periodo));
		} else if(ofertaDTO != null && periodo != null && cdDTO != null) {
			// Informou a disciplina somente
			list.add(CurriculoDisciplina.find(cdDTO.getId()));
		}
		
		for(CurriculoDisciplina curriculoDisciplina : list) {
			curriculoDisciplina.getSalas().add(sala);
			curriculoDisciplina.persist();
		}
	}
	
	@Override
	public void saveDisciplinaToSala(OfertaDTO ofertaDTO, Integer periodo, CurriculoDisciplinaDTO cdDTO, GrupoSalaDTO grupoSalaDTO) {
		GrupoSala grupoSala = GrupoSala.find(grupoSalaDTO.getId());
		Set<CurriculoDisciplina> list = new HashSet<CurriculoDisciplina>();
		
		if(ofertaDTO != null && periodo == null && cdDTO == null) {
			Oferta oferta = Oferta.find(ofertaDTO.getId());
			list.addAll(oferta.getCurriculo().getDisciplinas());
		} else if(ofertaDTO != null && periodo != null && cdDTO == null) {
			// Informou o periodo inteiro somente
			Oferta oferta = Oferta.find(ofertaDTO.getId());
			list.addAll(CurriculoDisciplina.findAllByCurriculoAndPeriodo(oferta.getCurriculo(), periodo));
		} else if(ofertaDTO != null && periodo != null && cdDTO != null) {
			// Informou a disciplina somente
			list.add(CurriculoDisciplina.find(cdDTO.getId()));
		}
		
		grupoSala.getCurriculoDisciplinas().addAll(list);
		grupoSala.persist();
	}
	
	@Override
	public void removeDisciplinaToSala(GrupoSalaDTO grupoSalaDTO, CurriculoDisciplinaDTO cdDTO) {
		GrupoSala grupoSala = GrupoSala.find(grupoSalaDTO.getId());
		CurriculoDisciplina curriculoDisciplina = CurriculoDisciplina.find(cdDTO.getId());
		
		grupoSala.getCurriculoDisciplinas().remove(curriculoDisciplina);
		grupoSala.merge();
	}
	@Override
	public void removeDisciplinaToSala(SalaDTO salaDTO, CurriculoDisciplinaDTO cdDTO) {
		Sala sala = Sala.find(salaDTO.getId());
		CurriculoDisciplina curriculoDisciplina = CurriculoDisciplina.find(cdDTO.getId());
		
		curriculoDisciplina.getSalas().remove(sala);
		curriculoDisciplina.merge();
	}
	
	@Override
	public List<DisciplinaIncompativelDTO> getDisciplinasIncompativeis(CurriculoDTO curriculoDTO, Integer periodo) {
		List<DisciplinaIncompativelDTO> list = new ArrayList<DisciplinaIncompativelDTO>();
		
		Curriculo curriculo = Curriculo.find(curriculoDTO.getId());
		List<CurriculoDisciplina> curriculoDisciplinas = curriculo.getCurriculoDisciplinasByPeriodo(periodo);
		
		for(CurriculoDisciplina cd1 : curriculoDisciplinas) {
			for(CurriculoDisciplina cd2 : curriculoDisciplinas) {
				if(cd1.getId().equals(cd2.getId())) continue;
				DisciplinaIncompativelDTO di = new DisciplinaIncompativelDTO();
				if(containsDisciplinaIncompativelDTO(list, cd1.getDisciplina(), cd2.getDisciplina())) continue;
				di.setDisciplina1Id(cd1.getDisciplina().getId());
				di.setDisciplina1String(cd1.getDisciplina().getCodigo());
				di.setDisciplina2Id(cd2.getDisciplina().getId());
				di.setDisciplina2String(cd2.getDisciplina().getCodigo());
				di.setIncompativel(cd1.getDisciplina().isIncompativelCom(cd2.getDisciplina()));
				
				list.add(di);
			}
		}

		return list;
	}
	
	@Override
	public void saveDisciplinasIncompativeis(List<DisciplinaIncompativelDTO> list) {
		Set<Disciplina> disciplinas = new HashSet<Disciplina>();
		for(DisciplinaIncompativelDTO diDTO : list) {
			disciplinas.add(Disciplina.find(diDTO.getDisciplina1Id()));
			disciplinas.add(Disciplina.find(diDTO.getDisciplina2Id()));
		}
		
		for(Disciplina d1 : disciplinas) {
			for(Disciplina d2 : disciplinas) {
				for(DisciplinaIncompativelDTO diDTO : list) {
					if(diDTO.getDisciplina1Id().equals(d1.getId()) && diDTO.getDisciplina2Id().equals(d2.getId())) {
						if(diDTO.getIncompativel()) {
							Incompatibilidade incomp = new Incompatibilidade();
							incomp.setDisciplina1(d1);
							incomp.setDisciplina2(d2);
							d1.getIncompatibilidades().add(incomp);
						} else {
							for(Incompatibilidade incomp : d1.getIncompatibilidades()) {
								if(incomp.getDisciplina2().getId().equals(d2)) incomp.remove();
							}
						}
					}
				}
			}
		}
	}
	
	private boolean containsDisciplinaIncompativelDTO(List<DisciplinaIncompativelDTO> list, Disciplina disciplina1, Disciplina disciplina2) {
		for(DisciplinaIncompativelDTO disciplinaIncompativelDTO : list) {
			if(disciplinaIncompativelDTO.getDisciplina1Id().equals(disciplina1.getId()) && disciplinaIncompativelDTO.getDisciplina2Id().equals(disciplina2.getId())) {
				return true;
			}
			if(disciplinaIncompativelDTO.getDisciplina1Id().equals(disciplina2.getId()) && disciplinaIncompativelDTO.getDisciplina2Id().equals(disciplina1.getId())) {
				return true;
			}
		}
		return false;
	}
}
