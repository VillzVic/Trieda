package com.gapso.web.trieda.shared.dtos;

import java.util.HashMap;
import java.util.Map;

public class UsuarioDTO extends AbstractDTO< String >
	implements Comparable< UsuarioDTO >
{
	private static final long serialVersionUID = 5815525344760896272L;

	// Propriedades
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_NOME = "nome";
	public static final String PROPERTY_EMAIL = "email";
	public static final String PROPERTY_USERNAME = "username";
	public static final String PROPERTY_PASSWORD = "maxpassword";
	public static final String PROPERTY_ENABLED = "enabled";
	public static final String PROPERTY_PROFESSOR_ID = "professorId";
	public static final String PROPERTY_PROFESSOR_DISPLAYTEXT = "professorDisplayText";
	public static final String PROPERTY_PROFESSOR_CPF = "professorCpf";
	public static final String PROPERTY_ADMINISTRADOR = "administrador";

	public UsuarioDTO() { }

	public void setVersion( Integer value )
	{
		set( PROPERTY_VERSION, value );
	}

	public Integer getVersion()
	{
		return get( PROPERTY_VERSION );
	}

	public void setNome( String value )
	{
		set( PROPERTY_NOME, value );
	}

	public String getNome()
	{
		return get( PROPERTY_NOME );
	}

	public void setEmail( String value )
	{
		set( PROPERTY_EMAIL, value );
	}

	public String getEmail()
	{
		return get( PROPERTY_EMAIL );
	}
	
	public void setUsername( String value )
	{
		set( PROPERTY_USERNAME, value );
	}

	public String getUsername()
	{
		return get( PROPERTY_USERNAME );
	}

	public void setPassword( String value )
	{
		set( PROPERTY_PASSWORD, value );
	}

	public String getPassword()
	{
		return get( PROPERTY_PASSWORD );
	}

	public void setEnabled( Boolean value )
	{
		set( PROPERTY_ENABLED, value );
	}

	public Boolean getEnabled()
	{
		return get( PROPERTY_ENABLED );
	}
	
	public void setProfessorId( Long value )
	{
		set( PROPERTY_PROFESSOR_ID, value );
	}

	public Long getProfessorId()
	{
		return get( PROPERTY_PROFESSOR_ID );
	}

	public void setProfessorDisplayText( String value )
	{
		set( PROPERTY_PROFESSOR_DISPLAYTEXT, value );
	}

	public String getProfessorDisplayText()
	{
		return get( PROPERTY_PROFESSOR_DISPLAYTEXT );
	}
	
	public void setProfessorCpf( String value )
	{
		set( PROPERTY_PROFESSOR_CPF, value );
	}

	public String getProfessorCpf()
	{
		return get( PROPERTY_PROFESSOR_CPF );
	}

	public boolean isProfessor()
	{
		return ( getProfessorId() != null && getProfessorId() > 0 );
	}
	
	public void setAdministrador( Boolean value )
	{
		set( PROPERTY_ADMINISTRADOR, value );
	}

	public Boolean getAdministrador()
	{
		return get( PROPERTY_ADMINISTRADOR );
	}
	
	public boolean isAdministrador()
	{
		return !isProfessor();
	}

	@Override
	public String getNaturalKey()
	{
		return getUsername();
	}
	
	private Map<String, Boolean> transacaoMapVisivel = new HashMap<String, Boolean>();
	private Map<String, Boolean> transacaoMapEditavel = new HashMap<String, Boolean>();
	
	public Map<String, Boolean> getTransacaoMapVisivel()
	{
		return transacaoMapVisivel;
	}
	
	public Map<String, Boolean> getTransacaoMapEditavel()
	{
		return transacaoMapEditavel;
	}
	

	@Override
	public int compareTo( UsuarioDTO o )
	{
		return getNome().compareTo( o.getNome() );
	}
}
