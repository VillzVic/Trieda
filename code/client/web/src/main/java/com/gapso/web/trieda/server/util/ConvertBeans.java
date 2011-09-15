package com.gapso.web.trieda.server.util;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collection;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.CursoDescompartilha;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.DeslocamentoCampus;
import com.gapso.trieda.domain.DeslocamentoUnidade;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.DivisaoCredito;
import com.gapso.trieda.domain.Equivalencia;
import com.gapso.trieda.domain.Fixacao;
import com.gapso.trieda.domain.GrupoSala;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Parametro;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorDisciplina;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.TipoCurso;
import com.gapso.trieda.domain.TipoDisciplina;
import com.gapso.trieda.domain.TipoSala;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.domain.Usuario;
import com.gapso.trieda.misc.Dificuldades;
import com.gapso.trieda.misc.Estados;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDescompartilhaDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoCampusDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoUnidadeDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.DivisaoCreditoDTO;
import com.gapso.web.trieda.shared.dtos.EquivalenciaDTO;
import com.gapso.web.trieda.shared.dtos.FixacaoDTO;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioAulaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorCampusDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.dtos.TipoCursoDTO;
import com.gapso.web.trieda.shared.dtos.TipoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.TipoSalaDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.util.TriedaCurrency;
import com.gapso.web.trieda.shared.util.TriedaUtil;

public class ConvertBeans
{
	// USUÁRIO
	public static Usuario toUsuario( UsuarioDTO dto )
	{
		Usuario domain = new Usuario();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setInstituicaoEnsino( instituicaoEnsino );
		domain.setVersion(dto.getVersion());
		domain.setNome(dto.getNome());
		domain.setEmail(dto.getEmail());
		domain.setUsername(dto.getUsername());
		domain.setPassword(dto.getPassword());
		domain.setEnabled(dto.getEnabled());
		domain.setProfessor( Professor.find(
			dto.getProfessorId(), instituicaoEnsino ) );

		return domain;
	}

	public static UsuarioDTO toUsuarioDTO( Usuario domain )
	{
		UsuarioDTO dto = new UsuarioDTO();

		InstituicaoEnsino instituicaoEnsino
			= domain.getInstituicaoEnsino();

		dto.setVersion( domain.getVersion() );
		dto.setNome( domain.getNome() );
		dto.setEmail( domain.getEmail() );
		dto.setUsername( domain.getUsername() );
		dto.setPassword( domain.getPassword() );
		dto.setEnabled( domain.getEnabled() );
		dto.setDisplayText( domain.getNome() );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		Professor professor = domain.getProfessor();
		if ( professor != null )
		{
			dto.setProfessorId( domain.getProfessor().getId() );
			dto.setProfessorDisplayText( domain.getProfessor().getNome() );
		}

		return dto;
	}

	// CENÁRIO
	public static Cenario toCenario( CenarioDTO dto )
	{
		Cenario domain = new Cenario();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setNome( dto.getNome() );
		domain.setMasterData( dto.getMasterData() );
		domain.setOficial( dto.getOficial() );
		domain.setAno( dto.getAno() );
		domain.setSemestre( dto.getSemestre() );
		domain.setComentario( dto.getComentario() );

		SemanaLetiva semanaLetiva = SemanaLetiva.find(
			dto.getSemanaLetivaId(), instituicaoEnsino );
		domain.setSemanaLetiva( semanaLetiva );

		return domain;
	}

	public static CenarioDTO toCenarioDTO( Cenario domain )
	{
		CenarioDTO dto = new CenarioDTO();

		InstituicaoEnsino instituicaoEnsino
			= domain.getSemanaLetiva().getInstituicaoEnsino();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setNome( domain.getNome() );
		dto.setMasterData( domain.getMasterData() );
		dto.setOficial( domain.getOficial() );
		dto.setAno( domain.getAno() );
		dto.setSemestre( domain.getSemestre() );
		dto.setComentario( domain.getComentario() );

		SemanaLetiva semanaLetiva = null;
		if ( !domain.getMasterData() )
		{
			List< SemanaLetiva > list = SemanaLetiva.findAll( instituicaoEnsino );
			semanaLetiva = ( list == null ? null : list.get( 0 ) );
			// semanaLetiva = SemanaLetiva.getByOficial();
		}
		else
		{
			List< SemanaLetiva > list = SemanaLetiva.findAll( instituicaoEnsino );
			semanaLetiva = ( list == null ? null : list.get( 0 ) );
			// semanaLetiva = SemanaLetiva.getByOficial();
		}

		dto.setSemanaLetivaId( ( ( semanaLetiva == null ) ? null : semanaLetiva.getId() ) );
		dto.setSemanaLetivaString( ( ( semanaLetiva == null ) ? "" : semanaLetiva.getCodigo() ) );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		dto.setDisplayText( domain.getNome() );

		return dto;
	}

	// CAMPUS
	public static Campus toCampus( CampusDTO dto )
	{
		Campus domain = new Campus();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setInstituicaoEnsino( instituicaoEnsino );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );

		Cenario cenario = Cenario.find(
			dto.getCenarioId(), instituicaoEnsino );
		domain.setCenario( cenario );
		domain.setNome( dto.getNome() );
		domain.setCodigo( dto.getCodigo() );

		for ( Estados estadoEnum : Estados.values() )
		{
			if ( estadoEnum.name().equals( dto.getEstado() ) )
			{
				domain.setEstado( estadoEnum );
				break;
			}
		}

		domain.setMunicipio( dto.getMunicipio() );
		domain.setBairro( dto.getBairro() );
		domain.setValorCredito( dto.getValorCredito().getDoubleValue() );
		domain.setPublicado( dto.getPublicado() );

