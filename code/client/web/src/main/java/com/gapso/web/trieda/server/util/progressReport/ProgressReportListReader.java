package com.gapso.web.trieda.server.util.progressReport;

import java.util.ArrayList;
import java.util.List;

public class ProgressReportListReader extends ProgressReportReader{
	private List<String> listRead;
	private int count;
	
	public ProgressReportListReader(List<String> listRead){
		this.listRead = listRead;
	}
	
	public void start(){
		super.start();
		count = 0;
	}
	
	public List<String> getProgressMessages(){
		List<String> resposta = new ArrayList<String>();
		String msg = "";
		
		for(; count < listRead.size(); count++){
			msg = listRead.get(count);
			if(msg.compareTo("$EOT$") == 0){
				finish();
				break;
			}
			else resposta.add(msg);
		}
		
		return resposta;
	}
	
}
