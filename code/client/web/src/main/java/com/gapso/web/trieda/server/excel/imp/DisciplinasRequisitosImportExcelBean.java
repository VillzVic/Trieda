package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Disciplina;

public class DisciplinasRequisitosImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< DisciplinasRequisitosImportExcelBean >
{
	private String curriculoStr;
	private String periodoStr;
	private String disciplinaStr;
	private String disciplinaRequisitoStr;
	
	private CurriculoDisciplina curriculoDisciplina;
	private Integer periodo;
	private Disciplina disciplinaRequisito;
	
	public DisciplinasRequisitosImportExcelBean( int row )
	{
		super( row );
	}
	
	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros = new ArrayList< ImportExcelError >();
	
		if ( !tudoVazio() )
		{
			checkMandatoryField( curriculoStr, ImportExcelError.CURRICULO_CODIGO_VAZIO,erros );
			checkMandatoryField( periodoStr, ImportExcelError.CURRICULO_PERIODO_VAZIO,erros );
			checkMandatoryField( disciplinaStr, ImportExcelError.CURRICULO_DISCIPLINA_VAZIO, erros );
			checkMandatoryField( disciplinaRequisitoStr, ImportExcelError.DISCIPLINA_CODIGO_VAZIO, erros );
	
			periodo = checkNonNegativeIntegerField( periodoStr,
				ImportExcelError.CURRICULO_PERIODO_FORMATO_INVALIDO,
				ImportExcelError.CURRICULO_PERIODO_VALOR_NEGATIVO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}
	
		return erros;
	}
	
	private boolean tudoVazio()
	{
		return ( isEmptyField( curriculoStr )
			&& isEmptyField( periodoStr )
			&& isEmptyField( disciplinaStr )
			&& isEmptyField( disciplinaRequisitoStr ) );
	}
	
	public String getCurriculoStr()
	{
		return curriculoStr;
	}
	public void setCurriculoStr( String curriculoStr )
	{
		this.curriculoStr = curriculoStr;
	}
	
	public String getPeriodoStr()
	{
		return periodoStr;
	}
	public void setPeriodoStr( String periodoStr )
	{
		this.periodoStr = periodoStr;
	}
	
	public String getDisciplinaStr()
	{
		return disciplinaStr;
	}
	public void setDisciplinaStr( String disciplinaStr )
	{
		this.disciplinaStr = disciplinaStr;
	}
	
	public String getDisciplinaRequisitoStr()
	{
		return disciplinaRequisitoStr;
	}
	public void setDisciplinaRequisitoStr( String disciplinaRequisitoStr )
	{
		this.disciplinaRequisitoStr = disciplinaRequisitoStr;
	}
	
	public Disciplina getDisciplinaRequisito()
	{
		return this.disciplinaRequisito;
	}
	public void setDisciplinaRequisito( Disciplina disciplinaRequisito )
	{
		this.disciplinaRequisito = disciplinaRequisito;
	}
	
	public CurriculoDisciplina getCurriculoDisciplina()
	{
		return this.curriculoDisciplina;
	}
	public void setCurriculoDisciplina( CurriculoDisciplina curriculoDisciplina )
	{
		this.curriculoDisciplina = curriculoDisciplina;
	}
	
	public Integer getPeriodo()
	{
		return this.periodo;
	}
	
	@Override
	public int compareTo( DisciplinasRequisitosImportExcelBean o )
	{
		return (getCurriculoStr() + "-" + getPeriodoStr() + "-" + getDisciplinaStr()
				+ "-" + getDisciplinaRequisitoStr()).compareTo( 
						o.getCurriculoStr() + "-" + o.getPeriodoStr() + "-" + o.getDisciplinaStr()
						+ "-" + o.getDisciplinaRequisitoStr());
	}
}