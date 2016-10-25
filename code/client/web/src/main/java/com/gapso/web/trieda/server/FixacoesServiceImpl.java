package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.TreeMap;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Fixacao;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.FixacaoDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.services.FixacoesService;

public class FixacoesServiceImpl extends RemoteService implements FixacoesService
{
	private static final long serialVersionUID = -594991176048559553L;

	@Override
	public FixacaoDTO getFixacao(Long id)
	{
		return ConvertBeans.toFixacaoDTO(Fixacao.find(id, getInstituicaoEnsinoUser()));
	}

	@Override
	public PagingLoadResult<FixacaoDTO> getBuscaList(CenarioDTO cenarioDTO, String descricao, PagingLoadConfig config)
	{
		Cenario cenario = ConvertBeans.toCenario(cenarioDTO);

		String orderBy = config.getSortField();

		if (orderBy != null)
		{
			if (config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC))
			{
				orderBy = (orderBy + " asc");
			}
			else
			{
				orderBy = (orderBy + " desc");
			}
		}

		List<FixacaoDTO> list = new ArrayList<FixacaoDTO>();
		List<Fixacao> listDomains = Fixacao.findBy(getInstituicaoEnsinoUser(), cenario, descricao, config.getOffset(), config.getLimit(), orderBy);

		for (Fixacao fixacao : listDomains)
		{
			list.add(ConvertBeans.toFixacaoDTO(fixacao));
		}

