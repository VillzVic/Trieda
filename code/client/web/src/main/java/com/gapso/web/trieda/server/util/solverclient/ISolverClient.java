package com.gapso.web.trieda.server.util.solverclient;


public interface ISolverClient {

	public String help();

	public String version();
	
	public boolean isFinished(Long round);
	
	public boolean containsResult(Long round);
	
	public long requestOptimization(byte[] fileBytes);

	public byte[] getContent(Long round);
	
	public boolean cancelOptimization(Long round);
	
	public void cancelAllOptimizations();
}