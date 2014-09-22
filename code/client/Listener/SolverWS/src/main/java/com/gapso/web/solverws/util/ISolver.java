package com.gapso.web.solverws.util;

import java.io.InputStream;

public interface ISolver {

	String help();

	String version();

	String getSolverVersion();

	boolean isFinished(long round);

	boolean hasResult(long round);

	long requestOptimization(String[] names, InputStream[] data);

	InputStream getFinalResult(long round);

	InputStream getFile(String prefix, long round);
	
	public boolean cancelOptimization(long round);

	public boolean isCanceled(long round);

	public boolean cancelAll();

	public String[] getQueue();
	
	public String getPositionQueue(long round);
}