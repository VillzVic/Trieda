package com.gapso.web.trieda.server;

import java.io.ByteArrayOutputStream;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
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
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Equivalencia;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Parametro;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
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
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.services.OtimizarService;
import com.google.gwt.dev.util.Pair;

@Transactional
@Service
@Repository
public class OtimizarServiceImpl
	extends RemoteService
	implements OtimizarService
{
	private static final long serialVersionUID = 5716065588362358065L;
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
				checkCicloDisciplinasEquivalentes(parametro.getCenario(),errors);
				time = (System.currentTimeMillis() - start)/1000;System.out.println(" tempo = " + time + " segundos"); // TODO: retirar
			}
			checksCleiton();//TODO: revisar
			
			response.setErrors(errors);
			response.setWarnings(warnings);
		} else {
			String message = "";
			if (parametroDTO.getCampusId() == null) {
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
		// coleta as disciplinas que serão enviadas para o solver e que exigem laboratório
		Set<Disciplina> disciplinasQueExigemLaboratio = new HashSet<Disciplina>();
		for (Oferta oferta : parametro.getCampus().getOfertas()) {
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
						if (oferta.getCampus().equals(parametro.getCampus())) {
							curriculoTemOfertaNoCampusASerOtimizado = true;
							break;
						}
					}
					
					// apenas faz validação se currículo tem oferta no campus a ser otimizado
					if (curriculoTemOfertaNoCampusASerOtimizado) {
						boolean estaAssociadoComAlgumLaboratorio = false;
						for (Sala sala : curriculoDisciplina.getSalas()) {
							if (sala.isLaboratorio() && sala.getUnidade().getCampus().equals(parametro.getCampus())) {
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
				errors.add(HtmlUtils.htmlUnescape("A disciplina [" + disciplina.getCodigo() + "], que exige laboratório, contém pares (Matriz Curricular, Período) não associados a nenhum laboratório do campus [" + parametro.getCampus().getCodigo() + ", são eles: " + pares));
				System.out.println("A disciplina [" + disciplina.getCodigo() + "], que exige laboratório, contém pares (Matriz Curricular, Período) não associados a nenhum laboratório do campus [" + parametro.getCampus().getCodigo() + ", são eles: " + pares);
			}
		}
	}
	
	private void checkMaxCreditosSemanaisPorPeriodo_e_DisciplinasRepetidasPorCurriculo(Parametro parametro, InstituicaoEnsino instituicaoEnsino, List<String> errors) {
		// obtém os currículos do campus selecionado para otimização
		Set<Curriculo> curriculosDoCampusSelecionado = new HashSet<Curriculo>();
		for (Oferta oferta : parametro.getCampus().getOfertas()) {
			curriculosDoCampusSelecionado.add(oferta.getCurriculo());
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
		for (Oferta oferta : parametro.getCampus().getOfertas()) {
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
	
	private void checkCicloDisciplinasEquivalentes(Cenario cenario, List<String> errors){
		int id = 0;
		Integer cIndex = 0, eIndex = 0;
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
				}
			}
		}
		
		// Constroi um grafo a partir dos pares obtidos e realiza a busca para verificar
		// a existencia de ciclos. Nesse grafo, os indices associados aos nos sao os
		// mesmos identificados em disMap. 
		
		Grafo g = new Grafo(Math.max(cIndex,eIndex)+1);
		for(Pair<Integer, Integer> par: pairs)
			g.insereArco(par.getLeft(), par.getRight());
		
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
		}
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

	@Override
	@Transactional
	public ParametroDTO getParametro( CenarioDTO cenarioDTO )
	{
		InstituicaoEnsino instituicaoEnsino = this.getInstituicaoEnsinoUser();

		Cenario cenario = Cenario.find( cenarioDTO.getId(), instituicaoEnsino );
		Parametro parametro = cenario.getUltimoParametro( instituicaoEnsino );

		if ( parametro == null )
		{
			parametro = this.getParametroDefault( instituicaoEnsino, cenario );
		}

		ParametroDTO parametroDTO = ConvertBeans.toParametroDTO( parametro );
		return parametroDTO;
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
		parametro.setCampus( ( listCampus == null || listCampus.size() == 0 ? null : listCampus.get( 0 ) ) );
		parametro.setTurno( ( listTurnos == null || listTurnos.size() == 0 ? null : listTurnos.get( 0 ) ) );
		
		return parametro;
	}

	@Override
	@Transactional
	public Long sendInput( ParametroDTO parametroDTO )
	{
		if ( parametroDTO.getCampusId() == null
			|| parametroDTO.getTurnoId() == null )
		{
			String message = "";

			if ( parametroDTO.getCampusId() == null )
			{
				message += HtmlUtils.htmlUnescape(
					"o campus n&atilde;o foi informado, " );
			}

			if ( parametroDTO.getTurnoId() == null )
			{
				message += HtmlUtils.htmlUnescape(
					"o turno n&atilde;o foi informado, " );
			}

			message = message.substring( 0, message.length() - 2 );
			throw new RuntimeException( message );
		}

		Parametro parametro = ConvertBeans.toParametro( parametroDTO );

		parametro.setId( null );
		parametro.flush();
		parametro.save();

		Cenario cenario = parametro.getCenario();
		cenario.getParametros().add( parametro );
		List< Campus > campi = new ArrayList< Campus >( 1 );
		campi.add( parametro.getCampus() );

		TriedaInput triedaInput = null;
		SolverInput solverInput = new SolverInput(
			getInstituicaoEnsinoUser(), cenario, parametro, campi );

		if ( parametro.isTatico() )
		{
			triedaInput = solverInput.generateTaticoTriedaInput();
		}
		else
		{
			triedaInput = solverInput.generateOperacionalTriedaInput();
		}

		byte [] fileBytes = null;

		try
		{
			final ByteArrayOutputStream temp = new ByteArrayOutputStream();

			JAXBContext jc = JAXBContext.newInstance(
				"com.gapso.web.trieda.server.xml.input" );

			Marshaller m = jc.createMarshaller();
			m.setProperty( Marshaller.JAXB_FORMATTED_OUTPUT, true );
			m.setProperty( Marshaller.JAXB_ENCODING, "ISO-8859-1" );
			m.marshal( triedaInput, temp );
			fileBytes = temp.toByteArray();
		}
		catch ( Exception e )
		{
			e.printStackTrace();
		}

		SolverClient solverClient
			= new SolverClient( getLinkSolver(), "trieda" );

		return solverClient.requestOptimization( fileBytes );
	}

	@Override
	public Boolean isOptimizing( Long round )
	{
		SolverClient solverClient
			= new SolverClient( getLinkSolver(), "trieda" );

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
			SolverClient solverClient = new SolverClient(
				getLinkSolver(), "trieda" );

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

			System.out.println("solverOutput.salvarAlunosDemanda(parametro.getCampus(),parametro.getTurno()); ...");// TODO: LOG
			solverOutput.salvarAlunosDemanda(parametro.getCampus(),parametro.getTurno());
			System.out.println("solverOutput.salvarAlunosDemanda(parametro.getCampus(),parametro.getTurno()); FINALIZADO");// TODO: LOG

			if ( parametro.isTatico() )
			{
				System.out.println("solverOutput.generateAtendimentosTatico(); ...");// TODO: LOG
				solverOutput.generateAtendimentosTatico();
				System.out.println("solverOutput.generateAtendimentosTatico(); FINALIZADO");// TODO: LOG

				System.out.println("solverOutput.salvarAtendimentosTatico(parametro.getCampus(),parametro.getTurno()); ...");// TODO: LOG
				solverOutput.salvarAtendimentosTatico(parametro.getCampus(),parametro.getTurno());
				System.out.println("solverOutput.salvarAtendimentosTatico(parametro.getCampus(),parametro.getTurno()); FINALIZADO");// TODO: LOG
			}
			else
			{
				solverOutput.generateAtendimentosOperacional();

				solverOutput.salvarAtendimentosOperacional(
					parametro.getCampus(), parametro.getTurno() );
			}
		}
		catch ( JAXBException e )
		{
			e.printStackTrace();
			ret.get( "error" ).add( "Erro ao salvar o resultado na base de dados" );
			return ret;
		}

		return ret;
	}

	private String getLinkSolver()
	{
		String link = OtimizarServiceImpl.linkSolverDefault;
		return link;
	}
}
