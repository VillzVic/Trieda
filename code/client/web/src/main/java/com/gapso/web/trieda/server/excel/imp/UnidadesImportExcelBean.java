package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.Campus;

public class UnidadesImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< UnidadesImportExcelBean >
{
	private String codigoStr;
	private String nomeStr;
	private String codigoCampusStr;
	private Campus campus;

	public UnidadesImportExcelBean( int row )
	{
		super( row );
	}

	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros
			= new ArrayList< ImportExcelError >();

		if ( !tudoVazio() )
		{
			checkMandatoryField( this.codigoStr, ImportExcelError.UNIDADE_CODIGO_VAZIO, erros );
			checkMandatoryField( this.nomeStr, ImportExcelError.UNIDADE_NOME_VAZIO, erros );
			checkMandatoryField( this.codigoCampusStr, ImportExcelError.UNIDADE_CAMPUS_VAZIO, erros );
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
			&& isEmptyField( this.nomeStr )
			&& isEmptyField( this.codigoCampusStr ) );
	}

	public String getCodigoStr()
	{
		return this.codigoStr;
	}

	public void setCodigoStr( String codigoStr )
	{
		this.codigoStr = codigoStr;
	}

	public String getNomeStr()
	{
		return this.nomeStr;
	}

	public void setNomeStr( String nomeStr )
	{
		this.nomeStr = nomeStr;
	}

	public String getCodigoCampusStr()
	{
		return this.codigoCampusStr;
	}

	public void setCodigoCampusStr( String codigoCampusStr )
	{
		this.codigoCampusStr = codigoCampusStr;
	}

	public Campus getCampus()
	{
		return this.campus;
	}

	public void setCampus( Campus campus )
	{
		this.campus = campus;
	}

	@Override
	public int compareTo( UnidadesImportExcelBean o )
	{
		return this.getCodigoStr().compareTo(o.getCodigoStr());
	}
}
