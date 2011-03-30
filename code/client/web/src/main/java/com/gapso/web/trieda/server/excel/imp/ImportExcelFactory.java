package com.gapso.web.trieda.server.excel.imp;

import com.gapso.trieda.domain.Cenario;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class ImportExcelFactory {
	
	static public IImportExcel createImporter(String infoToBeImported, Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		IImportExcel importer = null;
		
		ExcelInformationType informationToBeImported = ExcelInformationType.valueOf(infoToBeImported);
		switch (informationToBeImported) {
			//case TUDO: importer = new TRIEDAImportExcel(); break;
			case CAMPI: importer = new CampiImportExcel(cenario,i18nConstants,i18nMessages); break;
			case CURSOS: importer = new CursosImportExcel(cenario,i18nConstants,i18nMessages); break;
			case DISCIPLINAS: importer = new DisciplinasImportExcel(cenario,i18nConstants,i18nMessages); break;
			case UNIDADES: importer = new UnidadesImportExcel(cenario,i18nConstants,i18nMessages); break;
			case SALAS: importer = new SalasImportExcel(cenario,i18nConstants,i18nMessages); break;
		}
		
		return importer;
	}
}