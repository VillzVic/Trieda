package com.gapso.web.trieda.shared.dtos;

public class SalaUtilizadaDTO extends AbstractDTO< String >
	implements Comparable< SalaUtilizadaDTO >
{
	private static final long serialVersionUID = 2505634274930211439L;

	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_CODIGO = "codigo";
	public static final String PROPERTY_CAMPUS = "campus";
	public static final String PROPERTY_UNIDADE = "unidade";
	public static final String PROPERTY_NUMERO = "numero";
	public static final String PROPERTY_ANDAR = "andar";
	public static final String PROPERTY_CARGA_HORARIA = "cargaHoraria";
	public static final String PROPERTY_UTILZIACAO_HORARIO = "utilizacaoHorario";
	public static final String PROPERTY_OCUPACAO_CAPACIDADE = "ocupacaoCapacidade";

	public SalaUtilizadaDTO()
	{
		super();
	}
	
	public void setId(Long value) {
		set(PROPERTY_ID, value);
	}
	public Long getId() {
		return get(PROPERTY_ID);
	}
	
	public String getCodigo() {
		return get(PROPERTY_CODIGO);
	}
	public void setCodigo(String value) {
		set(PROPERTY_CODIGO, value);
	}
	
	public String getCampus() {
		return get(PROPERTY_CAMPUS);
	}
	public void setCampus(String value) {
		set(PROPERTY_CAMPUS, value);
	}

	public String getUnidade() {
		return get(PROPERTY_UNIDADE);
	}
	public void setUnidadeString(String value) {
		set(PROPERTY_UNIDADE, value);
	}
	
	public String getNumero() {
		return get(PROPERTY_NUMERO);
	}
	public void setNumero(String value) {
		set(PROPERTY_NUMERO, value);
	}
	
	public String getAndar() {
		return get(PROPERTY_ANDAR);
	}
	public void setAndar(String value) {
		set(PROPERTY_ANDAR, value);
	}
	
	public Integer getCargaHoraria() {
		return get(PROPERTY_CARGA_HORARIA);
	}
	public void setCargaHoraria(Integer value) {
		set(PROPERTY_CARGA_HORARIA, value);
	}
	
	public String getUtilizacaoHorario() {
		return get(PROPERTY_UTILZIACAO_HORARIO);
	}
	public void setUtilizacaoHorario(String value) {
		set(PROPERTY_UTILZIACAO_HORARIO, value);
	}
	
	public String getOcupacaoCapacidade() {
		return get(PROPERTY_OCUPACAO_CAPACIDADE);
	}
	public void setOcupacaoCapacidade(String value) {
		set(PROPERTY_OCUPACAO_CAPACIDADE, value);
	}

	@Override
	public int compareTo(SalaUtilizadaDTO o) {
		return getCodigo().compareTo( o.getCodigo() );
	}

	@Override
	public String getNaturalKey() {
		return ( getCampus() + "-" + getUnidade() + "-" + getId() );
	}
}
