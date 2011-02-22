package com.gapso.web.trieda.server.util;

public class TriedaUtil {

	static public double round(double value, int precision) {
		if (precision != 0) {
			double precisionFactor = precision * 10.0;
			value = Math.round(value * precisionFactor);
			return value / precisionFactor;
		}
		return value;
	}

}
