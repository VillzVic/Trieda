package com.gapso.web.trieda.shared.dtos;

import java.util.List;

public class ErrorsWarningsInputSolverDTO extends AbstractDTO<String> {
	private static final long serialVersionUID = -5134820110949139907L;

	public static final String PROPERTY_ERRORS = "errors";
	public static final String PROPERTY_WARNINGS = "warnings";

	public ErrorsWarningsInputSolverDTO() {}

	public void setErrors(List<String> value) {
		set(PROPERTY_ERRORS,value);
	}
	
	public void setWarnings(List<String> value) {
		set(PROPERTY_WARNINGS,value);
	}

	public List<String> getErrors() {
		return get(PROPERTY_ERRORS);
	}
	
	public List<String> getWarnings() {
		return get(PROPERTY_WARNINGS);
	}

	public Integer getTotalErrorsWarnings() {
		int totalErrors = (this.getErrors() == null) ? 0 : this.getErrors().size();
		int totalWarnings = (this.getWarnings() == null) ? 0 : this.getWarnings().size();
		
		return totalErrors + totalWarnings; 
	}

	@Override
	public String getNaturalKey() {
		return "";
	}
}