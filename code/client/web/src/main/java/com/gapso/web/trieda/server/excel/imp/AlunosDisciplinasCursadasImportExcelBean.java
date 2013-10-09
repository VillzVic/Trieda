package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.CurriculoDisciplina;

public class AlunosDisciplinasCursadasImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< AlunosDisciplinasCursadasImportExcelBean >
	{
	private String cursoStr;
	private String curriculoStr;
	private String periodoStr;
	private String disciplinaStr;
	private String alunoStr;
	
	private CurriculoDisciplina curriculoDisciplina;
	private Integer periodo;
	private Aluno aluno;
	
	public AlunosDisciplinasCursadasImportExcelBean( int row )
	{
		super( row );
	}
	
	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros = new ArrayList< ImportExcelError >();
	
		if ( !tudoVazio() )
		{
			checkMandatoryField( cursoStr, ImportExcelError.CURSO_CODIGO_VAZIO,erros );
			checkMandatoryField( curriculoStr, ImportExcelError.CURRICULO_CODIGO_VAZIO,erros );
			checkMandatoryField( periodoStr, ImportExcelError.CURRICULO_PERIODO_VAZIO,erros );
			checkMandatoryField( disciplinaStr, ImportExcelError.CURRICULO_DISCIPLINA_VAZIO, erros );
			checkMandatoryField( alunoStr, ImportExcelError.DISCIPLINA_CODIGO_VAZIO, erros );
	
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
			&& isEmptyField( alunoStr ) );
	}
	
	public String getCursoStr()
	{
		return cursoStr;
	}
	public void setCursoStr( String cursoStr )
	{
		this.cursoStr = cursoStr;
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
	
	public String getAlunoStr()
	{
		return alunoStr;
	}
	public void setAlunoStr( String alunoStr )
	{
		this.alunoStr = alunoStr;
	}
	
	public Aluno getAluno()
	{
		return this.aluno;
	}
	public void setAluno( Aluno aluno )
	{
		this.aluno = aluno;
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
	public int compareTo( AlunosDisciplinasCursadasImportExcelBean o )
	{
		return (getCurriculoStr() + "-" + getPeriodoStr() + "-" + getDisciplinaStr()
				+ "-" + getAlunoStr()).compareTo( 
						o.getCurriculoStr() + "-" + o.getPeriodoStr() + "-" + o.getDisciplinaStr()
						+ "-" + o.getAlunoStr());
	}
}