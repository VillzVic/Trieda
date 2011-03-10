package com.gapso.web.trieda.server.excel;

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
		CampiExportExcel campiExporter = new CampiExportExcel(false);
		UnidadesExportExcel unidadesExporter = new UnidadesExportExcel(false);
		SalasExportExcel salasExporter = new SalasExportExcel(false);
		
		campiExporter.fillInExcel(workbook);
		unidadesExporter.fillInExcel(workbook);
		salasExporter.fillInExcel(workbook);
		
		return true;
	}
}