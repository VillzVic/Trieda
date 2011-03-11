package com.gapso.web.trieda.server.excel.exp;

import com.gapso.web.trieda.shared.excel.ExcelInformationType;

public class ExportExcelFactory {
	
	static public IExportExcel createExporter(String infoToBeExported) {
		IExportExcel exporter = null;
		
		ExcelInformationType informationToBeExported = ExcelInformationType.valueOf(infoToBeExported);
		switch (informationToBeExported) {
			case TUDO: exporter = new TRIEDAExportExcel(); break;
			case CAMPI: exporter = new CampiExportExcel(); break;
			case UNIDADES: exporter = new UnidadesExportExcel(); break;
			case SALAS: exporter = new SalasExportExcel(); break;
		}
		
		return exporter;
	}
}