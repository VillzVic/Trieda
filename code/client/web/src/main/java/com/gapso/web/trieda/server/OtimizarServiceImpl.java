package com.gapso.web.trieda.server;

import java.io.ByteArrayOutputStream;
import java.io.StringReader;
import java.util.ArrayList;
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
import javax.xml.transform.stream.StreamSource;

import org.springframework.stereotype.Repository;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.DeslocamentoCampus;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Equivalencia;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Parametro;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.RequisicaoOtimizacao;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Usuario;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.Grafo;
import com.gapso.web.trieda.server.util.Profundidade;
import com.gapso.web.trieda.server.util.SolverInput;
import com.gapso.web.trieda.server.util.SolverOutput;
import com.gapso.web.trieda.server.util.solverclient.SolverClient;
import com.gapso.web.trieda.server.xml.input.TriedaInput;
import com.gapso.web.trieda.server.xml.output.ItemError;
import com.gapso.web.trieda.server.xml.output.ItemWarning;
import com.gapso.web.trieda.server.xml.output.TriedaOutput;
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
public class OtimizarServiceImpl extends RemoteService implements OtimizarService {
	private static final long serialVersionUID = 5716065588362358065L;
	private static final String solverName = "trieda";
	private static final String linkSolverDefault = "http://localhost:8080/SolverWS";
	
