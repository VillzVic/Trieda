package com.gapso.web.solverws.util;

import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;

public final class SolverStandAloneImpl implements ISolver {

	private final SolverQueue solverQueue;

	private final String problemName;

	private FileManager fileManager;
	
	public SolverStandAloneImpl(String problemName, String basedir) {
		super();
		this.problemName = problemName;
		this.fileManager = new FileManager(basedir);
		this.solverQueue = new SolverQueue(1, basedir);
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
		return fileManager.isExistOutputFile(round);
	}

	@Override
	public long requestOptimization(String[] names, InputStream[] data) {
		Long uniqueID = UniqueId.createID();
		int i=0;
		boolean result = true;
		for (InputStream bis : data) {
			result&=fileManager.createFile(names[i++], uniqueID, new BufferedInputStream(bis,1024*250));
		}
		solverQueue.addTask(problemName, uniqueID);
		return uniqueID;
	}

	@Override
	public InputStream getFinalResult(String filename, long round) {
		ByteArrayInputStream result = null;
		try {
			byte[] fileBytes = fileManager.getContentOutputFile(round);
			result = new ByteArrayInputStream(fileBytes);
		} catch (IOException e) {
			e.printStackTrace();
		}
		return result;
	}

	@Override
	public InputStream getFile(String filename, long round) {
		ByteArrayInputStream result = null;
		try {
			byte[] fileBytes = fileManager.getFile(filename, round);
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
