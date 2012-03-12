package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.DemandasService;
import com.gapso.web.trieda.shared.util.view.TriedaException;

public class DemandasServiceImpl
	extends RemoteService implements DemandasService
{
	private static final long serialVersionUID = 5250776996542788849L;
	
	/**
	 * @see com.gapso.web.trieda.shared.services.DemandasService#calculaQuantidadeDeNaoAtendimentosPorDemanda(java.util.List)
	 */
	public ParDTO<Map<Demanda,Integer>,Integer> calculaQuantidadeDeNaoAtendimentosPorDemanda(Collection<Oferta> ofertas) {
		// Preenche estruturas auxiliares
		List<Demanda> demandas = new ArrayList<Demanda>();
		List<AtendimentoTatico> atendimentosTaticos = new ArrayList<AtendimentoTatico>();
		Map<String,List<AtendimentoTatico>> ofertaIdDisciplinaIdToAtendimentosTaticoMap = new HashMap<String,List<AtendimentoTatico>>();
		for (Oferta oferta : ofertas) {
			demandas.addAll(oferta.getDemandas());
			atendimentosTaticos.addAll(oferta.getAtendimentosTaticos());
			for (AtendimentoTatico atendimento : oferta.getAtendimentosTaticos()) {
				String key = oferta.getId() + "-" + atendimento.getDisciplina().getId();
				List<AtendimentoTatico> atendimentos = ofertaIdDisciplinaIdToAtendimentosTaticoMap.get(key);
				if (atendimentos == null) {
					atendimentos = new ArrayList<AtendimentoTatico>();
					ofertaIdDisciplinaIdToAtendimentosTaticoMap.put(key,atendimentos);
				}
				atendimentos.add(atendimento);
			}
		}

		// Calcula map de demandas por quantidade de não atendimentos
		Integer qtdAlunosNaoAtendidosTotal = 0;
		Map<Demanda,Integer> demandaToQtdAlunosNaoAtendidosMap = new HashMap<Demanda,Integer>();
		for (Demanda demanda : demandas) {
			// obtém a lista de atendimentos relacionados com a demanda em questão
			String key = demanda.getOferta().getId() + "-" + demanda.getDisciplina().getId();
			List<AtendimentoTatico> atendimentosDaDemanda = ofertaIdDisciplinaIdToAtendimentosTaticoMap.get(key);
			if (atendimentosDaDemanda == null) {
				atendimentosDaDemanda = Collections.<AtendimentoTatico> emptyList();
			}

			// calcula a demanda em termos de créditos teóricos, práticos e total
			int demandaAlunosCreditosT = (demanda.getDisciplina().getCreditosTeorico() * demanda.getQuantidade());
			int demandaAlunosCreditosP = (demanda.getDisciplina().getCreditosPratico() * demanda.getQuantidade());
			int demandaAlunosCreditosTotal = (demandaAlunosCreditosT + demandaAlunosCreditosP);

			// para cada atendimento, desconta o total atendimento da demanda calculada em termos de créditos 
			for (AtendimentoTatico atendimento : atendimentosDaDemanda) {
				demandaAlunosCreditosT -= (atendimento.getCreditosTeorico() * atendimento.getQuantidadeAlunos());
				demandaAlunosCreditosP -= (atendimento.getCreditosPratico() * atendimento.getQuantidadeAlunos());
				demandaAlunosCreditosTotal -= (atendimento.getTotalCreditos() * atendimento.getQuantidadeAlunos());
			}

			// calcula a quantidade de alunos não atendidos
			int qtdAlunosNaoAtendidosDemanda = 0;
			if (atendimentosDaDemanda.isEmpty()) {
				qtdAlunosNaoAtendidosTotal += demanda.getQuantidade();
				qtdAlunosNaoAtendidosDemanda = demanda.getQuantidade();
			} else {
				// calcula a quantidade de alunos não atendidos de acordo com as situações específicas
				
				// verifica se a demanda possui créditos práticos não atendidos, créditos estes que não são exigidos em laboratório
				if ((demandaAlunosCreditosP > 0) && !demanda.getDisciplina().getLaboratorio()) {
					if (demandaAlunosCreditosTotal > 0) {
						qtdAlunosNaoAtendidosTotal += demandaAlunosCreditosTotal / demanda.getDisciplina().getTotalCreditos();
						qtdAlunosNaoAtendidosDemanda = demandaAlunosCreditosTotal / demanda.getDisciplina().getTotalCreditos();
					}
				} else {
					// verifica se a demanda possui créditos teóricos e práticos não atendidos
					if ((demandaAlunosCreditosT > 0) && (demandaAlunosCreditosP > 0)) {
						if (demandaAlunosCreditosT > demandaAlunosCreditosP) {
							qtdAlunosNaoAtendidosTotal += demandaAlunosCreditosT / demanda.getDisciplina().getCreditosTeorico();
							qtdAlunosNaoAtendidosDemanda = demandaAlunosCreditosT / demanda.getDisciplina().getCreditosTeorico();
						} else {
							qtdAlunosNaoAtendidosTotal += demandaAlunosCreditosP / demanda.getDisciplina().getCreditosPratico();
							qtdAlunosNaoAtendidosDemanda = demandaAlunosCreditosP / demanda.getDisciplina().getCreditosPratico();
						}
					} else {
						// verifica se a demanda possui créditos teóricos não atendidos
						if (demandaAlunosCreditosT > 0) {
							qtdAlunosNaoAtendidosTotal += demandaAlunosCreditosT / demanda.getDisciplina().getCreditosTeorico();
							qtdAlunosNaoAtendidosDemanda = demandaAlunosCreditosT / demanda.getDisciplina().getCreditosTeorico();
						}
						
						// verifica se a demanda possui créditos práticos não atendidos
						if (demandaAlunosCreditosP > 0) {
							qtdAlunosNaoAtendidosTotal += demandaAlunosCreditosP / demanda.getDisciplina().getCreditosPratico();
							qtdAlunosNaoAtendidosDemanda = demandaAlunosCreditosP / demanda.getDisciplina().getCreditosPratico();
						}
					}
				}
			}

			demandaToQtdAlunosNaoAtendidosMap.put(demanda,qtdAlunosNaoAtendidosDemanda);
			//System.out.println(demanda.getOferta().getCampus().getCodigo()+"@"+demanda.getOferta().getTurno().getNome()+"@"+demanda.getOferta().getCurso().getCodigo()+"@"+demanda.getOferta().getCurriculo().getCodigo()+"@"+demanda.getOferta().getCurriculo().getPeriodo(demanda.getDisciplina())+"@"+demanda.getDisciplina().getCodigo()+"@"+demanda.getQuantidade()+"@"+qtdAlunosNaoAtendidosDemanda+"@"+qtdAlunosNaoAtendidosTotal);
		}
		
		return ParDTO.create(demandaToQtdAlunosNaoAtendidosMap,qtdAlunosNaoAtendidosTotal);
	}

	@Override
	public PagingLoadResult< DemandaDTO > getBuscaList( CampusDTO campusDTO,
		CursoDTO cursoDTO, CurriculoDTO curriculoDTO,
		TurnoDTO turnoDTO, DisciplinaDTO disciplinaDTO, PagingLoadConfig config )
	{
		List< DemandaDTO > list = new ArrayList< DemandaDTO >();
		String orderBy = config.getSortField();

		if ( orderBy != null )
		{
			if ( config.getSortDir() != null
				&& config.getSortDir().equals( SortDir.DESC ) )
			{
				orderBy = ( orderBy + " asc" );
			}
			else
			{
				orderBy = ( orderBy + " desc" );
			}
		}

		Campus campus = null;
		if ( campusDTO != null )
		{
			campus = ConvertBeans.toCampus( campusDTO );
		}
		
		Curso curso = null;
		if ( cursoDTO != null )
		{
			curso = ConvertBeans.toCurso( cursoDTO );
		}

		Curriculo curriculo	= null;
		if ( curriculoDTO != null )
		{
			curriculo = ConvertBeans.toCurriculo( curriculoDTO );
		}

		Turno turno = null;
		if ( turnoDTO != null )
		{
			turno = ConvertBeans.toTurno( turnoDTO );
		}

		Disciplina disciplina = null;
		if ( disciplinaDTO != null )
		{
			disciplina = ConvertBeans.toDisciplina( disciplinaDTO );
		}

		List< Demanda > listDomains = Demanda.findBy( getInstituicaoEnsinoUser(),
			campus, curso, curriculo, turno, disciplina,
			config.getOffset(), config.getLimit(), orderBy );

		for ( Demanda demanda : listDomains )
		{
			list.add( ConvertBeans.toDemandaDTO( demanda ) );
		}

		BasePagingLoadResult< DemandaDTO > result
			= new BasePagingLoadResult< DemandaDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Demanda.count( getInstituicaoEnsinoUser(),
			campus, curso, curriculo, turno, disciplina ) );

		return result;
	}

	@Override
	public void save( DemandaDTO demandaDTO ) throws TriedaException
	{
		try {
			Demanda d = ConvertBeans.toDemanda( demandaDTO );
	
			if ( d.getId() != null && d.getId() > 0 )
			{
				d.merge();
			}
			else
			{
				List< Demanda > demandas = Demanda.findBy( getInstituicaoEnsinoUser(),
					d.getOferta().getCampus(), d.getOferta().getCurriculo().getCurso(),
					d.getOferta().getCurriculo(), d.getOferta().getTurno(), d.getDisciplina(), 0, 1, null );
	
				if ( !demandas.isEmpty() )
				{
					Integer qtd = d.getQuantidade();
	
					d = demandas.get( 0 );
					d.setQuantidade( qtd );
	
					d.merge();
				}
				else
				{
					d.persist();
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
	}

	@Override
	public void remove( List< DemandaDTO > demandaDTOList )
	{
		for ( DemandaDTO demandaDTO : demandaDTOList )
		{
			Demanda demanda = ConvertBeans.toDemanda( demandaDTO );

			if ( demanda != null )
			{
				demanda.remove();
			}
		}
	}
	
	@Override
	public Integer findPeriodo(DemandaDTO demandaDTO) throws TriedaException {
		Integer periodo = null;
		
		try {
			InstituicaoEnsino instituicaoEnsino = InstituicaoEnsino.find(demandaDTO.getInstituicaoEnsinoId());
			Curriculo curriculo = Curriculo.find(demandaDTO.getCurriculoId(),instituicaoEnsino);
			Disciplina disciplina = Disciplina.find(demandaDTO.getDisciplinaId(),instituicaoEnsino);
			
			periodo = curriculo.getPeriodo(disciplina);
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
		
		return periodo; 
	}
}
