package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

public class TiposCursoImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< TiposCursoImportExcelBean >
{
	private String codigoStr;
	private String descricaoStr = "";

	public TiposCursoImportExcelBean( int row )
	{
		super( row );
	}

	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros
			= new ArrayList< ImportExcelError >();

		if ( !tudoVazio() )
		{
			checkMandatoryField( this.codigoStr, ImportExcelError.TIPO_CURSO_CODIGO_VAZIO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}

	private boolean tudoVazio()
	{
		return (isEmptyField( this.codigoStr )
			&& isEmptyField( this.descricaoStr ) );
	}

	public String getCodigoStr()
	{
		return this.codigoStr;
	}

	public void setCodigoStr( String codigoStr )
	{
		this.codigoStr = codigoStr;
	}

	public String getDescricaoStr()
	{
		return this.descricaoStr;
	}

	public void setDescricaoStr( String descricaoStr )
	{
		this.descricaoStr = descricaoStr;
	}

	@Override
	public int compareTo( TiposCursoImportExcelBean o )
	{
		return this.getCodigoStr().compareTo(o.getCodigoStr());
	}
}

