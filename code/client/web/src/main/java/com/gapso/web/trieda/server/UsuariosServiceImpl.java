package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Authority;
import com.gapso.trieda.domain.InstituicaoEnsino;
/*import com.gapso.trieda.domain.PerfilAcesso;
import com.gapso.trieda.domain.PerfilAcessoTransacao;*/
import com.gapso.trieda.domain.Professor;
/*import com.gapso.trieda.domain.Transacao;*/
import com.gapso.trieda.domain.Usuario;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.Encryption;
import com.gapso.web.trieda.shared.dtos.AlterarSenhaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.services.UsuariosService;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.gapso.web.trieda.shared.util.view.TriedaException;

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
			= this.getInstituicaoEnsinoSuperUser();

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

			usuario.getAuthority().setAuthority( usuarioDTO.getAdministrador() ? "ROLE_SUPERVISOR" : "ROLE_USER" );
			usuario.merge();
		}
		else
		{
			usuario = ConvertBeans.toUsuario( usuarioDTO );
			usuario.setPassword( Encryption.toMD5( usuario.getPassword() ) );
			usuario.setEnabled( true );
/*			usuario.getPerfisAcesso().add(createDefaultProfile());*/
			usuario.persist();

			Authority authority = new Authority();
			authority.setAuthority( usuarioDTO.getAdministrador() ? "ROLE_SUPERVISOR" : "ROLE_USER" );
			authority.setUsername( usuario.getUsername() );
			authority.setInstituicaoEnsino(usuario.getInstituicaoEnsino());
			authority.persist();
		}
	}
	
/*	@Transactional
	private PerfilAcesso createDefaultProfile() {
		PerfilAcesso perfilPadrao = new PerfilAcesso();
		perfilPadrao.setNome("user");
		perfilPadrao.persist();
		
		List<Transacao> transacoes = Transacao.findAll();
		for (Transacao transacao : transacoes)
		{
			PerfilAcessoTransacao perfilAcessoTransacao = new PerfilAcessoTransacao();
			perfilAcessoTransacao.setVisivel(true);
			perfilAcessoTransacao.setPerfilAcesso(perfilPadrao);
			perfilAcessoTransacao.setTransacao(transacao);
			
			perfilAcessoTransacao.persist();
		}
		
		return perfilPadrao;
	}*/

	@Override
	public void changePassword( UsuarioDTO usuarioDTO, AlterarSenhaDTO alterarSenhaDTO ) throws TriedaException
	{
		Usuario usuario = Usuario.find( usuarioDTO.getUsername() );
		
		if (!alterarSenhaDTO.getSenhaAntiga().equals(usuario.getPassword()))
		{
			throw new TriedaException("Senha antiga foi digitada incorretamente.");
		}
		else
		{
			usuario.setPassword(alterarSenhaDTO.getSenhaNova());
			usuario.merge();
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
