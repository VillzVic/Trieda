package com.gapso.web.trieda.server.excel.exp;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;

public class TRIEDAExportExcel extends AbstractExportExcel {
	
	public TRIEDAExportExcel() {}
	
	@Override
	public String getFileName() {
		return "Trieda";
	}
	
	@Override
	protected String getPathExcelTemplate() {
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return "Trieda";
	}

	@Override
	protected boolean fillInExcel(HSSFWorkbook workbook) {
		IExportExcel campiExporter = new CampiExportExcel(false);
		IExportExcel unidadesExporter = new UnidadesExportExcel(false);
		IExportExcel salasExporter = new SalasExportExcel(false);
		
		campiExporter.export(workbook);
		unidadesExporter.export(workbook);
		salasExporter.export(workbook);
		
		return true;
	}
}