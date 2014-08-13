package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

public class DivisoesCreditoImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< DivisoesCreditoImportExcelBean >
{
	private String creditosStr;
	private String dia1Str;
	private String dia2Str;
	private String dia3Str;
	private String dia4Str;
	private String dia5Str;
	private String dia6Str;
	private String dia7Str;
	
	private int creditos;
	private int dia1;
	private int dia2;
	private int dia3;
	private int dia4;
	private int dia5;
	private int dia6;
	private int dia7;

	public DivisoesCreditoImportExcelBean( int row )
	{
		super( row );
	}

	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros
			= new ArrayList< ImportExcelError >();

		if ( !tudoVazio() )
		{
			checkMandatoryField( this.creditosStr, ImportExcelError.DIVISAO_CREDITO_CREDITOS_VAZIO, erros );
			checkMandatoryField( this.dia1Str, ImportExcelError.DIVISAO_CREDITO_DIA_VAZIO, erros );
			checkMandatoryField( this.dia2Str, ImportExcelError.DIVISAO_CREDITO_DIA_VAZIO, erros );
			checkMandatoryField( this.dia3Str, ImportExcelError.DIVISAO_CREDITO_DIA_VAZIO, erros );
			checkMandatoryField( this.dia4Str, ImportExcelError.DIVISAO_CREDITO_DIA_VAZIO, erros );
			checkMandatoryField( this.dia5Str, ImportExcelError.DIVISAO_CREDITO_DIA_VAZIO, erros );
			checkMandatoryField( this.dia6Str, ImportExcelError.DIVISAO_CREDITO_DIA_VAZIO, erros );
			checkMandatoryField( this.dia7Str, ImportExcelError.DIVISAO_CREDITO_DIA_VAZIO, erros );
			
			if(!erros.isEmpty())
				return erros;
			
			this.creditos = checkNonNegativeIntegerField( this.creditosStr,
					ImportExcelError.DIVISAO_CREDITO_DIA_FORMATO_INVALIDO,
					ImportExcelError.DIVISAO_CREDITO_DIA_VALOR_NEGATIVO, erros );
			
			this.dia1 = checkNonNegativeIntegerField( this.dia1Str,
					ImportExcelError.DIVISAO_CREDITO_DIA_FORMATO_INVALIDO,
					ImportExcelError.DIVISAO_CREDITO_DIA_VALOR_NEGATIVO, erros );
			
			this.dia2 = checkNonNegativeIntegerField( this.dia2Str,
					ImportExcelError.DIVISAO_CREDITO_DIA_FORMATO_INVALIDO,
					ImportExcelError.DIVISAO_CREDITO_DIA_VALOR_NEGATIVO, erros );
			
			this.dia3 = checkNonNegativeIntegerField( this.dia3Str,
					ImportExcelError.DIVISAO_CREDITO_DIA_FORMATO_INVALIDO,
					ImportExcelError.DIVISAO_CREDITO_DIA_VALOR_NEGATIVO, erros );
			
			this.dia4 = checkNonNegativeIntegerField( this.dia4Str,
					ImportExcelError.DIVISAO_CREDITO_DIA_FORMATO_INVALIDO,
					ImportExcelError.DIVISAO_CREDITO_DIA_VALOR_NEGATIVO, erros );
			
			this.dia5 = checkNonNegativeIntegerField( this.dia5Str,
					ImportExcelError.DIVISAO_CREDITO_DIA_FORMATO_INVALIDO,
					ImportExcelError.DIVISAO_CREDITO_DIA_VALOR_NEGATIVO, erros );
			
			this.dia6 = checkNonNegativeIntegerField( this.dia6Str,
					ImportExcelError.DIVISAO_CREDITO_DIA_FORMATO_INVALIDO,
					ImportExcelError.DIVISAO_CREDITO_DIA_VALOR_NEGATIVO, erros );
			
			this.dia7 = checkNonNegativeIntegerField( this.dia7Str,
					ImportExcelError.DIVISAO_CREDITO_DIA_FORMATO_INVALIDO,
					ImportExcelError.DIVISAO_CREDITO_DIA_VALOR_NEGATIVO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}

	private boolean tudoVazio()
	{
		return (isEmptyField( this.creditosStr ) &&
				isEmptyField( this.dia1Str ) && isEmptyField( this.dia2Str ) &&
				isEmptyField( this.dia3Str ) && isEmptyField( this.dia4Str ) &&
				isEmptyField( this.dia5Str ) && isEmptyField( this.dia6Str ) &&
				isEmptyField( this.dia7Str ) );
	}

	public String getCreditosStr()
	{
		return this.creditosStr;
	}

	public void setCreditoStr( String creditosStr )
	{
		this.creditosStr = creditosStr;
	}
	
	public String getDia1Str()
	{
		return this.dia1Str;
	}

	public void setDia1Str( String dia1Str )
	{
		this.dia1Str = dia1Str;
	}
	
	public String getDia2Str()
	{
		return this.dia2Str;
	}

	public void setDia2Str( String dia2Str )
	{
		this.dia2Str = dia2Str;
	}
	
	public String getDia3Str()
	{
		return this.dia3Str;
	}

	public void setDia3Str( String dia3Str )
	{
		this.dia3Str = dia3Str;
	}
	
	public String getDia4Str()
	{
		return this.dia4Str;
	}

	public void setDia4Str( String dia4Str )
	{
		this.dia4Str = dia4Str;
	}
	
	public String getDia5Str()
	{
		return this.dia5Str;
	}

	public void setDia5Str( String dia5Str )
	{
		this.dia5Str = dia5Str;
	}
	
	public String getDia6Str()
	{
		return this.dia6Str;
	}

	public void setDia6Str( String dia6Str )
	{
		this.dia6Str = dia6Str;
	}
	
	public String getDia7Str()
	{
		return this.dia7Str;
	}

	public void setDia7Str( String dia7Str )
	{
		this.dia7Str = dia7Str;
	}
	
	public int getCreditos()
	{
		return this.creditos;
	}
	
	public int getDia1()
	{
		return this.dia1;
	}
	
	public int getDia2()
	{
		return this.dia2;
	}

	public int getDia3()
	{
		return this.dia3;
	}

	public int getDia4()
	{
		return this.dia4;
	}

	public int getDia5()
	{
		return this.dia5;
	}
	
	public int getDia6()
	{
		return this.dia6;
	}

	public int getDia7()
	{
		return this.dia7;
	}


	@Override
	public int compareTo( DivisoesCreditoImportExcelBean o )
	{
		return this.getDivisaoKey().compareTo(o.getDivisaoKey());
	}
	
	public String getDivisaoKey()
	{
		return getDia1Str() + "-" + getDia2Str() + "-" + getDia3Str() + "-" +
				 getDia4Str() + "-" + getDia5Str() + "-" + getDia6Str() + "-" + getDia7Str();
	}
}
