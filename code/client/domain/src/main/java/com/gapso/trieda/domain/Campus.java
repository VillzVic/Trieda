package com.gapso.trieda.domain;

import javax.persistence.Entity;
import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.serializable.RooSerializable;
import org.springframework.transaction.annotation.Transactional;
import javax.validation.constraints.Size;
import com.gapso.trieda.domain.Endereco;
import javax.validation.constraints.NotNull;
import javax.persistence.Column;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.ManyToOne;
import javax.persistence.JoinColumn;
import javax.persistence.PersistenceContext;
import javax.persistence.Version;
import java.io.Serializable;
import java.util.List;
import java.util.Set;
import javax.persistence.ManyToMany;
import javax.persistence.CascadeType;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity
@RooSerializable
public class Campus implements java.io.Serializable {

    @Size(max = 255)
    private String codigo;

    @Size(max = 255)
    private String descricao;

    @NotNull
    @ManyToOne(targetEntity = Endereco.class)
    @JoinColumn
    private Endereco endereco;

    @ManyToMany(cascade = CascadeType.ALL)
    private Set<com.gapso.trieda.domain.Curriculo> curriculos = new java.util.HashSet<com.gapso.trieda.domain.Curriculo>();

    @ManyToMany(cascade = CascadeType.ALL)
    private Set<com.gapso.trieda.domain.Professor> professores = new java.util.HashSet<com.gapso.trieda.domain.Professor>();

	private static final long serialVersionUID = -8102694108028945468L;

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Codigo: ").append(getCodigo()).append(", ");
        sb.append("Descricao: ").append(getDescricao()).append(", ");
        sb.append("Endereco: ").append(getEndereco()).append(", ");
        sb.append("Curriculos: ").append(getCurriculos() == null ? "null" : getCurriculos().size()).append(", ");
        sb.append("Professores: ").append(getProfessores() == null ? "null" : getProfessores().size());
        return sb.toString();
    }

	public String getCodigo() {
        return this.codigo;
    }

	public void setCodigo(String codigo) {
        this.codigo = codigo;
    }

	public String getDescricao() {
        return this.descricao;
    }

	public void setDescricao(String descricao) {
        this.descricao = descricao;
    }

	public Endereco getEndereco() {
        return this.endereco;
    }

	public void setEndereco(Endereco endereco) {
        this.endereco = endereco;
    }

	public Set<Curriculo> getCurriculos() {
        return this.curriculos;
    }

	public void setCurriculos(Set<Curriculo> curriculos) {
        this.curriculos = curriculos;
    }

	public Set<Professor> getProfessores() {
        return this.professores;
    }

	public void setProfessores(Set<Professor> professores) {
        this.professores = professores;
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "id")
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

	public static long countCampuses() {
        return ((Number) entityManager().createQuery("select count(o) from Campus o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<Campus> findAllCampuses() {
        return entityManager().createQuery("select o from Campus o").getResultList();
    }

	public static Campus findCampus(Long id) {
        if (id == null) return null;
        return entityManager().find(Campus.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<Campus> findCampusEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from Campus o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
}