		BasePagingLoadResult<FixacaoDTO> result = new BasePagingLoadResult<FixacaoDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Fixacao.count(getInstituicaoEnsinoUser(), cenario));

		return result;
	}

	@Override
	public void save(FixacaoDTO fixacaoDTO, List<HorarioDisponivelCenarioDTO> hdcDTOList, List<ProfessorDTO> professorDTOList, List<UnidadeDTO> unidadeDTOList, List<SalaDTO> salaDTOList)
	{
		Fixacao.entityManager().clear();
		Fixacao fixacao = ConvertBeans.toFixacao(fixacaoDTO);

		List<HorarioDisponivelCenario> listSelecionados = ConvertBeans.toHorarioDisponivelCenario(hdcDTOList);

		List<HorarioDisponivelCenario> adicionarList = new ArrayList<HorarioDisponivelCenario>(listSelecionados);

		List<Professor> listProfessores = ConvertBeans.toProfessoresFixados(professorDTOList);

		List<Professor> adicionarListProfessor = new ArrayList<Professor>(listProfessores);

		List<Sala> listSalas = ConvertBeans.toSala(salaDTOList);

		List<Sala> adicionarListSala = new ArrayList<Sala>(listSalas);

		List<Unidade> listUnidades = ConvertBeans.toUnidade(unidadeDTOList);

		List<Unidade> adicionarListUnidade = new ArrayList<Unidade>(listUnidades);

		// List< SemanaLetiva > semanasLetivas
		// = SemanaLetiva.findAll( getInstituicaoEnsinoUser() );

		if (fixacao.getId() != null && fixacao.getId() > 0)
		{
			fixacao.merge();
		}
		else
		{
			fixacao.persist();
		}

		long id = fixacao.getId();
		Fixacao.entityManager().detach(fixacao);
		fixacao = Fixacao.find(id, getInstituicaoEnsinoUser());

		adicionarList.removeAll(fixacao.getHorarios(getInstituicaoEnsinoUser(), fixacao.getCenario()));

		for (HorarioDisponivelCenario o : adicionarList)
		{
			HorarioDisponivelCenario hdc = HorarioDisponivelCenario.find(o.getId(), getInstituicaoEnsinoUser());

			if (hdc != null)
			{
				hdc.getFixacoes().add(fixacao);
				hdc.merge();
				fixacao.setTeoricoHorario(hdc.getId().intValue());
			}
		}

		List<HorarioDisponivelCenario> removerList = new ArrayList<HorarioDisponivelCenario>(fixacao.getHorarios(getInstituicaoEnsinoUser(), fixacao.getCenario()));

		removerList.removeAll(listSelecionados);

		for (HorarioDisponivelCenario o : removerList)
		{
			o.getFixacoes().remove(fixacao);
			o.merge();
		}

		adicionarListProfessor.removeAll(fixacao.getProfessores(getInstituicaoEnsinoUser(), fixacao.getCenario()));// PROFESSOR

		for (Professor o : adicionarListProfessor)
		{
			Professor professor = Professor.find(o.getId(), getInstituicaoEnsinoUser());

			if (professor != null)
			{
				professor.getFixacoes().add(fixacao);
				professor.merge();
			}
		}

		List<Professor> removerListProfessor = new ArrayList<Professor>(fixacao.getProfessores(getInstituicaoEnsinoUser(), fixacao.getCenario()));

		removerListProfessor.removeAll(listProfessores);

		for (Professor o : removerListProfessor)
		{
			o.getFixacoes().remove(fixacao);
			o.merge();
		}

		adicionarListUnidade.removeAll(fixacao.getUnidades(getInstituicaoEnsinoUser(), fixacao.getCenario())); // UNIDADE

		for (Unidade o : adicionarListUnidade)
		{
			Unidade unidade = Unidade.find(o.getId(), getInstituicaoEnsinoUser());

			if (unidade != null)
			{
				unidade.getFixacoes().add(fixacao);
				unidade.merge();
			}
		}

		List<Unidade> removerListUnidade = new ArrayList<Unidade>(fixacao.getUnidades(getInstituicaoEnsinoUser(), fixacao.getCenario()));

		removerListUnidade.removeAll(listUnidades);

		for (Unidade o : removerListUnidade)
		{
			o.getFixacoes().remove(fixacao);
			o.merge();
		}

		adicionarListSala.removeAll(fixacao.getSalas(getInstituicaoEnsinoUser(), fixacao.getCenario())); // SALA

		for (Sala o : adicionarListSala)
		{
			Sala sala = Sala.find(o.getId(), getInstituicaoEnsinoUser());

			if (sala != null)
			{
				sala.getFixacoes().add(fixacao);
				sala.merge();
				fixacao.setTeoricoSala(sala.getId().intValue());
			}
		}

		List<Sala> removerListSala = new ArrayList<Sala>(fixacao.getSalas(getInstituicaoEnsinoUser(), fixacao.getCenario()));

		removerListSala.removeAll(listSalas);

		for (Sala o : removerListSala)
		{
			o.getFixacoes().remove(fixacao);
			o.merge();
		}
	}

	@Override
	public void remove(List<FixacaoDTO> fixacaoDTOList)
	{
		for (FixacaoDTO fixacaoDTO : fixacaoDTOList)
		{
			Fixacao.find(fixacaoDTO.getId(), getInstituicaoEnsinoUser()).remove();
		}
	}

	@Override
	public List<HorarioDisponivelCenarioDTO> getHorariosSelecionados(FixacaoDTO fixacaoDTO)
	{
		Fixacao fixacao = Fixacao.find(fixacaoDTO.getId(), getInstituicaoEnsinoUser());

		return ConvertBeans.toHorarioDisponivelCenarioDTO(new ArrayList<HorarioDisponivelCenario>(fixacao.getHorarios(getInstituicaoEnsinoUser(), fixacao.getCenario())));
	}
	
	@Override
	public List<ProfessorDTO> getProfessoresFixados(FixacaoDTO fixacaoDTO)
	{
		Fixacao fixacao = Fixacao.find(fixacaoDTO.getId(), getInstituicaoEnsinoUser());

		return ConvertBeans.toProfessoresFixadosDTO(new ArrayList<Professor>(fixacao.getProfessores(getInstituicaoEnsinoUser(), fixacao.getCenario())));
	}

	@Override
	public PagingLoadResult<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(DisciplinaDTO disciplinaDTO, AtendimentoOperacionalDTO turma)
	{
		List<HorarioDisponivelCenario> disciplinaHorarios = null;
		List<HorarioDisponivelCenario> turmaHorarios = null;
		Turno turnoHorario = null;
		Curriculo curriculoHorario = null;
		AtendimentoOperacional atendimentoTurma = null;
		List<HorarioDisponivelCenario> list = null;
		
		if (disciplinaDTO == null && turma.getTurma() == null)
		{
			return new BasePagingLoadResult<HorarioDisponivelCenarioDTO>(new ArrayList<HorarioDisponivelCenarioDTO>());
		}
		else
		{			
			try
			{
				if (disciplinaDTO != null)
				{
					Disciplina disciplina = Disciplina.find(disciplinaDTO.getId(), getInstituicaoEnsinoUser());

					if (disciplina != null)
					{
						disciplinaHorarios = disciplina.getHorarios(getInstituicaoEnsinoUser());
					}
				}
				
				if (turma.getTurma() != null) 
				{
					String curriculoStr = "", turnoStr = "", turmaStr = "";
					try
					{
						String[] str = turma.getTurma().split("-");
						turmaStr = str[0].trim();
						curriculoStr = str[1].trim();
						turnoStr = str[2].trim();
					}
					catch (Exception e){}
					
					turnoHorario = Turno.findByNome(getInstituicaoEnsinoSuperUser(), getCenario(), turnoStr );
					curriculoHorario = Curriculo.findByCodigo(curriculoStr, getCenario(), getInstituicaoEnsinoSuperUser());

					atendimentoTurma = new AtendimentoOperacional();
					turmaHorarios = atendimentoTurma.getHorarios(getInstituicaoEnsinoUser(), turmaStr, curriculoHorario, turnoHorario);
				}
			}
			catch (Exception e){}
	
			list = intercessaoHorarios(disciplinaHorarios, turmaHorarios);
	
			List<HorarioDisponivelCenarioDTO> listDTO = ConvertBeans.toHorarioDisponivelCenarioDTO(list);
	
			Map<String, List<HorarioDisponivelCenarioDTO>> horariosTurnos = new HashMap<String, List<HorarioDisponivelCenarioDTO>>();
	
			for (HorarioDisponivelCenarioDTO o : listDTO)
			{
				List<HorarioDisponivelCenarioDTO> horarios = horariosTurnos.get(o.getTurnoString());
	
				if (horarios == null)
				{
					horarios = new ArrayList<HorarioDisponivelCenarioDTO>();
					horariosTurnos.put(o.getTurnoString(), horarios);
				}
	
				horarios.add(o);
			}
	
			for (Entry<String, List<HorarioDisponivelCenarioDTO>> entry : horariosTurnos.entrySet())
			{
				Collections.sort(entry.getValue());
			}
	
			Map<Date, List<String>> horariosFinalTurnos = new TreeMap<Date, List<String>>();
	
			for (Entry<String, List<HorarioDisponivelCenarioDTO>> entry : horariosTurnos.entrySet())
			{
				Date ultimoHorario = entry.getValue().get(entry.getValue().size() - 1).getHorario();
	
				List<String> turnos = horariosFinalTurnos.get(ultimoHorario);
				if (turnos == null)
				{
					turnos = new ArrayList<String>();
					horariosFinalTurnos.put(ultimoHorario, turnos);
				}
	
				turnos.add(entry.getKey());
			}
	
			listDTO.clear();
			for (Entry<Date, List<String>> entry : horariosFinalTurnos.entrySet())
			{
				for (String turno : entry.getValue())
				{
					listDTO.addAll(horariosTurnos.get(turno));
				}
			}
	
			return new BasePagingLoadResult<HorarioDisponivelCenarioDTO>(listDTO);
		}
	}

	public List<HorarioDisponivelCenario> intercessaoHorarios(Collection<HorarioDisponivelCenario> horario1, Collection<HorarioDisponivelCenario> horario2)
	{
		List<HorarioDisponivelCenario> horarios = new ArrayList<HorarioDisponivelCenario>();

		if (horarios.size() == 0 && horario1 != null)
		{
			horarios.addAll(horario1);
		}

		if (horarios.size() == 0 && horario2 != null)
		{
			horarios.addAll(horario2);
		}

		if (horario1 != null)
		{
			horarios.retainAll(horario1);
		}

		if (horario2 != null)
		{
			horarios.retainAll(horario2);
		}

		return horarios;
	}

	@Override
	public List<UnidadeDTO> getUnidadesFixadas(FixacaoDTO fixacaoDTO)
	{
		Fixacao fixacao = Fixacao.find(fixacaoDTO.getId(), getInstituicaoEnsinoUser());

		return ConvertBeans.toUnidadesFixadosDTO(new ArrayList<Unidade>(fixacao.getUnidades(getInstituicaoEnsinoUser(), fixacao.getCenario())));
	
	}

	@Override
	public Collection<SalaDTO> getSalasFixadas(FixacaoDTO fixacaoDTO)
	{
		Fixacao fixacao = Fixacao.find(fixacaoDTO.getId(), getInstituicaoEnsinoUser());

		return ConvertBeans.toSalaDTO(new ArrayList<Sala>(fixacao.getSalas(getInstituicaoEnsinoUser(), fixacao.getCenario())));
	
	}

}
