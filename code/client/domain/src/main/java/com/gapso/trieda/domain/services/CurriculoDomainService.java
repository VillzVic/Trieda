package com.gapso.trieda.domain.services;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.misc.TriedaDomainUtil;

@Configurable
public class CurriculoDomainService {
	@PersistenceContext
	transient EntityManager entityManager;
	public static final EntityManager entityManager() {
		EntityManager em = new CurriculoDomainService().entityManager;
		if ( em == null ) {
			throw new IllegalStateException(" Entity manager has not been injected (is the Spring " + " Aspects JAR configured as an AJC/AJDT aspects library?)" );
		}
		return em;
	}
	
	private DisponibilidadeDomainService disponibilidadeDomainService = new DisponibilidadeDomainService();
	
	@Transactional
	public void updateDataBaseFromExcel(Set<Curriculo> curriculosParaAtualizar, Set<Curriculo> curriculosParaInserir, Set<CurriculoDisciplina> curriculosDiscParaInserir, boolean updateDisciplinaHorario, Cenario cenario) {
		TriedaDomainUtil.transactionRequired(this.getClass().getName() + ".updateDataBaseFromExcel(..)");
		
		// atualiza currículos existentes
		int count = 0, total=curriculosParaAtualizar.size(); System.out.print("curriculo.merge(); total= "+total);
		for (Curriculo curriculo : curriculosParaAtualizar) {
			curriculo.merge();
			count++;total--;if (count == 100) {System.out.println("   Faltam "+total); count = 0;}
		}
		
		// insere novos currículos
		count = 0; total=curriculosParaInserir.size(); System.out.print("curriculo.persist(); total= "+total);
		for (Curriculo curriculo : curriculosParaInserir) {
			curriculo.persist();
			count++;total--;if (count == 100) {System.out.println("   Faltam "+total); count = 0;}
		}
		
		// insere novas disciplinas em currículos
		count = 0; total=curriculosDiscParaInserir.size(); System.out.print("curriculoDisciplina.persist(); total= "+total);
		Map<Disciplina, Set<SemanaLetiva>> disciplinaMapSemanasLetivas = new HashMap<Disciplina, Set<SemanaLetiva>>();
		for (CurriculoDisciplina curriculoDisciplina : curriculosDiscParaInserir) {
			curriculoDisciplina.persist();
			if (disciplinaMapSemanasLetivas.get(curriculoDisciplina.getDisciplina()) == null) {
				Set<SemanaLetiva> semanasLetivas = new HashSet<SemanaLetiva>();
				semanasLetivas.add(curriculoDisciplina.getCurriculo().getSemanaLetiva());
				disciplinaMapSemanasLetivas.put(curriculoDisciplina.getDisciplina(), semanasLetivas);
			}
			else {
				disciplinaMapSemanasLetivas.get(curriculoDisciplina.getDisciplina()).add(curriculoDisciplina.getCurriculo().getSemanaLetiva());
			}
			count++;total--;if (count == 100) {System.out.println("   Faltam "+total); count = 0;}
		}
		
		if (updateDisciplinaHorario) {
			disponibilidadeDomainService.atualizaDisponibilidadesDisciplinas(disciplinaMapSemanasLetivas,cenario);
		}
	}
}
