package com.gapso.web.trieda.server;

import java.io.File;
import java.util.HashMap;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

import com.gapso.web.trieda.server.util.progressReport.ProgressReportCreate;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportFileReader;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportReader;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportWriter;
import com.gapso.web.trieda.shared.services.ProgressReportService;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

public class ProgressReportServiceImpl extends RemoteServiceServlet  
	implements ProgressReportService
{
	
	private static final long serialVersionUID = -4580690361232014543L;
	
	@SuppressWarnings("unchecked")
	public static HashMap<String, ProgressReportReader> getProgressReportSession(HttpServletRequest request){
		HttpSession session = request.getSession();
		HashMap<String, ProgressReportReader> progressReportSession = (HashMap<String, ProgressReportReader>) session.getAttribute("progressReport");
		if(progressReportSession == null){
			progressReportSession = new HashMap<String, ProgressReportReader>();
			session.setAttribute("progressReport", progressReportSession);
		}
		
		return progressReportSession;
	}
	
	@SuppressWarnings("unchecked")
	public static HashMap<String, ProgressReportWriter> getProgressReportWriterSession(HttpServletRequest request){
		HttpSession session = request.getSession();
		HashMap<String, ProgressReportWriter> progressReportSession = (HashMap<String, ProgressReportWriter>) session.getAttribute("progressReportWriter");
		if(progressReportSession == null){
			progressReportSession = new HashMap<String, ProgressReportWriter>();
			session.setAttribute("progressReportWriter", progressReportSession);
		}
		
		return progressReportSession;
	}
	
	private ProgressReportReader getProgressReport(String reportKey) throws Exception{
		HashMap<String, ProgressReportReader> progressReport = getProgressReportSession(this.getThreadLocalRequest());
		
		ProgressReportReader progressSource = progressReport.get(reportKey);
		
		return progressSource;
	}
	
	public String getNewKey(){
		return ProgressReportCreate.getNewKey(getProgressReportSession(this.getThreadLocalRequest()));
	}
	
	public Boolean isReadyToRead(String reportKey) throws Exception{
		ProgressReportReader progressSource = getProgressReport(reportKey);
		
		return progressSource != null;
	}
	
	public Boolean isFinished(String reportKey) throws Exception{
		ProgressReportReader progressSource = getProgressReport(reportKey);
		
		return (progressSource == null) ? null : progressSource.isFinished();
	}
	
	@SuppressWarnings({ "unchecked" })
	public Boolean isClosed(String reportKey) throws Exception{
		HttpSession session = this.getThreadLocalRequest().getSession();
		ProgressReportReader progressSource = ((HashMap<String, ProgressReportReader>) session.getAttribute("progressReport")).remove(reportKey);
		
		if(progressSource instanceof ProgressReportFileReader){
			progressSource = null;
			File f = ProgressReportCreate.getFile(reportKey);
			if(f.exists()){
				if(!f.delete()) return false;
			}
			f = null;
		}
		else progressSource = null;
		
		return true;
	}

	public List<String> getProgressMessages(String reportKey) throws Exception{
		ProgressReportReader progressSource = getProgressReport(reportKey);
		
		return (progressSource == null) ? null : progressSource.getProgressMessages();
	}

}
