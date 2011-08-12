package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.TipoCurso;

public class CursosImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< CursosImportExcelBean >
{
	private String codigoStr;
	private String nomeStr;
	private String tipoStr;
	private String minDoutorPrecentStr;
	private String minMestrePrecentStr;
	private String minTempoIntegralParcialPrecentStr;
	private String minTempoIntegralPrecentStr;
	private String maxDisciplinasProfessorStr;
	private String maisDeUmaDisciplinaProfessorStr;

	private TipoCurso tipo;
	private Integer minDoutorPrecent;
	private Integer minMestrePrecent;
	private Integer minTempoIntegralParcialPrecent;
	private Integer minTempoIntegralPrecent;
	private Integer maxDisciplinasProfessor;
	private Boolean maisDeUmaDisciplinaProfessor;

	public CursosImportExcelBean( int row )
	{
		super( row );
	}

	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros
			= new ArrayList< ImportExcelError >();

		if ( !tudoVazio() )
		{
			checkMandatoryField( codigoStr, ImportExcelError.CURSO_CODIGO_VAZIO, erros );
			checkMandatoryField( nomeStr, ImportExcelError.CURSO_NOME_VAZIO, erros );
			checkMandatoryField( tipoStr, ImportExcelError.CURSO_TIPO_VAZIO, erros );
			checkMandatoryField( minDoutorPrecentStr, ImportExcelError.CURSO_MIN_DOUTOR_VAZIO, erros );
			checkMandatoryField( minMestrePrecentStr, ImportExcelError.CURSO_MIN_MESTRE_VAZIO, erros );
			checkMandatoryField( minTempoIntegralParcialPrecentStr, ImportExcelError.CURSO_MIN_TEMPO_INTEGRAL_PARCIAL_VAZIO, erros );
			checkMandatoryField( minTempoIntegralPrecentStr, ImportExcelError.CURSO_MIN_TEMPO_INTEGRAL_VAZIO, erros );
			checkMandatoryField( maxDisciplinasProfessorStr, ImportExcelError.CURSO_MAX_DISC_PROF_VAZIO, erros );
			checkMandatoryField( maisDeUmaDisciplinaProfessorStr, ImportExcelError.CURSO_MAIS_DE_UMA_DISC_PROF_VAZIO, erros );

			minDoutorPrecent = checkNonNegativeIntegerField( minDoutorPrecentStr,
				ImportExcelError.CURSO_MIN_DOUTOR_FORMATO_INVALIDO,
				ImportExcelError.CURSO_MIN_DOUTOR_VALOR_NEGATIVO, erros );

			minMestrePrecent = checkNonNegativeIntegerField( minMestrePrecentStr,
				ImportExcelError.CURSO_MIN_MESTRE_FORMATO_INVALIDO,
				ImportExcelError.CURSO_MIN_MESTRE_VALOR_NEGATIVO, erros );

			minTempoIntegralParcialPrecent = checkNonNegativeIntegerField( minTempoIntegralParcialPrecentStr,
				ImportExcelError.CURSO_MIN_TEMPO_INTEGRAL_PARCIAL_FORMATO_INVALIDO,
				ImportExcelError.CURSO_MIN_TEMPO_INTEGRAL_PARCIAL_VALOR_NEGATIVO, erros );

			minTempoIntegralPrecent = checkNonNegativeIntegerField( minTempoIntegralPrecentStr,
				ImportExcelError.CURSO_MIN_TEMPO_INTEGRAL_FORMATO_INVALIDO,
				ImportExcelError.CURSO_MIN_TEMPO_INTEGRAL_VALOR_NEGATIVO, erros );

			maxDisciplinasProfessor = checkNonNegativeIntegerField( maxDisciplinasProfessorStr,
				ImportExcelError.CURSO_MAX_DISC_PROF_FORMATO_INVALIDO,
				ImportExcelError.CURSO_MAX_DISC_PROF_VALOR_NEGATIVO, erros );

			maisDeUmaDisciplinaProfessor = checkBooleanField( maisDeUmaDisciplinaProfessorStr,
				ImportExcelError.CURSO_MAIS_DE_UMA_DISC_PROF_FORMATO_INVALIDO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}
	
	private boolean tudoVazio()
	{
		return isEmptyField( codigoStr ) && isEmptyField( nomeStr )
			&& isEmptyField( tipoStr ) && isEmptyField( minDoutorPrecentStr )
			&& isEmptyField( minMestrePrecentStr ) && isEmptyField( maxDisciplinasProfessorStr )
			&& isEmptyField( maisDeUmaDisciplinaProfessorStr );
	}

	public String getCodigoStr() {
		return codigoStr;
	}

	public void setCodigoStr(String codigoStr) {
		this.codigoStr = codigoStr;
	}

	public String getNomeStr() {
		return nomeStr;
	}

	public void setNomeStr(String nomeStr) {
		this.nomeStr = nomeStr;
	}

	public String getTipoStr() {
		return tipoStr;
	}

	public void setTipoStr(String tipoStr) {
		this.tipoStr = tipoStr;
	}

	public String getMinDoutorPrecentStr() {
		return minDoutorPrecentStr;
	}

	public void setMinDoutorPrecentStr(String minDoutorPrecentStr) {
		this.minDoutorPrecentStr = minDoutorPrecentStr;
	}
	
	public String getMinTempoIntegralParcialPrecentStr() {
		return minTempoIntegralParcialPrecentStr;
	}

	public void setMinTempoIntegralParcialPrecentStr(
			String minTempoIntegralParcialPrecentStr) {
		this.minTempoIntegralParcialPrecentStr = minTempoIntegralParcialPrecentStr;
	}

	public String getMinTempoIntegralPrecentStr() {
		return minTempoIntegralPrecentStr;
	}

	public void setMinTempoIntegralPrecentStr(String minTempoIntegralPrecentStr) {
		this.minTempoIntegralPrecentStr = minTempoIntegralPrecentStr;
	}

	public Integer getMinTempoIntegralParcialPrecent() {
		return minTempoIntegralParcialPrecent;
	}

	public Integer getMinTempoIntegralPrecent() {
		return minTempoIntegralPrecent;
	}

	public String getMinMestrePrecentStr() {
		return minMestrePrecentStr;
	}

	public void setMinMestrePrecentStr(String minMestrePrecentStr) {
		this.minMestrePrecentStr = minMestrePrecentStr;
	}

	public String getMaxDisciplinasProfessorStr() {
		return maxDisciplinasProfessorStr;
	}

	public void setMaxDisciplinasProfessorStr(String maxDisciplinasProfessorStr) {
		this.maxDisciplinasProfessorStr = maxDisciplinasProfessorStr;
	}

	public String getMaisDeUmaDisciplinaProfessorStr() {
		return maisDeUmaDisciplinaProfessorStr;
	}

	public void setMaisDeUmaDisciplinaProfessorStr(
			String maisDeUmaDisciplinaProfessorStr) {
		this.maisDeUmaDisciplinaProfessorStr = maisDeUmaDisciplinaProfessorStr;
	}

	public TipoCurso getTipo() {
		return tipo;
	}

	public void setTipo(TipoCurso tipo) {
		this.tipo = tipo;
	}

	public Integer getMinDoutorPrecent() {
		return minDoutorPrecent;
	}

	public Integer getMinMestrePrecent() {
		return minMestrePrecent;
	}

	public Integer getMaxDisciplinasProfessor() {
		return maxDisciplinasProfessor;
	}

	public Boolean getMaisDeUmaDisciplinaProfessor() {
		return maisDeUmaDisciplinaProfessor;
	}

	@Override
	public int compareTo( CursosImportExcelBean o )
	{
		return getCodigoStr().compareTo( o.getCodigoStr() );
	}
}
