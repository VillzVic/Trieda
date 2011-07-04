package com.gapso.web.solverws;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.InputStream;
import java.net.URL;
import java.util.Arrays;

import org.junit.Test;

import com.gapso.web.solverws.util.ISolver;
import com.gapso.web.solverws.util.SolverStandAloneImpl;

public class ISolverTest {

	// @Test
	public final void testRequestOptimization() throws Exception {
		final URL resource = getClass().getResource("/prob2solver.exe");
		final File file = new File(resource.getFile());
		System.out.println(file.getPath());
		System.out.println(file.getParent());
		assertNotNull(resource);
		InputStream[] data = getData();
		ISolver solver = new SolverStandAloneImpl("prob2", file.getParent());
		System.out.println("Dir: " + file.getParent());
		final long id = solver.requestOptimization(new String[] { "input" }, data);
		assertTrue(id > 0);
		while (!solver.isFinished(id))
			;
		final InputStream content = solver.getFile("output", id);
		byte[] cont = new byte[10];
		content.read(cont);
		System.out.println(new String(cont));
	}

	// @Test
	public final void testCancelOptimization() throws Exception {
		final URL resource = getClass().getResource("/prob2solver.exe");
		final File file = new File(resource.getFile());
		assertNotNull(resource);
		InputStream[] data = getData();
		ISolver solver = new SolverStandAloneImpl("prob2", file.getParent());
		final long id = solver.requestOptimization(new String[] { "input" }, getData());
		final long id1 = solver.requestOptimization(new String[] { "input" }, getData());
		assertTrue(id > 0);
		Thread.sleep(500);
		solver.cancelOptimization(id);
		assertTrue(solver.isFinished(id));
		final InputStream content = solver.getFile("output", id);
		assertNull(content);
		Thread.sleep(1500);
		assertTrue(Arrays.deepToString(solver.getQueue()), Arrays.deepToString(solver.getQueue()).contains("Executando"));
		System.out.println(Arrays.deepToString(solver.getQueue()).replace(", ", "\n"));
		solver.cancelOptimization(id1);
		Thread.sleep(500);
		assertTrue(Arrays.deepToString(solver.getQueue()), !Arrays.deepToString(solver.getQueue()).contains("Executando"));
	}

	 @Test
	public void testQueueSize() throws Exception {
		ISolver solver = getSolver(3);
		final long id1 = solver.requestOptimization(new String[] { "input" }, getData());
		final long id2 = solver.requestOptimization(new String[] { "input" }, getData());
		final long id3 = solver.requestOptimization(new String[] { "input" }, getData());
		final long id4 = solver.requestOptimization(new String[] { "input" }, getData());
		final long id5 = solver.requestOptimization(new String[] { "input" }, getData());
		Thread.sleep(1000);
		System.out.println(Arrays.deepToString(solver.getQueue()).replace(", ", "\n"));
		while (!solver.isFinished(id1))
			;
		System.out.println(Arrays.deepToString(solver.getQueue()).replace(", ", "\n"));
		solver.cancelOptimization(id5);
		System.out.println(Arrays.deepToString(solver.getQueue()).replace(", ", "\n"));
		Thread.sleep(1000);
		solver.cancelAll();
		System.out.println(Arrays.deepToString(solver.getQueue()).replace(", ", "\n"));
	}

	private InputStream[] getData() {
		InputStream[] data = new InputStream[1];
		data[0] = new ByteArrayInputStream("hello world".getBytes());
		return data;
	}

	private ISolver getSolver(int queueSize) {
		final URL resource = getClass().getResource("/prob2solver.exe");
		final File file = new File(resource.getFile());
		assertNotNull(resource);
		ISolver solver = new SolverStandAloneImpl(queueSize, "prob2", file.getParent());
		return solver;
	}

//	@Test
	public void testGetFinalResult() throws Exception {
		final URL resource = getClass().getResource("/prob2solver.exe");
		final File file = new File(resource.getFile());
		assertNotNull(resource);
		InputStream[] data = getData();
		ISolver solver = new SolverStandAloneImpl(3, "prob2", file.getParent());
		final long id = solver.requestOptimization(new String[] { "input" }, getData());
		File f = new File(file.getParent() + "\\output" + id + "F");
		f.createNewFile();
		while (!solver.isFinished(id));
		assertTrue(solver.hasResult(id));
		final InputStream finalResult = solver.getFinalResult(id);
		assertNotNull(finalResult);
	}

	 @Test
	public void testCancelAll() throws Exception {
		ISolver solver = getSolver(1);
		InputStream[] data = getData();
		final long id1 = solver.requestOptimization(new String[] { "input" }, getData());
		final long id2 = solver.requestOptimization(new String[] { "input" }, getData());
		final long id3 = solver.requestOptimization(new String[] { "input" }, getData());
		final long id4 = solver.requestOptimization(new String[] { "input" }, getData());
		final long id5 = solver.requestOptimization(new String[] { "input" }, getData());
		Thread.sleep(500);
		System.out.println(Arrays.deepToString(solver.getQueue()).replace(", ", "\n"));
		solver.cancelAll();
		Thread.sleep(1000);
		System.out.println(Arrays.deepToString(solver.getQueue()).replace(", ", "\n"));

	}

	// @Test
	public void testSolverVersion() throws Exception {
		final ISolver solver = getSolver(1);
		System.out.println(solver.getSolverVersion());
	}

}
