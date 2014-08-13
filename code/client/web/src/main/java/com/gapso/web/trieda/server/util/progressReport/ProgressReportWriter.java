package com.gapso.web.trieda.server.util.progressReport;

import java.io.OutputStream;
import java.io.PrintStream;

public abstract class ProgressReportWriter extends ProgressReport{
	private Integer totalTime;
	private Long time;
	private Long subTime;
	private PrintStream ps, antigo;
	private int cont;
	private int subCont;
	
	public ProgressReportWriter(OutputStream os){
		super();
		cont = 0;
		subCont = 0;
		ps = new PrintStream(os){
			@Override
			public void print(String s) {
				if (s == null) {
				    s = "null";
				}
				writeMsg(s);
			}
			@Override
			public void println(String x) {
				synchronized (this) {}
				writeMsg(x);
			}
		};
	}
	
	public void start(){
		super.start();
		totalTime = 0;
/*		if(ps != null){
			antigo = System.out;
			//System.setOut(ps);
		}*/
	}
	
	protected String descricaoTempo(int tempo){
		String descricao = "";
		
		int temp = (int) Math.round(Math.ceil(tempo / 60));
		if(temp > 0){
			descricao += " " + temp + " minuto";
			if(temp > 1) descricao += "s";
		}
		
		temp = tempo % 60;
		descricao += " " + temp + " segundo";
		if(temp > 1) descricao += "s";
		
		return descricao;
	}
	
	protected abstract void writeMsg(String msg);
	
	public void finish(){
		finish(null);
	}
	
	public void finish(String msg){
		super.finish();
		if(msg == null) msg = "<b>Processo concluído</b>";
		writeMsg(msg);
		writeMsg("<b>Tempo total: </b>" + descricaoTempo(totalTime));
		writeMsg("$EOT$");
/*		if(antigo != null){
			//System.setOut(antigo);
			antigo = null;
		}*/
	}
	
	public void setInitNewPartial(String msg){
		writeMsg(++cont + ". " + msg);
		time = System.currentTimeMillis();
		subCont = 0;
	}
	
	public void setPartial(String msg){
		Integer interval = Math.round((System.currentTimeMillis() - time)/1000);
		writeMsg(msg + " - " + descricaoTempo(interval));
		totalTime += interval;
	}
	
	public void setPartial(String msg, boolean semSomatorio){
		Integer interval = Math.round((System.currentTimeMillis() - time)/1000);
		
		if(semSomatorio)
			time = System.currentTimeMillis();
		
		writeMsg(msg + " - " + descricaoTempo(interval));
		totalTime += interval;
	}
	
	public void setInitSubPartial(String msg){
		writeMsg(cont + "." + ++subCont + ". " + msg);
		subTime = System.currentTimeMillis();
	}
	
	public void endSubPartial(){
		Integer interval = Math.round((System.currentTimeMillis() - subTime)/1000);
		writeMsg("Finalizado: " + descricaoTempo(interval));
	}
	
}
