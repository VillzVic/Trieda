package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

public class FixacoesImportExcelBean extends AbstractImportExcelBean implements Comparable< FixacoesImportExcelBean >
{
	private String codigoStr;
	private String descricaoStr;
	private String professorStr;
	private String disciplinaStr;
	private String campusStr;
	private String unidadeStr;
	private String salaStr;
	private String horariosStr;

	public FixacoesImportExcelBean( int row )
	{
		super( row );
	}

	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros = new ArrayList< ImportExcelError >();

		if (!tudoVazio())
		{
			checkMandatoryField(this.codigoStr, ImportExcelError.FIXACAO_CODIGO_VAZIO, erros);
			checkMandatoryField(this.descricaoStr, ImportExcelError.FIXACAO_DESCRICAO_VAZIO, erros);
			checkMandatoryField(this.professorStr, ImportExcelError.FIXACAO_PROFESSOR_VAZIO, erros);
			checkMandatoryField(this.disciplinaStr, ImportExcelError.FIXACAO_DISCIPLINA_VAZIO, erros);
			checkMandatoryField(this.campusStr, ImportExcelError.FIXACAO_CAMPUS_VAZIO, erros);
			checkMandatoryField(this.unidadeStr, ImportExcelError.FIXACAO_UNIDADE_VAZIO, erros);
			checkMandatoryField(this.salaStr, ImportExcelError.FIXACAO_SALA_VAZIO, erros);
			checkMandatoryField(this.horariosStr, ImportExcelError.FIXACAO_HORARIOS_VAZIO, erros);
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}

	protected boolean tudoVazio()
	{
		return isEmptyField( this.codigoStr ) && isEmptyField( this.descricaoStr )
			&& isEmptyField( this.professorStr ) && isEmptyField( this.disciplinaStr )
			&& isEmptyField( this.campusStr ) && isEmptyField( this.unidadeStr )
			&& isEmptyField( this.salaStr ) && isEmptyField( this.horariosStr );
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
		this.descricaoStr = codigoStr;
	}

	public String getProfessorStr()
	{
		return this.professorStr;
	}

	public void setProfessorStr( String professorStr )
	{
		this.professorStr = professorStr;
	}

	public String getDisciplinaStr()
	{
		return this.disciplinaStr;
	}

	public void setDisciplinaStr( String disciplinaStr )
	{
		this.disciplinaStr = disciplinaStr;
	}

	public String getCampusStr()
	{
		return this.campusStr;
	}

	public void setCampusStr( String campusStr )
	{
		this.campusStr = campusStr;
	}

	public String getUnidadeStr()
	{
		return this.unidadeStr;
	}

	public void setUnidadeStr( String unidadeStr )
	{
		this.unidadeStr = unidadeStr;
	}

	public String getSalaStr()
	{
		return this.salaStr;
	}

	public void setSalaStr( String salaStr )
	{
		this.salaStr = salaStr;
	}

	public String getHorariosStr()
	{
		return this.horariosStr;
	}

	public void setHorariosStr( String horariosStr )
	{
		this.horariosStr = horariosStr;
	}

	@Override
	public int compareTo( FixacoesImportExcelBean o )
	{
		return this.getCodigoStr().compareTo( o.getCodigoStr() );
	}
}
