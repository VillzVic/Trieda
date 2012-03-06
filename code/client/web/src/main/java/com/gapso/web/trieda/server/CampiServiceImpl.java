package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.TreeMap;

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
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.DeslocamentoCampus;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Estados;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AbstractDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoCampusDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.CampiService;
import com.gapso.web.trieda.shared.util.TriedaUtil;

@Transactional
public class CampiServiceImpl extends RemoteService
	implements CampiService
{
	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public CampusDTO getCampus( Long id )
	{
		if ( id == null )
		{
			return null;
		}

		Campus campus = Campus.find(
			id, this.getInstituicaoEnsinoUser() );

		if ( campus == null )
		{
			return null;
		}

		return ConvertBeans.toCampusDTO( campus );
	}

	@Override
	public PagingLoadResult< HorarioDisponivelCenarioDTO > getHorariosDisponiveis(
		CampusDTO campusDTO )
	{
		Campus campus = Campus.find(
			campusDTO.getId(), this.getInstituicaoEnsinoUser() );

		List< HorarioDisponivelCenario > list
			= new ArrayList< HorarioDisponivelCenario>();

		if ( campus != null )
		{
			List< HorarioDisponivelCenario > listHorarios
				= campus.getHorarios( this.getInstituicaoEnsinoUser() ); 

			if ( listHorarios != null )
			{
				list.addAll( listHorarios );
			}
		}

		List< HorarioDisponivelCenarioDTO > listDTO
			= ConvertBeans.toHorarioDisponivelCenarioDTO( list );

		Map< String, List< HorarioDisponivelCenarioDTO > > horariosTurnos
			= new HashMap< String, List< HorarioDisponivelCenarioDTO > >();

		for ( HorarioDisponivelCenarioDTO o : listDTO )
		{
			List< HorarioDisponivelCenarioDTO > horarios
				= horariosTurnos.get( o.getTurnoString() );

			if ( horarios == null )
			{
				horarios = new ArrayList< HorarioDisponivelCenarioDTO >();
				horariosTurnos.put( o.getTurnoString(), horarios );
			}

			horarios.add( o );
		}

		for ( Entry< String, List< HorarioDisponivelCenarioDTO > > entry
			: horariosTurnos.entrySet() )
		{
			Collections.sort( entry.getValue() );
		}

		Map< Date, List< String > > horariosFinalTurnos
			= new TreeMap< Date, List< String > >();

		for ( Entry< String, List< HorarioDisponivelCenarioDTO > > entry
			: horariosTurnos.entrySet() )
		{
			Date ultimoHorario = entry.getValue().get(
				entry.getValue().size() - 1 ).getHorario();

			List< String > turnos = horariosFinalTurnos.get( ultimoHorario );
			if ( turnos == null )
			{
				turnos = new ArrayList< String >();
				horariosFinalTurnos.put( ultimoHorario, turnos );
			}

			turnos.add( entry.getKey() );
		}

		listDTO.clear();

		for ( Entry< Date, List< String > > entry
			: horariosFinalTurnos.entrySet() )
		{
			for ( String turno : entry.getValue() )
			{
				listDTO.addAll( horariosTurnos.get( turno ) );
			}
		}

		return new BasePagingLoadResult< HorarioDisponivelCenarioDTO >( listDTO );
	}

	@Override
	public void saveHorariosDisponiveis(
		CampusDTO campusDTO, List< HorarioDisponivelCenarioDTO > listDTO )
	{
		List< HorarioDisponivelCenario > listSelecionados
			= ConvertBeans.toHorarioDisponivelCenario( listDTO );

		Campus campus = Campus.find(
			campusDTO.getId(), this.getInstituicaoEnsinoUser() );

		List< Unidade > unidades = Unidade.findByCampus(
			getInstituicaoEnsinoUser(), campus );

		List< Sala > salas = new ArrayList< Sala >();

		for ( Unidade unidade : unidades )
		{
			salas.addAll( Sala.findByUnidade(
				getInstituicaoEnsinoUser(), unidade ) );
		}

		List< HorarioDisponivelCenario > removerList
			= new ArrayList< HorarioDisponivelCenario >(
				campus.getHorarios( this.getInstituicaoEnsinoUser() ) );

		removerList.removeAll( listSelecionados );

		for ( HorarioDisponivelCenario o : removerList )
		{
			o.getCampi().remove( campus );
			o.getUnidades().removeAll( unidades );
			o.getSalas().removeAll( salas );
			o.merge();
		}

		List< HorarioDisponivelCenario > adicionarList
			= new ArrayList< HorarioDisponivelCenario >( listSelecionados );

		adicionarList.removeAll( campus.getHorarios(
			this.getInstituicaoEnsinoUser() ) );

		for ( HorarioDisponivelCenario o : adicionarList )
		{
			o.getCampi().add( campus );
			o.getUnidades().addAll( unidades );
			o.getSalas().addAll( salas );
			o.merge();
		}
	}

	@Override
	public ListLoadResult< CampusDTO > getListByCurriculo(
		CurriculoDTO curriculoDTO )
	{
		Curriculo curriculo = Curriculo.find(
			curriculoDTO.getId(), getInstituicaoEnsinoUser() );

		List< CampusDTO > campiDTO = new ArrayList< CampusDTO >();
		List< Campus > campi = Campus.findByCurriculo(
			this.getInstituicaoEnsinoUser(), curriculo );

		for ( Campus c : campi )
		{
			campiDTO.add( ConvertBeans.toCampusDTO( c ) );
		}
		return new BaseListLoadResult< CampusDTO >( campiDTO );
	}

	@Override
	public ListLoadResult< CampusDTO > getListAll()
	{
		List< CampusDTO > campiDTO = new ArrayList< CampusDTO >();
		List< Campus > campi = Campus.findAll( this.getInstituicaoEnsinoUser() );

		for ( Campus c : campi )
		{
			campiDTO.add( ConvertBeans.toCampusDTO( c ) );
		}

		return new BaseListLoadResult< CampusDTO >( campiDTO );
	}

	@Override
	public PagingLoadResult< CampusDTO > getList(
		PagingLoadConfig config )
	{
		List< CampusDTO > list = new ArrayList< CampusDTO >();
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

		List< Campus > listDomains = Campus.find(
			this.getInstituicaoEnsinoUser(),
			config.getOffset(), config.getLimit(),	orderBy ); 

		if ( listDomains != null )
		{
			for ( Campus campus : listDomains )
			{
				list.add( ConvertBeans.toCampusDTO( campus ) );
			}
		}

		BasePagingLoadResult< CampusDTO > result
			= new BasePagingLoadResult< CampusDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Campus.count(
			this.getInstituicaoEnsinoUser() ) );

		return result;
	}

	@Override
	public ListLoadResult< CampusDTO > getList(
		BasePagingLoadConfig loadConfig )
	{
		CenarioDTO cenarioDTO = ConvertBeans.toCenarioDTO(
			Cenario.findMasterData( this.getInstituicaoEnsinoUser() ) );

		return getBuscaList( cenarioDTO, null,
			loadConfig.get( "query" ).toString(),
			null, null, null, loadConfig);
	}

	@Override
	public PagingLoadResult< CampusDTO > getBuscaList( CenarioDTO cenarioDTO,
		String nome, String codigo, String estadoString, String municipio,
		String bairro, PagingLoadConfig config )
	{
		Cenario cenario = Cenario.find(
			cenarioDTO.getId(), this.getInstituicaoEnsinoUser() );

		List< CampusDTO > list = new ArrayList< CampusDTO >();
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

		Estados estadoDomain = null;

		if ( estadoString != null )
		{
			for ( Estados estado : Estados.values() )
			{
				if ( estado.name().equals( estadoString ) )
				{
					estadoDomain = estado;
					break;
				}
			}
		}

		List< Campus > campi = Campus.findBy(
			this.getInstituicaoEnsinoUser(), cenario, nome, codigo, estadoDomain,
			municipio, bairro, config.getOffset(), config.getLimit(), orderBy );

		for ( Campus campus : campi )
		{
			list.add( ConvertBeans.toCampusDTO( campus ) );
		}

		BasePagingLoadResult< CampusDTO > result
			= new BasePagingLoadResult< CampusDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Campus.count(
			this.getInstituicaoEnsinoUser(), cenario,
			nome, codigo, estadoDomain, municipio, bairro ) );

		return result;
	}

	@Override
	public ListLoadResult< CampusDTO > getList()
	{
		List< CampusDTO > list = new ArrayList< CampusDTO >();
		List< Campus > listDomains =  Campus.findAll(
			this.getInstituicaoEnsinoUser() );

		for ( Campus campus : listDomains )
		{
			list.add( ConvertBeans.toCampusDTO( campus ) );
		}

		return new BaseListLoadResult< CampusDTO >( list );
	}

	@Override
	public void save( CampusDTO campusDTO )
	{
		Campus campus = ConvertBeans.toCampus( campusDTO );

		if ( campus.getId() != null && campus.getId() > 0 )
		{
			campus.merge();
		}
		else
		{
			campus.persistAndPreencheHorarios();
		}
	}

	@Override
	public void remove(List<CampusDTO> campusDTOList) {
		for (CampusDTO campusDTO : campusDTOList) {
			ConvertBeans.toCampus(campusDTO).remove();
		}
	}

	@Override
	public List<DeslocamentoCampusDTO> getDeslocamentos()
	{
		List< DeslocamentoCampusDTO > list
			= new ArrayList< DeslocamentoCampusDTO >();

		List< Campus > listCampi
			= Campus.findAll( this.getInstituicaoEnsinoUser() );

		for ( Campus unidade : listCampi )
		{
			list.add( ConvertBeans.toDeslocamentoCampusDTO( unidade, listCampi ) );
		}

		Collections.sort(list, new Comparator< DeslocamentoCampusDTO >()
		{
			@Override
			public int compare( DeslocamentoCampusDTO o1,
				DeslocamentoCampusDTO o2 )
			{
				return o1.get( "origemString" ).toString()
					.compareToIgnoreCase( o2.get( "origemString" ).toString() );
			}
		});

		return list;
	}

	@Override
	public void saveDeslocamento( CenarioDTO cenarioDTO,
		List< DeslocamentoCampusDTO > list )
	{
		Cenario cenario = Cenario.find(
			cenarioDTO.getId(), this.getInstituicaoEnsinoUser() );

		List< DeslocamentoCampus > deslocamentos = DeslocamentoCampus.findAllByCampus(
			this.getInstituicaoEnsinoUser(), cenario );

		for ( DeslocamentoCampus deslocamento : deslocamentos )
		{
			deslocamento.remove();
		}

		for ( DeslocamentoCampusDTO deslocamentoCampusDTO : list )
		{
			List< DeslocamentoCampus > deslCamList
				= ConvertBeans.toDeslocamentoCampus( deslocamentoCampusDTO );

			for ( DeslocamentoCampus desl : deslCamList )
			{
				desl.persist();
			}
		}
	}

	@Override
	public List< TreeNodeDTO > getResumos(
		CenarioDTO cenarioDTO, TreeNodeDTO currentNode )
	{
		List< TreeNodeDTO > list = new ArrayList< TreeNodeDTO >();

		if ( currentNode == null )
		{
			Cenario cenario = Cenario.find(
				cenarioDTO.getId(), this.getInstituicaoEnsinoUser() );

			List< Campus > campi
				= new ArrayList< Campus >( cenario.getCampi() );

			Collections.sort( campi );

			for ( Campus campus : campi )
			{
				CampusDTO campusDTO = ConvertBeans.toCampusDTO( campus );
				TreeNodeDTO nodeDTO = new TreeNodeDTO( campusDTO );
				nodeDTO.setLeaf( true );
				list.add( nodeDTO );
			}
		}
		else
		{
			AbstractDTO< ? > contentCurrentNode = currentNode.getContent();
			Campus campus = Campus.find( ( (CampusDTO) contentCurrentNode ).getId(),this.getInstituicaoEnsinoUser() );
			boolean tatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());

			if ( tatico )
			{
				list = getResumosTatico( cenarioDTO, currentNode );
			}
			else
			{
				list = getResumosOperacional( cenarioDTO, currentNode );
			}
		}

		return list;
	}

	private List< TreeNodeDTO > getResumosTatico(
		CenarioDTO cenarioDTO, TreeNodeDTO currentNode )
	{
		List< TreeNodeDTO > list = new ArrayList< TreeNodeDTO >();

		AbstractDTO< ? > contentCurrentNode = currentNode.getContent();

		Campus campus = Campus.find( ( (CampusDTO) contentCurrentNode ).getId(),
			this.getInstituicaoEnsinoUser() );

		Double custoCredito = ( ( campus.getValorCredito() != null ) ?
			campus.getValorCredito() : 0.0 );

		Integer qtdTurma = AtendimentoTatico.countTurma(getInstituicaoEnsinoUser(), campus );
		
		List< Demanda > demandas = new ArrayList<Demanda>();
		List< AtendimentoTatico > atendimentoTaticoList = new ArrayList<AtendimentoTatico>();
		
		Map<String,List<AtendimentoTatico>> ofertaIdDisciplinaIdToAtendimentosTaticoMap = new HashMap<String,List<AtendimentoTatico>>();
		Map<String,List<AtendimentoTatico>> salaIdTurnoIdSemanaLetivaIdToAtendimentosTaticoMap = new HashMap<String,List<AtendimentoTatico>>();
		for (Oferta oferta : campus.getOfertas()) {
			demandas.addAll(oferta.getDemandas());
			atendimentoTaticoList.addAll(oferta.getAtendimentosTaticos());
			for (AtendimentoTatico atendimento : oferta.getAtendimentosTaticos()) {
				
				String key1 = oferta.getId() + "-" + atendimento.getDisciplina().getId();
				List<AtendimentoTatico> atendimentos1 = ofertaIdDisciplinaIdToAtendimentosTaticoMap.get(key1);
				if (atendimentos1 == null) {
					atendimentos1 = new ArrayList<AtendimentoTatico>();
					ofertaIdDisciplinaIdToAtendimentosTaticoMap.put(key1,atendimentos1);
				}
				atendimentos1.add(atendimento);
				
				String key2 = atendimento.getSala().getId() + "-" + oferta.getTurno().getId() + "-" + oferta.getCurriculo().getSemanaLetiva().getId();
				List<AtendimentoTatico> atendimentos2 = salaIdTurnoIdSemanaLetivaIdToAtendimentosTaticoMap.get(key2);
				if (atendimentos2 == null) {
					atendimentos2 = new ArrayList<AtendimentoTatico>();
					salaIdTurnoIdSemanaLetivaIdToAtendimentosTaticoMap.put(key2,atendimentos2);
				}
				atendimentos2.add(atendimento);
			}
		}

		Integer qtdAlunosAtendidos = 0;
		Integer qtdAlunosNaoAtendidos = 0;

		Map< Demanda, Integer > qtdAlunosNaoAtendidosDemandaMap
			= new HashMap< Demanda, Integer >();

		if ( demandas != null && demandas.size() > 0 )
		{
			for ( Demanda demanda : demandas )
			{
				String key = demanda.getOferta().getId() + "-" + demanda.getDisciplina().getId();
				List< AtendimentoTatico > atendimentos = ofertaIdDisciplinaIdToAtendimentosTaticoMap.get(key);
				if (atendimentos == null) {
					atendimentos = Collections.<AtendimentoTatico>emptyList();
				}
		
				int demandaAlunosCreditosT
					= ( demanda.getDisciplina().getCreditosTeorico() * demanda.getQuantidade() );
		
				int demandaAlunosCreditosP
					= ( demanda.getDisciplina().getCreditosPratico() * demanda.getQuantidade() );
		
				int demandaAlunosCreditosNeutros = ( demandaAlunosCreditosT + demandaAlunosCreditosP );
		
				for ( AtendimentoTatico atendimento : atendimentos )
				{
					demandaAlunosCreditosT -= ( atendimento.getCreditosTeorico()
						* atendimento.getQuantidadeAlunos() );
		
					demandaAlunosCreditosP -= ( atendimento.getCreditosPratico()
						* atendimento.getQuantidadeAlunos() );
		
					demandaAlunosCreditosNeutros -= ( atendimento.getTotalCreditos()
						* atendimento.getQuantidadeAlunos() );
				}
		
				int qtdAlunosNaoAtendidosDemanda = 0;
		
				if ( atendimentos.isEmpty() )
				{
					qtdAlunosNaoAtendidos += demanda.getQuantidade();
					qtdAlunosNaoAtendidosDemanda = demanda.getQuantidade();
				}
				else
				{
					if ( ( demandaAlunosCreditosP > 0 )
						&& !demanda.getDisciplina().getLaboratorio() )
					{
						if ( demandaAlunosCreditosNeutros > 0 )
						{
							qtdAlunosNaoAtendidos += demandaAlunosCreditosNeutros
									/ demanda.getDisciplina().getTotalCreditos();
		
							qtdAlunosNaoAtendidosDemanda = demandaAlunosCreditosNeutros
									/ demanda.getDisciplina().getTotalCreditos();
						}
						else if ( demandaAlunosCreditosNeutros < 0 )
						{
		
						}
					}
					else
					{
						if ( demandaAlunosCreditosT > 0 )
						{
							qtdAlunosNaoAtendidos += demandaAlunosCreditosT
									/ demanda.getDisciplina().getCreditosTeorico();
		
							qtdAlunosNaoAtendidosDemanda = demandaAlunosCreditosT
									/ demanda.getDisciplina().getCreditosTeorico();
						}
						else if ( demandaAlunosCreditosT < 0 )
						{
		
						}
		
						if ( demandaAlunosCreditosP > 0 )
						{
							qtdAlunosNaoAtendidos += demandaAlunosCreditosP
									/ demanda.getDisciplina().getCreditosPratico();
		
							qtdAlunosNaoAtendidosDemanda = demandaAlunosCreditosP
									/ demanda.getDisciplina().getCreditosPratico();
						}
						else if ( demandaAlunosCreditosT < 0 )
						{
		
						}
					}
				}
		
				qtdAlunosNaoAtendidosDemandaMap.put(
					demanda, qtdAlunosNaoAtendidosDemanda );
			}
		}

		qtdAlunosAtendidos = ( Demanda.sumDemanda( getInstituicaoEnsinoUser(), campus ) - qtdAlunosNaoAtendidos );
		
		//***************TODO: LOG
//		for (Entry<Demanda, Integer> entry : qtdAlunosNaoAtendidosDemandaMap.entrySet()) {
//			Demanda demanda = entry.getKey();
//			Disciplina disciplina = demanda.getDisciplina();
//			Oferta oferta = demanda.getOferta();
//			Curriculo curriculo = oferta.getCurriculo();
//			Integer qtdeNaoAtendida = entry.getValue();
//			System.out.println(oferta.getCampus().getCodigo()+"@"+curriculo.getCodigo()+"@"+curriculo.getPeriodo(disciplina)+"@"+disciplina.getCodigo()+"@"+demanda.getQuantidade()+"@"+qtdeNaoAtendida);
//		}
		//***************TODO: LOG

		Set< Sala > salas = new HashSet< Sala >();
		Set< Turno > turnos = new HashSet< Turno >();
		Set< SemanaLetiva > semanasLetivas = new HashSet< SemanaLetiva >();

		for ( AtendimentoTatico atendimentoTatico : atendimentoTaticoList )
		{
			salas.add( atendimentoTatico.getSala() );
			turnos.add( atendimentoTatico.getOferta().getTurno() );
			semanasLetivas.add( atendimentoTatico.getOferta().getCurriculo().getSemanaLetiva() );
		}

//		Collection< SalaDTO > salasDTO = ConvertBeans.toSalaDTO( salas );
//		Collection< TurnoDTO > turnosDTO = ConvertBeans.toTurnoDTO( turnos );
//		Collection< SemanaLetivaDTO > semanasLetivasDTO = ConvertBeans.toSemanaLetivaDTO( semanasLetivas );

		double numeradorSala = 0.0;
		double denominadorSala = 0.0;
		double numeradorLab = 0.0;
		double denominadorLab = 0.0;

		Integer qtdCreditos = 0;
		AtendimentosServiceImpl atService = new AtendimentosServiceImpl();

		for ( Turno turno : turnos )
		{
			for ( Sala sala : salas )
			{
				for ( SemanaLetiva semanaLetiva : semanasLetivas )
				{
					//List< AtendimentoRelatorioDTO > atendimentosDTO = atService.getBusca( salaDTO, turnoDTO, semanaLetivaDTO );
					String key = sala.getId() + "-" + turno.getId() + "-" + semanaLetiva.getId();
					List<AtendimentoTatico> atendimentosTatico = salaIdTurnoIdSemanaLetivaIdToAtendimentosTaticoMap.get(key);
					if (atendimentosTatico != null) {
						List<AtendimentoRelatorioDTO> atendimentosTaticoDTO = new ArrayList<AtendimentoRelatorioDTO>();
						for (AtendimentoTatico atendimentoTatico : atendimentosTatico) {
							atendimentosTaticoDTO.add( ConvertBeans.toAtendimentoTaticoDTO( atendimentoTatico ) );
						}
						List< AtendimentoRelatorioDTO > atendimentosDTO = atService.montaListaParaVisaoSala(atendimentosTaticoDTO);
	
						for ( AtendimentoRelatorioDTO atendimentoDTO : atendimentosDTO )
						{
							if ( !sala.isLaboratorio() )
							{
								numeradorSala += atendimentoDTO.getQuantidadeAlunos();
								denominadorSala += sala.getCapacidade();
							}
							else
							{
								numeradorLab += atendimentoDTO.getQuantidadeAlunos();
								denominadorLab += sala.getCapacidade();
							}
		
							qtdCreditos += atendimentoDTO.getTotalCreditos();
						}
					}
				}
			}
		}

		double mediaSalaDeAula = TriedaUtil.round(
			numeradorSala / denominadorSala * 100.0, 2 );

		double mediaLaboratorio = TriedaUtil.round(
			numeradorLab / denominadorLab * 100.0, 2 );

		Double mediaCreditoTurma = ( ( qtdTurma == 0 ) ? 0.0 : ( qtdCreditos / qtdTurma ) );
		Double custoDocenteSemestral = ( qtdCreditos * custoCredito * 4.5 * 6.0 );

		Double receitaSemestral = 0.0;
		for ( Demanda demanda : qtdAlunosNaoAtendidosDemandaMap.keySet() )
		{
			int qtdAlunosAtendidosDemanda
				= ( demanda.getQuantidade() - qtdAlunosNaoAtendidosDemandaMap.get( demanda ) );

			receitaSemestral += ( demanda.getDisciplina().getCreditosTotal()
				* qtdAlunosAtendidosDemanda	* demanda.getOferta().getReceita() );
		}

		receitaSemestral *= ( 4.5 * 6.0 );

		list.add( new TreeNodeDTO( "Turmas abertas: <b>" + qtdTurma + "</b>" ) );
		list.add( new TreeNodeDTO( "Total de Cr&eacute;ditos semanais: <b>"	+ qtdCreditos + "</b>" ) );
		list.add( new TreeNodeDTO( "M&eacute;dia de cr&eacute;ditos por turma: <b>"	+ mediaCreditoTurma + "</b>" ) );
		list.add( new TreeNodeDTO( "Custo m&eacute;dio do cr&eacute;dito: <b>R$ " + custoCredito + "</b>" ) );
		list.add( new TreeNodeDTO( "Custo docente semestral estimado: <b>R$ "	+ custoDocenteSemestral + "</b>" ) );
		list.add( new TreeNodeDTO( "Receita: <b>R$ " + receitaSemestral + "</b>" ) );
		list.add( new TreeNodeDTO( "Utiliza&ccedil;&atilde;o m&eacute;dia das salas de aula: <b>" + mediaSalaDeAula + "%</b>" ) );
		list.add( new TreeNodeDTO( "Utiliza&ccedil;&atilde;o m&eacute;dia dos laborat&oacute;rios: <b>"	+ mediaLaboratorio + "%</b>" ) );
		list.add( new TreeNodeDTO( "Total de alunos atendidos: <b>"	+ qtdAlunosAtendidos + "</b>" ) );
		list.add( new TreeNodeDTO( "Total de alunos n&atilde;o atendidos: <b>" + qtdAlunosNaoAtendidos + "</b>" ) );
		
		return list;
	}

	private List< TreeNodeDTO > getResumosOperacional(
		CenarioDTO cenarioDTO, TreeNodeDTO currentNode )
	{
		List< TreeNodeDTO > list = new ArrayList< TreeNodeDTO >();

		AbstractDTO< ? > contentCurrentNode = currentNode.getContent();

		Campus campus = Campus.find( ( (CampusDTO) contentCurrentNode ).getId(),
			this.getInstituicaoEnsinoUser() );

		Double custoCredito = ( ( campus.getValorCredito() != null ) ?
			campus.getValorCredito() : 0.0 );

		Integer qtdTurma = AtendimentoOperacional.countTurma(
			getInstituicaoEnsinoUser(), campus );

		List< Demanda > demandas = Demanda.findAllByCampus(
			getInstituicaoEnsinoUser(), campus );

		Integer qtdAlunosAtendidos = 0;
		Integer qtdAlunosNaoAtendidos = 0;

		Map< Demanda, Integer > qtdAlunosNaoAtendidosDemandaMap
			= new HashMap< Demanda, Integer >();

		if ( demandas !=null && demandas.size() > 0 )
		{
			for ( Demanda demanda : demandas )
			{
				List< AtendimentoOperacional > atendimentos
					= AtendimentoOperacional.findAllByDemanda( getInstituicaoEnsinoUser(), demanda );
	
				int demandaAlunosCreditosT
					= ( demanda.getDisciplina().getCreditosTeorico() * demanda.getQuantidade() );
	
				int demandaAlunosCreditosP
					= ( demanda.getDisciplina().getCreditosPratico() * demanda.getQuantidade() );
	
				int demandaCredNeutros = ( demandaAlunosCreditosT + demandaAlunosCreditosP );
	
				for ( AtendimentoOperacional atendimento : atendimentos )
				{
					Integer qtd = atendimento.getQuantidadeAlunos();
					boolean creditosTeoricos = atendimento.getCreditoTeorico();
	
					if ( creditosTeoricos )
					{
						demandaAlunosCreditosT -= qtd;
					}
					else
					{
						demandaAlunosCreditosP -= qtd;
					}
	
					demandaCredNeutros -= qtd;
				}
	
				int qtdAlunosNaoAtendidosDemanda = 0;
	
				if ( atendimentos.isEmpty() )
				{
					qtdAlunosNaoAtendidos += demanda.getQuantidade();
					qtdAlunosNaoAtendidosDemanda = demanda.getQuantidade();
				}
				else
				{
					if ( ( demandaAlunosCreditosP > 0 )
						&& !demanda.getDisciplina().getLaboratorio() )
					{
						if ( demandaCredNeutros > 0 )
						{
							qtdAlunosNaoAtendidos += demandaCredNeutros
									/ demanda.getDisciplina().getTotalCreditos();
	
							qtdAlunosNaoAtendidosDemanda = demandaCredNeutros
									/ demanda.getDisciplina().getTotalCreditos();
						}
						else if ( demandaCredNeutros < 0 )
						{
	
						}
					}
					else
					{
						if ( demandaAlunosCreditosT > 0 )
						{
							qtdAlunosNaoAtendidos += demandaAlunosCreditosT
									/ demanda.getDisciplina().getCreditosTeorico();
	
							qtdAlunosNaoAtendidosDemanda = demandaAlunosCreditosT
									/ demanda.getDisciplina().getCreditosTeorico();
						}
						else if ( demandaAlunosCreditosT < 0 )
						{
	
						}
	
						if ( demandaAlunosCreditosP > 0 )
						{
							qtdAlunosNaoAtendidos += demandaAlunosCreditosP
									/ demanda.getDisciplina().getCreditosPratico();
	
							qtdAlunosNaoAtendidosDemanda = demandaAlunosCreditosP
									/ demanda.getDisciplina().getCreditosPratico();
						}
						else if ( demandaAlunosCreditosT < 0 )
						{
	
						}
					}
				}
	
				qtdAlunosNaoAtendidosDemandaMap.put(
					demanda, qtdAlunosNaoAtendidosDemanda );
			}
		}

		qtdAlunosAtendidos = ( Demanda.sumDemanda( getInstituicaoEnsinoUser(), campus ) - qtdAlunosNaoAtendidos );

		List< AtendimentoOperacional > atendimentoOperacionalList
			= AtendimentoOperacional.findAllByCampus( getInstituicaoEnsinoUser(), campus );

		Set< Sala > salas = new HashSet< Sala >();
		Set< Turno > turnos = new HashSet< Turno >();
		Set< SemanaLetiva > semanasLetivas = new HashSet< SemanaLetiva >();

		for ( AtendimentoOperacional atendimentoOperacional : atendimentoOperacionalList )
		{
			salas.add( atendimentoOperacional.getSala() );
			turnos.add( atendimentoOperacional.getOferta().getTurno() );
			semanasLetivas.add( atendimentoOperacional.getOferta().getCurriculo().getSemanaLetiva() );
		}

		Collection< SalaDTO > salasDTO = ConvertBeans.toSalaDTO( salas );
		Collection< TurnoDTO > turnosDTO = ConvertBeans.toTurnoDTO( turnos );
		Collection< SemanaLetivaDTO > semanasLetivasDTO = ConvertBeans.toSemanaLetivaDTO( semanasLetivas );

		double numeradorSala = 0.0;
		double denominadorSala = 0.0;
		double numeradorLab = 0.0;
		double denominadorLab = 0.0;

		Integer qtdCreditos = 0;
		AtendimentosServiceImpl atService = new AtendimentosServiceImpl();

		for ( TurnoDTO turnoDTO : turnosDTO )
		{
			for ( SalaDTO salaDTO : salasDTO )
			{
				for ( SemanaLetivaDTO semanaLetivaDTO : semanasLetivasDTO )
				{
					List< AtendimentoRelatorioDTO > atendimentosDTO
						= atService.getBusca( salaDTO, turnoDTO, semanaLetivaDTO );

					for ( AtendimentoRelatorioDTO atendimentoDTO : atendimentosDTO )
					{
						if ( !salaDTO.isLaboratorio() )
						{
							numeradorSala += atendimentoDTO.getQuantidadeAlunos();
							denominadorSala += salaDTO.getCapacidade();
						}
						else
						{
							numeradorLab += atendimentoDTO.getQuantidadeAlunos();
							denominadorLab += salaDTO.getCapacidade();
						}
	
						qtdCreditos += atendimentoDTO.getTotalCreditos();
					}
				}
			}
		}

		double mediaSalaDeAula = TriedaUtil.round(
			numeradorSala / denominadorSala * 100.0, 2 );

		double mediaLaboratorio = TriedaUtil.round(
			numeradorLab / denominadorLab * 100.0, 2 );

		Double receitaSemestral = 0.0;
		Double mediaCreditoTurma = ( ( qtdTurma == 0 ) ? 0.0 : ( qtdCreditos / qtdTurma ) );
		Double custoDocenteSemestral = ( qtdCreditos * custoCredito * 4.5 * 6.0 );

		for ( Demanda demanda : qtdAlunosNaoAtendidosDemandaMap.keySet() )
		{
			int qtdAlunosAtendidosDemanda
				= ( demanda.getQuantidade() - qtdAlunosNaoAtendidosDemandaMap.get( demanda ) );

			receitaSemestral += ( demanda.getDisciplina().getCreditosTotal()
				* qtdAlunosAtendidosDemanda	* demanda.getOferta().getReceita() );
		}

		receitaSemestral *= ( 4.5 * 6.0 );

		list.add( new TreeNodeDTO( "Turmas abertas: <b>" + qtdTurma + "</b>" ) );
		list.add( new TreeNodeDTO( "Total de Cr&eacute;ditos semanais: <b>"	+ qtdCreditos + "</b>" ) );
		list.add( new TreeNodeDTO( "M&eacute;dia de cr&eacute;ditos por turma: <b>"	+ mediaCreditoTurma + "</b>" ) );
		list.add( new TreeNodeDTO( "Custo m&eacute;dio do cr&eacute;dito: <b>R$ " + custoCredito + "</b>" ) );
		list.add( new TreeNodeDTO( "Custo docente semestral estimado: <b>R$ "	+ custoDocenteSemestral + "</b>" ) );
		list.add( new TreeNodeDTO( "Receita: <b>R$ " + receitaSemestral + "</b>" ) );
		list.add( new TreeNodeDTO( "Utiliza&ccedil;&atilde;o m&eacute;dia das salas de aula: <b>" + mediaSalaDeAula + "%</b>" ) );
		list.add( new TreeNodeDTO( "Utiliza&ccedil;&atilde;o m&eacute;dia dos laborat&oacute;rios: <b>"	+ mediaLaboratorio + "%</b>" ) );
		list.add( new TreeNodeDTO( "Total de alunos atendidos: <b>"	+ qtdAlunosAtendidos + "</b>" ) );
		list.add( new TreeNodeDTO( "Total de alunos n&atilde;o atendidos: <b>" + qtdAlunosNaoAtendidos + "</b>" ) );

		return list;
	}
}
