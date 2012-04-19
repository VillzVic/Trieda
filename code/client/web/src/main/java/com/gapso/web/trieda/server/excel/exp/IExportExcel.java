package com.gapso.web.trieda.server.excel.exp;

import java.util.List;
import java.util.Map;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;

public interface IExportExcel {
	public String getFileName();
	public HSSFWorkbook export();
	public boolean export(HSSFWorkbook workbook);
	public void resolveHyperlinks(Map<String,Map<String,Map<String,String>>> hyperlinksMap, HSSFWorkbook workbook);

	public Map<String,Map<String,Map<String,String>>> getHyperlinksMap();
	public List<String> getErrors();
	public List<String> getWarnings();
}
