package com.gapso.web.trieda.server.excel.exp;

import java.util.List;
import java.util.Map;

import org.apache.poi.ss.usermodel.Workbook;

public interface IExportExcel {
	public String getFileName();
	public Workbook export();
	public boolean export(Workbook workbook);
	public void resolveHyperlinks(Map<String,Map<String,Map<String,String>>> hyperlinksMap, Workbook workbook);

	public Map<String,Map<String,Map<String,String>>> getHyperlinksMap();
	public List<String> getErrors();
	public List<String> getWarnings();
}
