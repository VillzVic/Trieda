package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.Version;
import javax.validation.constraints.Digits;
import javax.validation.constraints.Max;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "PRF_ID")
@Table(name = "PROFESSORES")
public class Professor implements Serializable {

    @NotNull
    @ManyToOne(targetEntity = Cenario.class)
    @JoinColumn(name = "CEN_ID")
    private Cenario cenario;

    @NotNull
    @ManyToOne(targetEntity = TipoContrato.class)
    @JoinColumn(name = "TCO_ID")
    private TipoContrato tipoContrato;

    @NotNull
    @ManyToOne(targetEntity = Titulacao.class)
    @JoinColumn(name = "TIT_ID")
    private Titulacao titulacao;

    @NotNull
    @ManyToOne(targetEntity = AreaTitulacao.class)
    @JoinColumn(name = "ATI_ID")
    private AreaTitulacao areaTitulacao;

    @NotNull
    @Column(name = "PRF_CPF")
    @Size(min = 14, max = 14)
    private String cpf;

    @NotNull
    @Column(name = "PRF_NOME")
    @Size(min = 3, max = 20)
    private String nome;

    @Column(name = "PRF_CH_MIN")
    @Max(999L)
    private Integer cargaHorariaMin;

    @Column(name = "PRF_CH_MAX")
    @Max(999L)
    private Integer cargaHorariaMax;

    @Column(name = "PRF_CRED_ANTERIOR")
    @Max(999L)
    private Integer creditoAnterior;

    @Column(name = "PRF_VALOR_CREDITO")
    @Digits(integer = 6, fraction = 2)
    private Double valorCredito;

