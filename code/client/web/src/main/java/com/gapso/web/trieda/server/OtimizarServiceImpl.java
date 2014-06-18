package com.gapso.web.trieda.server;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.stream.XMLInputFactory;
import javax.xml.stream.XMLStreamReader;

import org.springframework.stereotype.Repository;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.DeslocamentoCampus;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.DivisaoCredito;
import com.gapso.trieda.domain.Equivalencia;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Parametro;
import com.gapso.trieda.domain.ParametroConfiguracao;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.RequisicaoOtimizacao;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Usuario;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.Grafo;
import com.gapso.web.trieda.server.util.Profundidade;
import com.gapso.web.trieda.server.util.SolverInput;
import com.gapso.web.trieda.server.util.SolverOutput;
import com.gapso.web.trieda.server.util.TriedaServerUtil;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclaration;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportFileWriter;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportListReader;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportListWriter;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportReader;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportWriter;
import com.gapso.web.trieda.server.util.solverclient.SolverClient;
import com.gapso.web.trieda.server.xml.input.TriedaInput;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoCampus;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoDiaSemana;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoHorarioAula;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoSala;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoTatico;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoTurno;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoUnidade;
import com.gapso.web.trieda.server.xml.output.ItemError;
import com.gapso.web.trieda.server.xml.output.ItemWarning;
import com.gapso.web.trieda.server.xml.output.TriedaOutput;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ErrorsWarningsInputSolverDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO;
import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO.StatusRequisicaoOtimizacao;
import com.gapso.web.trieda.shared.services.OtimizarService;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.dev.util.Pair;

@Transactional
@Service
@Repository
public class OtimizarServiceImpl extends RemoteService implements OtimizarService, ProgressDeclaration {
	private static final long serialVersionUID = 5716065588362358065L;
	private static final String solverName = "trieda";
	private static final String linkSolverDefault = "http://localhost:8080/SolverWS";
	private ProgressReportWriter progressReport;
	
	public void initProgressReport(String chave) {
		try {
			List<String> feedbackList = new ArrayList<String>();
			
			setProgressReport(feedbackList);
			ProgressReportReader progressSource = new ProgressReportListReader(feedbackList);
			progressSource.start();
			ProgressReportServiceImpl.getProgressReportSession(getThreadLocalRequest()).put(chave, progressSource);
			getProgressReport().start();
		}
		catch(Exception e){
			System.out.println("Nao foi possivel realizar o acompanhamento da progressao.");
		}
	}
	
	/**
	 * @see com.gapso.web.trieda.shared.services.OtimizarService#checkInputDataBeforeRequestOptimization(com.gapso.web.trieda.shared.dtos.ParametroDTO)
	 */
	@Override
	public ErrorsWarningsInputSolverDTO checkInputDataBeforeRequestOptimization(ParametroDTO parametroDTO) throws Exception {
		ErrorsWarningsInputSolverDTO response = new ErrorsWarningsInputSolverDTO();
		
		//Inicializa o relatorio de progresso
		initProgressReport("chaveOtimizacao");
		getProgressReport().setInitNewPartial("Iniciando checagem dos dados");
		
		
		if (parametroDTO.isValid()) {
			Parametro parametro = ConvertBeans.toParametro(parametroDTO);
			List<String> warnings = new ArrayList<String>();
			List<String> errors = new ArrayList<String>();
			
			// realiza verificações
			
			checkSemanasLetivas(parametro,warnings);
			
			System.out.print("Checando disciplinas sem curriculos");long start = System.currentTimeMillis(); // TODO: retirar
			checkDisciplinasSemCurriculo(parametro,warnings);
			long time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
			
			//System.out.print("Checando disciplinas sem laboratorios");start = System.currentTimeMillis(); // TODO: retirar
			checkDisciplinasSemLaboratorios(parametro,errors);
			//time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
			
			System.out.print("Checando disciplinas que nao exigem laboratorio associadas somente a laboratorios");start = System.currentTimeMillis(); // TODO: retirar
			checkDisciplinasComSomenteLaboratorios(parametro,warnings);
			time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
			
			System.out.print("Checando disciplinas com creditos zerados");start = System.currentTimeMillis(); // TODO: retirar
			checkDisciplinasComCreditosZerados(parametro,warnings);
			time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
			
			checkDisciplinasComMaxAlunosTeoricosZerados(parametro,errors);
			checkDisciplinasComMaxAlunosPraticosZerados(parametro,errors);
			checkDisciplinasComMaxAlunosDiferentes(parametro,errors);
			
			checkDivisoesCreditos(parametro,errors);
			
			if (ParametroDTO.OTIMIZAR_POR_BLOCO.equals(parametro.getOtimizarPor())) {
				System.out.print("Checando disciplinas repetidas por curriculo");start = System.currentTimeMillis(); // TODO: retirar
				checkMaxCreditosSemanaisPorPeriodo_e_DisciplinasRepetidasPorCurriculo(parametro,getInstituicaoEnsinoUser(),errors,warnings);
				time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
			} else {
				System.out.print("Checando creditos semanais e disciplinas repetidas por aluno");start = System.currentTimeMillis(); // TODO: retirar
				checkMaxCreditosSemanaisPorAluno_e_DisciplinasRepetidasPorAluno(parametro,getInstituicaoEnsinoUser(),errors,warnings);
				time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
			}

			System.out.print("Checando demandas com disciplinas sem curriculo");start = System.currentTimeMillis(); // TODO: retirar
			checkDemandasComDisciplinasSemCurriculo(parametro,errors);
			time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
			
			System.out.print("Checando professoress com Carga Horária Máxima zerada");start = System.currentTimeMillis(); // TODO: retirar
			checkProfessorComCargaHorariaMaximaZerada(parametro,errors, warnings);
			time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar

			checkOfertaComCargaReceitaCreditoZerada(parametro,errors, warnings);
			checkExigeEquivalenciaForcadaAlunosDemanda(parametro,getInstituicaoEnsinoUser(),errors);
			
//			System.out.print("checkSemanasLetivasIncompativeis(parametro,errors);");start = System.currentTimeMillis(); // TODO: retirar
//			checkSemanasLetivasIncompativeis(parametro, errors);
//			time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
			
			if (parametro.getConsiderarEquivalencia()) {
				if (parametro.getProibirTrocaPorDiscOnlineOuCredZeradosEmEquivalencia()) {
					System.out.print("Checando equivalencias que levam para disciplinas online ou sem creditos");start = System.currentTimeMillis(); // TODO: retirar
					checkEquivalenciasQueLevamParaDisciplinasOnlineOuSemCreditos(parametro.getCenario(),errors);
					time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
				}
				
				System.out.print("Checando ciclo de disciplinas equivalentes");start = System.currentTimeMillis(); // TODO: retirar
				boolean detectouCiclo = checkCicloDisciplinasEquivalentes(parametro.getCenario(),(parametro.getProibirCiclosEmEquivalencia() ? errors : warnings));
				time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
				
				if (!detectouCiclo || !parametro.getConsiderarTransitividadeEmEquivalencia()) {
					if (ParametroDTO.OTIMIZAR_POR_BLOCO.equals(parametro.getOtimizarPor())) {
						System.out.print("Checando equivalencias que geram disciplinas repetidas em um mesmo curriculo");start = System.currentTimeMillis(); // TODO: retirar
						checkEquivalenciasQueGeramDisciplinasRepetidasEmUmMesmoCurriculo(parametro,warnings);
						time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
					} else {
						System.out.print("Checando equivalencias que geram disciplinas repetidas em um aluno");start = System.currentTimeMillis(); // TODO: retirar
						checkEquivalenciasQueGeramDisciplinasRepetidasEmUmAluno(parametro,warnings);
						time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
					}
				}
			}
			
			if (parametro.getProfessorEmMuitosCampi()) {
				checkCampiSemDeslocamentos(parametro,errors);
			}
			checksCleiton();//TODO: revisar
			
			response.setErrors(errors);
			response.setWarnings(warnings);
		} else {
			String message = "";
			if (parametroDTO.getCampi() == null || parametroDTO.getCampi().isEmpty()) {
				message += HtmlUtils.htmlUnescape("O campus n&atilde;o foi informado.");
			}
			if (parametroDTO.getTurnos().isEmpty()) {
				if (!message.isEmpty()) {
					message += " ";
				}
				message += HtmlUtils.htmlUnescape("O turno n&atilde;o foi informado.");
			}
			
			List<String> errors = new ArrayList<String>();
			errors.add(message);
			
			response.setErrors(errors);
		}
		
		getProgressReport().setPartial("Etapa concluída");
		if (response.getTotalErrorsWarnings() > 0) {
			getProgressReport().finish();
		}
		return response;
	}
	
	private void checkExigeEquivalenciaForcadaAlunosDemanda(
			Parametro parametro, InstituicaoEnsino instituicaoEnsino, List<String> errors) {
		
		// obtém os alunos do campus selecionado para otimização
		List<AlunoDemanda> demandasDeAlunoDosCampiSelecionados = AlunoDemanda.findByCampusAndTurno(instituicaoEnsino,parametro.getCampi(),parametro.getTurnos());
		
		for(AlunoDemanda  alunoDemanda : demandasDeAlunoDosCampiSelecionados){
			if(alunoDemanda.getExigeEquivalenciaForcada()){
				if(alunoDemanda.getDemanda().getDisciplina().getEliminadasPor().isEmpty()){
					errors.add(HtmlUtils.htmlUnescape("A demanda do aluno ["+alunoDemanda.getAluno().getMatricula() +
							"] pela disciplina ["+alunoDemanda.getDemanda().getDisciplina().getCodigo() +
							"] exige uma equivalência forçada, porém não há regras de equivalencia " +
							" aplicaveis à discipina ["+alunoDemanda.getDemanda().getDisciplina().getCodigo()+ "]"));
					// Matricula do aluno, Codigo da disciplina
					// A demanda do aluno [matricula] pela disciplina [codigo] exige uma equivalencia forçada, parém não há regras de equivalencia
					// aplicaveis à discipina [código]
				}
			}
		}
		
		
	}

	private void checkDivisoesCreditos(Parametro parametro, List<String> errors) {
		
		Set< DivisaoCredito > divisoesCredito = parametro.getCenario().getDivisoesCredito();
		
		for(DivisaoCredito divisaoCredito : divisoesCredito ){
			int somaCreditos = divisaoCredito.getDia1() + divisaoCredito.getDia2() + divisaoCredito.getDia3()
					+ divisaoCredito.getDia4() + divisaoCredito.getDia5() + divisaoCredito.getDia6()
					+ divisaoCredito.getDia7();
			if (divisaoCredito.getCreditos() != somaCreditos)
			{
				errors.add(HtmlUtils.htmlUnescape("A regra genérica de divisão de créditos de [" +
						divisaoCredito.getCreditos() + 
						"] créditos na forma " + formataDivisoesCredito(divisaoCredito) +
						" está com a soma diferente de ["+divisaoCredito.getCreditos()+"]."));
			}
		}

		
		
		for(Disciplina disciplina : parametro.getCenario().getDisciplinas()){
			DivisaoCredito divisaoCredito = disciplina.getDivisaoCreditos(); 
			
			if(divisaoCredito != null){
			
				int somaCreditos = divisaoCredito.getDia1() + divisaoCredito.getDia2() + divisaoCredito.getDia3()
						+ divisaoCredito.getDia4() + divisaoCredito.getDia5() + divisaoCredito.getDia6()
						+ divisaoCredito.getDia7();
				if (divisaoCredito.getCreditos() != somaCreditos)
				{
					errors.add(HtmlUtils.htmlUnescape("A regra de divisão de créditos de  [" +
							divisaoCredito.getCreditos() + 
							"] créditos na forma " + formataDivisoesCredito(divisaoCredito) +
							" da disciplina [" + disciplina.getCodigo() + 
							"] está com a soma diferente de ["+divisaoCredito.getCreditos()+"]."));
				}
			}
		}
		
		
	}

	private String formataDivisoesCredito(DivisaoCredito divisaoCredito) {
		List<String> output = new ArrayList<String>();
			
		output.add((divisaoCredito.getDia1()==0)?"_":divisaoCredito.getDia1().toString());
		output.add((divisaoCredito.getDia2()==0)?"_":divisaoCredito.getDia2().toString());
		output.add((divisaoCredito.getDia3()==0)?"_":divisaoCredito.getDia3().toString());
		output.add((divisaoCredito.getDia4()==0)?"_":divisaoCredito.getDia4().toString());
		output.add((divisaoCredito.getDia5()==0)?"_":divisaoCredito.getDia5().toString());
		output.add((divisaoCredito.getDia6()==0)?"_":divisaoCredito.getDia6().toString());
		output.add((divisaoCredito.getDia7()==0)?"_":divisaoCredito.getDia7().toString());
		
		return output.toString();
	}

