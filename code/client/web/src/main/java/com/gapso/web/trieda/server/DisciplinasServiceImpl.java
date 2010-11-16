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
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.TipoDisciplina;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.FileModel;
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.OfertaDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.TipoDisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
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
		List<Disciplina> disciplinas = Disciplina.findByCodigoLikeAndNomeLikeAndTipo(nome, codigo, tipoDisciplina, config.getOffset(), config.getLimit(), orderBy);
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
	
	@Override
	public List<FileModel> getFolderChildrenSala(FileModel model) {
		List<FileModel> listDTO = new ArrayList<FileModel>();
		if(model != null) {
			if(model instanceof SalaDTO) {
				Sala sala = Sala.find(((SalaDTO)model).getId());
				List<Curriculo> curriculos = sala.getCurriculos();
				for(Curriculo c : curriculos) {
					CurriculoDTO curriculoDTO = ConvertBeans.toCurriculoDTO(c);
					curriculoDTO.setName(curriculoDTO.getCodigo() + " (" + curriculoDTO.getCursoString() + ")");
					curriculoDTO.setPath(model.getPath() + curriculoDTO.getCodigo() + "/");
					listDTO.add(curriculoDTO);
				}
			} else if(model instanceof GrupoSalaDTO) {
				GrupoSala grupoSala = GrupoSala.find(((GrupoSalaDTO)model).getId());
				List<Curriculo> curriculos = grupoSala.getCurriculos();
				for(Curriculo c : curriculos) {
					CurriculoDTO curriculoDTO = ConvertBeans.toCurriculoDTO(c);
					curriculoDTO.setName(curriculoDTO.getCodigo() + " (" + curriculoDTO.getCursoString() + ")");
					curriculoDTO.setPath(model.getPath() + curriculoDTO.getCodigo() + "/");
					listDTO.add(curriculoDTO);
				}
			} else if(model instanceof CurriculoDTO) {
				Curriculo curriculo = Curriculo.find(((CurriculoDTO)model).getId());
				List<Turno> turnos = new ArrayList<Turno>();
				Set<Oferta> ofertas = curriculo.getOfertas();
				for(Oferta o : ofertas) {
					Turno t = o.getTurno();
					if(!turnos.contains(t)) turnos.add(t);
				}
				for(Turno t : turnos) {
					TurnoDTO turnoDTO = ConvertBeans.toTurnoDTO(t);
					turnoDTO.setName(turnoDTO.getNome());
					turnoDTO.setPath(model.getPath() + turnoDTO.getNome() + "/");
					listDTO.add(turnoDTO);
				}
			} else if(model instanceof TurnoDTO) {
				Turno turno = Turno.find(((TurnoDTO)model).getId());
				Set<Oferta> ofertas = turno.getOfertas();
				Set<CurriculoDisciplina> cds = new HashSet<CurriculoDisciplina>();
				for(Oferta oferta : ofertas) {
					Set<CurriculoDisciplina> cdsTodas = oferta.getCurriculo().getDisciplinas();
					for(CurriculoDisciplina cd : cdsTodas) {
						if(cd.getSalas().size() > 0) cds.add(cd);
					}
				}
				for(CurriculoDisciplina cd : cds) {
					CurriculoDisciplinaDTO cdDTO = ConvertBeans.toCurriculoDisciplinaDTO(cd);
					cdDTO.setName(cdDTO.getDisciplinaString());
					cdDTO.setPath(model.getPath() + cdDTO.getDisciplinaString() + "/");
					cdDTO.setFolha(true);
					listDTO.add(cdDTO);
				}
			}
		}
		return listDTO;
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
		
		curriculoDisciplina.getGruposSala().remove(grupoSala);
		curriculoDisciplina.merge();
	}
	@Override
	public void removeDisciplinaToSala(SalaDTO salaDTO, CurriculoDisciplinaDTO cdDTO) {
		Sala sala = Sala.find(salaDTO.getId());
		CurriculoDisciplina curriculoDisciplina = CurriculoDisciplina.find(cdDTO.getId());
		
		curriculoDisciplina.getSalas().remove(sala);
		curriculoDisciplina.merge();
	}
}