    @ManyToMany
    private Set<Campus> campi = new HashSet<Campus>();

    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE}, mappedBy="professores")
    private Set<HorarioDisponivelCenario> horarios = new HashSet<HorarioDisponivelCenario>();

    @NotNull
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "professor")
    private Set<ProfessorDisciplina> disciplinas = new HashSet<ProfessorDisciplina>();
    
    @OneToMany(cascade = CascadeType.ALL, mappedBy="professor")
    private Set<AtendimentoOperacional> atendimentosOperacionais =  new HashSet<AtendimentoOperacional>();

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Cenario: ").append(getCenario()).append(", ");
        sb.append("TipoContrato: ").append(getTipoContrato()).append(", ");
        sb.append("Titulacao: ").append(getTitulacao()).append(", ");
        sb.append("AreaTitulacao: ").append(getAreaTitulacao()).append(", ");
        sb.append("Cpf: ").append(getCpf()).append(", ");
        sb.append("Nome: ").append(getNome()).append(", ");
        sb.append("CargaHorariaMin: ").append(getCargaHorariaMin()).append(", ");
        sb.append("CargaHorariaMax: ").append(getCargaHorariaMax()).append(", ");
        sb.append("CreditoAnterior: ").append(getCreditoAnterior()).append(", ");
        sb.append("ValorCredito: ").append(getValorCredito()).append(", ");
        sb.append("Campi: ").append(getCampi() == null ? "null" : getCampi().size()).append(", ");
        sb.append("Horarios: ").append(getHorarios() == null ? "null" : getHorarios().size()).append(", ");
        sb.append("Disciplinas: ").append(getDisciplinas() == null ? "null" : getDisciplinas().size());
        sb.append("Atendimentos Operacionais: ").append(getAtendimentosOperacionais() == null ? "null" : getAtendimentosOperacionais().size());
        return sb.toString();
    }

	public Cenario getCenario() {
        return this.cenario;
    }

	public void setCenario(Cenario cenario) {
        this.cenario = cenario;
    }

	public TipoContrato getTipoContrato() {
        return this.tipoContrato;
    }

	public void setTipoContrato(TipoContrato tipoContrato) {
        this.tipoContrato = tipoContrato;
    }

	public Titulacao getTitulacao() {
        return this.titulacao;
    }

	public void setTitulacao(Titulacao titulacao) {
        this.titulacao = titulacao;
    }

	public AreaTitulacao getAreaTitulacao() {
        return this.areaTitulacao;
    }

	public void setAreaTitulacao(AreaTitulacao areaTitulacao) {
        this.areaTitulacao = areaTitulacao;
    }

	public String getCpf() {
        return this.cpf;
    }

	public void setCpf(String cpf) {
        this.cpf = cpf;
    }

	public String getNome() {
        return this.nome;
    }

	public void setNome(String nome) {
        this.nome = nome;
    }

	public Integer getCargaHorariaMin() {
        return this.cargaHorariaMin;
    }

	public void setCargaHorariaMin(Integer cargaHorariaMin) {
        this.cargaHorariaMin = cargaHorariaMin;
    }

	public Integer getCargaHorariaMax() {
        return this.cargaHorariaMax;
    }

	public void setCargaHorariaMax(Integer cargaHorariaMax) {
        this.cargaHorariaMax = cargaHorariaMax;
    }

	public Integer getCreditoAnterior() {
        return this.creditoAnterior;
    }

	public void setCreditoAnterior(Integer creditoAnterior) {
        this.creditoAnterior = creditoAnterior;
    }

	public Double getValorCredito() {
        return this.valorCredito;
    }

	public void setValorCredito(Double valorCredito) {
        this.valorCredito = valorCredito;
    }

	public Set<Campus> getCampi() {
        return this.campi;
    }

	public void setCampi(Set<Campus> campi) {
        this.campi = campi;
    }

	private Set<HorarioDisponivelCenario> getHorarios() {
        return this.horarios;
    }

	public void setHorarios(Set<HorarioDisponivelCenario> horarios) {
        this.horarios = horarios;
    }

	public Set<ProfessorDisciplina> getDisciplinas() {
        return this.disciplinas;
    }

	public void setDisciplinas(Set<ProfessorDisciplina> disciplinas) {
        this.disciplinas = disciplinas;
    }
	
	public Set<AtendimentoOperacional> getAtendimentosOperacionais() {
		return this.atendimentosOperacionais;
	}
	
	public void setAtendimentosOperacionais(Set<AtendimentoOperacional> atendimentosOperacionais) {
		this.atendimentosOperacionais = atendimentosOperacionais;
	}
	
	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "PRF_ID")
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
        	this.removeHorariosDisponivelCenario();
            this.entityManager.remove(this);
        } else {
            Professor attached = this.entityManager.find(this.getClass(), this.id);
            attached.removeHorariosDisponivelCenario();
            this.entityManager.remove(attached);
        }
    }

    @Transactional
    public void removeHorariosDisponivelCenario() {
    	Set<HorarioDisponivelCenario> horarios = this.getHorarios();
    	for(HorarioDisponivelCenario horario : horarios) {
    		horario.getProfessores().remove(this);
    		horario.merge();
    	}
    }

	@SuppressWarnings("unchecked")
	public static List<Professor> findByCenario(Cenario cenario) {
    	Query q = entityManager().createQuery("SELECT o FROM Professor o WHERE o.cenario = :cenario");
    	q.setParameter("cenario", cenario);
    	return q.getResultList();
    }
    
	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Professor merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Professor merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Professor().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	@SuppressWarnings("unchecked")
    public static List<Professor> findAll() {
        return entityManager().createQuery("SELECT o FROM Professor o").getResultList();
    }
	
	public static int count(String cpf, TipoContrato tipoContrato, Titulacao titulacao, AreaTitulacao areaTitulacao) {
		String where = "";
		
		if(cpf != null)           where += " o.cpf = :cpf AND ";
		if(tipoContrato != null)  where += " o.tipoContrato = :tipoContrato AND ";
		if(titulacao != null)     where += " o.titulacao = :titulacao AND ";
		if(areaTitulacao != null) where += " o.areaTitulacao = :areaTitulacao AND ";
		if(where.length() > 1) where = " WHERE " + where.substring(0, where.length()-4);
		
		Query q = entityManager().createQuery("SELECT COUNT(o) FROM Professor o " + where);
		
		if(cpf != null)           q.setParameter("cpf", cpf);
		if(tipoContrato != null)  q.setParameter("tipoContrato", tipoContrato);
		if(titulacao != null)     q.setParameter("titulacao", titulacao);
		if(areaTitulacao != null) q.setParameter("areaTitulacao", areaTitulacao);
		
		return ((Number)q.getSingleResult()).intValue();
	}
	
	@SuppressWarnings("unchecked")
	public static List<Professor> findBy(String cpf, TipoContrato tipoContrato, Titulacao titulacao, AreaTitulacao areaTitulacao, int firstResult, int maxResults, String orderBy) {
		String where = "";
		
		if(cpf != null)           where += " o.cpf = :cpf AND ";
		if(tipoContrato != null)  where += " o.tipoContrato = :tipoContrato AND ";
		if(titulacao != null)     where += " o.titulacao = :titulacao AND ";
		if(areaTitulacao != null) where += " o.areaTitulacao = :areaTitulacao AND ";
		if(where.length() > 1) where = " WHERE " + where.substring(0, where.length()-4);
		
		where += (orderBy != null) ? " ORDER BY o." + orderBy : "";
		
		Query q = entityManager().createQuery("SELECT o FROM Professor o " + where);
		
		if(cpf != null)           q.setParameter("cpf", cpf);
		if(tipoContrato != null)  q.setParameter("tipoContrato", tipoContrato);
		if(titulacao != null)     q.setParameter("titulacao", titulacao);
		if(areaTitulacao != null) q.setParameter("areaTitulacao", areaTitulacao);
		
		return q.setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
	}

	public static Professor find(Long id) {
        if (id == null) return null;
        return entityManager().find(Professor.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<Professor> find(int firstResult, int maxResults) {
        return entityManager().createQuery("SELECT o FROM Professor o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	@SuppressWarnings("unchecked")
	public List<HorarioDisponivelCenario> getHorarios(SemanaLetiva semanaLetiva) {
		Query q = entityManager().createQuery("SELECT o FROM HorarioDisponivelCenario o, IN (o.professores) c WHERE c = :professor AND o.horarioAula.semanaLetiva = :semanaLetiva");
		q.setParameter("professor", this);
		q.setParameter("semanaLetiva", semanaLetiva);
		return q.getResultList();
	}
	
	public static Map<String,Professor> buildProfessorCpfToProfessorMap(List<Professor> professores) {
		Map<String,Professor> professoresMap = new HashMap<String,Professor>();
		for (Professor professor : professores) {
			professoresMap.put(professor.getCpf(), professor);
		}
		return professoresMap;
	}
	
	private static final long serialVersionUID = 265242535107921721L;
}
