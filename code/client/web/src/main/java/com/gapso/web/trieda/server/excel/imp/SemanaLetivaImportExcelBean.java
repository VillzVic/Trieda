package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

public class SemanaLetivaImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< SemanaLetivaImportExcelBean >
{
	private String codigoStr;
	private String descricaoStr;
	private String duracaoStr;
	private String permiteIntervaloStr;
	
	private Integer duracao;
	private Boolean permiteIntervalo;

	public SemanaLetivaImportExcelBean( int row )
	{
		super( row );
	}

	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros
			= new ArrayList< ImportExcelError >();

		if ( !tudoVazio() )
		{
			checkMandatoryField( this.codigoStr, ImportExcelError.SEMANA_LETIVA_CODIGO_VAZIO, erros );
			checkMandatoryField( this.duracaoStr, ImportExcelError.SEMANA_LETIVA_DURACAO_VAZIO, erros );
			checkMandatoryField( this.permiteIntervaloStr, ImportExcelError.SEMANA_LETIVA_PERMITE_INTERVALO_VAZIO, erros );
			
			this.duracao = checkNonNegativeIntegerField( this.duracaoStr,
					ImportExcelError.SEMANA_LETIVA_DURACAO_FORMATO_INVALIDO,
					ImportExcelError.SEMANA_LETIVA_DURACAO_VALOR_NEGATIVO, erros );
			
			permiteIntervalo = checkBooleanField( permiteIntervaloStr, ImportExcelError.SEMANA_LETIVA_FORMATO_INVALIDO, erros );

		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}

	private boolean tudoVazio()
	{
		return (isEmptyField( this.codigoStr ) && isEmptyField( this.descricaoStr )
				&& isEmptyField( this.duracaoStr ) && isEmptyField( this.permiteIntervaloStr ));
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
		return descricaoStr;
	}
	
	public void setDescricaoStr( String descricaoStr )
	{
		this.descricaoStr = descricaoStr;
	}
	
	public void setDuracaoStr( String duracaoStr )
	{
		this.duracaoStr = duracaoStr;
	}
	
	public void setPermiteIntervaloStr( String permiteIntervaloStr )
	{
		this.permiteIntervaloStr = permiteIntervaloStr;
	}
	
	public Integer getDuracao()
	{
		return duracao;
	}
	
	public Boolean getPermiteIntervalo()
	{
		return permiteIntervalo;
	}

	@Override
	public int compareTo( SemanaLetivaImportExcelBean o )
	{
		return this.getCodigoStr().compareTo(o.getCodigoStr());
	}
}

