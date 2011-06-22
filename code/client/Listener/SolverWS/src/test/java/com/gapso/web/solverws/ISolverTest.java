package com.gapso.web.solverws;

import static org.junit.Assert.*;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;

import org.junit.Before;
import org.junit.Test;

import com.gapso.web.solverws.util.ISolver;
import com.gapso.web.solverws.util.SolverStandAloneImpl;

public class ISolverTest {

	@Test
	public final void testRequestOptimization() throws Exception {
		final URL resource = getClass().getResource("/prob2solver.exe");
		final File file = new File(resource.getFile());
		System.out.println(file.getPath());
		System.out.println(file.getParent());
		assertNotNull(resource);
		InputStream[] data = new InputStream[1];
		data[0] = new ByteArrayInputStream("hello world".getBytes());
		ISolver solver = new SolverStandAloneImpl("prob2solver", file.getParent());
		System.out.println("Dir: " + file.getParent());
		final long id = solver.requestOptimization(new String[]{"input"}, data);
		assertTrue(id>0);
		while(!solver.isFinished(id));
		final InputStream content = solver.getFile("output", id);
		byte[] cont = new byte[10];
		content.read(cont);
		System.out.println(new String(cont));
	}
}
