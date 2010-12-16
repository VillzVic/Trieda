package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.List;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.Enumerated;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.Version;
import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.misc.Semanas;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "ATT_ID")
@Table(name = "ATENDIMENTO_TATICO")
public class AtendimentoTatico implements Serializable {

	private static final long serialVersionUID = 6191028820294903254L;

	@NotNull
    @ManyToOne(targetEntity = Cenario.class)
    @JoinColumn(name = "CEN_ID")
    private Cenario cenario;

    @NotNull
    private String turma;
    
    @NotNull
    @ManyToOne(targetEntity = Sala.class)
    @JoinColumn(name = "SAL_ID")
    private Sala sala;
    
    @NotNull
    @Enumerated
    private Semanas semana;
    
    @NotNull
    @ManyToOne(targetEntity = Oferta.class)
    @JoinColumn(name = "OFE_ID")
    private Oferta oferta;
    
    @NotNull
    @ManyToOne(targetEntity = Disciplina.class)
    @JoinColumn(name = "DIS_ID")
    private Disciplina disciplina;
    
    @NotNull
    @Column(name = "ATT_QUANTIDADE")
    @Min(0L)
    @Max(999L)
    private Integer quantidadeAlunos;

    @NotNull
    @Column(name = "ATT_CRED_TEORICO")
    @Min(0L)
    @Max(99L)
    private Integer creditosTeorico;
    
    @NotNull
    @Column(name = "ATT_CRED_PRATICO")
    @Min(0L)
    @Max(99L)
    private Integer creditosPratico;
    
	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Cenario: ").append(getCenario()).append(", ");
        sb.append("Turma: ").append(getTurma()).append(", ");
        sb.append("Sala: ").append(getSala()).append(", ");
        sb.append("Semana: ").append(getSemana()).append(", ");
        sb.append("Oferta: ").append(getOferta()).append(", ");
        sb.append("Disciplina: ").append(getDisciplina()).append(", ");
        sb.append("QuantidadeAlunos: ").append(getQuantidadeAlunos()).append(", ");
        sb.append("CreditosTeorico: ").append(getCreditosTeorico()).append(", ");
        sb.append("CreditosPratico: ").append(getCreditosPratico()).append(", ");
        return sb.toString();
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "ATT_ID")
    private Long id;

	@Version
    @Column(name = "version")
    private Integer version;

	public Long getId() {
        return this.id;
    }

	public void setId(Long id) {
        this.id = id;
    }

	public Integer getVersion() {
        return this.version;
    }

	public void setVersion(Integer version) {
        this.version = version;
    }
	
	@Transactional
	public void detach() {
		if (this.entityManager == null) this.entityManager = entityManager();
		this.entityManager.detach(this);
	}
	
	@Transactional
    public void persist() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.persist(this);
    }

	@Transactional
    public void remove() {
        if (this.entityManager == null) this.entityManager = entityManager();
        if (this.entityManager.contains(this)) {
            this.entityManager.remove(this);
        } else {
            AtendimentoTatico attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public AtendimentoTatico merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        AtendimentoTatico merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new AtendimentoTatico().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	@SuppressWarnings("unchecked")
    public static List<AtendimentoTatico> findBySalaAndTurno(Sala sala, Turno turno) {
        Query q = entityManager().createQuery("SELECT o FROM AtendimentoTatico o WHERE o.sala = :sala AND o.oferta.turno = :turno");
        q.setParameter("sala", sala);
        q.setParameter("turno", turno);
        return q.getResultList();
    }
	
	@SuppressWarnings("unchecked")
	public static List<AtendimentoTatico> findByCurriculoAndPeriodoAndTurno(Curriculo curriculo, Integer periodo, Turno turno) {
		Query q = entityManager().createQuery("SELECT o FROM AtendimentoTatico o WHERE o.oferta.curriculo = :curriculo AND o.oferta.turno = :turno AND o.disciplina IN (SELECT d.disciplina FROM CurriculoDisciplina d WHERE d.curriculo = :curriculo AND d.periodo = :periodo)");
		q.setParameter("curriculo", curriculo);
		q.setParameter("periodo", periodo);
		q.setParameter("turno", turno);
		return q.getResultList();
	}
	
	@SuppressWarnings("unchecked")
	public static List<AtendimentoTatico> findAll() {
		return entityManager().createQuery("SELECT o FROM AtendimentoTatico o").getResultList();
	}

	public static AtendimentoTatico find(Long id) {
        if (id == null) return null;
        return entityManager().find(AtendimentoTatico.class, id);
    }

	
	public Cenario getCenario() {
		return cenario;
	}

	public void setCenario(Cenario cenario) {
		this.cenario = cenario;
	}

	public String getTurma() {
		return turma;
	}

	public void setTurma(String turma) {
		this.turma = turma;
	}
	
	public Sala getSala() {
		return sala;
	}

	public void setSala(Sala sala) {
		this.sala = sala;
	}

	public Semanas getSemana() {
		return semana;
	}

	public void setSemana(Semanas semana) {
		this.semana = semana;
	}

	public Oferta getOferta() {
		return oferta;
	}

	public void setOferta(Oferta oferta) {
		this.oferta = oferta;
	}

	public Disciplina getDisciplina() {
		return disciplina;
	}

	public void setDisciplina(Disciplina disciplina) {
		this.disciplina = disciplina;
	}

	public Integer getQuantidadeAlunos() {
		return quantidadeAlunos;
	}

	public void setQuantidadeAlunos(Integer quantidadeAlunos) {
		this.quantidadeAlunos = quantidadeAlunos;
	}

	public Integer getCreditosTeorico() {
		return creditosTeorico;
	}

	public void setCreditosTeorico(Integer creditosTeorico) {
		this.creditosTeorico = creditosTeorico;
	}

	public Integer getCreditosPratico() {
		return creditosPratico;
	}

	public void setCreditosPratico(Integer creditosPratico) {
		this.creditosPratico = creditosPratico;
	}
	
}
