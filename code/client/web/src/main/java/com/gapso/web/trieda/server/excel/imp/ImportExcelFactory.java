package com.gapso.web.trieda.server.excel.imp;

import com.gapso.web.trieda.shared.excel.ExcelInformationType;

public class ImportExcelFactory {
	
	static public IImportExcel createImporter(String infoToBeImported) {
		IImportExcel importer = null;
		
		ExcelInformationType informationToBeImported = ExcelInformationType.valueOf(infoToBeImported);
		switch (informationToBeImported) {
			//case TUDO: importer = new TRIEDAImportExcel(); break;
			case CAMPI: importer = new CampiImportExcel(); break;
			//case UNIDADES: importer = new UnidadesImportExcel(); break;
			//case SALAS: importer = new SalasImportExcel(); break;
		}
		
		return importer;
	}
}