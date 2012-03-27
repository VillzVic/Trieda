package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.TreeMap;

import org.springframework.format.number.CurrencyFormatter;
import org.springframework.format.number.NumberFormatter;
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
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Estados;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AbstractDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoCampusDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
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
	public List<TreeNodeDTO> getResumos(CenarioDTO cenarioDTO, TreeNodeDTO currentNode) {
		List<TreeNodeDTO> list = new ArrayList<TreeNodeDTO>();

		if (currentNode == null){
			// disponibiliza uma pasta para cada campus no relatório de resumo por campi
			Cenario cenario = Cenario.find(cenarioDTO.getId(),this.getInstituicaoEnsinoUser());
			List<Campus> campi = new ArrayList<Campus>(cenario.getCampi());
			Collections.sort(campi);
			for (Campus campus : campi) {
				CampusDTO campusDTO = ConvertBeans.toCampusDTO(campus);
				TreeNodeDTO nodeDTO = new TreeNodeDTO(campusDTO);
				list.add( nodeDTO );
			}
		}
		else {
			// disponibiliza o conteúdo de uma pasta que representa um determinado campus no relatório de resumo por campi
			AbstractDTO<?> contentCurrentNode = currentNode.getContent();
			Campus campus = Campus.find(((CampusDTO)contentCurrentNode ).getId(),this.getInstituicaoEnsinoUser());
			list = getResumoParaCampus(campus,currentNode);
		}

		return list;
	}

	private List<TreeNodeDTO> getResumoParaCampus(Campus campus, TreeNodeDTO currentNode) {
		boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
		
		// cálculo dos indicadores de utilização das salas de aula e laboratórios
		Set<Turno> turnosConsiderados = new HashSet<Turno>();
		Set<Sala> salasUtilizadas = new HashSet<Sala>();
		Map<String,List<AtendimentoRelatorioDTO>> salaIdTurnoIdToAtendimentosMap = new HashMap<String,List<AtendimentoRelatorioDTO>>();
		for (Oferta oferta : campus.getOfertas()) {
			turnosConsiderados.add(oferta.getTurno());
			if (ehTatico) {
				// atendimentos táticos
				for (AtendimentoTatico aula : oferta.getAtendimentosTaticos()) {
					String key = aula.getSala().getId() + "-" + oferta.getTurno().getId();
					List<AtendimentoRelatorioDTO> aulasPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
					if (aulasPorSalaTurno == null) {
						aulasPorSalaTurno = new ArrayList<AtendimentoRelatorioDTO>();
						salaIdTurnoIdToAtendimentosMap.put(key,aulasPorSalaTurno);
					}
					aulasPorSalaTurno.add(ConvertBeans.toAtendimentoTaticoDTO(aula));
					
					salasUtilizadas.add(aula.getSala());
				}
			} else {
				// atendimentos operacionais
				for (AtendimentoOperacional atendimento : oferta.getAtendimentosOperacionais()) {
					String key = atendimento.getSala().getId() + "-" + oferta.getTurno().getId();
					List<AtendimentoRelatorioDTO> atendimetosPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
					if (atendimetosPorSalaTurno == null) {
						atendimetosPorSalaTurno = new ArrayList<AtendimentoRelatorioDTO>();
						salaIdTurnoIdToAtendimentosMap.put(key,atendimetosPorSalaTurno);
					}
					atendimetosPorSalaTurno.add(ConvertBeans.toAtendimentoOperacionalDTO(atendimento));
					
					salasUtilizadas.add(atendimento.getSala());
				}
			}
		}
		
		double somatorioDeAlunosDeTodasAsAulasEmSalasDeAula = 0.0;
		double somatorioDeAlunosDeTodasAsAulasEmLaboratorios = 0.0;
		double somatorioDaCapacidadeDasSalasParaTodasAsAulasEmSalasDeAula = 0.0;
		double somatorioDaCapacidadeDosLaboratoriosParaTodasAsAulasEmLaboratorios = 0.0;
		Integer totalCreditosSemanais = 0;
		Double custoDocenteSemanal = 0.0;
		AtendimentosServiceImpl atService = new AtendimentosServiceImpl();
		for (Turno turno : turnosConsiderados) {
			for (Sala sala : salasUtilizadas) {
				String key = sala.getId() + "-" + turno.getId();
				List<AtendimentoRelatorioDTO> atendimentosPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
				if (atendimentosPorSalaTurno != null) {
					List<AtendimentoRelatorioDTO> aulas = new ArrayList<AtendimentoRelatorioDTO>();
					if (ehTatico) {
						aulas.addAll(atendimentosPorSalaTurno);
					} else {
						List<AtendimentoOperacionalDTO> atendimentosOperacional = new ArrayList<AtendimentoOperacionalDTO>(atendimentosPorSalaTurno.size());
						for (AtendimentoRelatorioDTO atendimento : atendimentosPorSalaTurno) {
							atendimentosOperacional.add((AtendimentoOperacionalDTO)atendimento);
						}
						// processa os atendimentos do operacional e os transforma em aulas
						List<AtendimentoOperacionalDTO> aulasOperacional = atService.extraiAulas(atendimentosOperacional);
						// insere as aulas do modo operacional na lista de atendimentos
						aulas.addAll(aulasOperacional);
					}
					
					// trata compartilhamento de turmas entre cursos
					List<AtendimentoRelatorioDTO> aulasComCompartilhamentos = atService.uneAulasQuePodemSerCompartilhadas(aulas);
					
					for (AtendimentoRelatorioDTO aula : aulasComCompartilhamentos) {
						if (!sala.isLaboratorio()) {
							somatorioDeAlunosDeTodasAsAulasEmSalasDeAula += aula.getQuantidadeAlunos();
							somatorioDaCapacidadeDasSalasParaTodasAsAulasEmSalasDeAula += sala.getCapacidade();
						} else {
							somatorioDeAlunosDeTodasAsAulasEmLaboratorios += aula.getQuantidadeAlunos();
							somatorioDaCapacidadeDosLaboratoriosParaTodasAsAulasEmLaboratorios += sala.getCapacidade();
						}
						totalCreditosSemanais += aula.getTotalCreditos();
						custoDocenteSemanal += aula.getTotalCreditos() * aula.getProfessorCustoCreditoSemanal();
					}
				}
			}
		}
		double utilizacaoMediaDasSalasDeAula = TriedaUtil.round(somatorioDeAlunosDeTodasAsAulasEmSalasDeAula/somatorioDaCapacidadeDasSalasParaTodasAsAulasEmSalasDeAula*100.0,2);
		double utilizacaoMediaDosLaboratorios = TriedaUtil.round(somatorioDeAlunosDeTodasAsAulasEmLaboratorios/somatorioDaCapacidadeDosLaboratoriosParaTodasAsAulasEmLaboratorios*100.0,2);
		
		// cálculo das quantidades de alunos atendidos e não atendidos
		DemandasServiceImpl demandasService = new DemandasServiceImpl();
		ParDTO<Map<Demanda,ParDTO<Integer,Disciplina>>,Integer> pair = demandasService.calculaQuantidadeDeNaoAtendimentosPorDemanda(campus.getOfertas());
		Integer qtdAlunosNaoAtendidos = pair.getSegundo();
		Integer qtdAlunosAtendidos = (Demanda.sumDemanda(getInstituicaoEnsinoUser(),campus ) - qtdAlunosNaoAtendidos);
		Map<Demanda,ParDTO<Integer,Disciplina>> demandaToQtdAlunosNaoAtendidosMap = pair.getPrimeiro();

		// cálculo do indicador de receita semestral
		Double receitaSemestral = 0.0;
		for (Demanda demanda : demandaToQtdAlunosNaoAtendidosMap.keySet()) {
			ParDTO<Integer,Disciplina> par = demandaToQtdAlunosNaoAtendidosMap.get(demanda);
			int qtdAlunosNaoAtendidosDemanda = par.getPrimeiro();
			Disciplina disciplinaSubstituta = par.getSegundo();
			Disciplina disciplinaASerConsiderada = (disciplinaSubstituta != null) ? disciplinaSubstituta : demanda.getDisciplina();
			
			int qtdAlunosAtendidosDemanda = (demanda.getQuantidade() - qtdAlunosNaoAtendidosDemanda);
			receitaSemestral += (disciplinaASerConsiderada.getCreditosTotal()*qtdAlunosAtendidosDemanda*demanda.getOferta().getReceita());
		}
		receitaSemestral *= ( 4.5 * 6.0 );
		receitaSemestral = TriedaUtil.round(receitaSemestral,2);
		
		// cálculo de outros indicadores
		Integer qtdTurmasAbertas = ehTatico ? AtendimentoTatico.countTurma(getInstituicaoEnsinoUser(),campus) : AtendimentoOperacional.countTurma(getInstituicaoEnsinoUser(),campus);
		Double qtdMediaDeCreditosPorTurma = TriedaUtil.round(((qtdTurmasAbertas==0) ? 0.0 : (totalCreditosSemanais/qtdTurmasAbertas)),2);
		Double qtdMediaDeAlunosPorTurma = TriedaUtil.round(((qtdTurmasAbertas==0) ? 0.0 : (qtdAlunosAtendidos/qtdTurmasAbertas)),2);
		Double custoMedioSemanalPorCredito = TriedaUtil.round(((totalCreditosSemanais != 0) ? custoDocenteSemanal/totalCreditosSemanais : 0.0),2);
		Double custoDocenteSemestral = TriedaUtil.round((custoDocenteSemanal*4.5*6.0),2);
		
		// disponibilização dos indicadores
		Locale pt_BR = new Locale("pt","BR");
		CurrencyFormatter currencyFormatter = new CurrencyFormatter();
		NumberFormatter numberFormatter = new NumberFormatter();
		List<TreeNodeDTO> itensDoRelatorioParaUmCampus = new ArrayList<TreeNodeDTO>();
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Turmas abertas: <b>" + numberFormatter.print(qtdTurmasAbertas,pt_BR) + "</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Total de Cr&eacute;ditos semanais: <b>"	+ numberFormatter.print(totalCreditosSemanais,pt_BR) + "</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "M&eacute;dia de cr&eacute;ditos por turma: <b>"	+ numberFormatter.print(qtdMediaDeCreditosPorTurma,pt_BR) + "</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "M&eacute;dia de alunos por turma: <b>"	+ numberFormatter.print(qtdMediaDeAlunosPorTurma,pt_BR) + "</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Custo m&eacute;dio do cr&eacute;dito: <b> " + currencyFormatter.print(custoMedioSemanalPorCredito,pt_BR) + "</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Custo docente semestral estimado: <b> "	+ currencyFormatter.print(custoDocenteSemestral,pt_BR) + "</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Receita: <b> " + currencyFormatter.print(receitaSemestral,pt_BR) + "</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Utiliza&ccedil;&atilde;o m&eacute;dia das salas de aula: <b>" + utilizacaoMediaDasSalasDeAula + "%</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Utiliza&ccedil;&atilde;o m&eacute;dia dos laborat&oacute;rios: <b>"	+ utilizacaoMediaDosLaboratorios + "%</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Total de alunos atendidos: <b>"	+ numberFormatter.print(qtdAlunosAtendidos,pt_BR) + "</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Total de alunos n&atilde;o atendidos: <b>" + numberFormatter.print(qtdAlunosNaoAtendidos,pt_BR) + "</b>", currentNode ) );
		
		return itensDoRelatorioParaUmCampus;
	}
}