	/**
	 * @see com.gapso.web.trieda.shared.services.OtimizarService#checkInputDataBeforeRequestOptimization(com.gapso.web.trieda.shared.dtos.ParametroDTO)
	 */
	@Override
	public ErrorsWarningsInputSolverDTO checkInputDataBeforeRequestOptimization(ParametroDTO parametroDTO) throws Exception {
		ErrorsWarningsInputSolverDTO response = new ErrorsWarningsInputSolverDTO();
		if (parametroDTO.isValid()) {
			Parametro parametro = ConvertBeans.toParametro(parametroDTO);
			List<String> warnings = new ArrayList<String>();
			List<String> errors = new ArrayList<String>();
			
			// realiza verificações
			System.out.print("checkDisciplinasSemCurriculo(parametro,warnings);");long start = System.currentTimeMillis(); // TODO: retirar
			checkDisciplinasSemCurriculo(parametro,warnings);
			long time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
			
			System.out.print("checkDisciplinasSemLaboratorios(parametro,errors);");start = System.currentTimeMillis(); // TODO: retirar
			checkDisciplinasSemLaboratorios(parametro,errors);
			time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
			
			System.out.print("checkMaxCreditosSemanaisPorPeriodo_e_DisciplinasRepetidasPorCurriculo");start = System.currentTimeMillis(); // TODO: retirar
			checkMaxCreditosSemanaisPorPeriodo_e_DisciplinasRepetidasPorCurriculo(parametro,getInstituicaoEnsinoUser(),errors);
			time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar

			System.out.print("checkDemandasComDisciplinasSemCurriculo(parametro,errors);");start = System.currentTimeMillis(); // TODO: retirar
			checkDemandasComDisciplinasSemCurriculo(parametro,errors);
			time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
			
			if (parametro.getConsiderarEquivalencia()) {
				System.out.print("checkCicloDisciplinasEquivalentes(parametro.getCenario(),errors);");start = System.currentTimeMillis(); // TODO: retirar
				boolean detectouCiclo = checkCicloDisciplinasEquivalentes(parametro.getCenario(),errors);
				time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
				
				if (!detectouCiclo) {
					System.out.print("checkEquivalenciasQueGeramDisciplinasRepetidasEmUmMesmoCurriculo(parametro,errors);");start = System.currentTimeMillis(); // TODO: retirar
					checkEquivalenciasQueGeramDisciplinasRepetidasEmUmMesmoCurriculo(parametro,errors);
					time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
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
			if (parametroDTO.getTurnoId() == null) {
				if (!message.isEmpty()) {
					message += " ";
				}
				message += HtmlUtils.htmlUnescape("O turno n&atilde;o foi informado.");
			}
			
			List<String> errors = new ArrayList<String>();
			errors.add(message);
			
			response.setErrors(errors);
		}
	
		return response;
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
				List<Parametro> parametros = Parametro.findAll(parametro.getModoOtimizacao(),parametro.getOtimizarPor(),parametro.getFuncaoObjetivo(),parametro.getTurno(),parametro.getCenario(),parametro.getInstituicaoEnsino());
				Long maxId = -1L;
				Set<Campus> campi = parametro.getCampi();
				for (Parametro p : parametros) {
					if (p.getId() > maxId && campi.containsAll(p.getCampi())) {
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
		if (!parametroDTO.isValid()) {
			String errorMessage = "";
			if (parametroDTO.getCampi() == null || parametroDTO.getCampi().isEmpty()) {
				errorMessage += HtmlUtils.htmlUnescape("Nenhum campus foi selecionado.");
			}
			if (parametroDTO.getTurnoId() == null) {
				errorMessage += HtmlUtils.htmlUnescape(" Nenhum turno foi selecionado.");
			}
			throw new TriedaException(errorMessage);
		}

		try {
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
			SolverClient solverClient = new SolverClient(linkSolverDefault,solverName);
	
			Long round = solverClient.requestOptimization(fileBytes);
			
			return ParDTO.<Long,ParametroDTO>create(round,ConvertBeans.toParametroDTO(parametro));
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
	}
	
	/** 
	 * @see com.gapso.web.trieda.shared.services.OtimizarService#consultaRequisicoesDeOtimizacao()
	 */
	@Override
	public List<RequisicaoOtimizacaoDTO> consultaRequisicoesDeOtimizacao() throws TriedaException {
		try {
			// obtém o usuário logado em questão
			Usuario usuarioAtual = getUsuario();
			// obtém as requisições de otimização relacionadas com o usuário atual
			List<RequisicaoOtimizacao> requisicoesOtimizacao = RequisicaoOtimizacao.findBy(usuarioAtual);
			// ...
			List<RequisicaoOtimizacaoDTO> requisicoesOtimizacaoDTOs = new ArrayList<RequisicaoOtimizacaoDTO>();
			SolverClient solverClient = new SolverClient(linkSolverDefault,solverName);
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
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
	}
	
	/**
	 * @see com.gapso.web.trieda.shared.services.OtimizarService#cancelaRequisicaoDeOtimizacao(java.lang.Long)
	 */
	@Override
	public boolean cancelaRequisicaoDeOtimizacao(Long round) throws TriedaException {
		try {
			SolverClient solverClient = new SolverClient(linkSolverDefault,solverName);
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

	private void checkDisciplinasSemCurriculo(Parametro parametro, List<String> warnings) {
		Map<Long,Long> disciplinasComCurriculoMap = new HashMap<Long,Long>();
		for (Curso curso : parametro.getCenario().getCursos()) {
			for (Curriculo curriculo : curso.getCurriculos()) {
				for (CurriculoDisciplina curriculoDisciplina : curriculo.getDisciplinas()) {
					disciplinasComCurriculoMap.put(curriculoDisciplina.getDisciplina().getId(),curriculoDisciplina.getDisciplina().getId());
				}
			}
		}

		for (Disciplina disciplina : parametro.getCenario().getDisciplinas()) {
			if (disciplinasComCurriculoMap.get(disciplina.getId()) == null) {
				warnings.add(HtmlUtils.htmlUnescape("A disciplina " + disciplina.getCodigo() + " n&atilde;o foi associada a nenhuma matriz curricular."));
				System.out.println("A disciplina " + disciplina.getCodigo() + " n&atilde;o foi associada a nenhuma matriz curricular.");
			}
		}
	}

	private void checkDisciplinasSemLaboratorios(Parametro parametro, List<String> errors) {
		for (Campus campus : parametro.getCampi()) {
			checkDisciplinasSemLaboratorios(campus,errors);
		}
	}
	
	private void checkDisciplinasSemLaboratorios(Campus campus, List<String> errors) {
		// coleta as disciplinas que serão enviadas para o solver e que exigem laboratório
		Set<Disciplina> disciplinasQueExigemLaboratio = new HashSet<Disciplina>();
		for (Oferta oferta : campus.getOfertas()) {
			for (Demanda demanda : oferta.getDemandas()) {
				Disciplina disciplina = demanda.getDisciplina();
				if (disciplina.getLaboratorio()) {
					disciplinasQueExigemLaboratio.add(disciplina);
				}
			}
		}
		
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
						for (Sala sala : curriculoDisciplina.getSalas()) {
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
	}
	
	private void checkMaxCreditosSemanaisPorPeriodo_e_DisciplinasRepetidasPorCurriculo(Parametro parametro, InstituicaoEnsino instituicaoEnsino, List<String> errors) {
		// obtém os currículos do campus selecionado para otimização
		Set<Curriculo> curriculosDoCampusSelecionado = new HashSet<Curriculo>();
		for (Campus campus : parametro.getCampi()) {
			for (Oferta oferta : campus.getOfertas()) {
				curriculosDoCampusSelecionado.add(oferta.getCurriculo());
			}
		}
		
		// [CurriculoId -> Máximo Créditos Semanais]
		Map<Long,Integer> maxCreditosSemanaisPorSemanaLetivaMap = new HashMap<Long,Integer>();
 
		for (Curriculo curriculo : parametro.getCenario().getCurriculos()) {
			// filtra os currículos do campus selecionado para otimização
			if (curriculosDoCampusSelecionado.contains(curriculo)) {
				// obtém o máximo de créditos semanais da semana letiva associada com o currículo
				Integer maxCreditosSemanais = maxCreditosSemanaisPorSemanaLetivaMap.get(curriculo.getSemanaLetiva().getId());
				if (maxCreditosSemanais == null) {
					maxCreditosSemanais = curriculo.getSemanaLetiva().calcTotalCreditosSemanais(parametro.getTurno());
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
					errors.add(HtmlUtils.htmlUnescape("Na matriz curricular [" + curriculo.getCodigo() + "] existem períodos que violam a quantidade máxima de créditos semanais da Semana Letiva. Máximo de Créditos Semanais = " + maxCreditosSemanais + ". Período(TotalCréditos) = " + periodosQueViolamMaxCreditosSemanais.toString()));
					System.out.println("Na matriz curricular [" + curriculo.getCodigo() + "] existem períodos que violam a quantidade máxima de créditos semanais da Semana Letiva. Máximo de Créditos Semanais = " + maxCreditosSemanais + ". Período(TotalCréditos) = " + periodosQueViolamMaxCreditosSemanais.toString());
				}
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
	
	private boolean checkCicloDisciplinasEquivalentes(Cenario cenario, List<String> errors){
		boolean detectouCiclo = false;
		
		int id = 0;
		Integer cIndex = 0, eIndex = 0, maxIndex = 0;
		List<Long> nodeMap = new ArrayList<Long>();
		List<Disciplina> disMap = new ArrayList<Disciplina>();
		Set<Pair<Integer, Integer>> pairs = new HashSet<Pair<Integer, Integer>>();
//		Map<Long,Integer> disciplinaIdToQtdDemandadaMap = new HashMap<Long,Integer>();

		// Para cada disciplina, obtem-se as suas equivalencias para montar tres estruturas:
		// -> disMap mapeia o index associado a cada disciplina identificada
		// -> nodeMap faz o controle de qual disciplina estah mapeada ou nao
		// -> pairs obtem a relação de equivalencia de uma disciplina com outra. 
		for(Disciplina disciplina : cenario.getDisciplinas()){
//			Integer qtdDemandada = 0;
//			for (Demanda demanda : disciplina.getDemandas()) {
//				qtdDemandada += demanda.getQuantidade();
//			}
//			disciplinaIdToQtdDemandadaMap.put(disciplina.getId(),qtdDemandada);
			for(Equivalencia equivalencia : disciplina.getEquivalencias()){
				Disciplina cursou = equivalencia.getCursou();
				if((cIndex = (Integer) nodeMap.indexOf(cursou.getId())) == -1){
					cIndex = id++;
					nodeMap.add(cursou.getId());
					disMap.add(cursou);
				}
				for(Disciplina elimina : equivalencia.getElimina()){
					if((eIndex = (Integer) nodeMap.indexOf(elimina.getId())) == -1){
						eIndex = id++;
						nodeMap.add(elimina.getId());
						disMap.add(elimina);
					}
					pairs.add(Pair.create(cIndex, eIndex));
					if (cIndex > maxIndex) {
						maxIndex = cIndex;
					}
					if (eIndex > maxIndex) {
						maxIndex = eIndex;
					}
				}
			}
		}
		
		// Constroi um grafo a partir dos pares obtidos e realiza a busca para verificar
		// a existencia de ciclos. Nesse grafo, os indices associados aos nos sao os
		// mesmos identificados em disMap. 
		
		Grafo g = new Grafo(maxIndex+1);
		for(Pair<Integer, Integer> par: pairs) {
			g.insereArco(par.getLeft(), par.getRight());
		}
		
		Profundidade p = new Profundidade();
		if(!p.testeCiclos(g)){
			
			// Obtem-se os ciclos identificados e os imprime num formato adequado para
			// informar ao usuario quais sao eles.
			List<List<Integer>> ciclos = p.getCiclos();
			int i = 1;
			String ciclosStr = "Foram detectados os seguintes ciclos entre disciplinas equivalentes:<br /><br />";
			for(List<Integer> ciclo: ciclos){
				ciclosStr += i++ + ") ";
				for(Integer idc: ciclo){
					Disciplina dis = disMap.get(idc);
					ciclosStr += dis.getNome() + " [" + dis.getCodigo() + "] -> ";
				}
				String sugestaoParaEliminarCiclo = "";
//				sugestaoParaEliminarCiclo = avaliaSugestoesParaEliminacaoDeCiclos(disMap,disciplinaIdToQtdDemandadaMap,ciclo);
				ciclosStr = ciclosStr.substring(0, ciclosStr.length() - 3) + sugestaoParaEliminarCiclo + "<br /><br />";
			}
			ciclosStr += " \n";
			System.out.println(ciclosStr.replaceAll("<br />", "\n"));
			errors.add(HtmlUtils.htmlUnescape(ciclosStr));
			
			detectouCiclo = true;
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
		// monta estruturas para armazenar as equivalências
		Set<Pair<Long,Long>> equivalenciasOriginais = new HashSet<Pair<Long,Long>>();
		// [DisciplinaId -> Disciplina]
		Map<Long,Disciplina> disciplinasMap = new HashMap<Long,Disciplina>();
		// [DisciplinaEliminaId -> Par<DisciplinaCursouId,DisciplinaEliminaId>]
		Map<Long,Set<Pair<Long,Long>>> disciplinaEliminaIdToEquivalenciasOriginaisMap = new HashMap<Long,Set<Pair<Long,Long>>>();
		for(Disciplina disciplina : parametro.getCenario().getDisciplinas()){
			disciplinasMap.put(disciplina.getId(),disciplina);
			for(Equivalencia equivalencia : disciplina.getEquivalencias()){
				Disciplina cursou = equivalencia.getCursou();
				for(Disciplina elimina : equivalencia.getElimina()){
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
		}
		
		// aplica equivalências umas nas outras
		boolean consideraTransitividadeNasRegrasDeEquivalencia = false;
		Set<Pair<Long,Long>> equivalenciasAposAplicacoes = new HashSet<Pair<Long,Long>>();
		Map<Pair<Long,Long>,Set<Pair<Long,Long>>> equivalenciaCalculadaToEquivalenciasOriginaisMap = new HashMap<Pair<Long,Long>,Set<Pair<Long,Long>>>();
		if (consideraTransitividadeNasRegrasDeEquivalencia) {
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
		// [DisciplinaEliminaId -> Par<DisciplinaCursouId,DisciplinaEliminaId>]
		Map<Long,Set<Pair<Long,Long>>> disciplinaEliminaIdToEquivalenciasCalculadasMap = new HashMap<Long,Set<Pair<Long,Long>>>();
		for (Pair<Long,Long> equivalenciaCalculada : equivalenciasAposAplicacoes) {
			Long disEliminaIdCalculada = equivalenciaCalculada.getRight();
			
			Set<Pair<Long,Long>> equivalenciasCalculadas = disciplinaEliminaIdToEquivalenciasCalculadasMap.get(disEliminaIdCalculada);
			if (equivalenciasCalculadas == null) {
				equivalenciasCalculadas = new HashSet<Pair<Long,Long>>();
				disciplinaEliminaIdToEquivalenciasCalculadasMap.put(disEliminaIdCalculada,equivalenciasCalculadas);
			}
			equivalenciasCalculadas.add(equivalenciaCalculada);
		}
		
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
							
							errors.add(HtmlUtils.htmlUnescape("A(s) equivalência(s) [" + equivalenciasInfo + "] invalida(m) a matriz curricular [" + curriculo.getCodigo() + "] por conta da disciplina [" + curriculoDisciplina.getDisciplina().getCodigo() + "] no período [" + curriculoDisciplina.getPeriodo() + "]."));
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
						errors.add(HtmlUtils.htmlUnescape("As equivalências [" + equivalenciasStr + "] invalidam a matriz curricular [" + curriculo.getCodigo() + "] pois levam para uma mesma disciplina."));
					}
				}
			}
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

		List< Campus > listCampus = Campus.findAll( instituicaoEnsino );
		List< Turno > listTurnos = Turno.findAll( instituicaoEnsino );

		if ( cenario == null )
		{
			CenariosServiceImpl cenariosService = new CenariosServiceImpl();
			CenarioDTO cenarioDTO = cenariosService.getMasterData();
			cenario = Cenario.find( cenarioDTO.getId(), instituicaoEnsino );
		}

		parametro.setCenario( cenario );
		parametro.setCampi(new HashSet<Campus>(listCampus));
		parametro.setTurno( ( listTurnos == null || listTurnos.size() == 0 ? null : listTurnos.get( 0 ) ) );
		
		return parametro;
	}

	@Override
	public Boolean isOptimizing( Long round )
	{
		SolverClient solverClient = new SolverClient(linkSolverDefault,solverName);
		return ( !solverClient.isFinished( round ) );
	}

	@Override
	public Map< String, List< String > > saveContent(
		CenarioDTO cenarioDTO, Long round )
	{
		Cenario cenario = Cenario.find(
			cenarioDTO.getId(), this.getInstituicaoEnsinoUser() );

		Map< String, List< String > > ret
			= new HashMap< String, List< String > >( 2 );

		ret.put( "warning", new ArrayList< String >() );
		ret.put( "error", new ArrayList< String >() );

		try
		{
			SolverClient solverClient = new SolverClient(linkSolverDefault,solverName);

			System.out.println("solverClient.getContent( round ) ...");// TODO: LOG
			byte [] xmlBytes = solverClient.getContent( round );
			System.out.println("solverClient.getContent( round ) FINALIZADO");// TODO: LOG

			if ( xmlBytes == null )
			{
				ret.get( "error" ).add( "Erro no servidor" );
				return ret;
			}

			JAXBContext jc = JAXBContext.newInstance(
				"com.gapso.web.trieda.server.xml.output" );

			Unmarshaller u = jc.createUnmarshaller();
			System.out.println("StringBuffer xmlStr = new StringBuffer( new String( xmlBytes ) ); ...");// TODO: LOG
			StringBuffer xmlStr = new StringBuffer( new String( xmlBytes ) );
			System.out.println("StringBuffer xmlStr = new StringBuffer( new String( xmlBytes ) ); FINALIZADO");// TODO: LOG
			
			System.out.println("u.unmarshal(new StreamSource(new StringReader(xmlStr.toString()))); ...");// TODO: LOG
			TriedaOutput triedaOutput = ( TriedaOutput ) u.unmarshal(new StreamSource(new StringReader(xmlStr.toString())));
			System.out.println("u.unmarshal(new StreamSource(new StringReader(xmlStr.toString()))); FINALIZADO");// TODO: LOG

			for ( ItemError erro : triedaOutput.getErrors().getError() )
			{
				ret.get( "error" ).add( erro.getMessage() );
			}

			for ( ItemWarning warning : triedaOutput.getWarnings().getWarning() )
			{
				ret.get( "warning" ).add( warning.getMessage() );
			}

			if ( !triedaOutput.getErrors().getError().isEmpty() )
			{
				return ret;
			}

			Parametro parametro = cenario.getUltimoParametro(
				this.getInstituicaoEnsinoUser() );

			SolverOutput solverOutput = new SolverOutput(
				getInstituicaoEnsinoUser(), cenario, triedaOutput );

			System.out.println("solverOutput.atualizarAlunosDemanda(parametro.getCampi(),parametro.getTurno()); ...");// TODO: LOG
			solverOutput.atualizarAlunosDemanda(parametro.getCampi(),parametro.getTurno());
			System.out.println("solverOutput.atualizarAlunosDemanda(parametro.getCampi(),parametro.getTurno()); FINALIZADO");// TODO: LOG

			if (parametro.isTatico()) {
				System.out.println("solverOutput.generateAtendimentosTatico(); ...");// TODO: LOG
				solverOutput.generateAtendimentosTatico(parametro.getTurno());
				System.out.println("solverOutput.generateAtendimentosTatico(); FINALIZADO");// TODO: LOG

				System.out.println("solverOutput.salvarAtendimentosTatico(parametro.getCampus(),parametro.getTurno()); ...");// TODO: LOG
				solverOutput.salvarAtendimentosTatico(parametro.getCampi(),parametro.getTurno());
				System.out.println("solverOutput.salvarAtendimentosTatico(parametro.getCampus(),parametro.getTurno()); FINALIZADO");// TODO: LOG
			} else {
				System.out.println("solverOutput.generateAtendimentosOperacional(); ...");// TODO: LOG
				solverOutput.generateAtendimentosOperacional(parametro.getTurno());
				System.out.println("solverOutput.generateAtendimentosOperacional(); FINALIZADO");// TODO: LOG
				
				System.out.println("solverOutput.salvarAtendimentosOperacional(parametro.getCampi(),parametro.getTurno()); ...");// TODO: LOG
				solverOutput.salvarAtendimentosOperacional(parametro.getCampi(),parametro.getTurno());
				System.out.println("solverOutput.salvarAtendimentosOperacional(parametro.getCampi(),parametro.getTurno()); FINALIZADO");// TODO: LOG
			}
		}
		catch ( JAXBException e )
		{
			e.printStackTrace();
			ret.get( "error" ).add( "Erro ao salvar o resultado na base de dados" );
			return ret;
		}
		catch(Exception e){
			e.printStackTrace();
			ret.get("error").add(e.getMessage());
			return ret;
		}

		return ret;
	}
}
