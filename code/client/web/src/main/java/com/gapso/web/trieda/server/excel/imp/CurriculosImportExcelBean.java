package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.SemanaLetiva;

public class CurriculosImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< CurriculosImportExcelBean >
{
	private String codigoStr;
	private String descricaoStr;
	private String cursoCodigoStr;
	private String periodoStr;
	private String disciplinaCodigoStr;
	private String semanaLetivaCodigoStr;

	private Curso curso;
	private Integer periodo;
	private Disciplina disciplina;
	private SemanaLetiva semanaLetiva;

	public CurriculosImportExcelBean( int row )
	{
		super( row );
	}
	
	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros
			= new ArrayList< ImportExcelError >();

		if ( !tudoVazio() )
		{
			checkMandatoryField( this.codigoStr, ImportExcelError.CURRICULO_CODIGO_VAZIO, erros );
			checkMandatoryField( this.descricaoStr, ImportExcelError.CURRICULO_DESCRICAO_VAZIO, erros );
			checkMandatoryField( this.cursoCodigoStr, ImportExcelError.CURRICULO_CURSO_VAZIO, erros );
			checkMandatoryField( this.periodoStr, ImportExcelError.CURRICULO_PERIODO_VAZIO, erros );
			checkMandatoryField( this.disciplinaCodigoStr, ImportExcelError.CURRICULO_DISCIPLINA_VAZIO, erros );
			checkMandatoryField( this.semanaLetivaCodigoStr, ImportExcelError.CURRICULO_SEMANA_LETIVA_VAZIA, erros );

			this.periodo = checkNonNegativeIntegerField( this.periodoStr,
				ImportExcelError.CURRICULO_PERIODO_FORMATO_INVALIDO,
				ImportExcelError.CURRICULO_PERIODO_VALOR_NEGATIVO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}
	
	public boolean tudoVazio()
	{
		return ( isEmptyField( this.codigoStr )
			&& isEmptyField( this.descricaoStr )
			&& isEmptyField( this.cursoCodigoStr )
			&& isEmptyField( this.periodoStr )
			&& isEmptyField( this.disciplinaCodigoStr )
			&& isEmptyField( this.semanaLetivaCodigoStr ) );
	}

	public String getNaturalKeyString()
	{
		return this.getCursoCodigoStr() +
			"-" + this.getCodigoStr() +
			"-" + this.getPeriodo().toString() +
			"-" + this.getDisciplinaCodigoStr();
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

	public String getCursoCodigoStr()
	{
		return this.cursoCodigoStr;
	}

	public void setCursoCodigoStr( String cursoCodigoStr )
	{
		this.cursoCodigoStr = cursoCodigoStr;
	}

	public String getPeriodoStr()
	{
		return this.periodoStr;
	}

	public void setPeriodoStr( String periodoStr )
	{
		this.periodoStr = periodoStr;
	}

	public String getSemanaLetivaCodigoStr()
	{
		return this.semanaLetivaCodigoStr;
	}

	public void setSemanaLetivaCodigoStr( String semanaLetivaCodigoStr )
	{
		this.semanaLetivaCodigoStr = semanaLetivaCodigoStr;
	}

	public String getDisciplinaCodigoStr()
	{
		return this.disciplinaCodigoStr;
	}

	public void setDisciplinaCodigoStr( String disciplinaCodigoStr )
	{
		this.disciplinaCodigoStr = disciplinaCodigoStr;
	}

	public Curso getCurso()
	{
		return this.curso;
	}

	public void setCurso( Curso curso )
	{
		this.curso = curso;
	}

	public Disciplina getDisciplina()
	{
		return this.disciplina;
	}

	public void setDisciplina( Disciplina disciplina )
	{
		this.disciplina = disciplina;
	}

	public SemanaLetiva getSemanaLetiva()
	{
		return this.semanaLetiva;
	}

	public void setSemanaLetiva( SemanaLetiva semanaLetiva )
	{
		this.semanaLetiva = semanaLetiva;
	}

	public Integer getPeriodo()
	{
		return this.periodo;
	}

	public void setPeriodo( Integer periodo )
	{
		this.periodo = periodo;
	}

	@Override
	public int compareTo( CurriculosImportExcelBean o )
	{
		return getCodigoStr().compareTo( o.getCodigoStr() );
	}

	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null
			|| !( obj instanceof CurriculosImportExcelBean )  )
		{
			return false;
		}

		CurriculosImportExcelBean other = (CurriculosImportExcelBean) obj;

		return this.getNaturalKeyString().equals( other.getNaturalKeyString() );
	}

	public static Map< String, CurriculosImportExcelBean > buildCurriculoCodigoToImportExcelBeanMap(
		List< CurriculosImportExcelBean > beans )
	{
		Map< String, CurriculosImportExcelBean > beansMap
			= new HashMap< String, CurriculosImportExcelBean >();

		for ( CurriculosImportExcelBean bean : beans )
		{
			beansMap.put( bean.getCodigoStr(), bean );
		}

		return beansMap;
	}
}
