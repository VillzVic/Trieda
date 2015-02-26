package com.gapso.trieda.domain.services;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.misc.TriedaDomainUtil;

@Configurable
public class SolucaoDomainService {
	@PersistenceContext
	transient EntityManager entityManager;
	public static final EntityManager entityManager() {
		EntityManager em = new ProfessorDomainService().entityManager;
		if ( em == null ) {
			throw new IllegalStateException(" Entity manager has not been injected (is the Spring " + " Aspects JAR configured as an AJC/AJDT aspects library?)" );
		}
		return em;
	}
	
	private ProfessorDomainService professorDomainService = new ProfessorDomainService();
	
	@Transactional
	public void limpaSolucao(Cenario cenario) {
		TriedaDomainUtil.transactionRequired(this.getClass().getName() + ".limpaSolucao(..)");
		
		List<Query> nativeQueries = new ArrayList<Query>();
		nativeQueries.add(entityManager().createNativeQuery( "UPDATE alunos_demanda ad JOIN alunos a ON a.aln_id = ad.aln_id SET ald_atendido = FALSE WHERE a.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE adao FROM alunos_demanda_atendimentos_operacional adao JOIN atendimento_operacional ao ON adao.atendimentos_operacional = ao.atp_id WHERE ao.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE adat FROM alunos_demanda_atendimentos_tatico adat JOIN atendimento_tatico at ON adat.atendimentos_tatico = at.att_id WHERE at.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE muao FROM atendimento_operacional_motivos_uso muao JOIN atendimento_operacional ao ON muao.atp_id = ao.atp_id WHERE ao.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE mu FROM motivos_uso_prof_virtual mu LEFT JOIN atendimento_operacional_motivos_uso muao ON muao.mot_prv_id = mu.mot_prv_id LEFT JOIN atendimento_operacional ao ON ao.atp_id = muao.atp_id WHERE ao.cen_id = :cenario OR muao.atp_id IS NULL"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE aode FROM atendimento_operacional_dicas_eliminacao aode JOIN atendimento_operacional ao ON aode.atp_id = ao.atp_id WHERE ao.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE de FROM dicas_eliminacao_prof_virtual de JOIN professores p ON de.prf_id = p.prf_id WHERE p.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE pvd FROM professores_virtuais_disciplinas pvd JOIN professores_virtuais pv ON pv.prf_id = pvd.professores_virtuais WHERE pv.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE at FROM aulas_turmas at JOIN turmas t ON t.tur_id = at.tur_id WHERE t.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE tad FROM turmas_alunos_demanda tad JOIN turmas t ON t.tur_id = tad.tur_id WHERE t.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE t FROM turmas t WHERE t.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE a FROM aulas a WHERE a.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE at FROM atendimento_tatico at WHERE at.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE ao FROM atendimento_operacional ao WHERE ao.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE pv FROM professores_virtuais pv WHERE pv.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE afd FROM atendimentos_faixa_demanda afd JOIN campi c ON c.cam_id = afd.cam_id WHERE c.cen_id = :cenario"));
		
		for (Query q : nativeQueries) {
			q.setParameter( "cenario", cenario.getId() );
			q.executeUpdate();
		}
	}
	
	@Transactional
	public void escreveNovaSolucaoNoBD(Map<String, Set<Disciplina>> profVirtualIdToDisciplinasMap, Map<String, List<AtendimentoOperacional>> profVirtualKeyToAtendimentosOpMap, Collection<AtendimentoOperacional> atendimentosOperacional, Cenario cenario, InstituicaoEnsino instituicaoEnsino) {
		TriedaDomainUtil.transactionRequired(this.getClass().getName() + ".updateDataBase(..)");
		
		this.limpaSolucao(cenario);
		System.out.println("this.limpaSolucao(cenario) ... Finalizado");
		
		// cria os professores virtuais
		Map<String,ProfessorVirtual> profVirtualKeyToProfVirtuaiMap = this.professorDomainService.criaProfessoresVirtuaisPadrao(profVirtualIdToDisciplinasMap, cenario, instituicaoEnsino);
		System.out.println("this.professorDomainService.criaProfessoresVirtuaisPadrao(..) ... Finalizado");
		
		// atualiza os professores virtuais dos atendimentos
		for (Entry<String, List<AtendimentoOperacional>> entry : profVirtualKeyToAtendimentosOpMap.entrySet()) {
			String professorVirtualKey = entry.getKey();
			ProfessorVirtual pv = profVirtualKeyToProfVirtuaiMap.get(professorVirtualKey);
			for (AtendimentoOperacional at : entry.getValue()) {
				at.setProfessorVirtual(pv);
			}
		}
		
		Map<AlunoDemanda, Set<AtendimentoOperacional>> alunoDemandaToAtendimentosOpMap = new HashMap<AlunoDemanda, Set<AtendimentoOperacional>>();
		for (AtendimentoOperacional at : atendimentosOperacional) {
			at.setQuantidadeAlunos(at.getAlunosDemanda().size());
			at.persist();
			// colhe atendimentos por aluno demanda
			for (AlunoDemanda ald : at.getAlunosDemanda()) {
				Set<AtendimentoOperacional> atendimentos = alunoDemandaToAtendimentosOpMap.get(ald);
				if (atendimentos == null) {
					atendimentos = new HashSet<AtendimentoOperacional>();
					alunoDemandaToAtendimentosOpMap.put(ald, atendimentos);
				}
				atendimentos.add(at);
			}
		}
		System.out.println("escrita AtendimentoOperacional ... Finalizado");
		
		for (Entry<AlunoDemanda, Set<AtendimentoOperacional>> entry : alunoDemandaToAtendimentosOpMap.entrySet()) {
			AlunoDemanda ald = entry.getKey();
			Set<AtendimentoOperacional> atendimentosDoAlunoDemanda = entry.getValue();
			
			ald.getAtendimentosOperacional().addAll(atendimentosDoAlunoDemanda);
			
			int creditosAtendidos = 0;
			Disciplina disciplinaSubstituta = null;
			for (AtendimentoOperacional operacional : atendimentosDoAlunoDemanda) {
				disciplinaSubstituta = operacional.getDisciplinaSubstituta();
				creditosAtendidos += 1;
			}
			Disciplina disciplinaASerConsiderada = (disciplinaSubstituta != null) ? disciplinaSubstituta : ald.getDemanda().getDisciplina();
			int totalCreditosDemanda = disciplinaASerConsiderada.getCreditosTotal();
			ald.setAtendido(totalCreditosDemanda <= creditosAtendidos);
		}
		System.out.println("atualização AlunoDemanda ... Finalizado");
	}
}
