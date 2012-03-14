package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath("progressReport")
public interface ProgressReportService extends RemoteService{

	public String getNewKey();
	public Boolean isReadyToRead(String reportKey) throws Exception;
	public Boolean isFinished(String reportKey) throws Exception;
	public Boolean isClosed(String reportKey) throws Exception;
	public List<String> getProgressMessages(String reportKey) throws Exception;
	
}