	/** 
	 * @see com.gapso.web.trieda.shared.services.OtimizarService#registraRequisicaoDeOtimizacao(com.gapso.web.trieda.shared.dtos.ParametroDTO, java.lang.Long)
	 */
	@Override
	public RequisicaoOtimizacaoDTO registraRequisicaoDeOtimizacao(ParametroDTO parametroDTO, Long round) throws TriedaException {
		try {
			Usuario usuarioAtual = getUsuario();
			
			Parametro parametro = ConvertBeans.toParametro(parametroDTO);
			if (parametro.getId() == null) {
				// se entrou aqui é porque foi realizada uma requisição de otimização numa situação em que não havia nenhum parâmetro cadastrado no BD
				List<Parametro> parametros = Parametro.findAll(parametro.getModoOtimizacao(),parametro.getOtimizarPor(),parametro.getFuncaoObjetivo(),parametro.getCenario(),parametro.getInstituicaoEnsino());
				Long maxId = -1L;
				Set<Campus> campi = parametro.getCampi();
				Set<Turno> turnos = parametro.getTurnos();
				for (Parametro p : parametros) {
					if (p.getId() > maxId && campi.containsAll(p.getCampi()) && turnos.containsAll(p.getTurnos())) {
						maxId = p.getId();
						parametro = p;
					}
				}
			}
			
			Cenario cenario = parametro.getCenario();
			
			RequisicaoOtimizacao reqOtm = new RequisicaoOtimizacao();
			reqOtm.setCenario(cenario);
			reqOtm.setRound(round);
			reqOtm.setUsuario(usuarioAtual);
			reqOtm.setParametro(parametro);
			
			reqOtm.persist();
			
			return ConvertBeans.toRequisicaoOtimizacaoDTO(reqOtm);
		} catch (Exception e) {
			throw new TriedaException(e);
		}
	}
	
	/**
	 * @see com.gapso.web.trieda.shared.services.OtimizarService#removeRequisicaoDeOtimizacao(com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO)
	 */
	@Override
	public void removeRequisicaoDeOtimizacao(RequisicaoOtimizacaoDTO requisicaoDTO) throws TriedaException {
		try {
			RequisicaoOtimizacao reqOtm = RequisicaoOtimizacao.entityManager().find(RequisicaoOtimizacao.class,requisicaoDTO.getId());
			reqOtm.remove();
		} catch (Exception e) {
			throw new TriedaException(e);
		}
	}
	
	/** 
	 * @see com.gapso.web.trieda.shared.services.OtimizarService#removeRequisicoesDeOtimizacao(java.util.List)
	 */
	@Override
	public void removeRequisicoesDeOtimizacao(List<RequisicaoOtimizacaoDTO> requisicoesASeremRemovidas) throws TriedaException {
		try {
			List<Long> ids = new ArrayList<Long>(requisicoesASeremRemovidas.size());
			for (RequisicaoOtimizacaoDTO req : requisicoesASeremRemovidas) {
				ids.add(req.getId());
			}
			
			List<RequisicaoOtimizacao> requisicoes = RequisicaoOtimizacao.findBy(ids);
			for (RequisicaoOtimizacao req : requisicoes) {
				req.remove();
			}
		} catch (Exception e) {
			throw new TriedaException(e);
		}
	}
	
	/**
	 * @see com.gapso.web.trieda.shared.services.OtimizarService#getParametrosDaRequisicaoDeOtimizacao(com.gapso.web.trieda.shared.dtos.CenarioDTO)
	 */
	@Override
	@Transactional
	public ParametroDTO getParametrosDaRequisicaoDeOtimizacao(CenarioDTO cenarioDTO) {
		InstituicaoEnsino instituicaoEnsino = this.getInstituicaoEnsinoUser();
		Cenario cenario = Cenario.find(cenarioDTO.getId(),instituicaoEnsino);
		Parametro parametro = cenario.getUltimoParametro(instituicaoEnsino);

		if (parametro == null) {
			parametro = this.getParametroDefault(instituicaoEnsino,cenario);
		}

		return ConvertBeans.toParametroDTO(parametro);
	}
	
