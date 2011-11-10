package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.misc.Estados;

public class CampiImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< CampiImportExcelBean >
{
	private String codigoStr;
	private String nomeStr;
	private String estadoStr;
	private String municipioStr;
	private String bairroStr;
	private String custoMedioCreditoStr;
	private Double custoMedioCredito;
	private Estados estado;

	public CampiImportExcelBean( int row )
	{
		super( row );
		this.custoMedioCredito = null;
	}

	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros
			= new ArrayList< ImportExcelError >();

		if ( !tudoVazio() )
		{
			checkMandatoryField( this.codigoStr, ImportExcelError.CAMPUS_CODIGO_VAZIO, erros );
			checkMandatoryField( this.nomeStr, ImportExcelError.CAMPUS_NOME_VAZIO, erros );
			checkMandatoryField( this.custoMedioCreditoStr, ImportExcelError.CAMPUS_CUSTO_MEDIO_CREDITO_VAZIO, erros );

			this.custoMedioCredito = checkNonNegativeDoubleField( this.custoMedioCreditoStr,
				ImportExcelError.CAMPUS_CUSTO_MEDIO_CREDITO_FORMATO_INVALIDO,
				ImportExcelError.CAMPUS_CUSTO_MEDIO_CREDITO_VALOR_NEGATIVO, erros );

			this.estado = checkEnumField( this.estadoStr, Estados.class,
				ImportExcelError.CAMPUS_ESTADO_VALOR_INVALIDO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}

	protected boolean tudoVazio()
	{
		return isEmptyField( this.codigoStr ) && isEmptyField( this.nomeStr )
			&& isEmptyField( this.estadoStr ) && isEmptyField( this.municipioStr )
			&& isEmptyField( this.bairroStr ) && isEmptyField( this.custoMedioCreditoStr );
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

	public String getEstadoStr()
	{
		return this.estadoStr;
	}

	public void setEstadoStr( String estadoStr )
	{
		this.estadoStr = estadoStr;
	}

	public String getMunicipioStr()
	{
		return this.municipioStr;
	}

	public void setMunicipioStr( String municipioStr )
	{
		this.municipioStr = municipioStr;
	}

	public String getBairroStr()
	{
		return this.bairroStr;
	}

	public void setBairroStr( String bairroStr )
	{
		this.bairroStr = bairroStr;
	}

	public String getCustoMedioCreditoStr()
	{
		return this.custoMedioCreditoStr;
	}

	public void setCustoMedioCreditoStr( String custoMedioCreditoStr )
	{
		this.custoMedioCreditoStr = custoMedioCreditoStr;
	}

	public Double getCustoMedioCredito()
	{
		return this.custoMedioCredito;
	}

	public Estados getEstado()
	{
		return this.estado;
	}

	@Override
	public int compareTo( CampiImportExcelBean o )
	{
		return this.getCodigoStr().compareTo( o.getCodigoStr() );
	}
}
