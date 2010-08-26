package com.gapso.trieda.domain;

import java.util.List;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
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
@RooEntity(identifierColumn = "CAL_ID")
@Table(name = "CALENDARIOS")
public class Calendario implements java.io.Serializable {

    @NotNull
    @ManyToOne(targetEntity = Cenario.class)
    @JoinColumn(name = "CEN_ID")
    private Cenario cenario;

    @NotNull
    @Column(name = "CAL_CODIGO")
    @Size(min = 3, max = 20)
    private String codigo;

    @Column(name = "CAL_DESCRICAO")
    @Size(max = 255)
    private String descricao;

	private static final long serialVersionUID = 8987668892097072202L;

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

	public String getDescricao() {
        return this.descricao;
    }

	public void setDescricao(String descricao) {
        this.descricao = descricao;
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "CAL_ID")
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
            Calendario attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Calendario merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Calendario merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Calendario().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countCalendarios() {
        return ((Number) entityManager().createQuery("select count(o) from Calendario o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<Calendario> findAllCalendarios() {
        return entityManager().createQuery("select o from Calendario o").getResultList();
    }

	public static Calendario findCalendario(Long id) {
        if (id == null) return null;
        return entityManager().find(Calendario.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<Calendario> findCalendarioEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from Calendario o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Cenario: ").append(getCenario()).append(", ");
        sb.append("Codigo: ").append(getCodigo()).append(", ");
        sb.append("Descricao: ").append(getDescricao());
        return sb.toString();
    }
}
