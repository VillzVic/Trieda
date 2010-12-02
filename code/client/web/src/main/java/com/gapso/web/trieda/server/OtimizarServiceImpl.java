package com.gapso.web.trieda.server;

import java.io.ByteArrayOutputStream;
import java.io.StringReader;

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

	@Override
	@Transactional
	public Long input(CenarioDTO cenarioDTO) {
		Cenario cenario = Cenario.find(cenarioDTO.getId());
		SolverInput solverInput = new SolverInput(cenario);
		TriedaInput triedaInput = solverInput.generateTriedaInput();

		byte[] fileBytes = null;
		try {
			final ByteArrayOutputStream temp = new ByteArrayOutputStream();
			JAXBContext jc = JAXBContext
					.newInstance("com.gapso.web.trieda.server.xml.input");
			Marshaller m = jc.createMarshaller();
			m.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);
			m.setProperty(Marshaller.JAXB_ENCODING, "UTF-8");
			m.marshal(triedaInput, temp);
			// OutputStream xml = new FileOutputStream(new File("entrada.xml"));
			// xml.write(temp.toByteArray());

			fileBytes = temp.toByteArray();
			triedaInput.toString();
			
			new StreamSource();
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		SolverClient solverClient = new SolverClient("http://localhost:3402/SolverWS", "trieda");
		return solverClient.requestOptimization(fileBytes);
	}

	@Override
	public Boolean isOptimizing(Long round) {
		SolverClient solverClient = new SolverClient("http://localhost:3402/SolverWS", "trieda");
		return !solverClient.isFinished(round);
	}

	@Override
	public Boolean saveContent(CenarioDTO cenarioDTO, Long round) {
		Cenario cenario = Cenario.find(cenarioDTO.getId());
		try {
			SolverClient solverClient = new SolverClient("http://localhost:3402/SolverWS", "trieda");
			byte[] xmlBytes = solverClient.getContent(round);
			if(xmlBytes == null) return false;
			
			JAXBContext jc = JAXBContext.newInstance("com.gapso.web.trieda.server.xml.input");
			Unmarshaller u = jc.createUnmarshaller();
			StringBuffer xmlStr = new StringBuffer(new String(xmlBytes));
			TriedaOutput triedaOutput = (TriedaOutput) u.unmarshal(new StreamSource(new StringReader(xmlStr.toString())));
			
			SolverOutput solverOutput = new SolverOutput(cenario, triedaOutput);
			solverOutput.generateAtendimentosTatico();
			solverOutput.salvarAtendimentosTatico();
			
		} catch (JAXBException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}
	
}
