package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Turno;

public class DemandasImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< DemandasImportExcelBean >
{
	private String campusStr;
	private String turnoStr;
	private String cursoStr;
	private String matrizCurricularStr;
	private String periodoStr;
	private String disciplinaStr;
	private String demandaStr;
	private String receitaStr;

	private Campus campus;
	private Turno turno;
	private Curso curso;
	private Curriculo matrizCurricular;
	private Integer periodo;
	private Disciplina disciplina;
	private Integer demanda;
	private Double receita;

	public DemandasImportExcelBean( int row )
	{
		super( row );

		this.receita = 0.0;
		this.receitaStr = this.receita.toString();
	}

	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros
			= new ArrayList< ImportExcelError >();

		if ( !tudoVazio() )
		{
			checkMandatoryField( this.campusStr, ImportExcelError.DEMANDA_CAMPUS_VAZIO, erros );
			checkMandatoryField( this.turnoStr, ImportExcelError.DEMANDA_TURNO_VAZIO, erros );
			checkMandatoryField( this.cursoStr, ImportExcelError.DEMANDA_CURSO_VAZIO, erros );
			checkMandatoryField( this.matrizCurricularStr, ImportExcelError.DEMANDA_MATRIZ_CURRIULAR_VAZIO, erros );
			checkMandatoryField( this.periodoStr, ImportExcelError.DEMANDA_PERIODO_VAZIO, erros );
			checkMandatoryField( this.disciplinaStr, ImportExcelError.DEMANDA_DISCIPLINA_VAZIO, erros );
			checkMandatoryField( this.demandaStr, ImportExcelError.DEMANDA_DEMANDA_VAZIO, erros );

			this.receita = checkNonNegativeDoubleField( this.receitaStr,
				ImportExcelError.DEMANDA_RECEITA_FORMATO_INVALIDO,
				ImportExcelError.DEMANDA_RECEITA_VALOR_NEGATIVO, erros );

			this.periodo = checkNonNegativeIntegerField( this.periodoStr,
				ImportExcelError.DEMANDA_PERIODO_FORMATO_INVALIDO,
				ImportExcelError.DEMANDA_PERIODO_VALOR_NEGATIVO, erros );

			this.demanda = checkNonNegativeIntegerField( this.demandaStr,
				ImportExcelError.DEMANDA_DEMANDA_FORMATO_INVALIDO,
				ImportExcelError.DEMANDA_DEMANDA_VALOR_NEGATIVO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}

	private boolean tudoVazio()
	{
		return ( isEmptyField( this.campusStr )
			&& isEmptyField( this.turnoStr )
			&& isEmptyField( this.cursoStr )
			&& isEmptyField( this.matrizCurricularStr )
			&& isEmptyField( this.periodoStr )
			&& isEmptyField( this.disciplinaStr )
			&& isEmptyField( this.demandaStr )
			&& isEmptyField( this.receitaStr ) );
	}

	public String getReceitaStr()
	{
		return this.receitaStr;
	}

	public void setReceitaStr( String receitaStr )
	{
		this.receitaStr = receitaStr;
	}

	public Double getReceita()
	{
		return this.receita;
	}

	public void setReceita( Double receita )
	{
		this.receita = receita;
	}

	public String getCampusStr()
	{
		return this.campusStr;
	}

	public void setCampusStr( String campusStr )
	{
		this.campusStr = campusStr;
	}

	public String getTurnoStr()
	{
		return this.turnoStr;
	}

	public void setTurnoStr( String turnoStr )
	{
		this.turnoStr = turnoStr;
	}

	public String getCursoStr()
	{
		return this.cursoStr;
	}

	public void setCursoStr( String cursoStr )
	{
		this.cursoStr = cursoStr;
	}

	public String getMatrizCurricularStr()
	{
		return this.matrizCurricularStr;
	}

	public void setMatrizCurricularStr( String matrizCurricularStr )
	{
		this.matrizCurricularStr = matrizCurricularStr;
	}

	public String getPeriodoStr()
	{
		return this.periodoStr;
	}

	public void setPeriodoStr( String periodoStr )
	{
		this.periodoStr = periodoStr;
	}

	public String getDisciplinaStr()
	{
		return this.disciplinaStr;
	}

	public void setDisciplinaStr( String disciplinaStr )
	{
		this.disciplinaStr = disciplinaStr;
	}

	public String getDemandaStr()
	{
		return this.demandaStr;
	}

	public void setDemandaStr( String demandaStr )
	{
		this.demandaStr = demandaStr;
	}

	public Campus getCampus()
	{
		return this.campus;
	}

	public void setCampus( Campus campus )
	{
		this.campus = campus;
	}

	public Turno getTurno()
	{
		return this.turno;
	}

	public void setTurno( Turno turno )
	{
		this.turno = turno;
	}

	public Curso getCurso()
	{
		return this.curso;
	}

	public void setCurso( Curso curso )
	{
		this.curso = curso;
	}

	public Curriculo getMatrizCurricular()
	{
		return this.matrizCurricular;
	}

	public void setMatrizCurricular(
		Curriculo matrizCurricular )
	{
		this.matrizCurricular = matrizCurricular;
	}

	public Integer getPeriodo()
	{
		return this.periodo;
	}

	public void setPeriodo( Integer periodo )
	{
		this.periodo = periodo;
	}

	public Disciplina getDisciplina()
	{
		return this.disciplina;
	}

	public void setDisciplina( Disciplina disciplina )
	{
		this.disciplina = disciplina;
	}

	public Integer getDemanda()
	{
		return this.demanda;
	}

	public void setDemanda( Integer demanda )
	{
		this.demanda = demanda;
	}

	@Override
	public int compareTo( DemandasImportExcelBean o )
	{
		int result = this.getCampusStr().compareTo( o.getCampusStr() );

		if ( result == 0 )
		{
			result = this.getTurnoStr().compareTo( o.getTurnoStr() );

			if ( result == 0 )
			{
				result = this.getCursoStr().compareTo( o.getCursoStr() );

				if ( result == 0 )
				{
					result = this.getMatrizCurricularStr().compareTo( o.getMatrizCurricularStr() );

					if ( result == 0 )
					{
						result = this.getDisciplinaStr().compareTo( o.getDisciplinaStr() );
					}
				}
			}
		}

		return result;
	}
}
