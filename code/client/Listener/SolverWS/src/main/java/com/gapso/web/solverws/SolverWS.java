package com.gapso.web.solverws;

import java.io.BufferedInputStream;
import java.io.IOException;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.QueryParam;

import com.gapso.web.solverws.util.FileManager;
import com.gapso.web.solverws.util.SolverQueue;
import com.gapso.web.solverws.util.UniqueId;
import com.sun.jersey.multipart.BodyPartEntity;
import com.sun.jersey.multipart.MultiPart;

@Path("/")
public class SolverWS{

	private String version = "0.1";
	final static int NUMBER_OF_QUEUE = 2;
	private static SolverQueue solverQueue = new SolverQueue(NUMBER_OF_QUEUE);

	@GET
	public SolverResponse blank() {
		return help();
	}

	@GET
	@Path("/help")
	public SolverResponse help() {
		String content = "http://wiki.gapso.com.br";
		return new SolverResponse(true, content);
	}

	@GET
	@Path("/version")
	public SolverResponse version() {
		return new SolverResponse(true, version);
	}

	@POST
	@Path("/isFinished/{problemName}")
	public SolverResponse isFinished(@PathParam("problemName") String problemName, @QueryParam("round") String roundString) {
		Long round = new Long(roundString);
		return new SolverResponse(true, solverQueue.isFinish(round));
	}
	
	@POST
	@Path("/containsResult/{problemName}")
	public SolverResponse containsResult(@PathParam("problemName") String problemName, @QueryParam("round") String roundString) {
		Long round = new Long(roundString);
		return new SolverResponse(true, FileManager.isExistOutputFile(round));
	}
	
	@POST
	@Consumes("multipart/mixed")
	@Path("/requestOptimization/{problemName}")
	public SolverResponse requestOptimization(@PathParam("problemName") String problemName, MultiPart multiPart) {
		Long uniqueID = UniqueId.createID();
		boolean save = false;
		try {
			BodyPartEntity bpe = (BodyPartEntity)multiPart.getBodyParts().get(0).getEntity();
			BufferedInputStream bis = (BufferedInputStream) bpe.getInputStream();
			save = FileManager.createFile(uniqueID, bis);
	
			if(save) solverQueue.addTask("trieda", uniqueID);
		} catch (IOException e) {
			e.printStackTrace();
		}

		return new SolverResponse(save, (save) ? uniqueID : null);
	}
	
	@POST
	@Path("/getContent/{problemName}")
	public SolverResponse getContent(@PathParam("problemName") String problemName, @QueryParam("round") String roundString) {
//		roundString = "201105060917440"; // TODO
		Long round = new Long(roundString);
		SolverResponse sr = null;
		try {
			byte[] fileBytes = FileManager.getContentOutputFile(round);
			sr = new SolverResponse(true, fileBytes);
		} catch (IOException e) {
			e.printStackTrace();
			sr = new SolverResponse(false, null);
		}
		
		return sr;
	}

}
