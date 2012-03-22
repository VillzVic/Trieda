package com.gapso.web.trieda.server.util.progressReport;

import java.io.ByteArrayOutputStream;
import java.util.List;

public class ProgressReportListWriter extends ProgressReportWriter{
	private List<String> feedback;
	
	public ProgressReportListWriter(List<String> feedback){
		super(new ByteArrayOutputStream());
		this.feedback = feedback;
	}
	
	public void writeMsg(String msg){
		feedback.add(msg);
	}

}
