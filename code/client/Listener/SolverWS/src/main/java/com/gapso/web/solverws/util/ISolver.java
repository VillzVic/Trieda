package com.gapso.web.solverws.util;

import java.io.InputStream;

public interface ISolver {

	String help();

	String version();

	boolean isFinished(long round);

	boolean hasResult(long round);

	boolean requestOptimization(String[] names, InputStream[] data);

	InputStream getContent(String filename, long round);
	
	public boolean cancelOptimization(long round);

	public boolean isCanceled(long round);

	public boolean cancelAll();

	public String[] getQueue();


}