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
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.TipoDisciplina;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.FileModel;
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
	public List<FileModel> getFolderChildren(FileModel model) {
		List<FileModel> listDTO = new ArrayList<FileModel>();
		if(model != null) {
			if(model instanceof CurriculoDTO) {
				Curriculo curriculo = Curriculo.find(((CurriculoDTO) model).getId());
				Set<CurriculoDisciplina> disciplinas = curriculo.getDisciplinas();
				for(CurriculoDisciplina o : disciplinas);
				List<CurriculoDisciplina> list = new ArrayList<CurriculoDisciplina>(disciplinas);
				List<Integer> periodos = new ArrayList<Integer>();
				for(CurriculoDisciplina cd : list) {
					CurriculoDisciplinaDTO curriculoDisciplinaDTO = ConvertBeans.toCurriculoDisciplinaDTO(cd);
					curriculoDisciplinaDTO.setName("Período "+curriculoDisciplinaDTO.getPeriodo().toString());
					curriculoDisciplinaDTO.setPath(model.getPath() + curriculoDisciplinaDTO.getPeriodo().toString() + "/");
					if(!periodos.contains(curriculoDisciplinaDTO.getPeriodo())) {
						periodos.add(curriculoDisciplinaDTO.getPeriodo());
						listDTO.add(curriculoDisciplinaDTO);
					}
				}
			} else if(model instanceof CurriculoDisciplinaDTO) {
				Integer periodo = ((CurriculoDisciplinaDTO) model).getPeriodo();
				Curriculo curriculo = Curriculo.find(((CurriculoDisciplinaDTO) model).getCurriculoId());
				Set<CurriculoDisciplina> disciplinas = curriculo.getDisciplinas();
				List<CurriculoDisciplina> list = new ArrayList<CurriculoDisciplina>(disciplinas);
				for(CurriculoDisciplina cd : list) {
					DisciplinaDTO disciplinaDTO = ConvertBeans.toDisciplinaDTO(cd.getDisciplina());
					disciplinaDTO.setName(disciplinaDTO.getCodigo());
					disciplinaDTO.setPath(model.getPath() + disciplinaDTO.getCodigo() + "/");
					if(cd.getPeriodo().equals(periodo)) {
						listDTO.add(disciplinaDTO);
					}
				}
			}
		}
		return listDTO;
	}
	
	@Override
	public List<FileModel> getFolderChildrenSalaToDisciplinba(FileModel model) {
		// TODO
		List<FileModel> listDTO = new ArrayList<FileModel>();
		if(model != null) {
			if(model instanceof SalaDTO) {
				Sala sala = Sala.find(((SalaDTO) model).getId());
				
			} else if(model instanceof CurriculoDisciplinaDTO) {
				CurriculoDisciplina curriculoDisciplina = CurriculoDisciplina.find(((CurriculoDisciplinaDTO) model).getId());
				
			} else if(model instanceof TurnoDTO) {
				Turno turno = Turno.find(((TurnoDTO) model).getId());
			}
		}
		return listDTO;
	}
	
	public void saveSalaToCurriculoDisciplina(CurriculoDisciplinaDTO curriculoDisciplinaDTO, SalaDTO salaDTO) {
		CurriculoDisciplina curriculoDisciplina = CurriculoDisciplina.find(curriculoDisciplinaDTO.getId());
	}
}
