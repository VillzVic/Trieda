package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

import org.springframework.format.number.NumberFormatter;
import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.BaseTreeModel;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Aluno;
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
import com.gapso.trieda.domain.Professor;
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
import com.gapso.web.trieda.shared.dtos.ResumoDTO;
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
		CenarioDTO cenarioDTO, CurriculoDTO curriculoDTO )
	{
		Curriculo curriculo = Curriculo.find(
			curriculoDTO.getId(), getInstituicaoEnsinoUser() );
		
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());

		List< CampusDTO > campiDTO = new ArrayList< CampusDTO >();
		List< Campus > campi = Campus.findByCurriculo( cenario,
			this.getInstituicaoEnsinoUser(), curriculo );

		for ( Campus c : campi )
		{
			campiDTO.add( ConvertBeans.toCampusDTO( c ) );
		}
		return new BaseListLoadResult< CampusDTO >( campiDTO );
	}

	@Override
	public ListLoadResult< CampusDTO > getListAll( CenarioDTO cenarioDTO )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< CampusDTO > campiDTO = new ArrayList< CampusDTO >();
		List< Campus > campi = Campus.findByCenario( this.getInstituicaoEnsinoUser(), cenario );

		for ( Campus c : campi )
		{
			campiDTO.add( ConvertBeans.toCampusDTO( c ) );
		}

		return new BaseListLoadResult< CampusDTO >( campiDTO );
	}
	
	@Override
	public ListLoadResult< CampusDTO > getListAllCampiTodos( CenarioDTO cenarioDTO )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< CampusDTO > campiDTO = new ArrayList< CampusDTO >();
		List< Campus > campi = Campus.findByCenario(getInstituicaoEnsinoUser(), cenario);

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
	public ListLoadResult<CampusDTO> getCampiNaoSelecionadosParaOtimizacao(CenarioDTO cenarioDTO, List<CampusDTO> campiSelecionados) {
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		List<CampusDTO> campiDTOsNaoSelecionados = new ArrayList<CampusDTO>();
		
		Set<Long> campiIDsSelecionados = new HashSet<Long>();
		for (CampusDTO campusDTO : campiSelecionados) {
			campiIDsSelecionados.add(campusDTO.getId());
		}
		
		List<Campus> todosCampi = Campus.findByCenario(this.getInstituicaoEnsinoUser(), cenario);
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
			null, null, null,null, null, null, null, loadConfig);
	}

	@Override
	public PagingLoadResult< CampusDTO > getBuscaList( CenarioDTO cenarioDTO,
		String nome, String codigo, String estadoString, String municipio,	String bairro,
		String operadorCustoMedioCredito, Double custoMedioCredito, Boolean otimizadoOperacional, Boolean otimizadoTatico,
		PagingLoadConfig config )
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
			municipio, bairro,
			operadorCustoMedioCredito, custoMedioCredito, otimizadoOperacional, otimizadoTatico,
			config.getOffset(), config.getLimit(), orderBy );

		for ( Campus campus : campi )
		{
			list.add( ConvertBeans.toCampusDTO( campus ) );
		}

		BasePagingLoadResult< CampusDTO > result
			= new BasePagingLoadResult< CampusDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Campus.count(
			this.getInstituicaoEnsinoUser(), cenario,
			nome, codigo, estadoDomain, municipio, bairro,
			operadorCustoMedioCredito, custoMedioCredito, otimizadoOperacional, otimizadoTatico) );

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
	public List<DeslocamentoCampusDTO> getDeslocamentos( CenarioDTO cenarioDTO )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(),this.getInstituicaoEnsinoUser());
		
		List< DeslocamentoCampusDTO > list
			= new ArrayList< DeslocamentoCampusDTO >();

		List< Campus > listCampi
			= Campus.findByCenario( this.getInstituicaoEnsinoUser(), cenario );

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
		Cenario cenario = Cenario.find(cenarioDTO.getId(),this.getInstituicaoEnsinoUser());

		if (currentNode == null){
			// disponibiliza uma pasta para cada campus no relatório de resumo por campi
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
			list = getResumoParaCampus(campus, cenario, currentNode);
		}

		return list;
	}

	private List<TreeNodeDTO> getResumoParaCampus(Campus campus, Cenario cenario, TreeNodeDTO currentNode) {
		boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
		
		// cálculo dos indicadores de utilização das salas de aula e laboratórios
		
		long primeiro = System.currentTimeMillis(); 
		Set<Turno> turnosConsiderados = new HashSet<Turno>();
		Set<Sala> salasUtilizadas = new HashSet<Sala>();
		Set<SemanaLetiva> semanasLetivasUtilizadas = new HashSet<SemanaLetiva>();
		Map<String,List<AtendimentoRelatorioDTO>> salaIdTurnoIdToAtendimentosMap = new HashMap<String,List<AtendimentoRelatorioDTO>>();
		String statusOtimizacao = "";
		if (!campus.isOtimizado(getInstituicaoEnsinoUser()))
		{
			statusOtimizacao = "Não Otimizado";
		}
		else if (ehTatico)
		{
			statusOtimizacao = "Tático";
		}
		else
		{
			statusOtimizacao = "Operacional";
		}
		
		if (ehTatico) {
			// atendimentos táticos
			List<AtendimentoTatico> atendimentosTaticos = AtendimentoTatico.getAtendimentosTaticosByCampus(getInstituicaoEnsinoUser(), campus);
			for (AtendimentoTatico atendimento : atendimentosTaticos) {
				Oferta oferta = atendimento.getOferta();
				turnosConsiderados.add(oferta.getTurno());
				String key = atendimento.getSala().getId() + "-" + oferta.getTurno().getId();
				List<AtendimentoRelatorioDTO> aulasPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
				if (aulasPorSalaTurno == null) {
					aulasPorSalaTurno = new ArrayList<AtendimentoRelatorioDTO>();
					salaIdTurnoIdToAtendimentosMap.put(key,aulasPorSalaTurno);
				}
				aulasPorSalaTurno.add(ConvertBeans.toAtendimentoTaticoDTO(atendimento));
				
				salasUtilizadas.add(atendimento.getSala());
				semanasLetivasUtilizadas.add(oferta.getCurriculo().getSemanaLetiva());
			}
		} else {
			System.out.print("Checando atendimentos operacionais");long start = System.currentTimeMillis(); // TODO: retirar
			// atendimentos operacionais
			List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.getAtendimentosOperacionaisByCampus(getInstituicaoEnsinoUser(), campus);
			 long time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
			
			 System.out.print("Checando ConvertBeans.toAtendimentoOperacionalDTO(atendimento)"); start = System.currentTimeMillis(); // TODO: retirar
			for (AtendimentoOperacional atendimento : atendimentos) {
				Oferta oferta = atendimento.getOferta();
				Turno turno = oferta.getTurno();
				Sala sala = atendimento.getSala();
				turnosConsiderados.add(turno);
				String key = sala.getId() + "-" + turno.getId();
				List<AtendimentoRelatorioDTO> atendimetosPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
				if (atendimetosPorSalaTurno == null) {
					atendimetosPorSalaTurno = new ArrayList<AtendimentoRelatorioDTO>();
					salaIdTurnoIdToAtendimentosMap.put(key,atendimetosPorSalaTurno);
				}
				atendimetosPorSalaTurno.add(ConvertBeans.toAtendimentoOperacionalDTO(atendimento));
				
				salasUtilizadas.add(sala);
				semanasLetivasUtilizadas.add(oferta.getCurriculo().getSemanaLetiva());
			}
			time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
		}
		
		
		Integer totalCreditosSemanais = 0;
		Integer totalCreditosSemanaisProfessores = 0;
		Integer totalCreditosSemanaisProfessoresVirtuais = 0;
		Double custoDocenteSemanal = 0.0;
		//Integer qtdAlunosAtendidos = 0;
		//Integer qtdAlunosNaoAtendidos = 0;
		double utilizacaoMediaDosAmbientes = 0.0;
		double utilizacaoMediaDasSalasDeAula = 0.0;
		double utilizacaoMediaDosLaboratorios = 0.0;
		Double mediaUtilizacaoHorarioAmbientes = 0.0;
		Double mediaUtilizacaoHorarioSalas = 0.0;
		Double mediaUtilizacaoHorarioLaboratorios = 0.0;
		Double receitaSemestral = 0.0;
		if (!salaIdTurnoIdToAtendimentosMap.isEmpty()) {
			double somatorioDeAlunosDeTodasAsAulasEmAmbientes = 0.0;
			double somatorioDeAlunosDeTodasAsAulasEmSalasDeAula = 0.0;
			double somatorioDeAlunosDeTodasAsAulasEmLaboratorios = 0.0;
			double somatorioDaCapacidadeDasSalasParaTodasAsAulasEmAmbientes = 0.0;
			double somatorioDaCapacidadeDasSalasParaTodasAsAulasEmSalasDeAula = 0.0;
			double somatorioDaCapacidadeDosLaboratoriosParaTodasAsAulasEmLaboratorios = 0.0;
			
			// [SalaId -> Tempo de uso (min) semanal]
			Map<Long,Integer> salaIdToTempoUsoSemanalEmMinutosMap = new HashMap<Long,Integer>();
			Map<Long,Integer> laboratorioIdToTempoUsoSemanalEmMinutosMap = new HashMap<Long,Integer>();
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
							somatorioDeAlunosDeTodasAsAulasEmAmbientes += aula.getQuantidadeAlunos() * aula.getTotalCreditos();
							somatorioDaCapacidadeDasSalasParaTodasAsAulasEmAmbientes += sala.getCapacidadeInstalada() * aula.getTotalCreditos();
							if (!sala.isLaboratorio()) {
								somatorioDeAlunosDeTodasAsAulasEmSalasDeAula += aula.getQuantidadeAlunos() * aula.getTotalCreditos();
								somatorioDaCapacidadeDasSalasParaTodasAsAulasEmSalasDeAula += sala.getCapacidadeInstalada() * aula.getTotalCreditos();
							} else {
								somatorioDeAlunosDeTodasAsAulasEmLaboratorios += aula.getQuantidadeAlunos() * aula.getTotalCreditos();
								somatorioDaCapacidadeDosLaboratoriosParaTodasAsAulasEmLaboratorios += sala.getCapacidadeInstalada() * aula.getTotalCreditos();
							}
							totalCreditosSemanais += aula.getTotalCreditos();
							totalCreditosSemanaisProfessores += (aula.getProfessorId() == null) ? 0 : aula.getTotalCreditos();
							totalCreditosSemanaisProfessoresVirtuais += (aula.getProfessorVirtualId() == null) ? 0 : aula.getTotalCreditos();
							custoDocenteSemanal += aula.getTotalCreditos() * aula.getProfessorCustoCreditoSemanal();

							if(!sala.isLaboratorio()){
								Integer tempoUsoSemanalEmMinutos = salaIdToTempoUsoSemanalEmMinutosMap.get(aula.getSalaId());
								if (tempoUsoSemanalEmMinutos == null) tempoUsoSemanalEmMinutos = 0;
								salaIdToTempoUsoSemanalEmMinutosMap.put(aula.getSalaId(), tempoUsoSemanalEmMinutos + aula.getTotalCreditos()*aula.getSemanaLetivaTempoAula());
							}
							else {
								Integer tempoUsoSemanalEmMinutos = laboratorioIdToTempoUsoSemanalEmMinutosMap.get(aula.getSalaId());
								if (tempoUsoSemanalEmMinutos == null) tempoUsoSemanalEmMinutos = 0;
								laboratorioIdToTempoUsoSemanalEmMinutosMap.put(aula.getSalaId(), tempoUsoSemanalEmMinutos + aula.getTotalCreditos()*aula.getSemanaLetivaTempoAula());
							}
						}
					}
				}
			}
			utilizacaoMediaDosAmbientes = TriedaUtil.round(somatorioDeAlunosDeTodasAsAulasEmAmbientes/somatorioDaCapacidadeDasSalasParaTodasAsAulasEmAmbientes*100.0,2);
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
				mediaUtilizacaoHorarioAmbientes += ((double)tempoUsoSalaSemanalEmMinutos / cargaHorariaSemanalEmMinutos);
			}
			mediaUtilizacaoHorarioSalas = TriedaUtil.round(mediaUtilizacaoHorarioSalas / salaIdToTempoUsoSemanalEmMinutosMap.size() * 100, 2);
			for(Long salaId : laboratorioIdToTempoUsoSemanalEmMinutosMap.keySet()){
				Integer tempoUsoSalaSemanalEmMinutos = laboratorioIdToTempoUsoSemanalEmMinutosMap.get(salaId);
				mediaUtilizacaoHorarioLaboratorios += ((double)tempoUsoSalaSemanalEmMinutos / cargaHorariaSemanalEmMinutos);
				mediaUtilizacaoHorarioAmbientes += ((double)tempoUsoSalaSemanalEmMinutos / cargaHorariaSemanalEmMinutos);
			}
			mediaUtilizacaoHorarioLaboratorios = TriedaUtil.round(mediaUtilizacaoHorarioLaboratorios / laboratorioIdToTempoUsoSemanalEmMinutosMap.size() * 100, 2);
			mediaUtilizacaoHorarioAmbientes = TriedaUtil.round(mediaUtilizacaoHorarioAmbientes / (laboratorioIdToTempoUsoSemanalEmMinutosMap.size()+salaIdToTempoUsoSemanalEmMinutosMap.size()) * 100, 2);
			
			// cálculo das quantidades de alunos atendidos e não atendidos
			//DemandasServiceImpl demandasService = new DemandasServiceImpl();
			//ParDTO<Map<Demanda,ParDTO<Integer,Map<Disciplina,Integer>>>,Integer> pair = demandasService.calculaQuantidadeDeNaoAtendimentosPorDemanda(campus.getOfertas());
			//qtdAlunosNaoAtendidos = pair.getSegundo();
			//qtdAlunosAtendidos = (Demanda.sumDemanda(getInstituicaoEnsinoUser(),campus ) - qtdAlunosNaoAtendidos);
			
			receitaSemestral = (ehTatico) ? AtendimentoTatico.calcReceita(getInstituicaoEnsinoUser(),campus) : AtendimentoOperacional.calcReceita(getInstituicaoEnsinoUser(),campus);
			receitaSemestral = TriedaUtil.round(receitaSemestral,2);
		}
		
		// cálculo de outros indicadores
		long startIndic = System.currentTimeMillis(); // TODO: retirar
		System.out.print("Checando Aluno.findByCampus");long start = System.currentTimeMillis(); // TODO: retirar
		Integer qtdTotalAlunos = Aluno.findByCampus(getInstituicaoEnsinoUser(), campus, ehTatico).size();
		long time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
		
		System.out.print("Checando numAlunosComDemanda"); start = System.currentTimeMillis(); // TODO: retirar
		int numAlunosComDemanda = AlunoDemanda.sumAlunosComDemanda(getInstituicaoEnsinoUser(), campus);
		time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
		
		System.out.print("Checando Professor.findByCampus"); start = System.currentTimeMillis(); // TODO: retirar
		Integer qtdTotalDocentes = Professor.findByCampus(getInstituicaoEnsinoUser(), cenario, campus).size();
		time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
		
		System.out.print("Checando Professor.findProfessoresUteis"); start = System.currentTimeMillis(); // TODO: retirar
		Integer qtdDocentesHabilitados = Professor.findProfessoresUteis(getInstituicaoEnsinoUser(), cenario, campus).size();
		time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
		
		System.out.print("Checando AtendimentoOperacional.countTurmaProfessoresVirtuais"); start = System.currentTimeMillis(); // TODO: retirar
		Integer qtdTurmasAbertasProfessoresVirtuais = ehTatico ? 0 : AtendimentoOperacional.countTurmaProfessoresVirtuais(getInstituicaoEnsinoUser(),campus);
		time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
		
		System.out.print("Checando AtendimentoOperacional.countTurmaProfessoresInstituicao"); start = System.currentTimeMillis(); // TODO: retirar
		Integer qtdTurmasAbertasProfessoresInstituicao = ehTatico ? 0 : AtendimentoOperacional.countTurmaProfessoresInstituicao(getInstituicaoEnsinoUser(),campus);
		time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
		
		System.out.print("Checando qtdTurmasAbertas"); start = System.currentTimeMillis(); // TODO: retirar
		Integer qtdTurmasAbertas = ehTatico ? AtendimentoTatico.countTurma(getInstituicaoEnsinoUser(),campus) : AtendimentoOperacional.countTurma(getInstituicaoEnsinoUser(),campus);
		time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
		
		Double qtdMediaDeCreditosPorTurma = TriedaUtil.round(((qtdTurmasAbertas==0) ? 0.0 : ((double)totalCreditosSemanais/qtdTurmasAbertas)),2);
		
		System.out.print("Checando  AlunoDemanda.sumDemandaPorPrioridade"); start = System.currentTimeMillis(); // TODO: retirar
		Integer demandaTotalP1QtdeAlunos = AlunoDemanda.sumDemandaPorPrioridade(getInstituicaoEnsinoUser(),campus,1);
		time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
		
		System.out.print("Checando AlunoDemanda.sumDemandaPresencialPorPrioridade"); start = System.currentTimeMillis(); // TODO: retirar
		Integer demandaTotalPresencialP1QtdeAlunos = AlunoDemanda.sumDemandaPresencialPorPrioridade(getInstituicaoEnsinoUser(),cenario,campus,1);
		time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
		
		Integer demandaTotalNaoPresencialP1QtdeAlunos = demandaTotalP1QtdeAlunos-demandaTotalPresencialP1QtdeAlunos;
		
		System.out.print("Checando AlunoDemanda.sumDemandaAtendidaPorPrioridade"); start = System.currentTimeMillis(); // TODO: retirar
		Integer demandaAtendidaP1QtdeAlunos = AlunoDemanda.sumDemandaAtendidaPorPrioridade(getInstituicaoEnsinoUser(),campus,1);
		time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
		
		System.out.print("Checando AlunoDemanda.sumDemandaAtendidaPorPrioridade"); start = System.currentTimeMillis(); // TODO: retirar
		Integer demandaAtendidaP2QtdeAlunos = AlunoDemanda.sumDemandaAtendidaPorPrioridade(getInstituicaoEnsinoUser(),campus,2);
		time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
		
		Integer demandaAtendidaQtdeAlunos = demandaAtendidaP1QtdeAlunos + demandaAtendidaP2QtdeAlunos + demandaTotalNaoPresencialP1QtdeAlunos;
		Double qtdMediaDeAlunosPorTurma = TriedaUtil.round(((qtdTurmasAbertas==0) ? 0.0 : ((double)demandaAtendidaQtdeAlunos/qtdTurmasAbertas)),2);
		Double custoMedioSemanalPorCredito = TriedaUtil.round(((totalCreditosSemanais != 0) ? custoDocenteSemanal/totalCreditosSemanais : 0.0),2);
		Double custoDocenteSemestral = TriedaUtil.round((custoDocenteSemanal*4.5*6.0),2);
		
		System.out.print("Checando  totalCreditosSemanaisAlunos"); start = System.currentTimeMillis(); // TODO: retirar
		Double margemContribuicaoSemestral = TriedaUtil.round(receitaSemestral - custoDocenteSemestral, 2);
		Double razaoCustoDocentePorReceitaSemestral = (receitaSemestral == 0.0) ? null : TriedaUtil.round((custoDocenteSemestral/receitaSemestral*100.0),2);
		Integer demandaNaoAtendidaQtdeAlunos = demandaTotalP1QtdeAlunos - demandaAtendidaQtdeAlunos;
		demandaNaoAtendidaQtdeAlunos = (demandaNaoAtendidaQtdeAlunos < 0) ? 0 : demandaNaoAtendidaQtdeAlunos;
		Double atendimentoPercent = TriedaUtil.round( qtdTotalAlunos == 0 ? 0.0 : (((double)qtdTotalAlunos)/qtdTotalAlunos)*100, 2 );
		
		Integer totalCreditosSemanaisAlunos = ehTatico ? AtendimentoTatico.sumCredAlunosAtendidos(getInstituicaoEnsinoUser(),cenario,campus) :
			AtendimentoOperacional.sumCredAlunosAtendidos(getInstituicaoEnsinoUser(),cenario,campus);
		
		time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
		
		System.out.print("Checando AtendimentoOperacional.countProfessores"); start = System.currentTimeMillis(); // TODO: retirar
		Integer qtdProfessores = ( campus.isOtimizadoOperacional(getInstituicaoEnsinoUser()) ?
				AtendimentoOperacional.countProfessores(getInstituicaoEnsinoUser(), campus) : 0);
		time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
		
		System.out.print("Checando AtendimentoOperacional.countProfessoresVirtuais"); start = System.currentTimeMillis(); // TODO: retirar
		
		Integer qtdProfessoresVirtuais = ( campus.isOtimizadoOperacional(getInstituicaoEnsinoUser()) ?
				AtendimentoOperacional.countProfessoresVirtuais(getInstituicaoEnsinoUser(), campus) : 0);
		time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
		
		System.out.print("Checando somas finais"); start = System.currentTimeMillis(); // TODO: retirar
		
		Integer qtdDocentes = qtdProfessores + qtdProfessoresVirtuais;
		
		Double produtividadeTempoAula = TriedaUtil.round( totalCreditosSemanais == 0 ? 0.0 : ((double)totalCreditosSemanaisAlunos)/totalCreditosSemanais, 2 );
		Double custoSobreReceitaCreditoPercent = TriedaUtil.round( totalCreditosSemanaisAlunos == 0 ? 0.0 : (((double)totalCreditosSemanais)/totalCreditosSemanaisAlunos)*100, 2 );
		Double mediaCreditosPorDocente = TriedaUtil.round( qtdDocentes == 0 ? 0.0 : ((double)totalCreditosSemanais)/qtdDocentes, 2 );
		Double mediaCreditosProfessores = TriedaUtil.round( qtdProfessores == 0 ? 0.0 : ((double)totalCreditosSemanaisProfessores)/qtdProfessores, 2 );
		Double mediaCreditosProfessoresVirtuais = TriedaUtil.round( qtdProfessoresVirtuais == 0 ? 0.0 : ((double)totalCreditosSemanaisProfessoresVirtuais)/qtdProfessoresVirtuais, 2 );
		Double mediaTurmasPorDocente = TriedaUtil.round( qtdDocentes == 0 ? 0.0 : ((double)qtdTurmasAbertas)/qtdDocentes, 2 );
		Double mediaTurmasPorProfessor = TriedaUtil.round( qtdProfessores == 0 ? 0.0 : ((double)qtdTurmasAbertasProfessoresInstituicao)/qtdProfessores, 2 );
		Double mediaTurmasPorProfessorVirtual = TriedaUtil.round( qtdProfessoresVirtuais == 0 ? 0.0 : ((double)qtdTurmasAbertasProfessoresVirtuais)/qtdProfessoresVirtuais, 2 );
		
		time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
		time = (System.currentTimeMillis() - startIndic)/1000;System.out.println("Checando indicadores tempo = " + time + " segundos"); // TODO: retirar
		
		// disponibilização dos indicadores
		Locale pt_BR = new Locale("pt","BR");
		NumberFormatter numberFormatter = new NumberFormatter();
		List<TreeNodeDTO> itensDoRelatorioParaUmCampus = new ArrayList<TreeNodeDTO>();
		
		//Gerais
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Status de Otimização: ", "<b>" + statusOtimizacao) ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Número de alunos: ", "<b>" + numberFormatter.print(numAlunosComDemanda,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Alunos atendidos (pelo menos 1 disc.): ", "<b>" + numberFormatter.print(qtdTotalAlunos,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Atendimento % (pelo menos 1 disc.): ", "<b>" + numberFormatter.print(atendimentoPercent,pt_BR) + "%</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Demanda total (Qtde alunos): ", "<b>" + numberFormatter.print(demandaTotalP1QtdeAlunos,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"|--- Presencial: ", "<b>" + numberFormatter.print(demandaTotalPresencialP1QtdeAlunos,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"|--- Não presencial: ", "<b>" + numberFormatter.print(demandaTotalNaoPresencialP1QtdeAlunos,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Demanda atendida (matrículas): ", "<b>" + numberFormatter.print(demandaAtendidaQtdeAlunos,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"|--- Presencial P1: ", "<b>" + numberFormatter.print(demandaAtendidaP1QtdeAlunos,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"|--- Presencial P2: ", "<b>" + numberFormatter.print(demandaAtendidaP2QtdeAlunos,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Créditos pagos aos docentes (A): ", "<b>" + numberFormatter.print(totalCreditosSemanais,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"|--- Créditos de professores da instituição: ", "<b>" + numberFormatter.print(totalCreditosSemanaisProfessores,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"|--- Crédito de professores virtuais: ", "<b>" + numberFormatter.print(totalCreditosSemanaisProfessoresVirtuais,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Créditos pagos pelos alunos (B): ", "<b>" + numberFormatter.print(totalCreditosSemanaisAlunos,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Turmas abertas: ", "<b>" + numberFormatter.print(qtdTurmasAbertas,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"|--- Docentes da instituição: ", "<b>" + numberFormatter.print(qtdTurmasAbertasProfessoresInstituicao,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"|--- Docentes virtuais: ", "<b>" + numberFormatter.print(qtdTurmasAbertasProfessoresVirtuais,pt_BR) + "</b>") ,currentNode, true) );
		
		//Eficiencia de alocacao
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Produtividade do tempo de aula pago (B/A): ", "<b>" + numberFormatter.print(produtividadeTempoAula,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Custo/ receita do crédito (A/B): ", "<b>" + numberFormatter.print(custoSobreReceitaCreditoPercent,pt_BR) + "%</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Média de alunos por turma: ", "<b>" + numberFormatter.print(qtdMediaDeAlunosPorTurma,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Média de créditos por turma: ", "<b>" + numberFormatter.print(qtdMediaDeCreditosPorTurma,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Ocupação geral da capacidade de ambientes: ", "<b>" + numberFormatter.print(utilizacaoMediaDosAmbientes,pt_BR) + "%</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Utilização geral de horários de ambientes: ", "<b>" + numberFormatter.print(mediaUtilizacaoHorarioAmbientes,pt_BR) + "%</b>") ,currentNode, true) );
		
		//Financeiro
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Receita semestral estimada: ", "<b>" + numberFormatter.print(receitaSemestral,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Custo docente semestral estimado: ", "<b>" + numberFormatter.print(custoDocenteSemestral,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Margem de contribuição semestral estimada: ", "<b>" + numberFormatter.print(margemContribuicaoSemestral,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"% do custo docente sobre a receita: ", "<b>" + ((razaoCustoDocentePorReceitaSemestral == null) ? "n.d.a." : numberFormatter.print(razaoCustoDocentePorReceitaSemestral, pt_BR)) + "%</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Ticket médio: ", "<b>" + "-" + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Custo médio do crédito: ", "<b>" + numberFormatter.print(custoMedioSemanalPorCredito,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Custo médio docente (fornecido): ", "<b>" + numberFormatter.print(campus.getValorCredito(), pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Custo médio docente (real alocado): ", "<b>" + numberFormatter.print(custoMedioSemanalPorCredito,pt_BR) + "</b>") ,currentNode, true) );
		
		//Uso dos Ambientes
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Ocupação média da capacidade dos ambientes: ", "<b>" + numberFormatter.print(utilizacaoMediaDosAmbientes,pt_BR) + "%</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"|--- Ocupação média da capacidade das salas de aula:  ", "<b>" + numberFormatter.print(utilizacaoMediaDasSalasDeAula,pt_BR) + "%</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"|--- Ocupação média da capacidade dos laboratórios: ", "<b>" + numberFormatter.print(utilizacaoMediaDosLaboratorios,pt_BR) + "%</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Utilização média dos horários dos ambientes: ", "<b>" + numberFormatter.print(mediaUtilizacaoHorarioAmbientes,pt_BR) + "%</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"|--- Utilização média dos horários das salas de aula: ", "<b>" + numberFormatter.print(mediaUtilizacaoHorarioSalas,pt_BR) + "%</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"|--- Utilização média dos horários dos laboratorios: ", "<b>" + numberFormatter.print(mediaUtilizacaoHorarioLaboratorios,pt_BR) + "%</b>") ,currentNode, true) );
		
		//Uso dos Docentes
		//|--- Total
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Docentes cadastrados:  ", "<b>" + numberFormatter.print(qtdTotalDocentes,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Docentes habilitados e disponíveis: ", "<b>" + numberFormatter.print(qtdDocentesHabilitados,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Docentes utilizados (todos): ", "<b>" + numberFormatter.print(qtdDocentes,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Média de créditos semanais por docente: ", "<b>" + numberFormatter.print(mediaCreditosPorDocente,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Média de turmas por docente: ", "<b>" + numberFormatter.print(mediaTurmasPorDocente,pt_BR) + "</b>") ,currentNode, true) );
		
		//|--- Da Instituicao
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Docentes usados da instituição:  ", "<b>" + numberFormatter.print(qtdProfessores,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Créditos ministrados por docentes da instituição: ", "<b>" + numberFormatter.print(totalCreditosSemanaisProfessores,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Média de créditos semanais por docente da instituição: ", "<b>" + numberFormatter.print(mediaCreditosProfessores,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Turmas ministradas por docentes da instituição : ", "<b>" + numberFormatter.print(qtdTurmasAbertasProfessoresInstituicao,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Média de turmas por docente da instituição: ", "<b>" + numberFormatter.print(mediaTurmasPorProfessor,pt_BR) + "</b>") ,currentNode, true) );
		
		//|--- Virtuais
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Docentes usados - virtuais:  ", "<b>" + numberFormatter.print(qtdProfessoresVirtuais,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Créditos ministrados por docentes virtuais: ", "<b>" + numberFormatter.print(totalCreditosSemanaisProfessoresVirtuais,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Média de créditos semanais por docente virtual: ", "<b>" + numberFormatter.print(mediaCreditosProfessoresVirtuais,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Turmas ministradas por docentes virtuais: ", "<b>" + numberFormatter.print(qtdTurmasAbertasProfessoresVirtuais,pt_BR) + "</b>") ,currentNode, true) );
		itensDoRelatorioParaUmCampus.add( new TreeNodeDTO( new ResumoDTO(
				"Média de turmas por docente virtual: ", "<b>" + numberFormatter.print(mediaTurmasPorProfessorVirtual,pt_BR) + "</b>") ,currentNode, true) );
		
		time = (System.currentTimeMillis() - primeiro)/1000;System.out.println(" tempo total = " + time + " segundos"); // TODO: retirar
		return itensDoRelatorioParaUmCampus;
	}
	
	@Override
	public List<BaseTreeModel> getCenariosComparados(List<CenarioDTO> cenariosDTO)
	{
		List<BaseTreeModel> result = new ArrayList<BaseTreeModel>();
		
		Map<String, Set<Cenario>> campusCodigoMapCenarios = new HashMap<String, Set<Cenario>>();
		for (CenarioDTO cenarioDTO : cenariosDTO)
		{
			Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
			
			for (Campus campus : cenario.getCampi())
			{
				if (campusCodigoMapCenarios.get(campus.getCodigo()) == null)
				{
					Set<Cenario> newCenarios = new HashSet<Cenario>();
					newCenarios.add(cenario);
					campusCodigoMapCenarios.put(campus.getCodigo(), newCenarios);
				}
				else
				{
					campusCodigoMapCenarios.get(campus.getCodigo()).add(cenario);
				}
			}
		}
		
		for (String campusCodigo : campusCodigoMapCenarios.keySet())
		{
			BaseTreeModel campusBaseTree = new BaseTreeModel();
			campusBaseTree.set("label", campusCodigo);
			List<BaseTreeModel> rows = new ArrayList<BaseTreeModel>();
			for (Cenario cenario : campusCodigoMapCenarios.get(campusCodigo))
			{
				TreeNodeDTO nodeDTO = new TreeNodeDTO(null,campusCodigo,"",false,true,null);
				int i = 0;
				for (TreeNodeDTO resumoCampus : 
					getResumoParaCampus(Campus.findBy(getInstituicaoEnsinoUser(), cenario, campusCodigo), cenario, null) )
				{
					if (rows.size() <= i)
					{
						BaseTreeModel rowComparacao = new BaseTreeModel();
						rowComparacao.set("label", ((ResumoDTO) resumoCampus.getContent()).getLabel());
						rowComparacao.set(cenario.getNome(), ((ResumoDTO) resumoCampus.getContent()).getValor());
						rows.add(rowComparacao);
					}
					else
					{
						rows.get(i).set(cenario.getNome(), ((ResumoDTO) resumoCampus.getContent()).getValor());
					}
					i++;
				}
			}
			for (BaseTreeModel row : rows)
			{
				campusBaseTree.add(row);
			}
			
			result.add(campusBaseTree);
		}
		
		return result;
		
	}
}
