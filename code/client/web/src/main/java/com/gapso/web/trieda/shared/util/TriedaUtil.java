package com.gapso.web.trieda.shared.util;


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
	
}
