package com.gapso.web.trieda.server.util.progressReport;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;

public class ProgressReportFileWriter extends ProgressReportWriter{
	private BufferedWriter fileWrite;
	
	public ProgressReportFileWriter(File f) throws IOException{
		super(new FileOutputStream(f));
		fileWrite = new BufferedWriter(new FileWriter(f));
	}
	
	public void writeMsg(String msg){
		try{
			fileWrite.write(msg + "\n");
			fileWrite.flush();
		}
		catch(IOException e){
			e.printStackTrace();
		}
	}
	
	public void finish(String msg){
		super.finish(msg);
		try{
			fileWrite.close();
		}
		catch(IOException e){
			e.printStackTrace();
		}
		fileWrite = null;
	}
	
	protected void finalize() throws Throwable{
		if(fileWrite != null) fileWrite.close();
		
		super.finalize();
	}
}
