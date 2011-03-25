package com.gapso.web.trieda.server.excel.exp;

import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class ExportExcelFactory {
	
	static public IExportExcel createExporter(String infoToBeExported, TriedaI18nMessages i18nMessages) {
		IExportExcel exporter = null;
		
		ExcelInformationType informationToBeExported = ExcelInformationType.valueOf(infoToBeExported);
		switch (informationToBeExported) {
			case TUDO: exporter = new TRIEDAExportExcel(i18nMessages); break;
			case CAMPI: exporter = new CampiExportExcel(i18nMessages); break;
			case UNIDADES: exporter = new UnidadesExportExcel(i18nMessages); break;
			case SALAS: exporter = new SalasExportExcel(i18nMessages); break;
		}
		
		return exporter;
	}
}