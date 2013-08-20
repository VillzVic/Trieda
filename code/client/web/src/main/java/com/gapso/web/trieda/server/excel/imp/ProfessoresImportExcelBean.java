package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.Titulacao;

public class ProfessoresImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< ProfessoresImportExcelBean >
{
	private String cpfStr;
	private String nomeStr;
	private String tipoStr;
	private String cargaHorariaMaxStr;
	private String cargaHorariaMinStr;
	private String titulacaoStr;
	private String areaTitulacaoStr;
	private String cargaHorariaAnteriorStr;
	private String valorCreditoStr;
	private String maxDiasSemanaStr;
	private String minCreditosDiaStr;

	private TipoContrato tipo;
	private Integer cargaHorariaMax;
	private Integer cargaHorariaMin;
	private Titulacao titulacao;
	private AreaTitulacao areaTitulacao;
	private Integer cargaHorariaAnterior;
	private Double valorCredito;
	private Integer maxDiasSemana;
	private Integer minCreditosDia;
	
	public ProfessoresImportExcelBean( int row )
	{
		super( row );
	}

	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros
			= new ArrayList< ImportExcelError >();

		if ( !tudoVazio() )
		{
			checkMandatoryField( this.cpfStr, ImportExcelError.PROFESSOR_CPF_VAZIO, erros );
			checkMandatoryField( this.nomeStr, ImportExcelError.PROFESSOR_NOME_VAZIO, erros );
			checkMandatoryField( this.tipoStr, ImportExcelError.PROFESSOR_TIPO_VAZIO, erros );
			checkMandatoryField( this.cargaHorariaMaxStr, ImportExcelError.PROFESSOR_CARGA_HORARIA_MAX_VAZIO, erros );
			checkMandatoryField( this.cargaHorariaMinStr, ImportExcelError.PROFESSOR_CARGA_HORARIA_MIN_VAZIO, erros );
			checkMandatoryField( this.titulacaoStr, ImportExcelError.PROFESSOR_TITULACAO_VAZIO, erros );
			checkMandatoryField( this.areaTitulacaoStr, ImportExcelError.PROFESSOR_AREA_TITULACAO_VAZIO, erros );
			checkMandatoryField( this.cargaHorariaAnteriorStr, ImportExcelError.PROFESSOR_CARGA_HORARIA_ANTERIOR_VAZIO, erros );
			checkMandatoryField( this.valorCreditoStr, ImportExcelError.PROFESSOR_VALOR_CREDITO_VAZIO, erros );
			checkMandatoryField( this.maxDiasSemanaStr, ImportExcelError.PROFESSOR_MAX_DIAS_SEMANA_VAZIO, erros );
			checkMandatoryField( this.minCreditosDiaStr, ImportExcelError.PROFESSOR_MIN_CREDITOS_DIA_VAZIO, erros );

			this.cargaHorariaMax = checkNonNegativeIntegerField( this.cargaHorariaMaxStr,
				ImportExcelError.PROFESSOR_CARGA_HORARIA_MAX_FORMATO_INVALIDO,
				ImportExcelError.PROFESSOR_CARGA_HORARIA_MAX_VALOR_NEGATIVO, erros );

			this.cargaHorariaMin = checkNonNegativeIntegerField( this.cargaHorariaMinStr,
				ImportExcelError.PROFESSOR_CARGA_HORARIA_MIN_FORMATO_INVALIDO,
				ImportExcelError.PROFESSOR_CARGA_HORARIA_MIN_VALOR_NEGATIVO, erros );

			this.cargaHorariaAnterior = checkNonNegativeIntegerField( this.cargaHorariaAnteriorStr,
				ImportExcelError.PROFESSOR_CARGA_HORARIA_ANTERIOR_FORMATO_INVALIDO,
				ImportExcelError.PROFESSOR_CARGA_HORARIA_ANTERIOR_VALOR_NEGATIVO, erros );

			this.valorCredito = checkNonNegativeDoubleField( this.valorCreditoStr,
				ImportExcelError.PROFESSOR_VALOR_CREDITO_FORMATO_INVALIDO,
				ImportExcelError.PROFESSOR_VALOR_CREDITO_VALOR_NEGATIVO, erros );
			
			this.maxDiasSemana = checkNonNegativeIntegerField( this.maxDiasSemanaStr,
					ImportExcelError.PROFESSOR_MAX_DIAS_SEMANA_FORMATO_INVALIDO,
					ImportExcelError.PROFESSOR_MAX_DIAS_SEMANA_VALOR_NEGATIVO, erros );
			
			this.minCreditosDia = checkNonNegativeIntegerField( this.minCreditosDiaStr,
					ImportExcelError.PROFESSOR_MIN_CREDITOS_DIA_FORMATO_INVALIDO,
					ImportExcelError.PROFESSOR_MIN_CREDITOS_DIA_VALOR_NEGATIVO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}

	private boolean tudoVazio()
	{
		return ( isEmptyField( this.cpfStr ) && isEmptyField( this.nomeStr )
			&& isEmptyField( this.tipoStr ) && isEmptyField( this.cargaHorariaMaxStr )
			&& isEmptyField( this.cargaHorariaMinStr ) && isEmptyField( this.titulacaoStr )
			&& isEmptyField( this.areaTitulacaoStr ) && isEmptyField( this.cargaHorariaAnteriorStr )
			&& isEmptyField( this.valorCreditoStr ) ) && isEmptyField( this.maxDiasSemanaStr )
			&& isEmptyField( minCreditosDiaStr );
	}

	public String getCpfStr()
	{
		return this.cpfStr;
	}

	public void setCpfStr( String cpfStr )
	{
		this.cpfStr = cpfStr;
	}

	public String getNomeStr()
	{
		return this.nomeStr;
	}

	public void setNomeStr( String nomeStr )
	{
		this.nomeStr = nomeStr;
	}

	public String getTipoStr()
	{
		return this.tipoStr;
	}

	public void setTipoStr( String tipoStr )
	{
		this.tipoStr = tipoStr;
	}

	public String getCargaHorariaMaxStr()
	{
		return this.cargaHorariaMaxStr;
	}

	public void setCargaHorariaMaxStr( String cargaHorariaMaxStr )
	{
		this.cargaHorariaMaxStr = cargaHorariaMaxStr;
	}

	public String getCargaHorariaMinStr()
	{
		return this.cargaHorariaMinStr;
	}

	public void setCargaHorariaMinStr( String cargaHorariaMinStr )
	{
		this.cargaHorariaMinStr = cargaHorariaMinStr;
	}

	public String getTitulacaoStr()
	{
		return this.titulacaoStr;
	}

	public void setTitulacaoStr( String titulacaoStr )
	{
		this.titulacaoStr = titulacaoStr;
	}

	public String getAreaTitulacaoStr()
	{
		return this.areaTitulacaoStr;
	}

	public void setAreaTitulacaoStr( String areaTitulacaoStr )
	{
		this.areaTitulacaoStr = areaTitulacaoStr;
	}

	public String getCargaHorariaAnteriorStr()
	{
		return this.cargaHorariaAnteriorStr;
	}

	public void setCargaHorariaAnteriorStr( String cargaHorariaAnteriorStr )
	{
		this.cargaHorariaAnteriorStr = cargaHorariaAnteriorStr;
	}

	public String getValorCreditoStr()
	{
		return this.valorCreditoStr;
	}

	public void setValorCreditoStr( String valorCreditoStr )
	{
		this.valorCreditoStr = valorCreditoStr;
	}
	
	public String getMaxDiasSemanaStr()
	{
		return this.maxDiasSemanaStr;
	}

	public void setMaxDiasSemanaStr( String maxDiasSemanaStr )
	{
		this.maxDiasSemanaStr = maxDiasSemanaStr;
	}
	
	public String getMinCreditosDiaStr()
	{
		return this.minCreditosDiaStr;
	}

	public void setMinCreditosDiaStr( String minCreditosDiaStr )
	{
		this.minCreditosDiaStr = minCreditosDiaStr;
	}

	public TipoContrato getTipo()
	{
		return this.tipo;
	}

	public Integer getCargaHorariaMax()
	{
		return this.cargaHorariaMax;
	}

	public Integer getCargaHorariaMin()
	{
		return this.cargaHorariaMin;
	}

	public Titulacao getTitulacao()
	{
		return this.titulacao;
	}

	public AreaTitulacao getAreaTitulacao()
	{
		return this.areaTitulacao;
	}

	public Integer getCargaHorariaAnterior()
	{
		return this.cargaHorariaAnterior;
	}

	public Double getValorCredito()
	{
		return this.valorCredito;
	}
	
	public Integer getMaxDiasSemana()
	{
		return this.maxDiasSemana;
	}
	
	public Integer getMinCreditosDia()
	{
		return this.minCreditosDia;
	}

	public void setTipo( TipoContrato tipo )
	{
		this.tipo = tipo;
	}

	public void setTitulacao( Titulacao titulacao )
	{
		this.titulacao = titulacao;
	}
	
	public void setMaxDiasSemana( Integer maxDiasSemana )
	{
		this.maxDiasSemana = maxDiasSemana;
	}
	
	public void setMinCreditosDia( Integer minCreditosDia )
	{
		this.minCreditosDia = minCreditosDia;
	}

	public void setAreaTitulacao( AreaTitulacao areaTitulacao )
	{
		this.areaTitulacao = areaTitulacao;
	}

	@Override
	public int compareTo( ProfessoresImportExcelBean o )
	{
		return this.getCpfStr().compareTo( o.getCpfStr() );
	}
}
