package com.gapso.web.trieda.server.excel.imp;

import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;


public enum ImportExcelError {
	
	CAMPUS_CODIGO_VAZIO,
	CAMPUS_CUSTO_MEDIO_CREDITO_FORMATO_INVALIDO,
	CAMPUS_CUSTO_MEDIO_CREDITO_VALOR_NEGATIVO,
	CAMPUS_CUSTO_MEDIO_CREDITO_VAZIO,
	CAMPUS_ESTADO_VALOR_INVALIDO,
	CAMPUS_NOME_VAZIO,
	
	SALA_ANDAR_VAZIO,
	SALA_CAPACIDADE_FORMATO_INVALIDO,
	SALA_CAPACIDADE_VALOR_NEGATIVO,
	SALA_CAPACIDADE_VAZIO,
	SALA_CODIGO_VAZIO,
	SALA_NUMERO_VAZIO,
	SALA_TIPO_VAZIO,
	SALA_UNIDADE_VAZIO,
	
	TUDO_VAZIO,
	
	UNIDADE_CAMPUS_VAZIO,
	UNIDADE_CODIGO_VAZIO,
	UNIDADE_NOME_VAZIO;
	
	public String getMessage(String param1, TriedaI18nMessages i18nMessages) {
		
		switch (this) {
			case CAMPUS_CODIGO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CampiImportExcel.CODIGO_COLUMN_NAME);
			case CAMPUS_CUSTO_MEDIO_CREDITO_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,CampiImportExcel.CUSTO_CREDITO_COLUMN_NAME);
			case CAMPUS_CUSTO_MEDIO_CREDITO_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,CampiImportExcel.CUSTO_CREDITO_COLUMN_NAME);
			case CAMPUS_CUSTO_MEDIO_CREDITO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CampiImportExcel.CUSTO_CREDITO_COLUMN_NAME);
			case CAMPUS_ESTADO_VALOR_INVALIDO: return i18nMessages.excelErroSintaticoValorInvalido(param1,CampiImportExcel.ESTADO_COLUMN_NAME);
			case CAMPUS_NOME_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CampiImportExcel.NOME_COLUMN_NAME);

			case SALA_ANDAR_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,SalasImportExcel.ANDAR_COLUMN_NAME);
			case SALA_CAPACIDADE_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,SalasImportExcel.CAPACIDADE_COLUMN_NAME);
			case SALA_CAPACIDADE_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,SalasImportExcel.CAPACIDADE_COLUMN_NAME);
			case SALA_CAPACIDADE_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,SalasImportExcel.CAPACIDADE_COLUMN_NAME);
			case SALA_CODIGO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,SalasImportExcel.CODIGO_COLUMN_NAME);
			case SALA_NUMERO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,SalasImportExcel.NUMERO_COLUMN_NAME);
			case SALA_TIPO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,SalasImportExcel.TIPO_COLUMN_NAME);
			case SALA_UNIDADE_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,SalasImportExcel.UNIDADE_COLUMN_NAME);
			
			case TUDO_VAZIO: return i18nMessages.excelErroSintaticoLinhaVazia(param1);
			
			case UNIDADE_CAMPUS_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,UnidadesImportExcel.CAMPUS_COLUMN_NAME);
			case UNIDADE_CODIGO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,UnidadesImportExcel.CODIGO_COLUMN_NAME);
			case UNIDADE_NOME_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,UnidadesImportExcel.NOME_COLUMN_NAME);
		}
		return "";
	}
}