package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.TipoSala;
import com.gapso.trieda.domain.Unidade;

public class SalasImportExcelBean extends AbstractImportExcelBean implements Comparable<SalasImportExcelBean> {
	
	private String codigoStr;
	private String codigoUnidadeStr;
	private String tipoStr;
	private String numeroStr;
	private String andarStr;
	private String capacidadeStr;
	
	private TipoSala tipo;
	private Unidade unidade;
	private Integer capacidade;
	
	public SalasImportExcelBean( int row )
	{
		super( row );
	}
	
	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros
			= new ArrayList< ImportExcelError >();

		if ( !tudoVazio() )
		{
			checkMandatoryField( this.codigoStr, ImportExcelError.SALA_CODIGO_VAZIO, erros );
			checkMandatoryField( this.codigoUnidadeStr, ImportExcelError.SALA_UNIDADE_VAZIO, erros );
			checkMandatoryField( this.tipoStr, ImportExcelError.SALA_TIPO_VAZIO, erros );
			checkMandatoryField( this.numeroStr, ImportExcelError.SALA_NUMERO_VAZIO, erros );
			checkMandatoryField( this.andarStr, ImportExcelError.SALA_ANDAR_VAZIO, erros );
			checkMandatoryField( this.capacidadeStr, ImportExcelError.SALA_CAPACIDADE_VAZIO, erros );

			this.capacidade = checkNonNegativeIntegerField( this.capacidadeStr,
				ImportExcelError.SALA_CAPACIDADE_FORMATO_INVALIDO,
				ImportExcelError.SALA_CAPACIDADE_VALOR_NEGATIVO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}
	
	private boolean tudoVazio()
	{
		return ( isEmptyField( this.codigoStr )
			&& isEmptyField( this.codigoUnidadeStr )
			&& isEmptyField( this.tipoStr )
			&& isEmptyField( this.numeroStr )
			&& isEmptyField( this.andarStr )
			&& isEmptyField( this.capacidadeStr ) );
	}

	public String getCodigoStr()
	{
		return this.codigoStr;
	}

	public void setCodigoStr( String codigoStr )
	{
		this.codigoStr = codigoStr;
	}

	public String getCodigoUnidadeStr()
	{
		return this.codigoUnidadeStr;
	}

	public void setCodigoUnidadeStr( String codigoUnidadeStr )
	{
		this.codigoUnidadeStr = codigoUnidadeStr;
	}

	public String getTipoStr()
	{
		return this.tipoStr;
	}

	public void setTipoStr( String tipoStr )
	{
		this.tipoStr = tipoStr;
	}

	public String getNumeroStr()
	{
		return this.numeroStr;
	}

	public void setNumeroStr( String numeroStr )
	{
		this.numeroStr = numeroStr;
	}

	public String getAndarStr()
	{
		return this.andarStr;
	}

	public void setAndarStr( String andarStr )
	{
		this.andarStr = andarStr;
	}

	public String getCapacidadeStr()
	{
		return this.capacidadeStr;
	}

	public void setCapacidadeStr( String capacidadeStr )
	{
		this.capacidadeStr = capacidadeStr;
	}

	public Unidade getUnidade()
	{
		return this.unidade;
	}

	public void setUnidade( Unidade unidade )
	{
		this.unidade = unidade;
	}

	public TipoSala getTipo()
	{
		return this.tipo;
	}

	public void setTipo( TipoSala tipo )
	{
		this.tipo = tipo;
	}

	public Integer getCapacidade()
	{
		return this.capacidade;
	}

	@Override
	public int compareTo( SalasImportExcelBean o )
	{
		return this.getCodigoStr().compareTo( o.getCodigoStr() );
	}
}