		return domain;
	}

	public static CampusDTO toCampusDTO( Campus domain )
	{
		CampusDTO dto = new CampusDTO();

		InstituicaoEnsino instituicaoEnsino
			= domain.getInstituicaoEnsino();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setNome( domain.getNome() );
		dto.setCenarioId( domain.getCenario().getId() );
		dto.setCodigo( domain.getCodigo() );
		dto.setValorCredito( new TriedaCurrency( domain.getValorCredito() ) );
		dto.setDisplayText( domain.getCodigo() + " (" + domain.getNome() + ")" );

		if ( domain.getEstado() != null )
		{
			dto.setEstado( domain.getEstado().name() );
		}

		if ( domain.getMunicipio() != null )
		{
			dto.setMunicipio(domain.getMunicipio() );
		}

		if ( domain.getBairro() != null )
		{
			dto.setBairro( domain.getBairro() );
		}

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		dto.setPublicado(domain.getPublicado() == null ? false : domain.getPublicado());
		dto.setOtimizadoTatico( domain.isOtimizadoTatico( instituicaoEnsino ) );
		dto.setOtimizadoOperacional( domain.isOtimizadoOperacional( instituicaoEnsino ) );

		return dto;
	}

	// UNIDADE
	public static Unidade toUnidade( UnidadeDTO dto )
	{
		Unidade domain = new Unidade();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setNome( dto.getNome() );
		domain.setCodigo( dto.getCodigo() );

		Campus campus = Campus.find(
			dto.getCampusId(), instituicaoEnsino );
		domain.setCampus( campus );

		return domain;
	}

	public static UnidadeDTO toUnidadeDTO( Unidade domain )
	{
		UnidadeDTO dto = new UnidadeDTO();

		InstituicaoEnsino instituicaoEnsino
			= domain.getCampus().getInstituicaoEnsino();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setNome( domain.getNome() );
		dto.setCodigo( domain.getCodigo() );

		Campus campus = domain.getCampus();
		dto.setCampusId( campus.getId() );
		dto.setCampusString( campus.getCodigo() );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		dto.setDisplayText( domain.getNome() + " (" + domain.getCodigo() + ")" );

		return dto;
	}

	// PROFESSOR CAMPUS
	public static List< ProfessorCampusDTO > toProfessorCampusDTO( Campus campus )
	{
		Set< Professor > professores = campus.getProfessores();
		List< ProfessorCampusDTO > list
			= new ArrayList< ProfessorCampusDTO >( professores.size() );

		InstituicaoEnsino instituicaoEnsino = campus.getInstituicaoEnsino();

		for ( Professor professor : professores )
		{
			ProfessorCampusDTO dto = new ProfessorCampusDTO();

			if ( instituicaoEnsino != null )
			{
				dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
				dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
			}

			dto.setProfessorId( professor.getId() );
			dto.setProfessorString( professor.getNome() );
			dto.setProfessorCpf( professor.getCpf() );
			dto.setCampusId( campus.getId() );
			dto.setCampusString( campus.getCodigo() );
			dto.setDisplayText( campus.getCodigo() + "-" + professor.getNome() );

			list.add( dto );
		}

		return list;
	}

	public static List< ProfessorCampusDTO > toProfessorCampusDTO( Professor professor )
	{
		Set< Campus > campi = professor.getCampi();
		List< ProfessorCampusDTO > list
			= new ArrayList< ProfessorCampusDTO >( campi.size() );

		for ( Campus campus : campi )
		{
			ProfessorCampusDTO dto = new ProfessorCampusDTO();

			if ( campus.getInstituicaoEnsino() != null )
			{
				dto.setInstituicaoEnsinoId( campus.getInstituicaoEnsino().getId() );
				dto.setInstituicaoEnsinoString( campus.getInstituicaoEnsino().getNomeInstituicao() );
			}

			dto.setProfessorId( professor.getId() );
			dto.setProfessorString( professor.getNome() );
			dto.setProfessorCpf( professor.getCpf() );
			dto.setCampusId( campus.getId() );
			dto.setCampusString( campus.getCodigo() );
			dto.setDisplayText( campus.getCodigo() + "-" + professor.getNome() );

			list.add( dto );
		}

		return list;
	}

	// TIPO SALA
	public static TipoSala toTipoSala( TipoSalaDTO dto )
	{
		TipoSala domain = new TipoSala();
		
		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setInstituicaoEnsino( instituicaoEnsino );
		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setNome( dto.getNome() );
		domain.setDescricao( dto.getDescricao() );

		return domain;
	}

	public static TipoSalaDTO toTipoSalaDTO( TipoSala domain )
	{
		TipoSalaDTO dto = new TipoSalaDTO();
		
		InstituicaoEnsino instituicaoEnsino
			= domain.getInstituicaoEnsino();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setNome( domain.getNome() );
		dto.setDescricao( domain.getDescricao() );
		dto.setDisplayText( domain.getNome() + " (" + domain.getDescricao() + ")" );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		return dto;
	}

	// SALA
	public static Collection< SalaDTO > toSalaDTO( Collection< Sala > domains )
	{
		Collection< SalaDTO > list
			= new ArrayList< SalaDTO >( domains.size() );

		for ( Sala domain : domains )
		{
			list.add( ConvertBeans.toSalaDTO( domain ) );
		}

		return list;
	}

	public static Sala toSala( SalaDTO dto )
	{
		Sala domain = new Sala();

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setCodigo( dto.getCodigo() );
		domain.setNumero( dto.getNumero() );
		domain.setAndar( dto.getAndar() );
		domain.setCapacidade( dto.getCapacidade() );

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );  

		TipoSala tipoSala = TipoSala.find(
			dto.getTipoId(), instituicaoEnsino );
		domain.setTipoSala( tipoSala );

		Unidade unidade = Unidade.find(
			dto.getUnidadeId(), instituicaoEnsino );
		domain.setUnidade( unidade );

		return domain;
	}

	public static SalaDTO toSalaDTO( Sala domain )
	{
		SalaDTO dto = new SalaDTO();

		InstituicaoEnsino instituicaoEnsino
			= domain.getTipoSala().getInstituicaoEnsino(); 

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCodigo( domain.getCodigo() );
		dto.setNumero( domain.getNumero() );
		dto.setAndar( domain.getAndar() );
		dto.setCapacidade( domain.getCapacidade() );
		dto.setTipoId( domain.getTipoSala().getId() );
		dto.setTipoString( domain.getTipoSala().getNome() );
		dto.setCampusId( domain.getUnidade().getCampus().getId() );
		dto.setUnidadeId( domain.getUnidade().getId() );
		dto.setUnidadeString( domain.getUnidade().getCodigo() );
		dto.setContainsCurriculoDisciplina(
			domain.getContainsCurriculoDisciplina( instituicaoEnsino ) );
		dto.setDisplayText( domain.getCodigo() + " (" + domain.getNumero() + ")" );
		
		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		return dto;
	}

	// GRUPO DE SALA
	public static GrupoSala toGrupoSala( GrupoSalaDTO dto )
	{
		GrupoSala domain = new GrupoSala();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setCodigo( dto.getCodigo() );
		domain.setNome( dto.getNome() );

		Unidade unidade = Unidade.find(
			dto.getUnidadeId(), instituicaoEnsino );
		domain.setUnidade( unidade );

		return domain;
	}

	public static GrupoSalaDTO toGrupoSalaDTO( GrupoSala domain )
	{
		GrupoSalaDTO dto = new GrupoSalaDTO();

		InstituicaoEnsino instituicaoEnsino
			= domain.getUnidade().getCampus().getInstituicaoEnsino(); 

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCodigo( domain.getCodigo() );
		dto.setNome( domain.getNome() );
		dto.setUnidadeId( domain.getUnidade().getId() );
		dto.setUnidadeString( domain.getUnidade().getCodigo() );
		dto.setCampusId( domain.getUnidade().getCampus().getId() );
		dto.setDisplayText( domain.getNome() );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		return dto;
	}

	// TURNO
	public static Collection< TurnoDTO > toTurnoDTO(
		Collection< Turno > domains )
	{
		Collection< TurnoDTO > list
			= new ArrayList< TurnoDTO >( domains.size() );

		for ( Turno domain : domains )
		{
			list.add( ConvertBeans.toTurnoDTO( domain ) );
		}

		return list;
	}

	public static Turno toTurno( TurnoDTO dto )
	{
		Turno domain = new Turno();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );

		Cenario cenario = Cenario.find(
			dto.getCenarioId(), instituicaoEnsino );

		domain.setCenario( cenario );
		domain.setNome( dto.getNome() );
		domain.setTempo( dto.getTempo() );
		domain.setInstituicaoEnsino( instituicaoEnsino );

		return domain;
	}

	public static TurnoDTO toTurnoDTO( Turno domain )
	{
		TurnoDTO dto = new TurnoDTO();

		InstituicaoEnsino instituicaoEnsino
			= domain.getInstituicaoEnsino();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCenarioId( domain.getCenario().getId() );
		dto.setNome( domain.getNome() );
		dto.setTempo( domain.getTempo() );
		dto.setMaxCreditos( domain.getHorariosAula().size() );

		Set< HorarioAula > horarios = domain.getHorariosAula();

		Map< Integer, Integer > countHorariosAula
			= new HashMap< Integer, Integer >();

		for ( HorarioAula ha : horarios )
		{
			for ( HorarioDisponivelCenario hdc : ha.getHorariosDisponiveisCenario() )
			{
				int semanaInt = Semanas.toInt( hdc.getSemana() );
				Integer value = countHorariosAula.get( semanaInt );
				value = ( ( value == null ) ? 0 : value );

				countHorariosAula.put( semanaInt, value + 1 );
			}
		}

		dto.setCountHorariosAula( countHorariosAula );

		Map< Long, String > horariosStringMap
			= new HashMap< Long, String >();

		Map< Long, Date > horariosInicioMap
			= new HashMap< Long, Date >();

		for ( HorarioAula ha : horarios )
		{
			horariosStringMap.put( ha.getId(),
				dateToString( ha.getHorario(), ha.getTurno().getTempo() ) );

			horariosInicioMap.put( ha.getId(), ha.getHorario() );
		}

		dto.setHorariosStringMap( horariosStringMap );
		dto.setHorariosInicioMap( horariosInicioMap );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		dto.setDisplayText( domain.getNome() + " (" + domain.getTempo() + "min)" );

		return dto;
	}

	// CALENDÁRIO
	public static SemanaLetiva toSemanaLetiva( SemanaLetivaDTO dto )
	{
		SemanaLetiva domain = new SemanaLetiva();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setCodigo( dto.getCodigo() );
		domain.setDescricao( dto.getDescricao() );
		domain.setInstituicaoEnsino( instituicaoEnsino );
		domain.setOficial( dto.getOficial() );

		return domain;
	}

	public static SemanaLetivaDTO toSemanaLetivaDTO( SemanaLetiva domain )
	{
		SemanaLetivaDTO dto = new SemanaLetivaDTO();

		InstituicaoEnsino instituicaoEnsino
			= domain.getInstituicaoEnsino();

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCodigo( domain.getCodigo() );
		dto.setDescricao( domain.getDescricao() );
		dto.setOficial( domain.getOficial() );
		dto.setDisplayText( domain.getCodigo() + " (" + domain.getDescricao() + ")" );

		return dto;
	}

	// HORARIO DE AULA
	public static HorarioAula toHorarioAula( HorarioAulaDTO dto )
	{
		HorarioAula domain = new HorarioAula();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );

		SemanaLetiva calendario = SemanaLetiva.find(
			dto.getSemanaLetivaId(), instituicaoEnsino );
		domain.setSemanaLetiva( calendario );

		Turno turno = Turno.find(
			dto.getTurnoId(), instituicaoEnsino );
		domain.setTurno( turno );
		domain.setHorario( dto.getInicio() );

		return domain;
	}

	public static HorarioAulaDTO toHorarioAulaDTO( HorarioAula domain )
	{
		HorarioAulaDTO dto = new HorarioAulaDTO();

		InstituicaoEnsino instituicaoEnsino
			= domain.getTurno().getInstituicaoEnsino();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setSemanaLetivaId( domain.getSemanaLetiva().getId() );
		dto.setSemanaLetivaString( domain.getSemanaLetiva().getCodigo() );

		Turno turno = domain.getTurno();
		dto.setTurnoId( turno.getId() );
		dto.setTurnoString(turno.getNome() );

		dto.setInicio( domain.getHorario() );

		Calendar fimCal = Calendar.getInstance();
		fimCal.setTime( domain.getHorario() );
		fimCal.add( Calendar.MINUTE, turno.getTempo() );
		dto.setFim( fimCal.getTime() );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		dto.setDisplayText(domain.getHorario().toString());

		return dto;
	}

	// HORARIO DISPONÍVEL CENÁRIO
	public static List< HorarioDisponivelCenario > toHorarioDisponivelCenario(
		List< HorarioDisponivelCenarioDTO > listDTO )
	{
		List< HorarioDisponivelCenario > listDomain
			= new ArrayList< HorarioDisponivelCenario >();

		for ( HorarioDisponivelCenarioDTO dto : listDTO )
		{
			InstituicaoEnsino instituicaoEnsino
				= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

			HorarioAula horarioAula = HorarioAula.find(
				dto.getHorarioDeAulaId(), instituicaoEnsino );

			HorarioDisponivelCenario domain = null;

			if ( dto.getSegunda() )
			{
				domain = null;
				if ( dto.getSegundaId() == null )
				{
					domain = new HorarioDisponivelCenario();
					domain.setSemana( Semanas.SEG );
					domain.setHorarioAula( horarioAula );
				}
				else
				{
					domain = HorarioDisponivelCenario.find(
						dto.getSegundaId(), instituicaoEnsino );
				}

				if ( domain != null )
				{
					listDomain.add( domain );
				}
			}

			if ( dto.getTerca() )
			{
				domain = null;
				if ( dto.getTercaId() == null )
				{
					domain = new HorarioDisponivelCenario();
					domain.setSemana( Semanas.TER );
					domain.setHorarioAula( horarioAula );
				}
				else
				{
					domain = HorarioDisponivelCenario.find(
						dto.getTercaId(), instituicaoEnsino );
				}

				if ( domain != null )
				{
					listDomain.add( domain );
				}
			}

			if ( dto.getQuarta() )
			{
				domain = null;
				if ( dto.getQuartaId() == null )
				{
					domain = new HorarioDisponivelCenario();
					domain.setSemana( Semanas.QUA );
					domain.setHorarioAula( horarioAula );
				}
				else
				{
					domain = HorarioDisponivelCenario.find(
						dto.getQuartaId(), instituicaoEnsino );
				}

				if ( domain != null )
				{
					listDomain.add( domain );
				}
			}

			if ( dto.getQuinta() )
			{
				domain = null;
				if ( dto.getQuintaId() == null )
				{
					domain = new HorarioDisponivelCenario();
					domain.setSemana( Semanas.QUI );
					domain.setHorarioAula( horarioAula );
				}
				else
				{
					domain = HorarioDisponivelCenario.find(
						dto.getQuintaId(), instituicaoEnsino );
				}

				if ( domain != null )
				{
					listDomain.add( domain );
				}
			}

			if ( dto.getSexta() )
			{
				domain = null;
				if ( dto.getSextaId() == null )
				{
					domain = new HorarioDisponivelCenario();
					domain.setSemana( Semanas.SEX );
					domain.setHorarioAula( horarioAula );
				}
				else
				{
					domain = HorarioDisponivelCenario.find(
						dto.getSextaId(), instituicaoEnsino );
				}

				if ( domain != null )
				{
					listDomain.add( domain );
				}
			}

			if ( dto.getSabado() )
			{
				domain = null;
				if ( dto.getSabadoId() == null )
				{
					domain = new HorarioDisponivelCenario();
					domain.setSemana( Semanas.SAB );
					domain.setHorarioAula( horarioAula );
				}
				else
				{
					domain = HorarioDisponivelCenario.find(
						dto.getSabadoId(), instituicaoEnsino );
				}

				if ( domain != null )
				{
					listDomain.add( domain );
				}
			}

			if ( dto.getDomingo() )
			{
				domain = null;
				if ( dto.getDomingoId() == null )
				{
					domain = new HorarioDisponivelCenario();
					domain.setSemana( Semanas.DOM );
					domain.setHorarioAula( horarioAula );
				}
				else
				{
					domain = HorarioDisponivelCenario.find(
						dto.getDomingoId(), instituicaoEnsino );
				}

				if ( domain != null )
				{
					listDomain.add( domain );
				}
			}
		}

		return listDomain;
	}

	public static HorarioDisponivelCenarioDTO toHorarioDisponivelCenarioDTO(
			HorarioAula domain) {

		HorarioDisponivelCenarioDTO dto = new HorarioDisponivelCenarioDTO();

		dto.setHorarioDeAulaId( domain.getId() );
		dto.setHorarioDeAulaVersion( domain.getVersion() );
		dto.setTurnoString( domain.getTurno().getNome() );

		dto.setSegunda( false );
		dto.setTerca( false );
		dto.setQuarta( false );
		dto.setQuinta( false );
		dto.setSexta( false );
		dto.setSabado( false );
		dto.setDomingo( false );

		for ( HorarioDisponivelCenario o : domain.getHorariosDisponiveisCenario() )
		{
			switch ( o.getSemana() )
			{
			case SEG:
				dto.setSegunda(true);
				dto.setSegundaId(o.getId());
				break;
			case TER:
				dto.setTerca(true);
				dto.setTercaId(o.getId());
				break;
			case QUA:
				dto.setQuarta(true);
				dto.setQuartaId(o.getId());
				break;
			case QUI:
				dto.setQuinta(true);
				dto.setQuintaId(o.getId());
				break;
			case SEX:
				dto.setSexta(true);
				dto.setSextaId(o.getId());
				break;
			case SAB:
				dto.setSabado(true);
				dto.setSabadoId(o.getId());
				break;
			case DOM:
				dto.setDomingo(true);
				dto.setDomingoId(o.getId());
				break;
			default:
				break;
			}
		}

		dto.setHorarioString( ConvertBeans.dateToString(
			domain.getHorario(), domain.getTurno().getTempo() ) );

		dto.setHorario( domain.getHorario() );

		if ( domain.getSemanaLetiva() != null
			&& domain.getSemanaLetiva().getInstituicaoEnsino() != null )
		{
			dto.setInstituicaoEnsinoId( domain.getSemanaLetiva().getInstituicaoEnsino().getId() );
			dto.setInstituicaoEnsinoString( domain.getSemanaLetiva().getInstituicaoEnsino().getNomeInstituicao() );
		}

		dto.setDisplayText( dto.getHorarioString() );

		return dto;
	}

	public static List< HorarioDisponivelCenarioDTO > toHorarioDisponivelCenarioDTO(
		List< HorarioDisponivelCenario > domains )
	{
		Map< HorarioAula, List< HorarioDisponivelCenario > > horariosAula
			= new HashMap< HorarioAula, List< HorarioDisponivelCenario > >();

		for ( HorarioDisponivelCenario domain : domains )
		{
			if ( !horariosAula.containsKey( domain.getHorarioAula() ) )
			{
				horariosAula.put( domain.getHorarioAula(),
					new ArrayList< HorarioDisponivelCenario >() );
			}

			horariosAula.get( domain.getHorarioAula() ).add( domain );
		}

		List< HorarioDisponivelCenarioDTO > dtos
			= new ArrayList< HorarioDisponivelCenarioDTO >( horariosAula.keySet().size() );

		for ( HorarioAula horarioAula : horariosAula.keySet() )
		{
			HorarioDisponivelCenarioDTO dto = new HorarioDisponivelCenarioDTO();

			InstituicaoEnsino instituicaoEnsino = null;
			if ( horarioAula.getSemanaLetiva() != null )
			{
				instituicaoEnsino = horarioAula.getSemanaLetiva().getInstituicaoEnsino();
			}

			if ( instituicaoEnsino != null )
			{
				dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
				dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
			}

			dto.setHorarioDeAulaId( horarioAula.getId() );
			dto.setHorarioDeAulaVersion( horarioAula.getVersion() );
			dto.setTurnoString( horarioAula.getTurno().getNome() );

			dto.setSegunda( false );
			dto.setTerca( false );
			dto.setQuarta( false );
			dto.setQuinta( false );
			dto.setSexta( false );
			dto.setSabado( false );
			dto.setDomingo(false );

			for ( HorarioDisponivelCenario o : horariosAula.get( horarioAula ) )
			{
				switch ( o.getSemana() )
				{
				case SEG:
					dto.setSegunda(true);
					dto.setSegundaId(o.getId());
					break;
				case TER:
					dto.setTerca(true);
					dto.setTercaId(o.getId());
					break;
				case QUA:
					dto.setQuarta(true);
					dto.setQuartaId(o.getId());
					break;
				case QUI:
					dto.setQuinta(true);
					dto.setQuintaId(o.getId());
					break;
				case SEX:
					dto.setSexta(true);
					dto.setSextaId(o.getId());
					break;
				case SAB:
					dto.setSabado(true);
					dto.setSabadoId(o.getId());
					break;
				case DOM:
					dto.setDomingo(true);
					dto.setDomingoId(o.getId());
					break;
				default:
					break;
				}
			}

			DateFormat df = new SimpleDateFormat( "HH:mm" );
			String inicio = df.format( horarioAula.getHorario() );

			Calendar fimCal = Calendar.getInstance();
			fimCal.setTime( horarioAula.getHorario() );
			fimCal.add( Calendar.MINUTE, horarioAula.getTurno().getTempo() );
			String fim = df.format( fimCal.getTime() );

			dto.setHorarioString( inicio + " / " + fim );
			dto.setHorario( horarioAula.getHorario() );
			dto.setDisplayText( dto.getHorarioString() );

			dtos.add( dto );
		}

		return dtos;
	}

	// TIPO CURSO
	public static TipoCurso toTipoCurso( TipoCursoDTO dto )
	{
		TipoCurso domain = new TipoCurso();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setCodigo( dto.getCodigo() );
		domain.setDescricao( dto.getDescricao() );
		domain.setInstituicaoEnsino( instituicaoEnsino );

		return domain;
	}

	public static TipoCursoDTO toTipoCursoDTO( TipoCurso domain )
	{
		TipoCursoDTO dto = new TipoCursoDTO();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCodigo( domain.getCodigo() );
		dto.setDescricao( domain.getDescricao() );
		dto.setInstituicaoEnsinoId( domain.getInstituicaoEnsino().getId() );
		dto.setInstituicaoEnsinoString( domain.getInstituicaoEnsino().getNomeInstituicao() );
		dto.setDisplayText( domain.getCodigo() + " (" + domain.getDescricao() + ")" );

		return dto;
	}

	// CURSO
	public static Curso toCurso( CursoDTO dto )
	{
		Curso domain = new Curso();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
	
		Cenario cenario = Cenario.find(
			dto.getCenarioId(), instituicaoEnsino );

		domain.setCenario( cenario );
		domain.setCodigo( dto.getCodigo() );
		domain.setNome( dto.getNome() );
		domain.setNumMinDoutores( dto.getNumMinDoutores() );
		domain.setNumMinMestres( dto.getNumMinMestres() );
		domain.setMinTempoIntegralParcial( dto.getMinTempoIntegralParcial() );
		domain.setMinTempoIntegral( dto.getMinTempoIntegral() );
		domain.setMaxDisciplinasPeloProfessor( dto.getMaxDisciplinasPeloProfessor() );
		domain.setAdmMaisDeUmDisciplina( dto.getAdmMaisDeUmDisciplina() );

		TipoCurso tipoCurso = TipoCurso.find(
			dto.getTipoId(), instituicaoEnsino );
		domain.setTipoCurso( tipoCurso );

		return domain;
	}

	public static CursoDTO toCursoDTO( Curso domain )
	{
		CursoDTO dto = new CursoDTO();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCenarioId( domain.getCenario().getId() );
		dto.setCodigo( domain.getCodigo() );
		dto.setNome( domain.getNome() );
		dto.setNumMinDoutores( domain.getNumMinDoutores() );
		dto.setNumMinMestres( domain.getNumMinMestres() );
		dto.setMinTempoIntegralParcial( domain.getMinTempoIntegralParcial() );
		dto.setMinTempoIntegral( domain.getMinTempoIntegral() );
		dto.setMaxDisciplinasPeloProfessor( domain.getMaxDisciplinasPeloProfessor() );
		dto.setAdmMaisDeUmDisciplina( domain.getAdmMaisDeUmDisciplina() );
		dto.setTipoId( domain.getTipoCurso().getId() );
		dto.setTipoString( domain.getTipoCurso().getCodigo() );

		if ( domain.getTipoCurso() != null
			&& domain.getTipoCurso().getInstituicaoEnsino() != null )
		{
			dto.setInstituicaoEnsinoId( domain.getTipoCurso().getInstituicaoEnsino().getId() );
			dto.setInstituicaoEnsinoString( domain.getTipoCurso().getInstituicaoEnsino().getNomeInstituicao() );
		}

		dto.setDisplayText( domain.getNome() + " (" + domain.getCodigo() + ")" );

		return dto;
	}

	// ÁREA DE TITULAÇÂO
	public static AreaTitulacao toAreaTitulacao( AreaTitulacaoDTO dto )
	{
		AreaTitulacao domain = new AreaTitulacao();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setCodigo( dto.getCodigo() );
		domain.setDescricao( dto.getDescricao() );

		return domain;
	}

	public static AreaTitulacaoDTO toAreaTitulacaoDTO( AreaTitulacao domain )
	{
		Long instituicaoEnsinoId = null;
		String instituicaoEnsinoString = "";
		
		if ( domain.getInstituicaoEnsino() != null )
		{
			instituicaoEnsinoId = domain.getInstituicaoEnsino().getId();
			instituicaoEnsinoString = domain.getInstituicaoEnsino().getNomeInstituicao();
		}

		AreaTitulacaoDTO dto = new AreaTitulacaoDTO(
			domain.getId(), domain.getCodigo(), domain.getDescricao(),
			domain.getVersion(), instituicaoEnsinoId, instituicaoEnsinoString );

		return dto;
	}

	// TITULAÇÃO
	public static Titulacao toTitulacao( TitulacaoDTO dto )
	{
		Titulacao domain = new Titulacao();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setInstituicaoEnsino( instituicaoEnsino );
		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setNome( dto.getNome() );

		return domain;
	}

	public static TitulacaoDTO toTitulacaoDTO( Titulacao domain )
	{
		TitulacaoDTO dto = new TitulacaoDTO();

		InstituicaoEnsino instituicaoEnsino
			= domain.getInstituicaoEnsino();

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setNome( domain.getNome() );
		dto.setDisplayText( domain.getNome() );

		return dto;
	}


	// TIPO DE CONTRATO
	public static TipoContrato toTipoContrato( TipoContratoDTO dto )
	{
		TipoContrato domain = new TipoContrato();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setInstituicaoEnsino( instituicaoEnsino );
		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setNome( dto.getNome() );

		return domain;
	}

	public static TipoContratoDTO toTipoContratoDTO( TipoContrato domain )
	{
		TipoContratoDTO dto = new TipoContratoDTO();

		InstituicaoEnsino instituicaoEnsino
			= domain.getInstituicaoEnsino();

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setNome( domain.getNome() );
		dto.setDisplayText( domain.getNome() );

		return dto;
	}

	// DESLOCAMENTO UNIDADES
	public static List< DeslocamentoUnidade > toDeslocamentoUnidade(
		DeslocamentoUnidadeDTO deslocamentoUnidadeDTO )
	{
		List< Long > listIds = new ArrayList< Long >();

		for ( String keyString : deslocamentoUnidadeDTO.getPropertyNames() )
		{
			if ( keyString.startsWith( "destinoTempo" ) )
			{
				Long id = Long.valueOf(
					keyString.replace( "destinoTempo", "" ) );

				if ( !listIds.contains( id ) )
				{
					listIds.add( id );
				}
			}
			else if ( keyString.startsWith( "destinoCusto" ) )
			{
				Long id = 0L;

				if ( !listIds.contains( id ) )
				{
					listIds.add( id );
				}
			}
		}

		InstituicaoEnsino instituicaoEnsino = InstituicaoEnsino.find(
			deslocamentoUnidadeDTO.getInstituicaoEnsinoId() );

		List< DeslocamentoUnidade > list
			= new ArrayList< DeslocamentoUnidade >();

		Unidade unidade = Unidade.find(
			deslocamentoUnidadeDTO.getOrigemId(), instituicaoEnsino );

		for ( Long idUnidade : listIds )
		{
			Integer tempo = deslocamentoUnidadeDTO.getDestinoTempo( idUnidade );
			Double custo = deslocamentoUnidadeDTO.getDestinoCusto( idUnidade );

			if ( tempo <= 0 && custo <= 0.0 )
			{
				continue;
			}

			DeslocamentoUnidade du = new DeslocamentoUnidade();

			du.setDestino( Unidade.find( idUnidade, instituicaoEnsino ) );
			du.setOrigem( unidade );
			du.setTempo( tempo );
			du.setCusto( custo );

			list.add( du );
		}

		return list;
	}


	public static DeslocamentoUnidadeDTO toDeslocamentoUnidadeDTO(
		Unidade unidade, Set< Unidade > unidadesDestinos )
	{
		DeslocamentoUnidadeDTO deslocamentoUnidadeDTO = new DeslocamentoUnidadeDTO();

		deslocamentoUnidadeDTO.setOrigemId( unidade.getId() );
		deslocamentoUnidadeDTO.setOrigemString( unidade.getCodigo() );

		if ( unidade.getCampus().getInstituicaoEnsino() != null )
		{
			deslocamentoUnidadeDTO.setInstituicaoEnsinoId(
				unidade.getCampus().getInstituicaoEnsino().getId() );

			deslocamentoUnidadeDTO.setInstituicaoEnsinoString(
					unidade.getCampus().getInstituicaoEnsino().getNomeInstituicao() );
		}

		Set< DeslocamentoUnidade > set = unidade.getDeslocamentos();

		for ( Unidade u : unidadesDestinos )
		{
			deslocamentoUnidadeDTO.addDestino(
				u.getId(), u.getCodigo(), 0, 0.0 );
		}

		for ( DeslocamentoUnidade du : set )
		{
			deslocamentoUnidadeDTO.addDestino(
				du.getDestino().getId(), du.getDestino().getCodigo(),
				du.getTempo(), du.getCusto() );
		}

		return deslocamentoUnidadeDTO;
	}

	// DESLOCAMENTO CAMPUS
	public static List< DeslocamentoCampus > toDeslocamentoCampus(
		DeslocamentoCampusDTO deslocamentoCampusDTO )
	{
		List< Long > listIds = new ArrayList< Long >();

		for ( String keyString : deslocamentoCampusDTO.getPropertyNames() )
		{
			if ( keyString.startsWith( DeslocamentoCampusDTO.PROPERTY_DESTINO_TEMPO ) )
			{
				Long id = Long.valueOf( keyString.replace(
					DeslocamentoCampusDTO.PROPERTY_DESTINO_TEMPO, "" ) );

				if ( !listIds.contains( id ) )
				{
					listIds.add( id );
				}
			}
			else if ( keyString.startsWith( DeslocamentoCampusDTO.PROPERTY_DESTINO_CUSTO ) )
			{
				Long id = Long.valueOf( keyString.replace(
					DeslocamentoCampusDTO.PROPERTY_DESTINO_CUSTO, "" ) );

				if ( !listIds.contains( id ) )
				{
					listIds.add( id );
				}
			}
		}

		List< DeslocamentoCampus > list
			= new ArrayList< DeslocamentoCampus >();

		InstituicaoEnsino instituicaoEnsino = InstituicaoEnsino.find(
			deslocamentoCampusDTO.getInstituicaoEnsinoId() );

		Campus campus = Campus.find(
			deslocamentoCampusDTO.getOrigemId(), instituicaoEnsino );

		for ( Long idCampus : listIds )
		{
			Integer tempo = deslocamentoCampusDTO.getDestinoTempo( idCampus );
			Double custo = deslocamentoCampusDTO.getDestinoCusto( idCampus );

			if ( tempo <= 0 && custo <= 0.0 )
			{
				continue;
			}

			DeslocamentoCampus du = new DeslocamentoCampus();

			du.setDestino( Campus.find( idCampus, instituicaoEnsino ) );
			du.setOrigem( campus );
			du.setTempo( tempo );
			du.setCusto( custo );

			list.add( du );
		}

		return list;
	}


	public static DeslocamentoCampusDTO toDeslocamentoCampusDTO(
		Campus campus, List< Campus > campusDestinos )
	{
		DeslocamentoCampusDTO deslocamentoCampusDTO = new DeslocamentoCampusDTO();

		deslocamentoCampusDTO.setOrigemId( campus.getId() );
		deslocamentoCampusDTO.setOrigemString( campus.getCodigo() );

		Set< DeslocamentoCampus > set = campus.getDeslocamentos();

		for ( Campus u : campusDestinos )
		{
			deslocamentoCampusDTO.addDestino(
				u.getId(), u.getCodigo(), 0, 0.0 );
		}

		for ( DeslocamentoCampus du : set )
		{
			deslocamentoCampusDTO.addDestino(
				du.getDestino().getId(), du.getDestino().getCodigo(),
				du.getTempo(), du.getCusto() );
		}

		if ( campus.getInstituicaoEnsino() != null )
		{
			deslocamentoCampusDTO.setInstituicaoEnsinoId(
				campus.getInstituicaoEnsino().getId() );

			deslocamentoCampusDTO.setInstituicaoEnsinoString(
				campus.getInstituicaoEnsino().getNomeInstituicao() );
		}

		deslocamentoCampusDTO.setDisplayText( campus.getCodigo() );

		return deslocamentoCampusDTO;
	}

	// TIPO DISCIPLINA
	public static TipoDisciplina toTipoDisciplina( TipoDisciplinaDTO dto )
	{
		TipoDisciplina domain = new TipoDisciplina();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setInstituicaoEnsino( instituicaoEnsino );
		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setNome( dto.getNome() );

		return domain;
	}

	public static TipoDisciplinaDTO toTipoDisciplinaDTO( TipoDisciplina domain )
	{
		TipoDisciplinaDTO dto = new TipoDisciplinaDTO();

		InstituicaoEnsino instituicaoEnsino
			= domain.getInstituicaoEnsino();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setNome( domain.getNome() );
		dto.setDisplayText( domain.getNome() );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		return dto;
	}

	// DISCIPLINA
	public static Disciplina toDisciplina( DisciplinaDTO dto )
	{
		Disciplina domain = new Disciplina();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());

		Cenario cenario = Cenario.find(
			dto.getCenarioId(), instituicaoEnsino );

		domain.setCenario(cenario);
		domain.setCodigo(dto.getCodigo());
		domain.setNome(dto.getNome());
		domain.setCreditosPratico(dto.getCreditosPratico());
		domain.setCreditosTeorico(dto.getCreditosTeorico());
		domain.setLaboratorio(dto.getLaboratorio());
		domain.setMaxAlunosTeorico(dto.getMaxAlunosTeorico());
		domain.setMaxAlunosPratico(dto.getMaxAlunosPratico());
		domain.setDificuldade(Dificuldades.get(dto.getDificuldade()));

		domain.setTipoDisciplina( TipoDisciplina.find(
			dto.getTipoId(), instituicaoEnsino ) );

		return domain;
	}

	public static DisciplinaDTO toDisciplinaDTO( Disciplina domain )
	{
		DisciplinaDTO dto = new DisciplinaDTO();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCenarioId( domain.getCenario().getId() );
		dto.setCodigo( domain.getCodigo() );
		dto.setNome( domain.getNome() );
		dto.setCreditosPratico( domain.getCreditosPratico() );
		dto.setCreditosTeorico( domain.getCreditosTeorico() );
		dto.setLaboratorio( domain.getLaboratorio() );
		dto.setMaxAlunosTeorico( domain.getMaxAlunosTeorico() );
		dto.setMaxAlunosPratico( domain.getMaxAlunosPratico() );
		dto.setDificuldade( domain.getDificuldade().name() );
		dto.setTipoId( domain.getTipoDisciplina().getId() );
		dto.setTipoString( domain.getTipoDisciplina().getNome() );
		dto.setDisplayText( domain.getCodigo() + " (" + domain.getNome() + ")" );

		dto.setInstituicaoEnsinoId(
			domain.getTipoDisciplina().getInstituicaoEnsino().getId() );
		dto.setInstituicaoEnsinoString(
			domain.getTipoDisciplina().getInstituicaoEnsino().getNomeInstituicao() );

		return dto;
	}

	// CURRICULO (MATRIZ CURRICULAR)
	public static Curriculo toCurriculo( CurriculoDTO dto )
	{
		Curriculo domain = new Curriculo();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );

		Cenario cenario = Cenario.find( dto.getCenarioId(), instituicaoEnsino );
		domain.setCenario( cenario );
		domain.setCodigo( dto.getCodigo() );
		domain.setDescricao( dto.getDescricao() );

		Curso curso = Curso.find(
			dto.getCursoId(), instituicaoEnsino );
		domain.setCurso( curso );

		SemanaLetiva sl = SemanaLetiva.find(
			dto.getSemanaLetivaId(), instituicaoEnsino );
		domain.setSemanaLetiva( sl );

		return domain;
	}

	public static CurriculoDTO toCurriculoDTO( Curriculo domain )
	{
		CurriculoDTO dto = new CurriculoDTO();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCenarioId( domain.getId() );
		dto.setCodigo( domain.getCodigo() );
		dto.setDescricao( domain.getDescricao() );

		Set< Integer > periodosSet = new TreeSet< Integer >();
		for ( CurriculoDisciplina cd : domain.getDisciplinas() )
		{
			periodosSet.add( cd.getPeriodo() );
		}

		String periodos = "";
		for ( Integer periodo : periodosSet )
		{
			periodos += ( periodo + ", " );
		}

		periodos = TriedaUtil.truncate( periodos,
			periodos.length() - 2, false );
		dto.setPeriodos( periodos );

		Curso curso = domain.getCurso();
		dto.setCursoId( curso.getId() );
		dto.setCursoString( curso.getCodigo() + " (" + curso.getNome() + ")" );
		dto.setDisplayText( domain.getCodigo() + " (" + curso.getNome()	+ ") Periodos: " + periodos );

		SemanaLetiva sl = domain.getSemanaLetiva(); 
		dto.setSemanaLetivaId( sl == null ? null : sl.getId() );
		dto.setSemanaLetivaString( sl == null ? "" : sl.getCodigo() );

		if ( sl != null && sl.getInstituicaoEnsino() != null )
		{
			dto.setInstituicaoEnsinoId( sl.getInstituicaoEnsino().getId() );
			dto.setInstituicaoEnsinoString( sl.getInstituicaoEnsino().getNomeInstituicao() );
		}

		return dto;
	}

	// CURRICULO DISCIPLINA (DISCIPLINA NA MATRIZ CURRICULAR)
	public static CurriculoDisciplina toCurriculoDisciplina(
		CurriculoDisciplinaDTO dto )
	{
		CurriculoDisciplina domain = new CurriculoDisciplina();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setPeriodo( dto.getPeriodo() );

		Disciplina disciplina = Disciplina.find(
			dto.getDisciplinaId(), instituicaoEnsino );
		domain.setDisciplina( disciplina );

		Curriculo curriculo = Curriculo.find(
			dto.getCurriculoId(), instituicaoEnsino );

		domain.setCurriculo( curriculo );

		return domain;
	}

	public static CurriculoDisciplinaDTO toCurriculoDisciplinaDTO(
		CurriculoDisciplina domain )
	{
		CurriculoDisciplinaDTO dto = new CurriculoDisciplinaDTO();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );

		Disciplina disciplina = domain.getDisciplina();
		dto.setDisciplinaId( disciplina.getId() );
		dto.setDisciplinaString( disciplina.getCodigo() );
		dto.setPeriodo( domain.getPeriodo() );

		Integer crTeorico = disciplina.getCreditosTeorico();
		Integer crPratico = disciplina.getCreditosPratico();
		dto.setCreditosTeorico( crTeorico );
		dto.setCreditosPratico( crPratico );
		dto.setCreditosTotal( crTeorico + crPratico );
		dto.setCurriculoId( domain.getCurriculo().getId() );

		dto.setInstituicaoEnsinoId( domain.getDisciplina().getTipoDisciplina().getInstituicaoEnsino().getId() );
		dto.setInstituicaoEnsinoString( domain.getDisciplina().getTipoDisciplina().getInstituicaoEnsino().getNomeInstituicao() );

		dto.setDisplayText( disciplina.getCodigo() + " (" + disciplina.getNome() + ")" );

		return dto;
	}

	// OFERTA DE CURSO EM UM CAMPUS
	public static Oferta toOferta( OfertaDTO dto )
	{
		Oferta domain = new Oferta();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setCampus( Campus.find( dto.getCampusId(), instituicaoEnsino ) );

		domain.setCurriculo( Curriculo.find(
			dto.getMatrizCurricularId(), instituicaoEnsino ) );

		domain.setCurso( domain.getCurriculo().getCurso() );
		domain.setTurno( Turno.find( dto.getTurnoId(), instituicaoEnsino ) );
		domain.setReceita( dto.getReceita().getDoubleValue() );

		return domain;
	}

	public static OfertaDTO toOfertaDTO( Oferta domain )
	{
		OfertaDTO dto = new OfertaDTO();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCampusString( domain.getCampus().getCodigo() );
		dto.setCampusId( domain.getCampus().getId() );

		Curso curso = domain.getCurriculo().getCurso();
		dto.setCursoString( curso.getCodigo() + " (" + curso.getNome() + ")" );
		dto.setMatrizCurricularString( domain.getCurriculo().getCodigo() );
		dto.setMatrizCurricularId( domain.getCurriculo().getId() );
		dto.setTurnoString( domain.getTurno().getNome() );
		dto.setTurnoId( domain.getTurno().getId() );
		dto.setReceita( TriedaUtil.parseTriedaCurrency( domain.getReceita() ) );

		dto.setInstituicaoEnsinoId(
			domain.getCampus().getInstituicaoEnsino().getId() );
		dto.setInstituicaoEnsinoString(
			domain.getCampus().getInstituicaoEnsino().getNomeInstituicao() );

		dto.setDisplayText( dto.getMatrizCurricularString() + " (" + curso.getNome() + ")" );

		return dto;
	}

	// DEMANDA
	public static Demanda toDemanda( DemandaDTO dto )
	{
		Demanda domain = new Demanda();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		Oferta oferta = Oferta.find( dto.getOfertaId(), instituicaoEnsino );

		if ( oferta != null )
		{
			domain.setOferta( oferta );
		}
		else
		{
			Campus campus = Campus.find( dto.getCampusId(), instituicaoEnsino );
			Curso curso = Curso.find( dto.getCursoId(), instituicaoEnsino );
			Curriculo curriculo = Curriculo.find( dto.getCurriculoId(), instituicaoEnsino );
			Turno turno = Turno.find( dto.getTurnoId(), instituicaoEnsino );

			List< Oferta> ofertas = Oferta.findBy( instituicaoEnsino,
				turno, campus, curso, curriculo, 0, 1, null );

			domain.setOferta( ( ofertas == null || ofertas.size() == 0 ) ? null : ofertas.get( 0 ) );
		}

		domain.setDisciplina( Disciplina.find(
			dto.getDisciplinaId(), instituicaoEnsino ) );
		domain.setQuantidade( dto.getDemanda() );

		return domain;
	}

	public static DemandaDTO toDemandaDTO( Demanda domain )
	{
		DemandaDTO dto = new DemandaDTO();

		InstituicaoEnsino instituicaoEnsino
			= domain.getOferta().getCampus().getInstituicaoEnsino();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );

		Oferta oferta = domain.getOferta();

		Campus campus = oferta.getCampus();
		dto.setCampusString( campus.getCodigo() );
		dto.setCampusId( campus.getId() );

		Curriculo curriculo = oferta.getCurriculo();
		dto.setCurriculoString( curriculo.getCodigo() );
		dto.setCurriculoId( curriculo.getId() );

		Curso curso = curriculo.getCurso();
		dto.setCursoString( curso.getCodigo() + " (" + curso.getNome() + ")" );
		dto.setCursoId( curso.getId() );

		Turno turno = oferta.getTurno();
		dto.setTurnoString( turno.getNome() );
		dto.setTurnoId( turno.getId() );

		Disciplina disciplina = domain.getDisciplina();
		dto.setDisciplinaString( disciplina.getCodigo() );
		dto.setDisciplinaId( disciplina.getId() );

		dto.setCenarioId( disciplina.getCenario().getId() );

		// A quantidade da demandaserá calculada:
		// Quando houver PELO MENOS Um aluno associado à essa demanda,
		// então a quantidade será dada pelo total de alunos associados a ela.
		// Caso contrário, não havendo nenhum aluno associado, o valor exibido
		// para a quantidade será o valor armazenado no banco de dados.
		Integer alunosDemanda = AlunoDemanda.findByDemanda(
			instituicaoEnsino, domain ).size();

		if ( alunosDemanda == 0 )
		{
			dto.setDemanda( domain.getQuantidade() );
			dto.setQuantidadeDemandaEnable( true );
		}
		else
		{
			dto.setDemanda( alunosDemanda );
			dto.setQuantidadeDemandaEnable( false );
		}

		dto.setInstituicaoEnsinoId(
			domain.getOferta().getCampus().getInstituicaoEnsino().getId() );
		dto.setInstituicaoEnsinoString(
			domain.getOferta().getCampus().getInstituicaoEnsino().getNomeInstituicao() );

		dto.setDisplayText( dto.getNaturalKey() );

		return dto;
	}

	// ATENDIMENTO TÁTICO
	public static AtendimentoTatico toAtendimentoTatico( AtendimentoTaticoDTO dto )
	{
		AtendimentoTatico domain = new AtendimentoTatico();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() ); 

		domain.setInstituicaoEnsino( instituicaoEnsino );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );

		domain.setCenario( Cenario.find(
			dto.getCenarioId(), instituicaoEnsino ) );

		domain.setTurma( dto.getTurma() );

		domain.setSala( Sala.find(
			dto.getSalaId(), instituicaoEnsino ) );

		domain.setSemana( Semanas.get( dto.getSemana() ) );

		domain.setOferta( Oferta.find(
			dto.getOfertaId(), instituicaoEnsino ) );

		domain.setDisciplina( Disciplina.find( dto.getDisciplinaId(), instituicaoEnsino ) );
		domain.setQuantidadeAlunos( dto.getQuantidadeAlunos() );
		domain.setCreditosTeorico( dto.getCreditosTeorico() );
		domain.setCreditosPratico( dto.getCreditosPratico() );
		domain.setInstituicaoEnsino( instituicaoEnsino );

		return domain;
	}

	public static AtendimentoTaticoDTO toAtendimentoTaticoDTO( AtendimentoTatico domain )
	{
		AtendimentoTaticoDTO dto = new AtendimentoTaticoDTO();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCenarioId( domain.getCenario().getId() );
		dto.setTurma( domain.getTurma() );
		dto.setCampusId( domain.getSala().getUnidade().getCampus().getId() );
		dto.setCampusString( domain.getSala().getUnidade().getCampus().getCodigo() );
		dto.setUnidadeId( domain.getSala().getUnidade().getId() );
		dto.setUnidadeString( domain.getSala().getUnidade().getCodigo() );
		dto.setSalaId( domain.getSala().getId() );
		dto.setSalaString( domain.getSala().getNumero() );
		dto.setSemana( Semanas.toInt( domain.getSemana() ) );
		dto.setOfertaId( domain.getOferta().getId() );
		dto.setDisciplinaId( domain.getDisciplina().getId() );
		dto.setDisciplinaString( domain.getDisciplina().getCodigo() );
		dto.setDisciplinaNome( domain.getDisciplina().getNome() );
		dto.setQuantidadeAlunos( domain.getQuantidadeAlunos() );
		dto.setQuantidadeAlunosString( domain.getQuantidadeAlunos().toString() );
		dto.setCreditosTeorico( domain.getCreditosTeorico() );
		dto.setCreditosPratico( domain.getCreditosPratico() );
		dto.setCursoString( domain.getOferta().getCurriculo().getCurso().getCodigo() );
		dto.setCursoNome( domain.getOferta().getCurriculo().getCurso().getNome());
		dto.setCursoId( domain.getOferta().getCurriculo().getCurso().getId() );
		dto.setCurricularString( domain.getOferta().getCurriculo().getCodigo() );
		dto.setCurricularId( domain.getOferta().getCurriculo().getId() );

		dto.setPeriodo( domain.getOferta().getCurriculo().getPeriodo(
			domain.getInstituicaoEnsino(), domain.getDisciplina() ) );

		dto.setPeriodoString( String.valueOf( domain.getOferta().getCurriculo().getPeriodo(
			domain.getInstituicaoEnsino(), domain.getDisciplina() ) ) );

		dto.setTotalCreditoDisciplina( domain.getDisciplina().getTotalCreditos() );
		dto.setTurnoId( domain.getOferta().getTurno().getId() );
		dto.setTurnoString( domain.getOferta().getTurno().getNome() );
		dto.setDisplayText( dto.getNaturalKey() );
		dto.setInstituicaoEnsinoId( domain.getInstituicaoEnsino().getId() );
		dto.setInstituicaoEnsinoString( domain.getInstituicaoEnsino().getNomeInstituicao() );
		dto.setCompartilhamentoCursosString( "" );

		return dto;
	}

	public static List < AtendimentoOperacional > toListAtendimentoOperacional(
		List< AtendimentoOperacionalDTO > listDTO )
	{
		if ( listDTO == null )
		{
			return Collections.< AtendimentoOperacional > emptyList();
		}

		List< AtendimentoOperacional > listDomains
			= new ArrayList< AtendimentoOperacional >();

		for ( AtendimentoOperacionalDTO dto : listDTO )
		{
			listDomains.add( ConvertBeans.toAtendimentoOperacional( dto ) );
		}

		return listDomains;
	}

	public static List < AtendimentoTatico > toListAtendimentoTatico(
			List< AtendimentoTaticoDTO > listDTO )
	{
		if ( listDTO == null )
		{
			return Collections.< AtendimentoTatico > emptyList();
		}

		List< AtendimentoTatico > listDomains
			= new ArrayList< AtendimentoTatico >();

		for ( AtendimentoTaticoDTO dto : listDTO )
		{
			listDomains.add( ConvertBeans.toAtendimentoTatico( dto ) );
		}

		return listDomains;
	}

	public static List < AtendimentoOperacionalDTO > toListAtendimentoOperacionalDTO(
		List< AtendimentoOperacional > listDomains )
	{
		if ( listDomains == null )
		{
			return Collections.< AtendimentoOperacionalDTO > emptyList();
		}

		List< AtendimentoOperacionalDTO > listDTOs
			= new ArrayList< AtendimentoOperacionalDTO >();

		for ( AtendimentoOperacional domain : listDomains )
		{
			listDTOs.add( ConvertBeans.toAtendimentoOperacionalDTO( domain ) );
		}

		return listDTOs;
	}

	public static List < AtendimentoTaticoDTO > toListAtendimentoTaticoDTO(
		List< AtendimentoTatico > listDomains )
	{
		if ( listDomains == null )
		{
			return Collections.< AtendimentoTaticoDTO > emptyList();
		}

		List< AtendimentoTaticoDTO > listDTOs
			= new ArrayList< AtendimentoTaticoDTO >();

		for ( AtendimentoTatico domain : listDomains )
		{
			listDTOs.add( ConvertBeans.toAtendimentoTaticoDTO( domain ) );
		}

		return listDTOs;
	}

	// ATENDIMENTO OPERACIONAL
	public static AtendimentoOperacional toAtendimentoOperacional(
		AtendimentoOperacionalDTO dto )
	{
		AtendimentoOperacional domain = new AtendimentoOperacional();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setInstituicaoEnsino( instituicaoEnsino );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setCenario( Cenario.find( dto.getCenarioId(), instituicaoEnsino ) );
		domain.setSala( Sala.find( dto.getSalaId(), instituicaoEnsino ) );

		domain.setHorarioDisponivelCenario( HorarioDisponivelCenario.find(
			dto.getHorarioDisponivelCenarioId(), instituicaoEnsino ) );

		domain.setProfessor( Professor.find(
			dto.getProfessorId(), instituicaoEnsino ) );

		domain.setCreditoTeorico( dto.getCreditoTeoricoBoolean() );

		domain.setOferta( Oferta.find(
			dto.getOfertaId(), instituicaoEnsino ) );

		domain.setDisciplina( Disciplina.find(
			dto.getDisciplinaId(), instituicaoEnsino ) );
		domain.setQuantidadeAlunos( dto.getQuantidadeAlunos() );
		domain.setTurma( dto.getTurma() );

		return domain;
	}

	public static AtendimentoOperacionalDTO toAtendimentoOperacionalDTO(
		AtendimentoOperacional domain )
	{
		AtendimentoOperacionalDTO dto = new AtendimentoOperacionalDTO();

		InstituicaoEnsino instituicaoEnsino = domain.getInstituicaoEnsino();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCenarioId( domain.getCenario().getId() );
		dto.setCampusId( domain.getSala().getUnidade().getCampus().getId() );
		dto.setCampusString( domain.getSala().getUnidade().getCampus().getCodigo() );
		dto.setUnidadeId( domain.getSala().getUnidade().getId() );
		dto.setUnidadeString( domain.getSala().getUnidade().getCodigo() );
		dto.setSalaId( domain.getSala().getId() );
		dto.setSalaString( domain.getSala().getNumero() );

		HorarioDisponivelCenario hdc = domain.getHorarioDisponivelCenario();
		dto.setHorarioDisponivelCenarioId( hdc.getId() );
		dto.setSemana( Semanas.toInt( hdc.getSemana() ) );

		HorarioAula ha = hdc.getHorarioAula();
		dto.setHorarioId( ha.getId() );
		dto.setHorarioString( TriedaUtil.shortTimeString( ha.getHorario() ) );
		dto.setTurnoId( ha.getTurno().getId() );
		dto.setTurnoString( ha.getTurno().getNome() );

		if ( domain.getProfessorVirtual() == null )
		{
			dto.setProfessorId( domain.getProfessor().getId() );
			dto.setProfessorString( domain.getProfessor().getNome() );
		}
		else
		{
			dto.setProfessorVirtualId( domain.getProfessorVirtual().getId() );
			dto.setProfessorVirtualString(
				"Professor Virtual " + domain.getProfessorVirtual().getId() );
		}

		dto.setCreditoTeoricoBoolean( domain.getCreditoTeorico() );
		dto.setOfertaId( domain.getOferta().getId() );
		dto.setDisciplinaId( domain.getDisciplina().getId() );
		dto.setDisciplinaString( domain.getDisciplina().getCodigo() );
		dto.setDisciplinaNome( domain.getDisciplina().getNome() );
		dto.setTotalCreditoDisciplina( domain.getDisciplina().getTotalCreditos() );
		dto.setQuantidadeAlunos( domain.getQuantidadeAlunos() );
		dto.setQuantidadeAlunosString( domain.getQuantidadeAlunos().toString() );
		dto.setTurma( domain.getTurma() );

		dto.setCursoString( domain.getOferta().getCurriculo().getCurso().getCodigo() );
		dto.setCursoNome( domain.getOferta().getCurriculo().getCurso().getNome() );
		dto.setCursoId( domain.getOferta().getCurriculo().getCurso().getId() );
		dto.setCurricularString( domain.getOferta().getCurriculo().getCodigo() );
		dto.setCurricularId( domain.getOferta().getCurriculo().getId() );

		dto.setPeriodo( domain.getOferta().getCurriculo().getPeriodo(
			instituicaoEnsino, domain.getDisciplina() ) );

		dto.setPeriodoString( String.valueOf( domain.getOferta().getCurriculo().getPeriodo(
			instituicaoEnsino, domain.getDisciplina() ) ) );

		dto.setTotalCreditos( domain.getDisciplina().getTotalCreditos() );
		dto.setDisplayText( dto.getNaturalKey() );
		dto.setCompartilhamentoCursosString( "" );

		dto.setInstituicaoEnsinoId( domain.getInstituicaoEnsino().getId() );
		dto.setInstituicaoEnsinoString( domain.getInstituicaoEnsino().getNomeInstituicao() );

		return dto;
	}

	// DIVISÃO DE CREDITO
	public static DivisaoCredito toDivisaoCredito( DivisaoCreditoDTO dto )
	{
		DivisaoCredito domain = new DivisaoCredito();
		
		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );

		if ( dto.getDisciplinaId() == null )
		{
			domain.getCenario().add( Cenario.find(
				dto.getCenarioId(), instituicaoEnsino ) );
		}
		else
		{
			domain.setDisciplina( Disciplina.find(
				dto.getDisciplinaId(), instituicaoEnsino ) );
		}

		domain.setCreditos( dto.getDia1() + dto.getDia2() + dto.getDia3()
			+ dto.getDia4() + dto.getDia5() + dto.getDia6() + dto.getDia7() );

		domain.setDia1( dto.getDia1() );
		domain.setDia2( dto.getDia2() );
		domain.setDia3( dto.getDia3() );
		domain.setDia4( dto.getDia4() );
		domain.setDia5( dto.getDia5() );
		domain.setDia6( dto.getDia6() );
		domain.setDia7( dto.getDia7() );

		return domain;
	}

	public static DivisaoCreditoDTO toDivisaoCreditoDTO( DivisaoCredito domain )
	{
		DivisaoCreditoDTO dto = new DivisaoCreditoDTO();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );

		if ( domain.getCenario().size() > 0 )
		{
			dto.setCenarioId( ( new ArrayList< Cenario >( domain.getCenario() ) ).get( 0 ).getId() );
		}

		Disciplina disciplina = domain.getDisciplina();
		if ( disciplina != null )
		{
			dto.setDisciplinaId( disciplina.getId() );
			dto.setDisciplinaString(
				disciplina.getCodigo() + " (" + disciplina.getNome() + ")" );
		}

		dto.setDia1( domain.getDia1() );
		dto.setDia2( domain.getDia2() );
		dto.setDia3( domain.getDia3() );
		dto.setDia4( domain.getDia4() );
		dto.setDia5( domain.getDia5() );
		dto.setDia6( domain.getDia6() );
		dto.setDia7( domain.getDia7() );
		dto.setTotalCreditos( dto.getDia1() + dto.getDia2() + dto.getDia3()
				+ dto.getDia4() + dto.getDia5() + dto.getDia6() + dto.getDia7() );

		dto.setInstituicaoEnsinoId( disciplina.getTipoDisciplina().getInstituicaoEnsino().getId() );
		dto.setInstituicaoEnsinoString( disciplina.getTipoDisciplina().getInstituicaoEnsino().getNomeInstituicao() );

		return dto;
	}

	// PROFESSOR
	public static Professor toProfessor( ProfessorDTO dto )
	{
		Professor domain = new Professor();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setCenario( Cenario.find( dto.getCenarioId(), instituicaoEnsino ) );
		domain.setNome( dto.getNome() );
		domain.setCpf( dto.getCpf() );
		domain.setTipoContrato( TipoContrato.find(
			dto.getTipoContratoId(), instituicaoEnsino ) );
		domain.setCargaHorariaMax( dto.getCargaHorariaMax() );
		domain.setCargaHorariaMin( dto.getCargaHorariaMin() );
		domain.setTitulacao( Titulacao.find( dto.getTitulacaoId(), instituicaoEnsino ) );
		domain.setAreaTitulacao( AreaTitulacao.find(
			dto.getAreaTitulacaoId(), instituicaoEnsino ) );
		domain.setCreditoAnterior( dto.getCreditoAnterior() );
		domain.setValorCredito( dto.getValorCredito().getDoubleValue() );

		return domain;
	}

	public static ProfessorDTO toProfessorDTO( Professor domain )
	{
		ProfessorDTO dto = new ProfessorDTO();

		InstituicaoEnsino instituicaoEnsino = null;
		if ( domain != null
			&& domain.getTipoContrato() != null )
		{
			instituicaoEnsino = domain.getTipoContrato().getInstituicaoEnsino();
		}

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCenarioId( domain.getCenario().getId() );
		dto.setNome( domain.getNome() );
		dto.setCpf( domain.getCpf() );
		dto.setTipoContratoId( domain.getTipoContrato().getId() );
		dto.setTipoContratoString( domain.getTipoContrato().getNome() );
		dto.setCargaHorariaMax( domain.getCargaHorariaMax() );
		dto.setCargaHorariaMin( domain.getCargaHorariaMin() );
		dto.setTitulacaoId( domain.getTitulacao().getId() );
		dto.setTitulacaoString( domain.getTitulacao().getNome() );
		dto.setAreaTitulacaoId( domain.getAreaTitulacao().getId() );
		dto.setAreaTitulacaoString( domain.getAreaTitulacao().getCodigo() );
		dto.setCreditoAnterior( domain.getCreditoAnterior() );
		dto.setValorCredito( TriedaUtil.parseTriedaCurrency( domain.getValorCredito() ) );
		dto.setDisplayText( domain.getNome() + " (" + domain.getCpf() + ")" );
		dto.setNotaDesempenho( ConvertBeans.getNotaDesempenho( domain ).intValue() );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		return dto;
	}

	static private Double getNotaDesempenho( Professor p )
	{
		if ( p == null || p.getDisciplinas() == null
			|| p.getDisciplinas().size() <= 0 )
		{
			return 0.0;
		}

		Double average = 0.0;
		for ( ProfessorDisciplina pd : p.getDisciplinas() )
		{
			average += pd.getNota();
		}

		average /= p.getDisciplinas().size();
		return average;
	}
	
	// PROFESSOR-DISCIPLINA
	public static ProfessorDisciplina toProfessorDisciplina(
		ProfessorDisciplinaDTO dto )
	{
		ProfessorDisciplina domain = new ProfessorDisciplina();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setProfessor( Professor.find(
			dto.getProfessorId(), instituicaoEnsino ) );

		domain.setDisciplina( Disciplina.find(
			dto.getDisciplinaId(), instituicaoEnsino ) );

		domain.setPreferencia( dto.getPreferencia() );
		domain.setNota( dto.getNotaDesempenho() );

		return domain;
	}

	public static ProfessorDisciplinaDTO toProfessorDisciplinaDTO(
		ProfessorDisciplina domain )
	{
		ProfessorDisciplinaDTO dto = new ProfessorDisciplinaDTO();

		InstituicaoEnsino instituicaoEnsino
			= domain.getProfessor().getTipoContrato().getInstituicaoEnsino();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setProfessorId( domain.getProfessor().getId() );
		dto.setProfessorString( domain.getProfessor().getNome() );
		dto.setProfessorCpf( domain.getProfessor().getCpf() );
		dto.setDisciplinaId( domain.getDisciplina().getId() );
		dto.setDisciplinaString( domain.getDisciplina().getCodigo() );
		dto.setPreferencia( domain.getPreferencia() );
		dto.setNotaDesempenho( domain.getNota() );

		dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
		dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );

		dto.setDisplayText( domain.getDisciplina().getCodigo() +
			"-" + domain.getProfessor().getNome() );

		return dto;
	}

	// PROFESSOR VIRTUAL
	public static ProfessorVirtual toProfessorVirtual(
		ProfessorVirtualDTO dto )
	{
		ProfessorVirtual domain = new ProfessorVirtual();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setInstituicaoEnsino( instituicaoEnsino );
		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setCargaHorariaMax( dto.getCargaHorariaMax() );
		domain.setCargaHorariaMin( dto.getCargaHorariaMin() );

		domain.setTitulacao( Titulacao.find(
			dto.getTitulacaoId(), instituicaoEnsino ) );

		domain.setAreaTitulacao( AreaTitulacao.find(
			dto.getAreaTitulacaoId(), instituicaoEnsino ) );

		return domain;
	}

	public static ProfessorVirtualDTO toProfessorVirtualDTO(
		ProfessorVirtual domain )
	{
		ProfessorVirtualDTO dto = new ProfessorVirtualDTO();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setNome( "Virtual " + domain.getId() );
		dto.setCargaHorariaMax( domain.getCargaHorariaMax() );
		dto.setCargaHorariaMin( domain.getCargaHorariaMin() );

		Titulacao titulacao = domain.getTitulacao();
		if ( titulacao != null )
		{
			dto.setTitulacaoId( titulacao.getId() );
			dto.setTitulacaoString( titulacao.getNome() );
		}

		AreaTitulacao areaTitulacao = domain.getAreaTitulacao();
		if ( areaTitulacao != null )
		{
			dto.setAreaTitulacaoId( areaTitulacao.getId() );
			dto.setAreaTitulacaoString( areaTitulacao.getCodigo() );
		}

		if ( domain.getInstituicaoEnsino() != null )
		{
			dto.setInstituicaoEnsinoId( domain.getInstituicaoEnsino().getId() );
			dto.setInstituicaoEnsinoString( domain.getInstituicaoEnsino().getNomeInstituicao() );
		}

		return dto;
	}

	// EQUIVALENCIA
	public static Equivalencia toEquivalencia( EquivalenciaDTO dto )
	{
		Equivalencia domain = new Equivalencia();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setCursou( Disciplina.find(
			dto.getCursouId(), instituicaoEnsino ) );

		return domain;
	}

	public static EquivalenciaDTO toEquivalenciaDTO( Equivalencia domain )
	{
		EquivalenciaDTO dto = new EquivalenciaDTO();

		InstituicaoEnsino instituicaoEnsino
			= domain.getCursou().getTipoDisciplina().getInstituicaoEnsino();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCursouId( domain.getCursou().getId() );
		dto.setCursouString( domain.getCursou().getCodigo() );

		Set< Disciplina > eliminaList
			= new TreeSet< Disciplina >( domain.getElimina() );

		String eliminaString = "";
		for ( Disciplina d : eliminaList )
		{
			eliminaString += ( d.getCodigo() + "; " );
		}

		if ( eliminaString.length() > 0 )
		{
			eliminaString = eliminaString.substring(
				0, eliminaString.length() - 2 );
		}

		dto.setEliminaString(eliminaString);

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		dto.setDisplayText( dto.getNaturalKey() );

		return dto;
	}

	// FIXACAO
	public static Fixacao toFixacao( FixacaoDTO dto )
	{
		Fixacao domain = new Fixacao();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setInstituicaoEnsino( instituicaoEnsino );
		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setCodigo( dto.getCodigo() );
		domain.setDescricao( dto.getDescricao() );

		if ( dto.getProfessorId() != null )
		{
			domain.setProfessor( Professor.find(
				dto.getProfessorId(), instituicaoEnsino ) );
		}

		if ( dto.getDisciplinaId() != null )
		{
			domain.setDisciplina( Disciplina.find(
				dto.getDisciplinaId(), instituicaoEnsino ) );
		}

		if ( dto.getCampusId() != null )
		{
			domain.setCampus( Campus.find(
				dto.getCampusId(), instituicaoEnsino ) );
		}

		if ( dto.getUnidadeId() != null )
		{
			domain.setUnidade( Unidade.find(
				dto.getUnidadeId(), instituicaoEnsino ) );
		}

		if ( dto.getSalaId() != null )
		{
			domain.setSala( Sala.find(
				dto.getSalaId(), instituicaoEnsino ) );
		}

		return domain;
	}

	public static FixacaoDTO toFixacaoDTO( Fixacao domain )
	{
		FixacaoDTO dto = new FixacaoDTO();

		InstituicaoEnsino instituicaoEnsino
			= domain.getInstituicaoEnsino();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCodigo( domain.getCodigo() );
		dto.setDescricao( domain.getDescricao() );

		Professor professor = domain.getProfessor();
		if ( professor != null )
		{
			dto.setProfessorId( professor.getId() );
			dto.setProfessorString( professor.getNome() );
		}

		Disciplina disciplina = domain.getDisciplina();
		if ( disciplina != null )
		{
			dto.setDisciplinaId( disciplina.getId() );
			dto.setDisciplinaString( disciplina.getCodigo() );
		}

		Campus campus = domain.getCampus();
		if ( campus != null )
		{
			dto.setCampusId( campus.getId() );
			dto.setCampusString( campus.getCodigo() );
		}

		Unidade unidade = domain.getUnidade();
		if ( unidade != null )
		{
			dto.setUnidadeId( unidade.getId() );
			dto.setUnidadeString( unidade.getCodigo() );
		}

		Sala sala = domain.getSala();
		if ( sala != null )
		{
			dto.setSalaId( sala.getId() );
			dto.setSalaString( sala.getCodigo() );
		}

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		dto.setDisplayText( domain.getCodigo() );

		return dto;
	}

	// PARÂMETROS
	public static Parametro toParametro( ParametroDTO dto )
	{
		Parametro domain = new Parametro();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );

		Cenario cenario = Cenario.find(
			dto.getCenarioId(), instituicaoEnsino );

		Cenario.entityManager().refresh( cenario );
		domain.setCenario( cenario );
		domain.setModoOtimizacao( dto.getModoOtimizacao() );

		SemanaLetiva semanaLetiva = SemanaLetiva.find(
			dto.getSemanaLetivaId(), instituicaoEnsino );
		semanaLetiva.flush();
		domain.setSemanaLetiva( semanaLetiva );

		Campus campus = Campus.find(
			dto.getCampusId(), instituicaoEnsino );
		campus.flush();
		domain.setCampus( campus );

		Turno turno = Turno.find(
			dto.getTurnoId(), instituicaoEnsino );
		turno.flush();
		domain.setTurno( turno );

		domain.setCargaHorariaAluno(dto.getCargaHorariaAluno());
		domain.setCargaHorariaAlunoSel(dto.getCargaHorariaAlunoSel());
		domain.setAlunoDePeriodoMesmaSala(dto.getAlunoDePeriodoMesmaSala());
		domain.setAlunoEmMuitosCampi(dto.getAlunoEmMuitosCampi());
		domain.setMinimizarDeslocamentoAluno(dto
				.getMinimizarDeslocamentoAluno());
		domain.setCargaHorariaProfessor(dto.getCargaHorariaProfessor());
		domain.setCargaHorariaProfessorSel(dto.getCargaHorariaProfessorSel());
		domain.setProfessorEmMuitosCampi(dto.getProfessorEmMuitosCampi());
		domain.setMinimizarDeslocamentoProfessor(dto
				.getMinimizarDeslocamentoProfessor());
		domain.setMinimizarDeslocamentoProfessorValue(dto
				.getMinimizarDeslocamentoProfessorValue());
		domain.setMinimizarGapProfessor(dto.getMinimizarGapProfessor());
		domain.setEvitarReducaoCargaHorariaProfessor(dto
				.getEvitarReducaoCargaHorariaProfessor());
		domain.setEvitarReducaoCargaHorariaProfessorValue(dto
				.getEvitarReducaoCargaHorariaProfessorValue());
		domain.setEvitarUltimoEPrimeiroHorarioProfessor(dto
				.getEvitarUltimoEPrimeiroHorarioProfessor());
		domain.setPreferenciaDeProfessores(dto.getPreferenciaDeProfessores());
		domain.setAvaliacaoDesempenhoProfessor(dto
				.getAvaliacaoDesempenhoProfessor());
		domain.setConsiderarEquivalencia(dto.getConsiderarEquivalencia());
		domain.setMinAlunosParaAbrirTurma(dto.getMinAlunosParaAbrirTurma());
		domain.setMinAlunosParaAbrirTurmaValue(dto
				.getMinAlunosParaAbrirTurmaValue());
		domain.setNivelDificuldadeDisciplina(dto
				.getNivelDificuldadeDisciplina());
		domain.setCompatibilidadeDisciplinasMesmoDia(dto
				.getCompatibilidadeDisciplinasMesmoDia());
		domain.setRegrasGenericasDivisaoCredito(dto
				.getRegrasGenericasDivisaoCredito());
		domain.setRegrasEspecificasDivisaoCredito(dto
				.getRegrasEspecificasDivisaoCredito());
		domain.setMaximizarNotaAvaliacaoCorpoDocente(dto
				.getMaximizarNotaAvaliacaoCorpoDocente());
		domain.setMinimizarCustoDocenteCursos(dto
				.getMinimizarCustoDocenteCursos());
		domain.setCompartilharDisciplinasCampi(dto
				.getCompartilharDisciplinasCampi());
		domain.setPercentuaisMinimosMestres(dto.getPercentuaisMinimosMestres());
		domain.setPercentuaisMinimosDoutores(dto
				.getPercentuaisMinimosDoutores());
		domain.setAreaTitulacaoProfessoresECursos(dto
				.getAreaTitulacaoProfessoresECursos());
		domain.setLimitarMaximoDisciplinaProfessor(dto
				.getLimitarMaximoDisciplinaProfessor());
		domain.setCargaHorariaAlunoSel(dto.getCargaHorariaAlunoSel());
		domain.setCargaHorariaProfessorSel(dto.getCargaHorariaProfessorSel());
		domain.setFuncaoObjetivo(dto.getFuncaoObjetivo());

		for ( CursoDTO cursoDTO
			: dto.getMaximizarNotaAvaliacaoCorpoDocenteList() )
		{
			Curso curso = Curso.find(
				cursoDTO.getId(), instituicaoEnsino );

			if ( !domain.getCursosMaxNotaAval().contains( curso ) )
			{
				curso.flush();
				domain.getCursosMaxNotaAval().add( curso );
			}
		}

		for ( CursoDTO cursoDTO
			: dto.getMinimizarCustoDocenteCursosList() )
		{
			Curso curso = Curso.find(
				cursoDTO.getId(), instituicaoEnsino );

			curso.flush();
			domain.getCursosMinCust().add( curso );
		}

		for ( CursoDescompartilhaDTO cursoDescompartilhaDTO
			: dto.getDescompartilharDisciplinasList() )
		{
			CursoDescompartilha cursoDescompartilha = new CursoDescompartilha();

			Curso curso1 = Curso.find( cursoDescompartilhaDTO.getCurso1Id(), instituicaoEnsino );
			Curso curso2 = Curso.find( cursoDescompartilhaDTO.getCurso2Id(), instituicaoEnsino );

			cursoDescompartilha.setCurso1( curso1 );
			cursoDescompartilha.setCurso2( curso2 );
			cursoDescompartilha.setParametro( domain );

			domain.getCursosDescompartDiscCampi().add( cursoDescompartilha );
		}

		return domain;
	}

	public static ParametroDTO toParametroDTO( Parametro domain )
	{
		ParametroDTO dto = new ParametroDTO();

		InstituicaoEnsino instituicaoEnsino
			 = domain.getCenario().getSemanaLetiva().getInstituicaoEnsino();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCenarioId( domain.getCenario().getId() );
		dto.setModoOtimizacao( domain.getModoOtimizacao() );

		SemanaLetiva semanaLetiva = domain.getSemanaLetiva();
		if ( semanaLetiva == null )
		{
			List< SemanaLetiva > list = new ArrayList< SemanaLetiva >(
				SemanaLetiva.getByOficial( instituicaoEnsino, domain.getCampus() ) );

			if ( !( list == null || list.size() == 0 ) )
			{
				semanaLetiva = list.get( 0 );
			}
			else
			{
				list = new ArrayList< SemanaLetiva >( SemanaLetiva.findAll( instituicaoEnsino ) );

				semanaLetiva = ( ( list == null || list.size() == 0 ) ? null : list.get( 0 ) );				
			}
		}

		dto.setSemanaLetivaId( semanaLetiva.getId() );
		dto.setSemanaLetivaDisplay( semanaLetiva.getCodigo() );

		Campus campus = domain.getCampus();
		if ( campus != null )
		{
			dto.setCampusId( campus.getId() );
			dto.setCampusDisplay( campus.getCodigo() );
		}

		Turno turno = domain.getTurno();
		if ( turno != null )
		{
			dto.setTurnoId( turno.getId() );
			dto.setTurnoDisplay( turno.getNome() );
		}

		dto.setCargaHorariaAluno( domain.getCargaHorariaAluno() );
		dto.setAlunoDePeriodoMesmaSala( domain.getAlunoDePeriodoMesmaSala() );
		dto.setAlunoEmMuitosCampi( domain.getAlunoEmMuitosCampi() );
		dto.setMinimizarDeslocamentoAluno( domain.getMinimizarDeslocamentoAluno() );
		dto.setCargaHorariaProfessor( domain.getCargaHorariaProfessor() );
		dto.setProfessorEmMuitosCampi( domain.getProfessorEmMuitosCampi() );
		dto.setMinimizarDeslocamentoProfessor( domain.getMinimizarDeslocamentoProfessor() );
		dto.setMinimizarGapProfessor( domain.getMinimizarGapProfessor() );
		dto.setEvitarReducaoCargaHorariaProfessor( domain.getEvitarReducaoCargaHorariaProfessor() );
		dto.setEvitarUltimoEPrimeiroHorarioProfessor( domain.getEvitarUltimoEPrimeiroHorarioProfessor() );
		dto.setPreferenciaDeProfessores( domain.getPreferenciaDeProfessores() );
		dto.setAvaliacaoDesempenhoProfessor( domain.getAvaliacaoDesempenhoProfessor() );
		dto.setConsiderarEquivalencia( domain.getConsiderarEquivalencia() );
		dto.setMinAlunosParaAbrirTurma( domain.getMinAlunosParaAbrirTurma() );
		dto.setMinAlunosParaAbrirTurmaValue( domain.getMinAlunosParaAbrirTurmaValue() );
		dto.setNivelDificuldadeDisciplina( domain.getNivelDificuldadeDisciplina() );
		dto.setCompatibilidadeDisciplinasMesmoDia( domain.getCompatibilidadeDisciplinasMesmoDia() );
		dto.setRegrasGenericasDivisaoCredito( domain.getRegrasGenericasDivisaoCredito() );
		dto.setRegrasEspecificasDivisaoCredito( domain.getRegrasEspecificasDivisaoCredito() );
		dto.setMaximizarNotaAvaliacaoCorpoDocente( domain.getMaximizarNotaAvaliacaoCorpoDocente() );
		dto.setMinimizarCustoDocenteCursos( domain.getMinimizarCustoDocenteCursos() );
		dto.setCompartilharDisciplinasCampi( domain.getCompartilharDisciplinasCampi() );
		dto.setPercentuaisMinimosMestres( domain.getPercentuaisMinimosMestres() );
		dto.setPercentuaisMinimosDoutores( domain.getPercentuaisMinimosDoutores() );
		dto.setAreaTitulacaoProfessoresECursos( domain.getAreaTitulacaoProfessoresECursos() );
		dto.setLimitarMaximoDisciplinaProfessor( domain.getLimitarMaximoDisciplinaProfessor() );
		dto.setCargaHorariaAlunoSel( domain.getCargaHorariaAlunoSel() );
		dto.setCargaHorariaProfessorSel( domain.getCargaHorariaProfessorSel() );
		dto.setMinimizarDeslocamentoProfessorValue( domain.getMinimizarDeslocamentoProfessorValue() );
		dto.setEvitarReducaoCargaHorariaProfessorValue( domain.getEvitarReducaoCargaHorariaProfessorValue() );
		dto.setFuncaoObjetivo( domain.getFuncaoObjetivo() );

		Set< Curso > cursosMaxNotaAvalList = domain.getCursosMaxNotaAval();
		List< CursoDTO > cursosMaxNotaAvalDTOList
			= new ArrayList< CursoDTO >( cursosMaxNotaAvalList.size() );

		for ( Curso curso : cursosMaxNotaAvalList )
		{
			cursosMaxNotaAvalDTOList.add( ConvertBeans.toCursoDTO( curso ) );
		}
		dto.setMaximizarNotaAvaliacaoCorpoDocenteList( cursosMaxNotaAvalDTOList );

		Set< Curso > cursosMinCustList = domain.getCursosMinCust();
		List< CursoDTO > cursosMinCustDTOList
			= new ArrayList< CursoDTO >( cursosMinCustList.size() );
		for ( Curso curso : cursosMinCustList )
		{
			cursosMinCustDTOList.add( ConvertBeans.toCursoDTO( curso ) );
		}
		dto.setMinimizarCustoDocenteCursosList( cursosMinCustDTOList );

		Set< CursoDescompartilha > cursosCompartDiscCampiList
			= domain.getCursosDescompartDiscCampi();

		List< CursoDescompartilhaDTO > cursosCompartDiscCampiDTOList
			= new ArrayList< CursoDescompartilhaDTO >( cursosCompartDiscCampiList.size() );

		for ( CursoDescompartilha cursoDescompartilha : cursosCompartDiscCampiList )
		{
			cursosCompartDiscCampiDTOList.add(
				ConvertBeans.toCursoDescompartilhaDTO( cursoDescompartilha ) );
		}
		dto.setDescompartilharDisciplinasCampiList( cursosCompartDiscCampiDTOList );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		return dto;
	}

	public static CursoDescompartilhaDTO toCursoDescompartilhaDTO(
		CursoDescompartilha domain )
	{
		CursoDescompartilhaDTO dto = new CursoDescompartilhaDTO();

		InstituicaoEnsino instituicaoEnsino
			= domain.getCurso1().getTipoCurso().getInstituicaoEnsino();

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );			
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCurso1Id(domain.getCurso1().getId());
		dto.setCurso1Display(domain.getCurso1().getNome());
		dto.setCurso2Id(domain.getCurso2().getId());
		dto.setCurso2Display(domain.getCurso2().getNome());
		dto.setParametroId(domain.getParametro().getId());

		return dto;
	}

	public static CursoDescompartilha toCursoDescompartilha(
		Parametro parametro, CursoDescompartilhaDTO dto )
	{
		CursoDescompartilha domain = new CursoDescompartilha();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setCurso1( Curso.find( dto.getCurso1Id(), instituicaoEnsino ) );
		domain.setCurso2( Curso.find( dto.getCurso2Id(), instituicaoEnsino ) );
		domain.setParametro( parametro );

		return domain;
	}

	static public String dateToString( Date date, int interval )
	{
		DateFormat df = new SimpleDateFormat( "HH:mm" );
		String inicio = df.format( date );

		Calendar fimCal = Calendar.getInstance();
		fimCal.setTime( date );
		fimCal.add( Calendar.MINUTE, interval );
		String fim = df.format( fimCal.getTime() );

		return ( inicio + " / " + fim );
	}

	// Instituição de Ensino
	public static InstituicaoEnsino toInstituicaoEnsino( InstituicaoEnsinoDTO dto )
	{
		InstituicaoEnsino domain = new InstituicaoEnsino();

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setNomeInstituicao( dto.getNomeInstituicao() );

		return domain;
	}

	public static InstituicaoEnsinoDTO toInstituicaoEnsinoDTO( InstituicaoEnsino domain )
	{
		InstituicaoEnsinoDTO dto = new InstituicaoEnsinoDTO();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setNomeInstituicao( domain.getNomeInstituicao() );
		dto.setDisplayText( domain.getNomeInstituicao() );

		return dto;
	}

	// Instituição de Ensino
	public static AlunoDemanda toAlunoDemanda( AlunoDemandaDTO dto )
	{
		AlunoDemanda domain = new AlunoDemanda();

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		Demanda demanda = Demanda.find(
			dto.getDemandaId(), instituicaoEnsino );
		domain.setDemanda( demanda );

		Aluno aluno = Aluno.find(
			dto.getIdAluno(), instituicaoEnsino );
		domain.setAluno( aluno );

		return domain;
	}

	public static AlunoDemandaDTO toAlunoDemandaDTO( AlunoDemanda domain )
	{
		AlunoDemandaDTO dto = new AlunoDemandaDTO();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setDisplayText( domain.getAluno().getNome() );

		// Aluno
		dto.setIdAluno( domain.getAluno().getId() );
		dto.setAlunoString( domain.getAluno().getNome() );
		
		// Demanda
		dto.setDemandaId( domain.getDemanda().getId() );

		String demandaStr = domain.getDemanda().getOferta().getCampus().getCodigo()
			+ " - " + domain.getDemanda().getOferta().getCurriculo().getCodigo()
			+ " - " + domain.getDemanda().getOferta().getTurno().getNome()
			+ " - " + domain.getDemanda().getDisciplina().getCodigo();
		dto.setDemandaString( demandaStr );

		// Instituição de Ensino
		dto.setInstituicaoEnsinoId( domain.getDemanda().getOferta().getCampus().getInstituicaoEnsino().getId() );
		dto.setInstituicaoEnsinoString( domain.getDemanda().getOferta().getCampus().getInstituicaoEnsino().getNomeInstituicao() );
		
		// Disciplina
		dto.setDisciplinaId( domain.getDemanda().getDisciplina().getId() );
		dto.setDisciplinaString( domain.getDemanda().getDisciplina().getNome() );

		return dto;
	}

	public static List < AlunoDemanda > toListAlunoDemanda(
		List< AlunoDemandaDTO > listDTO )
	{
		if ( listDTO == null )
		{
			return Collections.< AlunoDemanda > emptyList();
		}

		List< AlunoDemanda > listDomains
			= new ArrayList< AlunoDemanda >();

		for ( AlunoDemandaDTO dto : listDTO )
		{
			listDomains.add( ConvertBeans.toAlunoDemanda( dto ) );
		}

		return listDomains;
	}
	
	public static List < AlunoDemandaDTO > toListAlunoDemandaDTO(
		List< AlunoDemanda > listDomains )
	{
		if ( listDomains == null )
		{
			return Collections.< AlunoDemandaDTO > emptyList();
		}

		List< AlunoDemandaDTO > listDTOs
			= new ArrayList< AlunoDemandaDTO >();

		for ( AlunoDemanda domain : listDomains )
		{
			listDTOs.add( ConvertBeans.toAlunoDemandaDTO( domain ) );
		}

		return listDTOs;
	}

	public static Aluno toAluno( AlunoDTO dto )
	{
		Aluno domain = new Aluno();

		InstituicaoEnsino instituicaoEnsino = InstituicaoEnsino.find(
			dto.getInstituicaoEnsinoId() );

		Cenario cenario = Cenario.find(
			dto.getCenarioId(), instituicaoEnsino );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setNome( dto.getNome() );
		domain.setCpf( dto.getCpf() );
		domain.setCenario( cenario );
		domain.setInstituicaoEnsino( instituicaoEnsino );

		return domain;
	}

	public static AlunoDTO toAlunoDTO( Aluno domain )
	{
		AlunoDTO dto = new AlunoDTO();

		dto.setId( domain.getId() );
		dto.setNome( domain.getNome() );
		dto.setVersion( domain.getVersion() );
		dto.setCpf( domain.getCpf() );
		dto.setCenarioId( domain.getCenario().getId() );
		dto.setInstituicaoEnsinoId( domain.getInstituicaoEnsino().getId() );
		dto.setInstituicaoEnsinoString( domain.getInstituicaoEnsino().getNomeInstituicao() );
		dto.setDisplayText( domain.getNome() );

		return dto;
	}

	public static List < Aluno > toListAluno(
		List< AlunoDTO > listDTO )
	{
		if ( listDTO == null )
		{
			return Collections.< Aluno > emptyList();
		}

		List< Aluno > listDomains
			= new ArrayList< Aluno >();

		for ( AlunoDTO dto : listDTO )
		{
			listDomains.add( ConvertBeans.toAluno( dto ) );
		}

		return listDomains;
	}
	
	public static List < AlunoDTO > toListAlunoDTO(
		List< Aluno > listDomains )
	{
		if ( listDomains == null )
		{
			return Collections.< AlunoDTO > emptyList();
		}

		List< AlunoDTO > listDTOs
			= new ArrayList< AlunoDTO >();

		for ( Aluno domain : listDomains )
		{
			listDTOs.add( ConvertBeans.toAlunoDTO( domain ) );
		}

		return listDTOs;
	}
}
