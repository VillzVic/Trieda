package com.gapso.web.trieda.server.util.progressReport;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;


public class ProgressReportFileReader extends ProgressReportReader{
	
	private BufferedReader fileRead;
	
	public ProgressReportFileReader(File f) throws IOException{
		super();
		fileRead = new BufferedReader(new FileReader(f));
	}
	
	public void finish(){
		super.finish();
		try{
			fileRead.close();
			fileRead = null;
		}
		catch(IOException e){
			e.printStackTrace();
		}
	}
	
	public List<String> getProgressMessages(){
		List<String> resposta = new ArrayList<String>();
		String msg = "";
		
		try{
			while((msg = fileRead.readLine()) != null){
				if(msg.compareTo("$EOT$") == 0){
					finish();
					break;
				}
				else resposta.add(msg);
			}
		}
		catch(IOException e){
			e.printStackTrace();
		}
		
		return resposta;
	}
	
	protected void finalize() throws Throwable{
		if(fileRead != null) fileRead.close();
		
		super.finalize();
	}
	
}
