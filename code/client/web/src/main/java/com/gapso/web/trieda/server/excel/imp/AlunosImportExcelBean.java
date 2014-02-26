package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

public class AlunosImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< AlunosImportExcelBean >
{
	private String matriculaStr;
	private String nomeStr;
	private String formandoStr;
	private String virtualStr;

	private Boolean formando;
	private Boolean virtual;

	public AlunosImportExcelBean( int row )
	{
		super( row );
	}

	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros = new ArrayList< ImportExcelError >();

		if ( !tudoVazio() )
		{
			checkMandatoryField( matriculaStr, ImportExcelError.ALUNO_MATRICULA_VAZIO, erros );
			checkMandatoryField( nomeStr, ImportExcelError.ALUNO_NOME_VAZIO, erros );
			checkMandatoryField( formandoStr, ImportExcelError.ALUNO_FORMANDO_VAZIO, erros );
			checkMandatoryField( virtualStr, ImportExcelError.ALUNO_VIRTUAL_VAZIO, erros );

			formando = checkBooleanField( formandoStr, ImportExcelError.ALUNO_FORMANDO_FORMATO_INVALIDO, erros );
			virtual = checkBooleanField( virtualStr, ImportExcelError.ALUNO_VIRTUAL_FORMATO_INVALIDO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}
	
	private boolean tudoVazio()
	{
		return ( isEmptyField( matriculaStr )
			&& isEmptyField( nomeStr )
			&& isEmptyField( formandoStr )
			&& isEmptyField( virtualStr ) );
	}

	public String getMatriculaStr()
	{
		return matriculaStr;
	}

	public void setMatriculaStr( String matriculaStr )
	{
		this.matriculaStr = matriculaStr;
	}

	public String getNomeStr()
	{
		return nomeStr;
	}

	public void setNomeStr( String nomeStr )
	{
		this.nomeStr = nomeStr;
	}

	public String getFormandoStr()
	{
		return formandoStr;
	}

	public void setFormandoStr( String formandoStr )
	{
		this.formandoStr = formandoStr;
	}

	public String getVirtualStr()
	{
		return virtualStr;
	}

	public void setVirtualStr( String virtualStr )
	{
		this.virtualStr = virtualStr;
	}
	
	public Boolean getFormando()
	{
		return formando;
	}
	
	public Boolean getVirtual()
	{
		return virtual;
	}

	@Override
	public int compareTo( AlunosImportExcelBean o )
	{
		return getMatriculaStr().compareTo( o.getMatriculaStr() );
	}
}