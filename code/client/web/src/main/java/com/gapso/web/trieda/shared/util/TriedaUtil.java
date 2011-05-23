package com.gapso.web.trieda.shared.util;

import java.util.List;

import com.google.gwt.user.client.Window.Location;


public class TriedaUtil {

	static public double round(double value, int precision) {
		if (precision != 0) {
			double precisionFactor = precision * 10.0;
			value = Math.round(value * precisionFactor);
			return value / precisionFactor;
		}
		return value;
	}

	static public String truncate(String text, int max) {
		return truncate(text, max, true);
	}
	static public String truncate(String text, int max, boolean etc) {
		if(max <= 0) return text;
		String ret = text;
		if(ret.length() > max) {
			ret = ret.substring(0, max);
			if(etc) ret += "...";
		}
		return ret;
	}
	
	static public String financialFormatToDoubleFormat(String value) {
		boolean contemVirgula = value.contains(",");
		boolean contemPonto = value.contains(".");
		if (contemVirgula && contemPonto) {
			value = value.replace(".","");
		}
		if (contemVirgula) {
			value = value.replace(",",".");
		}
		return value;
	}
	
	static public boolean isBlank(String value) {
		return value == null || value == "";
	}
	static public boolean isBlank(Long value) {
		return value == null || value == 0;
	}
	
	static public String paramsDebug() {
		String codesvr = Location.getParameter("gwt.codesvr");
		if(isBlank(codesvr)) return "";
		return "?gwt.codesvr="+codesvr;
	}
	
	static public String arrayJoin(List<String> l, String s) {
		String ret = "";
		if(l.size() == 0) return ret;
		ret = l.get(0);
		for(int i = 1; i < l.size(); i++) {
			ret = ret + s + l.get(i);
		}
		return ret;
	}
	
	static public Double roundTwoDecimals(double d) {
	    long y=(long)(d*100);
	    return (double)y/100;
	}
	
}
