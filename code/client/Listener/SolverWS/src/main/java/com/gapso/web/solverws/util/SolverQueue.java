package com.gapso.web.solverws.util;

import java.io.BufferedReader;
import java.io.File;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.Callable;
import java.util.concurrent.CompletionService;
import java.util.concurrent.ExecutorCompletionService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


public class SolverQueue {
	private static final Logger logger = LoggerFactory.getLogger(SolverQueue.class);
	private final CompletionService<Process> completionService;
	private final Producer producer;
	private final Map<Long, Future<Process>> tasks;
	private final String basedir;
	
	public SolverQueue(int numberProducer, String basedir) {
		this.basedir = basedir;
		completionService = new ExecutorCompletionService<Process>(Executors.newFixedThreadPool(numberProducer));
		tasks = new HashMap<Long, Future<Process>>();
		producer = new Producer();
	}

	public SolverQueue(int numberProducer){
		this(numberProducer, ".");
	}
	
	private class Producer {
		public Future<Process> addTask(final ProcessBuilder value) {
			return completionService.submit(new Callable<Process>() {
				public Process call() throws Exception {
					Process process = value.start();
					InputStream is = process.getInputStream();
					InputStreamReader isr = new InputStreamReader(is);
					BufferedReader br = new BufferedReader(isr);
					while (br.readLine() != null);
					return process;
				}
			});
		}
	}
	
	// Adiciona na file
	public void addTask(String problemName, Long uniqueID) {
		StringBuffer sb = new StringBuffer();
		sb.append(basedir).append(File.separator);
		String dir = sb.toString();
		sb.append(problemName).append(".exe");
		final String solverfile = sb.toString();
		logger.info("dir: {} solver: {}", new Object[]{dir, solverfile});
		Future<Process> future = producer.addTask(new ProcessBuilder(solverfile, uniqueID.toString(), dir));
		tasks.put(uniqueID, future);
	}
	
	// Nao esta na fila ou ja finalizou
	public Boolean isFinish(Long uniqueID) {
		return (tasks.get(uniqueID) == null)? true : tasks.get(uniqueID).isDone();
	}
	
	public void cancelAll(){
		tasks.clear();
	}
	
	public Boolean cancelOptimization(Long uniqueID) {
		return tasks.remove(uniqueID)!=null;
	}

	public String[] getQueue() {
		return new String[]{"Queue Size: " + tasks.size()};
	}

}
