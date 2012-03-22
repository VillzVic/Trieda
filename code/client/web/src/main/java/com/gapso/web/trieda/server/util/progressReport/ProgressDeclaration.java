package com.gapso.web.trieda.server.util.progressReport;

import java.io.File;
import java.io.IOException;
import java.util.List;

public interface ProgressDeclaration {
	
	public void setProgressReport(List<String> fbl);
	public void setProgressReport(File fw) throws IOException;
	public ProgressReportWriter getProgressReport();
}
