package com.gapso.web.trieda.shared.dtos;

public class AtendimentoCargaHorariaDTO extends AbstractDTO< String >
	implements Comparable< AtendimentoCargaHorariaDTO >
{

	private static final long serialVersionUID = -2130477512164161492L;
	
	// Propriedades
	public static final String PROPERTY_ALUNO_MATRICULA = "alunoMatricula";
	public static final String PROPERTY_DIA1 = "dia1";
	public static final String PROPERTY_DIA2 = "dia2";
	public static final String PROPERTY_DIA3 = "dia3";
	public static final String PROPERTY_DIA4 = "dia4";
	public static final String PROPERTY_DIA5 = "dia5";
	public static final String PROPERTY_DIA6 = "dia6";
	public static final String PROPERTY_DIA7 = "dia7";
	
	public AtendimentoCargaHorariaDTO()
	{
		super();
	}

	public void setAlunoMatricula(String value)	{
		set( PROPERTY_ALUNO_MATRICULA, value );
	}

	public String getAlunoMatricula() {
		return get( PROPERTY_ALUNO_MATRICULA );
	}
	
	public void setDia1(Integer value)	{
		set( PROPERTY_DIA1, value );
	}

	public Integer getDia1() {
		return get( PROPERTY_DIA1 );
	}
	
	public void setDia2(Integer value)	{
		set( PROPERTY_DIA2, value );
	}

	public Integer getDia2() {
		return get( PROPERTY_DIA2 );
	}
	
	public void setDia3(Integer value)	{
		set( PROPERTY_DIA3, value );
	}

	public Integer getDia3() {
		return get( PROPERTY_DIA3 );
	}
	
	public void setDia4(Integer value)	{
		set( PROPERTY_DIA4, value );
	}

	public Integer getDia4() {
		return get( PROPERTY_DIA4 );
	}
	
	public void setDia5(Integer value)	{
		set( PROPERTY_DIA5, value );
	}

	public Integer getDia5() {
		return get( PROPERTY_DIA5 );
	}
	
	public void setDia6(Integer value)	{
		set( PROPERTY_DIA6, value );
	}

	public Integer getDia6() {
		return get( PROPERTY_DIA6 );
	}
	
	public void setDia7(Integer value)	{
		set( PROPERTY_DIA7, value );
	}

	public Integer getDia7() {
		return get( PROPERTY_DIA7 );
	}
	
	@Override
	public int compareTo(AtendimentoCargaHorariaDTO o) {
		return getAlunoMatricula().compareTo(o.getAlunoMatricula());
	}

	@Override
	public String getNaturalKey() {
		return getAlunoMatricula();
	}
	
}
