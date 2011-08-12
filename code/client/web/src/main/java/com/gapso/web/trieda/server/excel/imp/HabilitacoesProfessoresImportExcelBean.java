package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Professor;

public class HabilitacoesProfessoresImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< HabilitacoesProfessoresImportExcelBean >
{
	private String codigoCampusStr;
	private String cpfProfessorStr;
	private String codigoDisciplinaStr;
	private String preferenciaStr;
	private String notaStr;

	private Campus campus;
	private Disciplina disciplina;
	private Professor professor;
	private Integer preferencia;
	private Integer nota;

	public HabilitacoesProfessoresImportExcelBean( int row )
	{
		super( row );
	}

	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros = new ArrayList< ImportExcelError >();

		if ( !tudoVazio() )
		{
			checkMandatoryField( codigoCampusStr, ImportExcelError.HABILITACOESPROFESSORES_CODIGO_CAMPUS_VAZIO, erros );
			checkMandatoryField( cpfProfessorStr, ImportExcelError.HABILITACOESPROFESSORES_CPF_PROFESSOR_VAZIO, erros );
			checkMandatoryField( codigoDisciplinaStr, ImportExcelError.HABILITACOESPROFESSORES_DISCIPLINA_VAZIO, erros );
			checkMandatoryField( preferenciaStr, ImportExcelError.HABILITACOESPROFESSORES_PREFERENCIA_VAZIO, erros );
			checkMandatoryField( notaStr, ImportExcelError.HABILITACOESPROFESSORES_NOTA_VAZIO, erros );

			preferencia = checkNonNegativeIntegerField( preferenciaStr,
				ImportExcelError.HABILITACOESPROFESSORES_PREFERENCIA_FORMATO_INVALIDO,
				ImportExcelError.HABILITACOESPROFESSORES_PREFERENCIA_VALOR_NEGATIVO, erros );

			nota = checkNonNegativeIntegerField( notaStr,
				ImportExcelError.HABILITACOESPROFESSORES_NOTA_FORMATO_INVALIDO,
				ImportExcelError.HABILITACOESPROFESSORES_NOTA_VALOR_NEGATIVO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}

	private boolean tudoVazio()
	{
		return isEmptyField( codigoCampusStr ) && isEmptyField( cpfProfessorStr )
			&& isEmptyField( codigoDisciplinaStr ) && isEmptyField( preferenciaStr )
			&& isEmptyField( notaStr );
	}

	public String getCodigoCampusStr() {
		return codigoCampusStr;
	}

	public void setCodigoCampusStr(String codigoCampusStr) {
		this.codigoCampusStr = codigoCampusStr;
	}

	public String getCpfProfessorStr() {
		return cpfProfessorStr;
	}

	public void setCpfProfessorStr(String cpfProfessorStr) {
		this.cpfProfessorStr = cpfProfessorStr;
	}

	public String getCodigoDisciplinaStr() {
		return codigoDisciplinaStr;
	}

	public void setCodigoDisciplinaStr(String codigoDisciplinaStr) {
		this.codigoDisciplinaStr = codigoDisciplinaStr;
	}

	public Campus getCampus() {
		return campus;
	}

	public void setCampus(Campus campus) {
		this.campus = campus;
	}

	public String getPreferenciaStr() {
		return preferenciaStr;
	}

	public void setPreferenciaStr(String preferenciaStr) {
		this.preferenciaStr = preferenciaStr;
	}

	public String getNotaStr() {
		return notaStr;
	}

	public void setNotaStr(String notaStr) {
		this.notaStr = notaStr;
	}
	
	public Disciplina getDisciplina() {
		return disciplina;
	}

	public void setDisciplina(Disciplina disciplina) {
		this.disciplina = disciplina;
	}

	public Integer getPreferencia() {
		return preferencia;
	}

	public void setPreferencia(Integer preferencia) {
		this.preferencia = preferencia;
	}

	public Integer getNota() {
		return nota;
	}

	public void setNota(Integer nota) {
		this.nota = nota;
	}

	public Professor getProfessor() {
		return professor;
	}

	public void setProfessor(Professor professor) {
		this.professor = professor;
	}

	@Override
	public int compareTo( HabilitacoesProfessoresImportExcelBean o )
	{
		int result = getCodigoDisciplinaStr().compareTo( o.getCodigoDisciplinaStr() );
		if ( result == 0 )
		{
			result = getCpfProfessorStr().compareTo( o.getCpfProfessorStr() );
		}

		return result;
	}

	public String getNaturalKeyString()
	{
		return this.getCodigoCampusStr()
			+ "-" + this.getCpfProfessorStr()
			+ "-" + this.getCodigoDisciplinaStr();
	}
}
