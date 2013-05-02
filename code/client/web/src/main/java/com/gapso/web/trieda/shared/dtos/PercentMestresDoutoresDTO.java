package com.gapso.web.trieda.shared.dtos;

public class PercentMestresDoutoresDTO extends AbstractDTO< String >
	implements Comparable< PercentMestresDoutoresDTO >
{

	private static final long serialVersionUID = -34200463960347258L;
	
	public static final String PROPERTY_CAMPUS_STRING = "campusString";
	public static final String PROPERTY_CURSO_STRING = "cursoString";
	public static final String PROPERTY_DOUTORES = "doutores";
	public static final String PROPERTY_MESTRES = "mestres";
	public static final String PROPERTY_OUTROS = "outros";
	public static final String PROPERTY_TOTAL = "total";
	public static final String PROPERTY_DOUTORES_PERCENT = "doutoresPercent";
	public static final String PROPERTY_MESTRES_DOUTORES_PERCENT = "mestresDoutoresPercent";
	public static final String PROPERTY_DOUTORES_MIN = "doutoresMin";
	public static final String PROPERTY_MESTRES_DOUTORES_MIN = "mestresDoutoresMin";
	
	public void setCampusString(String value) {
		set(PROPERTY_CAMPUS_STRING, value);
	}

	public String getCampusString() {
		return get(PROPERTY_CAMPUS_STRING);
	}
	
	public void setCursoString(String value) {
		set(PROPERTY_CURSO_STRING, value);
	}

	public String getCursoString() {
		return get(PROPERTY_CURSO_STRING);
	}
	
	public Integer getDoutores() {
		return get(PROPERTY_DOUTORES);
	}
	
	public void setDoutores(Integer value) {
		set(PROPERTY_DOUTORES, value);
	}
	
	public Integer getMestres() {
		return get(PROPERTY_MESTRES);
	}
	
	public void setMestres(Integer value) {
		set(PROPERTY_MESTRES, value);
	}
	
	public Integer getOutros() {
		return get(PROPERTY_DOUTORES);
	}
	
	public void setOutros(Integer value) {
		set(PROPERTY_OUTROS, value);
	}
	
	public Integer getTotal() {
		return get(PROPERTY_TOTAL);
	}
	
	public void setTotal(Integer value) {
		set(PROPERTY_TOTAL, value);
	}
	
	public String getDoutoresPercent() {
		return get(PROPERTY_DOUTORES_PERCENT);
	}
	
	public void setDoutoresPercent(String value) {
		set(PROPERTY_DOUTORES_PERCENT, value);
	}
	
	public String getMestresDoutoresPercent() {
		return get(PROPERTY_MESTRES_DOUTORES_PERCENT);
	}
	
	public void setMestresDoutoresPercent(String value) {
		set(PROPERTY_MESTRES_DOUTORES_PERCENT, value);
	}
	
	public String getDoutoresMin() {
		return get(PROPERTY_DOUTORES_MIN);
	}
	
	public void setDoutoresMin(String value) {
		set(PROPERTY_DOUTORES_MIN, value);
	}
	
	public String getMestresDoutoresMin() {
		return get(PROPERTY_MESTRES_DOUTORES_MIN);
	}
	
	public void setMestresDoutoresMin(String value) {
		set(PROPERTY_MESTRES_DOUTORES_MIN, value);
	}

	@Override
	public int compareTo(PercentMestresDoutoresDTO o) {
		int result = getCursoString().compareTo(o.getCursoString());
		if (result == 0) {
			result =  getCampusString().compareTo(o.getCampusString());
		}
		
		return result;
	}

	@Override
	public String getNaturalKey() {
		return getCursoString() + "-" + getCampusString();
	}
	
}
