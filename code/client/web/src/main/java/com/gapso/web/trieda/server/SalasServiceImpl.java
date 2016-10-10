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
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Equivalencia;
import com.gapso.trieda.domain.GrupoSala;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoSala;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.TriedaServerUtil;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.FaixaCapacidadeSalaDTO;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SalaUtilizadaDTO;
import com.gapso.web.trieda.shared.dtos.TipoSalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.services.SalasService;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.gapso.web.trieda.shared.util.view.RelatorioSalaFiltro;

@Transactional
public class SalasServiceImpl
	extends RemoteService
	implements SalasService
{
	private static final long serialVersionUID = -5850050305078103981L;

	@Override
	public SalaDTO getSala( Long id )
	{
		if (id == null) {
			return null;
		}
		Sala sala = Sala.find(id, getInstituicaoEnsinoUser());
		SalaDTO dto = ConvertBeans.toSalaDTO(sala);
		return dto;
	}

	@Override
	public List<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(SalaDTO salaDTO) {
		Sala sala = Sala.find(salaDTO.getId(),getInstituicaoEnsinoUser());
		List<HorarioDisponivelCenario> list = new ArrayList<HorarioDisponivelCenario>(sala.getHorarios(getInstituicaoEnsinoUser()));
		List<HorarioDisponivelCenarioDTO> listDTO = TriedaServerUtil.ordenaHorariosPorSemanaLetivaETurno(list);
		return listDTO;
	}

	@Override
	public void saveHorariosDisponiveis(
		SalaDTO salaDTO, List< HorarioDisponivelCenarioDTO > listDTO )
	{
/*		Sala sala = Sala.find( salaDTO.getId(), getInstituicaoEnsinoUser() );

		List< HorarioDisponivelCenario > listSelecionados
			= ConvertBeans.toHorarioDisponivelCenario( listDTO );

		List< HorarioDisponivelCenario > adicionarList
			= new ArrayList< HorarioDisponivelCenario >( listSelecionados );

		adicionarList.removeAll( sala.getHorarios(
			getInstituicaoEnsinoUser() ) );

		List< HorarioDisponivelCenario > removerList
			= new ArrayList< HorarioDisponivelCenario >(
				sala.getHorarios( getInstituicaoEnsinoUser() ) );

		removerList.removeAll( listSelecionados );

		for ( HorarioDisponivelCenario o : removerList )
		{
			o.getSalas().remove( sala );
			o.merge();
		}

		for ( HorarioDisponivelCenario o : adicionarList )
		{
			o.getSalas().add( sala );
			o.merge();
		}*/
	}

	@Override
	public TipoSalaDTO getTipoSala( Long id )
	{
		return ConvertBeans.toTipoSalaDTO(
			TipoSala.find( id, getInstituicaoEnsinoUser() ) );
	}

	@Override
	public ListLoadResult< SalaDTO > getSalasOtimizadas(  AtendimentoOperacionalDTO turma, ProfessorDTO professorDTO, DisciplinaDTO disciplinaDTO)
	{
		List< SalaDTO > list = new ArrayList< SalaDTO >();
		List< Sala > salas = Sala.findByTurmaOtimizada(
			getInstituicaoEnsinoUser(), ConvertBeans.toAtendimentoOperacional(turma), getCenario(), 
			ConvertBeans.toProfessor(professorDTO), ConvertBeans.toDisciplina(disciplinaDTO) );

		for ( Sala sala : salas )
		{
			list.add( ConvertBeans.toSalaDTO( sala ) );
		}

		return new BaseListLoadResult< SalaDTO >( list );
	}
	
	@Override
	public ListLoadResult< SalaDTO > getBuscaList( UnidadeDTO unidadeDTO )
	{
		Unidade unidade = Unidade.find(
			unidadeDTO.getId(), getInstituicaoEnsinoUser() );

		List< SalaDTO > listDTO = new ArrayList< SalaDTO >();

		List< Sala > list = Sala.findByUnidade(
			getInstituicaoEnsinoUser(), unidade );

		for ( Sala sala : list )
		{
			listDTO.add( ConvertBeans.toSalaDTO( sala ) );
		}

		BaseListLoadResult< SalaDTO > result
			= new BaseListLoadResult< SalaDTO >( listDTO );

		return result;
	}
	
	@Override
	public ListLoadResult< SalaDTO > getAutoCompleteList(
		CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< SalaDTO > list = new ArrayList< SalaDTO >();
		
		List< Sala > listDomains = Sala.find( getInstituicaoEnsinoUser(), cenario,
			loadConfig.get("query").toString(), loadConfig.getOffset(), loadConfig.getLimit() );

		for ( Sala sala : listDomains )
		{
			list.add( ConvertBeans.toSalaDTO( sala ) );
		}

		BasePagingLoadResult< SalaDTO > result
			= new BasePagingLoadResult< SalaDTO >( list );
		result.setOffset( loadConfig.getOffset() );
		return result;
	}

	@Override
	public PagingLoadResult< SalaDTO > getList( CenarioDTO cenarioDTO, CampusDTO campusDTO, UnidadeDTO unidadeDTO, 
			TipoSalaDTO tipoSalaDTO, String operadorCapacidadeInstalada, Integer capacidadeInstalada,
			String operadorCapacidadeMaxima, Integer capacidadeMaxima, String operadorCustoOperacao, Double custoOperacao,
			String  numero, String descricao, String andar, String codigo,
		PagingLoadConfig config )
	{
		List< SalaDTO > list = new ArrayList< SalaDTO >();
		String orderBy = config.getSortField();
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());

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

		Campus campus = campusDTO == null ? null :
			Campus.find( campusDTO.getId(), getInstituicaoEnsinoUser() );

		Unidade unidade = unidadeDTO == null ? null :
			Unidade.find( unidadeDTO.getId(), getInstituicaoEnsinoUser() );
		
		TipoSala tipoSala = tipoSalaDTO == null ? null :
			TipoSala.find(tipoSalaDTO.getId(), getInstituicaoEnsinoUser());

		List< Sala > listDomains = Sala.find( getInstituicaoEnsinoUser(), cenario,
			campus, unidade, tipoSala, operadorCapacidadeInstalada, capacidadeInstalada,
			 operadorCapacidadeMaxima,  capacidadeMaxima,  operadorCustoOperacao, custoOperacao,
			 numero,  descricao,  andar, codigo,
			config.getOffset(), config.getLimit(), orderBy );

		for ( Sala sala : listDomains )
		{
			list.add( ConvertBeans.toSalaDTO( sala ) );
		}

		BasePagingLoadResult< SalaDTO > result
			= new BasePagingLoadResult< SalaDTO >( list );
		result.setOffset( config.getOffset() );
		result.setTotalLength( Sala.count(
				getInstituicaoEnsinoUser(), cenario, campus, unidade,
				tipoSala, operadorCapacidadeInstalada, capacidadeInstalada,
				 operadorCapacidadeMaxima,  capacidadeMaxima,  operadorCustoOperacao, custoOperacao,
				 numero,  descricao,  andar, codigo) );
		return result;
	}

	@Override
	public ListLoadResult< SalaDTO > getAndaresList()
	{
		return getAndaresList( null );
	}

	@Override
	public ListLoadResult< SalaDTO > getAndaresList( Long unidadeId )
	{
		Unidade unidade = Unidade.find(
			unidadeId, getInstituicaoEnsinoUser() );

		List< Sala > listDomains = Sala.findAndaresAll(
				getInstituicaoEnsinoUser(), unidade );

		List< SalaDTO > list = new ArrayList< SalaDTO >();

		for ( Sala sala : listDomains )
		{
			list.add( ConvertBeans.toSalaDTO( sala ) );
		}

		return new BaseListLoadResult< SalaDTO >( list );
	}

	@Override
	public ListLoadResult<SalaDTO> getSalasDoAndareList(
		UnidadeDTO unidadeDTO, List< String > andares )
	{
		Unidade unidade = Unidade.find(
			unidadeDTO.getId(), getInstituicaoEnsinoUser() );

		List< Sala > listDomain = Sala.findSalasDoAndarAll(
				getInstituicaoEnsinoUser(), unidade, andares );

		List< SalaDTO > list = new ArrayList< SalaDTO >();

		for ( Sala sala : listDomain )
		{
			list.add( ConvertBeans.toSalaDTO( sala ) );
		}

		return new BaseListLoadResult< SalaDTO >( list );
	}

	@Override
	public Map< String, List< SalaDTO > > getSalasEAndareMap( Long unidadeId )
	{
		Unidade unidade = Unidade.find(
			unidadeId, getInstituicaoEnsinoUser() );

		List< Sala > salas = Sala.findByUnidade( getInstituicaoEnsinoUser(), unidade );
		Map< String, List< SalaDTO > > map = new HashMap< String, List< SalaDTO > >();

		for ( Sala sala : salas )
		{
			if ( !map.containsKey( sala.getAndar() ) )
			{
				map.put( sala.getAndar(), new ArrayList< SalaDTO >() );
			}

			SalaDTO salaDTO = ConvertBeans.toSalaDTO( sala );
			map.get( sala.getAndar() ).add( salaDTO );
		}

		return map;
	}

	@Override
	public ListLoadResult< SalaDTO > getList()
	{
		List< Sala > listDomain = Sala.findAll(
			getInstituicaoEnsinoUser() );

		List< SalaDTO > list = new ArrayList< SalaDTO >();

		for ( Sala sala : listDomain )
		{
			list.add( ConvertBeans.toSalaDTO( sala ) );
		}

		return new BaseListLoadResult< SalaDTO >( list );
	}

	@Override
	public List< GrupoSalaDTO > getGruposDeSalas( Long unidadeId )
	{
		Unidade unidade = Unidade.find(
			unidadeId, getInstituicaoEnsinoUser() );

		List< GrupoSala > grupoSalas = GrupoSala.findByUnidade(
			getInstituicaoEnsinoUser(), unidade );

		List< GrupoSalaDTO > grupoSalasDTO
			= new ArrayList< GrupoSalaDTO >();

		for ( GrupoSala gs : grupoSalas )
		{
			GrupoSalaDTO gsDTO = ConvertBeans.toGrupoSalaDTO( gs );
			grupoSalasDTO.add( gsDTO );
		}

		return grupoSalasDTO;
	}

	@Override
	public void save( SalaDTO salaDTO )
	{
		Sala sala = ConvertBeans.toSala( salaDTO );

		if ( sala.getId() != null && sala.getId() > 0 )
		{
			sala.merge();
		}
		else
		{
			sala.persistAndPreencheHorarios();
		}
	}

	@Override
	public void remove( List< SalaDTO > salaDTOList )
	{
		for ( SalaDTO salaDTO : salaDTOList )
		{
			Sala.find( salaDTO.getId(),
				getInstituicaoEnsinoUser() ).remove();
		}
	}

	@Override
	public ListLoadResult< TipoSalaDTO > getTipoSalaList( CenarioDTO cenarioDTO )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		InstituicaoEnsino instituicaoEnsino = getInstituicaoEnsinoUser();
		List< TipoSala > list = TipoSala.findByCenario( instituicaoEnsino, cenario );

		if ( list.size() == 0 )
		{
			TipoSala tipo1 = new TipoSala();
			tipo1.setNome( TipoSala.TIPO_SALA_DE_AULA );
			tipo1.setDescricao( TipoSala.TIPO_SALA_DE_AULA );
			tipo1.setInstituicaoEnsino( instituicaoEnsino );
			tipo1.setCenario(cenario);
			tipo1.persist();

			TipoSala tipo2 = new TipoSala();
			tipo2.setNome( "Laboratório" );
			tipo2.setDescricao( "Laboratório" );
			tipo2.setInstituicaoEnsino( instituicaoEnsino );
			tipo2.setCenario(cenario);
			tipo2.persist();

			TipoSala tipo3 = new TipoSala();
			tipo3.setNome( "	" );
			tipo3.setDescricao( "Auditório" );
			tipo3.setInstituicaoEnsino( instituicaoEnsino );
			tipo3.setCenario(cenario);
			tipo3.persist();

			list = TipoSala.findByCenario( getInstituicaoEnsinoUser(), cenario );
		}

		List< TipoSalaDTO > listDTO = new ArrayList< TipoSalaDTO >();

		for ( TipoSala tipo : list )
		{
			listDTO.add( ConvertBeans.toTipoSalaDTO( tipo ) );
		}

		return new BaseListLoadResult< TipoSalaDTO >( listDTO );
	}
	
	@Override
	public void vincula( DisciplinaDTO disciplinaDTO, List< SalaDTO > salasDTO )
	{
		Disciplina disciplina = Disciplina.find(
				disciplinaDTO.getId(), this.getInstituicaoEnsinoUser() );

		for ( SalaDTO salaDTO : salasDTO )
		{
			Sala sal = Sala.find(
					salaDTO.getId(), this.getInstituicaoEnsinoUser() );
			sal.getDisciplinas().add( disciplina );
			sal.merge();
		}
	}
	
	@Override
	public void desvincula( DisciplinaDTO disciplinaDTO, List< SalaDTO > salasDTO )
	{
		Disciplina disciplina = Disciplina.find(
				disciplinaDTO.getId(), this.getInstituicaoEnsinoUser() );

		for ( SalaDTO salaDTO : salasDTO )
		{
			Sala sal = Sala.find(
					salaDTO.getId(), this.getInstituicaoEnsinoUser() );
			sal.getDisciplinas().remove( disciplina );
			sal.merge();
		}
	}
	
	@Override
	public List< SalaDTO > getListVinculadas( DisciplinaDTO disciplinaDTO )
	{
		if ( disciplinaDTO == null )
		{
			return Collections.< SalaDTO >emptyList();
		}

		Disciplina disciplina = Disciplina.find(
				disciplinaDTO.getId(), this.getInstituicaoEnsinoUser() );

		Set< Sala > salaList = disciplina.getSalas();
		List< SalaDTO > salaDTOList
			= new ArrayList< SalaDTO >( salaList.size() );

		for ( Sala sala : salaList )
		{
			salaDTOList.add(
				ConvertBeans.toSalaDTO( sala ) );
		}
		
		Collections.sort(salaDTOList);

		return salaDTOList;
	}
	
	@Override
	public List<DisciplinaDTO> getListDisciplinasVinculadas(SalaDTO salaDTO) {
		if ( salaDTO == null )
		{
			return Collections.< DisciplinaDTO >emptyList();
		}

		Sala sala = Sala.find(
				salaDTO.getId(), this.getInstituicaoEnsinoUser() );

		Set< Disciplina > disciplinaList = sala.getDisciplinas();

		List< DisciplinaDTO > disciplinaDTOList
			= new ArrayList< DisciplinaDTO >( disciplinaList.size() );

		for ( Disciplina disciplina : disciplinaList )
		{
			disciplinaDTOList.add(
				ConvertBeans.toDisciplinaDTO(disciplina) );
		}
		
		Collections.sort(disciplinaDTOList);

		return disciplinaDTOList;
	}

	@Override
	public List< SalaDTO > getListNaoVinculadas( CenarioDTO cenarioDTO, DisciplinaDTO disciplinaDTO )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		if ( disciplinaDTO == null )
		{
			return Collections.< SalaDTO >emptyList();
		}

		Disciplina disciplina = Disciplina.find(
				disciplinaDTO.getId(), this.getInstituicaoEnsinoUser() );

		Set< Sala > salaList = disciplina.getSalas();
		List< Sala > salaDTOList
			= new ArrayList< Sala >( Sala.findByCenario(getInstituicaoEnsinoUser(), cenario) );

		salaDTOList.removeAll( salaList );

		List< SalaDTO > areaTitulacaoDTOList
			= new ArrayList< SalaDTO >( salaDTOList.size() );

		for ( Sala sala : salaDTOList )
		{
			areaTitulacaoDTOList.add(
				ConvertBeans.toSalaDTO( sala ) );
		}
		
		Collections.sort(salaDTOList);

		return areaTitulacaoDTOList;
	}
	
	
	@Override
	public List<DisciplinaDTO> getListDisciplinasNaoVinculadas(CenarioDTO cenarioDTO,
			SalaDTO salaDTO) {
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		if ( salaDTO == null )
		{
			return Collections.< DisciplinaDTO >emptyList();
		}

		Sala sala = Sala.find(
				salaDTO.getId(), this.getInstituicaoEnsinoUser() );

		Set< Disciplina > disciplinaList = sala.getDisciplinas();
		List< Disciplina > disciplinaCenarioList
			= new ArrayList< Disciplina >( Disciplina.findByCenario(getInstituicaoEnsinoUser(), cenario) );

		disciplinaCenarioList.removeAll( disciplinaList );

		List< DisciplinaDTO > disciplinaDTOList
			= new ArrayList< DisciplinaDTO >( disciplinaCenarioList.size() );

		for ( Disciplina disciplina : disciplinaCenarioList )
		{
			disciplinaDTOList.add(
				ConvertBeans.toDisciplinaDTO( disciplina ) );
		}
		
		Collections.sort(disciplinaDTOList);

		return disciplinaDTOList;
	}
	@Override
	public ListLoadResult<FaixaCapacidadeSalaDTO> getFaixasCapacidadeSala(CenarioDTO cenarioDTO, Integer tamanhoFaixa)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		List<FaixaCapacidadeSalaDTO> faixas = new ArrayList<FaixaCapacidadeSalaDTO>();
		
		int capacidadeMaxima = Sala.findCapacidadeMax(getInstituicaoEnsinoUser(), cenario);
		for (int i = 0; i<capacidadeMaxima; i+=tamanhoFaixa)
		{
			FaixaCapacidadeSalaDTO novaFaixa = new FaixaCapacidadeSalaDTO(i, i+tamanhoFaixa);
			faixas.add(novaFaixa);
		}
		
		return new BaseListLoadResult< FaixaCapacidadeSalaDTO >( faixas );
	}
	
	
	@Override
	public List<RelatorioDTO> getRelatorio(CenarioDTO cenarioDTO, RelatorioSalaFiltro salaFiltro, RelatorioDTO currentNode) {
		List<RelatorioDTO> list = new ArrayList<RelatorioDTO>();
		Cenario cenario = Cenario.find(cenarioDTO.getId(),this.getInstituicaoEnsinoUser());
		if (currentNode == null){
			// disponibiliza uma pasta para cada campus no relatório de resumo por campi
			List<Campus> campi = new ArrayList<Campus>(cenario.getCampi());
			Collections.sort(campi);
			for (Campus campus : campi) {
				RelatorioDTO nodeDTO = new RelatorioDTO(campus.getCodigo() + "(" + campus.getNome() + ")");
				nodeDTO.setCampusId(campus.getId());
				getRelatorioParaCampus(cenario, campus, salaFiltro, nodeDTO);
				list.add( nodeDTO );
			}
		}

		return list;
	}
	
	public void getRelatorioParaCampus(Cenario cenario, Campus campus, RelatorioSalaFiltro salaFiltro, RelatorioDTO currentNode) {
		
		Turno turnoFiltro = salaFiltro.getTurno() == null ? null :
			Turno.find(salaFiltro.getTurno().getId(), getInstituicaoEnsinoUser());
		int faixaSuperior = salaFiltro.getFaixaCapacidadeSala() == null ? 0 :
			salaFiltro.getFaixaCapacidadeSala().getFaixaSuperior();
		int faixaInferior = salaFiltro.getFaixaCapacidadeSala() == null ? 0 :
			salaFiltro.getFaixaCapacidadeSala().getFaixaInferior();
		
		List<Sala> todasSalas = Sala.findByCampus(getInstituicaoEnsinoUser(), cenario, campus);
		
		int numAmbientes = 0;
		int numSalas = 0;
		int numLaboratorios = 0;
		
		int numAmbientesExternos = 0;
		int numSalasExternas = 0;
		int numLaboratoriosExternos = 0;
		
		for (Sala sala : todasSalas)
		{
			if (sala.getExterna())
			{
				numAmbientesExternos++;
				if (sala.isLaboratorio())
				{
					numLaboratoriosExternos++;
				}
				else
				{
					numSalasExternas++;
				}
			}
			else
			{
				numAmbientes++;
				if (sala.isLaboratorio())
				{
					numLaboratorios++;
				}
				else
				{
					numSalas++;
				}
			}
		}
		
		boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
		// cálculo dos indicadores de utilização das salas de aula e laboratórios
		Set<Turno> turnosConsiderados = new HashSet<Turno>();
		Set<Sala> salasUtilizadas = new HashSet<Sala>();
		Set<SemanaLetiva> semanasLetivasUtilizadas = new HashSet<SemanaLetiva>();
		Map<String,List<AtendimentoRelatorioDTO>> salaIdTurnoIdToAtendimentosMap = new HashMap<String,List<AtendimentoRelatorioDTO>>();
		if (turnoFiltro != null )
		{
			turnosConsiderados.add(turnoFiltro);
		}
		for (Oferta oferta : campus.getOfertas()) {
			if (turnoFiltro == null) {
				turnosConsiderados.add(oferta.getTurno());
			}
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
					
					if (salaFiltro.getFaixaCapacidadeSala() != null)
					{
						if (aula.getSala().getCapacidadeInstalada() >= faixaInferior && 
									aula.getSala().getCapacidadeInstalada() <= faixaSuperior)
						{
							salasUtilizadas.add(aula.getSala());
						}
					}
					else
					{
						salasUtilizadas.add(aula.getSala());
					}
					semanasLetivasUtilizadas.add(aula.getOferta().getCurriculo().getSemanaLetiva());
				}
			} else {
				// atendimentos operacionais
				List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.getAtendimentosByOferta(getInstituicaoEnsinoUser(), oferta, campus.getCenario());
				for (AtendimentoOperacional atendimento : atendimentos) {
					String key = atendimento.getSala().getId() + "-" + oferta.getTurno().getId();
					List<AtendimentoRelatorioDTO> atendimetosPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
					if (atendimetosPorSalaTurno == null) {
						atendimetosPorSalaTurno = new ArrayList<AtendimentoRelatorioDTO>();
						salaIdTurnoIdToAtendimentosMap.put(key,atendimetosPorSalaTurno);
					}
					atendimetosPorSalaTurno.add(ConvertBeans.toAtendimentoOperacionalDTO(atendimento));
					
					if (salaFiltro.getFaixaCapacidadeSala() != null)
					{
						if (atendimento.getSala().getCapacidadeInstalada() >= faixaInferior && 
										atendimento.getSala().getCapacidadeInstalada() <= faixaSuperior)
						{
							salasUtilizadas.add(atendimento.getSala());
						}
					}
					else
					{
						salasUtilizadas.add(atendimento.getSala());
					}
					semanasLetivasUtilizadas.add(atendimento.getOferta().getCurriculo().getSemanaLetiva());
				}
			}
		}
		
		
		int numAmbientesUtilizados = 0;
		int numSalasUtilizadas = 0;
		int numLaboratoriosUtilizados = 0;
		
		int numAmbientesUtilizadosExternos = 0;
		int numSalasUtilizadasExternas = 0;
		int numLaboratoriosUtilizadosExternos = 0;
		for (Sala sala : salasUtilizadas)
		{
			if (sala.getExterna())
			{
				numAmbientesUtilizadosExternos++;
				if (sala.isLaboratorio())
				{
					numLaboratoriosUtilizadosExternos++;
				}
				else
				{
					numSalasUtilizadasExternas++;
				}
			}
			else
			{
				numAmbientesUtilizados++;
				if (sala.isLaboratorio())
				{
					numLaboratoriosUtilizados++;
				}
				else
				{
					numSalasUtilizadas++;
				}
			}
		}
		
		double utilizacaoMediaDosAmbientes = 0.0;
		double utilizacaoMediaDasSalasDeAula = 0.0;
		double utilizacaoMediaDosLaboratorios = 0.0;
		Double mediaUtilizacaoHorarioAmbientes = 0.0;
		Double mediaUtilizacaoHorarioSalas = 0.0;
		Double mediaUtilizacaoHorarioLaboratorios = 0.0;
		double somatorioDeAlunosDeTodasAsAulasEmSalasDeAula = 0.0;
		double somatorioDeAlunosDeTodasAsAulasEmLaboratorios = 0.0;
		double somatorioDaCapacidadeDasSalasParaTodasAsAulasEmSalasDeAula = 0.0;
		double somatorioDaCapacidadeDosLaboratoriosParaTodasAsAulasEmLaboratorios = 0.0;
		double mediaAlunosPorTurmaPorSala = 0;
		double mediaAlunosPorTurmaPorLaboratorio = 0;
		double mediaAlunosPorTurmaPorAmbientes = 0;
		
		double utilizacaoMediaDosAmbientesExternos = 0.0;
		double utilizacaoMediaDasSalasDeAulaExternas = 0.0;
		double utilizacaoMediaDosLaboratoriosExternos = 0.0;
		Double mediaUtilizacaoHorarioAmbientesExternos = 0.0;
		Double mediaUtilizacaoHorarioSalasExternas = 0.0;
		Double mediaUtilizacaoHorarioLaboratoriosExternos = 0.0;
		double somatorioDeAlunosDeTodasAsAulasEmSalasDeAulaExternas = 0.0;
		double somatorioDeAlunosDeTodasAsAulasEmLaboratoriosExternos = 0.0;
		double somatorioDaCapacidadeDasSalasParaTodasAsAulasEmSalasDeAulaExternas = 0.0;
		double somatorioDaCapacidadeDosLaboratoriosParaTodasAsAulasEmLaboratoriosExternos = 0.0;
		double mediaAlunosPorTurmaPorSalaExternas = 0;
		double mediaAlunosPorTurmaPorLaboratorioExternos = 0;
		double mediaAlunosPorTurmaPorAmbientesExternos = 0;
		Map<Sala, List<Integer>> mediaAlunosPorTurmaPorSalaMap =  new HashMap<Sala, List<Integer>>();
		if (!salaIdTurnoIdToAtendimentosMap.isEmpty()) {
			// [SalaId -> Tempo de uso (min) semanal]
			Map<Long,Integer> salaIdToTempoUsoSemanalEmMinutosMap = new HashMap<Long,Integer>();
			Map<Long,Integer> laboratorioIdToTempoUsoSemanalEmMinutosMap = new HashMap<Long,Integer>();
			Map<Long,Integer> salaExternaIdToTempoUsoSemanalEmMinutosMap = new HashMap<Long,Integer>();
			Map<Long,Integer> laboratorioExternoIdToTempoUsoSemanalEmMinutosMap = new HashMap<Long,Integer>();
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
							if (mediaAlunosPorTurmaPorSalaMap.get(sala) == null)
							{
								List<Integer> novaQuantidadeAlunos = new ArrayList<Integer>();
								novaQuantidadeAlunos.add(aula.getQuantidadeAlunos());
								mediaAlunosPorTurmaPorSalaMap.put(sala, novaQuantidadeAlunos);
							}
							else
							{
								mediaAlunosPorTurmaPorSalaMap.get(sala).add(aula.getQuantidadeAlunos());
							}
							if (sala.getExterna())
							{
								if (!sala.isLaboratorio()) {
									somatorioDeAlunosDeTodasAsAulasEmSalasDeAulaExternas += aula.getQuantidadeAlunos();
									somatorioDaCapacidadeDasSalasParaTodasAsAulasEmSalasDeAulaExternas += sala.getCapacidadeInstalada();
								} else {
									somatorioDeAlunosDeTodasAsAulasEmLaboratoriosExternos += aula.getQuantidadeAlunos();
									somatorioDaCapacidadeDosLaboratoriosParaTodasAsAulasEmLaboratoriosExternos += sala.getCapacidadeInstalada();
								}

								if(!sala.isLaboratorio()){
									Integer tempoUsoSemanalEmMinutos = salaExternaIdToTempoUsoSemanalEmMinutosMap.get(aula.getSalaId());
									if (tempoUsoSemanalEmMinutos == null) tempoUsoSemanalEmMinutos = 0;
									salaExternaIdToTempoUsoSemanalEmMinutosMap.put(aula.getSalaId(), tempoUsoSemanalEmMinutos + aula.getTotalCreditos()*aula.getDuracaoDeUmaAulaEmMinutos());
								}
								else {
									Integer tempoUsoSemanalEmMinutos = laboratorioExternoIdToTempoUsoSemanalEmMinutosMap.get(aula.getSalaId());
									if (tempoUsoSemanalEmMinutos == null) tempoUsoSemanalEmMinutos = 0;
									laboratorioExternoIdToTempoUsoSemanalEmMinutosMap.put(aula.getSalaId(), tempoUsoSemanalEmMinutos + aula.getTotalCreditos()*aula.getDuracaoDeUmaAulaEmMinutos());
								}
							}
							else
							{
								if (!sala.isLaboratorio()) {
									somatorioDeAlunosDeTodasAsAulasEmSalasDeAula += aula.getQuantidadeAlunos();
									somatorioDaCapacidadeDasSalasParaTodasAsAulasEmSalasDeAula += sala.getCapacidadeInstalada();
								} else {
									somatorioDeAlunosDeTodasAsAulasEmLaboratorios += aula.getQuantidadeAlunos();
									somatorioDaCapacidadeDosLaboratoriosParaTodasAsAulasEmLaboratorios += sala.getCapacidadeInstalada();
								}

								if(!sala.isLaboratorio()){
									Integer tempoUsoSemanalEmMinutos = salaIdToTempoUsoSemanalEmMinutosMap.get(aula.getSalaId());
									if (tempoUsoSemanalEmMinutos == null) tempoUsoSemanalEmMinutos = 0;
									salaIdToTempoUsoSemanalEmMinutosMap.put(aula.getSalaId(), tempoUsoSemanalEmMinutos + aula.getTotalCreditos()*aula.getDuracaoDeUmaAulaEmMinutos());
								}
								else {
									Integer tempoUsoSemanalEmMinutos = laboratorioIdToTempoUsoSemanalEmMinutosMap.get(aula.getSalaId());
									if (tempoUsoSemanalEmMinutos == null) tempoUsoSemanalEmMinutos = 0;
									laboratorioIdToTempoUsoSemanalEmMinutosMap.put(aula.getSalaId(), tempoUsoSemanalEmMinutos + aula.getTotalCreditos()*aula.getDuracaoDeUmaAulaEmMinutos());
								}
							}
						}
					}
				}
			}
			utilizacaoMediaDosAmbientes = TriedaUtil.round((somatorioDeAlunosDeTodasAsAulasEmSalasDeAula+somatorioDeAlunosDeTodasAsAulasEmLaboratorios)
					/(somatorioDaCapacidadeDasSalasParaTodasAsAulasEmSalasDeAula+somatorioDaCapacidadeDosLaboratoriosParaTodasAsAulasEmLaboratorios)*100.0,2);
			utilizacaoMediaDasSalasDeAula = TriedaUtil.round(somatorioDeAlunosDeTodasAsAulasEmSalasDeAula/somatorioDaCapacidadeDasSalasParaTodasAsAulasEmSalasDeAula*100.0,2);
			utilizacaoMediaDosLaboratorios = TriedaUtil.round(somatorioDeAlunosDeTodasAsAulasEmLaboratorios/somatorioDaCapacidadeDosLaboratoriosParaTodasAsAulasEmLaboratorios*100.0,2);
			
			utilizacaoMediaDosAmbientesExternos = TriedaUtil.round((somatorioDeAlunosDeTodasAsAulasEmSalasDeAulaExternas+somatorioDeAlunosDeTodasAsAulasEmLaboratoriosExternos)
					/(somatorioDaCapacidadeDasSalasParaTodasAsAulasEmSalasDeAulaExternas+somatorioDaCapacidadeDosLaboratoriosParaTodasAsAulasEmLaboratoriosExternos)*100.0,2);
			utilizacaoMediaDasSalasDeAulaExternas = TriedaUtil.round(somatorioDeAlunosDeTodasAsAulasEmSalasDeAulaExternas/somatorioDaCapacidadeDasSalasParaTodasAsAulasEmSalasDeAulaExternas*100.0,2);
			utilizacaoMediaDosLaboratoriosExternos = TriedaUtil.round(somatorioDeAlunosDeTodasAsAulasEmLaboratoriosExternos/somatorioDaCapacidadeDosLaboratoriosParaTodasAsAulasEmLaboratoriosExternos*100.0,2);
			
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
			
			for(Long salaId : salaExternaIdToTempoUsoSemanalEmMinutosMap.keySet()){
				Integer tempoUsoSalaSemanalEmMinutos = salaExternaIdToTempoUsoSemanalEmMinutosMap.get(salaId);
				mediaUtilizacaoHorarioSalasExternas += ((double)tempoUsoSalaSemanalEmMinutos / cargaHorariaSemanalEmMinutos);
				mediaUtilizacaoHorarioAmbientesExternos += ((double)tempoUsoSalaSemanalEmMinutos / cargaHorariaSemanalEmMinutos);
			}
			mediaUtilizacaoHorarioSalasExternas = TriedaUtil.round(mediaUtilizacaoHorarioSalasExternas / salaExternaIdToTempoUsoSemanalEmMinutosMap.size() * 100, 2);
			for(Long salaId : laboratorioExternoIdToTempoUsoSemanalEmMinutosMap.keySet()){
				Integer tempoUsoSalaSemanalEmMinutos = laboratorioExternoIdToTempoUsoSemanalEmMinutosMap.get(salaId);
				mediaUtilizacaoHorarioLaboratoriosExternos += ((double)tempoUsoSalaSemanalEmMinutos / cargaHorariaSemanalEmMinutos);
				mediaUtilizacaoHorarioAmbientesExternos += ((double)tempoUsoSalaSemanalEmMinutos / cargaHorariaSemanalEmMinutos);
			}
			mediaUtilizacaoHorarioLaboratoriosExternos = TriedaUtil.round(mediaUtilizacaoHorarioLaboratoriosExternos / laboratorioExternoIdToTempoUsoSemanalEmMinutosMap.size() * 100, 2);
			mediaUtilizacaoHorarioAmbientesExternos = TriedaUtil.round(mediaUtilizacaoHorarioAmbientesExternos / (laboratorioExternoIdToTempoUsoSemanalEmMinutosMap.size()+salaExternaIdToTempoUsoSemanalEmMinutosMap.size()) * 100, 2);
			
			for (Entry<Sala, List<Integer>> salaQuantidadeAlunosPorTurma : mediaAlunosPorTurmaPorSalaMap.entrySet())
			{
				int totalAlunos = 0;
				if (salaQuantidadeAlunosPorTurma.getKey().getExterna())
				{
					if (salaQuantidadeAlunosPorTurma.getKey().isLaboratorio())
					{
						for (Integer quantidadeAlunos : salaQuantidadeAlunosPorTurma.getValue())
						{
							totalAlunos += quantidadeAlunos;
						}
						mediaAlunosPorTurmaPorLaboratorioExternos += (double)totalAlunos/(double)salaQuantidadeAlunosPorTurma.getValue().size();
					}
					else
					{
						for (Integer quantidadeAlunos : salaQuantidadeAlunosPorTurma.getValue())
						{
							totalAlunos += quantidadeAlunos;
						}
						mediaAlunosPorTurmaPorSalaExternas += (double)totalAlunos/(double)salaQuantidadeAlunosPorTurma.getValue().size();
					}
				}
				else
				{
					if (salaQuantidadeAlunosPorTurma.getKey().isLaboratorio())
					{
						for (Integer quantidadeAlunos : salaQuantidadeAlunosPorTurma.getValue())
						{
							totalAlunos += quantidadeAlunos;
						}
						mediaAlunosPorTurmaPorLaboratorio += (double)totalAlunos/(double)salaQuantidadeAlunosPorTurma.getValue().size();
					}
					else
					{
						for (Integer quantidadeAlunos : salaQuantidadeAlunosPorTurma.getValue())
						{
							totalAlunos += quantidadeAlunos;
						}
						mediaAlunosPorTurmaPorSala += (double)totalAlunos/(double)salaQuantidadeAlunosPorTurma.getValue().size();
					}
				}
			}
			mediaAlunosPorTurmaPorAmbientes = (mediaAlunosPorTurmaPorSala + mediaAlunosPorTurmaPorLaboratorio) / numAmbientesUtilizados;
			mediaAlunosPorTurmaPorAmbientesExternos = (mediaAlunosPorTurmaPorSalaExternas + mediaAlunosPorTurmaPorLaboratorioExternos) / numAmbientesUtilizadosExternos;
		}

		mediaAlunosPorTurmaPorLaboratorio = mediaAlunosPorTurmaPorLaboratorio/numLaboratoriosUtilizados;
		mediaAlunosPorTurmaPorSala = mediaAlunosPorTurmaPorSala/numSalasUtilizadas;
		
		mediaAlunosPorTurmaPorLaboratorioExternos = mediaAlunosPorTurmaPorLaboratorioExternos/numLaboratoriosUtilizadosExternos;
		mediaAlunosPorTurmaPorSalaExternas = mediaAlunosPorTurmaPorSalaExternas/numSalasUtilizadasExternas;
		
		Locale pt_BR = new Locale("pt","BR");
		NumberFormatter numberFormatter = new NumberFormatter();

		RelatorioDTO perfil = new RelatorioDTO( "<b>Perfil</b>");
		perfil.add( new RelatorioDTO( " Total de Ambientes Cadastrados: <b>" + numberFormatter.print(numAmbientes,pt_BR) + "</b>") );
		perfil.add( new RelatorioDTO( " |--Total de Salas de Aula: <b>" + numberFormatter.print(numSalas,pt_BR) + "</b>") );
		perfil.add( new RelatorioDTO( " |--Total de Laboratorios: <b>" + numberFormatter.print(numLaboratorios,pt_BR) + "</b>") );
		perfil.add( new RelatorioDTO( " Total de Ambientes Externos Cadastrados: <b>" + numberFormatter.print(numAmbientesExternos,pt_BR) + "</b>") );
		perfil.add( new RelatorioDTO( " |--Total de Salas de Aula Externas: <b>" + numberFormatter.print(numSalasExternas,pt_BR) + "</b>") );
		perfil.add( new RelatorioDTO( " |--Total de Laboratorios Externos: <b>" + numberFormatter.print(numLaboratoriosExternos,pt_BR) + "</b>") );
		currentNode.add(perfil);
		
		RelatorioDTO atendimento = new RelatorioDTO( "<b>Atendimento</b>");
		atendimento.add( new RelatorioDTO( " Total de Ambientes Utilizados: <b>" + numberFormatter.print(numAmbientesUtilizados,pt_BR) + "</b>") );
		atendimento.add( new RelatorioDTO( " |--Total de Salas de Aula Utilizadas: <b>" + numberFormatter.print(numSalasUtilizadas,pt_BR) + "</b>") );
		atendimento.add( new RelatorioDTO( " |--Total de Laboratorios Utilizados: <b>" + numberFormatter.print(numLaboratoriosUtilizados,pt_BR) + "</b>") );
		atendimento.add( new RelatorioDTO( " Total de Ambientes Externos Utilizados: <b>" + numberFormatter.print(numAmbientesUtilizadosExternos,pt_BR) + "</b>") );
		atendimento.add( new RelatorioDTO( " |--Total de Salas de Aula Externas Utilizadas: <b>" + numberFormatter.print(numSalasUtilizadasExternas,pt_BR) + "</b>") );
		atendimento.add( new RelatorioDTO( " |--Total de Laboratorios Externos Utilizados: <b>" + numberFormatter.print(numLaboratoriosUtilizadosExternos,pt_BR) + "</b>") );
		atendimento.add( new RelatorioDTO( " Média de Alunos por Turma Alocada por Ambiente: <b>" + numberFormatter.print(
				TriedaUtil.round(mediaAlunosPorTurmaPorAmbientes, 2),pt_BR) + "</b>") );
		atendimento.add( new RelatorioDTO( " |--Média de Alunos por Turma Alocada por Sala de Aula: <b>" + numberFormatter.print(
				TriedaUtil.round(mediaAlunosPorTurmaPorSala, 2),pt_BR) + "</b>") );
		atendimento.add( new RelatorioDTO( " |--Média de Alunos por Turma Alocada por Laboratório: <b>" + numberFormatter.print(
				TriedaUtil.round(mediaAlunosPorTurmaPorLaboratorio, 2),pt_BR) + "</b>") );
		atendimento.add( new RelatorioDTO( " Média de Alunos por Turma Alocada por Ambiente Externo: <b>" + numberFormatter.print(
				TriedaUtil.round(mediaAlunosPorTurmaPorAmbientesExternos, 2),pt_BR) + "</b>") );
		atendimento.add( new RelatorioDTO( " |--Média de Alunos por Turma Alocada por Sala de Aula Externa: <b>" + numberFormatter.print(
				TriedaUtil.round(mediaAlunosPorTurmaPorSalaExternas, 2),pt_BR) + "</b>") );
		atendimento.add( new RelatorioDTO( " |--Média de Alunos por Turma Alocada por Laboratório Externo: <b>" + numberFormatter.print(
				TriedaUtil.round(mediaAlunosPorTurmaPorLaboratorioExternos, 2),pt_BR) + "</b>") );
		atendimento.add( new RelatorioDTO( " Ocupação Média da Capacidade dos Ambientes (%): <b>" + TriedaServerUtil.percentFormat(
				utilizacaoMediaDosAmbientes) + "</b>") );
		atendimento.add( new RelatorioDTO( " |--Ocupação Média da Capacidade das Salas de Aula: <b>" + TriedaServerUtil.percentFormat(
				utilizacaoMediaDasSalasDeAula) + "</b>") );
		atendimento.add( new RelatorioDTO( " |--Ocupação Média da Capacidade dos Laboratórios: <b>" + TriedaServerUtil.percentFormat(
				utilizacaoMediaDosLaboratorios) + "</b>") );
		atendimento.add( new RelatorioDTO( " Ocupação Média da Capacidade dos Ambientes Externos(%): <b>" + TriedaServerUtil.percentFormat(
				utilizacaoMediaDosAmbientesExternos) + "</b>") );
		atendimento.add( new RelatorioDTO( " |--Ocupação Média da Capacidade das Salas de Aula Externas: <b>" + TriedaServerUtil.percentFormat(
				utilizacaoMediaDasSalasDeAulaExternas) + "</b>") );
		atendimento.add( new RelatorioDTO( " |--Ocupação Média da Capacidade dos Laboratórios Externos: <b>" + TriedaServerUtil.percentFormat(
				utilizacaoMediaDosLaboratoriosExternos) + "</b>") );
		atendimento.add( new RelatorioDTO( " Utilização Média dos Horários dos Ambientes (%): <b>" + TriedaServerUtil.percentFormat(
				mediaUtilizacaoHorarioAmbientes) + "</b>") );
		atendimento.add( new RelatorioDTO( " |--Utilização Média dos Horários das Salas de Aula: <b>" + TriedaServerUtil.percentFormat(
				mediaUtilizacaoHorarioSalas) + "</b>") );
		atendimento.add( new RelatorioDTO( " |--Utilização Média dos Horários dos Laboratórios: <b>" + TriedaServerUtil.percentFormat(
				mediaUtilizacaoHorarioLaboratorios) + "</b>") );
		atendimento.add( new RelatorioDTO( " Utilização Média dos Horários dos Ambientes Externos(%): <b>" + TriedaServerUtil.percentFormat(
				mediaUtilizacaoHorarioAmbientesExternos) + "</b>") );
		atendimento.add( new RelatorioDTO( " |--Utilização Média dos Horários das Salas de Aula Externas: <b>" + TriedaServerUtil.percentFormat(
				mediaUtilizacaoHorarioSalasExternas) + "</b>") );
		atendimento.add( new RelatorioDTO( " |--Utilização Média dos Horários dos Laboratórios Externos: <b>" + TriedaServerUtil.percentFormat(
				mediaUtilizacaoHorarioLaboratoriosExternos) + "</b>") );
		RelatorioDTO listaAmbientes = new RelatorioDTO( "Lista de Ambientes" );
		listaAmbientes.setButtonText(listaAmbientes.getText());
		listaAmbientes.setButtonIndex(2);
		listaAmbientes.setCampusId(campus.getId());
		atendimento.add( listaAmbientes );
		currentNode.add(atendimento);
		
		RelatorioDTO histogramas = new RelatorioDTO( "<b>Histogramas</b>");
		RelatorioDTO histograma1 = new RelatorioDTO( "Utilização dos Horários" );
		histograma1.setButtonText(histograma1.getText());
		histograma1.setButtonIndex(0);
		histogramas.add( histograma1 );
		RelatorioDTO histograma2 = new RelatorioDTO( "Ocupação da Capacidade" );
		histograma2.setButtonText(histograma2.getText());
		histograma2.setButtonIndex(1);
		histogramas.add( histograma2 );
		currentNode.add(histogramas);
	}
	
	@Override
	public PagingLoadResult<SalaUtilizadaDTO> getSalasUtilizadas(CenarioDTO cenarioDTO, Long campusDTO, RelatorioSalaFiltro salaFiltro, final BasePagingLoadConfig loadConfig)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO, getInstituicaoEnsinoUser());
		
		Turno turnoFiltro = salaFiltro.getTurno() == null ? null :
			Turno.find(salaFiltro.getTurno().getId(), getInstituicaoEnsinoUser());
		int faixaSuperior = salaFiltro.getFaixaCapacidadeSala() == null ? 0 :
			salaFiltro.getFaixaCapacidadeSala().getFaixaSuperior();
		int faixaInferior = salaFiltro.getFaixaCapacidadeSala() == null ? 0 :
			salaFiltro.getFaixaCapacidadeSala().getFaixaInferior();
		
		boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
		// cálculo dos indicadores de utilização das salas de aula e laboratórios
		Set<Turno> turnosConsiderados = new HashSet<Turno>();
		Set<Sala> salasUtilizadas = new HashSet<Sala>();
		Set<SemanaLetiva> semanasLetivasUtilizadas = new HashSet<SemanaLetiva>();
		Map<String,List<AtendimentoRelatorioDTO>> salaIdTurnoIdToAtendimentosMap = new HashMap<String,List<AtendimentoRelatorioDTO>>();
		List<SalaUtilizadaDTO> salaUtilizadaDTOList = new ArrayList<SalaUtilizadaDTO>();
		if (turnoFiltro != null )
		{
			turnosConsiderados.add(turnoFiltro);
		}
		for (Oferta oferta : campus.getOfertas()) {
			if (turnoFiltro == null) {
				turnosConsiderados.add(oferta.getTurno());
			}
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
					
					if (salaFiltro.getFaixaCapacidadeSala() != null)
					{
						if (aula.getSala().getCapacidadeInstalada() >= faixaInferior && 
									aula.getSala().getCapacidadeInstalada() <= faixaSuperior)
						{
							salasUtilizadas.add(aula.getSala());
						}
					}
					else
					{
						salasUtilizadas.add(aula.getSala());
					}
					semanasLetivasUtilizadas.add(aula.getOferta().getCurriculo().getSemanaLetiva());
				}
			} else {
				// atendimentos operacionais
				List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.getAtendimentosByOferta(getInstituicaoEnsinoUser(), oferta, cenario);
				for (AtendimentoOperacional atendimento : atendimentos) {
					String key = atendimento.getSala().getId() + "-" + oferta.getTurno().getId();
					List<AtendimentoRelatorioDTO> atendimetosPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
					if (atendimetosPorSalaTurno == null) {
						atendimetosPorSalaTurno = new ArrayList<AtendimentoRelatorioDTO>();
						salaIdTurnoIdToAtendimentosMap.put(key,atendimetosPorSalaTurno);
					}
					atendimetosPorSalaTurno.add(ConvertBeans.toAtendimentoOperacionalDTO(atendimento));
					
					if (salaFiltro.getFaixaCapacidadeSala() != null)
					{
						if (atendimento.getSala().getCapacidadeInstalada() >= faixaInferior && 
										atendimento.getSala().getCapacidadeInstalada() <= faixaSuperior)
						{
							salasUtilizadas.add(atendimento.getSala());
						}
					}
					else
					{
						salasUtilizadas.add(atendimento.getSala());
					}
					semanasLetivasUtilizadas.add(atendimento.getOferta().getCurriculo().getSemanaLetiva());
				}
			}
		}
		
		//calculo do indicador de taxa de uso dos horarios das salas de aula
		SemanaLetiva maiorSemanaLetiva = SemanaLetiva.getSemanaLetivaComMaiorCargaHoraria(semanasLetivasUtilizadas);
		Map<Integer, Integer> countHorariosAula = new HashMap<Integer, Integer>();
		if (semanasLetivasUtilizadas.size() > 0)
		{
			for(HorarioAula ha : maiorSemanaLetiva.getHorariosAula()){
				for(HorarioDisponivelCenario hdc : ha.getHorariosDisponiveisCenario()){
					int semanaInt = Semanas.toInt(hdc.getDiaSemana());
					Integer value = countHorariosAula.get(semanaInt);
					value = ((value == null) ? 0 : value);
					countHorariosAula.put(semanaInt, value + 1);
				}
			}
		}
		int cargaHorariaSemanalEmMinutos = 0;
		for(Integer i : countHorariosAula.keySet()){
			cargaHorariaSemanalEmMinutos += countHorariosAula.get(i) * maiorSemanaLetiva.getTempo();
		}
		
		Locale pt_BR = new Locale("pt","BR");
		NumberFormatter numberFormatter = new NumberFormatter();
		
		Map<Sala, List<Integer>> mediaAlunosPorTurmaPorSalaMap =  new HashMap<Sala, List<Integer>>();
		if (!salaIdTurnoIdToAtendimentosMap.isEmpty()) {
			// [SalaId -> Tempo de uso (min) semanal]
			AtendimentosServiceImpl atService = new AtendimentosServiceImpl();
			for (Sala sala : salasUtilizadas) {
				double somatorioDeAlunos = 0.0;
				double somatorioDaCapacidade = 0.0;
				int cargaHoraria = 0;
				double mediaUtilizacaoHorario = 0.0;
				for (Turno turno : turnosConsiderados) {
					
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
							if (mediaAlunosPorTurmaPorSalaMap.get(sala) == null)
							{
								List<Integer> novaQuantidadeAlunos = new ArrayList<Integer>();
								novaQuantidadeAlunos.add(aula.getQuantidadeAlunos());
								mediaAlunosPorTurmaPorSalaMap.put(sala, novaQuantidadeAlunos);
							}
							else
							{
								mediaAlunosPorTurmaPorSalaMap.get(sala).add(aula.getQuantidadeAlunos());
							}
							
							somatorioDeAlunos +=  aula.getQuantidadeAlunos();
							somatorioDaCapacidade += sala.getCapacidadeInstalada();
							
							cargaHoraria += aula.getTotalCreditos()*aula.getDuracaoDeUmaAulaEmMinutos();
						}
						mediaUtilizacaoHorario += (double) cargaHoraria / cargaHorariaSemanalEmMinutos;
					}
				}
					SalaUtilizadaDTO novaSalaUtilizada = new SalaUtilizadaDTO();
					novaSalaUtilizada.setId(sala.getId());
					novaSalaUtilizada.setCodigo(sala.getCodigo());
					novaSalaUtilizada.setCampus(sala.getUnidade().getCampus().getCodigo());
					novaSalaUtilizada.setUnidadeString(sala.getUnidade().getCodigo());
					novaSalaUtilizada.setAndar(sala.getAndar());
					novaSalaUtilizada.setNumero(sala.getNumero());
					novaSalaUtilizada.setCargaHoraria(cargaHoraria);
					novaSalaUtilizada.setOcupacaoCapacidade(numberFormatter.print(TriedaUtil.round((double)somatorioDeAlunos/somatorioDaCapacidade * 100, 2), pt_BR) + "%");
					novaSalaUtilizada.setUtilizacaoHorario(numberFormatter.print(TriedaUtil.round(mediaUtilizacaoHorario*100, 2), pt_BR) + "%");
					
					salaUtilizadaDTOList.add(novaSalaUtilizada);
			}
		}
		final String orderBy = loadConfig.getSortField();
		Collections.sort(salaUtilizadaDTOList,new Comparator<SalaUtilizadaDTO>() {
			@Override
			public int compare(SalaUtilizadaDTO o1, SalaUtilizadaDTO o2) {
				if (orderBy == null)
				{
					return o1.getId().compareTo(o2.getId());
				}
				else if (orderBy.equals(SalaUtilizadaDTO.PROPERTY_CODIGO))
				{
					if (loadConfig.getSortDir() == SortDir.ASC)
						return o1.getCodigo().compareTo(o2.getCodigo());
					else
						return o2.getCodigo().compareTo(o1.getCodigo());
				}
				else if (orderBy.equals(SalaUtilizadaDTO.PROPERTY_CAMPUS))
				{
					if (loadConfig.getSortDir() == SortDir.ASC)
						return o1.getCampus().compareTo(o2.getCampus());
					else
						return o2.getCampus().compareTo(o1.getCampus());
				}
				else if (orderBy.equals(SalaUtilizadaDTO.PROPERTY_UNIDADE))
				{
					if (loadConfig.getSortDir() == SortDir.ASC)
						return o1.getUnidade().compareTo(o2.getUnidade());
					else
						return o2.getUnidade().compareTo(o1.getUnidade());
				}
				else if (orderBy.equals(SalaUtilizadaDTO.PROPERTY_ANDAR))
				{
					if (loadConfig.getSortDir() == SortDir.ASC)
						return o1.getAndar().compareTo(o2.getAndar());
					else
						return o2.getAndar().compareTo(o1.getAndar());
				}
				else if (orderBy.equals(SalaUtilizadaDTO.PROPERTY_NUMERO))
				{
					if (loadConfig.getSortDir() == SortDir.ASC)
						return o1.getNumero().compareTo(o2.getNumero());
					else
						return o2.getNumero().compareTo(o1.getNumero());
				}
				else if (orderBy.equals(SalaUtilizadaDTO.PROPERTY_CARGA_HORARIA))
				{
					if (loadConfig.getSortDir() == SortDir.ASC)
						return o1.getCargaHoraria().compareTo(o2.getCargaHoraria());
					else
						return o2.getCargaHoraria().compareTo(o1.getCargaHoraria());
				}
				else if (orderBy.equals(SalaUtilizadaDTO.PROPERTY_UTILZIACAO_HORARIO))
				{
					if (loadConfig.getSortDir() == SortDir.ASC)
						return o1.getUtilizacaoHorario().compareTo(o2.getUtilizacaoHorario());
					else
						return o2.getUtilizacaoHorario().compareTo(o1.getUtilizacaoHorario());
				}
				else if (orderBy.equals(SalaUtilizadaDTO.PROPERTY_OCUPACAO_CAPACIDADE))
				{
					if (loadConfig.getSortDir() == SortDir.ASC)
						return o1.getOcupacaoCapacidade().compareTo(o2.getOcupacaoCapacidade());
					else
						return o2.getOcupacaoCapacidade().compareTo(o1.getOcupacaoCapacidade());
				}
				else
				{
					return o1.getId().compareTo(o2.getId());
				}
			}
		});
		
		List<SalaUtilizadaDTO> salasPagina = new ArrayList<SalaUtilizadaDTO>();
		for ( int i = loadConfig.getOffset(); (i < salaUtilizadaDTOList.size() && i < (loadConfig.getOffset() + loadConfig.getLimit())); i++ )
		{
			salasPagina.add( salaUtilizadaDTOList.get(i) );
		}
		
		BasePagingLoadResult< SalaUtilizadaDTO > result
		= new BasePagingLoadResult< SalaUtilizadaDTO >( salasPagina );
		result.setOffset( loadConfig.getOffset() );
		result.setTotalLength( salaUtilizadaDTOList.size() );
		return result;
		
	}
	
	@Override
	public SalaDTO getProxSala( CenarioDTO cenarioDTO, SalaDTO salaDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Sala sala = Sala.find(salaDTO.getId(), getInstituicaoEnsinoUser());
		
		Sala proxSala = Sala.findProx(getInstituicaoEnsinoUser(), cenario, sala);

		return ConvertBeans.toSalaDTO(proxSala);
	}
	
	@Override
	public SalaDTO getAntSala( CenarioDTO cenarioDTO, SalaDTO salaDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Sala sala = Sala.find(salaDTO.getId(), getInstituicaoEnsinoUser());
		
		Sala proxSala = Sala.findAnt(getInstituicaoEnsinoUser(), cenario, sala);

		return ConvertBeans.toSalaDTO(proxSala);
	}

	@Override
	public void associarDisciplinas(CenarioDTO cenarioDTO, boolean salas, boolean lab) {
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		List<Sala> listaSala = Sala.findByCenario(getInstituicaoEnsinoUser(), cenario);
		List<Equivalencia> listaEquivalencia =  Equivalencia.find(getInstituicaoEnsinoUser());
		
		for(Equivalencia equivalencia : listaEquivalencia){
			for(Sala sala : listaSala){
				if(!salas && !sala.isLaboratorio()){
					continue;
				}
				if(!lab && sala.isLaboratorio()){
					continue;
				}
				for(Disciplina disciplina : sala.getDisciplinas()){
					if(disciplina.equals(equivalencia.getCursou())){
						sala.getDisciplinas().add(equivalencia.getElimina());
						sala.persist();
					}
				}
			}
		}
		
		
	}

	@Override
	public void vincula(SalaDTO salaDTO, List<DisciplinaDTO> disciplinasDTO) {
		Sala sala = Sala.find(
				salaDTO.getId(), this.getInstituicaoEnsinoUser() );

		for ( DisciplinaDTO disciplinaDTO : disciplinasDTO )
		{
			Disciplina disciplina = Disciplina.find(
					disciplinaDTO.getId(), this.getInstituicaoEnsinoUser() );
			disciplina.getSalas().add( sala );
			disciplina.merge();
		}
		
	}

	@Override
	public void desvincula(SalaDTO salaDTO, List<DisciplinaDTO> disciplinasDTO) {
		Sala sala = Sala.find(
				salaDTO.getId(), this.getInstituicaoEnsinoUser() );

		for ( DisciplinaDTO disciplinaDTO : disciplinasDTO )
		{
			Disciplina disciplina = Disciplina.find(
					disciplinaDTO.getId(), this.getInstituicaoEnsinoUser() );
			disciplina.getSalas().remove( sala );
			disciplina.merge();
		}
		
	}

}
