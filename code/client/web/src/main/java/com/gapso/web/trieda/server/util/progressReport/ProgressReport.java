package com.gapso.web.trieda.server.util.progressReport;

public abstract class ProgressReport{
	
	private int status;
	
	public ProgressReport(){
		status = 0;
	}
	
	public void start(){
		status = 1;
	}
	
	public void finish(){
		status = 0;
	}
	
	public boolean isFinished(){
		return status == 0;
	}
	
	public int getStatus(){
		return status;
	}
	
}
