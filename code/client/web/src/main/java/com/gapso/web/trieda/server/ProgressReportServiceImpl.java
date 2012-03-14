package com.gapso.web.trieda.server;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.gapso.web.trieda.server.util.progressReport.ProgressReportFileCreate;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportFileReader;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportReader;
import com.gapso.web.trieda.shared.services.ProgressReportService;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

public class ProgressReportServiceImpl extends RemoteServiceServlet  
	implements ProgressReportService
{
	
	private static final long serialVersionUID = -4580690361232014543L;
	private static Map<String, ProgressReportReader> readers;
	
	static{
		readers = new HashMap<String, ProgressReportReader>();
	}
	
	private ProgressReportReader getProgressReport(String reportKey) throws Exception{
		ProgressReportReader progressSource = readers.get(reportKey);
		
		if(progressSource == null){
			File f = ProgressReportFileCreate.getFile(reportKey);
			if(f.canRead()){
				try{
					progressSource = new ProgressReportFileReader(f);
					progressSource.start();
					readers.put(reportKey, progressSource);
				}
				catch(IOException e){
					throw new Exception("Ocorreu um erro inesperado e não " +
							"será possível acompanhar o progresso da importação desses dados. " +
							"No entanto, eles foram enviados ao servidor e em breve será " +
							"exibida uma mensagem confirmando o seu processamento.");
				}
			}
		}
		
		return progressSource;
	}
	
	public String getNewKey(){
		return ProgressReportFileCreate.getNewKey();
	}
	
	public Boolean isReadyToRead(String reportKey) throws Exception{
		ProgressReportReader progressSource = getProgressReport(reportKey);
		
		return progressSource != null;
	}
	
	public Boolean isFinished(String reportKey) throws Exception{
		ProgressReportReader progressSource = getProgressReport(reportKey);
		
		return (progressSource == null) ? null : progressSource.isFinished();
	}
	
	@SuppressWarnings("unused")
	public Boolean isClosed(String reportKey) throws Exception{
		File f = ProgressReportFileCreate.getFile(reportKey);
		ProgressReportReader progressSource = readers.get(reportKey);
		progressSource = null;
		
		if(f.exists()){
			if(!f.delete()) return false;
		}
		f = null;
		return true;
	}

	public List<String> getProgressMessages(String reportKey) throws Exception{
		ProgressReportReader progressSource = getProgressReport(reportKey);
		
		return (progressSource == null) ? null : progressSource.getProgressMessages();
	}

}
