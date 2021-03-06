package com.gapso.web.solverws.util;

import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public final class SolverStandAloneImpl implements ISolver {
	private static final Logger logger = LoggerFactory.getLogger(SolverStandAloneImpl.class);

	private final SolverQueue solverQueue;

	private final String problemName;

	private FileManager fileManager;
	
	public SolverStandAloneImpl(String problemName, String basedir) {
		this(1, problemName, basedir);
	}	
	
	public SolverStandAloneImpl(int queueSize, String problemName, String basedir) {
		this.problemName = problemName+"solver";
		this.fileManager = new FileManager(basedir);
		this.solverQueue = new SolverQueue(queueSize, basedir);
	}

	
	public String help() {
		return "Stand Alone Solver Queue";
	}

	
	public String version() {
		return "0.0.1";
	}

	
	public boolean isFinished(long round) {
		return solverQueue.isFinish(round);
	}

	
	public boolean hasResult(long round) {
		return fileManager.isExistOutputFile(round);
	}

	
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

	
	public InputStream getFinalResult(long round) {
		ByteArrayInputStream result = null;
		try {
			byte[] fileBytes = fileManager.getContentOutputFile(round);
			result = new ByteArrayInputStream(fileBytes);
		} catch (IOException e) {
			e.printStackTrace();
		}
		return result;
	}

	
	public InputStream getFile(String filename, long round) {
		ByteArrayInputStream result = null;
		try {
			byte[] fileBytes = fileManager.getFile(filename, round);
			result = new ByteArrayInputStream(fileBytes);
		} catch (IOException e) {
			logger.debug("Error fetching file", e);
		}
		return result;
	}
	
	
	public boolean cancelOptimization(long round) {
		return solverQueue.cancelOptimization(round);
	}

	
	public boolean isCanceled(long round) {
		return false;
	}

	
	public boolean cancelAll() {
		solverQueue.cancelAll();
		return true;
	}

	
	public String[] getQueue() {
		return solverQueue.getQueue();
	}

	public String getPositionQueue(long round) {
		return solverQueue.getPositionQueue(round);
	}
	
	public String getSolverVersion() {
		return solverQueue.getSolverVersion(problemName);
	}
}
