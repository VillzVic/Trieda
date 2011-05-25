package com.gapso.web.trieda.server.excel.imp;

import java.io.InputStream;
import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;

public interface IImportExcel {
	
	public boolean load(String fileName, InputStream inputStream);
	public boolean load(String fileName, HSSFWorkbook workbook);
	public String getSheetName();
	public List<String> getErrors();
	public List<String> getWarnings();
}