package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import com.gapso.trieda.domain.Disciplina;

public class EquivalenciasImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< EquivalenciasImportExcelBean >
{
	private String cursouStr;
	private String eliminaStr;

	private Disciplina disciplinaCursou;
	private Set< Disciplina > disciplinasElimina = new HashSet< Disciplina >();

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
			checkMandatoryField( cursouStr, ImportExcelError.EQUIVALENCIA_CURSOU_VAZIO, erros );
			checkMandatoryField( eliminaStr, ImportExcelError.EQUIVALENCIA_ELIMINA_VAZIO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}

	private boolean tudoVazio()
	{
		return ( isEmptyField( cursouStr )
			&& isEmptyField( eliminaStr ) );
	}

	public String getCursouStr() {
		return cursouStr;
	}

	public void setCursouStr(String cursouStr) {
		this.cursouStr = cursouStr;
	}

	public String getEliminaStr() {
		return eliminaStr;
	}

	public void setEliminaStr(String eliminaStr) {
		this.eliminaStr = eliminaStr;
	}

	public Disciplina getDisciplinaCursou() {
		return disciplinaCursou;
	}

	public void setDisciplinaCursou(Disciplina disciplinaCursou) {
		this.disciplinaCursou = disciplinaCursou;
	}

	public Set<Disciplina> getDisciplinasElimina() {
		return disciplinasElimina;
	}

	public void setDisciplinasElimina(Set<Disciplina> disciplinasElimina) {
		this.disciplinasElimina = disciplinasElimina;
	}

	@Override
	public int compareTo(EquivalenciasImportExcelBean o) {
		return getCursouStr().compareTo(o.getCursouStr());
	}
}
