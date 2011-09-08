package com.gapso.web.trieda.server;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.GrupoSala;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.domain.Usuario;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.services.UniquesService;
import com.gapso.web.trieda.shared.util.view.UniqueDomain;

public class UniquesServiceImpl
	extends RemoteService
	implements UniquesService
{
	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public Boolean existUnique( CenarioDTO cenarioDTO,
		String value, String uniqueDomain )
	{
		Cenario cenario = Cenario.find(
			cenarioDTO.getId(), this.getInstituicaoEnsinoUser() );

		Boolean exist = true;
		UniqueDomain uniqueDomainEnum
			= UniqueDomain.valueOf( uniqueDomain );

		switch ( uniqueDomainEnum )
		{
			case CAMPI: exist = checkCampus( cenario, value ); break;
			case UNIDADE: exist = checkUnidade( cenario, value ); break;
			case SALA: exist = checkSala( cenario, value ); break;
			case GRUPO_SALA: exist = checkGrupoSala( cenario, value ); break;
			case CURSO: exist = checkCurso( cenario, value ); break;
			case SEMANA_LETIVA: exist = checkSemanaLetiva( cenario, value ); break;
			case PROFESSOR: exist = checkProfessor( cenario, value ); break;
			case DISCIPLINA: exist = checkDisciplina( cenario, value ); break;
			case MATRIZ_CURRICULAR: exist = checkMatrizCurricular( cenario, value ); break;
			case USUARIO: exist = checkUsuario( cenario, value ); break;
		}

		return exist;
	}

	private Boolean checkCampus(Cenario cenario, String value) {
		return Campus.checkCodigoUnique(
			this.getInstituicaoEnsinoUser(), cenario, value);
	}

	private Boolean checkUnidade( Cenario cenario, String value )
	{
		return Unidade.checkCodigoUnique(
			getInstituicaoEnsinoUser(), cenario, value );
	}

	private Boolean checkSala( Cenario cenario, String value )
	{
		return Sala.checkCodigoUnique(
			getInstituicaoEnsinoUser() ,cenario, value );
	}

	private Boolean checkGrupoSala(Cenario cenario, String value) {
		return GrupoSala.checkCodigoUnique( getInstituicaoEnsinoUser(), cenario, value );
	}

	private Boolean checkCurso( Cenario cenario, String value )
	{
		return Curso.checkCodigoUnique(
			getInstituicaoEnsinoUser(), cenario, value );
	}

	private Boolean checkSemanaLetiva( Cenario cenario, String value )
	{
		return SemanaLetiva.checkCodigoUnique(
			getInstituicaoEnsinoUser(), cenario, value );
	}

	private Boolean checkProfessor(Cenario cenario, String value) {
		return Professor.checkCodigoUnique( getInstituicaoEnsinoUser(), cenario, value);
	}

	private Boolean checkDisciplina( Cenario cenario, String value )
	{
		return Disciplina.checkCodigoUnique(
			getInstituicaoEnsinoUser(), cenario, value );
	}

	private Boolean checkMatrizCurricular( Cenario cenario, String value )
	{
		return Curriculo.checkCodigoUnique(
			getInstituicaoEnsinoUser(), cenario, value );
	}

	private Boolean checkUsuario( Cenario cenario,
		String value )
	{
		return Usuario.checkUsernameUnique(
			cenario, value, getInstituicaoEnsinoUser() );
	}
}
