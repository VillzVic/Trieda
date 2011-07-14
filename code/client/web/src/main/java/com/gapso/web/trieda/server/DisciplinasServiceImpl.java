package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.DivisaoCredito;
import com.gapso.trieda.domain.GrupoSala;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Incompatibilidade;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoDisciplina;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AbstractDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaIncompativelDTO;
import com.gapso.web.trieda.shared.dtos.DivisaoCreditoDTO;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.dtos.ResumoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TipoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.gapso.web.trieda.shared.services.DisciplinasService;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
@Transactional
public class DisciplinasServiceImpl extends RemoteServiceServlet implements
		DisciplinasService
{
	private static final long serialVersionUID = -4850774141421616870L;

	@Override
	public DisciplinaDTO getDisciplina(Long id) {
		if (id == null)
			return null;
		return ConvertBeans.toDisciplinaDTO(Disciplina.find(id));
	}

	@Override
	public List<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(
			DisciplinaDTO disciplinaDTO, SemanaLetivaDTO semanaLetivaDTO) {
		SemanaLetiva semanaLetiva = SemanaLetiva.getByOficial();
		List<HorarioDisponivelCenario> list = new ArrayList<HorarioDisponivelCenario>(
				Disciplina.find(disciplinaDTO.getId())
						.getHorarios(semanaLetiva));
		List<HorarioDisponivelCenarioDTO> listDTO = ConvertBeans
				.toHorarioDisponivelCenarioDTO(list);

		return listDTO;
	}

	@Override
	public void saveHorariosDisponiveis(DisciplinaDTO disciplinaDTO,
			List<HorarioDisponivelCenarioDTO> listDTO) {
		List<HorarioDisponivelCenario> listSelecionados = ConvertBeans
				.toHorarioDisponivelCenario(listDTO);
		Disciplina disciplina = Disciplina.find(disciplinaDTO.getId());
		List<HorarioDisponivelCenario> adicionarList = new ArrayList<HorarioDisponivelCenario>(
				listSelecionados);
		SemanaLetiva semanaLetiva = SemanaLetiva.getByOficial();
		adicionarList.removeAll(disciplina.getHorarios(semanaLetiva));
		List<HorarioDisponivelCenario> removerList = new ArrayList<HorarioDisponivelCenario>(
				disciplina.getHorarios(semanaLetiva));
		removerList.removeAll(listSelecionados);
		for (HorarioDisponivelCenario o : removerList) {
			o.getDisciplinas().remove(disciplina);
			o.merge();
		}
		for (HorarioDisponivelCenario o : adicionarList) {
			o.getDisciplinas().add(disciplina);
			o.merge();
		}
	}

	@Override
	public ListLoadResult<DisciplinaDTO> getListByCursos(
			List<CursoDTO> cursosDTO) {
		List<DisciplinaDTO> list = new ArrayList<DisciplinaDTO>();
		List<Curso> cursos = new ArrayList<Curso>();
		for (CursoDTO cursoDTO : cursosDTO) {
			cursos.add(ConvertBeans.toCurso(cursoDTO));
		}
		List<Disciplina> disciplinas = Disciplina.findByCursos(cursos);
		for (Disciplina disciplina : disciplinas) {
			list.add(ConvertBeans.toDisciplinaDTO(disciplina));
		}
		return new BaseListLoadResult<DisciplinaDTO>(list);
	}

	public ListLoadResult<DisciplinaDTO> getListBySalas(List<SalaDTO> salasDTO) {
		List<DisciplinaDTO> list = new ArrayList<DisciplinaDTO>();
		List<Sala> salas = new ArrayList<Sala>();
		for (SalaDTO salaDTO : salasDTO) {
			salas.add(ConvertBeans.toSala(salaDTO));
		}
		List<Disciplina> disciplinas = Disciplina.findBySalas(salas);
		for (Disciplina disciplina : disciplinas) {
			list.add(ConvertBeans.toDisciplinaDTO(disciplina));
		}
		return new BaseListLoadResult<DisciplinaDTO>(list);
	}

	@Override
	public ListLoadResult<DisciplinaDTO> getList(BasePagingLoadConfig loadConfig) {
		return getBuscaList(null, loadConfig.get("query").toString(), null,
				loadConfig);
	}

	@Override
	public PagingLoadResult<DisciplinaDTO> getBuscaList(String nome,
			String codigo, TipoDisciplinaDTO tipoDisciplinaDTO,
			PagingLoadConfig config) {
		List<DisciplinaDTO> list = new ArrayList<DisciplinaDTO>();
		String orderBy = config.getSortField();
		if (orderBy != null) {
			if (config.getSortDir() != null
					&& config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		TipoDisciplina tipoDisciplina = null;
		if (tipoDisciplinaDTO != null) {
			tipoDisciplina = ConvertBeans.toTipoDisciplina(tipoDisciplinaDTO);
		}
		List<Disciplina> disciplinas = Disciplina.findBy(codigo, nome,
				tipoDisciplina, config.getOffset(), config.getLimit(), orderBy);
		for (Disciplina disciplina : disciplinas) {
			DisciplinaDTO disciplinaDTO = ConvertBeans
					.toDisciplinaDTO(disciplina);
			list.add(disciplinaDTO);
		}
		BasePagingLoadResult<DisciplinaDTO> result = new BasePagingLoadResult<DisciplinaDTO>(
				list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Disciplina.count(codigo, nome, tipoDisciplina));
		return result;
	}

	@Override
	public void save(DisciplinaDTO disciplinaDTO) {
		Disciplina disciplina = ConvertBeans.toDisciplina(disciplinaDTO);
		if (disciplina.getId() != null && disciplina.getId() > 0) {
			disciplina.merge();
		} else {
			disciplina.persist();
			// TODO Pegar a semana letiva do cenario da disciplina
			Set<HorarioAula> horariosAula = SemanaLetiva.getByOficial()
					.getHorariosAula();
			for (HorarioAula horarioAula : horariosAula) {
				Set<HorarioDisponivelCenario> horariosDisponiveis = horarioAula
						.getHorariosDisponiveisCenario();
				for (HorarioDisponivelCenario horarioDisponivel : horariosDisponiveis) {
					horarioDisponivel.getDisciplinas().add(disciplina);
					horarioDisponivel.merge();
				}
			}
		}
	}

	@Override
	public void remove(List<DisciplinaDTO> disciplinaDTOList) {
		for (DisciplinaDTO disciplinaDTO : disciplinaDTOList) {
			Disciplina.find(disciplinaDTO.getId()).remove();
		}
	}

	@Override
	public DivisaoCreditoDTO getDivisaoCredito(DisciplinaDTO disciplinaDTO) {
		Disciplina disciplina = Disciplina.find(disciplinaDTO.getId());
		DivisaoCredito dc = disciplina.getDivisaoCreditos();
		if (dc == null) {
			return new DivisaoCreditoDTO();
		}
		return ConvertBeans.toDivisaoCreditoDTO(dc);
	}

	@Override
	public void salvarDivisaoCredito(DisciplinaDTO disciplinaDTO,
			DivisaoCreditoDTO divisaoCreditoDTO) {
		Disciplina disciplina = Disciplina.find(disciplinaDTO.getId());
		DivisaoCredito divisaoCredito = disciplina.getDivisaoCreditos();
		if (divisaoCredito != null) {
			int d1 = divisaoCreditoDTO.getDia1();
			int d2 = divisaoCreditoDTO.getDia2();
			int d3 = divisaoCreditoDTO.getDia3();
			int d4 = divisaoCreditoDTO.getDia4();
			int d5 = divisaoCreditoDTO.getDia5();
			int d6 = divisaoCreditoDTO.getDia6();
			int d7 = divisaoCreditoDTO.getDia7();

			divisaoCredito.setDia1(d1);
			divisaoCredito.setDia2(d2);
			divisaoCredito.setDia3(d3);
			divisaoCredito.setDia4(d4);
			divisaoCredito.setDia5(d5);
			divisaoCredito.setDia6(d6);
			divisaoCredito.setDia7(d7);

			divisaoCredito.setCreditos(d1 + d2 + d3 + d4 + d5 + d6 + d7);
			divisaoCredito.merge();
		} else {
			divisaoCreditoDTO.setDisciplinaId(disciplinaDTO.getId());
			divisaoCredito = ConvertBeans.toDivisaoCredito(divisaoCreditoDTO);
			disciplina.setDivisaoCreditos(divisaoCredito);
			disciplina.merge();
		}

	}

	@Override
	public TipoDisciplinaDTO getTipoDisciplina(Long id) {
		return ConvertBeans.toTipoDisciplinaDTO(TipoDisciplina.find(id));
	}

	@Override
	public ListLoadResult<TipoDisciplinaDTO> getTipoDisciplinaList() {
		// TODO REMOVER AS LINHAS DE BAIXO
		if (TipoDisciplina.count() == 0) {
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
		for (TipoDisciplina tipo : TipoDisciplina.findAll()) {
			list.add(ConvertBeans.toTipoDisciplinaDTO(tipo));
		}
		return new BaseListLoadResult<TipoDisciplinaDTO>(list);
	}

	@Override
	@Transactional
	public List<TreeNodeDTO> getFolderChildren(TreeNodeDTO currentNode) {
		List<TreeNodeDTO> currentNodeChildren = new ArrayList<TreeNodeDTO>();
		if (currentNode != null) {
			AbstractDTO<?> contentNode = currentNode.getContent();
			if (contentNode != null) {
				if (contentNode instanceof OfertaDTO) {
					OfertaDTO ofertaDTOContentNode = (OfertaDTO) contentNode;
					Curriculo curriculo = Curriculo.find(ofertaDTOContentNode
							.getMatrizCurricularId());

					Set<Integer> periodos = new HashSet<Integer>();
					for (CurriculoDisciplina cd : curriculo.getDisciplinas()) {
						if (periodos.add(cd.getPeriodo())) {
							CurriculoDisciplinaDTO curriculoDisciplinaDTO = ConvertBeans
									.toCurriculoDisciplinaDTO(cd);
							TreeNodeDTO newChildNode = new TreeNodeDTO(
									curriculoDisciplinaDTO, currentNode);
							newChildNode.setText("Periodo "
									+ curriculoDisciplinaDTO.getPeriodo()
											.toString());
							currentNodeChildren.add(newChildNode);
						}
					}
				} else if (contentNode instanceof CurriculoDisciplinaDTO) {
					CurriculoDisciplinaDTO curricDiscDTOContentNode = (CurriculoDisciplinaDTO) contentNode;
					Curriculo curriculo = Curriculo
							.find(curricDiscDTOContentNode.getCurriculoId());

					for (CurriculoDisciplina cd : curriculo.getDisciplinas()) {
						if (cd.getPeriodo().equals(
								curricDiscDTOContentNode.getPeriodo())) {
							CurriculoDisciplinaDTO curriculoDisciplinaDTO = ConvertBeans
									.toCurriculoDisciplinaDTO(cd);
							TreeNodeDTO newChildNode = new TreeNodeDTO(
									curriculoDisciplinaDTO, currentNode);
							newChildNode.setLeaf(true);
							currentNodeChildren.add(newChildNode);
						}
					}
				}
			}
		}

		Collections.sort(currentNodeChildren);

		return currentNodeChildren;
	}

	public List<TreeNodeDTO> getOfertasByTreeSalas(TreeNodeDTO salaTreeNodeDTO) {
		List<TreeNodeDTO> nodeChildrenList = new ArrayList<TreeNodeDTO>();

		SalaDTO salaDTO = (SalaDTO) salaTreeNodeDTO.getContent();
		Sala sala = Sala.find(salaDTO.getId());
		List<Oferta> ofertas = Oferta.findAllBy(sala);

		for (Oferta oferta : ofertas) {
			OfertaDTO ofertaDTO = ConvertBeans.toOfertaDTO(oferta);
			TreeNodeDTO nodeDTO = new TreeNodeDTO(ofertaDTO, salaTreeNodeDTO);
			nodeDTO.setEmpty(false);
			nodeChildrenList.add(nodeDTO);
		}

		Collections.sort(nodeChildrenList);

		return nodeChildrenList;
	}

	public List<TreeNodeDTO> getPeriodosByTreeSalas(
			TreeNodeDTO salaTreeNodeDTO, TreeNodeDTO ofertaTreeNodeDTO) {
		List<TreeNodeDTO> nodeChildrenList = new ArrayList<TreeNodeDTO>();

		SalaDTO salaDTO = (SalaDTO) salaTreeNodeDTO.getContent();
		Sala sala = Sala.find(salaDTO.getId());
		OfertaDTO ofertaDTO = (OfertaDTO) ofertaTreeNodeDTO.getContent();
		Oferta oferta = Oferta.find(ofertaDTO.getId());
		List<CurriculoDisciplina> curriculoDisciplinas = CurriculoDisciplina
				.findAllPeriodosBy(sala, oferta);

		for (CurriculoDisciplina cd : curriculoDisciplinas) {
			CurriculoDisciplinaDTO cdDTO = ConvertBeans
					.toCurriculoDisciplinaDTO(cd);
			TreeNodeDTO nodeDTO = new TreeNodeDTO(cdDTO, ofertaTreeNodeDTO);
			nodeDTO.setText("Periodo " + cdDTO.getPeriodo());
			nodeDTO.setEmpty(false);
			nodeChildrenList.add(nodeDTO);
		}

		Collections.sort(nodeChildrenList);

		return nodeChildrenList;
	}

	@Override
	public List<TreeNodeDTO> getDisciplinasByTreeSalas(
			TreeNodeDTO salaTreeNodeDTO, TreeNodeDTO ofertaTreeNodeDTO,
			TreeNodeDTO curriculoDisciplinaTreeNodeDTO) {
		if (ofertaTreeNodeDTO == null)
			return getOfertasByTreeSalas(salaTreeNodeDTO);
		if (curriculoDisciplinaTreeNodeDTO == null)
			return getPeriodosByTreeSalas(salaTreeNodeDTO, ofertaTreeNodeDTO);

		List<TreeNodeDTO> nodeChildrenList = new ArrayList<TreeNodeDTO>();

		SalaDTO salaDTO = (SalaDTO) salaTreeNodeDTO.getContent();
		Sala sala = Sala.find(salaDTO.getId());
		OfertaDTO ofertaDTO = (OfertaDTO) ofertaTreeNodeDTO.getContent();
		Oferta oferta = Oferta.find(ofertaDTO.getId());
		CurriculoDisciplinaDTO curriculoDisciplinaDTO = (CurriculoDisciplinaDTO) curriculoDisciplinaTreeNodeDTO
				.getContent();
		List<CurriculoDisciplina> curriculoDisciplinas = CurriculoDisciplina
				.findBy(sala, oferta, curriculoDisciplinaDTO.getPeriodo());

		for (CurriculoDisciplina cd : curriculoDisciplinas) {
			CurriculoDisciplinaDTO cdDTO = ConvertBeans
					.toCurriculoDisciplinaDTO(cd);
			TreeNodeDTO nodeDTO = new TreeNodeDTO(cdDTO,
					curriculoDisciplinaTreeNodeDTO);
			nodeDTO.setLeaf(true);
			nodeChildrenList.add(nodeDTO);
		}

		Collections.sort(nodeChildrenList);

		return nodeChildrenList;
	}

	public List<TreeNodeDTO> getOfertasByTreeGrupoSalas(
			TreeNodeDTO grupoSalaTreeNodeDTO) {
		List<TreeNodeDTO> nodeChildrenList = new ArrayList<TreeNodeDTO>();

		GrupoSalaDTO grupoSalaDTO = (GrupoSalaDTO) grupoSalaTreeNodeDTO
				.getContent();
		GrupoSala grupoSala = GrupoSala.find(grupoSalaDTO.getId());
		List<Oferta> ofertas = Oferta.findAllBy(grupoSala);

		for (Oferta oferta : ofertas) {
			OfertaDTO ofertaDTO = ConvertBeans.toOfertaDTO(oferta);
			TreeNodeDTO nodeDTO = new TreeNodeDTO(ofertaDTO,
					grupoSalaTreeNodeDTO);
			nodeChildrenList.add(nodeDTO);
		}

		Collections.sort(nodeChildrenList);

		return nodeChildrenList;
	}

	public List<TreeNodeDTO> getPeriodosByTreeGrupoSalas(
			TreeNodeDTO grupoSalaTreeNodeDTO, TreeNodeDTO ofertaTreeNodeDTO) {
		List<TreeNodeDTO> nodeChildrenList = new ArrayList<TreeNodeDTO>();

		GrupoSalaDTO grupoSalaDTO = (GrupoSalaDTO) grupoSalaTreeNodeDTO
				.getContent();
		GrupoSala grupoSala = GrupoSala.find(grupoSalaDTO.getId());
		OfertaDTO ofertaDTO = (OfertaDTO) ofertaTreeNodeDTO.getContent();
		Oferta oferta = Oferta.find(ofertaDTO.getId());
		List<CurriculoDisciplina> curriculoDisciplinas = CurriculoDisciplina
				.findAllPeriodosBy(grupoSala, oferta);

		for (CurriculoDisciplina cd : curriculoDisciplinas) {
			CurriculoDisciplinaDTO cdDTO = ConvertBeans
					.toCurriculoDisciplinaDTO(cd);
			TreeNodeDTO nodeDTO = new TreeNodeDTO(cdDTO, ofertaTreeNodeDTO);
			nodeDTO.setText("Periodo " + cdDTO.getPeriodo());
			nodeChildrenList.add(nodeDTO);
		}

		Collections.sort(nodeChildrenList);

		return nodeChildrenList;
	}

	@Override
	public List<TreeNodeDTO> getDisciplinasByTreeGrupoSalas(
			TreeNodeDTO grupoSalaTreeNodeDTO, TreeNodeDTO ofertaTreeNodeDTO,
			TreeNodeDTO curriculoDisciplinaTreeNodeDTO) {
		if (ofertaTreeNodeDTO == null)
			return getOfertasByTreeGrupoSalas(grupoSalaTreeNodeDTO);
		if (curriculoDisciplinaTreeNodeDTO == null)
			return getPeriodosByTreeGrupoSalas(grupoSalaTreeNodeDTO,
					ofertaTreeNodeDTO);

		List<TreeNodeDTO> nodeChildrenList = new ArrayList<TreeNodeDTO>();

		GrupoSalaDTO grupoSalaDTO = (GrupoSalaDTO) grupoSalaTreeNodeDTO
				.getContent();
		GrupoSala grupoSala = GrupoSala.find(grupoSalaDTO.getId());
		OfertaDTO ofertaDTO = (OfertaDTO) ofertaTreeNodeDTO.getContent();
		Oferta oferta = Oferta.find(ofertaDTO.getId());
		CurriculoDisciplinaDTO curriculoDisciplinaDTO = (CurriculoDisciplinaDTO) curriculoDisciplinaTreeNodeDTO
				.getContent();
		List<CurriculoDisciplina> curriculoDisciplinas = CurriculoDisciplina
				.findBy(grupoSala, oferta, curriculoDisciplinaDTO.getPeriodo());

		for (CurriculoDisciplina cd : curriculoDisciplinas) {
			CurriculoDisciplinaDTO cdDTO = ConvertBeans
					.toCurriculoDisciplinaDTO(cd);
			TreeNodeDTO nodeDTO = new TreeNodeDTO(cdDTO,
					curriculoDisciplinaTreeNodeDTO);
			nodeDTO.setLeaf(true);
			nodeChildrenList.add(nodeDTO);
		}

		Collections.sort(nodeChildrenList);

		return nodeChildrenList;
	}

	@Override
	public void saveDisciplinaToSala(OfertaDTO ofertaDTO, Integer periodo,
			CurriculoDisciplinaDTO cdDTO, SalaDTO salaDTO) {
		Sala sala = Sala.find(salaDTO.getId());
		Set<CurriculoDisciplina> list = new HashSet<CurriculoDisciplina>();

		if (ofertaDTO != null && periodo == null && cdDTO == null) {
			Oferta oferta = Oferta.find(ofertaDTO.getId());
			list.addAll(oferta.getCurriculo().getDisciplinas());
		} else if (ofertaDTO != null && periodo != null && cdDTO == null) {
			// Informou o periodo inteiro somente
			Oferta oferta = Oferta.find(ofertaDTO.getId());
			list.addAll(CurriculoDisciplina.findAllByCurriculoAndPeriodo(
					oferta.getCurriculo(), periodo));
		} else if (ofertaDTO != null && periodo != null && cdDTO != null) {
			// Informou a disciplina somente
			list.add(CurriculoDisciplina.find(cdDTO.getId()));
		}

		for (CurriculoDisciplina curriculoDisciplina : list) {
			curriculoDisciplina.getSalas().add(sala);
			curriculoDisciplina.persist();
		}
	}

	@Override
	public void saveDisciplinaToSala(OfertaDTO ofertaDTO, Integer periodo,
			CurriculoDisciplinaDTO cdDTO, GrupoSalaDTO grupoSalaDTO) {
		GrupoSala grupoSala = GrupoSala.find(grupoSalaDTO.getId());
		Set<CurriculoDisciplina> list = new HashSet<CurriculoDisciplina>();

		if (ofertaDTO != null && periodo == null && cdDTO == null) {
			Oferta oferta = Oferta.find(ofertaDTO.getId());
			list.addAll(oferta.getCurriculo().getDisciplinas());
		} else if (ofertaDTO != null && periodo != null && cdDTO == null) {
			// Informou o periodo inteiro somente
			Oferta oferta = Oferta.find(ofertaDTO.getId());
			list.addAll(CurriculoDisciplina.findAllByCurriculoAndPeriodo(
					oferta.getCurriculo(), periodo));
		} else if (ofertaDTO != null && periodo != null && cdDTO != null) {
			// Informou a disciplina somente
			list.add(CurriculoDisciplina.find(cdDTO.getId()));
		}

		grupoSala.getCurriculoDisciplinas().addAll(list);
		grupoSala.persist();
	}

	@Override
	public void removeDisciplinaToSala(GrupoSalaDTO grupoSalaDTO,
			CurriculoDisciplinaDTO cdDTO) {
		GrupoSala grupoSala = GrupoSala.find(grupoSalaDTO.getId());
		CurriculoDisciplina curriculoDisciplina = CurriculoDisciplina
				.find(cdDTO.getId());

		grupoSala.getCurriculoDisciplinas().remove(curriculoDisciplina);
		grupoSala.merge();
	}

	@Override
	public void removeDisciplinaToSala(SalaDTO salaDTO,
			CurriculoDisciplinaDTO cdDTO) {
		Sala sala = Sala.find(salaDTO.getId());
		CurriculoDisciplina curriculoDisciplina = CurriculoDisciplina
				.find(cdDTO.getId());

		curriculoDisciplina.getSalas().remove(sala);
		curriculoDisciplina.merge();
	}

	@Override
	public List<DisciplinaIncompativelDTO> getDisciplinasIncompativeis(
			CurriculoDTO curriculoDTO, Integer periodo) {
		List<DisciplinaIncompativelDTO> list = new ArrayList<DisciplinaIncompativelDTO>();

		Curriculo curriculo = Curriculo.find(curriculoDTO.getId());
		List<CurriculoDisciplina> curriculoDisciplinas = curriculo
				.getCurriculoDisciplinasByPeriodo(periodo);

		for (CurriculoDisciplina cd1 : curriculoDisciplinas) {
			for (CurriculoDisciplina cd2 : curriculoDisciplinas) {
				if (cd1.getId().equals(cd2.getId()))
					continue;
				DisciplinaIncompativelDTO di = new DisciplinaIncompativelDTO();
				if (containsDisciplinaIncompativelDTO(list,
						cd1.getDisciplina(), cd2.getDisciplina()))
					continue;
				di.setDisciplina1Id(cd1.getDisciplina().getId());
				di.setDisciplina1String(cd1.getDisciplina().getCodigo() + " ("
						+ cd1.getDisciplina().getNome() + ")");
				di.setDisciplina2Id(cd2.getDisciplina().getId());
				di.setDisciplina2String(cd2.getDisciplina().getCodigo() + " ("
						+ cd2.getDisciplina().getNome() + ")");
				di.setIncompativel(cd1.getDisciplina().isIncompativelCom(
						cd2.getDisciplina()));

				list.add(di);
			}
		}

		return list;
	}

	@Override
	public void saveDisciplinasIncompativeis(
			List<DisciplinaIncompativelDTO> list )
	{
		for ( DisciplinaIncompativelDTO disciplinaIncompativelDTO : list )
		{
			Disciplina disciplina1 = Disciplina.find(
					disciplinaIncompativelDTO.getDisciplina1Id() );

			Disciplina disciplina2 = Disciplina.find(
					disciplinaIncompativelDTO.getDisciplina2Id() );

			Incompatibilidade incompatibilidade
				= disciplina1.getIncompatibilidadeWith( disciplina2 );

			if ( incompatibilidade == null )
			{
				if ( disciplinaIncompativelDTO.getIncompativel() )
				{
					incompatibilidade = new Incompatibilidade();
					incompatibilidade.setDisciplina1( disciplina1 );
					incompatibilidade.setDisciplina2( disciplina2 );
					incompatibilidade.persist();
				}
			}
			else
			{
				if ( !disciplinaIncompativelDTO.getIncompativel() )
				{
					incompatibilidade.remove();
				}
			}
		}
	}

	private boolean containsDisciplinaIncompativelDTO(
			List< DisciplinaIncompativelDTO > list,
			Disciplina disciplina1, Disciplina disciplina2 )
	{
		for ( DisciplinaIncompativelDTO disciplinaIncompativelDTO : list )
		{
			if ( disciplinaIncompativelDTO.getDisciplina1Id().equals(
					disciplina1.getId() )
					&& disciplinaIncompativelDTO.getDisciplina2Id().equals(
							disciplina2.getId() ) )
			{
				return true;
			}

			if ( disciplinaIncompativelDTO.getDisciplina1Id().equals(
					disciplina2.getId() )
					&& disciplinaIncompativelDTO.getDisciplina2Id().equals(
							disciplina1.getId() ) )
			{
				return true;
			}
		}

		return false;
	}

	@Override
	public List< ResumoDisciplinaDTO > getResumos(
			CenarioDTO cenarioDTO, CampusDTO campusDTO )
	{
		// Cenario cenario = Cenario.find( cenarioDTO.getId() );
		Campus campus = Campus.find( campusDTO.getId() );

     	// Juntando os atendimentos tático e operacional
		List< AtendimentoTatico > listTatico = AtendimentoTatico.findAllByCampus( campus );
		List< AtendimentoTaticoDTO > listTaticoDTO = new ArrayList< AtendimentoTaticoDTO >();
		for ( AtendimentoTatico att : listTatico )
		{
			AtendimentoTaticoDTO dto = ConvertBeans.toAtendimentoTaticoDTO( att );
			listTaticoDTO.add( dto );
		}

		List< AtendimentoOperacional > listOperacional = AtendimentoOperacional.findAll( campus );
		List< AtendimentoOperacionalDTO > listOperacionalDTO = new ArrayList< AtendimentoOperacionalDTO >();
		for ( AtendimentoOperacional atop : listOperacional )
		{
			AtendimentoOperacionalDTO dto = ConvertBeans.toAtendimentoOperacionalDTO( atop );
			listOperacionalDTO.add( dto );
		}

		List< AtendimentoRelatorioDTO > listRelatorioDTO = new ArrayList< AtendimentoRelatorioDTO >();
		listRelatorioDTO.addAll( listTaticoDTO );
		listRelatorioDTO.addAll( listOperacionalDTO );
		///////
		
		List< Oferta > ofertas = new ArrayList< Oferta >( campus.getOfertas() );
		Collections.sort( ofertas );

		Map< String, AtendimentoRelatorioDTO > atendimentoTaticoMap
			= new HashMap< String, AtendimentoRelatorioDTO >();

		Map< String, ResumoDisciplinaDTO > nivel1Map
			= new HashMap< String, ResumoDisciplinaDTO >();

		Map< String, Map< String, ResumoDisciplinaDTO > > nivel2Map
			= new HashMap< String, Map< String, ResumoDisciplinaDTO > >();

		for ( AtendimentoRelatorioDTO atendimento : listRelatorioDTO )
		{
			String key = atendimento.getDisciplinaId()
					+ "-" + atendimento.getTurma()
					+ "-" + ( atendimento.isTeorico() );

			if ( !atendimentoTaticoMap.containsKey( key ) )
			{
				atendimentoTaticoMap.put( key, atendimento );
			}

			Disciplina disciplina = Disciplina.find( atendimento.getDisciplinaId() );
			ResumoDisciplinaDTO resumoDTO = new ResumoDisciplinaDTO();

			resumoDTO.setDisciplinaId( disciplina.getId() );
			resumoDTO.setDisciplinaString( disciplina.getNome() );
			resumoDTO.setTurma( atendimento.getTurma() );
			resumoDTO.setTipoCreditoTeorico( atendimento.isTeorico() );
			resumoDTO.setCreditos( atendimento.getTotalCreditos() );
			resumoDTO.setTotalCreditos( disciplina.getCreditosTotal() );
			resumoDTO.setQuantidadeAlunos( atendimento.getQuantidadeAlunos() );

			createResumoNivel1( nivel1Map, nivel2Map, resumoDTO );
			createResumoNivel2( nivel2Map, resumoDTO );
		}

		calculaResumo2( nivel2Map, atendimentoTaticoMap );
		calculaResumo1( nivel1Map, nivel2Map );

		return createResumoEstrutura( nivel1Map, nivel2Map );
	}

	private List< ResumoDisciplinaDTO > createResumoEstrutura(
			Map< String, ResumoDisciplinaDTO > map1,
			Map< String, Map< String, ResumoDisciplinaDTO > > map2 )
	{
		List< ResumoDisciplinaDTO > list = new ArrayList< ResumoDisciplinaDTO >();

		for ( String key1 : map1.keySet() )
		{
			ResumoDisciplinaDTO rd1DTO = map1.get( key1 );
			list.add( rd1DTO );

			for ( String key2 : map2.get( key1 ).keySet() )
			{
				ResumoDisciplinaDTO rd2DTO = map2.get( key1 ).get( key2 );
				rd1DTO.add( rd2DTO );
			}
		}

		return list;
	}

	private void calculaResumo2(
			Map< String, Map< String, ResumoDisciplinaDTO > > map2,
			Map< String, AtendimentoRelatorioDTO > atendimentosMap )
	{
		for ( String key1 : map2.keySet() )
		{
			for ( String key2 : map2.get( key1 ).keySet() )
			{
				ResumoDisciplinaDTO resumo2DTO = map2.get( key1 ).get( key2 );
				AtendimentoRelatorioDTO atendimento = atendimentosMap.get( key2 );

				Oferta ofertaAtendimento = Oferta.find( atendimento.getOfertaId() );

				Campus campus = ofertaAtendimento.getCampus();
				Double docente = campus.getValorCredito();
				Double receita = ofertaAtendimento.getReceita();
				int qtdAlunos = atendimento.getQuantidadeAlunos();
				int creditos = atendimento.getTotalCreditos();
				resumo2DTO.setCustoDocente( creditos * docente * 4.5 * 6 );
				resumo2DTO.setReceita( receita * creditos * qtdAlunos * 4.5 * 6 );
				resumo2DTO.setMargem( resumo2DTO.getReceita() - resumo2DTO.getCustoDocente() );
				resumo2DTO.setMargemPercente( TriedaUtil.roundTwoDecimals( resumo2DTO.getMargem() / resumo2DTO.getReceita() ) );
			}
		}
	}

	private void calculaResumo1( Map< String, ResumoDisciplinaDTO > map1,
			Map< String, Map< String, ResumoDisciplinaDTO > > map2 )
	{
		for ( String key1 : map2.keySet() )
		{
			ResumoDisciplinaDTO rc1 = map1.get( key1 );

			rc1.setCustoDocente( 0.0 );
			rc1.setReceita( 0.0 );
			rc1.setMargem( 0.0 );
			rc1.setMargemPercente( 0.0 );

			for ( String key2 : map2.get( key1 ).keySet() )
			{
				ResumoDisciplinaDTO rc2 = map2.get( key1 ).get( key2 );

				rc1.setCustoDocente( rc1.getCustoDocente() + rc2.getCustoDocente() );
				rc1.setReceita( rc1.getReceita() + rc2.getReceita() );
				rc1.setMargem( rc1.getMargem() + rc2.getMargem() );
				rc1.setMargemPercente( TriedaUtil.roundTwoDecimals( rc1.getMargem() / rc1.getReceita() ) );
			}
		}
	}

	private void createResumoNivel1( Map< String, ResumoDisciplinaDTO > map1,
			Map< String, Map< String, ResumoDisciplinaDTO > > map2,
			ResumoDisciplinaDTO resumoDTO )
	{
		String key1 = resumoDTO.getDisciplinaId() + "";

		if ( !map1.containsKey( key1 ) )
		{
			ResumoDisciplinaDTO resumoDTONew = new ResumoDisciplinaDTO();

			resumoDTONew.setDisciplinaId( resumoDTO.getDisciplinaId() );
			resumoDTONew.setDisciplinaString( resumoDTO.getDisciplinaString() );

			map1.put( key1, resumoDTONew );
			map2.put( key1, new HashMap< String, ResumoDisciplinaDTO >() );
		}
	}

	private void createResumoNivel2(
			Map< String, Map< String, ResumoDisciplinaDTO > > map2,
			ResumoDisciplinaDTO resumoDTO )
	{
		String key1 = resumoDTO.getDisciplinaId() + "";
		String key2 = key1 + "-" + resumoDTO.getTurma()
				+ "-" + resumoDTO.getTipoCreditoTeorico();

		if ( !map2.get( key1 ).containsKey( key2 ) )
		{
			ResumoDisciplinaDTO resumoDTONew = new ResumoDisciplinaDTO();

			resumoDTONew.setDisciplinaId( resumoDTO.getDisciplinaId() );
			resumoDTONew.setDisciplinaString( resumoDTO.getDisciplinaString() );
			resumoDTONew.setTurma( resumoDTO.getTurma() );
			resumoDTONew.setTipoCreditoTeorico( resumoDTO.getTipoCreditoTeorico() );
			resumoDTONew.setCreditos( resumoDTO.getCreditos() );
			resumoDTONew.setTotalCreditos( resumoDTO.getTotalCreditos() );
			resumoDTONew.setQuantidadeAlunos( resumoDTO.getQuantidadeAlunos() );

			map2.get( key1 ).put( key2, resumoDTONew );
		}
	}
}
