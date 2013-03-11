package com.gapso.web.trieda.shared.dtos;

import com.gapso.web.trieda.shared.util.TriedaCurrency;
import com.gapso.web.trieda.shared.util.TriedaUtil;

public class ResumoDisciplinaDTO extends AbstractTreeDTO< String >
	implements Comparable< ResumoDisciplinaDTO >
{
	private static final long serialVersionUID = -5134820110949139907L;

	// Propriedades
	public static final String PROPERTY_CAMPUS_ID = "campusId";
	public static final String PROPERTY_CAMPUS_STRING = "campusString";
	public static final String PROPERTY_DISCIPLINA_ID_DA_AULA = "disciplinaIdDaAula";
	public static final String PROPERTY_DISCIPLINA_STRING_DA_AULA = "disciplinaStringDaAula";
	public static final String PROPERTY_DISCIPLINA_ID_DEMANDADA = "disciplinaIdDemandada";
	public static final String PROPERTY_DISCIPLINA_STRING_DEMANDADA = "disciplinaStringDemandada";
	public static final String PROPERTY_TURMA_STRING = "turmaString";
	public static final String PROPERTY_TIPO_CREDITO_TEORICO_BOOLEAN = "tipoCreditoString";
	public static final String PROPERTY_CREDITOS_INT = "creditosInt";
	public static final String PROPERTY_TOTAL_CREDITOS_INT = "totalCreditosInt";
	public static final String PROPERTY_QUANTIDADE_ALUNOS_INT = "quantidadeAlunosInt";
	public static final String PROPERTY_CUSTO_DOCENTE_DOUBLE = "custoDocenteDouble";
	public static final String PROPERTY_CUSTO_DOCENTE_STRING = "custoDocenteString";
	public static final String PROPERTY_RECEITA_DOUBLE = "receitaDouble";
	public static final String PROPERTY_RECEITA_STRING = "receitaString";
	public static final String PROPERTY_MARGEM_DOUBLE = "margemDouble";
	public static final String PROPERTY_MARGEM_STRING = "margemString";
	public static final String PROPERTY_MARGEM_PERCENTE_DOUBLE = "margemPercenteDouble";
	public static final String PROPERTY_MARGEM_PERCENTE_STRING = "margemPercenteString";
	public static final String PROPERTY_CURSO_ID = "CursoId";
	public static final String PROPERTY_CURRICULO_ID = "curriculoId";
	public static final String PROPERTY_DIA_SEMANA_ID = "diaSemanaId";
	public static final String PROPERTY_HORARIO_ID = "horarioId";

	private boolean hasChildren = false;

	public ResumoDisciplinaDTO()
	{
		super();
	}

	public Long getCursoId()
	{
		return get( PROPERTY_CURSO_ID );
	}
	
	public void setCursoId( Long value )
	{
		set(PROPERTY_CURSO_ID, value);
	}
	
	public Long getCurriculoId()
	{
		return get( PROPERTY_CURRICULO_ID );
	}
	
	public void setCurriculoId( Long value )
	{
		set(PROPERTY_CURRICULO_ID, value);
	}

	public void setDisciplinaIdDaAula( Long value )
	{
		set(PROPERTY_DISCIPLINA_ID_DA_AULA, value);
	}
	
	public Long getDisciplinaIdDaAula()
	{
		return get( PROPERTY_DISCIPLINA_ID_DA_AULA );
	}

	public void setDisciplinaStringDaAula( String value )
	{
		set( PROPERTY_DISCIPLINA_STRING_DA_AULA, value );
	}

	public String getDisciplinaStringDaAula()
	{
		return get( PROPERTY_DISCIPLINA_STRING_DA_AULA );
	}
	
	public void setDisciplinaIdDemandada( Long value )
	{
		set(PROPERTY_DISCIPLINA_ID_DEMANDADA, value);
	}
	
	public Long getDisciplinaIdDemandada()
	{
		return get( PROPERTY_DISCIPLINA_ID_DEMANDADA );
	}

	public void setDisciplinaStringDemandada( String value )
	{
		set( PROPERTY_DISCIPLINA_STRING_DEMANDADA, value );
	}

	public String getDisciplinaStringDemandada()
	{
		return get( PROPERTY_DISCIPLINA_STRING_DEMANDADA );
	}
	
	public void setCampusId( Long value )
	{
		set(PROPERTY_CAMPUS_ID, value);
	}
	
	public Long getCampusId()
	{
		return get( PROPERTY_CAMPUS_ID );
	}

	public void setCampusString( String value )
	{
		set( PROPERTY_CAMPUS_STRING, value );
	}

	public String getCampusString()
	{
		return get( PROPERTY_CAMPUS_STRING );
	}

	public void setTurma( String value )
	{
		set( PROPERTY_TURMA_STRING, value );
	}

	public String getTurma()
	{
		return get( PROPERTY_TURMA_STRING );
	}

	public void setTipoCreditoTeorico( Boolean value )
	{
		set( PROPERTY_TIPO_CREDITO_TEORICO_BOOLEAN, value );
	}

	public Boolean getTipoCreditoTeorico()
	{
		return get( PROPERTY_TIPO_CREDITO_TEORICO_BOOLEAN );
	}

	public void setCreditos( Integer value )
	{
		set( PROPERTY_CREDITOS_INT, value );
	}

	public Integer getCreditos()
	{
		return get( PROPERTY_CREDITOS_INT );
	}

	public void setTotalCreditos( Integer value )
	{
		set( PROPERTY_TOTAL_CREDITOS_INT, value );
	}

	public Integer getTotalCreditos(){
		return get( PROPERTY_TOTAL_CREDITOS_INT );
	}

	public void setQuantidadeAlunos( Integer value )
	{
		set( PROPERTY_QUANTIDADE_ALUNOS_INT, value );
	}

	public Integer getQuantidadeAlunos()
	{
		return get( PROPERTY_QUANTIDADE_ALUNOS_INT );
	}

	public void setCustoDocente( TriedaCurrency value )
	{
		set( PROPERTY_CUSTO_DOCENTE_DOUBLE, value.toString() );
	}

	public TriedaCurrency getCustoDocente()
	{
		return TriedaUtil.parseTriedaCurrency(
			get( PROPERTY_CUSTO_DOCENTE_DOUBLE ) );
	}
	
	public void setCustoDocenteString(String value) {
		set(PROPERTY_CUSTO_DOCENTE_STRING, value);
	}

	public String getCustoDocenteString() {
		return get(PROPERTY_CUSTO_DOCENTE_STRING);
	}

	public void setReceita( TriedaCurrency value )
	{
		set( PROPERTY_RECEITA_DOUBLE, value.toString() );
	}

	public TriedaCurrency getReceita()
	{
		return TriedaUtil.parseTriedaCurrency(
			get( PROPERTY_RECEITA_DOUBLE ) );
	}
	
	public void setReceitaString(String value) {
		set(PROPERTY_RECEITA_STRING, value);
	}

	public String getReceitaString() {
		return get(PROPERTY_RECEITA_STRING);
	}

	public void setMargem( TriedaCurrency value )
	{ 
		set( PROPERTY_MARGEM_DOUBLE, value.toString() );
	}

	public TriedaCurrency getMargem()
	{
		return TriedaUtil.parseTriedaCurrency(
			get( PROPERTY_MARGEM_DOUBLE ) );
	}
	
	public void setMargemString(String value) {
		set(PROPERTY_MARGEM_STRING, value);
	}

	public String getMargemString() {
		return get(PROPERTY_MARGEM_STRING);
	}

	public void setMargemPercente( Double value )
	{
		set( PROPERTY_MARGEM_PERCENTE_DOUBLE, value );
	}

	public Double getMargemPercente()
	{
		return get( PROPERTY_MARGEM_PERCENTE_DOUBLE );
	}
	
	public void setMargemPercenteString(String value) {
		set(PROPERTY_MARGEM_PERCENTE_STRING, value);
	}

	public String getMargemPercenteString() {
		return get(PROPERTY_MARGEM_PERCENTE_STRING);
	}
	
	public void setDiaSemanaId(Integer value) {
		set(PROPERTY_DIA_SEMANA_ID, value);
	}

	public Integer getDiaSemanaId() {
		return get(PROPERTY_DIA_SEMANA_ID);
	}
	
	public void setHorarioId(Long value) {
		set(PROPERTY_HORARIO_ID, value);
	}

	public Long getHorarioId() {
		return get(PROPERTY_HORARIO_ID);
	}

	public boolean hasChildren()
	{
		return hasChildren;
	}

	public void hasChildren( boolean hasChildren )
	{
		this.hasChildren = hasChildren;
	}

	@Override
	public String getNaturalKey()
	{
		return getDisciplinaIdDaAula() +
			"-" + getTurma() +
			"-" + getTipoCreditoTeorico();
	}

	@Override
	public int compareTo( ResumoDisciplinaDTO o )
	{
		return getDisciplinaStringDaAula().compareTo( o.getDisciplinaStringDaAula() );
	}
}
