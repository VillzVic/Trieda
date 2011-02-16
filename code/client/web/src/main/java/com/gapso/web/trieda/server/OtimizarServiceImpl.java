package com.gapso.web.trieda.server;

import java.io.ByteArrayOutputStream;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.transform.stream.StreamSource;

import org.springframework.stereotype.Repository;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.domain.Cenario;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.services.OtimizarService;
import com.gapso.web.trieda.server.util.SolverInput;
import com.gapso.web.trieda.server.util.SolverOutput;
import com.gapso.web.trieda.server.util.solverclient.SolverClient;
import com.gapso.web.trieda.server.xml.input.ItemError;
import com.gapso.web.trieda.server.xml.input.ItemWarning;
import com.gapso.web.trieda.server.xml.input.TriedaInput;
import com.gapso.web.trieda.server.xml.input.TriedaOutput;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
@Transactional
@Service
@Repository
public class OtimizarServiceImpl extends RemoteServiceServlet implements OtimizarService {

	private static final long serialVersionUID = 5716065588362358065L;
//	private static final String linkSolver = "http://offspring:8080/SolverWS";    // SERVIDOR // OFFSPRING
//	private static final String linkSolver = "http://toquinho:8080/SolverWS";     // MAQUINA DO MÁRIO // TOQUINHO
	private static final String linkSolver = "http://192.168.1.39:8080/SolverWS"; // MAQUINA DO CLAUDIO // NIRVANA
//	private static final String linkSolver = "http://localhost:3402/SolverWS";    // MAQUINA DESENVOLVIMENTO (CLAUDIO)

	@Override
	@Transactional
	public Long input(CenarioDTO cenarioDTO) {
		Cenario cenario = Cenario.find(cenarioDTO.getId());
		SolverInput solverInput = new SolverInput(cenario);
		TriedaInput triedaInput = solverInput.generateTriedaInput();

		byte[] fileBytes = null;
		try {
			final ByteArrayOutputStream temp = new ByteArrayOutputStream();
			JAXBContext jc = JAXBContext.newInstance("com.gapso.web.trieda.server.xml.input");
			Marshaller m = jc.createMarshaller();
			m.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);
			m.setProperty(Marshaller.JAXB_ENCODING, "ISO-8859-1");
			m.marshal(triedaInput, temp);
			fileBytes = temp.toByteArray();
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		SolverClient solverClient = new SolverClient(linkSolver, "trieda");
		return solverClient.requestOptimization(fileBytes);
	}

	@Override
	public Boolean isOptimizing(Long round) {
		SolverClient solverClient = new SolverClient(linkSolver, "trieda");
		return !solverClient.isFinished(round);
	}

	@Override
	public Map<String, List<String>> saveContent(CenarioDTO cenarioDTO, Long round) {
		Cenario cenario = Cenario.find(cenarioDTO.getId());
		
		Map<String, List<String>> ret = new HashMap<String, List<String>>(2);
		ret.put("warning", new ArrayList<String>());
		ret.put("error", new ArrayList<String>());
		
		try {
			SolverClient solverClient = new SolverClient(linkSolver, "trieda");
			byte[] xmlBytes = solverClient.getContent(round);
			if(xmlBytes == null) {
				ret.get("error").add("Erro no servidor");
				return ret;
			}
			
			JAXBContext jc = JAXBContext.newInstance("com.gapso.web.trieda.server.xml.input");
			Unmarshaller u = jc.createUnmarshaller();
			StringBuffer xmlStr = new StringBuffer(new String(xmlBytes));
			TriedaOutput triedaOutput = (TriedaOutput) u.unmarshal(new StreamSource(new StringReader(xmlStr.toString())));
			
			for(ItemError erro : triedaOutput.getErrors().getError()) {
				ret.get("error").add(erro.getMessage());
			}
			for(ItemWarning warning : triedaOutput.getWarnings().getWarning()) {
				ret.get("warning").add(warning.getMessage());
			}
			
			if(!triedaOutput.getErrors().getError().isEmpty()) {
				return ret;
			}
			SolverOutput solverOutput = new SolverOutput(cenario, triedaOutput);
			solverOutput.generateAtendimentosTatico();
			solverOutput.salvarAtendimentosTatico();
			
		} catch (JAXBException e) {
			e.printStackTrace();
			ret.get("error").add("Erro ao salvar no banco");
			return ret;
		}
		return ret;
	}
	
}
