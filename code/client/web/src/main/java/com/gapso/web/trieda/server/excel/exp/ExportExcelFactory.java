package com.gapso.web.trieda.server.excel.exp;

import com.gapso.trieda.domain.Cenario;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class ExportExcelFactory {
	
	static public IExportExcel createExporter(String infoToBeExported, Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		IExportExcel exporter = null;
		
		ExcelInformationType informationToBeExported = ExcelInformationType.valueOf(infoToBeExported);
		switch (informationToBeExported) {
			case TUDO: exporter = new TRIEDAExportExcel(cenario,i18nConstants,i18nMessages); break;
			case CAMPI: exporter = new CampiExportExcel(cenario,i18nConstants,i18nMessages); break;
			case CURRICULOS: exporter = new CurriculosExportExcel(cenario,i18nConstants,i18nMessages); break;
			case CURSOS: exporter = new CursosExportExcel(cenario,i18nConstants,i18nMessages); break;
			case DEMANDAS: exporter = new DemandasExportExcel(cenario,i18nConstants,i18nMessages); break;
			case DISCIPLINAS: exporter = new DisciplinasExportExcel(cenario,i18nConstants,i18nMessages); break;
			case UNIDADES: exporter = new UnidadesExportExcel(cenario,i18nConstants,i18nMessages); break;
			case SALAS: exporter = new SalasExportExcel(cenario,i18nConstants,i18nMessages); break;
		}
		
		return exporter;
	}
}