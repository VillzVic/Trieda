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
		String ret = text;
		if(ret.length() > max) {
			ret = ret.substring(0, max);
			ret += "...";
		}
		return ret;
	}
}