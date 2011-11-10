package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Professor;

public class CampiTrabalhoImportExcelBean	
	extends AbstractImportExcelBean
	implements Comparable< CampiTrabalhoImportExcelBean >
{
	private String campusStr;
	private String cpfStr;
	private String professorStr;
	private Campus campus;
	private Professor professor;

	public CampiTrabalhoImportExcelBean( int row )
	{
		super( row );
	}

	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros
			= new ArrayList< ImportExcelError >();

		if ( !tudoVazio() )
		{
			checkMandatoryField( this.campusStr, ImportExcelError.CAMPITRABALHO_CAMPUS_VAZIO, erros );
			checkMandatoryField( this.cpfStr, ImportExcelError.CAMPITRABALHO_CPF_VAZIO, erros );
			checkMandatoryField( this.professorStr, ImportExcelError.CAMPITRABALHO_PROFESSOR_VAZIO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}

	private boolean tudoVazio()
	{
		return isEmptyField( this.campusStr )
			&& isEmptyField( this.cpfStr )
			&& isEmptyField( this.professorStr );
	}

	public String getCampusStr()
	{
		return this.campusStr;
	}

	public void setCampusStr( String campusStr )
	{
		this.campusStr = campusStr;
	}

	public String getCpfStr()
	{
		return this.cpfStr;
	}

	public void setCpfStr( String turnoStr )
	{
		this.cpfStr = turnoStr;
	}

	public String getProfessorStr()
	{
		return this.professorStr;
	}

	public void setProfessorStr( String professorStr )
	{
		this.professorStr = professorStr;
	}

	public Campus getCampus()
	{
		return this.campus;
	}

	public void setCampus( Campus campus )
	{
		this.campus = campus;
	}

	public Professor getProfessor()
	{
		return this.professor;
	}

	public void setProfessor( Professor professor )
	{
		this.professor = professor;
	}

	@Override
	public int compareTo( CampiTrabalhoImportExcelBean o )
	{
		int result = this.getCampusStr().compareTo( o.getCampusStr() );

		if ( result == 0 )
		{
			result = this.getProfessorStr().compareTo( o.getProfessorStr() );
		}

		return result;
	}
}
