package com.gapso.web.trieda.server.excel.imp;

import java.util.List;

import com.gapso.web.trieda.shared.util.TriedaUtil;

public abstract class AbstractImportExcelBean {
	
	private int row;
	
	public AbstractImportExcelBean(int row) {
		this.row = row;
	}
	
	public int getRow() {
		return row;
	}
	
	protected boolean isEmptyField(String value) {
		return value == null || value.equals("");
	}
	
	protected void checkMandatoryField(String value, ImportExcelError errorType, List<ImportExcelError> errorsList) {
		if (isEmptyField(value)) {
			errorsList.add(errorType);
		}
	}
	
	protected Double checkNonNegativeDoubleField(String value, ImportExcelError doubleErrorType, ImportExcelError nonNegativeErrorType, List<ImportExcelError> errorsList) {
		Double doubleValue = null;
		
		if (!isEmptyField(value)) {
			try {
				value = TriedaUtil.financialFormatToDoubleFormat(value);
				doubleValue = Double.valueOf(value);
				if (doubleValue < 0.0) {
					errorsList.add(nonNegativeErrorType);
				}
			} catch (NumberFormatException e) {
				errorsList.add(doubleErrorType);
			}
		}
		
		return doubleValue;
	} 
}