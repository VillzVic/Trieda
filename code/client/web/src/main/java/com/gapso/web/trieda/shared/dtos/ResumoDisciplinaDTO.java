package com.gapso.web.trieda.shared.dtos;

import com.gapso.web.trieda.shared.util.TriedaCurrency;
import com.gapso.web.trieda.shared.util.TriedaUtil;

public class ResumoDisciplinaDTO extends AbstractTreeDTO< String >
	implements Comparable< ResumoDisciplinaDTO >
{
	private static final long serialVersionUID = -5134820110949139907L;

	// Propriedades
	public static final String PROPERTY_DISCIPLINA_ID = "disciplinaId";
	public static final String PROPERTY_DISCIPLINA_STRING = "disciplinaString";
	public static final String PROPERTY_TURMA_STRING = "turmaString";
	public static final String PROPERTY_TIPO_CREDITO_TEORICO_BOOLEAN = "tipoCreditoString";
	public static final String PROPERTY_CREDITOS_INT = "creditosInt";
	public static final String PROPERTY_TOTAL_CREDITOS_INT = "totalCreditosInt";
	public static final String PROPERTY_QUANTIDADE_ALUNOS_INT = "quantidadeAlunosInt";
	public static final String PROPERTY_CUSTO_DOCENTE_DOUBLE = "custoDocenteDouble";
	public static final String PROPERTY_RECEITA_DOUBLE = "receitaDouble";
	public static final String PROPERTY_MARGEM_DOUBLE = "margemDouble";
	public static final String PROPERTY_MARGEM_PERCENTE_DOUBLE = "margemPercenteDouble";

	private boolean hasChildren = false;

	public ResumoDisciplinaDTO()
	{
		super();
	}

	public void setDisciplinaId( Long value )
	{
		set(PROPERTY_DISCIPLINA_ID, value);
	}

	public Long getDisciplinaId()
	{
		return get( PROPERTY_DISCIPLINA_ID );
	}

	public void setDisciplinaString( String value )
	{
		set( PROPERTY_DISCIPLINA_STRING, value );
	}

	public String getDisciplinaString()
	{
		return get( PROPERTY_DISCIPLINA_STRING );
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

	public void setReceita( TriedaCurrency value )
	{
		set( PROPERTY_RECEITA_DOUBLE, value.toString() );
	}

	public TriedaCurrency getReceita()
	{
		return TriedaUtil.parseTriedaCurrency(
			get( PROPERTY_RECEITA_DOUBLE ) );
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

	public void setMargemPercente( Double value )
	{
		set( PROPERTY_MARGEM_PERCENTE_DOUBLE, value );
	}

	public Double getMargemPercente()
	{
		return get( PROPERTY_MARGEM_PERCENTE_DOUBLE );
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
		return getDisciplinaId() +
			"-" + getTurma() +
			"-" + getTipoCreditoTeorico();
	}

	@Override
	public int compareTo( ResumoDisciplinaDTO o )
	{
		return getDisciplinaString().compareTo( o.getDisciplinaString() );
	}
}
