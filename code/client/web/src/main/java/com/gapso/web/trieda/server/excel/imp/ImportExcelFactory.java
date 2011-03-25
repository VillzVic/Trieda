package com.gapso.web.trieda.server.excel.imp;

import com.gapso.trieda.domain.Cenario;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class ImportExcelFactory {
	
	static public IImportExcel createImporter(String infoToBeImported, Cenario cenario, TriedaI18nMessages i18nMessages) {
		IImportExcel importer = null;
		
		ExcelInformationType informationToBeImported = ExcelInformationType.valueOf(infoToBeImported);
		switch (informationToBeImported) {
			//case TUDO: importer = new TRIEDAImportExcel(); break;
			case CAMPI: importer = new CampiImportExcel(cenario,i18nMessages); break;
			//case UNIDADES: importer = new UnidadesImportExcel(); break;
			//case SALAS: importer = new SalasImportExcel(); break;
		}
		
		return importer;
	}
}