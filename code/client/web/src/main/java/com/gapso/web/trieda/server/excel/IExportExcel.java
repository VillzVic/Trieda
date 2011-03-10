package com.gapso.web.trieda.server.excel;

import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;

public interface IExportExcel {
	
	public String getFileName();
	public HSSFWorkbook export();
	public boolean export(HSSFWorkbook workbook);
	
	public List<String> getErrors();
	public List<String> getWarnings();
}