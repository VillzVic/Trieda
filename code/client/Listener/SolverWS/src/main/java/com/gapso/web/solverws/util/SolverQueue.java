package com.gapso.web.solverws.util;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.Callable;
import java.util.concurrent.CompletionService;
import java.util.concurrent.ExecutorCompletionService;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class SolverQueue {
	private static final Logger logger = LoggerFactory.getLogger(SolverQueue.class);
	private final CompletionService<Process> completionService;
	private final Producer producer;
	private final Map<Long, Future<Process>> tasks;
	private final Map<Long, String> status;
	private final String basedir;
	private final ExecutorService executor;
	private final Map<Long, Process> runningProcesses;

	public SolverQueue(int numberProducer, String basedir) {
		this.basedir = basedir;
		executor = Executors.newFixedThreadPool(numberProducer);
		completionService = new ExecutorCompletionService<Process>(executor);
		tasks = new HashMap<Long, Future<Process>>();
		status = new HashMap<Long, String>();
		producer = new Producer();
		runningProcesses = new HashMap<Long, Process>();
	}

	public SolverQueue(int numberProducer) {
		this(numberProducer, ".");
	}

	private class Producer {

		private final class SolverProcess implements Callable<Process> {
			private final ProcessBuilder value;

			private SolverProcess(ProcessBuilder value) {
				this.value = value;
			}

			public Process call() throws Exception {
				final long id = Long.parseLong(value.command().get(1));
				final Process proc = value.start();
				StreamGobbler outputGobbler = new StreamGobbler(proc.getInputStream(), "OUTPUT");
				outputGobbler.start();
				runningProcesses.put(id, proc);
				logger.info("Process {} Started", value.command().get(0));
				status.put(id, "Executando");
				final int exitValue = proc.waitFor();
				logger.info("Process Exited with code {} ", exitValue);
				status.put(id, "Finalizado");
				runningProcesses.remove(id);
				return proc;
			}
		}

		public Future<Process> addTask(final ProcessBuilder value) {
			return completionService.submit(new SolverProcess(value));
		}
	}

	// Adiciona na file
	public void addTask(String problemName, Long uniqueID) {
		StringBuffer sb = new StringBuffer();
		sb.append(basedir).append(File.separator);
		String dir = sb.toString();
		sb.append(problemName).append(".exe");
		final String solverfile = sb.toString();
		logger.info("dir: {} solver: {}", new Object[] { dir, solverfile });
		final ProcessBuilder builder = new ProcessBuilder(solverfile, uniqueID.toString(), dir);
		builder.redirectErrorStream(true);
		Future<Process> future = producer.addTask(builder);
		tasks.put(uniqueID, future);
		status.put(uniqueID, "Aguardando");
	}

	// Nao esta na fila ou ja finalizou
	public Boolean isFinish(Long uniqueID) {
		return (tasks.get(uniqueID) == null) ? true : tasks.get(uniqueID).isDone();
	}

	public void cancelAll() {
		executor.shutdownNow();
		final Long[] keySet = tasks.keySet().toArray(new Long[] {});
		for (Long key : keySet) {
			cancelOptimization(key);
		}
	}

	
	public Boolean cancelOptimization(Long uniqueID) {
		final Future<Process> future = tasks.get(uniqueID);
		final Process runningProcess = runningProcesses.get(uniqueID);
		if (runningProcess != null)
			runningProcess.destroy();
		else
			return false;
		
		final boolean result = future.cancel(true);
		if (result) {
			status.put(uniqueID, "Cancelado");
			tasks.remove(uniqueID);
		}
		logger.info("cancel task {} result {}", new Object[] { uniqueID, result });
		return result;
	}

	public String[] getQueue() {
		List<String> result = new ArrayList<String>();
		result.add("Queue Size: " + tasks.size());
		StringBuffer sb = new StringBuffer();
		for (long id : status.keySet()) {
			sb.append(id).append(' ');
			sb.append(status.get(id));
			result.add(sb.toString());
			sb.setLength(0);
		}
		return result.toArray(new String[] {});
	}

	public String getSolverVersion(String problemName) {
		StringBuffer sb = new StringBuffer();
		sb.append(basedir).append(File.separator);
		String param = "-version";
		sb.append(problemName).append(".exe");
		final String solverfile = sb.toString();
		final ProcessBuilder processBuilder = new ProcessBuilder(solverfile, param);
		final InputStream processOut;
		String result = "";
		try {
			final Process proc = processBuilder.start();
			processOut = proc.getInputStream();
			proc.waitFor();
			byte[] buffer = new byte[1024];
			final int read = processOut.read(buffer);
			StringBuilder sbr = new StringBuilder();
			if (read > 0)
				sbr.append(new String(buffer), 0, read);
			result = sbr.toString();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}

}

class StreamGobbler extends Thread {
	InputStream is;
	String type;

	StreamGobbler(InputStream is, String type) {
		this.is = is;
		this.type = type;
	}

	public void run() {
		try {
			InputStreamReader isr = new InputStreamReader(is);
			BufferedReader br = new BufferedReader(isr);
			String line = null;
			while ((line = br.readLine()) != null)
				System.out.println(type + ">" + line);
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}
}