package com.gapso.web.trieda.server.util.progressReport;

import java.io.OutputStream;
import java.io.PrintStream;

public abstract class ProgressReportWriter extends ProgressReport{
	private Integer totalTime;
	private Long time;
	private PrintStream ps, antigo;
	private int cont;
	
	public ProgressReportWriter(OutputStream os){
		super();
		cont = 0;
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
		if(ps != null){
			antigo = System.out;
			System.setOut(ps);
		}
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
		if(msg == null) msg = "Processo concluído";
		writeMsg(msg);
		writeMsg("Tempo total: " + descricaoTempo(totalTime));
		writeMsg("$EOT$");
		if(antigo != null){
			System.setOut(antigo);
			antigo = null;
		}
	}
	
	public void setInitNewPartial(String msg){
		writeMsg(++cont + ". " + msg);
		time = System.currentTimeMillis();
	}
	
	public void setPartial(String msg){
		Integer interval = Math.round((System.currentTimeMillis() - time)/1000);
		writeMsg(msg + " - " + descricaoTempo(interval));
		totalTime += interval;
	}
	
}
