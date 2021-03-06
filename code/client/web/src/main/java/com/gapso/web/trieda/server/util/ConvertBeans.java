package com.gapso.web.trieda.server.util;

import java.text.DateFormat;
import java.text.DecimalFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collection;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.TreeSet;
import java.util.concurrent.TimeUnit;

import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.AtendimentoFaixaDemanda;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Aula;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.CursoDescompartilha;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.DeslocamentoCampus;
import com.gapso.trieda.domain.DeslocamentoUnidade;
import com.gapso.trieda.domain.DicaEliminacaoProfessorVirtual;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Disponibilidade;
import com.gapso.trieda.domain.DisponibilidadeCampus;
import com.gapso.trieda.domain.DisponibilidadeDisciplina;
import com.gapso.trieda.domain.DisponibilidadeProfessor;
import com.gapso.trieda.domain.DisponibilidadeSala;
import com.gapso.trieda.domain.DisponibilidadeUnidade;
import com.gapso.trieda.domain.DivisaoCredito;
import com.gapso.trieda.domain.Equivalencia;
import com.gapso.trieda.domain.Fixacao;
import com.gapso.trieda.domain.GrupoSala;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.MotivoUsoProfessorVirtual;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Parametro;
import com.gapso.trieda.domain.ParametroConfiguracao;
import com.gapso.trieda.domain.ParametroGeracaoDemanda;
/*import com.gapso.trieda.domain.PerfilAcesso;
import com.gapso.trieda.domain.PerfilAcessoTransacao;*/
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorDisciplina;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.RequisicaoOtimizacao;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.TipoCurso;
import com.gapso.trieda.domain.TipoDisciplina;
import com.gapso.trieda.domain.TipoSala;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.trieda.domain.Turma;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.domain.Usuario;
import com.gapso.trieda.misc.Dificuldades;
import com.gapso.trieda.misc.Estados;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaDemandaDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.AulaDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDescompartilhaDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoCampusDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoUnidadeDTO;
import com.gapso.web.trieda.shared.dtos.DicaEliminacaoProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.DisponibilidadeDTO;
import com.gapso.web.trieda.shared.dtos.DivisaoCreditoDTO;
import com.gapso.web.trieda.shared.dtos.EquivalenciaDTO;
import com.gapso.web.trieda.shared.dtos.FixacaoDTO;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioAulaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.MotivoUsoProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.dtos.ParametroConfiguracaoDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.dtos.ParametroGeracaoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorCampusDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.dtos.TipoCursoDTO;
import com.gapso.web.trieda.shared.dtos.TipoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.TipoSalaDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.util.TriedaCurrency;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.gapso.web.trieda.shared.util.view.FuncaoObjetivoComboBox;

public class ConvertBeans {
	
	// USUÁRIO
	public static Usuario toUsuario( UsuarioDTO dto )
	{
		Usuario domain = new Usuario();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setInstituicaoEnsino( instituicaoEnsino );
		domain.setVersion( dto.getVersion() );
		domain.setNome( dto.getNome() );
		domain.setEmail( dto.getEmail() );
		domain.setUsername( dto.getUsername() );
		domain.setPassword( dto.getPassword() );
		domain.setEnabled( dto.getEnabled() );
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
			dto.setProfessorCpf( domain.getProfessor().getCpf() );
		}
		
		dto.setAdministrador(domain.getAuthority().getAuthority().equals("ROLE_SUPERVISOR"));
		
/*		for (PerfilAcesso perfilAcesso : domain.getPerfisAcesso())
		{
			for (PerfilAcessoTransacao perfilAcessoTransacao : perfilAcesso.getPerfisAcessoTransacao())
			{
				if (dto.getTransacaoMapVisivel().get(perfilAcessoTransacao.getTransacao().getNome()) == null)
				{
					dto.getTransacaoMapVisivel().put(perfilAcessoTransacao.getTransacao().getNome(), perfilAcessoTransacao.getVisivel());
				}
				else
				{
					Boolean novoValor = dto.getTransacaoMapVisivel().get(perfilAcessoTransacao.getTransacao().getNome()) || perfilAcessoTransacao.getVisivel();
					dto.getTransacaoMapVisivel().put(perfilAcessoTransacao.getTransacao().getNome(), novoValor);
				}
				
				if (dto.getTransacaoMapEditavel().get(perfilAcessoTransacao.getTransacao().getNome()) == null)
				{
					dto.getTransacaoMapEditavel().put(perfilAcessoTransacao.getTransacao().getNome(), perfilAcessoTransacao.getEditavel());
				}
				else
				{
					Boolean novoValor = dto.getTransacaoMapEditavel().get(perfilAcessoTransacao.getTransacao().getNome()) || perfilAcessoTransacao.getEditavel();
					dto.getTransacaoMapEditavel().put(perfilAcessoTransacao.getTransacao().getNome(), novoValor);
				}
			}
		}*/

