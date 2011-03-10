package com.gapso.web.trieda.server.excel;

import com.gapso.web.trieda.shared.excel.ExportedInformationType;

public class ExportExcelFactory {
	
	static public IExportExcel createExporter(String infoToBeExported) {
		IExportExcel exporter = null;
		
		ExportedInformationType informationToBeExported = ExportedInformationType.valueOf(infoToBeExported);
		switch (informationToBeExported) {
			case TUDO: exporter = new TRIEDAExportExcel(); break;
			case CAMPI: exporter = new CampiExportExcel(); break;
			case UNIDADES: exporter = new UnidadesExportExcel(); break;
			case SALAS: exporter = new SalasExportExcel(); break;
		}
		
		return exporter;
	}
}