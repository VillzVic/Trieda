package com.gapso.web.trieda.shared.excel;

public enum ExcelInformationType {
	TUDO(""),
	CAMPI("Campi"),
	CURRICULOS("Curriculos"),
	CURSOS("Cursos"),
	AREAS_TITULACAO("Areas de Titulacao"),
	CURSO_AREAS_TITULACAO("Curso - Area de Titulacao"),
	DEMANDAS("Ofertas e Demandas"),
	DISCIPLINAS("Disciplinas"),
	EQUIVALENCIAS("Equivalencias"),
	DISCIPLINAS_SALAS("Disciplinas-Salas"),
	PALETA_CORES("PaletaCores"),
	PROFESSORES("Professores"),
	CAMPI_TRABALHO("Campi de Trabalho"),
	RELATORIO_VISAO_SALA("Relatorio Visao Sala"),
	RELATORIO_VISAO_CURSO("Relatorio Visao Curso"),
	SALAS("Salas"),
	UNIDADES("Unidades"),
	RESUMO_CURSO("Resumo por curso"),
	RESUMO_DISCIPLINA("Resumo por disciplina"),
	HABILITACAO_PROFESSORES("Habilitacao dos Professores");
	
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