	@Override
	@Transactional
	public ParDTO<Long,ParametroDTO> enviaRequisicaoDeOtimizacao(ParametroDTO parametroDTO) throws TriedaException {
		TriedaException exception = null;
		
		try {
			if (!parametroDTO.isValid()) {

				String errorMessage = "";
				if (parametroDTO.getCampi() == null || parametroDTO.getCampi().isEmpty()) {
					errorMessage += HtmlUtils.htmlUnescape("Nenhum campus foi selecionado.");
				}
				if (parametroDTO.getTurnos().isEmpty()) {
					errorMessage += HtmlUtils.htmlUnescape(" Nenhum turno foi selecionado.");
				}
				
				exception = new TriedaException(errorMessage);
			} else {
				ParDTO<Boolean,String> parDTOConflito = checkExistenciaRequisicaoOtimizacaoConflitanteEmAndamento(parametroDTO);
				boolean haConflito = parDTOConflito.getPrimeiro();
				if (!haConflito) {
					
					if (getProgressReport().getStatus() == 0) {
						initProgressReport("chaveOtimizacaoReq");
					}
					getProgressReport().setInitNewPartial("Enviando requisicao de otimizacao");
					
					Parametro parametro = ConvertBeans.toParametro(parametroDTO);
			
					parametro.setId(null);
					parametro.flush();
					parametro.save();
			
					Cenario cenario = parametro.getCenario();
					cenario.getParametros().add(parametro);
					List<Campus> campi = new ArrayList<Campus>(parametro.getCampi().size());
					campi.addAll(parametro.getCampi());
			
					SolverInput solverInput = new SolverInput(getInstituicaoEnsinoUser(),cenario,parametro,campi);
					TriedaInput triedaInput = null;
					if (parametro.isTatico()) {
						triedaInput = solverInput.generateTaticoTriedaInput();
					} else {
						triedaInput = solverInput.generateOperacionalTriedaInput();
					}
			
					final ByteArrayOutputStream temp = new ByteArrayOutputStream();
					JAXBContext jc = JAXBContext.newInstance("com.gapso.web.trieda.server.xml.input");
					Marshaller m = jc.createMarshaller();
					m.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT,true);
					m.setProperty(Marshaller.JAXB_ENCODING,"ISO-8859-1");
					m.marshal(triedaInput,temp);
					byte [] fileBytes = temp.toByteArray();
					ParametroConfiguracao config = ParametroConfiguracao.findConfiguracoes(getInstituicaoEnsinoUser());
					SolverClient solverClient = new SolverClient(config.getUrlOtimizacao(),config.getNomeOtimizacao());
			
					Long round = solverClient.requestOptimization(fileBytes);
					
					return ParDTO.<Long,ParametroDTO>create(round,ConvertBeans.toParametroDTO(parametro));
				} else {
					String msgConflito = parDTOConflito.getSegundo();
					exception = new TriedaException(HtmlUtils.htmlUnescape(msgConflito));
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
			exception = new TriedaException(e);
		}
		
		getProgressReport().setPartial("Etapa concluída");
		getProgressReport().finish();
		
		throw exception;
	}

	private ParDTO<Boolean,String> checkExistenciaRequisicaoOtimizacaoConflitanteEmAndamento(ParametroDTO parametroDTO) throws TriedaException {
		boolean haConflito = false;
		String msgConflito = "";
		Set<Long> professoresRelacionadosIDsRequisicaoAtual = null;
		
		// obtém as requisições de otimização
		List<RequisicaoOtimizacaoDTO> requisicoesOtimizacaoDTO = consultaRequisicoesDeOtimizacao(false);
		// verifica se há alguma requisição de otimização em andamento que tenha conflito com a nova requisição
		boolean haIntersecaoDeCampi = false;
		boolean haIntersecaoDeProfessor = false;
		RequisicaoOtimizacaoDTO requisicaoConflitanteEmAndamento = null;
		for (RequisicaoOtimizacaoDTO reqOtmDTO : requisicoesOtimizacaoDTO) {
			StatusRequisicaoOtimizacao status = StatusRequisicaoOtimizacao.values()[reqOtmDTO.getStatusIndex()];
			if (StatusRequisicaoOtimizacao.EM_ANDAMENTO.equals(status)) {
				// VERIFICAÇÃO 1
				// verifica se há interseção de campi selecionados
				String campiSelecionados = reqOtmDTO.getCampiSelecionados();
				for (CampusDTO campusDTO : parametroDTO.getCampi()) {
					if (campiSelecionados.contains(campusDTO.getCodigo())) {
						haIntersecaoDeCampi = true;
						requisicaoConflitanteEmAndamento = reqOtmDTO;
						break;
					}
				}
				
				// VERIFICAÇÃO 2
				// verifica se há requisições com modo de otimização operacional e, em caso afirmativo, se há
				// recursos compartilhados entre as requisições
				if (!haIntersecaoDeCampi && Parametro.OPERACIONAL.equals(parametroDTO.getModoOtimizacao())) {
					if (Parametro.OPERACIONAL.equals(reqOtmDTO.getModoOtimizacao())) {
						// se necessário, obtém professores relacionados com a requisição de otimização atual
						if (professoresRelacionadosIDsRequisicaoAtual == null) {
							List<Long> campiIDs = new ArrayList<Long>(parametroDTO.getCampi().size());
							for (CampusDTO campusDTO : parametroDTO.getCampi()) {
								campiIDs.add(campusDTO.getId());
							}
							List<Campus> campi = Campus.find(campiIDs,getInstituicaoEnsinoUser());
							professoresRelacionadosIDsRequisicaoAtual = new HashSet<Long>();
							for (Campus campus : campi) {
								for (Professor professor : campus.getProfessores()) {
									professoresRelacionadosIDsRequisicaoAtual.add(professor.getId());
								}
							}
						}
						// verifica se há professores em comum
						for (Long professorId : professoresRelacionadosIDsRequisicaoAtual) {
							if (reqOtmDTO.getProfessoresRelacionadosIDs().contains(professorId)) {
								haIntersecaoDeProfessor = true;
								requisicaoConflitanteEmAndamento = reqOtmDTO;
								break;
							}
						}
					}
				} else {
					break;
				}
				
				if (haIntersecaoDeProfessor) {
					break;
				}
			}
		}
		
		if (haIntersecaoDeCampi || haIntersecaoDeProfessor) {
			msgConflito = "No momento não será possível tratar a requisição de otimização para";
			
			if (parametroDTO.getCampi().size() == 1) {
				msgConflito += " o campus [";
			} else {
				msgConflito += " os campi [";
			}
			
			for (CampusDTO campusDTO : parametroDTO.getCampi()) {
				msgConflito += campusDTO.getCodigo() + ", ";
			}
			msgConflito = msgConflito.substring(0,msgConflito.length()-2) + "]";
			
			if (haIntersecaoDeCampi) {
				msgConflito += " pois há outra requisição em andamento para";
				
				if (!requisicaoConflitanteEmAndamento.getCampiSelecionados().contains(",")) {
					msgConflito += " o campus [";
				} else {
					msgConflito += " os campi [";
				}
				
				msgConflito += requisicaoConflitanteEmAndamento.getCampiSelecionados() + "].";
			} else {
				msgConflito += " pois há outra requisição em andamento que leva em consideração professores associados com a requisição atual.";
			}
			
			haConflito = true;
		}
		
		return ParDTO.<Boolean,String>create(haConflito,msgConflito);
	}
	
	/** 
	 * @see com.gapso.web.trieda.shared.services.OtimizarService#consultaRequisicoesDeOtimizacao()
	 */
	@Override
	public List<RequisicaoOtimizacaoDTO> consultaRequisicoesDeOtimizacao() throws TriedaException {
		try {
			return consultaRequisicoesDeOtimizacao(true);
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
	}
	
	private List<RequisicaoOtimizacaoDTO> consultaRequisicoesDeOtimizacao(boolean onlyCurrentUser) {
		// obtém o usuário logado em questão
		Usuario usuarioAtual = getUsuario();
		// obtém as requisições de otimização relacionadas com o usuário atual
		List<RequisicaoOtimizacao> requisicoesOtimizacao = onlyCurrentUser ? RequisicaoOtimizacao.findBy(usuarioAtual) : RequisicaoOtimizacao.findAll();
		// ...
		List<RequisicaoOtimizacaoDTO> requisicoesOtimizacaoDTOs = new ArrayList<RequisicaoOtimizacaoDTO>();
		ParametroConfiguracao config = ParametroConfiguracao.findConfiguracoes(getInstituicaoEnsinoUser());
		SolverClient solverClient = new SolverClient(config.getUrlOtimizacao(),config.getNomeOtimizacao());
		for (RequisicaoOtimizacao requisicaoOtimizacao : requisicoesOtimizacao) {
			RequisicaoOtimizacaoDTO dto = ConvertBeans.toRequisicaoOtimizacaoDTO(requisicaoOtimizacao);
			
			if (solverClient.isFinished(requisicaoOtimizacao.getRound())) {
				if (solverClient.containsResult(requisicaoOtimizacao.getRound())) {
					dto.setStatusIndex(StatusRequisicaoOtimizacao.FINALIZADA_COM_OUTPUT.ordinal());
				} else {
					dto.setStatusIndex(StatusRequisicaoOtimizacao.FINALIZADA_SEM_OUTPUT.ordinal());
				}
			} else {
				dto.setStatusIndex(StatusRequisicaoOtimizacao.EM_ANDAMENTO.ordinal());
			}
			
			requisicoesOtimizacaoDTOs.add(dto);
		}
		
		return requisicoesOtimizacaoDTOs;
	}
	
	/**
	 * @see com.gapso.web.trieda.shared.services.OtimizarService#cancelaRequisicaoDeOtimizacao(java.lang.Long)
	 */
	@Override
	public boolean cancelaRequisicaoDeOtimizacao(Long round) throws TriedaException {
		try {
			ParametroConfiguracao config = ParametroConfiguracao.findConfiguracoes(getInstituicaoEnsinoUser());
			SolverClient solverClient = new SolverClient(config.getUrlOtimizacao(),config.getNomeOtimizacao());
			return solverClient.cancelOptimization(round);
		} catch(Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
	}

	private void checksCleiton() {
//		// SEGUNDA VERIFICAÇÃO
//
//		// Fixação de disciplina com qtde de horários
//		// menores do que a qtde de créditos da disciplinas
//		List< Fixacao > fixacoes
//			= Fixacao.findAll( this.instituicaoEnsino );
//
//		for ( Disciplina disciplinaCenario : this.cenario.getDisciplinas() )
//		{
//			for ( Fixacao fixacao : fixacoes )
//			{
//				Integer totalCreditos = this.getHorarios( fixacao  ).size();
//
//				Disciplina disciplinaFixacao = fixacao.getDisciplina();
//
//				if ( totalCreditos != null && totalCreditos > 0
//					&& disciplinaFixacao != null
//					&& disciplinaFixacao.getId() == disciplinaCenario.getId()
//					&& totalCreditos < disciplinaCenario.getTotalCreditos() )
//				{
//					String warningMessage = "A disciplina " + disciplinaCenario.getCodigo()
//						+ ", que possui " + disciplinaCenario.getTotalCreditos() + " cr&eacute;ditos,"
//						+ " tem uma fixa&ccedil;&atilde;o de apenas " + totalCreditos.toString() + ".";
//
//					createWarningMessage( warningMessage );
//
//					break;
//				}
//			}
//		}
//
//		// TERCEIRA VERIFICAÇÃO
//
//		// Impossibilidade de associar uma regra
//		// genérica de divisão de créditos a uma disciplina
//		Set< DivisaoCredito > regras
//			= this.cenario.getDivisoesCredito();
//
//		if ( regras.size() > 0 )
//		{
//			for ( Disciplina disciplinaCenario : this.cenario.getDisciplinas() )
//			{
//				int creditosDisciplina = disciplinaCenario.getTotalCreditos();
//
//				boolean encontrou = false;
//
//				for ( DivisaoCredito divisaoCredito : regras )
//				{
//					if ( divisaoCredito.getCreditos() >= creditosDisciplina )
//					{
//						encontrou = true;
//						break;
//					}
//				}
//
//				if ( !encontrou )
//				{
//					String warningMessage = "A disciplina " + disciplinaCenario.getCodigo()
//						+ " n&atilde;o possui nenhuma regra de cr&eacute;ditos gen&eacute;rica "
//						+ "que atenda ao total de cr&eacute;ditos da disciplina.";
//
//					createWarningMessage( warningMessage );
//				}
//			}
//		}
	}
	
	private void checkSemanasLetivas(Parametro parametro, List<String> warnings){
		Cenario cenario = parametro.getCenario();
		Set<SemanaLetiva> semanasLetivas = cenario.getSemanasLetivas();
		List<HorarioDisponivelCenario> horariosAulasAux = new ArrayList<HorarioDisponivelCenario>();
		
		if(horariosAulasAux != null){ 
		if(!semanasLetivas.isEmpty()){
			for(SemanaLetiva semanaLetiva : semanasLetivas){
				if(!semanaLetiva.getHorariosAula().isEmpty()){
					for(HorarioAula horarioAula : semanaLetiva.getHorariosAula()){
						if(!horarioAula.getHorariosDisponiveisCenario().isEmpty()){
							for(HorarioDisponivelCenario hdc : horarioAula.getHorariosDisponiveisCenario()){
								horariosAulasAux.add(hdc);
							}
							
						}
						else{
							warnings.add(HtmlUtils.htmlUnescape("Não é possível prosseguir com a otimização, pois, nas semanas letivas do cenário " + cenario.getNome()  + " não foram cadastros os possíveis dias e horários de aula."));
						}
					}
				} else {
					warnings.add(HtmlUtils.htmlUnescape("Não é possível prosseguir com a otimização, pois, nas semanas letivas do cenário " + cenario.getNome()  + " não foram cadastros os possíveis dias e horários de aula."));
				}
			}
		} else {
			warnings.add(HtmlUtils.htmlUnescape("Não é possível prosseguir com a otimização, pois, não há nenhuma semana letiva cadastrada no cenário [" + cenario.getNome() + " ]."));
		}
		
		List<HorarioDisponivelCenario> horariosDisponiveisAll = HorarioDisponivelCenario.findAll(getInstituicaoEnsinoUser());
		Map<Long, List<HorarioDisponivelCenario>> horariosDisponiveisMap = new HashMap<Long, List<HorarioDisponivelCenario>>();
		
		for(HorarioDisponivelCenario hdc : horariosDisponiveisAll){
			for(Disciplina disciplina : hdc.getDisciplinas()){
				if(horariosDisponiveisMap.get(disciplina.getId()) == null){
					List<HorarioDisponivelCenario> horariosDisponiveisNew = new ArrayList<HorarioDisponivelCenario>();
					horariosDisponiveisNew.add(hdc);
					horariosDisponiveisMap.put(disciplina.getId(), horariosDisponiveisNew);
				} else {
					horariosDisponiveisMap.get(disciplina.getId()).add(hdc);
				}
					
			}
		}
		
		for (Disciplina disciplina : cenario.getDisciplinas()) {
			List<HorarioDisponivelCenario> horariosDisponiveisBD = horariosDisponiveisMap.get(disciplina.getId());
			
			if(horariosDisponiveisBD != null){
			// obtém as semanas letivas associadas com a disciplina em questão
				Set<SemanaLetiva> semanasLetivasDis = disciplina.getSemanasLetivas();
				// se necessário, filtra os horários discponíveis
				if (!semanasLetivasDis.isEmpty()) {
					// filtra os horários disponíveis da disciplina de acordo com as semanas letivas associadas com a disciplina
					List<HorarioDisponivelCenario> horariosDisponiveisBDFiltrados = new ArrayList<HorarioDisponivelCenario>();
					for (HorarioDisponivelCenario horario : horariosDisponiveisBD) {
						if (semanasLetivasDis.contains(horario.getHorarioAula().getSemanaLetiva())) {
							horariosDisponiveisBDFiltrados.add(horario);
						}
					}
					
					horariosDisponiveisBD.clear();
					horariosDisponiveisBD.addAll(horariosDisponiveisBDFiltrados);
				}
			if(!disciplina.getUsaSabado()){
				for(HorarioDisponivelCenario hdc : horariosDisponiveisBD){
					if(Semanas.SAB.equals(hdc.getDiaSemana())){
						warnings.add(HtmlUtils.htmlUnescape("A disciplina [" + disciplina.getNome() + "], cujo valor do atributo Usa Sábado = Não, está com disponibilidade no Sábado"));
					}
				}
			} else {
				boolean hasSat = false;
				for(HorarioDisponivelCenario hdc : horariosDisponiveisBD){
					if(Semanas.SAB.equals(hdc.getDiaSemana())){
						hasSat = true;
					}
				}
				if(hasSat){
					warnings.add(HtmlUtils.htmlUnescape("A disciplina [" + disciplina.getNome() + "], cujo valor do atributo Usa Sábado = Sim, não está com disponibilidade no Sábado"));
				}
			}
			if(!disciplina.getUsaDomingo()){
				for(HorarioDisponivelCenario hdc : horariosDisponiveisBD){
					if(Semanas.DOM.equals(hdc.getDiaSemana())){
						warnings.add(HtmlUtils.htmlUnescape("A disciplina [" + disciplina.getNome() + "], cujo valor do atributo Usa Domingo = Não, está com disponibilidade no Sábado"));
					}
				}
			} else {
				boolean hasSat = false;
				for(HorarioDisponivelCenario hdc : horariosDisponiveisBD){
					if(Semanas.DOM.equals(hdc.getDiaSemana())){
						hasSat = true;
					}
				}
				if(hasSat){
					warnings.add(HtmlUtils.htmlUnescape("A disciplina [" + disciplina.getNome() + "], cujo valor do atributo Usa Domingo = Sim, não está com disponibilidade no Sábado"));
				}
			}
		}
		}
		}
	}

	private void checkDisciplinasSemCurriculo(Parametro parametro, List<String> warnings) {
		Map<Long,Long> disciplinasComCurriculoMap = new HashMap<Long,Long>();
		for (Curso curso : parametro.getCenario().getCursos()) {
			for (Curriculo curriculo : curso.getCurriculos()) {
				for (CurriculoDisciplina curriculoDisciplina : curriculo.getDisciplinas()) {
					disciplinasComCurriculoMap.put(curriculoDisciplina.getDisciplina().getId(),curriculoDisciplina.getDisciplina().getId());
				}
			}
		}

		List<String> disciplinasSemCurriculo = new ArrayList<String>();
		for (Disciplina disciplina : parametro.getCenario().getDisciplinas()) {
			if (disciplinasComCurriculoMap.get(disciplina.getId()) == null) {
				disciplinasSemCurriculo.add(disciplina.getCodigo());
			}
		}
		
		if (!disciplinasSemCurriculo.isEmpty()) {
			warnings.add(HtmlUtils.htmlUnescape("As disciplinas " + disciplinasSemCurriculo + " n&atilde;o estão associadas a nenhuma matriz curricular."));
		}
	}

	private void checkDisciplinasSemLaboratorios(Parametro parametro, List<String> errors) {
		for (Campus campus : parametro.getCampi()) {
			checkDisciplinasSemLaboratorios(campus,errors);
		}
	}
	
	private void checkDisciplinasSemLaboratorios(Campus campus, List<String> errors) {
		// coleta as disciplinas que serão enviadas para o solver e que exigem laboratório
		
		System.out.print("for 1");long start = System.currentTimeMillis(); // TODO: retirar
		
		Set<Disciplina> disciplinasQueExigemLaboratio = new HashSet<Disciplina>();
		for (Oferta oferta : campus.getOfertas()) {
			for (Demanda demanda : oferta.getDemandas()) {
				Disciplina disciplina = demanda.getDisciplina();
				if (disciplina.getLaboratorio()) {
					disciplinasQueExigemLaboratio.add(disciplina);
				}
			}
		}
		
		System.out.println(disciplinasQueExigemLaboratio.size()+ " tamanhoDisciplinasQueExigemLaboratio");
		
		long time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
		
		System.out.print("for 2");start = System.currentTimeMillis(); // TODO: retirar
		
		// verifica se há disciplinas que exigem laboratório, porém, não estão associadas a laboratórios
		for (Disciplina disciplina : disciplinasQueExigemLaboratio) {
			// coleta os CurriculoDisciplina não associados a laboratórios
			Set<CurriculoDisciplina> curriculosDisciplinasNaoAssociadosALaboratorios = new HashSet<CurriculoDisciplina>();
			for (CurriculoDisciplina curriculoDisciplina : disciplina.getCurriculos()) {
				// apenas faz a validação para curriculos com alguma oferta
				if (!curriculoDisciplina.getCurriculo().getOfertas().isEmpty()) {
					// verifica se tem oferta no campus a ser otimizado
					boolean curriculoTemOfertaNoCampusASerOtimizado = false;
					for (Oferta oferta : curriculoDisciplina.getCurriculo().getOfertas()) {
						if (oferta.getCampus().equals(campus)) {
							curriculoTemOfertaNoCampusASerOtimizado = true;
							break;
						}
					}
					
					// apenas faz validação se currículo tem oferta no campus a ser otimizado
					if (curriculoTemOfertaNoCampusASerOtimizado) {
						boolean estaAssociadoComAlgumLaboratorio = false;
						for (Sala sala : curriculoDisciplina.getDisciplina().getSalas()) {
							if (sala.isLaboratorio() && sala.getUnidade().getCampus().equals(campus)) {
								estaAssociadoComAlgumLaboratorio = true;
								break;
							}
						}
						 
						if (!estaAssociadoComAlgumLaboratorio) {
							curriculosDisciplinasNaoAssociadosALaboratorios.add(curriculoDisciplina);
						}
					}
				}
			}
			 
			if (!curriculosDisciplinasNaoAssociadosALaboratorios.isEmpty()) {
				String pares = "";
				for (CurriculoDisciplina curriculoDisciplina : curriculosDisciplinasNaoAssociadosALaboratorios) {
					pares += "(" + curriculoDisciplina.getCurriculo().getCodigo() + "," + curriculoDisciplina.getPeriodo() + "); ";
				}
				errors.add(HtmlUtils.htmlUnescape("A disciplina [" + disciplina.getCodigo() + "], que exige laboratório, contém pares (Matriz Curricular, Período) não associados a nenhum laboratório do campus [" + campus.getCodigo() + ", são eles: " + pares));
			}
		}
		time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
	}
	
	private void checkDisciplinasComSomenteLaboratorios(Parametro parametro, List<String> warnings) {
		for (Campus campus : parametro.getCampi()) {
			checkDisciplinasComSomenteLaboratorios(campus,warnings);
		}
	}
	
	private void checkDisciplinasComSomenteLaboratorios(Campus campus, List<String> warnings) {
		// coleta as disciplinas que serão enviadas para o solver e que nao exigem laboratório
		Set<Disciplina> disciplinasQueNaoExigemLaboratorio = new HashSet<Disciplina>();
		for (Oferta oferta : campus.getOfertas()) {
			for (Demanda demanda : oferta.getDemandas()) {
				Disciplina disciplina = demanda.getDisciplina();
				if (!disciplina.getLaboratorio()) {
					disciplinasQueNaoExigemLaboratorio.add(disciplina);
				}
			}
		}
		
		// verifica se há disciplinas que não exigem laboratório, porém, estão associadas somente a laboratórios
		for (Disciplina disciplina : disciplinasQueNaoExigemLaboratorio) {
			// coleta os CurriculoDisciplina associados somente a laboratórios
			Set<CurriculoDisciplina> curriculosDisciplinasAssociadosSomenteALaboratorios = new HashSet<CurriculoDisciplina>();
			for (CurriculoDisciplina curriculoDisciplina : disciplina.getCurriculos()) {
				// apenas faz a validação para curriculos com alguma oferta
				if (!curriculoDisciplina.getCurriculo().getOfertas().isEmpty()) {
					// verifica se tem oferta no campus a ser otimizado
					boolean curriculoTemOfertaNoCampusASerOtimizado = false;
					for (Oferta oferta : curriculoDisciplina.getCurriculo().getOfertas()) {
						if (oferta.getCampus().equals(campus)) {
							curriculoTemOfertaNoCampusASerOtimizado = true;
							break;
						}
					}
					
					// apenas faz validação se currículo tem oferta no campus a ser otimizado
					if (curriculoTemOfertaNoCampusASerOtimizado) {
						boolean estaAssociadoSomenteComLaboratorios = true;
						for (Sala sala : curriculoDisciplina.getDisciplina().getSalas()) {
							if (!sala.isLaboratorio() && sala.getUnidade().getCampus().equals(campus)) {
								estaAssociadoSomenteComLaboratorios = false;
								break;
							}
						}
						 
						if (estaAssociadoSomenteComLaboratorios && !curriculoDisciplina.getDisciplina().getSalas().isEmpty()) {
							curriculosDisciplinasAssociadosSomenteALaboratorios.add(curriculoDisciplina);
						}
					}
				}
			}
			 
			if (!curriculosDisciplinasAssociadosSomenteALaboratorios.isEmpty()) {
				String pares = "";
				for (CurriculoDisciplina curriculoDisciplina : curriculosDisciplinasAssociadosSomenteALaboratorios) {
					pares += "(" + curriculoDisciplina.getCurriculo().getCodigo() + "," + curriculoDisciplina.getPeriodo() + "); ";
				}
				warnings.add(HtmlUtils.htmlUnescape("A disciplina [" + disciplina.getCodigo() + "], que nao exige laboratório, contém pares (Matriz Curricular, Período) associados a somente laboratórios [" + campus.getCodigo() + ", são eles: " + pares));
			}
		}
	}
	
	private void checkDisciplinasComCreditosZerados(Parametro parametro, List<String> warnings) {
		// colhe as disciplinas de acordo com os campi selecionados para otimização
		Set<Disciplina> disciplinasSelecionadas = new HashSet<Disciplina>();
		for (Campus campus : parametro.getCampi()) {
			for (Oferta oferta : campus.getOfertas()) {
				for (Demanda demanda : oferta.getDemandas()) {
					disciplinasSelecionadas.add(demanda.getDisciplina());
				}
			}
		}
		
		List<String> disciplinasComCreditosZerados = new ArrayList<String>();
		for (Disciplina disciplina : disciplinasSelecionadas) {
			if (disciplina.getCreditosTotal() == 0) {
				disciplinasComCreditosZerados.add(disciplina.getCodigo());
			}
		}
		
		if (!disciplinasComCreditosZerados.isEmpty()) {
			warnings.add(HtmlUtils.htmlUnescape("A(s) disciplina(s) " + disciplinasComCreditosZerados + " apresentam total de créditos igual a zero e, por isso, serão desconsideradas da otimização."));
		}
	}
	
	private void checkDisciplinasComMaxAlunosTeoricosZerados(Parametro parametro, List<String> errors) {
		// colhe as disciplinas de acordo com os campi selecionados para otimização
		Set<Disciplina> disciplinasSelecionadas = new HashSet<Disciplina>();
		
		for (Campus campus : parametro.getCampi()) {
			for (Disciplina disciplina : campus.getCenario().getDisciplinas()) {
				disciplinasSelecionadas.add(disciplina);
			}
		}
		
		List<String> disciplinasComCreditosTeoricosZerados = new ArrayList<String>();
		for (Disciplina disciplina : disciplinasSelecionadas) {
			if (disciplina.getCreditosTeorico() != 0 &&  disciplina.getMaxAlunosTeorico() == 0){
				disciplinasComCreditosTeoricosZerados.add(disciplina.getCodigo());
			}
		}
		
		if (!disciplinasComCreditosTeoricosZerados.isEmpty()) {
			errors.add(HtmlUtils.htmlUnescape("A(s) disciplina(s) " + disciplinasComCreditosTeoricosZerados + " possui créditos teóricos, porém o máximos de alunos teóricos é igual a zero."));
		}
		
	}
	/**
	 * ISSUE TRIEDA-1926
	 * 
	 * Método que verifica as disciplinas que possuem:
	 * 	a) créditos teóricos e práticos diferentes de zero;
	 *  b) não exigem laboratórios;
	 *  c) e com valores diferentes nos campos dis_max_alun_teorico e dis_max_alun_pratico
	 *  
	 *  Para essas disciplinas será retornado um erro, quando ocorrer os casos a) e b) 
	 *  os valores dis_max_alun_teorico e dis_max_alun_pratico devem ser iguais
	 * 
	 * 
	 * @param parametro
	 * @param errors
	 */
	
	private void checkDisciplinasComMaxAlunosDiferentes(Parametro parametro, List<String> errors) {
		// colhe as disciplinas de acordo com os campi selecionados para otimização
		Set<Disciplina> disciplinasSelecionadas = new HashSet<Disciplina>();
		for (Campus campus : parametro.getCampi()) {
			for (Disciplina disciplina : campus.getCenario().getDisciplinas()) {
				disciplinasSelecionadas.add(disciplina);
			}
		}
		
		for (Disciplina disciplina : disciplinasSelecionadas) {
			if (disciplina.getCreditosPratico() != 0 &&  disciplina.getMaxAlunosPratico() != 0)
			{
				if(!disciplina.getLaboratorio())
				{
					if(!disciplina.getMaxAlunosPratico().equals(disciplina.getMaxAlunosTeorico()))
					{
						
						errors.add(HtmlUtils.htmlUnescape("A disciplina [" + disciplina.getCodigo() + "] "+
								" (com ["+ disciplina.getCreditosTeorico() + "] crédito(s) teórico(s), ["+
								 disciplina.getCreditosPratico() + "] crédito(s) práticos(s) e e \"Exige Laboratório?\"=["+
								(disciplina.getLaboratorio()?"Sim":"Não") +  "]) está com valores diferentes para \"Máx. Alunos Teóricos\"=["+
								disciplina.getMaxAlunosTeorico()+"] e \"Máx. Alunos Práticos\"= ["+disciplina.getMaxAlunosPratico()+"]"
								));
					}
				}
			}
		}
		
		
	}
	
	private void checkDisciplinasComMaxAlunosPraticosZerados(Parametro parametro, List<String> errors) {
		// colhe as disciplinas de acordo com os campi selecionados para otimização
		Set<Disciplina> disciplinasSelecionadas = new HashSet<Disciplina>();
		for (Campus campus : parametro.getCampi()) {
			for (Disciplina disciplina : campus.getCenario().getDisciplinas()) {
				disciplinasSelecionadas.add(disciplina);
			}
		}
		
		List<String> disciplinasComCreditosPraticosZerados = new ArrayList<String>();
		for (Disciplina disciplina : disciplinasSelecionadas) {
			if (disciplina.getCreditosPratico() != 0 &&  disciplina.getMaxAlunosPratico() == 0){
				disciplinasComCreditosPraticosZerados.add(disciplina.getCodigo());
			}
		}
		
		if (!disciplinasComCreditosPraticosZerados.isEmpty()) {
			errors.add(HtmlUtils.htmlUnescape("A(s) disciplina(s) " + disciplinasComCreditosPraticosZerados + " possui créditos práticos, porém o máximos de alunos práticos é igual a zero."));
		}
	}
	
	private void checkMaxCreditosSemanaisPorPeriodo_e_DisciplinasRepetidasPorCurriculo(Parametro parametro, InstituicaoEnsino instituicaoEnsino, List<String> errors, List<String> warnings) {
		// obtém os currículos do campus selecionado para otimização
		Set<Curriculo> curriculosDoCampusSelecionado = new HashSet<Curriculo>();
		for (Campus campus : parametro.getCampi()) {
			for (Oferta oferta : campus.getOfertas()) {
				curriculosDoCampusSelecionado.add(oferta.getCurriculo());
			}
		}
		
		// [SemanaLetivaId -> Máximo Créditos Semanais]
		Map<Long,Integer> maxCreditosSemanaisPorSemanaLetivaMap = new HashMap<Long,Integer>();
 
		for (Curriculo curriculo : parametro.getCenario().getCurriculos()) {
			// filtra os currículos do campus selecionado para otimização
			if (curriculosDoCampusSelecionado.contains(curriculo)) {
				// obtém o máximo de créditos semanais da semana letiva associada com o currículo
				Integer maxCreditosSemanais = maxCreditosSemanaisPorSemanaLetivaMap.get(curriculo.getSemanaLetiva().getId());
				if (maxCreditosSemanais == null) {
					maxCreditosSemanais = 0;
					for (Turno turno : parametro.getTurnos())
					{
						maxCreditosSemanais += curriculo.getSemanaLetiva().calcTotalCreditosSemanais(turno);
					}
					maxCreditosSemanaisPorSemanaLetivaMap.put(curriculo.getSemanaLetiva().getId(),maxCreditosSemanais);
				}
				
				Set<Long> disciplinasDoCurriculo = new HashSet<Long>();
				Set<String> disciplinasRepetidasNoCurriculo = new HashSet<String>();
				List<String> periodosQueViolamMaxCreditosSemanais = new ArrayList<String>();
				for (Integer periodo : curriculo.getPeriodos()) {
					Integer totalCreditosDoPeriodo = 0;
					for (CurriculoDisciplina curriculoDisciplina : curriculo.getCurriculoDisciplinasByPeriodo(periodo)) {
						if (!disciplinasDoCurriculo.add(curriculoDisciplina.getDisciplina().getId())) {
							disciplinasRepetidasNoCurriculo.add(curriculoDisciplina.getDisciplina().getCodigo());
						}
						totalCreditosDoPeriodo += curriculoDisciplina.getDisciplina().getCreditosTotal();
					}
					
					if (totalCreditosDoPeriodo > maxCreditosSemanais) {
						periodosQueViolamMaxCreditosSemanais.add(periodo + "("+totalCreditosDoPeriodo+")");
					}
					
				}
				
				if (!disciplinasRepetidasNoCurriculo.isEmpty()) {
					errors.add(HtmlUtils.htmlUnescape("A matriz curricular [" + curriculo.getCodigo() + "] contém disciplinas repetidas, são elas: " + disciplinasRepetidasNoCurriculo.toString()));
					System.out.println("A matriz curricular [" + curriculo.getCodigo() + "] contém disciplinas repetidas, são elas: " + disciplinasRepetidasNoCurriculo.toString());
				}
				
				if (!periodosQueViolamMaxCreditosSemanais.isEmpty()) {
					warnings.add(HtmlUtils.htmlUnescape("Na matriz curricular [" + curriculo.getCodigo() + "] existem períodos que violam a quantidade máxima de créditos semanais da Semana Letiva. Máximo de Créditos Semanais = " + maxCreditosSemanais + ". Período(TotalCréditos) = " + periodosQueViolamMaxCreditosSemanais.toString()));
					System.out.println("Na matriz curricular [" + curriculo.getCodigo() + "] existem períodos que violam a quantidade máxima de créditos semanais da Semana Letiva. Máximo de Créditos Semanais = " + maxCreditosSemanais + ". Período(TotalCréditos) = " + periodosQueViolamMaxCreditosSemanais.toString());
				}
			}
		}
	}
	
	private void checkMaxCreditosSemanaisPorAluno_e_DisciplinasRepetidasPorAluno(Parametro parametro, InstituicaoEnsino instituicaoEnsino, List<String> errors, List<String> warnings) {
		boolean realizaVerificacaoSomenteParaDemandasDePrioridade1 = true;
		
		// obtém os alunos do campus selecionado para otimização
		List<AlunoDemanda> demandasDeAlunoDosCampiSelecionados = AlunoDemanda.findByCampusAndTurno(instituicaoEnsino,parametro.getCampi(),parametro.getTurnos());
		Map<Aluno,List<AlunoDemanda>> alunoToAlunoDemandasMap = new HashMap<Aluno,List<AlunoDemanda>>();
		Map<Aluno,List<SemanaLetiva>> alunoToSemanasLetivasMap = new HashMap<Aluno,List<SemanaLetiva>>();
		for (AlunoDemanda alunoDemanda : demandasDeAlunoDosCampiSelecionados) {
			// demandas do aluno
			List<AlunoDemanda> demandasDoAluno = alunoToAlunoDemandasMap.get(alunoDemanda.getAluno());
			if (demandasDoAluno == null) {
				demandasDoAluno = new ArrayList<AlunoDemanda>();
				alunoToAlunoDemandasMap.put(alunoDemanda.getAluno(),demandasDoAluno);
			}
			demandasDoAluno.add(alunoDemanda);
			
			// semanas letivas associadas com o aluno
			List<SemanaLetiva> semanasLetivasAssociadasComAluno = alunoToSemanasLetivasMap.get(alunoDemanda.getAluno());
			if (semanasLetivasAssociadasComAluno == null) {
				semanasLetivasAssociadasComAluno = new ArrayList<SemanaLetiva>();
				alunoToSemanasLetivasMap.put(alunoDemanda.getAluno(),semanasLetivasAssociadasComAluno);
			}
			semanasLetivasAssociadasComAluno.add(alunoDemanda.getDemanda().getOferta().getCurriculo().getSemanaLetiva());
		}
		
		// [SemanaLetivaId -> Máximo Créditos Semanais]
		Map<Long,Integer> maxCreditosSemanaisPorSemanaLetivaMap = new HashMap<Long,Integer>();
 
		for (Entry<Aluno,List<AlunoDemanda>> entry : alunoToAlunoDemandasMap.entrySet()) {
			Aluno aluno = entry.getKey();
			List<AlunoDemanda> demandasDoAluno = entry.getValue();
			
			// obtém o máximo de créditos semanais das semanas letivas associadas com o aluno
			Integer maxCreditosSemanais = 0;
			List<SemanaLetiva> semanasLetivasAssociadasComAluno = alunoToSemanasLetivasMap.get(aluno);
			for (SemanaLetiva semanaLetiva : semanasLetivasAssociadasComAluno) {
				Integer localMaxCreditosSemanais = maxCreditosSemanaisPorSemanaLetivaMap.get(semanaLetiva.getId());
				if (localMaxCreditosSemanais == null) {
					localMaxCreditosSemanais = 0;
					for (Turno turno : parametro.getTurnos())
					{
						if (semanaLetiva.calcTotalCreditosSemanais(turno) > localMaxCreditosSemanais)
							localMaxCreditosSemanais = semanaLetiva.calcTotalCreditosSemanais(turno);
					}
					maxCreditosSemanaisPorSemanaLetivaMap.put(semanaLetiva.getId(),localMaxCreditosSemanais);
				}
				
				if (localMaxCreditosSemanais > maxCreditosSemanais) {
					maxCreditosSemanais = localMaxCreditosSemanais;
				}
			}
			
			// calcula total de créditos do aluno e verifica existência de disciplinas repetidas
			Set<Long> disciplinasDoAluno = new HashSet<Long>();
			Set<String> disciplinasRepetidasParaOAluno = new HashSet<String>();
			Integer totalCreditosDoAluno = 0;
			for (AlunoDemanda alunoDemanda : demandasDoAluno) {
				if (!realizaVerificacaoSomenteParaDemandasDePrioridade1 || (alunoDemanda.getPrioridade() == 1)) { // IF utilizado para, quando for o caso, considerar o check somente para demandas de prioridade 1
					Disciplina disciplina = alunoDemanda.getDemanda().getDisciplina();
					if (!disciplinasDoAluno.add(disciplina.getId())) {
						disciplinasRepetidasParaOAluno.add(disciplina.getCodigo());
					}
					
					totalCreditosDoAluno += disciplina.getCreditosTotal();
				}
			}
			
			if (!disciplinasRepetidasParaOAluno.isEmpty()) {
				errors.add(HtmlUtils.htmlUnescape("O aluno [" + aluno.getNome() + "] de matrícula [" + aluno.getMatricula() + "] pede por disciplinas repetidas, são elas: " + disciplinasRepetidasParaOAluno.toString()));
			}
			
			if (totalCreditosDoAluno > maxCreditosSemanais) {
				warnings.add(HtmlUtils.htmlUnescape("O aluno [" + aluno.getNome() + "] de matrícula [" + aluno.getMatricula() + "] viola a quantidade máxima de créditos semanais da Semana Letiva. Máximo de Créditos Semanais = " + maxCreditosSemanais + ". Total de créditos do aluno = " + totalCreditosDoAluno));
			}
		}
	}
	
	private void checkDemandasComDisciplinasSemCurriculo(Parametro parametro, List<String> errors) {
		// [CurriculoId -> Set<DisciplinaId>]
		Map<Long,Set<Long>> curriculoIdToDisciplinasIdsMap = new HashMap<Long,Set<Long>>();
		for (Campus campus : parametro.getCampi()) {
			for (Oferta oferta : campus.getOfertas()) {
				Curriculo curriculo = oferta.getCurriculo();
				
				// obtém as disciplinas associadas com o currículo em questão
				Set<Long> disciplinasDoCurriculo = curriculoIdToDisciplinasIdsMap.get(curriculo.getId());
				if (disciplinasDoCurriculo == null) {
					disciplinasDoCurriculo = new HashSet<Long>();
					for (CurriculoDisciplina curriculoDisciplina : curriculo.getDisciplinas()) {
						disciplinasDoCurriculo.add(curriculoDisciplina.getDisciplina().getId());
					}
					curriculoIdToDisciplinasIdsMap.put(curriculo.getId(),disciplinasDoCurriculo);
				}
				
				// verifica se alguma demanda contém alguma disciplina que não esteja no currículo
				// da oferta associada com a demanda
				for (Demanda demanda : oferta.getDemandas()) {
					if (!disciplinasDoCurriculo.contains(demanda.getDisciplina().getId())) {
						errors.add(HtmlUtils.htmlUnescape("A demanda [" + demanda.getNaturalKeyString() + "] é inválida pois a disciplina [" + demanda.getDisciplina().getCodigo() + "] não pertence a nenhum período da matriz curricular [" + curriculo.getCodigo() + "]."));
						System.out.println("A demanda [" + demanda.getNaturalKeyString() + "] é inválida pois a disciplina [" + demanda.getDisciplina().getCodigo() + "] não pertence a nenhum período da matriz curricular [" + curriculo.getCodigo() + "].");
					}
				}
			}
		}
	}
	
	private void checkProfessorComCargaHorariaMaximaZerada(Parametro parametro, List<String> errors, List<String> warnings) {
		
		List<String> warningsAux = new ArrayList<String>();
		
		boolean todos = true;
		boolean professoresVazios = true;
		
		for (Campus campus : parametro.getCampi()) {
			for (Professor professor : campus.getProfessores()) {
				professoresVazios = false;
				if(professor.getCargaHorariaMax() == 0){
					warningsAux.add(HtmlUtils.htmlUnescape("O professor "+professor.getNome() + " de CPF "+professor.getCpf()+" está com carga horária máxima igual a zero e, portanto, não poderá ser utilizado"));
				} else {
					todos = false;
				}
			}
		}
		if (!professoresVazios)
		{
			if(todos){
				errors.add(HtmlUtils.htmlUnescape("Todos os professores estão com a carga horária máxima zerada"));
			} else
				warnings.addAll(warningsAux);
		}
	}
	
	private void checkOfertaComCargaReceitaCreditoZerada(Parametro parametro, List<String> errors, List<String> warnings) {
		
		List<String> warningsAux = new ArrayList<String>();
		
		boolean todos = true;
		
		for (Campus campus : parametro.getCampi()) {
			for (Oferta oferta : campus.getOfertas()) {
				if(oferta.getReceita() == 0){
					warningsAux.add(HtmlUtils.htmlUnescape("A oferta  [" + oferta.getNaturalKeyString() + "] está com receita de crétido igual a zero, o que prejudica a otimização"));
				} else {
					todos = false;
				}
			}
		}
		
		if(todos){
			errors.add(HtmlUtils.htmlUnescape("Todas as ofertas estão com a receita de crédito zerada"));
		} else
			warnings.addAll(warningsAux);
	}
	
	private void checkSemanasLetivasIncompativeis(Parametro parametro, List<String> errors) {
		// obtém os currículos do campus selecionado para otimização
		Set<Curriculo> curriculosDoCampusSelecionado = new HashSet<Curriculo>();
		for(Campus campus : parametro.getCampi()){
			for(Oferta oferta : campus.getOfertas()) curriculosDoCampusSelecionado.add(oferta.getCurriculo());
		}
		
		final Map<SemanaLetiva, List<Pair<Calendar, Calendar>>> horariosDasSemanas = new HashMap<SemanaLetiva, List<Pair<Calendar, Calendar>>>();

		// filtra os currículos do campus selecionado para otimização e armazena para cada semana letiva
		// desses curriculos, seus respectivos horarios aulas em ordem crescente
		for(Curriculo curriculo : parametro.getCenario().getCurriculos()){
			if(curriculosDoCampusSelecionado.contains(curriculo)){
				SemanaLetiva semanaLetiva = curriculo.getSemanaLetiva();
				List<Pair<Calendar, Calendar>> pairs = new ArrayList<Pair<Calendar, Calendar>>();
				if(horariosDasSemanas.get(semanaLetiva) == null){
					for(HorarioAula horarioAula : HorarioAula.findBySemanaLetiva(parametro.getInstituicaoEnsino(), semanaLetiva)){
						Calendar hi = TriedaServerUtil.dateToCalendar(horarioAula.getHorario());
					
						Calendar hf = Calendar.getInstance();
						hf.clear();
						hf.setTime(hi.getTime());
						hf.add(Calendar.MINUTE, semanaLetiva.getTempo());
						
						pairs.add(Pair.create(hi,hf));
					}
					TriedaServerUtil.ordenaParesDeHorarios(pairs);
					horariosDasSemanas.put(semanaLetiva, pairs);
				}
			}
		}
		
		// obtem uma lista contendo as semanas letivas ordenadas de acordo com seus horarios em que comecam 
		List<SemanaLetiva> semanasLetivas = new ArrayList<SemanaLetiva>(horariosDasSemanas.keySet());
		Collections.min(semanasLetivas, new Comparator<SemanaLetiva>(){
			@Override
			public int compare(SemanaLetiva o1, SemanaLetiva o2){
				Pair<Calendar,Calendar> par1 = horariosDasSemanas.get(o1).get(0);
				Pair<Calendar,Calendar> par2 = horariosDasSemanas.get(o2).get(0);
				
				Calendar horarioInicial1 = par1.getLeft();
				Calendar horarioInicial2 = par2.getLeft();
				int ret = horarioInicial1.compareTo(horarioInicial2);
				if(ret == 0){
					Calendar horarioFinal1 = par1.getRight();
					Calendar horarioFinal2 = par2.getRight();
					return horarioFinal1.compareTo(horarioFinal2);
				}
				return ret;
			}
		});
		
		while(semanasLetivas.size() > 1){
			SemanaLetiva s1 = semanasLetivas.remove(0);
			List<Pair<Calendar, Calendar>> hs1 = horariosDasSemanas.get(s1);
			for(SemanaLetiva s2 : semanasLetivas){
				List<Pair<Calendar, Calendar>> hs2 = horariosDasSemanas.get(s2);
				int index = 0;
				boolean intersect = false;
				Pair<Calendar, Calendar> previous = null;
				try{
					for(Pair<Calendar, Calendar> h1 : hs1){
						Pair<Calendar, Calendar> h2 = hs2.get(index);
						// se ainda nao houve intersecao, continua percorrendo os horarios ateh que haja uma
						if(!intersect){
							if(h2.getRight().compareTo(h1.getLeft()) >= 0) intersect = true;
							else continue;
						}
						// o right do h2 tem sempre que ser maior ou igual ao left do h1. Senao, significa
						// que ha um intervalo maior do que o h2, e portanto, uma incompatibilidade.
						// Ex.: s1: 19:00/19:50 - 20:10/21:30 ; s2: 19:20/20:00 
						
						if(h2.getRight().compareTo(h1.getLeft()) >= 0){
							if(h2.getRight().compareTo(h1.getRight()) > 0){
								if(previous != null && previous.getRight().compareTo(h2.getLeft()) != 0) throw new Exception();
								previous = h1;
								continue;
							}
							if(previous != null && previous.getRight().compareTo(h1.getLeft()) != 0) throw new Exception();
							previous = h2;
							index++;
						}
						else throw new Exception();
					}
				}
				catch(Exception e){
					errors.add("As semanas letivas " + s1.getCodigo() + " e " + s2.getCodigo() + "são incompatíveis.");
				}
			}
		}
	}
	
	private void checkEquivalenciasQueLevamParaDisciplinasOnlineOuSemCreditos(Cenario cenario, List<String> errors) { 
		// TRIEDA-1404: Notificar usuário da existência de regras de equivalência que levam para disciplinas disciplinas com total de créditos (teóricos + práticos) zerados
		
		StringBuffer equivalenciasStrBuffer = new StringBuffer("");
		List<Equivalencia> equivalencias = Equivalencia.findByCenario(getInstituicaoEnsinoUser(),cenario);
		for (Equivalencia eq : equivalencias) {
			Disciplina discCursou = eq.getCursou();
			if (!discCursou.ocupaGrade()) {
				Disciplina discElimina = eq.getElimina();
				if (discElimina.ocupaGrade()) {
					equivalenciasStrBuffer.append(discCursou.getCodigo() + "->" + discElimina.getCodigo() + "; ");
				}
			}
		}
		
		if (equivalenciasStrBuffer.length() > 0) {
			String equivalenciasStr = equivalenciasStrBuffer.substring(0,equivalenciasStrBuffer.length()-2);
			errors.add(HtmlUtils.htmlUnescape("As equivalências [" + equivalenciasStr + "] substituem disciplinas que ocupam grade (presenciais ou tele-presenciais) por disciplinas que não ocupam grade (online ou com créditos zerados)."));
		}
	}
	
	private boolean checkCicloDisciplinasEquivalentes(Cenario cenario, List<String> errors){
		boolean detectouCiclo = false;
		
		int id = 0;
		Integer cIndex = 0, eIndex = 0, maxIndex = 0;
		List<Long> nodeMap = new ArrayList<Long>();
		List<Disciplina> disMap = new ArrayList<Disciplina>();
		Map<String, Set<Pair<Integer, Integer>>> pairs = new HashMap<String, Set<Pair<Integer, Integer>>>();
		Map<Long,Integer> disciplinaIdToQtdDemandadaMap = new HashMap<Long,Integer>();

		// Para cada disciplina, obtem-se as suas equivalencias para montar tres estruturas:
		// -> disMap mapeia o index associado a cada disciplina identificada
		// -> nodeMap faz o controle de qual disciplina estah mapeada ou nao
		// -> pairs obtem a relação de equivalencia de uma disciplina com outra. 
		for(Disciplina disciplina : cenario.getDisciplinas()){
			Integer qtdDemandada = 0;
			for (Demanda demanda : disciplina.getDemandas()) {
				qtdDemandada += demanda.getQuantidade();
			}
			disciplinaIdToQtdDemandadaMap.put(disciplina.getId(),qtdDemandada);
			for(Equivalencia equivalencia : disciplina.getEliminam()){
				Disciplina cursou = equivalencia.getCursou();
				Disciplina elimina = equivalencia.getElimina();
				if((cIndex = (Integer) nodeMap.indexOf(cursou.getId())) == -1){
					cIndex = id++;
					nodeMap.add(cursou.getId());
					disMap.add(cursou);
				}
				if((eIndex = (Integer) nodeMap.indexOf(elimina.getId())) == -1){
					eIndex = id++;
					nodeMap.add(elimina.getId());
					disMap.add(elimina);
				}
				if (equivalencia.getEquivalenciaGeral()){
					if ( pairs.get("GERAL") != null ){
						pairs.get("GERAL").add( Pair.create(cIndex, eIndex) );
					}
					else{
						Set<Pair<Integer, Integer>> pair = new HashSet<Pair<Integer, Integer>>();
						pair.add(Pair.create(cIndex, eIndex));
						pairs.put("GERAL", pair);
					}
				}
				else {
					for (Curso curso : equivalencia.getCursos()){
						if ( pairs.get(curso.getCodigo()) != null ){
							pairs.get(curso.getCodigo()).add( Pair.create(cIndex, eIndex) );
						}
						else {
							Set<Pair<Integer, Integer>> pair = new HashSet<Pair<Integer, Integer>>();
							pair.add(Pair.create(cIndex, eIndex));
							pairs.put(curso.getCodigo(), pair);
						}
					}
				}
				if (cIndex > maxIndex) {
					maxIndex = cIndex;
				}
				if (eIndex > maxIndex) {
					maxIndex = eIndex;
				}
			}	
		}
		
		// Constroi um grafo a partir dos pares obtidos e realiza a busca para verificar
		// a existencia de ciclos. Nesse grafo, os indices associados aos nos sao os
		// mesmos identificados em disMap.
		Map<String, Grafo> grafos = new HashMap<String, Grafo>();
		for (String curso : pairs.keySet()){
			Grafo g = new Grafo(maxIndex+1);
			for(Pair<Integer, Integer> par: pairs.get(curso)) {
				g.insereArco(par.getLeft(), par.getRight());
			}
			grafos.put(curso, g);
		}
		
		Profundidade p = new Profundidade();
		for (String curso : grafos.keySet()){
			Grafo g = grafos.get(curso);
			if(!p.testeCiclos(g)){
				
				// Obtem-se os ciclos identificados e os imprime num formato adequado para
				// informar ao usuario quais sao eles.
				List<List<Integer>> ciclos = p.getCiclos();
				int i = 1;
				String ciclosStr = "Foram detectados os seguintes ciclos entre disciplinas equivalentes do Curso ( " + curso + " ) :<br /><br />";
				for(List<Integer> ciclo: ciclos){
					ciclosStr += i++ + ") ";
					for(Integer idc: ciclo){
						Disciplina dis = disMap.get(idc);
						ciclosStr += dis.getCodigo() + "->";
					}
					String sugestaoParaEliminarCiclo = "";
					sugestaoParaEliminarCiclo = avaliaSugestoesParaEliminacaoDeCiclos(disMap,disciplinaIdToQtdDemandadaMap,ciclo);
					ciclosStr = ciclosStr.substring(0, ciclosStr.length() - 2) + " " + sugestaoParaEliminarCiclo + "<br /><br />";
				}
				ciclosStr = ciclosStr.substring(0, ciclosStr.length() - 12);
				errors.add(HtmlUtils.htmlUnescape(ciclosStr));
				
				detectouCiclo = true;
			}
		}
		
		return detectouCiclo;
	}

	private String avaliaSugestoesParaEliminacaoDeCiclos(List<Disciplina> disMap, Map<Long, Integer> disciplinaIdToQtdDemandadaMap, List<Integer> ciclo) {
		String sugestao;
		// avalia sugestões para eliminação de ciclos de tamanho 2
		if (ciclo.size() == 3) {
			Disciplina dis1 = disMap.get(ciclo.get(0));
			Disciplina dis2 = disMap.get(ciclo.get(1));
			if (!dis1.getCurriculos().isEmpty() && !dis2.getCurriculos().isEmpty()) {
				CurriculoDisciplina c1 = dis1.getCurriculos().iterator().next();
				CurriculoDisciplina c2 = dis2.getCurriculos().iterator().next();
				SemanaLetiva s1 = c1.getCurriculo().getSemanaLetiva();
				SemanaLetiva s2 = c2.getCurriculo().getSemanaLetiva();
				if (!s1.equals(s2)) {
					if (s1.getCodigo().equals("NOVO")) {
						sugestao = "[Sugestão: eliminar " + dis2.getCodigo() + " -> " + dis1.getCodigo() + "]";
					} else {
						sugestao = "[Sugestão: eliminar " + dis1.getCodigo() + " -> " + dis2.getCodigo() + "]";
					}
				} else {
					Integer qtdeDemandada1 = disciplinaIdToQtdDemandadaMap.get(dis1.getId());
					Integer qtdeDemandada2 = disciplinaIdToQtdDemandadaMap.get(dis2.getId());
					if (qtdeDemandada1 > qtdeDemandada2) {
						sugestao = "[Sugestão: eliminar " + dis2.getCodigo() + " -> " + dis1.getCodigo() + "]";
					} else {
						sugestao = "[Sugestão: eliminar " + dis1.getCodigo() + " -> " + dis2.getCodigo() + "]";
					}
				}
			} else  {
				Integer qtdeDemandada1 = disciplinaIdToQtdDemandadaMap.get(dis1.getId());
				Integer qtdeDemandada2 = disciplinaIdToQtdDemandadaMap.get(dis2.getId());
				if (qtdeDemandada1 > qtdeDemandada2) {
					sugestao = "[Sugestão: eliminar " + dis2.getCodigo() + " -> " + dis1.getCodigo() + "]";
				} else {
					sugestao = "[Sugestão: eliminar " + dis1.getCodigo() + " -> " + dis2.getCodigo() + "]";
				}
			}
		} else {
			int index = ciclo.size() / 2 - 1;
			Disciplina dis1 = disMap.get(ciclo.get(index));
			Disciplina dis2 = disMap.get(ciclo.get(index+1));
			sugestao = "[Sugestão: eliminar " + dis1.getCodigo() + " -> " + dis2.getCodigo() + "]";
		}
		return sugestao;
	}
	
	private void checkEquivalenciasQueGeramDisciplinasRepetidasEmUmMesmoCurriculo(Parametro parametro, List<String> errors) {
		// [DisciplinaId -> Disciplina]
		Map<Long,Disciplina> disciplinasMap = new HashMap<Long,Disciplina>();
		//
		Map<Pair<Long,Long>,Set<Pair<Long,Long>>> equivalenciaCalculadaToEquivalenciasOriginaisMap = new HashMap<Pair<Long,Long>,Set<Pair<Long,Long>>>();
		// [DisciplinaEliminaId -> Par<DisciplinaCursouId,DisciplinaEliminaId>]
		Map<Long,Set<Pair<Long,Long>>> disciplinaEliminaIdToEquivalenciasCalculadasMap = new HashMap<Long,Set<Pair<Long,Long>>>();
		
		preencheEstruturasParaCheckDeEquivalencias(parametro,disciplinasMap,equivalenciaCalculadaToEquivalenciasOriginaisMap,disciplinaEliminaIdToEquivalenciasCalculadasMap);
		
		// obtém curriculos
		Set<Curriculo> curriculos = new HashSet<Curriculo>();
		for (Campus campus : parametro.getCampi()) {
			for (Oferta oferta : campus.getOfertas()) {
				curriculos.add(oferta.getCurriculo());
			}
		}
		
		for (Curriculo curriculo : curriculos) {
			// obtém as disciplinas associadas com o currículo em questão
			Map<Long,CurriculoDisciplina> disciplinasDoCurriculo = new HashMap<Long,CurriculoDisciplina>();
			for (CurriculoDisciplina curriculoDisciplina : curriculo.getDisciplinas()) {
				disciplinasDoCurriculo.put(curriculoDisciplina.getDisciplina().getId(),curriculoDisciplina);
			}
			
			Set<Pair<Long,Long>> equivalenciasAplicaveisNoCurriculo = new HashSet<Pair<Long,Long>>();
			
			// verifica se alguma equivalência invalida o currículo em questão
			boolean detectouEquivalenciaInconsistentePrimeiroTeste = false;
			
			// primeiro, verifica se a aplicação de cada equivalência ao currículo gera repetição de disciplina
			for (Long disciplinaDoCurriculoId : disciplinasDoCurriculo.keySet()) {
				// obtém as equivalências que podem ser aplicadas na disciplina em questão
				Set<Pair<Long,Long>> equivalenciasAplicaveisNaDisciplina = disciplinaEliminaIdToEquivalenciasCalculadasMap.get(disciplinaDoCurriculoId);
				if (equivalenciasAplicaveisNaDisciplina != null) {
					// acumula todas as equivalências aplicáveis no currículo em questão
					equivalenciasAplicaveisNoCurriculo.addAll(equivalenciasAplicaveisNaDisciplina);
					 
					for (Pair<Long,Long> equivalenciaAplicavelNaDisciplina : equivalenciasAplicaveisNaDisciplina) {
						Long disciplinaCursouId = equivalenciaAplicavelNaDisciplina.getLeft();
						Long disciplinaEliminaId = equivalenciaAplicavelNaDisciplina.getRight();
						// verifica se a equivalência aplicável na disciplina em questão irá transformá-la em outra disciplina
						// já existente no currículo
						CurriculoDisciplina curriculoDisciplina = disciplinasDoCurriculo.get(disciplinaCursouId);
						if (curriculoDisciplina != null) {
							String equivalenciasInfo = "";
							Set<Pair<Long,Long>> equivalenciasOriginaisQueGeraramACalculada = equivalenciaCalculadaToEquivalenciasOriginaisMap.get(equivalenciaAplicavelNaDisciplina);
							if (equivalenciasOriginaisQueGeraramACalculada != null) {
								// a equivalência aplicável na disciplina é uma equivalência calculada
								equivalenciasInfo = imprimeEquivalenciasEmUmaString(equivalenciasOriginaisQueGeraramACalculada,disciplinasMap);
							} else {
								// a equivalência aplicável na disciplina é uma equivalência original
								Disciplina cursou = disciplinasMap.get(disciplinaCursouId);
								Disciplina elimina = disciplinasMap.get(disciplinaEliminaId);
								equivalenciasInfo = cursou.getCodigo() + "->" + elimina.getCodigo();
							}
							
							errors.add(HtmlUtils.htmlUnescape("A(s) equivalência(s) [" + equivalenciasInfo + "] pode(m) invalidar a matriz curricular [" + curriculo.getCodigo() + "] por conta da disciplina [" + curriculoDisciplina.getDisciplina().getCodigo() + "] no período [" + curriculoDisciplina.getPeriodo() + "]."));
							detectouEquivalenciaInconsistentePrimeiroTeste = true;
						}
					}
				}
			}
			
			if (!detectouEquivalenciaInconsistentePrimeiroTeste) {
				// organiza equivalências aplicáveis no currículo pela extremidade Cursou da relação (Cursou,Elimina) 
				// [DisciplinaCursouId -> Set<Par<DisciplinaCursouId,DisciplinaEliminaId>>]
				Map<Long,Set<Pair<Long,Long>>> cursouToEquivalenciasAplicaveisMap = new HashMap<Long,Set<Pair<Long,Long>>>();
				for (Pair<Long,Long> parCursouElimina : equivalenciasAplicaveisNoCurriculo) {
					Long disciplinaCursouId = parCursouElimina.getLeft();
					Set<Pair<Long,Long>> equivalencias = cursouToEquivalenciasAplicaveisMap.get(disciplinaCursouId);
					if (equivalencias == null) {
						equivalencias = new HashSet<Pair<Long,Long>>();
						cursouToEquivalenciasAplicaveisMap.put(disciplinaCursouId,equivalencias);
					}
					equivalencias.add(parCursouElimina);
				}
				
				// segundo, verifica se há mais de uma equivalência, aplicável ao currículo, que leva para uma mesma disciplina
				for (Entry<Long,Set<Pair<Long,Long>>> entry : cursouToEquivalenciasAplicaveisMap.entrySet()) {
					Set<Pair<Long,Long>> equivalenciasComMesmaExtremidadeCursou = entry.getValue();
					// verifica se há mais de uma equivalência, aplicável ao currículo, que leva para uma mesma disciplina
					if (equivalenciasComMesmaExtremidadeCursou.size() > 1) {
						Set<Pair<Long,Long>> equivalenciasParaImprimir = new HashSet<Pair<Long,Long>>();
						for (Pair<Long,Long> e : equivalenciasComMesmaExtremidadeCursou) {
							Set<Pair<Long,Long>> equivalenciasOriginaisQueGeraramACalculada = equivalenciaCalculadaToEquivalenciasOriginaisMap.get(e);
							if (equivalenciasOriginaisQueGeraramACalculada != null) {
								// a equivalência 'e' é uma equivalência calculada
								equivalenciasParaImprimir.addAll(equivalenciasOriginaisQueGeraramACalculada);
							} else {
								// a equivalência 'e' é uma equivalência original
								equivalenciasParaImprimir.add(e);
							}
						}
						String equivalenciasStr = imprimeEquivalenciasEmUmaString(equivalenciasParaImprimir,disciplinasMap);
						// gera mensagem de erro
						errors.add(HtmlUtils.htmlUnescape("As equivalências [" + equivalenciasStr + "] pode(m) invalidar a matriz curricular [" + curriculo.getCodigo() + "] pois levam para uma mesma disciplina."));
					}
				}
			}
		}
	}
	
	private void checkEquivalenciasQueGeramDisciplinasRepetidasEmUmAluno(Parametro parametro, List<String> errors) {
		boolean realizaVerificacaoSomenteParaDemandasDePrioridade1 = true;
		
		// [DisciplinaId -> Disciplina]
		Map<Long,Disciplina> disciplinasMap = new HashMap<Long,Disciplina>();
		// 
		Map<Pair<Long,Long>,Set<Pair<Long,Long>>> equivalenciaCalculadaToEquivalenciasOriginaisMap = new HashMap<Pair<Long,Long>,Set<Pair<Long,Long>>>();
		// [DisciplinaEliminaId -> Par<DisciplinaCursouId,DisciplinaEliminaId>]
		Map<Long,Set<Pair<Long,Long>>> disciplinaEliminaIdToEquivalenciasCalculadasMap = new HashMap<Long,Set<Pair<Long,Long>>>();
		
		preencheEstruturasParaCheckDeEquivalencias(parametro,disciplinasMap,equivalenciaCalculadaToEquivalenciasOriginaisMap,disciplinaEliminaIdToEquivalenciasCalculadasMap);
		
		// obtém os alunos do campus selecionado para otimização
		List<AlunoDemanda> demandasDeAlunoDosCampiSelecionados = AlunoDemanda.findByCampusAndTurno(getInstituicaoEnsinoUser(),parametro.getCampi(),parametro.getTurnos());
		Map<Aluno,List<AlunoDemanda>> alunoToAlunoDemandasMap = new HashMap<Aluno,List<AlunoDemanda>>();
		for (AlunoDemanda alunoDemanda : demandasDeAlunoDosCampiSelecionados) {
			if (!realizaVerificacaoSomenteParaDemandasDePrioridade1 || (alunoDemanda.getPrioridade() == 1)) { // IF utilizado para, quando for o caso, considerar o check somente para demandas de prioridade 1
				// demandas do aluno
				List<AlunoDemanda> demandasDoAluno = alunoToAlunoDemandasMap.get(alunoDemanda.getAluno());
				if (demandasDoAluno == null) {
					demandasDoAluno = new ArrayList<AlunoDemanda>();
					alunoToAlunoDemandasMap.put(alunoDemanda.getAluno(),demandasDoAluno);
				}
				demandasDoAluno.add(alunoDemanda);
			}
		}
		
		for (Entry<Aluno,List<AlunoDemanda>> entryAluno : alunoToAlunoDemandasMap.entrySet()) {
			Aluno aluno = entryAluno.getKey();
			List<AlunoDemanda> demandasDoAluno = entryAluno.getValue();
			
			// obtém as disciplinas associadas com o aluno em questão
			Map<Long,Disciplina> disciplinasDoAluno = new HashMap<Long,Disciplina>();
			for (AlunoDemanda alunoDemanda : demandasDoAluno) {
				Disciplina disciplina = alunoDemanda.getDemanda().getDisciplina();
				disciplinasDoAluno.put(disciplina.getId(),disciplina);
			}
			
			Set<Pair<Long,Long>> equivalenciasAplicaveisNasDisciplinasDoAluno = new HashSet<Pair<Long,Long>>();
			
			// verifica se alguma equivalência invalida as escolhas (disciplinas) do aluno em questão
			boolean detectouEquivalenciaInconsistentePrimeiroTeste = false;
			
			// primeiro, verifica se a aplicação de cada equivalência nas escolhas (disciplinas) do aluno gera repetição de disciplina
			for (Long disciplinaDoAlunoId : disciplinasDoAluno.keySet()) {
				// obtém as equivalências que podem ser aplicadas na disciplina em questão
				Set<Pair<Long,Long>> equivalenciasAplicaveisNaDisciplina = disciplinaEliminaIdToEquivalenciasCalculadasMap.get(disciplinaDoAlunoId);
				if (equivalenciasAplicaveisNaDisciplina != null) {
					// acumula todas as equivalências aplicáveis nas escolhas (disciplinas) do aluno em questão
					equivalenciasAplicaveisNasDisciplinasDoAluno.addAll(equivalenciasAplicaveisNaDisciplina);
					 
					for (Pair<Long,Long> equivalenciaAplicavelNaDisciplina : equivalenciasAplicaveisNaDisciplina) {
						Long disciplinaCursouId = equivalenciaAplicavelNaDisciplina.getLeft();
						Long disciplinaEliminaId = equivalenciaAplicavelNaDisciplina.getRight();
						// verifica se a equivalência aplicável na disciplina em questão irá transformá-la em outra disciplina
						// já escolhida pelo aluno
						Disciplina disciplinaDoAluno = disciplinasDoAluno.get(disciplinaCursouId);
						if (disciplinaDoAluno != null) {
							String equivalenciasInfo = "";
							Set<Pair<Long,Long>> equivalenciasOriginaisQueGeraramACalculada = equivalenciaCalculadaToEquivalenciasOriginaisMap.get(equivalenciaAplicavelNaDisciplina);
							if (equivalenciasOriginaisQueGeraramACalculada != null) {
								// a equivalência aplicável na disciplina é uma equivalência calculada
								equivalenciasInfo = imprimeEquivalenciasEmUmaString(equivalenciasOriginaisQueGeraramACalculada,disciplinasMap);
							} else {
								// a equivalência aplicável na disciplina é uma equivalência original
								Disciplina cursou = disciplinasMap.get(disciplinaCursouId);
								Disciplina elimina = disciplinasMap.get(disciplinaEliminaId);
								equivalenciasInfo = cursou.getCodigo() + "->" + elimina.getCodigo();
							}
							
							errors.add(HtmlUtils.htmlUnescape("A(s) equivalência(s) [" + equivalenciasInfo + "] pode(m) invalidar a(s) escolha(s) do aluno [" + aluno.getNome() + "] de matrícula [" + aluno.getMatricula() + "] por conta da disciplina [" + disciplinaDoAluno.getCodigo() + "]."));
							detectouEquivalenciaInconsistentePrimeiroTeste = true;
						}
					}
				}
			}
			
			if (!detectouEquivalenciaInconsistentePrimeiroTeste) {
				// organiza equivalências aplicáveis nas escolhas (disciplinas) do aluno pela extremidade Cursou da relação (Cursou,Elimina) 
				// [DisciplinaCursouId -> Set<Par<DisciplinaCursouId,DisciplinaEliminaId>>]
				Map<Long,Set<Pair<Long,Long>>> cursouToEquivalenciasAplicaveisMap = new HashMap<Long,Set<Pair<Long,Long>>>();
				for (Pair<Long,Long> parCursouElimina : equivalenciasAplicaveisNasDisciplinasDoAluno) {
					Long disciplinaCursouId = parCursouElimina.getLeft();
					Set<Pair<Long,Long>> equivalencias = cursouToEquivalenciasAplicaveisMap.get(disciplinaCursouId);
					if (equivalencias == null) {
						equivalencias = new HashSet<Pair<Long,Long>>();
						cursouToEquivalenciasAplicaveisMap.put(disciplinaCursouId,equivalencias);
					}
					equivalencias.add(parCursouElimina);
				}
				
				// segundo, verifica se há mais de uma equivalência, aplicável nas escolhas (disciplinas) do aluno, que leva para uma mesma disciplina
				for (Entry<Long,Set<Pair<Long,Long>>> entry : cursouToEquivalenciasAplicaveisMap.entrySet()) {
					Set<Pair<Long,Long>> equivalenciasComMesmaExtremidadeCursou = entry.getValue();
					// verifica se há mais de uma equivalência, aplicável nas escolhas (disciplinas) do aluno, que leva para uma mesma disciplina
					if (equivalenciasComMesmaExtremidadeCursou.size() > 1) {
						Set<Pair<Long,Long>> equivalenciasParaImprimir = new HashSet<Pair<Long,Long>>();
						for (Pair<Long,Long> e : equivalenciasComMesmaExtremidadeCursou) {
							Set<Pair<Long,Long>> equivalenciasOriginaisQueGeraramACalculada = equivalenciaCalculadaToEquivalenciasOriginaisMap.get(e);
							if (equivalenciasOriginaisQueGeraramACalculada != null) {
								// a equivalência 'e' é uma equivalência calculada
								equivalenciasParaImprimir.addAll(equivalenciasOriginaisQueGeraramACalculada);
							} else {
								// a equivalência 'e' é uma equivalência original
								equivalenciasParaImprimir.add(e);
							}
						}
						String equivalenciasStr = imprimeEquivalenciasEmUmaString(equivalenciasParaImprimir,disciplinasMap);
						// gera mensagem de erro
						errors.add(HtmlUtils.htmlUnescape("As equivalências [" + equivalenciasStr + "] pode(m) invalidar a(s) escolha(s) do aluno [" + aluno.getNome() + "] de matrícula [" + aluno.getMatricula() + "] pois levam para uma mesma disciplina."));
					}
				}
			}
		}
	}

	private void preencheEstruturasParaCheckDeEquivalencias(Parametro parametro, Map<Long,Disciplina> disciplinasMap, Map<Pair<Long,Long>,Set<Pair<Long,Long>>> equivalenciaCalculadaToEquivalenciasOriginaisMap, Map<Long,Set<Pair<Long,Long>>> disciplinaEliminaIdToEquivalenciasCalculadasMap) {
		// monta estruturas para armazenar as equivalências
		Set<Pair<Long,Long>> equivalenciasOriginais = new HashSet<Pair<Long,Long>>();
		// [DisciplinaEliminaId -> Par<DisciplinaCursouId,DisciplinaEliminaId>]
		Map<Long,Set<Pair<Long,Long>>> disciplinaEliminaIdToEquivalenciasOriginaisMap = new HashMap<Long,Set<Pair<Long,Long>>>();
		for(Disciplina disciplina : parametro.getCenario().getDisciplinas()){
			disciplinasMap.put(disciplina.getId(),disciplina);
			for(Equivalencia equivalencia : disciplina.getEliminam()){
				Disciplina cursou = equivalencia.getCursou();
				Disciplina elimina = equivalencia.getElimina();

				Pair<Long,Long> equivalenciaOriginal = Pair.create(cursou.getId(),elimina.getId());
					
				equivalenciasOriginais.add(equivalenciaOriginal);

				Set<Pair<Long,Long>> eliminaEquivalencias = disciplinaEliminaIdToEquivalenciasOriginaisMap.get(elimina.getId());
				if (eliminaEquivalencias == null) {
					eliminaEquivalencias = new HashSet<Pair<Long,Long>>();
					disciplinaEliminaIdToEquivalenciasOriginaisMap.put(elimina.getId(),eliminaEquivalencias);
				}
				eliminaEquivalencias.add(equivalenciaOriginal);

			}
		}
		
		// aplica equivalências umas nas outras
		Set<Pair<Long,Long>> equivalenciasAposAplicacoes = new HashSet<Pair<Long,Long>>();
		if (parametro.getConsiderarTransitividadeEmEquivalencia()) {
			Set<Pair<Long,Long>> equivalenciasAnalisadas = new HashSet<Pair<Long,Long>>(equivalenciasOriginais);
			boolean ocorreuAlgumaSubstituicao = false;
			do {
				ocorreuAlgumaSubstituicao = false;
				for (Pair<Long,Long> equivalenciaAnalisada : equivalenciasAnalisadas) {
					Long disCursouIdAnalisada = equivalenciaAnalisada.getLeft();
					Long disEliminaIdAnalisada = equivalenciaAnalisada.getRight();
					
					Set<Pair<Long,Long>> equivalenciasOriginaisAplicaveis = disciplinaEliminaIdToEquivalenciasOriginaisMap.get(disCursouIdAnalisada);
					if (equivalenciasOriginaisAplicaveis != null) {
						for (Pair<Long,Long> equivalenciaOriginalAplicavel : equivalenciasOriginaisAplicaveis) {
							Long disCursouIdOriginalAplicavel = equivalenciaOriginalAplicavel.getLeft();
							Pair<Long,Long> equivalenciaCalculada = Pair.<Long,Long>create(disCursouIdOriginalAplicavel,disEliminaIdAnalisada); 
							
							equivalenciasAposAplicacoes.add(equivalenciaCalculada);
							
							Set<Pair<Long,Long>> equivalenciasOriginaisQueGeraramACalculada = equivalenciaCalculadaToEquivalenciasOriginaisMap.get(equivalenciaCalculada);
							if (equivalenciasOriginaisQueGeraramACalculada == null) {
								equivalenciasOriginaisQueGeraramACalculada = new HashSet<Pair<Long,Long>>();
								equivalenciaCalculadaToEquivalenciasOriginaisMap.put(equivalenciaCalculada,equivalenciasOriginaisQueGeraramACalculada);
							}
							equivalenciasOriginaisQueGeraramACalculada.add(equivalenciaOriginalAplicavel);
						}
						ocorreuAlgumaSubstituicao = true;
					} else {
						equivalenciasAposAplicacoes.add(equivalenciaAnalisada);
					}
				}
				
				if (ocorreuAlgumaSubstituicao) {
					equivalenciasAnalisadas.clear();
					equivalenciasAnalisadas.addAll(equivalenciasAposAplicacoes);
					equivalenciasAposAplicacoes.clear();
				}
			} while (ocorreuAlgumaSubstituicao);
		} else {
			equivalenciasAposAplicacoes.addAll(equivalenciasOriginais);
		}
		
		// recalcula algumas estruturas com base nas equivalencias calculadas 
		for (Pair<Long,Long> equivalenciaCalculada : equivalenciasAposAplicacoes) {
			Long disEliminaIdCalculada = equivalenciaCalculada.getRight();
			
			Set<Pair<Long,Long>> equivalenciasCalculadas = disciplinaEliminaIdToEquivalenciasCalculadasMap.get(disEliminaIdCalculada);
			if (equivalenciasCalculadas == null) {
				equivalenciasCalculadas = new HashSet<Pair<Long,Long>>();
				disciplinaEliminaIdToEquivalenciasCalculadasMap.put(disEliminaIdCalculada,equivalenciasCalculadas);
			}
			equivalenciasCalculadas.add(equivalenciaCalculada);
		}
	}

	private String imprimeEquivalenciasEmUmaString(Set<Pair<Long,Long>> equivalencias, Map<Long,Disciplina> disciplinasMap) {
		// organiza as equivalências em uma string
		String equivalenciasStr = "";
		for (Pair<Long,Long> par : equivalencias) {
			Disciplina cursou = disciplinasMap.get(par.getLeft());
			Disciplina elimina = disciplinasMap.get(par.getRight());
			equivalenciasStr += cursou.getCodigo() + "->" + elimina.getCodigo() + "; ";
		}
		equivalenciasStr = equivalenciasStr.substring(0,equivalenciasStr.length()-2);
		return equivalenciasStr;
	}
	
	private void checkCampiSemDeslocamentos(Parametro parametro, List<String> errors) {
		if (parametro.getCampi().size() > 1) {
			// [ProfessorId -> List<Campus>]
			Map<Long,List<Campus>> professorIdToCampiMap = new HashMap<Long,List<Campus>>();
			// [CampusId -> Set<CampusId>]
			Map<Long,Set<Long>> campusIdToDeslocamentosDestinoCampusIdMap = new HashMap<Long,Set<Long>>();
			// preenche estruturas auxiliares
			for (Campus campus : parametro.getCampi()) {
				for (Professor professor : campus.getProfessores()) {
					List<Campus> campi = professorIdToCampiMap.get(professor.getId());
					if (campi == null) {
						campi = new ArrayList<Campus>();
						professorIdToCampiMap.put(professor.getId(),campi);
					}
					campi.add(campus);
				}
				
				for (DeslocamentoCampus deslocamentoDestino : campus.getDeslocamentos()) {
					Set<Long> campiIdsDestino = campusIdToDeslocamentosDestinoCampusIdMap.get(campus.getId());
					if (campiIdsDestino == null) {
						campiIdsDestino = new HashSet<Long>();
						campusIdToDeslocamentosDestinoCampusIdMap.put(campus.getId(),campiIdsDestino);
					}
					campiIdsDestino.add(deslocamentoDestino.getDestino().getId());
				}
			}
			
			// Identifica quais são os campi que irão exigir informação de deslocamento por conta de professores
			Set<Campus> campiQueExigemInformacaoDeDeslocamento = new HashSet<Campus>();
			for (Entry<Long,List<Campus>> entry : professorIdToCampiMap.entrySet()) {
				if (entry.getValue().size() > 1) {
					campiQueExigemInformacaoDeDeslocamento.addAll(entry.getValue());
				}
			}
			
			// Verifica se há informação de deslocamento entre os campi que exigem tal informação
			List<ParDTO<String,String>> deslocamentosNaoCadastrados = new ArrayList<ParDTO<String,String>>();
			for (Campus campus : campiQueExigemInformacaoDeDeslocamento) {
				// obtém que são os outros campi que exigem deslocamento
				Set<Campus> outrosCampi = new HashSet<Campus>(campiQueExigemInformacaoDeDeslocamento);
				outrosCampi.remove(campus);
				// obtém os destinos cadastrados para o campus em questão
				Set<Long> campiDestinosIds = campusIdToDeslocamentosDestinoCampusIdMap.get(campus.getId());
				// verifica se há algum campus que não aparece como destino cadastrado do campus em questão
				for (Campus outroCampus : outrosCampi) {
					if (campiDestinosIds == null || !campiDestinosIds.contains(outroCampus.getId())) {
						deslocamentosNaoCadastrados.add(ParDTO.create(campus.getCodigo(),outroCampus.getCodigo()));
					}
				}
			}
			
			if (!deslocamentosNaoCadastrados.isEmpty()) {
				errors.add(HtmlUtils.htmlUnescape("A requisição de otimização é multi-campi, porém, a informação de deslocamento entre campi não foi cadastrada para os pares (origem,destino) informados a seguir: " + deslocamentosNaoCadastrados.toString()));
			}
		}
	}

	private Parametro getParametroDefault(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Parametro parametro = new Parametro();

		List< Campus > listCampus = Campus.findByCenario( instituicaoEnsino, cenario );
		List< Turno > listTurnos = Turno.findAll( instituicaoEnsino, cenario );

		if ( cenario == null )
		{
			CenariosServiceImpl cenariosService = new CenariosServiceImpl();
			CenarioDTO cenarioDTO = cenariosService.getMasterData();
			cenario = Cenario.find( cenarioDTO.getId(), instituicaoEnsino );
		}

		parametro.setCenario( cenario );
		parametro.setCampi(new HashSet<Campus>(listCampus));
		parametro.setTurnos( new HashSet<Turno>(listTurnos) );
		
		return parametro;
	}

	@Override
	public Boolean isOptimizing( Long round )
	{
		ParametroConfiguracao config = ParametroConfiguracao.findConfiguracoes(getInstituicaoEnsinoUser());
		SolverClient solverClient = new SolverClient(config.getUrlOtimizacao(),config.getNomeOtimizacao());
		return ( !solverClient.isFinished( round ) );
	}

	@Override
	@Transactional
	public Map<String, List<String>> saveContent(CenarioDTO cenarioDTO,Long round) {
		Cenario cenario = Cenario.find(cenarioDTO.getId(),this.getInstituicaoEnsinoUser());
		
		initProgressReport("chaveOtimizacao");
		getProgressReport().setInitNewPartial("Carregando output");
		
		
		Map<String, List<String>> ret = new HashMap<String, List<String>>(2);
		ret.put("warning", new ArrayList<String>());
		ret.put("error", new ArrayList<String>());

		try {
			ParametroConfiguracao config = ParametroConfiguracao.findConfiguracoes(getInstituicaoEnsinoUser());
			SolverClient solverClient = new SolverClient(config.getUrlOtimizacao(),config.getNomeOtimizacao());

			getProgressReport().setPartial("Carregando xml( round ). Passo 1 de 4...");// TODO:
			
			byte[] xmlBytes = solverClient.getContent(round);

			if (xmlBytes == null) {
				ret.get("error").add("Erro no servidor");
				return ret;
			}

			JAXBContext jc = JAXBContext.newInstance("com.gapso.web.trieda.server.xml.output");
			Unmarshaller u = jc.createUnmarshaller();
			XMLInputFactory xif = XMLInputFactory.newInstance();
			XMLStreamReader xsr = xif.createXMLStreamReader(new ByteArrayInputStream(xmlBytes));
			
			TriedaOutput triedaOutput = (TriedaOutput) u.unmarshal(xsr);

			for (ItemError erro : triedaOutput.getErrors().getError()) {
				ret.get("error").add(erro.getMessage());
			}
			for (ItemWarning warning : triedaOutput.getWarnings().getWarning()) {
				ret.get("warning").add(warning.getMessage());
			}

			if (!triedaOutput.getErrors().getError().isEmpty()) {
				return ret;
			}

			getProgressReport().setPartial("Processando campi e turnos utilizados...");// TODO:
			Set<Campus> campi = new HashSet<Campus>();
			Set<Turno> turnos = new HashSet<Turno>();
			boolean ehTatico = false;
			// preenche horários
			Map<Long,HorarioAula> horarioAulaIdToHorarioAulaMap = new HashMap<Long,HorarioAula>();
			Map<String,Campus> campusIdToCampusMap =  Campus.buildCampusCodigoToCampusMap(new ArrayList<Campus>(cenario.getCampi()));
			for (Turno turno : cenario.getTurnos()) {
				for (HorarioAula horarioAula : turno.getHorariosAula()) {
					horarioAulaIdToHorarioAulaMap.put(horarioAula.getId(),horarioAula);
				}
			}
			for (ItemAtendimentoCampus atendimento: triedaOutput.getAtendimentos().getAtendimentoCampus())
			{
				if (campusIdToCampusMap.get(atendimento.getCampusCodigo()) != null)
					campi.add(campusIdToCampusMap.get(atendimento.getCampusCodigo()));
				else
				{
					ret.get("error").add("Campus Codigo("+ atendimento.getCampusCodigo() +") especificado na solução não esta cadastrado");
					return ret;
				}
					
				for (ItemAtendimentoUnidade atendimentoUnidade : atendimento.getAtendimentosUnidades().getAtendimentoUnidade())
				{
					for (ItemAtendimentoSala atendimentoSala : atendimentoUnidade.getAtendimentosSalas().getAtendimentoSala())
					{
						for (ItemAtendimentoDiaSemana atendimentoDia : atendimentoSala.getAtendimentosDiasSemana().getAtendimentoDiaSemana())
						{
							if (atendimentoDia.getAtendimentosTatico() != null )
							{
								System.out.println("AtendimentoTatico: " + atendimentoDia.getAtendimentosTatico());
								System.out.println("AtendimentoTaticoSize: " + atendimentoDia.getAtendimentosTatico().getAtendimentoTatico().size());
								ehTatico = true;
								for (ItemAtendimentoTatico atendimentoTatico : atendimentoDia.getAtendimentosTatico().getAtendimentoTatico())
								{
									for (Integer id : atendimentoTatico.getHorariosAula().getHorarioAulaId())
									{
										if (horarioAulaIdToHorarioAulaMap.get(Long.valueOf(id)).getTurno() != null)
											turnos.add(horarioAulaIdToHorarioAulaMap.get(Long.valueOf(id)).getTurno());
										else
										{
											ret.get("error").add("Turno id("+ id +") especificado na solução não esta cadastrado");
											return ret;
										}
									}
								}
							}
							else if (atendimentoDia.getAtendimentosTurnos() != null)
							{
								for (ItemAtendimentoTurno atendimentoTurno : atendimentoDia.getAtendimentosTurnos().getAtendimentoTurno())
								{
									for (ItemAtendimentoHorarioAula atendimentoHorarioAula : atendimentoTurno.getAtendimentosHorariosAula().getAtendimentoHorarioAula())
									{
										if (horarioAulaIdToHorarioAulaMap.get(Long.valueOf(atendimentoHorarioAula.getHorarioAulaId())).getTurno() != null)
											turnos.add(horarioAulaIdToHorarioAulaMap.get(Long.valueOf(atendimentoHorarioAula.getHorarioAulaId())).getTurno());
										else
										{
											ret.get("error").add("Turno id("+ atendimentoTurno.getTurnoId() +") especificado na solução não esta cadastrado");
											return ret;
										}
									}
								}
							}
						}
					}
				}
			}
			for (Campus campus : campi)
			{
				System.out.println("campus:" + campus.getNome());
			}
			getProgressReport().setPartial("Processando campi e turnos utilizados FINALIZADO");// TODO:
			getProgressReport().setPartial("Carregando xml( round ). Passo 1 de 4 FINALIZADO");// TODO:

			SolverOutput solverOutput = new SolverOutput(getInstituicaoEnsinoUser(), cenario, triedaOutput);
			getProgressReport().setPartial("Atualizando demandas de alunos. Passo 2 de 4");// TODO:
			solverOutput.atualizarAlunosDemanda(campi,turnos);
			getProgressReport().setPartial("Atualizando demandas de alunos. Passo 2 de 4 FINALIZADO");// TODO:

			if (ehTatico) {
				getProgressReport().setPartial("Gerando atendimentos tatico. Passo 3 de 4");// TODO:
				solverOutput.generateAtendimentosTatico(turnos);
				getProgressReport().setPartial("Gerando atendimentos tatico. Passo 3 de 4 FINALIZADO");// TODO:

				getProgressReport().setPartial("Salvando atendimentos tatico. Passo 4 de 4");// TODO:
				solverOutput.salvarAtendimentosTatico(campi,turnos);
				getProgressReport().setPartial("Salvando atendimentos tatico. Passo 4 de 4 FINALIZADO");// TODO:
			} else {
				getProgressReport().setPartial("Gerando atendimentos operacional. Passo 3 de 4");// TODO:
				solverOutput.generateAtendimentosOperacional(turnos);
				getProgressReport().setPartial("Gerando atendimentos operacional. Passo 3 de 4 FINALIZADO");// TODO:

				getProgressReport().setPartial("Salvando atendimentos operacional. Passo 4 de 4");// TODO:
				solverOutput.salvarAtendimentosOperacional(campi, turnos);
				getProgressReport().setPartial("Salvando atendimentos operacional. Passo 4 de 4 FINALIZADO");// TODO:
			}
			getProgressReport().setPartial("Atualizando motivos de nao atendimento FINALIZADO");// TODO:
			solverOutput.generateMotivosNaoAtendimento();
		} catch (JAXBException e) {
			e.printStackTrace();
			ret.get("error").add("Erro ao salvar o resultado na base de dados");
			return ret;
		} catch (Exception e) {
			e.printStackTrace();
			ret.get("error").add(e.getMessage());
			return ret;
		}

		
		getProgressReport().setPartial("Etapa concluída");
		getProgressReport().finish();
		
		return ret;
	}

	public void setProgressReport(List<String> fbl){
		progressReport = new ProgressReportListWriter(fbl);
	}
	
	public void setProgressReport(File f) throws IOException{
		progressReport = new ProgressReportFileWriter(f);
	}
	
	public ProgressReportWriter getProgressReport(){
		return progressReport;
	}
}
