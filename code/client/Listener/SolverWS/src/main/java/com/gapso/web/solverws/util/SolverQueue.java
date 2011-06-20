package com.gapso.web.solverws.util;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.Callable;
import java.util.concurrent.CompletionService;
import java.util.concurrent.ExecutorCompletionService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;


public class SolverQueue {
	
	private final CompletionService<Process> completionService;
	private final Producer producer;
	private final Map<Long, Future<Process>> tasks;
	
	public SolverQueue(int numberProducer) {
		completionService = new ExecutorCompletionService<Process>(Executors.newFixedThreadPool(numberProducer));
		tasks = new HashMap<Long, Future<Process>>();
		producer = new Producer();
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
		Future<Process> future = producer.addTask(new ProcessBuilder(problemName+".exe", uniqueID.toString(), "."));
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
