package com.gapso.web.trieda.server.excel.imp;

import java.io.InputStream;
import java.util.List;

public interface IImportExcel {
	
	public boolean load(String fileName, InputStream inputStream);
	
	public List<String> getErrors();
	public List<String> getWarnings();
}