package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.google.gwt.user.client.rpc.AsyncCallback;

public interface ProgressReportServiceAsync {

	public void getNewKey(AsyncCallback<String> callback);
	public void isReadyToRead(String reportKey, AsyncCallback<Boolean> callback);
	public void isFinished(String reportKey, AsyncCallback<Boolean> callback);
	public void isClosed(String reportKey, AsyncCallback<Boolean> callback);
	public void getProgressMessages(String reportKey, AsyncCallback<List<String>> callback);
	
}
