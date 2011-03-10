package com.gapso.web.trieda.server.excel;

import com.gapso.web.trieda.shared.excel.ExportedInformationType;

public class ExportExcelFactory {
	
	static public IExportExcel createExporter(String infoToBeExported) {
		IExportExcel exporter = null;
		
		ExportedInformationType informationToBeExported = ExportedInformationType.valueOf(infoToBeExported);
		switch (informationToBeExported) {
			case UNIDADES: exporter = new UnidadesExportExcel(); break;
		}
		
		return exporter;
	}
}