		return dto;
	}

	// CENÁRIO
	public static Cenario toCenario( CenarioDTO dto )
	{
		Cenario domain = new Cenario();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );
		
		Usuario criadoPor = Usuario.find(dto.getCriadoUsuarioId());
		
		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setNome( dto.getNome() );
		domain.setMasterData( dto.getMasterData() );
		domain.setOficial( dto.getOficial() );
		domain.setAno( dto.getAno() );
		domain.setSemestre( dto.getSemestre() );
		domain.setComentario( dto.getComentario() );
		domain.setInstituicaoEnsino(instituicaoEnsino);
		domain.setDataCriacao(dto.getCriadoUsuarioDate());
		domain.setCriadoPor(criadoPor);
		

		return domain;
	}
	
	public static CenarioDTO toCenarioDTO( Cenario domain )
	{
		CenarioDTO dto = new CenarioDTO();
		InstituicaoEnsino instituicaoEnsino = domain.getInstituicaoEnsino();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setNome( domain.getNome() );
		dto.setMasterData( domain.getMasterData() );
		dto.setOficial( domain.getOficial() );
		dto.setAno( domain.getAno() );
		dto.setSemestre( domain.getSemestre() );
		dto.setComentario( domain.getComentario() );
		dto.setCriadoUsuarioId(domain.getCriadoPor() == null ? null: domain.getCriadoPor().getUsername());
		dto.setCriadoUsuarioString( domain.getCriadoPor() == null ? null : domain.getCriadoPor().getNome() );
		dto.setCriadoUsuarioDate( domain.getDataCriacao() );
		dto.setAtualizadoUsuarioString( domain.getAtualizadoPor() == null ? null : domain.getAtualizadoPor().getNome() );
		dto.setAtualizadoUsuarioDate( domain.getDataAtualizacao() );
		dto.setAlunos(domain.hasAlunos());
		dto.setProfessores(domain.hasProfessores() || (ProfessorVirtual.count(instituicaoEnsino,domain)>0));
		dto.setSalas(domain.hasSalas());
		dto.setOptimized(domain.isOptimized());
			
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

		dto.setPublicado( domain.getPublicado() == null ? false : domain.getPublicado() );
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
				dto.setInstituicaoEnsinoId(
					campus.getInstituicaoEnsino().getId() );

				dto.setInstituicaoEnsinoString(
					campus.getInstituicaoEnsino().getNomeInstituicao() );
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
		
		Cenario cenario = Cenario.find( dto.getCenarioId(), instituicaoEnsino );

		domain.setInstituicaoEnsino( instituicaoEnsino );
		domain.setCenario( cenario );
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
		
		Cenario cenario = domain.getCenario();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setNome( domain.getNome() );
		dto.setDescricao( domain.getDescricao() );
		dto.setDisplayText( domain.getNome()
			+ " (" + domain.getDescricao() + ")" );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}
		
		if ( cenario != null )
		{
			dto.setCenarioId( cenario.getId() );
		}

		return dto;
	}

	// SALA
	public static Collection< SalaDTO > toSalaDTO(
		Collection< Sala > domains )
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
		domain.setDescricao( dto.getDescricao() );
		domain.setAndar( dto.getAndar() );
		domain.setCapacidadeInstalada( dto.getCapacidadeInstalada() );
		domain.setCapacidadeMax( dto.getCapacidadeMax() );
		domain.setCustoOperacaoCred( dto.getCustoOperacaoCred() );
		domain.setExterna( dto.getExterna() );

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
		dto.setDescricao( domain.getDescricao() );
		dto.setAndar( domain.getAndar() );
		dto.setCapacidadeInstalada( domain.getCapacidadeInstalada() );
		dto.setCapacidadeMax( domain.getCapacidadeMax() );
		dto.setCustoOperacaoCred( domain.getCustoOperacaoCred() );
		dto.setExterna(domain.getExterna());
		dto.setTipoId( domain.getTipoSala().getId() );
		dto.setTipoString( domain.getTipoSala().getNome() );
		dto.setCampusId( domain.getUnidade().getCampus().getId() );
		dto.setUnidadeId( domain.getUnidade().getId() );
		dto.setUnidadeString( domain.getUnidade().getCodigo() );
		dto.setDisplayText( domain.getCodigo() + " (" + domain.getNumero() + ")" + " (" + domain.getCapacidadeInstalada() + " lugares)");
		
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
		dto.setMaxCreditos( domain.getHorariosAula().size() );

		Set< HorarioAula > horarios = domain.getHorariosAula();

		Map< Long, Map< Integer, Integer > > countHorariosAula
			= new HashMap< Long, Map< Integer, Integer > >();

		for ( HorarioAula ha : horarios )
		{
			
			if(countHorariosAula.get(ha.getSemanaLetiva().getId()) == null){
				countHorariosAula.put(ha.getSemanaLetiva().getId(), new HashMap <Integer, Integer>());
			}
			for ( HorarioDisponivelCenario hdc
				: ha.getHorariosDisponiveisCenario() )
			{
				int semanaInt = Semanas.toInt( hdc.getDiaSemana() );
				Integer value = countHorariosAula.get(ha.getSemanaLetiva().getId()).get( semanaInt );
				value = ( ( value == null ) ? 0 : value );

				countHorariosAula.get(ha.getSemanaLetiva().getId()).put( semanaInt, value + 1 );
			}
		}

		dto.setCountHorariosAula( countHorariosAula );

		Map< Long, String > horariosStringMap
			= new HashMap< Long, String >();

		Map< Long, Date > horariosInicioMap
			= new HashMap< Long, Date >();

		for ( HorarioAula ha : horarios )
		{
			horariosStringMap.put( ha.getId(), ConvertBeans.dateToString(
				ha.getHorario(), ha.getSemanaLetiva().getTempo() ) );

			horariosInicioMap.put( ha.getId(), ha.getHorario() );
		}

		dto.setHorariosStringMap( horariosStringMap );
		dto.setHorariosInicioMap( horariosInicioMap );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		dto.setDisplayText( domain.getNome() );

		return dto;
	}

	// CALENDÁRIO
	public static SemanaLetiva toSemanaLetiva( SemanaLetivaDTO dto )
	{
		SemanaLetiva domain = new SemanaLetiva();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );
		Cenario cenario
			= Cenario.find(dto.getCenarioId(), instituicaoEnsino);

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setCodigo( dto.getCodigo() );
		domain.setDescricao( dto.getDescricao() );
		domain.setInstituicaoEnsino( instituicaoEnsino );
		domain.setTempo( dto.getTempo() );
		domain.setPermiteIntervaloAula( dto.getPermiteIntervaloAula() );
		domain.setCenario( cenario );

		return domain;
	}

	public static Set< SemanaLetivaDTO > toSemanaLetivaDTO(
		Collection< SemanaLetiva > semanasLetivas )
	{
		Set< SemanaLetivaDTO > semanasLetivasDTO = new HashSet< SemanaLetivaDTO >();

		for ( SemanaLetiva semanaLetiva : semanasLetivas )
		{
			semanasLetivasDTO.add( ConvertBeans.toSemanaLetivaDTO( semanaLetiva ) );
		}

		return semanasLetivasDTO;
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
		
		Cenario cenario
			= domain.getCenario();
		
		if (cenario != null)
		{
			dto.setCenarioId( cenario.getId() );
		}

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCodigo( domain.getCodigo() );
		dto.setDescricao( domain.getDescricao() );
		dto.setTempo( domain.getTempo() );
		dto.setMaxCreditos( domain.getHorariosAula().size() );
		dto.setPermiteIntervaloAula( domain.getPermiteIntervaloAula() );

		Map< Long, String > horariosStringMap
			= new HashMap< Long, String >();

		Map< Long, Date > horariosInicioMap
			= new HashMap< Long, Date >();

		for ( HorarioAula ha : domain.getHorariosAula() )
		{
			horariosStringMap.put( ha.getId(), ConvertBeans.dateToString(
				ha.getHorario(), ha.getSemanaLetiva().getTempo() ) );

			horariosInicioMap.put( ha.getId(), ha.getHorario() );
		}

		dto.setHorariosStringMap( horariosStringMap );
		dto.setHorariosInicioMap( horariosInicioMap );

		dto.setDisplayText( domain.getCodigo() + " (" + domain.getTempo() + " min)" );

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
		dto.setTurnoString( turno.getNome() );
		dto.setInicio( domain.getHorario() );

		Calendar fimCal = Calendar.getInstance();
		fimCal.setTime( domain.getHorario() );
		fimCal.add( Calendar.MINUTE, domain.getSemanaLetiva().getTempo() );
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
					domain.setDiaSemana( Semanas.SEG );
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
					domain.setDiaSemana( Semanas.TER );
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
					domain.setDiaSemana( Semanas.QUA );
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
					domain.setDiaSemana( Semanas.QUI );
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
					domain.setDiaSemana( Semanas.SEX );
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
					domain.setDiaSemana( Semanas.SAB );
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
					domain.setDiaSemana( Semanas.DOM );
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
		HorarioAula domain )
	{
		HorarioDisponivelCenarioDTO dto = new HorarioDisponivelCenarioDTO();

		dto.setHorarioDeAulaId( domain.getId() );
		dto.setHorarioDeAulaVersion( domain.getVersion() );
		dto.setTurnoString( domain.getTurno().getNome() );
		dto.setSemanaLetivaId( domain.getSemanaLetiva().getId() );
		dto.setSemanaLetivaString(domain.getSemanaLetiva().getCodigo());

		dto.setSegunda( false );
		dto.setTerca( false );
		dto.setQuarta( false );
		dto.setQuinta( false );
		dto.setSexta( false );
		dto.setSabado( false );
		dto.setDomingo( false );

		for ( HorarioDisponivelCenario o
			: domain.getHorariosDisponiveisCenario() )
		{
			switch ( o.getDiaSemana() )
			{
				case SEG:
					dto.setSegunda( true );
					dto.setSegundaId( o.getId() );
					break;
				case TER:
					dto.setTerca( true );
					dto.setTercaId( o.getId() );
					break;
				case QUA:
					dto.setQuarta( true );
					dto.setQuartaId( o.getId() );
					break;
				case QUI:
					dto.setQuinta( true );
					dto.setQuintaId( o.getId() );
					break;
				case SEX:
					dto.setSexta( true );
					dto.setSextaId( o.getId() );
					break;
				case SAB:
					dto.setSabado( true );
					dto.setSabadoId( o.getId() );
					break;
				case DOM:
					dto.setDomingo( true );
					dto.setDomingoId( o.getId() );
					break;
				default:
					break;
			}
		}

		dto.setHorarioString( ConvertBeans.dateToString(
			domain.getHorario(), domain.getSemanaLetiva().getTempo() ) );

		dto.setHorario( domain.getHorario() );

		InstituicaoEnsino instituicaoEnsino = null;

		if ( domain.getSemanaLetiva() != null )
		{
			instituicaoEnsino = domain.getSemanaLetiva().getInstituicaoEnsino();
		}

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
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
				dto.setSemanaLetivaId( horarioAula.getSemanaLetiva().getId() );
				dto.setSemanaLetivaString(horarioAula.getSemanaLetiva().getCodigo());

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
				switch ( o.getDiaSemana() )
				{
					case SEG:
					{
						dto.setSegunda( true );
						dto.setSegundaId( o.getId() );
						break;
					}
					case TER:
					{
						dto.setTerca( true );
						dto.setTercaId( o.getId() );
						break;
					}
					case QUA:
					{
						dto.setQuarta( true );
						dto.setQuartaId( o.getId() );
						break;
					}
					case QUI:
					{
						dto.setQuinta( true );
						dto.setQuintaId( o.getId() );
						break;
					}
					case SEX:
					{
						dto.setSexta( true );
						dto.setSextaId( o.getId() );
						break;
					}
					case SAB:
					{
						dto.setSabado( true );
						dto.setSabadoId( o.getId() );
						break;
					}
					case DOM:
					{
						dto.setDomingo( true );
						dto.setDomingoId( o.getId() );
						break;
					}
					default:
					{
						break;
					}
				}
			}

			DateFormat df = new SimpleDateFormat( "HH:mm" );
			String inicio = df.format( horarioAula.getHorario() );

			Calendar fimCal = Calendar.getInstance();
			fimCal.setTime( horarioAula.getHorario() );

			fimCal.add( Calendar.MINUTE, horarioAula.getSemanaLetiva().getTempo() );
			String fim = df.format( fimCal.getTime() );

			dto.setHorarioString( inicio + " / " + fim );
			dto.setHorarioInicioString( inicio );
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
		
		Cenario cenario = Cenario.find(dto.getCenarioId(), instituicaoEnsino);

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setCodigo( dto.getCodigo() );
		domain.setDescricao( dto.getDescricao() );
		domain.setInstituicaoEnsino( instituicaoEnsino );
		domain.setCenario( cenario );

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
		dto.setCenarioId( domain.getCenario().getId() );
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

		InstituicaoEnsino instituicaoEnsino = null;
		
		if ( domain.getTipoCurso() != null )
		{
			instituicaoEnsino = domain.getTipoCurso().getInstituicaoEnsino();
		}

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
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
		Cenario cenario
			= Cenario.find(dto.getCenarioId(), instituicaoEnsino);

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setCodigo( dto.getCodigo() );
		domain.setDescricao( dto.getDescricao() );
		domain.setInstituicaoEnsino( instituicaoEnsino );
		domain.setCenario( cenario );

		return domain;
	}

	public static AreaTitulacaoDTO toAreaTitulacaoDTO( AreaTitulacao domain )
	{
		Long instituicaoEnsinoId = null;
		String instituicaoEnsinoString = "";

		InstituicaoEnsino instituicaoEnsino = domain.getInstituicaoEnsino();

		if ( instituicaoEnsino != null )
		{
			instituicaoEnsinoId = instituicaoEnsino.getId();
			instituicaoEnsinoString = instituicaoEnsino.getNomeInstituicao();
		}

		AreaTitulacaoDTO dto = new AreaTitulacaoDTO(
			domain.getId(), domain.getCodigo(), domain.getDescricao(),
			domain.getVersion(), instituicaoEnsinoId, instituicaoEnsinoString,
			domain.getCenario().getId());

		return dto;
	}

	// TITULAÇÃO
	public static Titulacao toTitulacao( TitulacaoDTO dto )
	{
		Titulacao domain = new Titulacao();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );
		
		Cenario cenario = Cenario.find(dto.getCenarioId(), instituicaoEnsino);

		domain.setInstituicaoEnsino( instituicaoEnsino );
		domain.setCenario( cenario );
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
		
		Cenario cenario =  domain.getCenario();

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}
		
		if ( cenario != null )
		{
			dto.setCenarioId( cenario.getId() );
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
		
		Cenario cenario = Cenario.find(dto.getCenarioId(), instituicaoEnsino);

		domain.setInstituicaoEnsino( instituicaoEnsino );
		domain.setCenario(cenario);
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
		
		Cenario cenario = domain.getCenario();

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}
		
		if ( cenario != null )
		{
			dto.setCenarioId( cenario.getId() );
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

		InstituicaoEnsino instituicaoEnsino
			= unidade.getCampus().getInstituicaoEnsino();

		if ( instituicaoEnsino != null )
		{
			deslocamentoUnidadeDTO.setInstituicaoEnsinoId(
				instituicaoEnsino.getId() );

			deslocamentoUnidadeDTO.setInstituicaoEnsinoString(
				instituicaoEnsino.getNomeInstituicao() );
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

		InstituicaoEnsino instituicaoEnsino
			= campus.getInstituicaoEnsino();

		if ( instituicaoEnsino != null )
		{
			deslocamentoCampusDTO.setInstituicaoEnsinoId(
				instituicaoEnsino.getId() );

			deslocamentoCampusDTO.setInstituicaoEnsinoString(
				instituicaoEnsino.getNomeInstituicao() );
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
		
		Cenario cenario = Cenario.find( dto.getCenarioId(), instituicaoEnsino );

		domain.setInstituicaoEnsino( instituicaoEnsino );
		domain.setCenario( cenario );
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
		
		Cenario cenario = domain.getCenario();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setNome( domain.getNome() );
		dto.setDisplayText( domain.getNome() );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}
		
		if ( cenario != null )
		{
			dto.setCenarioId( cenario.getId() );
		}

		return dto;
	}

	// DISCIPLINA
	public static Disciplina toDisciplina( DisciplinaDTO dto )
	{
		Disciplina domain = new Disciplina();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );

		Cenario cenario = Cenario.find(
			dto.getCenarioId(), instituicaoEnsino );

		domain.setCenario( cenario );
		domain.setCodigo( dto.getCodigo() );
		domain.setNome( dto.getNome() );
		domain.setCreditosPratico( dto.getCreditosPratico() );
		domain.setCreditosTeorico( dto.getCreditosTeorico() );
		domain.setLaboratorio( dto.getLaboratorio() );
		domain.setMaxAlunosTeorico( dto.getMaxAlunosTeorico() );
		domain.setMaxAlunosPratico( dto.getMaxAlunosPratico() );
		domain.setCargaHoraria( dto.getCargaHoraria() );
		domain.setDificuldade( Dificuldades.get( dto.getDificuldade() ) );
		domain.setUsaDomingo( dto.getUsaDomingo() );
		domain.setUsaSabado( dto.getUsaSabado() );
		domain.setAulasContinuas( dto.getAulasContinuas() );
		domain.setProfessorUnico( dto.getProfessorUnico() );

		domain.setTipoDisciplina( TipoDisciplina.find(
			dto.getTipoId(), instituicaoEnsino ) );

		return domain;
	}

	public static DisciplinaDTO toDisciplinaDTO( Disciplina domain )
	{
		DisciplinaDTO dto = new DisciplinaDTO();

		InstituicaoEnsino instituicaoEnsino
			= domain.getTipoDisciplina().getInstituicaoEnsino();

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
		dto.setUsaDomingo( domain.getUsaDomingo() );
		dto.setUsaSabado( domain.getUsaSabado() );
		dto.setCargaHoraria( domain.getCargaHoraria() );
		dto.setAulasContinuas( domain.getAulasContinuas() );
		dto.setProfessorUnico( domain.getProfessorUnico() );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		return dto;
	}

	// CURRICULO ( MATRIZ CURRICULAR )
	public static Curriculo toCurriculo( CurriculoDTO dto )
	{
		Curriculo domain = new Curriculo();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );

		Cenario cenario = Cenario.find(
			dto.getCenarioId(), instituicaoEnsino );
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
		dto.setPeriodosList(periodosSet);
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
		dto.setCursoString( curso.getCodigo()
			+ " (" + curso.getNome() + ")" );
		dto.setDisplayText( domain.getCodigo() + " ("
			+ curso.getNome()	+ ") Periodos: " + periodos );

		SemanaLetiva sl = domain.getSemanaLetiva();
		
		InstituicaoEnsino instituicaoEnsino = null;
		
		if ( sl != null )
		{
			instituicaoEnsino = sl.getInstituicaoEnsino();

			dto.setSemanaLetivaId( sl == null ? null : sl.getId() );
			dto.setSemanaLetivaString( sl == null ? "" : sl.getCodigo() );
		}
		else
		{
			instituicaoEnsino = null;

			dto.setSemanaLetivaId( null );
			dto.setSemanaLetivaString( "" );
		}

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		return dto;
	}
	
	public static List < CurriculoDisciplina > toListCurriculoDisciplina(
			List< CurriculoDisciplinaDTO > listDTO )
	{
		if ( listDTO == null )
		{
			return Collections.< CurriculoDisciplina > emptyList();
		}

		List< CurriculoDisciplina > listDomains
			= new ArrayList< CurriculoDisciplina >();

		for ( CurriculoDisciplinaDTO dto : listDTO )
		{
			listDomains.add( ConvertBeans.toCurriculoDisciplina( dto ) );
		}

		return listDomains;
	}

	// CURRICULO DISCIPLINA ( DISCIPLINA NA MATRIZ CURRICULAR )
	public static CurriculoDisciplina toCurriculoDisciplina(
		CurriculoDisciplinaDTO dto )
	{
		CurriculoDisciplina domain = new CurriculoDisciplina();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setPeriodo( dto.getPeriodo() );
		domain.setMaturidade( dto.getMaturidade() );

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

		InstituicaoEnsino instituicaoEnsino
			= domain.getDisciplina().getTipoDisciplina().getInstituicaoEnsino();

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
		dto.setMaturidade( domain.getMaturidade() );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

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

		Double receita = ( dto.getReceita().getDoubleValue() == null ?
			0.0 : dto.getReceita().getDoubleValue() );
		domain.setReceita( receita );
		
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

		InstituicaoEnsino instituicaoEnsino
			= domain.getCampus().getInstituicaoEnsino();

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		dto.setDisplayText( dto.getMatrizCurricularString() + " (" + curso.getNome() + ")" );

		return dto;
	}

	// DEMANDA
	public static Demanda toDemanda( DemandaDTO dto )
	{
		Demanda domain = new Demanda();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );
		Cenario cenario
			= Cenario.find(dto.getCenarioId(), instituicaoEnsino);

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

			List< Oferta > ofertas = Oferta.findBy( instituicaoEnsino, cenario,
				turno, campus, curso, curriculo, null,null, 0, 1, null );

			domain.setOferta( ( ofertas == null || ofertas.size() == 0 ) ? null : ofertas.get( 0 ) );
		}

		domain.setDisciplina( Disciplina.find(
			dto.getDisciplinaId(), instituicaoEnsino ) );

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
		dto.setOfertaId(oferta.getId());

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
		Integer alunosDemandaReal = AlunoDemanda.findByDemandaReal(
			instituicaoEnsino, domain ).size();
		Integer alunosDemandaVirtual = AlunoDemanda.findByDemandaVirtual(
				instituicaoEnsino, domain ).size();

		dto.setDemanda( domain.getQuantidade() );
		dto.setDemandaReal( alunosDemandaReal );
		dto.setDemandaVirtual(alunosDemandaVirtual);
		dto.setQuantidadeDemandaEnable( true );
		
		dto.setPeriodo(curriculo.getPeriodo(disciplina));

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		dto.setDisplayText( dto.getNaturalKey() );

		return dto;
	}
	
	public static List<DemandaDTO> toDemandaDTOPorPeriodo( Demanda domain )
	{
		InstituicaoEnsino instituicaoEnsino
			= domain.getOferta().getCampus().getInstituicaoEnsino();
		
		List<AlunoDemanda> alunosDemanda = new ArrayList<AlunoDemanda>();
		alunosDemanda.addAll(AlunoDemanda.findByDemandaReal(
				instituicaoEnsino, domain ));
		alunosDemanda.addAll(AlunoDemanda.findByDemandaVirtual(
				instituicaoEnsino, domain ));
		
		Map<Integer, Map<Boolean, List<AlunoDemanda>>> periodoMapExigeEquivalenciaMapAlunoDemanda
			= new HashMap<Integer, Map<Boolean, List<AlunoDemanda>>>();
		for (AlunoDemanda alunoDemanda : alunosDemanda)
		{
			if (periodoMapExigeEquivalenciaMapAlunoDemanda.get(alunoDemanda.getAluno().getPrioridade()) == null)
			{
				List<AlunoDemanda> newAlunosDemandaList = new ArrayList<AlunoDemanda>();
				newAlunosDemandaList.add(alunoDemanda);
				Map<Boolean, List<AlunoDemanda>> exigeEquivalenciaMapAlunoDemanda = new HashMap<Boolean, List<AlunoDemanda>>();
				exigeEquivalenciaMapAlunoDemanda.put(alunoDemanda.getExigeEquivalenciaForcada(), newAlunosDemandaList);
				periodoMapExigeEquivalenciaMapAlunoDemanda.put(alunoDemanda.getAluno().getPrioridade(), exigeEquivalenciaMapAlunoDemanda);
			}
			else
			{
				if (periodoMapExigeEquivalenciaMapAlunoDemanda.get(alunoDemanda.getAluno().getPrioridade()).get(
						alunoDemanda.getExigeEquivalenciaForcada()) == null)
				{
					List<AlunoDemanda> newAlunosDemandaList = new ArrayList<AlunoDemanda>();
					newAlunosDemandaList.add(alunoDemanda);
					periodoMapExigeEquivalenciaMapAlunoDemanda.get(alunoDemanda.getAluno().getPrioridade()).put(alunoDemanda.getExigeEquivalenciaForcada(), newAlunosDemandaList);
				}
				else
				{
					periodoMapExigeEquivalenciaMapAlunoDemanda.get(alunoDemanda.getAluno().getPrioridade()).get(
							alunoDemanda.getExigeEquivalenciaForcada()).add(alunoDemanda);
				}
					
			}
		}
		
		List<DemandaDTO> demandasDTO = new ArrayList<DemandaDTO>();
		for (Entry<Integer, Map<Boolean, List<AlunoDemanda>>> prioridade : periodoMapExigeEquivalenciaMapAlunoDemanda.entrySet())
		{
			for (Entry<Boolean, List<AlunoDemanda>> exigeEquivalencia : prioridade.getValue().entrySet())
			{
				DemandaDTO dto = new DemandaDTO();

				dto.setId( domain.getId() );
				dto.setVersion( domain.getVersion() );
				
				dto.setExigeEquivalenciaForcada(exigeEquivalencia.getKey());
				dto.setPrioridadeAlunos(prioridade.getKey());

				Oferta oferta = domain.getOferta();
				dto.setOfertaId(oferta.getId());

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
				
				Integer alunosDemandaReal = 0;
				Integer alunosDemandaVirtual = 0;
				Integer quantidadeNaoAtendidaPorPrioridade = 0;
				for (AlunoDemanda alunos : exigeEquivalencia.getValue())
				{
					if (alunos.getAluno().getVirtual())
						alunosDemandaVirtual++;
					else
						alunosDemandaReal++;
					
					if (!alunos.getAtendido())
						quantidadeNaoAtendidaPorPrioridade++;
						
				}

				dto.setDemanda( domain.getQuantidade() );
				dto.setDemandaReal( alunosDemandaReal );
				dto.setDemandaVirtual(alunosDemandaVirtual);
				dto.setQuantidadeDemandaEnable( true );
				dto.setQuantidadeNaoAtendida(quantidadeNaoAtendidaPorPrioridade);
				
				dto.setPeriodo(curriculo.getPeriodo(disciplina));

				if ( instituicaoEnsino != null )
				{
					dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
					dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
				}

				dto.setDisplayText( dto.getNaturalKey() );
				demandasDTO.add(dto);
			}
		}

		return demandasDTO;
	}
	
	public static List < DemandaDTO > toListDemandaDTO(
			List< Demanda > listDomains )
		{
			if ( listDomains == null )
			{
				return Collections.< DemandaDTO > emptyList();
			}

			List< DemandaDTO > listDTOs
				= new ArrayList< DemandaDTO >();

			for ( Demanda domain : listDomains )
			{
				listDTOs.add( ConvertBeans.toDemandaDTO( domain ) );
			}

			return listDTOs;
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
		
		domain.setHorarioAula(HorarioAula.find(dto.getHorarioAulaId(), instituicaoEnsino));

		domain.setOferta( Oferta.find(
			dto.getOfertaId(), instituicaoEnsino ) );

		domain.setDisciplina( Disciplina.find( dto.getDisciplinaId(), instituicaoEnsino ) );
		
		if (dto.getDisciplinaSubstitutaId() != null) {
			domain.setDisciplinaSubstituta(Disciplina.find(dto.getDisciplinaSubstitutaId(),instituicaoEnsino));
		}
		
		domain.setQuantidadeAlunos( dto.getQuantidadeAlunos() );
		domain.setCreditosTeorico( dto.getCreditosTeorico() );
		domain.setCreditosPratico( dto.getCreditosPratico() );
		domain.setInstituicaoEnsino( instituicaoEnsino );

		return domain;
	}

	public static AtendimentoTaticoDTO toAtendimentoTaticoDTO(AtendimentoTatico domain) {
		AtendimentoTaticoDTO dto = new AtendimentoTaticoDTO();

		Sala sala = domain.getSala();
		Unidade unidade = sala.getUnidade();
		Campus campus = unidade.getCampus();
		Oferta oferta = domain.getOferta();
		Turno turno = oferta.getTurno();
		Curriculo curriculo = oferta.getCurriculo();
		SemanaLetiva semanaLetiva = curriculo.getSemanaLetiva();
		Curso curso = curriculo.getCurso();
		Disciplina disciplina = domain.getDisciplina();
		Disciplina disciplinaSubstituta = domain.getDisciplinaSubstituta();
		SemanaLetiva semanaLetivaDisciplinaSubstituta = null;
		if (disciplinaSubstituta != null) {
			if (!disciplinaSubstituta.getCurriculos().isEmpty()) {
				semanaLetivaDisciplinaSubstituta = disciplinaSubstituta.getCurriculos().iterator().next().getCurriculo().getSemanaLetiva(); 
			}
		}

		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCenarioId(domain.getCenario().getId());
		dto.setTurma(domain.getTurma());
		dto.setCampusId(campus.getId());
		dto.setCampusString(campus.getCodigo() );
		dto.setUnidadeId(unidade.getId());
		dto.setUnidadeString(unidade.getCodigo());
		dto.setSalaId(sala.getId());
		dto.setSalaString(sala.getCodigo());
		dto.setSemana(Semanas.toInt(domain.getSemana()));
		dto.setDiaSemana( dto.getSemana() );
		if (domain.getHorarioAula() != null) {
			dto.setHorarioAulaId(domain.getHorarioAula().getId());
			dto.setHorarioAulaString( TriedaUtil.shortTimeString(domain.getHorarioAula().getHorario()));
			semanaLetiva = domain.getHorarioAula().getSemanaLetiva();
			semanaLetivaDisciplinaSubstituta = semanaLetiva; 
		}
		dto.setOfertaId(oferta.getId());
		dto.setDisciplinaId(disciplina.getId());
		dto.setDisciplinaString(disciplina.getCodigo());
		dto.setDisciplinaNome(disciplina.getNome());
		dto.setDisciplinaOriginalCodigo(disciplina.getCodigo());
		dto.setDisciplinaUsaLaboratorio(disciplina.getLaboratorio());
		if (disciplinaSubstituta != null) {
			dto.setDisciplinaSubstitutaId(disciplinaSubstituta.getId());
			dto.setDisciplinaSubstitutaString(disciplinaSubstituta.getCodigo());
			dto.setDisciplinaSubstitutaNome(disciplinaSubstituta.getNome());
			dto.setTotalCreditoDisciplinaSubstituta(disciplinaSubstituta.getCreditosTotal());
			if (semanaLetivaDisciplinaSubstituta != null) {
				dto.setDisciplinaSubstitutaSemanaLetivaId(semanaLetivaDisciplinaSubstituta.getId());
				dto.setDisciplinaSubstitutaSemanaLetivaTempoAula(semanaLetivaDisciplinaSubstituta.getTempo());
			}
		}
		dto.setQuantidadeAlunos(domain.getQuantidadeAlunos());
		dto.setQuantidadeAlunosString(domain.getQuantidadeAlunos().toString());
		dto.setCreditosTeorico(domain.getCreditosTeorico());
		dto.setCreditosPratico(domain.getCreditosPratico());
		dto.setCursoString(curso.getCodigo());
		dto.setCursoNome(curso.getNome());
		dto.setCursoId(curso.getId());
		dto.setCurricularString(curriculo.getCodigo());
		dto.setCurricularId(curriculo.getId());

		//int periodo = domain.getOferta().getCurriculo().getPeriodo(instituicaoEnsino, domain.getDisciplina(), domain.getOferta() );
		// TODO: retirar
		Integer per = curriculo.getPeriodo(domain.getDisciplina());
		if (per == null) {
			System.out.println("*** disc="+domain.getDisciplina().getCodigo()+" curr="+curriculo.getCodigo()+" discSubs="+(disciplinaSubstituta!=null?disciplinaSubstituta.getCodigo():"null"));
		}
		int periodo = per==null?-1:per;
		dto.setPeriodo( periodo );
		dto.setPeriodoString(String.valueOf(periodo));

		dto.setTotalCreditosTeoricosDisciplina( domain.getDisciplina().getCreditosTeorico() );
		dto.setTotalCreditosPraticosDisciplina( domain.getDisciplina().getCreditosPratico() );
		dto.setTotalCreditoDisciplina(disciplina.getTotalCreditos());
		dto.setTurnoId(turno.getId());
		dto.setTurnoString(turno.getNome());
		dto.setDisplayText(dto.getNaturalKey());
		dto.setCompartilhamentoCursosString("");
		dto.setSemanaLetivaId(semanaLetiva.getId());
		dto.setSemanaLetivaTempoAula(semanaLetiva.getTempo());

		InstituicaoEnsino instituicaoEnsino = domain.getInstituicaoEnsino();
		if (instituicaoEnsino != null) {
			dto.setInstituicaoEnsinoId(instituicaoEnsino.getId());
			dto.setInstituicaoEnsinoString(instituicaoEnsino.getNomeInstituicao());
		}
		
		dto.setProfessorCustoCreditoSemanal(campus.getValorCredito());
		
		StringBuffer str = new StringBuffer(""); 
		int totalAtendimentosP1 = 0;
		int totalAtendimentosP2 = 0;
		dto.getAlunosDemandas().clear();
		for (AlunoDemanda alunoDemanda : domain.getAlunosDemanda()) {
			dto.getAlunosDemandas().add(toAlunoDemandaDTO(alunoDemanda));
			Aluno aluno = alunoDemanda.getAluno();
			str.append(aluno.getMatricula()+"-"+aluno.getNome()+", ");
			if (alunoDemanda.getPrioridade() == 1) {
				totalAtendimentosP1++;	
			} else {
				totalAtendimentosP2++;
			}
		}
		if (str.length() > 1) {
			dto.setNomesAlunos(str.toString().substring(0,str.length()-2));
		} else {
			dto.setNomesAlunos("");
		}
		dto.setQuantidadeAlunosP1(totalAtendimentosP1);
		dto.setQuantidadeAlunosP2(totalAtendimentosP2);
		
//		Demanda demanda = null;
//		for (Demanda d : domain.getOferta().getDemandas()) {
//			if (d.getDisciplina().getId().equals(domain.getDisciplina().getId())) {
//				demanda = d;
//				break;
//			}
//		}
//		List<AlunoDemanda> alunosDemanda = AlunoDemanda.findByDemanda(instituicaoEnsino,demanda);
//		int totalDemandaP1 = 0;
//		int totalDemandaP2 = 0;
//		for (AlunoDemanda ad : alunosDemanda) {
//			if (ad.getPrioridade() == 1) {
//				totalDemandaP1++;
//			} else {
//				totalDemandaP2++;
//			}
//		}
//		dto.setQtdDemandaAlunosP1(totalDemandaP1);
//		dto.setQtdDemandaAlunosP2(totalDemandaP2);
//		dto.setQtdDemandaAlunosTotal(demanda.getQuantidade());

		return dto;
	}
	
	public static AtendimentoTaticoDTO toAtendimentoTaticoDTO(AtendimentoTatico domain, Map<String,Integer[]> demandaKeyToQtdAlunosMap) {
		AtendimentoTaticoDTO dto = toAtendimentoTaticoDTO(domain);
		
		String demandaId = dto.getOfertaId()+"-"+dto.getDisciplinaId();
		Integer[] trio = demandaKeyToQtdAlunosMap.get(demandaId);
		if (trio != null) {
			dto.setQtdDemandaAlunosP1(trio[0]);
			dto.setQtdDemandaAlunosP2(trio[1]);
			dto.setQtdDemandaAlunosTotal(trio[2]);			
		}
		
		if (dto.getQtdDemandaAlunosP1() == null)
			System.out.println(1);

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

		domain.setDisciplina( Disciplina.find(dto.getDisciplinaId(), instituicaoEnsino ) );
		if (dto.getDisciplinaSubstitutaId() != null) {
			domain.setDisciplinaSubstituta(Disciplina.find(dto.getDisciplinaSubstitutaId(),instituicaoEnsino));
		}
		domain.setQuantidadeAlunos( dto.getQuantidadeAlunos() );
		domain.setTurma( dto.getTurma() );

		return domain;
	}
	
	public static AtendimentoOperacionalDTO toAtendimentoOperacionalDTO(
			AtendimentoOperacional domain )
	{
		return toAtendimentoOperacionalDTO(domain, true);
	}
	
	public static AtendimentoOperacionalDTO toAtendimentoOperacionalDTOSemAlunosDemanda(
			AtendimentoOperacional domain )
	{
		return toAtendimentoOperacionalDTO(domain, false);
	}

	public static AtendimentoOperacionalDTO toAtendimentoOperacionalDTO(
			AtendimentoOperacional domain, boolean processaAlunosDemanda )
		{
			AtendimentoOperacionalDTO dto = new AtendimentoOperacionalDTO();

			InstituicaoEnsino instituicaoEnsino
				= domain.getInstituicaoEnsino();
				
			Sala sala = domain.getSala();
			Unidade unidade = sala.getUnidade();
			Campus campus = unidade.getCampus();

			dto.setId( domain.getId() );
			dto.setVersion( domain.getVersion() );
			dto.setCenarioId( domain.getCenario().getId() );
			dto.setCampusId( campus.getId() );
			dto.setCampusString( campus.getCodigo() );
			dto.setUnidadeId( unidade.getId() );
			dto.setUnidadeString( unidade.getCodigo() );
			dto.setSalaId( sala.getId() );
			dto.setSalaString( sala.getCodigo() );

			HorarioDisponivelCenario hdc = domain.getHorarioDisponivelCenario();
			dto.setHorarioDisponivelCenarioId( hdc.getId() );
			dto.setSemana( Semanas.toInt( hdc.getDiaSemana() ) );
			dto.setDiaSemana( dto.getSemana() );

			HorarioAula ha = hdc.getHorarioAula();
			dto.setHorarioAulaId( ha.getId() );
			dto.setHorarioAulaString( TriedaUtil.shortTimeString( ha.getHorario() ) );
			SemanaLetiva semanaLetiva = ha.getSemanaLetiva();
			dto.setSemanaLetivaId( semanaLetiva.getId() );
			dto.setSemanaLetivaTempoAula(semanaLetiva.getTempo());
			
			ProfessorVirtual professorVirtual = domain.getProfessorVirtual();
			Oferta oferta = domain.getOferta();
			dto.setTurnoId( oferta.getTurno().getId() );
			dto.setTurnoString( oferta.getTurno().getNome() );
			
			Disciplina disciplinaSubstituta = domain.getDisciplinaSubstituta();
			Disciplina disciplina = domain.getDisciplina();

			if ( professorVirtual == null )
			{
				Professor professor = domain.getProfessor();
				dto.setProfessorId( professor.getId() );
				dto.setProfessorCPF( professor.getCpf() );
				dto.setProfessorString( professor.getNome() );
				dto.setProfessorCustoCreditoSemanal(professor.getValorCredito());
			}
			else
			{
				dto.setProfessorVirtualId( professorVirtual.getId() );
				dto.setProfessorCPF( professorVirtual.getId().toString() );
				dto.setProfessorVirtualString("Professor Virtual " + professorVirtual.getId() );
				dto.setProfessorCustoCreditoSemanal(oferta.getCampus().getValorCredito());
			}

			dto.setCreditoTeoricoBoolean( domain.getCreditoTeorico() );
			dto.setOfertaId( oferta.getId() );
			dto.setDisciplinaId( disciplina.getId() );
			dto.setDisciplinaString( disciplina.getCodigo() );
			dto.setDisciplinaNome( disciplina.getNome() );
			dto.setDisciplinaOriginalCodigo(disciplina.getCodigo());
			dto.setDisciplinaUsaLaboratorio(disciplina.getLaboratorio());
			if (disciplinaSubstituta != null) {
				dto.setDisciplinaSubstitutaId( disciplinaSubstituta.getId() );
				dto.setDisciplinaSubstitutaString( disciplinaSubstituta.getCodigo() );
				dto.setDisciplinaSubstitutaNome( disciplinaSubstituta.getNome() );
				dto.setTotalCreditoDisciplinaSubstituta(disciplinaSubstituta.getCreditosTotal());
				dto.setDisciplinaSubstitutaSemanaLetivaId(semanaLetiva.getId());
				dto.setDisciplinaSubstitutaSemanaLetivaTempoAula(semanaLetiva.getTempo());
			}
			dto.setTotalCreditosTeoricosDisciplina( disciplina.getCreditosTeorico() );
			dto.setTotalCreditosPraticosDisciplina( disciplina.getCreditosPratico() );
			dto.setTotalCreditoDisciplina( disciplina.getTotalCreditos() );
			dto.setQuantidadeAlunos( domain.getQuantidadeAlunos() );
			dto.setQuantidadeAlunosString( domain.getQuantidadeAlunos().toString() );
			dto.setTurma( domain.getTurma() );
			
			Curriculo curriculo = oferta.getCurriculo();
			Curso curso = curriculo.getCurso();

			dto.setCursoString( curso.getCodigo() );
			dto.setCursoNome( curso.getNome() );
			dto.setCursoId( curso.getId() );
			dto.setCurricularString( curriculo.getCodigo() );
			dto.setCurricularId( curriculo.getId() );

			int periodo = curriculo.getPeriodo(disciplina);

			dto.setPeriodo( periodo );
			dto.setPeriodoString( String.valueOf( periodo ) );

			dto.setTotalCreditos(1); // todo atendimento operacional representa apenas 1 crédito atendido
			dto.setDisplayText( dto.getNaturalKey() );
			dto.setCompartilhamentoCursosString( "" );

			if ( instituicaoEnsino != null )
			{
				dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
				dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
			}
			
			StringBuffer str = new StringBuffer("");
			int totalAtendimentosP1 = 0;
			int totalAtendimentosP2 = 0;
			dto.getAlunosDemandas().clear();
			if (processaAlunosDemanda)
			{
				for (AlunoDemanda alunoDemanda : domain.getAlunosDemanda()) {
					dto.getAlunosDemandas().add(toAlunoDemandaDTO(alunoDemanda));
					Aluno aluno = alunoDemanda.getAluno();
					str.append(aluno.getMatricula()+"-"+aluno.getNome()+", ");
					if (alunoDemanda.getPrioridade() == 1) {
						totalAtendimentosP1++;	
					} else {
						totalAtendimentosP2++;
					}
				}
			}
			if (str.length() > 1) {
				dto.setNomesAlunos(str.toString().substring(0,str.length()-2));
			} else {
				dto.setNomesAlunos("");
			}
			dto.setQuantidadeAlunosP1(totalAtendimentosP1);
			dto.setQuantidadeAlunosP2(totalAtendimentosP2);
			
//			Demanda demanda = null;
//			for (Demanda d : oferta.getDemandas()) {
//				if (d.getDisciplina().getId().equals(disciplina.getId())) {
//					demanda = d;
//					break;
//				}
//			}
//			List<AlunoDemanda> alunosDemanda = AlunoDemanda.findByDemanda(instituicaoEnsino,demanda);
//			int totalDemandaP1 = 0;
//			int totalDemandaP2 = 0;
//			for (AlunoDemanda ad : alunosDemanda) {
//				if (ad.getPrioridade() == 1) {
//					totalDemandaP1++;
//				} else {
//					totalDemandaP2++;
//				}
//			}
//			dto.setQtdDemandaAlunosP1(totalDemandaP1);
//			dto.setQtdDemandaAlunosP2(totalDemandaP2);
//			dto.setQtdDemandaAlunosTotal(demanda.getQuantidade());

			return dto;
		}

	public static AtendimentoOperacionalDTO toAtendimentoOperacionalDTO(AtendimentoOperacional domain, Map<String,Integer[]> demandaKeyToQtdAlunosMap) {
		AtendimentoOperacionalDTO dto = toAtendimentoOperacionalDTO(domain);
		
		String demandaId = dto.getOfertaId()+"-"+dto.getDisciplinaId();
		Integer[] trio = demandaKeyToQtdAlunosMap.get(demandaId);
		if (trio != null) {
			dto.setQtdDemandaAlunosP1(trio[0]);
			dto.setQtdDemandaAlunosP2(trio[1]);
			dto.setQtdDemandaAlunosTotal(trio[2]);			
		}

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
		domain.setInstituicaoEnsino( instituicaoEnsino );

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

		InstituicaoEnsino instituicaoEnsino
			= domain.getInstituicaoEnsino();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );

		if ( domain.getCenario().size() > 0 )
		{
			dto.setCenarioId( ( new ArrayList< Cenario >(
				domain.getCenario() ) ).get( 0 ).getId() );
		}

		Disciplina disciplina = domain.getDisciplina();

		if ( disciplina != null )
		{
			dto.setDisciplinaId( disciplina.getId() );
			dto.setDisciplinaString( disciplina.getCodigo()
				+ " (" + disciplina.getNome() + ")" );
		}

		dto.setDia1( domain.getDia1() );
		dto.setDia2( domain.getDia2() );
		dto.setDia3( domain.getDia3() );
		dto.setDia4( domain.getDia4() );
		dto.setDia5( domain.getDia5() );
		dto.setDia6( domain.getDia6() );
		dto.setDia7( domain.getDia7() );
		dto.setTotalCreditos( domain.getCreditos() );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		return dto;
	}

	// PROFESSOR
	public static Professor toProfessor(ProfessorDTO dto) {
		Professor domain = new Professor();
		InstituicaoEnsino instituicaoEnsino = InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );
		fillProfessorWithProfessorDTO(domain, dto, instituicaoEnsino);
		return domain;
	}

	private static void fillProfessorWithProfessorDTO(Professor domain, ProfessorDTO dto,  InstituicaoEnsino instituicaoEnsino) {
		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setCenario( Cenario.find(dto.getCenarioId(), instituicaoEnsino) );
		domain.setNome( dto.getNome() );
		domain.setCpf( dto.getCpf() );
		domain.setTipoContrato( TipoContrato.find(dto.getTipoContratoId(), instituicaoEnsino ) );
		domain.setCargaHorariaMax( dto.getCargaHorariaMax() );
		domain.setCargaHorariaMin( dto.getCargaHorariaMin() );
		domain.setTitulacao( Titulacao.find( dto.getTitulacaoId(), instituicaoEnsino ) );
		domain.setAreaTitulacao( AreaTitulacao.find(dto.getAreaTitulacaoId(), instituicaoEnsino ) );
		domain.setCreditoAnterior( dto.getCreditoAnterior() );
		domain.setValorCredito( dto.getValorCredito().getDoubleValue() );
		domain.setMaxDiasSemana( dto.getMaxDiasSemana() );
		domain.setMinCreditosDia( dto.getMinCreditosDia() );
	}
	
	public static Professor toProfessorComCampiTrabalho(ProfessorDTO dto) {
		InstituicaoEnsino instituicaoEnsino = InstituicaoEnsino.find(dto.getInstituicaoEnsinoId());
		
		Professor domain = null;
		if (dto.getId() != null) {
			domain = Professor.find(dto.getId(),instituicaoEnsino); // esta busca pelo professor preenche a sua lista de campi
		}
		else {
			domain = new Professor();
		}
		
		fillProfessorWithProfessorDTO(domain,dto,instituicaoEnsino);
		
		return domain;
	}
	
	public static ProfessorDTO toProfessorDTO( ProfessorVirtual domain ){
		ProfessorDTO dto = new ProfessorDTO();
		
		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCenarioId( domain.getCenario().getId() );
		dto.setNome( domain.getNome() );
		dto.setCargaHorariaMax( domain.getCargaHorariaMax() );
		dto.setCargaHorariaMin( domain.getCargaHorariaMin() );
		
		
		return dto;
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
		dto.setAreaTitulacaoId( domain.getAreaTitulacao() == null ? null : domain.getAreaTitulacao().getId() );
		dto.setAreaTitulacaoString( domain.getAreaTitulacao() == null ? null : domain.getAreaTitulacao().getCodigo() );
		dto.setCreditoAnterior( domain.getCreditoAnterior() );
		dto.setValorCredito( TriedaUtil.parseTriedaCurrency( domain.getValorCredito() ) );
		dto.setDisplayText( domain.getNome() + " (" + domain.getCpf() + ")" );
		dto.setNotaDesempenho( ConvertBeans.getNotaDesempenho( domain ).intValue() );
		dto.setMaxDiasSemana(domain.getMaxDiasSemana());
		dto.setMinCreditosDia(domain.getMinCreditosDia());

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		return dto;
	}
	
	public static ProfessorDTO toProfessorDTO( Professor domain, Integer cargaHoraria, Integer totalCred )
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
		dto.setAreaTitulacaoId( domain.getAreaTitulacao() == null ? null : domain.getAreaTitulacao().getId() );
		dto.setAreaTitulacaoString( domain.getAreaTitulacao() == null ? null : domain.getAreaTitulacao().getCodigo() );
		dto.setCreditoAnterior( domain.getCreditoAnterior() );
		dto.setValorCredito( TriedaUtil.parseTriedaCurrency( domain.getValorCredito() ) );
		dto.setDisplayText( domain.getNome() + " (" + domain.getCpf() + ")" );
		dto.setNotaDesempenho( ConvertBeans.getNotaDesempenho( domain ).intValue() );
		dto.setMaxDiasSemana(domain.getMaxDiasSemana());
		dto.setMinCreditosDia(domain.getMinCreditosDia());
		dto.setCargaHorariaSemanal( cargaHoraria );
		dto.setTotalCred( totalCred );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		return dto;
	}
	
	private static Integer getCargaHorariaSemanal(Professor p) {
		int totalCred = 0;
		Set<String> creditos = new HashSet<String>();
		for (AtendimentoOperacional atendimento : p.getAtendimentosOperacionais())
		{
			String key = atendimento.getTurma() + "-" + 
					(atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getCodigo() : atendimento.getDisciplina().getCodigo())
						+ "-" + atendimento.getHorarioDisponivelCenario().getId();
			if (!creditos.contains(key))
			{
				totalCred += atendimento.getHorarioDisponivelCenario().getHorarioAula().getSemanaLetiva().getTempo();
				creditos.add(key);
			}
		}
		return totalCred;
	}

	private static Integer getTotalCred(Professor p) {
		Set<String> creditos = new HashSet<String>();
		for (AtendimentoOperacional atendimento : p.getAtendimentosOperacionais())
		{
			creditos.add(atendimento.getTurma() + "-" + 
					(atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getCodigo() : atendimento.getDisciplina().getCodigo())
						+ "-" + atendimento.getHorarioDisponivelCenario().getId());
		}
		
		return creditos.size();
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

		InstituicaoEnsino instituicaoEnsino = null;

		if ( domain != null && domain.getProfessor() != null
			&& domain.getProfessor().getTipoContrato() != null )
		{
			instituicaoEnsino = domain.getProfessor().getTipoContrato().getInstituicaoEnsino();
		}

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setProfessorId( domain.getProfessor().getId() );
		dto.setProfessorString( domain.getProfessor().getNome() );
		dto.setProfessorCpf( domain.getProfessor().getCpf() );
		dto.setDisciplinaId( domain.getDisciplina().getId() );
		dto.setDisciplinaString( domain.getDisciplina().getCodigo() );
		dto.setPreferencia( domain.getPreferencia() );
		dto.setNotaDesempenho( domain.getNota() );

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		dto.setDisplayText( domain.getDisciplina().getCodigo()
			+ "-" + domain.getProfessor().getNome() );

		return dto;
	}

	// PROFESSOR VIRTUAL
	public static ProfessorVirtual toProfessorVirtual(
		ProfessorVirtualDTO dto )
	{
		ProfessorVirtual domain = new ProfessorVirtual();

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );
		Cenario cenario
			= Cenario.find(dto.getCenarioId(), instituicaoEnsino);

		domain.setInstituicaoEnsino( instituicaoEnsino );
		domain.setCenario( cenario );
		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setCargaHorariaMax( dto.getCargaHorariaMax() );
		domain.setCargaHorariaMin( dto.getCargaHorariaMin() );

		domain.setTitulacao( Titulacao.find(
			dto.getTitulacaoId(), instituicaoEnsino ) );

		domain.setAreaTitulacao( AreaTitulacao.find(
			dto.getAreaTitulacaoId(), instituicaoEnsino ) );
		
		domain.setAreaTitulacao( AreaTitulacao.find(
				dto.getAreaTitulacaoId(), instituicaoEnsino ) );
		
		domain.setTipoContrato( TipoContrato.find(
				dto.getTipoContratoId(), instituicaoEnsino ) );

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
		
		TipoContrato tipoContrato = domain.getTipoContrato();
		
		if ( tipoContrato != null )
		{
			dto.setTipoContratoId( tipoContrato.getId() );
			dto.setTipoContratoString( tipoContrato.getNome() );
		}

		InstituicaoEnsino instituicaoEnsino
			= domain.getInstituicaoEnsino();

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}
		
		Cenario cenario = domain.getCenario();
		if ( cenario != null )
		{
			dto.setCenarioId( cenario.getId() );
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
		domain.setElimina( Disciplina.find(
				dto.getEliminaId(), instituicaoEnsino) );
		domain.setEquivalenciaGeral( dto.getEquivalenciaGeral() );
		
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
		dto.setCursouCodigo( domain.getCursou().getCodigo());
		dto.setCursouNome( domain.getCursou().getNome());
		dto.setEliminaId( domain.getElimina().getId()  );
		dto.setEliminaCodigo( domain.getElimina().getCodigo());
		dto.setEliminaNome( domain.getElimina().getNome());
		dto.setEquivalenciaGeral( domain.getEquivalenciaGeral() );
		if ( domain.getCursos().isEmpty() || domain.getEquivalenciaGeral() )
		{
			dto.setCursoString( "GERAL" );
		}
		else
		{
			String cursos = "";
			for (Curso curso : domain.getCursos() )
			{
				if (cursos == "")
					cursos += curso.getNome();
				else
					cursos += ", " + curso.getNome();
			}
			dto.setCursoString( cursos );
		}

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
		domain.setInstituicaoEnsino(instituicaoEnsino);

		Cenario cenario = Cenario.find(
			dto.getCenarioId(), instituicaoEnsino );

		Cenario.entityManager().refresh( cenario );
		domain.setCenario( cenario );
		domain.setModoOtimizacao( dto.getModoOtimizacao() );
		
		List<Long> campiIDs = new ArrayList<Long>(dto.getCampi().size());
		for (CampusDTO campusDTO : dto.getCampi()) {
			campiIDs.add(campusDTO.getId());
		}
		List<Campus> campi = Campus.find(campiIDs,instituicaoEnsino);
		domain.setCampi(new HashSet<Campus>(campi));

		List<Long> turnosIDs = new ArrayList<Long>(dto.getTurnos().size());
		for (TurnoDTO turnosDTO : dto.getTurnos()) {
			turnosIDs.add(turnosDTO.getId());
		}
		List<Turno> turnos = Turno.find(turnosIDs, instituicaoEnsino);
		domain.setTurnos(new HashSet<Turno>(turnos));

		domain.setCargaHorariaAluno( dto.getCargaHorariaAluno() );
		domain.setCargaHorariaAlunoSel( dto.getCargaHorariaAlunoSel() );
		domain.setAlunoDePeriodoMesmaSala( dto.getAlunoDePeriodoMesmaSala() );
		domain.setAlunoEmMuitosCampi( dto.getAlunoEmMuitosCampi() );
		domain.setMinimizarDeslocamentoAluno( dto.getMinimizarDeslocamentoAluno() );
		domain.setCargaHorariaProfessor( dto.getCargaHorariaProfessor() );
		domain.setCargaHorariaProfessorSel( dto.getCargaHorariaProfessorSel() );
		domain.setProfessorEmMuitosCampi( dto.getProfessorEmMuitosCampi() );
		domain.setMinimizarDeslocamentoProfessor( dto.getMinimizarDeslocamentoProfessor() );
		domain.setMinimizarDeslocamentoProfessorValue( dto.getMinimizarDeslocamentoProfessorValue() );
		domain.setMinimizarGapProfessor( dto.getMinimizarGapProfessor() );
		domain.setProibirGapProfessor( dto.getProibirGapProfessor() );
		domain.setEvitarReducaoCargaHorariaProfessor( dto.getEvitarReducaoCargaHorariaProfessor() );
		domain.setEvitarReducaoCargaHorariaProfessorValue( dto.getEvitarReducaoCargaHorariaProfessorValue() );
		domain.setEvitarUltimoEPrimeiroHorarioProfessor( dto.getEvitarUltimoEPrimeiroHorarioProfessor() );
		domain.setEvitarUltimoEPrimeiroHorarioProfessorValue( dto.getEvitarUltimoEPrimeiroHorarioProfessorValue() );
		domain.setPreferenciaDeProfessores( dto.getPreferenciaDeProfessores() );
		domain.setAvaliacaoDesempenhoProfessor( dto.getAvaliacaoDesempenhoProfessor() );
		domain.setConsiderarEquivalencia( dto.getConsiderarEquivalencia() );
		domain.setProibirCiclosEmEquivalencia( dto.getProibirCiclosEmEquivalencia() );
		domain.setConsiderarTransitividadeEmEquivalencia( dto.getConsiderarTransitividadeEmEquivalencia() );
		domain.setProibirTrocaPorDiscOnlineOuCredZeradosEmEquivalencia( dto.getProibirTrocaPorDiscOnlineOuCredZeradosEmEquivalencia() );
		domain.setMinAlunosParaAbrirTurma( dto.getMinAlunosParaAbrirTurma() );
		domain.setMinAlunosParaAbrirTurmaValue( dto.getMinAlunosParaAbrirTurmaValue() );
		domain.setViolarMinTurmasFormandos( dto.getViolarMinAlunosAbrirTurmaParaFormandos() );
		domain.setNivelDificuldadeDisciplina( dto.getNivelDificuldadeDisciplina() );
		domain.setCompatibilidadeDisciplinasMesmoDia( dto.getCompatibilidadeDisciplinasMesmoDia() );
		domain.setRegrasGenericasDivisaoCredito( dto.getRegrasGenericasDivisaoCredito() );
		domain.setRegrasEspecificasDivisaoCredito( dto.getRegrasEspecificasDivisaoCredito() );
		domain.setMaximizarNotaAvaliacaoCorpoDocente( dto.getMaximizarNotaAvaliacaoCorpoDocente() );
		domain.setMinimizarCustoDocenteCursos( dto.getMinimizarCustoDocenteCursos() );
		domain.setCompartilharDisciplinasCampi( dto.getCompartilharDisciplinasCampi() );
		domain.setPercentuaisMinimosMestres( dto.getPercentuaisMinimosMestres() );
		domain.setPercentuaisMinimosDoutores( dto.getPercentuaisMinimosDoutores() );
		domain.setAreaTitulacaoProfessoresECursos( dto.getAreaTitulacaoProfessoresECursos() );
		domain.setLimitarMaximoDisciplinaProfessor( dto.getLimitarMaximoDisciplinaProfessor() );
		domain.setUtilizarDemandasP2( dto.getConsiderarDemandasDePrioridade2() );
		domain.setCargaHorariaAlunoSel( dto.getCargaHorariaAlunoSel() );
		domain.setCargaHorariaProfessorSel( dto.getCargaHorariaProfessorSel() );
		domain.setFuncaoObjetivo( dto.getFuncaoObjetivo() );
		domain.setOtimizarPor(dto.getOtimizarPor());
		domain.setCapacidadeMaxSalas(dto.getConsiderarCapacidadeMaxSalas());
		domain.setPercentuaisMinimosParcialIntegral(dto.getPercentuaisMinimosParcialIntegral());
		domain.setPercentuaisMinimosIntegral(dto.getPercentuaisMinimosIntegral());
		domain.setConsiderarCoRequisitos(dto.getConsiderarCoRequisitos());
		domain.setPriorizarCalouros(dto.getPriorizarCalouros());
		domain.setConsiderarPrioridadePorAlunos(dto.getConsiderarPrioridadePorAlunos());

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

		return domain;
	}

	public static ParametroDTO toParametroDTO( Parametro domain )
	{
		ParametroDTO dto = new ParametroDTO();

		InstituicaoEnsino instituicaoEnsino
			 = domain.getCenario().getInstituicaoEnsino();

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCenarioId( domain.getCenario().getId() );
		dto.setModoOtimizacao( domain.getModoOtimizacao() );

		List<CampusDTO> campiDTOs = new ArrayList<CampusDTO>(domain.getCampi().size());
		for (Campus campus : domain.getCampi()) {
			campiDTOs.add(toCampusDTO(campus));
		}
		dto.setCampi(campiDTOs);

		List<TurnoDTO> turnosDTOs = new ArrayList<TurnoDTO>(domain.getTurnos().size());
		for (Turno turno : domain.getTurnos()) {
			turnosDTOs.add(toTurnoDTO(turno));
		}
		dto.setTurnos(turnosDTOs);

		dto.setCargaHorariaAluno( domain.getCargaHorariaAluno() );
		dto.setAlunoDePeriodoMesmaSala( domain.getAlunoDePeriodoMesmaSala() );
		dto.setAlunoEmMuitosCampi( domain.getAlunoEmMuitosCampi() );
		dto.setMinimizarDeslocamentoAluno( domain.getMinimizarDeslocamentoAluno() );
		dto.setCargaHorariaProfessor( domain.getCargaHorariaProfessor() );
		dto.setProfessorEmMuitosCampi( domain.getProfessorEmMuitosCampi() );
		dto.setMinimizarDeslocamentoProfessor( domain.getMinimizarDeslocamentoProfessor() );
		dto.setMinimizarGapProfessor( domain.getMinimizarGapProfessor() );
		dto.setProibirGapProfessor( domain.getProibirGapProfessor() );
		if( domain.getProibirGapProfessor()){
			dto.setMinimizarGapProfessor( false );
		}
		dto.setEvitarReducaoCargaHorariaProfessor( domain.getEvitarReducaoCargaHorariaProfessor() );
		dto.setEvitarUltimoEPrimeiroHorarioProfessor( domain.getEvitarUltimoEPrimeiroHorarioProfessor() );
		dto.setEvitarUltimoEPrimeiroHorarioProfessorValue( domain.getEvitarUltimoEPrimeiroHorarioProfessorValue() );
		dto.setPreferenciaDeProfessores( domain.getPreferenciaDeProfessores() );
		dto.setAvaliacaoDesempenhoProfessor( domain.getAvaliacaoDesempenhoProfessor() );
		dto.setConsiderarEquivalencia( domain.getConsiderarEquivalencia() );
		dto.setProibirCiclosEmEquivalencia( domain.getProibirCiclosEmEquivalencia() );
		dto.setConsiderarTransitividadeEmEquivalencia( domain.getConsiderarTransitividadeEmEquivalencia() );
		dto.setProibirTrocaPorDiscOnlineOuCredZeradosEmEquivalencia( domain.getProibirTrocaPorDiscOnlineOuCredZeradosEmEquivalencia() );
		dto.setMinAlunosParaAbrirTurma( domain.getMinAlunosParaAbrirTurma() );
		dto.setMinAlunosParaAbrirTurmaValue( domain.getMinAlunosParaAbrirTurmaValue() );
		dto.setViolarMinAlunosAbrirTurmaParaFormandos( domain.getViolarMinTurmasFormandos() );
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
		dto.setConsiderarDemandasDePrioridade2( domain.getUtilizarDemandasP2() );
		dto.setCargaHorariaAlunoSel( domain.getCargaHorariaAlunoSel() );
		dto.setCargaHorariaProfessorSel( domain.getCargaHorariaProfessorSel() );
		dto.setMinimizarDeslocamentoProfessorValue( domain.getMinimizarDeslocamentoProfessorValue() );
		dto.setEvitarReducaoCargaHorariaProfessorValue( domain.getEvitarReducaoCargaHorariaProfessorValue() );
		dto.setFuncaoObjetivo( domain.getFuncaoObjetivo() );
		dto.setOtimizarPor(domain.getOtimizarPor());
		dto.setConsiderarCapacidadeMaxSalas(domain.getCapacidadeMaxSalas());
		dto.setPercentuaisMinimosParcialIntegral(domain.getPercentuaisMinimosParcialIntegral());
		dto.setPercentuaisMinimosIntegral(domain.getPercentuaisMinimosIntegral());
		dto.setConsiderarCoRequisitos(domain.getConsiderarCoRequisitos());
		dto.setPriorizarCalouros(domain.getPriorizarCalouros());
		dto.setConsiderarPrioridadePorAlunos(domain.getConsiderarPrioridadePorAlunos());

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

		dto.setId( domain.getId() );
		dto.setVersion( domain.getVersion() );
		dto.setCurso1Id( domain.getCurso1().getId() );
		dto.setCurso1Display( domain.getCurso1().getNome() );
		dto.setCurso2Id( domain.getCurso2().getId() );
		dto.setCurso2Display( domain.getCurso2().getNome() );
		dto.setParametroId( domain.getParametro().getId() );

		return dto;
	}

	public static CursoDescompartilha toCursoDescompartilha(CursoDescompartilhaDTO dto) {
		CursoDescompartilha domain = new CursoDescompartilha();

		InstituicaoEnsino instituicaoEnsino = InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setCurso1( Curso.find( dto.getCurso1Id(), instituicaoEnsino ) );
		domain.setCurso2( Curso.find( dto.getCurso2Id(), instituicaoEnsino ) );
		domain.setParametro(Parametro.find(dto.getParametroId(), instituicaoEnsino));

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
		domain.setPeriodo( dto.getPeriodo() );
		domain.setAtendido( ( dto.getAlunoAtendido() == null ) ?
			false : dto.getAlunoAtendido() );
		domain.setExigeEquivalenciaForcada(dto.getExigeEquivalenciaForcada());
		domain.setPrioridade(dto.getAlunoPrioridade());

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
		dto.setPeriodo( domain.getPeriodo() );
		dto.setAlunoAtendido( ( domain.getAtendido() == null ) ?
			false : domain.getAtendido() );
		dto.setExigeEquivalenciaForcada(domain.getExigeEquivalenciaForcada());
		dto.setAlunoPrioridade(domain.getPrioridade());

		if ( dto.getAlunoAtendido() )
		{
			dto.setAlunoAtendidoString( "Sim" );
		}
		else
		{
			dto.setAlunoAtendidoString(
				HtmlUtils.htmlUnescape( "N&atilde;o" ) );
		}
		
		if ( dto.getExigeEquivalenciaForcada() )
		{
			dto.setExigeEquivalenciaForcadaString( "Sim" );
		}
		else
		{
			dto.setExigeEquivalenciaForcadaString(
				HtmlUtils.htmlUnescape( "Não" ) );
		}

		// Aluno
		dto.setIdAluno( domain.getAluno().getId() );
		dto.setAlunoString( domain.getAluno().getNome() );
		dto.setAlunoMatricula( domain.getAluno().getMatricula() );
		dto.setMotivoNaoAtendimento( domain.getMotivoNaoAtendimento() );
		
		// Demanda
		dto.setCampusString( domain.getDemanda().getOferta().getCampus().getCodigo() );
		dto.setTurnoString( domain.getDemanda().getOferta().getTurno().getNome() );
		dto.setCursoString( domain.getDemanda().getOferta().getCurso().getCodigo() );
		dto.setCurriculoString( domain.getDemanda().getOferta().getCurriculo().getCodigo() );
		dto.setDemandaId( domain.getDemanda().getId() );

		String demandaStr = domain.getDemanda().getOferta().getCampus().getCodigo()
			+ " - " + domain.getDemanda().getOferta().getCurriculo().getCodigo()
			+ " - " + domain.getDemanda().getOferta().getTurno().getNome()
			+ " - " + domain.getDemanda().getDisciplina().getCodigo();
		dto.setDemandaString( demandaStr );

		InstituicaoEnsino instituicaoEnsino
			= domain.getDemanda().getOferta().getCampus().getInstituicaoEnsino();
		
		// Instituição de Ensino
		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );
		}

		// Disciplina
		dto.setDisciplinaId( domain.getDemanda().getDisciplina().getId() );
		dto.setDisciplinaString( domain.getDemanda().getDisciplina().getCodigo() );

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

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );

		Cenario cenario = Cenario.find(
			dto.getCenarioId(), instituicaoEnsino );

		domain.setId( dto.getId() );
		domain.setVersion( dto.getVersion() );
		domain.setNome( dto.getNome() );
		domain.setMatricula( dto.getMatricula() );
		domain.setFormando( dto.getFormando() );
		domain.setPeriodo( dto.getPeriodo() );
		domain.setPrioridade( dto.getPrioridade() );
		domain.setVirtual( dto.getVirtual() );
		domain.setCriadoTrieda( dto.getCriadoTrieda() );
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
		dto.setMatricula( domain.getMatricula() );
		dto.setFormando( domain.getFormando() );
		dto.setPeriodo( domain.getPeriodo() );
		dto.setPrioridade( domain.getPrioridade() );
		dto.setVirtual( domain.getVirtual() );
		dto.setCriadoTrieda( domain.getCriadoTrieda() );
		dto.setCenarioId( domain.getCenario().getId() );
		dto.setDisplayText( domain.getNome() );

		InstituicaoEnsino instituicaoEnsino
			= domain.getInstituicaoEnsino();

		if ( instituicaoEnsino != null )
		{
			dto.setInstituicaoEnsinoId( instituicaoEnsino.getId() );
			dto.setInstituicaoEnsinoString( instituicaoEnsino.getNomeInstituicao() );			
		}

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
	
	public static RequisicaoOtimizacaoDTO toRequisicaoOtimizacaoDTO(RequisicaoOtimizacao domain) {
		RequisicaoOtimizacaoDTO dto = new RequisicaoOtimizacaoDTO();
		
		dto.setId(domain.getId());
		dto.setCenarioId(domain.getCenario().getId());
		dto.setCenarioNome("["+domain.getCenario().getId()+"] "+domain.getCenario().getNome());
		dto.setParametroId(domain.getParametro().getId());
		dto.setRound(domain.getRound());
		dto.setUserName(domain.getUsuario().getUsername());
		dto.setUserNameCancel(domain.getUsuarioCancel() == null ? null : domain.getUsuarioCancel().getUsername() );
		
		dto.setStatusIndex(domain.getStatusCodigo());
		
		Parametro parametro = domain.getParametro();
		dto.setModoOtimizacao(parametro.getModoOtimizacao());
		dto.setOtimizarPor(parametro.getOtimizarPor());
		dto.setFuncaoObjetivo(FuncaoObjetivoComboBox.CargaHoraria.values()[parametro.getFuncaoObjetivo()].getText());
		
		dto.setTotalCampi(domain.getTotalCampi());
		dto.setCampiSelecionados(domain.getCampiSelecionados());
		dto.setTotalTurnos(domain.getTotalTurnos());
		dto.setTurnosSelecionados(domain.getTurnosSelecionados());
		dto.setTotalAlunos(domain.getTotalAlunos());
		dto.setTotalAlunosDemandasP1(domain.getTotalAlunosDemandasP1());
		dto.setTotalAlunosDemandasP2(domain.getTotalAlunosDemandasP2());
		dto.setTotalAmbientes(domain.getTotalAmbientes());
		dto.setTotalProfessores(domain.getTotalProfessores());
		SimpleDateFormat df = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss");
		dto.setInstanteInicioRequisicao(domain.getInstanteInicioRequisicao() == null ? null : df.format(domain.getInstanteInicioRequisicao()));
		dto.setInstanteInicioOtimizacao(domain.getInstanteInicioOtimizacao() == null ? null : df.format(domain.getInstanteInicioOtimizacao()));
		dto.setInstanteTermino(domain.getInstanteTermino() == null ? null : df.format(domain.getInstanteTermino()));
		if (domain.getInstanteInicioRequisicao() != null && domain.getInstanteTermino() != null)
		{
			long duracao = domain.getInstanteTermino().getTime() - domain.getInstanteInicioRequisicao().getTime();
	        long hours = TimeUnit.MILLISECONDS.toHours(duracao);
	        duracao -= TimeUnit.HOURS.toMillis(hours);
	        long minutes = TimeUnit.MILLISECONDS.toMinutes(duracao);
	        duracao -= TimeUnit.MINUTES.toMillis(minutes);
	        long seconds = TimeUnit.MILLISECONDS.toSeconds(duracao);
			dto.setDuracaoRequisicao(String.format("%02d:%02d:%02d", 
					hours,
					minutes,
					seconds	));
		}
		if (domain.getInstanteInicioOtimizacao() != null && domain.getInstanteTermino() != null)
		{
			long duracao = domain.getInstanteTermino().getTime() - domain.getInstanteInicioOtimizacao().getTime();
	        long hours = TimeUnit.MILLISECONDS.toHours(duracao);
	        duracao -= TimeUnit.HOURS.toMillis(hours);
	        long minutes = TimeUnit.MILLISECONDS.toMinutes(duracao);
	        duracao -= TimeUnit.MINUTES.toMillis(minutes);
	        long seconds = TimeUnit.MILLISECONDS.toSeconds(duracao);
			dto.setDuracaoOtimizacao(String.format("%02d:%02d:%02d", 
					hours,
					minutes,
					seconds	));
		}
		
		Set<Long> professoresRelacionadosIDs = new HashSet<Long>();
		for (Campus campus : parametro.getCampi()) {
			for (Professor professor : campus.getProfessores()) {
				professoresRelacionadosIDs.add(professor.getId());
			}
		}
		dto.setProfessoresRelacionadosIDs(professoresRelacionadosIDs);
		
		InstituicaoEnsino instituicaoEnsino = domain.getCenario().getInstituicaoEnsino();
		dto.setInstituicaoEnsinoId(instituicaoEnsino.getId());
		dto.setInstituicaoEnsinoString(instituicaoEnsino.getNomeInstituicao());
		
		return dto;
	}

	public static AtendimentoFaixaDemandaDTO toAtendimentoFaixaDemandaDTO(
			AtendimentoFaixaDemanda domain) {
		AtendimentoFaixaDemandaDTO dto = new AtendimentoFaixaDemandaDTO();
		DecimalFormat dec = new DecimalFormat("#0.00");
		
		dto.setCampusNome( domain.getCampus().getNome() );
		dto.setDemandaP1( domain.getDemandaP1() );
		dto.setAtendimentoP1( domain.getAtendimentoP1() );
		dto.setAtendimentoPercentP1( dec.format(TriedaUtil.round(domain.getAtendimentoPercentP1()*100.0,2))+"%" );
		dto.setAtendimentoSoma(domain.getAtendimentoSoma());
		dto.setAtendimentoSomaPercent( dec.format(TriedaUtil.round(domain.getAtendimentoSomaPercent()*100.0,2))+"%" );
		dto.setTurmasAbertas( domain.getTurmasAbertas() );
		dto.setMediaTurma( domain.getMediaTurma() );
		dto.setCreditosPagos( domain.getCreditosPagos() );
		dto.setDemandaAcumP1( domain.getDemandaAcumP1() );
		dto.setAtendimentoSomaAcum( domain.getAtendimentoSomaAcum() );
		dto.setAtendimentoSomaAcumPercent( dec.format(TriedaUtil.round(domain.getAtendimentoAcumPercent()*100.0,2))+"%" );
		dto.setReceitaSemanal( domain.getReceitaSemanal() );
		dto.setCustoDocenteSemanal( domain.getCustoDocenteSemanal() );
		dto.setCustoDocentePorReceitaPercent(dec.format(TriedaUtil.round(domain.getCustoDocentePercent()*100.0,2))+"%" );
		dto.setReceitaAcumulada( domain.getReceitaAcumulada() );
		dto.setCustoDocenteAcumulado( domain.getCustoDocenteAcumulado() );
		dto.setCustoDocentePorReceitaAcumuladoPercent(dec.format(TriedaUtil.round(domain.getCustoDocentePorReceitaAcumuladoPercent()*100.0,2))+"%" );
		
		if(domain.getFaixaInferior() == 0 && domain.getFaixaSuperior() > 0) {
			dto.setDemandaDisc("Acima de " + domain.getFaixaSuperior() + " alunos");
		}
		else if (domain.getFaixaInferior() > 0 && domain.getFaixaSuperior() == 0) {
			dto.setDemandaDisc("Abaixo de " + domain.getFaixaInferior() + " alunos");
		}
		else if (domain.getFaixaInferior() != 0 && domain.getFaixaSuperior() != 0) {
			dto.setDemandaDisc("Entre " + domain.getFaixaSuperior() + " e " + domain.getFaixaInferior() + " alunos");
		}

		return dto;
		
	}
	
	public static List < AtendimentoFaixaDemandaDTO > toListAtendimentoFaixaDemandaDTO(
			List< AtendimentoFaixaDemanda > listDomains )
		{
			if ( listDomains == null )
			{
				return Collections.< AtendimentoFaixaDemandaDTO > emptyList();
			}

			List< AtendimentoFaixaDemandaDTO > listDTOs
				= new ArrayList< AtendimentoFaixaDemandaDTO >();

			for ( AtendimentoFaixaDemanda domain : listDomains )
			{
				listDTOs.add( ConvertBeans.toAtendimentoFaixaDemandaDTO( domain ) );
			}

			return listDTOs;
		}
	
	public static ParametroGeracaoDemandaDTO toParametroGeracaoDemandaDTO(ParametroGeracaoDemanda domain)
	{
		ParametroGeracaoDemandaDTO dto = new ParametroGeracaoDemandaDTO();
		
		dto.setCampusId(domain.getCampus().getId());
		dto.setTurnoId(domain.getTurno().getId());
		dto.setUsarDemandasDePrioridade2(domain.getUsarDemandasDePrioridade2());
		dto.setDistanciaMaxEmPeriodosParaPrioridade2(domain.getDistanciaMaxEmPeriodosParaPrioridade2());
		dto.setConsiderarPreRequisitos(domain.getConsiderarPreRequisitos());
		dto.setConsiderarCoRequisitos(domain.getConsiderarCoRequisitos());
		dto.setMaxCreditosPeriodo(domain.getMaxCreditosPeriodo());
		dto.setMaxCreditosManual(domain.getMaxCreditosManual());
		dto.setAumentaMaxCreditosParaAlunosComDisciplinasAtrasadas(domain.getAumentaMaxCreditosParaAlunosComDisciplinasAtrasadas());
		dto.setFatorDeAumentoDeMaxCreditos(domain.getFatorDeAumentoDeMaxCreditos());
		
		return dto;
	}
	
	public static ParametroGeracaoDemanda toParametroGeracaoDemanda(ParametroGeracaoDemandaDTO dto)
	{
		ParametroGeracaoDemanda domain = new ParametroGeracaoDemanda();
		
		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );
		Campus campus = Campus.find(dto.getCampusId(), instituicaoEnsino);
		Turno turno = Turno.find(dto.getTurnoId(), instituicaoEnsino);
		
		domain.setCampus(campus);
		domain.setTurno(turno);
		domain.setUsarDemandasDePrioridade2(dto.getUsarDemandasDePrioridade2());
		domain.setDistanciaMaxEmPeriodosParaPrioridade2(dto.getDistanciaMaxEmPeriodosParaPrioridade2());
		domain.setConsiderarPreRequisitos(dto.getConsiderarPreRequisitos());
		domain.setConsiderarCoRequisitos(dto.getConsiderarCoRequisitos());
		domain.setMaxCreditosPeriodo(dto.getMaxCreditosPeriodo());
		domain.setMaxCreditosManual(dto.getMaxCreditosManual());
		domain.setAumentaMaxCreditosParaAlunosComDisciplinasAtrasadas(dto.getAumentaMaxCreditosParaAlunosComDisciplinasAtrasadas());
		domain.setFatorDeAumentoDeMaxCreditos(dto.getFatorDeAumentoDeMaxCreditos());
		
		return domain;
	}
	
	public static ParametroConfiguracao toParametroConfiguracao( ParametroConfiguracaoDTO dto )
	{
		ParametroConfiguracao domain = new ParametroConfiguracao();
		
		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );
		
		domain.setNomeOtimizacao( dto.getNomeOtimizacao() );
		domain.setUrlOtimizacao( dto.getUrlOtimizacao() );
		domain.setInstituicaoEnsino( instituicaoEnsino );
		
		return domain;
	}
	
	public static ParametroConfiguracaoDTO toParametroConfiguracaoDTO( ParametroConfiguracao domain )
	{
		ParametroConfiguracaoDTO dto = new ParametroConfiguracaoDTO();
		
		dto.setInstituicaoEnsinoId( domain.getInstituicaoEnsino().getId() );
		dto.setUrlOtimizacao( domain.getUrlOtimizacao() );
		dto.setNomeOtimizacao( domain.getNomeOtimizacao() );
		
		return dto;
	}

	public static MotivoUsoProfessorVirtualDTO toMotivoUsoProfessorVirtualDTO( MotivoUsoProfessorVirtual domain )
	{
		MotivoUsoProfessorVirtualDTO dto = new MotivoUsoProfessorVirtualDTO();
		
		dto.setId(domain.getId());
		dto.setMotivo(domain.getMotivoUso().replace("magistério", "habilitação").replace("magisterio", "habilitação"));
		dto.setProfessorCpf(domain.getProfessor() == null ? null : domain.getProfessor().getCpf());
		dto.setProfessorString(domain.getProfessor() == null ? null : domain.getProfessor().getNome());
		
		return dto;
	}
	
	public static DicaEliminacaoProfessorVirtualDTO toDicaEliminacaoProfessorVirtualDTO( DicaEliminacaoProfessorVirtual domain )
	{
		DicaEliminacaoProfessorVirtualDTO dto = new DicaEliminacaoProfessorVirtualDTO();
		
		dto.setId(domain.getId());
		dto.setDica(domain.getDicaEliminacao());
		dto.setProfessorCpf(domain.getProfessor() == null ? null : domain.getProfessor().getCpf());
		dto.setProfessorString(domain.getProfessor() == null ? null : domain.getProfessor().getNome());
		
		return dto;
	}
	
	public static TurmaDTO toTurmaDTO( Turma domain )
	{
		TurmaDTO dto = new TurmaDTO();
		
		dto.setInstituicaoEnsinoId(domain.getCenario().getInstituicaoEnsino().getId());
		dto.setNome(domain.getNome());
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCenarioId(domain.getCenario().getId());
		dto.setDisciplinaId(domain.getDisciplina().getId());
		dto.setDisciplinaString(domain.getDisciplina().getCodigo());
		dto.setParcial(domain.getParcial());
		dto.setNoAlunos(domain.getAlunos().size());
		
		Integer credAlocados = 0;
		if (domain.getAulas().size() > 0)
		{
			for (Aula aula : domain.getAulas())
			{
				credAlocados += aula.getCreditosPraticos() + aula.getCreditosTeoricos();
			}
		}
		dto.setCredAlocados(credAlocados);
		
		return dto;
	}
	
	public static Turma toTurma( TurmaDTO dto )
	{
		Turma domain = new Turma();
		
		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( dto.getInstituicaoEnsinoId() );
		Cenario cenario = Cenario.find(
				dto.getCenarioId(), instituicaoEnsino );
		Disciplina disciplina = Disciplina.find(dto.getDisciplinaId(), instituicaoEnsino);
		
		domain.setNome(dto.getNome());
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setCenario(cenario);
		domain.setDisciplina(disciplina);
		domain.setParcial(dto.getParcial());
		
		return domain;
	}
	
	public static AulaDTO toAulaDTO(Aula domain)
	{
		AulaDTO dto = new AulaDTO();
		
		dto.setInstituicaoEnsinoId(domain.getCenario().getInstituicaoEnsino().getId());
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCenarioId(domain.getCenario().getId());
		dto.setCreditosPraticos(domain.getCreditosPraticos());
		dto.setCreditosTeoricos(domain.getCreditosTeoricos());
		dto.setProfessorId(domain.getProfessor() == null ? null : domain.getProfessor().getId() );
		dto.setProfessorVirtualId(domain.getProfessor() == null ? null : domain.getProfessor().getId() );
		String professorNome = "";
		if (domain.getProfessor() != null)
		{
			professorNome = domain.getProfessor().getNome();
		}
		else if (domain.getProfessorVirtual() != null)
		{
			professorNome = domain.getProfessorVirtual().getNome();
		}
		dto.setProfessorNome( professorNome );
		dto.setSalaId(domain.getSala().getId());
		dto.setSalaString(domain.getSala().getCodigo());
		dto.setHorarioDisponivelCenarioId(domain.getHorarioDisponivelCenario().getId());
		dto.setHorarioAulaId(domain.getHorarioDisponivelCenario().getHorarioAula().getId());
		dto.setSemana(Semanas.toInt(domain.getHorarioDisponivelCenario().getDiaSemana()));
		dto.setSemanaString(domain.getHorarioDisponivelCenario().getDiaSemana().name());
		
		DateFormat df = new SimpleDateFormat( "HH:mm" );
		String inicio = df.format( domain.getHorarioDisponivelCenario().getHorarioAula().getHorario() );

		Calendar fimCal = Calendar.getInstance();
		fimCal.setTime( domain.getHorarioDisponivelCenario().getHorarioAula().getHorario() );

		fimCal.add( Calendar.MINUTE, domain.getHorarioDisponivelCenario().getHorarioAula().getSemanaLetiva().getTempo() * (dto.getCreditosPraticos() + dto.getCreditosTeoricos()));
		String fim = df.format( fimCal.getTime() );

		String tipoCredito = domain.getCreditosPraticos() == 0 ? domain.getCreditosTeoricos() + "T" : domain.getCreditosPraticos() + "P";
		
		String horarioString = domain.getHorarioDisponivelCenario().getDiaSemana().name() + " " + inicio + "/" + fim + " " + tipoCredito;
		
		dto.setHorarioString(horarioString);
		
		return dto;
	}
	
	public static AulaDTO toAulaDTO(AtendimentoOperacionalDTO aula)
	{
		InstituicaoEnsino instituicaoEnsino = InstituicaoEnsino.find(aula.getInstituicaoEnsinoId());
		
		AulaDTO aulaDTO = new AulaDTO();
		aulaDTO.setAtendimentosIds(aula.getIdsAtendimentosConcatenados());
		aulaDTO.setCenarioId(aula.getCenarioId());
		aulaDTO.setInstituicaoEnsinoId(aula.getInstituicaoEnsinoId());
		aulaDTO.setCreditosPraticos(aula.getCreditoTeoricoBoolean() ? 0 : aula.getTotalCreditos());
		aulaDTO.setCreditosTeoricos(aula.getCreditoTeoricoBoolean() ? aula.getTotalCreditos() : 0);
		aulaDTO.setSalaId(aula.getSalaId());
		aulaDTO.setSalaString(aula.getSalaString());
		aulaDTO.setHorarioDisponivelCenarioId(aula.getHorarioDisponivelCenarioId());
		aulaDTO.setHorarioAulaId(aula.getHorarioAulaId());
		aulaDTO.setSemana(aula.getSemana());
		aulaDTO.setSemanaString(Semanas.get(aula.getSemana()).name());
		
		HorarioDisponivelCenario hdc = HorarioDisponivelCenario.find(aula.getHorarioDisponivelCenarioId(), instituicaoEnsino);
		DateFormat df = new SimpleDateFormat( "HH:mm" );
		String inicio = df.format( hdc.getHorarioAula().getHorario() );

		Calendar fimCal = Calendar.getInstance();
		fimCal.setTime( hdc.getHorarioAula().getHorario() );

		fimCal.add( Calendar.MINUTE, hdc.getHorarioAula().getSemanaLetiva().getTempo() * (aulaDTO.getCreditosPraticos() + aulaDTO.getCreditosTeoricos()));
		String fim = df.format( fimCal.getTime() );

		String tipoCredito = aula.getCreditoTeoricoBoolean() ? aula.getTotalCreditos() + "T" : aula.getTotalCreditos() + "P";
		
		String horarioString = Semanas.get(aula.getSemana()) + " " + inicio + "/" + fim + " " + tipoCredito;
		
		aulaDTO.setHorarioString(horarioString);

		aulaDTO.setProfessorId(aula.getProfessorId());
		String professorNome = "";
		if (aula.getProfessorId() != null)
		{
			professorNome = aula.getProfessorString();
		}
		else if (aula.getProfessorVirtualId() != null)
		{
			professorNome = aula.getProfessorVirtualString();
		}
		
		aulaDTO.setProfessorNome(professorNome);
		aulaDTO.setProfessorVirtualId(aula.getProfessorVirtualId());
		
		return aulaDTO;
	}
	
	public static AulaDTO toAulaDTO(AtendimentoTatico aula)
	{
		AulaDTO aulaDTO = new AulaDTO();
		Set<Long> atendimentosIds = new HashSet<Long>();
		atendimentosIds.add(aula.getId());
		
		aulaDTO.setAtendimentosIds(atendimentosIds);
		aulaDTO.setCenarioId(aula.getCenario().getId());
		aulaDTO.setInstituicaoEnsinoId(aula.getInstituicaoEnsino().getId());
		aulaDTO.setCreditosPraticos(aula.getCreditosPratico());
		aulaDTO.setCreditosTeoricos(aula.getCreditosTeorico());
		aulaDTO.setSalaId(aula.getSala().getId());
		aulaDTO.setSalaString(aula.getSala().getCodigo());
		aulaDTO.setHorarioDisponivelCenarioId(HorarioDisponivelCenario.findBy(aula.getInstituicaoEnsino(), aula.getHorarioAula(), aula.getSemana()).getId());
		aulaDTO.setHorarioAulaId(aula.getHorarioAula().getId());		
		aulaDTO.setSemana(Semanas.toInt(aula.getSemana()));//aulaDTO.setSemana(aula.getSemana().ordinal());
		aulaDTO.setSemanaString(aula.getSemana().name());
		aulaDTO.setProfessorNome("");
		
		DateFormat df = new SimpleDateFormat( "HH:mm" );
		String inicio = df.format( aula.getHorarioAula().getHorario() );

		Calendar fimCal = Calendar.getInstance();
		fimCal.setTime( aula.getHorarioAula().getHorario() );

		fimCal.add( Calendar.MINUTE, aula.getHorarioAula().getSemanaLetiva().getTempo() );
		String fim = df.format( fimCal.getTime() );

		String tipoCredito = aula.getCreditosPratico() == 0 ? aula.getCreditosTeorico() + "T" : aula.getCreditosPratico() + "P";
		
		String horarioString = aula.getSemana().name() + " " + inicio + "/" + fim + " " + tipoCredito;
		
		aulaDTO.setHorarioString(horarioString);
		
		
		return aulaDTO;
	}
	
	public static Aula toAula(AulaDTO dto)
	{
		Aula domain = new Aula();
		
		InstituicaoEnsino instituicaoEnsino = InstituicaoEnsino.find(dto.getInstituicaoEnsinoId());
		Cenario cenario = Cenario.find(dto.getCenarioId(), instituicaoEnsino);
		HorarioAula horarioAula = HorarioAula.find(dto.getHorarioAulaId(), instituicaoEnsino);
		HorarioDisponivelCenario hdc = HorarioDisponivelCenario.findBy(instituicaoEnsino, horarioAula, Semanas.get(dto.getSemanaString()));
		Sala sala = Sala.find(dto.getSalaId(), instituicaoEnsino);
		Professor professor = Professor.find(dto.getProfessorId(), instituicaoEnsino);
		ProfessorVirtual professorVirtual = ProfessorVirtual.find(dto.getProfessorVirtualId(), instituicaoEnsino);
		
		
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setCenario(cenario);
		domain.setHorarioDisponivelCenario(hdc);
		domain.setSala(sala);
		domain.setProfessor(professor);
		domain.setProfessorVirtual(professorVirtual);
		domain.setCreditosPraticos(dto.getCreditosPraticos());
		domain.setCreditosTeoricos(dto.getCreditosTeoricos());
		
		
		return domain;
	}
	
	public static List < AulaDTO > toListAulasDTO(
			List< Aula > listDomains )
		{
			if ( listDomains == null )
			{
				return Collections.< AulaDTO > emptyList();
			}

			List< AulaDTO > listDTOs
				= new ArrayList< AulaDTO >();

			for ( Aula dto : listDomains )
			{
				listDTOs.add( ConvertBeans.toAulaDTO( dto ) );
			}

			return listDTOs;
		}

	public static List<DisponibilidadeDTO> toDisponibilidadeDTO(
			List<Disponibilidade> listDomains) {
		if ( listDomains == null )
		{
			return Collections.< DisponibilidadeDTO > emptyList();
		}

		List< DisponibilidadeDTO > listDTOs
			= new ArrayList< DisponibilidadeDTO >();

		for ( Disponibilidade dto : listDomains )
		{
			listDTOs.add( ConvertBeans.toDisponibilidadeDTO( dto ) );
		}

		return listDTOs;
	}

	private static DisponibilidadeDTO toDisponibilidadeDTO(Disponibilidade domain)
	{
		DisponibilidadeDTO dto = new DisponibilidadeDTO();
		
		dto.setDisponibilidadeId(domain.getId());
		dto.setEntidadeId(domain.getEntidadeId());
		dto.setEntidadeTipo(domain.getEntidadeTipo());
		DateFormat df = new SimpleDateFormat( "HH:mm" );
		String inicio = df.format( domain.getHorarioInicio() );
		String fim = df.format( domain.getHorarioFim() );
		dto.setHorarioInicioString(inicio);
		dto.setHorarioFimString(fim);
		dto.setSegunda(domain.getSegunda());
		dto.setTerca(domain.getTerca());
		dto.setQuarta(domain.getQuarta());
		dto.setQuinta(domain.getQuinta());
		dto.setSexta(domain.getSexta());
		dto.setSabado(domain.getSabado());
		dto.setDomingo(domain.getDomingo());
		
		return dto;
	}

	public static Disponibilidade toDisponibilidade(DisponibilidadeDTO dto, String tipoEntidade)
	{
		InstituicaoEnsino instituicaoEnsino = InstituicaoEnsino.find(dto.getInstituicaoEnsinoId());
		Disponibilidade domain = null;
		if (tipoEntidade.equals(DisponibilidadeDTO.DISCIPLINA))
		{
			domain = new DisponibilidadeDisciplina();
			((DisponibilidadeDisciplina)domain).setDisciplina(Disciplina.find(dto.getEntidadeId(), instituicaoEnsino));
		}
		else if (tipoEntidade.equals(DisponibilidadeDTO.PROFESSOR))
		{
			domain = new DisponibilidadeProfessor();
			((DisponibilidadeProfessor)domain).setProfessor(Professor.find(dto.getEntidadeId(), instituicaoEnsino));
		}
		else if (tipoEntidade.equals(DisponibilidadeDTO.CAMPUS))
		{
			domain = new DisponibilidadeCampus();
			((DisponibilidadeCampus)domain).setCampus(Campus.find(dto.getEntidadeId(), instituicaoEnsino));
		}
		else if (tipoEntidade.equals(DisponibilidadeDTO.UNIDADE))
		{
			domain = new DisponibilidadeUnidade();
			((DisponibilidadeUnidade)domain).setUnidade(Unidade.find(dto.getEntidadeId(), instituicaoEnsino));
		}
		else if (tipoEntidade.equals(DisponibilidadeDTO.SALA))
		{
			domain = new DisponibilidadeSala();
			((DisponibilidadeSala)domain).setSala(Sala.find(dto.getEntidadeId(), instituicaoEnsino));
		}
		try {
			domain.setHorarioInicio(new SimpleDateFormat("HH:mm").parse(dto.getHorarioInicioString()));
			domain.setHorarioFim(new SimpleDateFormat("HH:mm").parse(dto.getHorarioFimString()));
		} catch (ParseException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		domain.setSegunda(true);
		domain.setTerca(true);
		domain.setQuarta(true);
		domain.setQuinta(true);
		domain.setSexta(true);
		domain.setSabado(false);
		domain.setDomingo(false);
		
		return domain;
	}
}
