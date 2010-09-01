package com.gapso.trieda.domain;

import java.util.List;
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
import javax.persistence.Table;
import javax.persistence.Version;
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
@RooEntity(identifierColumn = "CAM_ID")
@Table(name = "CAMPI")
public class Campus implements java.io.Serializable {

//  TODO  @NotNull
    @ManyToOne(targetEntity = Cenario.class)
    @JoinColumn(name = "CEN_ID")
    private Cenario cenario;

    @NotNull
    @Column(name = "CAL_CODIGO")
    @Size(min = 3, max = 20)
    private String codigo;

    @NotNull
    @Column(name = "CAL_NOME")
    @Size(min = 5, max = 20)
    private String nome;

    @NotNull
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "origem")
    private Set<com.gapso.trieda.domain.DeslocamentoCampus> deslocamentos = new java.util.HashSet<com.gapso.trieda.domain.DeslocamentoCampus>();

    @ManyToMany(cascade = CascadeType.ALL, mappedBy = "campi")
    private Set<com.gapso.trieda.domain.Professor> professores = new java.util.HashSet<com.gapso.trieda.domain.Professor>();

    @ManyToMany(cascade = CascadeType.ALL, mappedBy = "campi")
    private Set<com.gapso.trieda.domain.HorarioDisponivelCenario> horarios = new java.util.HashSet<com.gapso.trieda.domain.HorarioDisponivelCenario>();

    @NotNull
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "campus")
    private Set<com.gapso.trieda.domain.CampusCurriculo> campusCurriculo = new java.util.HashSet<com.gapso.trieda.domain.CampusCurriculo>();

	public Cenario getCenario() {
        return this.cenario;
    }

	public void setCenario(Cenario cenario) {
        this.cenario = cenario;
    }

	public String getCodigo() {
        return this.codigo;
    }

	public void setCodigo(String codigo) {
        this.codigo = codigo;
    }

	public String getNome() {
        return this.nome;
    }

	public void setNome(String nome) {
        this.nome = nome;
    }

	public Set<DeslocamentoCampus> getDeslocamentos() {
        return this.deslocamentos;
    }

	public void setDeslocamentos(Set<DeslocamentoCampus> deslocamentos) {
        this.deslocamentos = deslocamentos;
    }

	public Set<Professor> getProfessores() {
        return this.professores;
    }

	public void setProfessores(Set<Professor> professores) {
        this.professores = professores;
    }

	public Set<HorarioDisponivelCenario> getHorarios() {
        return this.horarios;
    }

	public void setHorarios(Set<HorarioDisponivelCenario> horarios) {
        this.horarios = horarios;
    }

	public Set<CampusCurriculo> getCampusCurriculo() {
        return this.campusCurriculo;
    }

	public void setCampusCurriculo(Set<CampusCurriculo> campusCurriculo) {
        this.campusCurriculo = campusCurriculo;
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "CAM_ID")
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
            Campus attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Campus merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Campus merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Campus().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static int count() {
        return ((Number) entityManager().createQuery("select count(o) from Campus o").getSingleResult()).intValue();
    }

	@SuppressWarnings("unchecked")
    public static List<Campus> findAll() {
        return entityManager().createQuery("select o from Campus o").getResultList();
    }

	public static Campus find(Long id) {
        if (id == null) return null;
        return entityManager().find(Campus.class, id);
    }

	public static List<Campus> find(int firstResult, int maxResults) {
		return find(firstResult, maxResults, null);
	}
	@SuppressWarnings("unchecked")
    public static List<Campus> find(int firstResult, int maxResults, String orderBy) {
		orderBy = (orderBy != null)? "ORDER BY o."+orderBy : "";
        return entityManager().createQuery("select o from Campus o "+orderBy).setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	private static final long serialVersionUID = 6690100103369325015L;

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Cenario: ").append(getCenario()).append(", ");
        sb.append("Codigo: ").append(getCodigo()).append(", ");
        sb.append("Nome: ").append(getNome()).append(", ");
        sb.append("Deslocamentos: ").append(getDeslocamentos() == null ? "null" : getDeslocamentos().size()).append(", ");
        sb.append("Professores: ").append(getProfessores() == null ? "null" : getProfessores().size()).append(", ");
        sb.append("Horarios: ").append(getHorarios() == null ? "null" : getHorarios().size()).append(", ");
        sb.append("CampusCurriculo: ").append(getCampusCurriculo() == null ? "null" : getCampusCurriculo().size());
        return sb.toString();
    }
}
