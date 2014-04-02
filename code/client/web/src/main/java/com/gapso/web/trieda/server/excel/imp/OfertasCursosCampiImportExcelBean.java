package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Turno;

public class OfertasCursosCampiImportExcelBean
extends AbstractImportExcelBean
implements Comparable< OfertasCursosCampiImportExcelBean >
{
	
	private String campusStr;
	private String turnoStr;
	private String cursoStr;
	private String matrizCurricularStr;
	private String receitaStr;
	
	private Campus campus;
	private Turno turno;
	private Curso curso;
	private Curriculo matrizCurricular;
	private Double receita;
	
	public OfertasCursosCampiImportExcelBean( int row )
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

			this.receita = checkNonNegativeDoubleField( this.receitaStr,
				ImportExcelError.DEMANDA_RECEITA_FORMATO_INVALIDO,
				ImportExcelError.DEMANDA_RECEITA_VALOR_NEGATIVO, erros );

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
			&& isEmptyField( this.receitaStr ) );
	}
	
	

	@Override
	public int compareTo(OfertasCursosCampiImportExcelBean o) {
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

				}
			}
		}

		return result;
	}

	public String getCampusStr() {
		return campusStr;
	}

	public void setCampusStr(String campusStr) {
		this.campusStr = campusStr;
	}

	public String getTurnoStr() {
		return turnoStr;
	}

	public void setTurnoStr(String turnoStr) {
		this.turnoStr = turnoStr;
	}

	public String getCursoStr() {
		return cursoStr;
	}

	public void setCursoStr(String cursoStr) {
		this.cursoStr = cursoStr;
	}

	public String getMatrizCurricularStr() {
		return matrizCurricularStr;
	}

	public void setMatrizCurricularStr(String matrizCurricularStr) {
		this.matrizCurricularStr = matrizCurricularStr;
	}

	public String getReceitaStr() {
		return receitaStr;
	}

	public void setReceitaStr(String receitaStr) {
		this.receitaStr = receitaStr;
	}

	public Campus getCampus() {
		return campus;
	}

	public void setCampus(Campus campus) {
		this.campus = campus;
	}

	public Turno getTurno() {
		return turno;
	}

	public void setTurno(Turno turno) {
		this.turno = turno;
	}

	public Curso getCurso() {
		return curso;
	}

	public void setCurso(Curso curso) {
		this.curso = curso;
	}

	public Curriculo getMatrizCurricular() {
		return matrizCurricular;
	}

	public void setMatrizCurricular(Curriculo matrizCurricular) {
		this.matrizCurricular = matrizCurricular;
	}

	public Double getReceita() {
		return receita;
	}

	public void setReceita(Double receita) {
		this.receita = receita;
	}

}
