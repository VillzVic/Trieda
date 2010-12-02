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
	public Boolean saveContent(Long round) {
		try {
		       JAXBContext jc = JAXBContext.newInstance("com.gapso.web.trieda.server.xml.input");
		       Unmarshaller u = jc.createUnmarshaller();
		       StringBuffer xmlStr = new StringBuffer(getContent());
		       TriedaOutput o = (TriedaOutput) u.unmarshal( new StreamSource( new StringReader( xmlStr.toString() ) ) );
		       System.out.println(o);
		} catch (JAXBException e) {
			e.printStackTrace();
		}
		saveContent(null);
		return true;
	}
	
	private String getContent() {
		return "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?><TriedaOutput><atendimentos><AtendimentoCampus><campusId>Campus1</campusId><atendimentosUnidades><AtendimentoUnidade><unidadeId>UN-1</unidadeId><atendimentosSalas><AtendimentoSala><salaId>105</salaId><atendimentosDiasSemana><AtendimentoDiaSemana><diaSemana>6</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>2</ofertaCursoCampiId><disciplinaId>14</disciplinaId><quantidade>40</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>0</qtdeCreditosTeoricos><qtdeCreditosPraticos>2</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>5</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>1</ofertaCursoCampiId><disciplinaId>3</disciplinaId><quantidade>20</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>2</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>2</ofertaCursoCampiId><disciplinaId>3</disciplinaId><quantidade>20</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>2</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>4</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>2</ofertaCursoCampiId><disciplinaId>14</disciplinaId><quantidade>40</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>0</qtdeCreditosTeoricos><qtdeCreditosPraticos>2</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>3</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>1</ofertaCursoCampiId><disciplinaId>3</disciplinaId><quantidade>20</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>2</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>2</ofertaCursoCampiId><disciplinaId>3</disciplinaId><quantidade>20</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>2</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>0</diaSemana></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>1</diaSemana></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>2</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>2</ofertaCursoCampiId><disciplinaId>3</disciplinaId><quantidade>40</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>4</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana></atendimentosDiasSemana></AtendimentoSala><AtendimentoSala><salaId>103</salaId><atendimentosDiasSemana><AtendimentoDiaSemana><diaSemana>6</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>2</ofertaCursoCampiId><disciplinaId>15</disciplinaId><quantidade>25</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>2</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>5</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>2</ofertaCursoCampiId><disciplinaId>11</disciplinaId><quantidade>1</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>4</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>4</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>1</ofertaCursoCampiId><disciplinaId>10</disciplinaId><quantidade>40</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>2</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>2</ofertaCursoCampiId><disciplinaId>16</disciplinaId><quantidade>27</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>2</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>1</diaSemana></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>0</diaSemana></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>2</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>1</ofertaCursoCampiId><disciplinaId>1</disciplinaId><quantidade>25</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>2</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>3</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>2</ofertaCursoCampiId><disciplinaId>11</disciplinaId><quantidade>1</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>2</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana></atendimentosDiasSemana></AtendimentoSala><AtendimentoSala><salaId>104</salaId><atendimentosDiasSemana><AtendimentoDiaSemana><diaSemana>6</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>2</ofertaCursoCampiId><disciplinaId>14</disciplinaId><quantidade>40</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>2</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>3</diaSemana></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>2</diaSemana></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>1</diaSemana></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>0</diaSemana></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>4</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>1</ofertaCursoCampiId><disciplinaId>8</disciplinaId><quantidade>26</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>4</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>5</diaSemana></AtendimentoDiaSemana></atendimentosDiasSemana></AtendimentoSala><AtendimentoSala><salaId>101</salaId><atendimentosDiasSemana><AtendimentoDiaSemana><diaSemana>4</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>2</ofertaCursoCampiId><disciplinaId>7</disciplinaId><quantidade>45</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>4</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>3</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>2</ofertaCursoCampiId><disciplinaId>2</disciplinaId><quantidade>53</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>4</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>2</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>1</ofertaCursoCampiId><disciplinaId>5</disciplinaId><quantidade>34</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>1</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>2</ofertaCursoCampiId><disciplinaId>13</disciplinaId><quantidade>23</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>2</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>1</diaSemana></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>5</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>1</ofertaCursoCampiId><disciplinaId>4</disciplinaId><quantidade>24</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>2</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>1</ofertaCursoCampiId><disciplinaId>5</disciplinaId><quantidade>34</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>1</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>6</diaSemana></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>0</diaSemana></AtendimentoDiaSemana></atendimentosDiasSemana></AtendimentoSala><AtendimentoSala><salaId>102</salaId><atendimentosDiasSemana><AtendimentoDiaSemana><diaSemana>6</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>2</ofertaCursoCampiId><disciplinaId>16</disciplinaId><quantidade>27</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>4</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>5</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>2</ofertaCursoCampiId><disciplinaId>12</disciplinaId><quantidade>22</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>4</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>2</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>2</ofertaCursoCampiId><disciplinaId>11</disciplinaId><quantidade>20</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>3</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>1</diaSemana></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>0</diaSemana></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>3</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>2</ofertaCursoCampiId><disciplinaId>11</disciplinaId><quantidade>20</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>3</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana><AtendimentoDiaSemana><diaSemana>4</diaSemana><atendimentosTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>2</ofertaCursoCampiId><disciplinaId>6</disciplinaId><quantidade>60</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>2</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico><AtendimentoTatico><atendimentoOferta><AtendimentoOferta><ofertaCursoCampiId>1</ofertaCursoCampiId><disciplinaId>9</disciplinaId><quantidade>30</quantidade></AtendimentoOferta></atendimentoOferta><qtdeCreditosTeoricos>2</qtdeCreditosTeoricos><qtdeCreditosPraticos>0</qtdeCreditosPraticos></AtendimentoTatico></atendimentosTatico></AtendimentoDiaSemana></atendimentosDiasSemana></AtendimentoSala></atendimentosSalas></AtendimentoUnidade></atendimentosUnidades></AtendimentoCampus></atendimentos></TriedaOutput>";
	}
}
