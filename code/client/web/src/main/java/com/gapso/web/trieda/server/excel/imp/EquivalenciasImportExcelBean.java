package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;

public class EquivalenciasImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< EquivalenciasImportExcelBean >
{
	private String cursouStr;
	private String eliminaStr;
	private String cursoStr;

	private Disciplina disciplinaCursou;
	private Disciplina disciplinaElimina;
	private Curso curso;

	public EquivalenciasImportExcelBean( int row )
	{
		super( row );
	}

	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros
			= new ArrayList< ImportExcelError >();

		if ( !tudoVazio() )
		{
			checkMandatoryField( this.cursouStr, ImportExcelError.EQUIVALENCIA_CURSOU_VAZIO, erros );
			checkMandatoryField( this.eliminaStr, ImportExcelError.EQUIVALENCIA_ELIMINA_VAZIO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}

	private boolean tudoVazio()
	{
		return ( isEmptyField( this.cursouStr )
			&& isEmptyField( this.eliminaStr ) );
	}

	public String getCursouStr()
	{
		return this.cursouStr;
	}

	public void setCursouStr( String cursouStr )
	{
		this.cursouStr = cursouStr;
	}

	public String getEliminaStr()
	{
		return this.eliminaStr;
	}

	public void setEliminaStr( String eliminaStr )
	{
		this.eliminaStr = eliminaStr;
	}
	
	public String getCursoStr()
	{
		return this.cursoStr;
	}

	public void setCursoStr( String cursoStr )
	{
		this.cursoStr = cursoStr;
	}

	public Disciplina getDisciplinaCursou()
	{
		return this.disciplinaCursou;
	}

	public void setDisciplinaCursou( Disciplina disciplinaCursou )
	{
		this.disciplinaCursou = disciplinaCursou;
	}

	public Disciplina getDisciplinaElimina()
	{
		return this.disciplinaElimina;
	}

	public void setDisciplinaElimina(Disciplina disciplinaElimina )
	{
		this.disciplinaElimina = disciplinaElimina;
	}
	
	public Curso getCurso()
	{
		return this.curso;
	}

	public void setCurso(Curso curso )
	{
		this.curso = curso;
	}

	@Override
	public int compareTo( EquivalenciasImportExcelBean o )
	{
		return getCursouStr().compareTo( o.getCursouStr() );
	}
}
