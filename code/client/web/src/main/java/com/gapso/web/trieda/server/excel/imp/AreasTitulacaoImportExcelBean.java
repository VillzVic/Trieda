package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

public class AreasTitulacaoImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< AreasTitulacaoImportExcelBean >
{
	private String codigoStr;
	private String descricaoStr;

	public AreasTitulacaoImportExcelBean( int row )
	{
		super( row );
	}
	
	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros
			= new ArrayList< ImportExcelError >();

		if ( !tudoVazio() )
		{
			checkMandatoryField( this.codigoStr,
				ImportExcelError.AREA_TITULACAO_CODIGO_VAZIO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}
	
	private boolean tudoVazio()
	{
		return isEmptyField( this.codigoStr );
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
	public int compareTo( AreasTitulacaoImportExcelBean o )
	{
		return this.getCodigoStr().compareTo( o.getCodigoStr() );
	}
}
