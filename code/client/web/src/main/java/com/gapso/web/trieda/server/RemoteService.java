package com.gapso.web.trieda.server;

import javax.naming.InitialContext;
import javax.naming.NameClassPair;
import javax.naming.NamingEnumeration;
import javax.naming.NamingException;

import org.eclipse.jetty.jndi.NamingContext;
import org.springframework.security.core.context.SecurityContext;
import org.springframework.security.core.context.SecurityContextHolder;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Usuario;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

public class RemoteService
	extends RemoteServiceServlet
{
	private static final long serialVersionUID = 5939530994258762775L;

	final public InstituicaoEnsino getInstituicaoEnsinoUser()
	{
		Usuario usuario = this.getUsuario();
		if (usuario == null)
			return null;
		//Caso a instituicaoEnsino for null temos um superusuario.
		//A instituicaoEnsino do superusuario é transiente e varia de acordo
		//com o cenário que esta selecionado no momento
		else if (usuario.getInstituicaoEnsino() == null)
			return getCenario().getInstituicaoEnsino();
		else
			return usuario.getInstituicaoEnsino();
	}
	
	final public InstituicaoEnsino getInstituicaoEnsinoSuperUser()
	{
		Usuario usuario = this.getUsuario();
		if (usuario == null)
			return null;
		else
			return usuario.getInstituicaoEnsino();
	}

	final protected Usuario getUsuario()
	{
		SecurityContext context = SecurityContextHolder.getContext();
		String username = context.getAuthentication().getName();
		return Usuario.find( username );
	}

	final protected Cenario getCenario()
	{
		if (getThreadLocalRequest().getSession(false).getAttribute("cenario") != null)
		{
			return Cenario.find((Long)getThreadLocalRequest().getSession().getAttribute("cenario"), getInstituicaoEnsinoSuperUser());
		}
		else
		{
			return Cenario.findMasterData(getInstituicaoEnsinoSuperUser());
		}
	}
	
	@SuppressWarnings("unchecked")
	final protected String getDataSourceName() throws NamingException
	{
		String result = "";
		InitialContext ctx = new InitialContext();
		NamingEnumeration<NameClassPair> list = ((NamingContext) ctx.lookup("jdbc")).list("");
		if (list.hasMore()) {
			result += list.next().getName();
		}

		return result;
	}
	
	final protected void setCenario(long cenarioId)
	{
		 getThreadLocalRequest().getSession(false).setAttribute("cenario", cenarioId);
	}
	
	protected boolean isProfessor()
	{
		return ( getUsuario().getProfessor() != null );
	}

	protected void onlyProfessor()
	{
		if ( !isProfessor() )
		{
			throw new RuntimeException(
				"Permissão negada, usuário \""
				+ getUsuario().getUsername()
				+ "\" não é de Professor" );
		}
	}

	protected boolean isAdministrador()
	{
		return ( getUsuario().getProfessor() == null );
	}
	
	protected boolean isRoleAdministrator()
	{
		return getUsuario().getAuthority().getAuthority().equals("ROLE_SUPERVISOR");
	}

	protected void onlyAdministrador()
	{
		if ( !isAdministrador() )
		{
			throw new RuntimeException(
				"Permissão negada, usuário \""
				+ getUsuario().getUsername()
				+ "\" não é de Administrador" );
		}
	}
}
