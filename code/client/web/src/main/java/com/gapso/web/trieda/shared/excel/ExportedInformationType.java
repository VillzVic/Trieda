package com.gapso.web.trieda.shared.excel;

public enum ExportedInformationType {
	TUDO,
	CAMPI,
	UNIDADES,
	SALAS;
	
	public static String getParameterName() {
		return "exportedInformationType";
	}
}