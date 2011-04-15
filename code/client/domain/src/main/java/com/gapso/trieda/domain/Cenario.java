package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.Date;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.OneToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import javax.persistence.Version;
import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.format.annotation.DateTimeFormat;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "CEN_ID")
@Table(name = "CENARIOS")
public class Cenario implements Serializable {

    @OneToOne(mappedBy="cenario", fetch=FetchType.LAZY)
    private Parametro parametro;
	
//    TODO @NotNull
    @ManyToOne(targetEntity = Usuario.class)
    @JoinColumn(name = "USU_CRIACAO_ID")
    private Usuario criadoPor;

//   TODO @NotNull
    @ManyToOne(targetEntity = Usuario.class)
    @JoinColumn(name = "USU_ATUALIZACAO_ID")
    private Usuario atualizadoPor;
    
    @NotNull
    @Column(name = "CEN_MASTERDATA")
    private Boolean masterData;

    @NotNull
    @Column(name = "CEN_NOME")
    @Size(min = 1, max = 50)
    private String nome;

    @Column(name = "CEN_ANO")
    @Min(1L)
    @Max(9999L)
    private Integer ano;

    @Column(name = "CEN_SEMESTRE")
    @Min(1L)
    @Max(12L)
    private Integer semestre;

//    TODO @NotNull
    @Column(name = "CEN_DT_CRIACAO")
    @Temporal(TemporalType.TIMESTAMP)
    @DateTimeFormat(style = "S-")
    private Date dataCriacao;

//    TODO @NotNull
    @Column(name = "CEN_DT_ATUALIZACAO")
    @Temporal(TemporalType.TIMESTAMP)
    @DateTimeFormat(style = "S-")
    private Date dataAtualizacao;

    @Column(name = "CEN_COMENTARIO")
    private String comentario;

    @Column(name = "CEN_OFICIAL")
    private Boolean oficial;

    @OneToOne
    @JoinColumn(name="SLE_ID")
    private SemanaLetiva semanaLetiva;
    
    @ManyToMany(cascade = CascadeType.ALL, mappedBy = "cenario")
    private Set<DivisaoCredito> divisoesCredito = new HashSet<DivisaoCredito>();

    @OneToMany(mappedBy="cenario")
    private Set<Turno> turnos = new HashSet<Turno>();
    
    @OneToMany(mappedBy="cenario")
    private Set<Curso> cursos = new HashSet<Curso>();
    
    @OneToMany(mappedBy="cenario")
    private Set<Campus> campi = new HashSet<Campus>();
    
    @OneToMany(mappedBy="cenario")
    private Set<Disciplina> disciplinas = new HashSet<Disciplina>();
    
    @OneToMany(mappedBy="cenario")
    private Set<Professor> professores = new HashSet<Professor>();
    
    @OneToMany(mappedBy="cenario")
    private Set<Curriculo> curriculos = new HashSet<Curriculo>();

    @OneToMany(cascade = CascadeType.ALL, mappedBy="cenario")
    private Set<AtendimentoOperacional> atendimentosOperacionais =  new HashSet<AtendimentoOperacional>();
    
