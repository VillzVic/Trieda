package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Authority;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.Usuario;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.Encryption;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.services.UsuariosService;
import com.gapso.web.trieda.shared.util.TriedaUtil;

public class UsuariosServiceImpl
	extends RemoteService
	implements UsuariosService
{
	private static final long serialVersionUID = 5672570072070386404L;

	@Override
	public UsuarioDTO getUsuario( String username )
	{
		return ConvertBeans.toUsuarioDTO(
			Usuario.find( username ) );
	}

	@Override
	public UsuarioDTO getCurrentUser()
	{
		Usuario usuario = getUsuario();

		return ( ( usuario == null ) ? null
			: ConvertBeans.toUsuarioDTO( usuario ) );
	}

	@Override
	public PagingLoadResult< UsuarioDTO > getBuscaList(
		String nome, String username,
		String email, PagingLoadConfig config )
	{
		List< UsuarioDTO > list = new ArrayList< UsuarioDTO >();
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

		InstituicaoEnsino instituicaoEnsino
			= this.getUsuario().getInstituicaoEnsino();

		List< Usuario > usuarios = Usuario.findAllBy(
			nome, username, email, config.getOffset(),
			config.getLimit(), orderBy, instituicaoEnsino );

		for ( Usuario usuario : usuarios )
		{
			list.add( ConvertBeans.toUsuarioDTO(usuario ) );
		}

		BasePagingLoadResult< UsuarioDTO > result
			= new BasePagingLoadResult< UsuarioDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Usuario.count(
			nome, username, email, instituicaoEnsino ) );

		return result;
	}

	@Override
	public void save( UsuarioDTO usuarioDTO )
	{
		Usuario usuario = Usuario.find( usuarioDTO.getUsername() );

		if ( usuario != null )
		{
			usuario.setNome( usuarioDTO.getNome() );
			usuario.setEmail( usuarioDTO.getEmail() );

			if ( !TriedaUtil.isBlank( usuarioDTO.getPassword() ) )
			{
				usuario.setPassword( Encryption.toMD5( usuarioDTO.getPassword() ) );
			}

			if ( !TriedaUtil.isBlank( usuarioDTO.getProfessorId() ) )
			{
				usuario.setProfessor( Professor.find(
					usuarioDTO.getProfessorId(), getInstituicaoEnsinoUser() ) );
			}

			usuario.merge();
		}
		else
		{
			usuario = ConvertBeans.toUsuario( usuarioDTO );
			usuario.setPassword( Encryption.toMD5( usuario.getPassword() ) );
			usuario.setEnabled( true );
			usuario.persist();

			Authority authority = new Authority();
			authority.setAuthority( "ROLE_USER" );
			authority.setUsername( usuario.getUsername() );
			authority.persist();
		}
	}

	@Override
	public void remove(
		List< UsuarioDTO > usuarioDTOList )
	{
		for ( UsuarioDTO usuarioDTO : usuarioDTOList )
		{
			Usuario usuario = Usuario.find(
				usuarioDTO.getUsername() );

			if ( usuario != null )
			{
				usuario.remove();
			}
		}
	}

	@Override
	public Boolean avoidSessionExpire()
	{
		return true;
	}

	@Override
	public InstituicaoEnsinoDTO getInstituicaoEnsinoUserDTO()
	{
		InstituicaoEnsino instituicaoEnsino
			= this.getInstituicaoEnsinoUser();

		return ( instituicaoEnsino == null ? null :
			ConvertBeans.toInstituicaoEnsinoDTO( instituicaoEnsino ) );
	}
}
