package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

public class TurnosImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< TurnosImportExcelBean >
{
	private String nomeStr;

	public TurnosImportExcelBean( int row )
	{
		super( row );
	}

	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros
			= new ArrayList< ImportExcelError >();

		if ( !tudoVazio() )
		{
			checkMandatoryField( this.nomeStr, ImportExcelError.TURNO_NOME_VAZIO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}

	private boolean tudoVazio()
	{
		return (isEmptyField( this.nomeStr ) );
	}

	public String getNomeStr()
	{
		return this.nomeStr;
	}

	public void setNomeStr( String nomeStr )
	{
		this.nomeStr = nomeStr;
	}

	@Override
	public int compareTo( TurnosImportExcelBean o )
	{
		return this.getNomeStr().compareTo(o.getNomeStr());
	}
}