    @OneToMany(cascade = CascadeType.ALL, mappedBy="cenario")
    private Set<AtendimentoTatico> atendimentosTaticos =  new HashSet<AtendimentoTatico>();
    
	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("CriadoPor: ").append(getCriadoPor()).append(", ");
        sb.append("AtualizadoPor: ").append(getAtualizadoPor()).append(", ");
        sb.append("MasterData: ").append(getMasterData()).append(", ");
        sb.append("Nome: ").append(getNome()).append(", ");
        sb.append("Ano: ").append(getAno()).append(", ");
        sb.append("Semestre: ").append(getSemestre()).append(", ");
        sb.append("DataCriacao: ").append(getDataCriacao()).append(", ");
        sb.append("DataAtualizacao: ").append(getDataAtualizacao()).append(", ");
        sb.append("Comentario: ").append(getComentario()).append(", ");
        sb.append("Oficial: ").append(getOficial()).append(", ");
        sb.append("SemanaLetiva: ").append(getSemanaLetiva()).append(", ");
        sb.append("DivisoesCredito: ").append(getDivisoesCredito() == null ? "null" : getDivisoesCredito().size());
        sb.append("Turnos: ").append(getTurnos() == null ? "null" : getTurnos().size());
        sb.append("Cursos: ").append(getCursos() == null ? "null" : getCursos().size());
        sb.append("Campi: ").append(getCampi() == null ? "null" : getCampi().size());
        sb.append("Disciplinas: ").append(getDisciplinas() == null ? "null" : getDisciplinas().size());
        sb.append("Professores: ").append(getProfessores() == null ? "null" : getProfessores().size());
        sb.append("Curriculos: ").append(getCurriculos() == null ? "null" : getCurriculos().size());
        sb.append("Atendimentos Operacionais: ").append(getAtendimentosOperacionais() == null ? "null" : getAtendimentosOperacionais().size());
        sb.append("Atendimentos Taticos: ").append(getAtendimentosTaticos() == null ? "null" : getAtendimentosTaticos().size());
        sb.append("Parametro: ").append(getParametro()).append(", ");
        return sb.toString();
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "CEN_ID")
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
        	removeDivisoesCredito();
            this.entityManager.remove(this);
        } else {
            Cenario attached = this.entityManager.find(this.getClass(), this.id);
            attached.removeDivisoesCredito();
            this.entityManager.remove(attached);
        }
    }

    @Transactional
    public void removeDivisoesCredito() {
    	Set<DivisaoCredito> divisoesCredito = this.getDivisoesCredito();
    	for(DivisaoCredito divisaoCredito : divisoesCredito) {
    		divisaoCredito.getCenario().remove(this);
    		divisaoCredito.merge();
    	}
    }
	
	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Cenario merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Cenario merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Cenario().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static int count() {
        return ((Number) entityManager().createQuery("SELECT count(o) FROM Cenario o").getSingleResult()).intValue();
    }

	@SuppressWarnings("unchecked")
    public static List<Cenario> findAll() {
        Query q = entityManager().createQuery("SELECT o FROM Cenario o WHERE o.masterData = :masterData");
        q.setParameter("masterData", false);
        return q.getResultList();
    }

	public static Cenario find(Long id) {
        if (id == null) return null;
        return entityManager().find(Cenario.class, id);
    }
	
	public static Cenario findMasterData() {
		Query q = entityManager().createQuery("SELECT o FROM Cenario o WHERE o.masterData = :masterData");
		q.setParameter("masterData", true);
		Cenario cenario = null;
		try {
		cenario = (Cenario)q.getSingleResult();
		} catch (EmptyResultDataAccessException e) { }
		return cenario; 
	}

	public static List<Cenario> find(int firstResult, int maxResults) {
		return find(firstResult, maxResults, null);
	}
	@SuppressWarnings("unchecked")
    public static List<Cenario> find(int firstResult, int maxResults, String orderBy) {
		orderBy = (orderBy != null) ? "ORDER BY o." + orderBy : "";
        Query q = entityManager().createQuery("SELECT o FROM Cenario o WHERE o.masterData = :masterData " + orderBy);
        q.setParameter("masterData", false);
        return q.setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

    @SuppressWarnings("unchecked")
	public static List<Cenario> findByAnoAndSemestre(Integer ano, Integer semestre, int firstResult, int maxResults, String orderBy) {
        orderBy = (orderBy != null) ? "ORDER BY o." + orderBy : "";
        
        String queryAno = "";
        String querySemestre = "";
        if(ano != null) queryAno = "o.ano = :ano AND ";
        if(ano != null) querySemestre = "o.semestre = :semestre AND ";
        
        Query q = entityManager().createQuery("SELECT o FROM Cenario o WHERE "+queryAno+querySemestre+" o.masterData = :masterData ");

        q.setParameter("masterData", false);
        if(ano != null) q.setParameter("ano", semestre);
        if(semestre != null) q.setParameter("ano", semestre);
        
        return q.setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
	
	public Usuario getCriadoPor() {
        return this.criadoPor;
    }

	public void setCriadoPor(Usuario criadoPor) {
        this.criadoPor = criadoPor;
    }

	public Usuario getAtualizadoPor() {
        return this.atualizadoPor;
    }

	public void setAtualizadoPor(Usuario atualizadoPor) {
        this.atualizadoPor = atualizadoPor;
    }

	public Boolean getMasterData() {
		return masterData;
	}

	public void setMasterData(Boolean masterData) {
		this.masterData = masterData;
	}

	public String getNome() {
        return this.nome;
    }

	public void setNome(String nome) {
        this.nome = nome;
    }

	public Integer getAno() {
        return this.ano;
    }

	public void setAno(Integer ano) {
        this.ano = ano;
    }

	public Integer getSemestre() {
        return this.semestre;
    }

	public void setSemestre(Integer semestre) {
        this.semestre = semestre;
    }

	public Date getDataCriacao() {
        return this.dataCriacao;
    }

	public void setDataCriacao(Date dataCriacao) {
        this.dataCriacao = dataCriacao;
    }

	public Date getDataAtualizacao() {
        return this.dataAtualizacao;
    }

	public void setDataAtualizacao(Date dataAtualizacao) {
        this.dataAtualizacao = dataAtualizacao;
    }

	public String getComentario() {
        return this.comentario;
    }

	public void setComentario(String comentario) {
        this.comentario = comentario;
    }

	public Boolean getOficial() {
        return this.oficial;
    }

	public void setOficial(Boolean oficial) {
        this.oficial = oficial;
    }
	
	public SemanaLetiva getSemanaLetiva() {
		return this.semanaLetiva;
	}
	
	public void setSemanaLetiva(SemanaLetiva semanaLetiva) {
		this.semanaLetiva = semanaLetiva;
	}

	public Set<DivisaoCredito> getDivisoesCredito() {
        return this.divisoesCredito;
    }

	public void setDivisoesCredito(Set<DivisaoCredito> divisoesCredito) {
        this.divisoesCredito = divisoesCredito;
    }
	
	public Set<Turno> getTurnos() {
		return this.turnos;
	}
	
	public void setTurnos(Set<Turno> turnos) {
		this.turnos = turnos;
	}
	
	public Set<Curso> getCursos() {
		return this.cursos;
	}
	
	public void setCursos(Set<Curso> cursos) {
		this.cursos = cursos;
	}
	
	public Set<Campus> getCampi() {
		return this.campi;
	}
	
	public void setCampi(Set<Campus> campi) {
		this.campi = campi;
	}
	
	public Set<Disciplina> getDisciplinas() {
		return this.disciplinas;
	}
	
	public void setDisciplinas(Set<Disciplina> disciplinas) {
		this.disciplinas = disciplinas;
	}
	
	public Set<Professor> getProfessores() {
		return this.professores;
	}
	
	public void setProfessores(Set<Professor> professores) {
		this.professores = professores;
	}
	
	public Set<Curriculo> getCurriculos() {
		return this.curriculos;
	}
	
	public void setCurriculos(Set<Curriculo> curriculos) {
		this.curriculos = curriculos;
	}
	
	public Set<AtendimentoOperacional> getAtendimentosOperacionais() {
		return this.atendimentosOperacionais;
	}
	
	public void setAtendimentosOperacionais(Set<AtendimentoOperacional> atendimentosOperacionais) {
		this.atendimentosOperacionais = atendimentosOperacionais;
	}
	
	public Set<AtendimentoTatico> getAtendimentosTaticos() {
		return this.atendimentosTaticos;
	}
	
	public void setAtendimentosTaticos(Set<AtendimentoTatico> atendimentosTaticos) {
		this.atendimentosTaticos = atendimentosTaticos;
	}
	
	public Parametro getParametro() {
		return parametro;
	}

	public void setParametro(Parametro parametro) {
		this.parametro = parametro;
	}

	private static final long serialVersionUID = -8610380359760552949L;
}
