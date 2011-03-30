package com.gapso.web.trieda.server.excel.exp;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class TRIEDAExportExcel extends AbstractExportExcel {
	
	public TRIEDAExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		super(cenario,i18nConstants,i18nMessages);
	}
	
	@Override
	public String getFileName() {
		return getI18nConstants().trieda();
	}
	
	@Override
	protected String getPathExcelTemplate() {
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return getI18nConstants().trieda();
	}

	@Override
	protected boolean fillInExcel(HSSFWorkbook workbook) {
		IExportExcel campiExporter = new CampiExportExcel(false,getCenario(),getI18nConstants(),getI18nMessages());
		IExportExcel unidadesExporter = new UnidadesExportExcel(false,getCenario(),getI18nConstants(),getI18nMessages());
		IExportExcel salasExporter = new SalasExportExcel(false,getCenario(),getI18nConstants(),getI18nMessages());
		IExportExcel cursosExporter = new CursosExportExcel(false,getCenario(),getI18nConstants(),getI18nMessages());
		IExportExcel disciplinasExporter = new DisciplinasExportExcel(false,getCenario(),getI18nConstants(),getI18nMessages());
		
		campiExporter.export(workbook);
		unidadesExporter.export(workbook);
		salasExporter.export(workbook);
		cursosExporter.export(workbook);
		disciplinasExporter.export(workbook);
		
		return true;
	}
}