package com.gapso.web.solverws;

import static org.junit.Assert.*;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.zip.GZIPInputStream;

import org.junit.Test;

import com.gapso.web.solverws.util.ISolver;
import com.gapso.web.solverws.util.SolverStandAloneImpl;

public class APSSoverTest {

	@Test
	public void testAPSSolver() throws Exception {
		ISolver solver = getSolver(1);
		final long id = solver.requestOptimization(new String[] { "input" }, getData());
		while (!solver.isFinished(id));
		System.in.read();
	}

	private ISolver getSolver(int queueSize) {
		final URL resource = getClass().getResource("/prob2solver.exe");
		final File file = new File(resource.getFile());
		assertNotNull(resource);
		ISolver solver = new SolverStandAloneImpl(queueSize, "APS", file.getParent());
		return solver;
	}

	private InputStream[] getData() throws Exception {
		InputStream[] data = new InputStream[1];		
		data[0] =new GZIPInputStream(getClass().getResourceAsStream("/inputAPS.gz")); 
		return data;
	}

}
