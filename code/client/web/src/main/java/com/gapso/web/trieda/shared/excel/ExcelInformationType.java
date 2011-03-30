package com.gapso.web.trieda.shared.excel;

public enum ExcelInformationType {
	TUDO(""),
	CAMPI("Campi"),
	CURSOS("Cursos"),
	DISCIPLINAS("Disciplinas"),
	UNIDADES("Unidades"),
	SALAS("Salas");
	
	private String sheetName;
	
	ExcelInformationType(String sheetName) {
		this.sheetName = sheetName;
	}
	
	public String getSheetName() {
		return sheetName;
	}
	
	public static String getInformationParameterName() {
		return "excelInformationType";
	}
	
	public static String getFileParameterName() {
		return "uploadedFile";
	}
	
	public static String prefixError() {
		return "@e@";
	}
	
	public static String prefixWarning() {
		return "@w@";
	}
}