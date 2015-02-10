package com.gapso.trieda.domain.services;

import java.util.Map;
import java.util.Set;

import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.misc.TriedaDomainUtil;

@Configurable
public class ProfessorDomainService {
	@PersistenceContext
	transient EntityManager entityManager;
	public static final EntityManager entityManager() {
		EntityManager em = new ProfessorDomainService().entityManager;
		if ( em == null ) {
			throw new IllegalStateException(" Entity manager has not been injected (is the Spring " + " Aspects JAR configured as an AJC/AJDT aspects library?)" );
		}
		return em;
	}
	
	@Transactional
	public Map<String,ProfessorVirtual> criaProfessoresVirtuaisPadrao(Map<String, Set<Disciplina>> profVirtualIdToDisciplinasMap, Cenario cenario, InstituicaoEnsino instituicaoEnsino) {
		TriedaDomainUtil.transactionRequired(this.getClass().getName() + ".criaProfessoresVirtuaisPadrao()");
		return ProfessorVirtual.criaProfessoresVirtuaisPadrao(profVirtualIdToDisciplinasMap, cenario, instituicaoEnsino);
	}
}
