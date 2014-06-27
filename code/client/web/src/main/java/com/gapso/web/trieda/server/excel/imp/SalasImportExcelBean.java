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
	private String desricaoStr;
	private String andarStr;
	private String capacidadeInstaladaStr;
	private String capacidadeMaxStr;
	private String custoOperacaoCredStr;
	private String externaStr;
	
	private TipoSala tipo;
	private Unidade unidade;
	private Integer capacidadeInstalada;
	private Integer capacidadeMax;
	private Double custoOperacaoCred;
	private Boolean externa;
	
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
			checkMandatoryField( this.capacidadeInstaladaStr, ImportExcelError.SALA_CAPACIDADE_INSTALADA_VAZIO, erros );
			checkMandatoryField( this.capacidadeMaxStr, ImportExcelError.SALA_CAPACIDADE_MAX_VAZIO, erros );
			checkMandatoryField( this.custoOperacaoCredStr, ImportExcelError.SALA_CUSTO_OPERACAO_CRED_VAZIO, erros );
			checkMandatoryField( this.externaStr, ImportExcelError.SALA_EXTERNA_VAZIO, erros );

			this.capacidadeInstalada = checkNonNegativeIntegerField( this.capacidadeInstaladaStr,
				ImportExcelError.SALA_CAPACIDADE_INSTALADA_FORMATO_INVALIDO,
				ImportExcelError.SALA_CAPACIDADE_INSTALADA_VALOR_NEGATIVO, erros );
			this.capacidadeMax = checkNonNegativeIntegerField( this.capacidadeMaxStr,
				ImportExcelError.SALA_CAPACIDADE_MAX_FORMATO_INVALIDO,
				ImportExcelError.SALA_CAPACIDADE_MAX_VALOR_NEGATIVO, erros );
			this.custoOperacaoCred = checkNonNegativeDoubleField( this.custoOperacaoCredStr,
				ImportExcelError.SALA_CUSTO_OPERACAO_CRED_FORMATO_INVALIDO,
				ImportExcelError.SALA_CUSTO_OPERACAO_CRED_VALOR_NEGATIVO, erros );
			this.externa = checkBooleanField( externaStr, ImportExcelError.SALA_EXTERNA_FORMATO_INVALIDO, erros );
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
			&& isEmptyField( this.desricaoStr )
			&& isEmptyField( this.andarStr )
			&& isEmptyField( this.capacidadeInstaladaStr )
			&& isEmptyField( this.externaStr ) );
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
	
	public String getDescricaoStr()
	{
		return this.desricaoStr;
	}

	public void setDescricaoStr( String desricaoStr )
	{
		this.desricaoStr = desricaoStr;
	}

	public String getAndarStr()
	{
		return this.andarStr;
	}

	public void setAndarStr( String andarStr )
	{
		this.andarStr = andarStr;
	}

	public String getCapacidadeInstaladaStr()
	{
		return this.capacidadeInstaladaStr;
	}

	public void setCapacidadeInstaladaStr( String capacidadeInstaladaStr )
	{
		this.capacidadeInstaladaStr = capacidadeInstaladaStr;
	}

	public String getCapacidadeMaxStr()
	{
		return this.capacidadeMaxStr;
	}

	public void setCapacidadeMaxStr( String capacidadeMaxStr )
	{
		this.capacidadeMaxStr = capacidadeMaxStr;
	}
	
	public String getCustoOperacaoCredStr()
	{
		return this.custoOperacaoCredStr;
	}

	public void setCustoOperacaoCredStr( String custoOperacaoCredStr )
	{
		this.custoOperacaoCredStr = custoOperacaoCredStr;
	}
	
	public String getExternaStr()
	{
		return this.externaStr;
	}
	
	public void setExternaStr( String externaStr )
	{
		this.externaStr = externaStr;
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

	public Integer getCapacidadeInstalada()
	{
		return this.capacidadeInstalada;
	}
	
	public Integer getCapacidadeMax()
	{
		return this.capacidadeMax;
	}
	
	public Double getCustoOperacaoCred()
	{
		return this.custoOperacaoCred;
	}
	
	public Boolean getExterna()
	{
		return this.externa;
	}

	@Override
	public int compareTo( SalasImportExcelBean o )
	{
		return this.getCodigoStr().compareTo( o.getCodigoStr() );
	}
}
