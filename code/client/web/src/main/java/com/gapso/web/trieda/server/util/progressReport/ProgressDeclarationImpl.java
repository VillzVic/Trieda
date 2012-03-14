package com.gapso.web.trieda.server.util.progressReport;

import java.io.File;
import java.io.IOException;

public class ProgressDeclarationImpl implements ProgressDeclaration {

	private ProgressReportWriter progressReport;
	
	public void setProgressReport(File f) throws IOException{
		progressReport = new ProgressReportFileWriter(f);
	}
	
	public ProgressReportWriter getProgressReport(){
		return progressReport;
	}
	
}
