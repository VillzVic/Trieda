package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

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
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.DeslocamentoCampus;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Estados;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.TriedaServerUtil;
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
	public PagingLoadResult<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(CampusDTO campusDTO) {
		Campus campus = Campus.find(campusDTO.getId(),this.getInstituicaoEnsinoUser());

		List<HorarioDisponivelCenario> list = new ArrayList<HorarioDisponivelCenario>();
		if (campus != null) {
			List<HorarioDisponivelCenario> listHorarios = campus.getHorarios(this.getInstituicaoEnsinoUser()); 
			if (listHorarios != null) {
				list.addAll(listHorarios);
			}
		}
		
		List<HorarioDisponivelCenarioDTO> listDTO = TriedaServerUtil.ordenaHorariosPorSemanaLetivaETurno(list);

		return new BasePagingLoadResult<HorarioDisponivelCenarioDTO>(listDTO);
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
	public ListLoadResult< CampusDTO > getListAllCampiTodos()
	{
		List< CampusDTO > campiDTO = new ArrayList< CampusDTO >();
		List< Campus > campi = Campus.findAll( this.getInstituicaoEnsinoUser() );

		for ( Campus c : campi )
		{
			campiDTO.add( ConvertBeans.toCampusDTO( c ) );
		}
		
		CampusDTO todos = new CampusDTO();
		todos.setNome("TODOS");
		todos.setCodigo("Todos os campi");
		todos.setId(-1L);
		
		campiDTO.add(todos);

		return new BaseListLoadResult< CampusDTO >( campiDTO );
	}
	
	@Override
	public ListLoadResult<CampusDTO> getCampiNaoSelecionadosParaOtimizacao(List<CampusDTO> campiSelecionados) {
		List<CampusDTO> campiDTOsNaoSelecionados = new ArrayList<CampusDTO>();
		
		Set<Long> campiIDsSelecionados = new HashSet<Long>();
		for (CampusDTO campusDTO : campiSelecionados) {
			campiIDsSelecionados.add(campusDTO.getId());
		}
		
		List<Campus> todosCampi = Campus.findAll(this.getInstituicaoEnsinoUser());
		for (Campus campus : todosCampi) {
			if (!campiIDsSelecionados.contains(campus.getId())) {
				campiDTOsNaoSelecionados.add(ConvertBeans.toCampusDTO(campus));
			}
		}

		return new BaseListLoadResult<CampusDTO>(campiDTOsNaoSelecionados);
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
		Set<SemanaLetiva> semanasLetivasUtilizadas = new HashSet<SemanaLetiva>();
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
					semanasLetivasUtilizadas.add(aula.getOferta().getCurriculo().getSemanaLetiva());
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
					semanasLetivasUtilizadas.add(atendimento.getOferta().getCurriculo().getSemanaLetiva());
				}
			}
		}
		
		Integer totalCreditosSemanais = 0;
		Integer totalCreditosSemanaisProfessores = 0;
		Integer totalCreditosSemanaisProfessoresVirtuais = 0;
		Double custoDocenteSemanal = 0.0;
		Integer qtdAlunosAtendidos = 0;
		Integer qtdAlunosNaoAtendidos = 0;
		double utilizacaoMediaDasSalasDeAula = 0.0;
		double utilizacaoMediaDosLaboratorios = 0.0;
		Double mediaUtilizacaoHorarioSalas = 0.0;
		Double receitaSemestral = 0.0;
		if (!salaIdTurnoIdToAtendimentosMap.isEmpty()) {
			double somatorioDeAlunosDeTodasAsAulasEmSalasDeAula = 0.0;
			double somatorioDeAlunosDeTodasAsAulasEmLaboratorios = 0.0;
			double somatorioDaCapacidadeDasSalasParaTodasAsAulasEmSalasDeAula = 0.0;
			double somatorioDaCapacidadeDosLaboratoriosParaTodasAsAulasEmLaboratorios = 0.0;
			// [SalaId -> Tempo de uso (min) semanal]
			Map<Long,Integer> salaIdToTempoUsoSemanalEmMinutosMap = new HashMap<Long,Integer>();
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
							totalCreditosSemanaisProfessores += (aula.getProfessorId() == null) ? 0 : aula.getTotalCreditos();
							totalCreditosSemanaisProfessoresVirtuais += (aula.getProfessorVirtualId() == null) ? 0 : aula.getTotalCreditos();
							custoDocenteSemanal += aula.getTotalCreditos() * aula.getProfessorCustoCreditoSemanal();
							
							Integer tempoUsoSemanalEmMinutos = salaIdToTempoUsoSemanalEmMinutosMap.get(aula.getSalaId());
							if (tempoUsoSemanalEmMinutos == null) tempoUsoSemanalEmMinutos = 0;
							salaIdToTempoUsoSemanalEmMinutosMap.put(aula.getSalaId(), tempoUsoSemanalEmMinutos + aula.getTotalCreditos()*aula.getSemanaLetivaTempoAula());
						}
					}
				}
			}
			utilizacaoMediaDasSalasDeAula = TriedaUtil.round(somatorioDeAlunosDeTodasAsAulasEmSalasDeAula/somatorioDaCapacidadeDasSalasParaTodasAsAulasEmSalasDeAula*100.0,2);
			utilizacaoMediaDosLaboratorios = TriedaUtil.round(somatorioDeAlunosDeTodasAsAulasEmLaboratorios/somatorioDaCapacidadeDosLaboratoriosParaTodasAsAulasEmLaboratorios*100.0,2);
			
			//calculo do indicador de taxa de uso dos horarios das salas de aula
			SemanaLetiva maiorSemanaLetiva = SemanaLetiva.getSemanaLetivaComMaiorCargaHoraria(semanasLetivasUtilizadas);
			Map<Integer, Integer> countHorariosAula = new HashMap<Integer, Integer>();
			for(HorarioAula ha : maiorSemanaLetiva.getHorariosAula()){
				for(HorarioDisponivelCenario hdc : ha.getHorariosDisponiveisCenario()){
					int semanaInt = Semanas.toInt(hdc.getDiaSemana());
					Integer value = countHorariosAula.get(semanaInt);
					value = ((value == null) ? 0 : value);
					countHorariosAula.put(semanaInt, value + 1);
				}
			}
			int cargaHorariaSemanalEmMinutos = 0;
			for(Integer i : countHorariosAula.keySet()){
				cargaHorariaSemanalEmMinutos += countHorariosAula.get(i) * maiorSemanaLetiva.getTempo();
			}
			
			for(Long salaId : salaIdToTempoUsoSemanalEmMinutosMap.keySet()){
				Integer tempoUsoSalaSemanalEmMinutos = salaIdToTempoUsoSemanalEmMinutosMap.get(salaId);
				mediaUtilizacaoHorarioSalas += ((double)tempoUsoSalaSemanalEmMinutos / cargaHorariaSemanalEmMinutos);
			}
			mediaUtilizacaoHorarioSalas = TriedaUtil.round(mediaUtilizacaoHorarioSalas / salaIdToTempoUsoSemanalEmMinutosMap.size() * 100, 2);		
			
			// cálculo das quantidades de alunos atendidos e não atendidos
			DemandasServiceImpl demandasService = new DemandasServiceImpl();
			ParDTO<Map<Demanda,ParDTO<Integer,Map<Disciplina,Integer>>>,Integer> pair = demandasService.calculaQuantidadeDeNaoAtendimentosPorDemanda(campus.getOfertas());
			qtdAlunosNaoAtendidos = pair.getSegundo();
			qtdAlunosAtendidos = (Demanda.sumDemanda(getInstituicaoEnsinoUser(),campus ) - qtdAlunosNaoAtendidos);
			Map<Demanda,ParDTO<Integer,Map<Disciplina,Integer>>> demandaToQtdAlunosNaoAtendidosMap = pair.getPrimeiro();
	
			// cálculo do indicador de receita semestral
			for (Demanda demanda : demandaToQtdAlunosNaoAtendidosMap.keySet()) {
				ParDTO<Integer,Map<Disciplina,Integer>> par = demandaToQtdAlunosNaoAtendidosMap.get(demanda);
				int qtdAlunosNaoAtendidosDemanda = par.getPrimeiro();
				Map<Disciplina,Integer> disciplinaSubstitutaToQtdAlunos = par.getSegundo();
				
				// calcula receita com uso de equivalências
				int totalAlunosAtendidosComSubstituta = 0;
				for (Entry<Disciplina, Integer> e : disciplinaSubstitutaToQtdAlunos.entrySet()) {
					Disciplina disciplinaSubstituta = e.getKey();
					int qtdAlunosAtendidosComSubstituta = e.getValue();
					receitaSemestral += (disciplinaSubstituta.getCreditosTotal()*qtdAlunosAtendidosComSubstituta*demanda.getOferta().getReceita());
					totalAlunosAtendidosComSubstituta += qtdAlunosAtendidosComSubstituta;
				}
				
				// calcula receita sem uso de equivalências
				int qtdAlunosAtendidosDemanda = (demanda.getQuantidade() - qtdAlunosNaoAtendidosDemanda) - totalAlunosAtendidosComSubstituta;
				receitaSemestral += (demanda.getDisciplina().getCreditosTotal()*qtdAlunosAtendidosDemanda*demanda.getOferta().getReceita());
			}
			receitaSemestral *= ( 4.5 * 6.0 );
			receitaSemestral = TriedaUtil.round(receitaSemestral,2);
		}
		
		// cálculo de outros indicadores
		Integer qtdTurmasAbertas = ehTatico ? AtendimentoTatico.countTurma(getInstituicaoEnsinoUser(),campus) : AtendimentoOperacional.countTurma(getInstituicaoEnsinoUser(),campus);
		Double qtdMediaDeCreditosPorTurma = TriedaUtil.round(((qtdTurmasAbertas==0) ? 0.0 : (totalCreditosSemanais/qtdTurmasAbertas)),2);
		Double qtdMediaDeAlunosPorTurma = TriedaUtil.round(((qtdTurmasAbertas==0) ? 0.0 : (qtdAlunosAtendidos/qtdTurmasAbertas)),2);
		Double custoMedioSemanalPorCredito = TriedaUtil.round(((totalCreditosSemanais != 0) ? custoDocenteSemanal/totalCreditosSemanais : 0.0),2);
		Double custoDocenteSemestral = TriedaUtil.round((custoDocenteSemanal*4.5*6.0),2);
		Double razaoCustoDocentePorReceitaSemestral = (receitaSemestral == 0.0) ? null : TriedaUtil.round((custoDocenteSemestral/receitaSemestral*100.0),2);
		Integer demandaTotalP1QtdeAlunos = AlunoDemanda.sumDemandaPorPrioridade(getInstituicaoEnsinoUser(),campus,1);
		Integer demandaTotalPresencialP1QtdeAlunos = AlunoDemanda.sumDemandaPresencialPorPrioridade(getInstituicaoEnsinoUser(),campus,1);
		Integer demandaTotalNaoPresencialP1QtdeAlunos = demandaTotalP1QtdeAlunos-demandaTotalPresencialP1QtdeAlunos;
		Integer demandaAtendidaP1QtdeAlunos = AlunoDemanda.sumDemandaAtendidaPorPrioridade(getInstituicaoEnsinoUser(),campus,1);
		Integer demandaAtendidaP2QtdeAlunos = AlunoDemanda.sumDemandaAtendidaPorPrioridade(getInstituicaoEnsinoUser(),campus,2);
		Integer demandaAtendidaQtdeAlunos = demandaAtendidaP1QtdeAlunos + demandaAtendidaP2QtdeAlunos + demandaTotalNaoPresencialP1QtdeAlunos;
		Integer demandaNaoAtendidaQtdeAlunos = demandaTotalP1QtdeAlunos - demandaAtendidaQtdeAlunos;
		demandaNaoAtendidaQtdeAlunos = (demandaNaoAtendidaQtdeAlunos < 0) ? 0 : demandaNaoAtendidaQtdeAlunos;
		Double demandaTotalNaoPresencialP1Percent = TriedaUtil.round(((double)demandaTotalNaoPresencialP1QtdeAlunos)/((double)demandaTotalP1QtdeAlunos)*100.0,2);
		Double demandaAtendidaP1Percent = TriedaUtil.round(((double)demandaAtendidaP1QtdeAlunos)/((double)demandaTotalP1QtdeAlunos)*100.0,2);
		Double demandaAtendidaP2Percent = TriedaUtil.round(((double)demandaAtendidaP2QtdeAlunos)/((double)demandaTotalP1QtdeAlunos)*100.0,2);
		Double demandaAtendidaPercent = TriedaUtil.round(((double)demandaAtendidaQtdeAlunos)/((double)demandaTotalP1QtdeAlunos)*100.0,2);
		Double demandaNaoAtendidaPercent = TriedaUtil.round(((double)demandaNaoAtendidaQtdeAlunos)/((double)demandaTotalP1QtdeAlunos)*100.0,2);
		
		Integer qtdProfessores = ( campus.isOtimizadoOperacional(getInstituicaoEnsinoUser()) ?
				AtendimentoOperacional.countProfessores(getInstituicaoEnsinoUser(), campus) : 0);
		Integer qtdProfessoresVirtuais = ( campus.isOtimizadoOperacional(getInstituicaoEnsinoUser()) ?
				AtendimentoOperacional.countProfessoresVirtuais(getInstituicaoEnsinoUser(), campus) : 0);
		Integer qtdDocentes = qtdProfessores + qtdProfessoresVirtuais;
		
		Double mediaCreditosPorDocente = TriedaUtil.round( qtdDocentes == 0 ? 0.0 : ((double)totalCreditosSemanais)/qtdDocentes, 2 );
		Double mediaCreditosProfessores = TriedaUtil.round( qtdProfessores == 0 ? 0.0 : ((double)totalCreditosSemanaisProfessores)/qtdProfessores, 2 );
		Double mediaCreditosProfessoresVirtuais = TriedaUtil.round( qtdProfessoresVirtuais == 0 ? 0.0 : ((double)totalCreditosSemanaisProfessoresVirtuais)/qtdProfessoresVirtuais, 2 );
		
		// disponibilização dos indicadores
		Locale pt_BR = new Locale("pt","BR");
		CurrencyFormatter currencyFormatter = new CurrencyFormatter();
		NumberFormatter numberFormatter = new NumberFormatter();
		List<TreeNodeDTO> itensDoRelatorioParaUmCampus = new ArrayList<TreeNodeDTO>();
		
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Demanda Total (Qtde Alunos): <b>" + numberFormatter.print(demandaTotalP1QtdeAlunos,pt_BR) + "</b>",currentNode) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "|--- Presencial: <b>" + numberFormatter.print(demandaTotalPresencialP1QtdeAlunos,pt_BR) + "</b>",currentNode) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "|--- N&atilde;o Presencial: <b>" + numberFormatter.print(demandaTotalNaoPresencialP1QtdeAlunos,pt_BR) + "</b>",currentNode) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Demanda Atendida (Qtde Alunos): <b>" + numberFormatter.print(demandaAtendidaQtdeAlunos,pt_BR) + "</b>  <b>(" + demandaAtendidaPercent + "%)</b>",currentNode) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "|--- Presencial P1: <b>" + numberFormatter.print(demandaAtendidaP1QtdeAlunos,pt_BR) + "</b>  <b>(" + demandaAtendidaP1Percent + "%)</b>",currentNode) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "|--- Presencial P2: <b>" + numberFormatter.print(demandaAtendidaP2QtdeAlunos,pt_BR) + "</b>  <b>(" + demandaAtendidaP2Percent + "%)</b>",currentNode) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "|--- N&atilde;o Presencial: <b>" + numberFormatter.print(demandaTotalNaoPresencialP1QtdeAlunos,pt_BR) + "</b>  <b>(" + demandaTotalNaoPresencialP1Percent + "%)</b>",currentNode) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Demanda N&atilde;o Atendida (Qtde Alunos): <b>" + numberFormatter.print(demandaNaoAtendidaQtdeAlunos,pt_BR) + "</b>  <b>(" + demandaNaoAtendidaPercent + "%)</b>",currentNode) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Docentes Utilizados: <b>" + numberFormatter.print(qtdDocentes,pt_BR) + "</b>",currentNode) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "|--- Da Instituição: <b>" + numberFormatter.print(qtdProfessores,pt_BR) + "</b>",currentNode) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "|--- Gerados pelo Trieda: <b>" + numberFormatter.print(qtdProfessoresVirtuais,pt_BR) + "</b>",currentNode) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Média de Créditos Semanais por Docente: <b>" + numberFormatter.print(mediaCreditosPorDocente,pt_BR) + "</b>",currentNode) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "|--- Da Instituição: <b>" + numberFormatter.print(mediaCreditosProfessores,pt_BR) + " </b>",currentNode) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "|--- Gerados pelo Trieda: <b>" + numberFormatter.print(mediaCreditosProfessoresVirtuais,pt_BR) + " </b>",currentNode) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Turmas Abertas: <b>" + numberFormatter.print(qtdTurmasAbertas,pt_BR) + "</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Total de Cr&eacute;ditos Semanais: <b>"	+ numberFormatter.print(totalCreditosSemanais,pt_BR) + "</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "M&eacute;dia de Cr&eacute;ditos por Turma: <b>"	+ numberFormatter.print(qtdMediaDeCreditosPorTurma,pt_BR) + "</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "M&eacute;dia de Alunos por Turma: <b>"	+ numberFormatter.print(qtdMediaDeAlunosPorTurma,pt_BR) + "</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Custo M&eacute;dio do Cr&eacute;dito: <b> " + currencyFormatter.print(custoMedioSemanalPorCredito,pt_BR) + "</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Custo Docente Semestral Estimado: <b> "	+ currencyFormatter.print(custoDocenteSemestral,pt_BR) + "</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Receita: <b> " + currencyFormatter.print(receitaSemestral,pt_BR) + "</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Margem: <b> " + currencyFormatter.print(receitaSemestral-custoDocenteSemestral,pt_BR) + "</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "% do Custo Docente sobre a Receita: <b> " + ((razaoCustoDocentePorReceitaSemestral == null) ? "n.d.a." : razaoCustoDocentePorReceitaSemestral) + "%</b>", currentNode));
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Utiliza&ccedil;&atilde;o M&eacute;dia das Salas de Aula: <b>" + utilizacaoMediaDasSalasDeAula + "%</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Utiliza&ccedil;&atilde;o M&eacute;dia dos Laborat&oacute;rios: <b>"	+ utilizacaoMediaDosLaboratorios + "%</b>", currentNode ) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Utiliza&ccedil;&atilde;o M&eacute;dia dos Hor&aacute;rios das Salas de Aula: <b>" + mediaUtilizacaoHorarioSalas + "%</b>", currentNode ) );
		//itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Total de alunos atendidos: <b>"	+ numberFormatter.print(qtdAlunosAtendidos,pt_BR) + "</b>", currentNode ) );
		//itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( "Total de alunos n&atilde;o atendidos: <b>" + numberFormatter.print(qtdAlunosNaoAtendidos,pt_BR) + "</b>", currentNode ) );
		
		return itensDoRelatorioParaUmCampus;
	}
}
