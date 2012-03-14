package com.gapso.web.trieda.server.util.progressReport;

import java.io.File;
import java.io.IOException;

public interface ProgressDeclaration {
	
	public void setProgressReport(File fw) throws IOException;
	public ProgressReportWriter getProgressReport();
}
