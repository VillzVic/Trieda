package com.gapso.web.solverws.util;

import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;

public final class SolverStandAloneImpl implements ISolver {

	private static final SolverQueue solverQueue = new SolverQueue(1);

	private final String problemName;
	
	public SolverStandAloneImpl(String problemName) {
		super();
		this.problemName = problemName;
	}	
	
	@Override
	public String help() {
		return "Stand Alone Solver Queue";
	}

	@Override
	public String version() {
		return "0.0.1";
	}

	@Override
	public boolean isFinished(long round) {
		return solverQueue.isFinish(round);
	}

	@Override
	public boolean hasResult(long round) {
		return FileManager.isExistOutputFile(round);
	}

	@Override
	public boolean requestOptimization(String[] names, InputStream[] data) {
		Long uniqueID = UniqueId.createID();
		int i=0;
		boolean result = true;
		for (InputStream bis : data) {
			result&=FileManager.createFile(names[i++], uniqueID, new BufferedInputStream(bis,1024*250));
		}
		solverQueue.addTask(problemName, uniqueID);
		return result;
	}

	@Override
	public InputStream getContent(String filename, long round) {
		ByteArrayInputStream result = null;
		try {
			byte[] fileBytes = FileManager.getContentOutputFile(round);
			result = new ByteArrayInputStream(fileBytes);
		} catch (IOException e) {
			e.printStackTrace();
		}
		return result;
	}

	@Override
	public boolean cancelOptimization(long round) {
		return solverQueue.cancelOptimization(round);
	}

	@Override
	public boolean isCanceled(long round) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean cancelAll() {
		solverQueue.cancelAll();
		return true;
	}

	@Override
	public String[] getQueue() {
		return solverQueue.getQueue();
	}
}
