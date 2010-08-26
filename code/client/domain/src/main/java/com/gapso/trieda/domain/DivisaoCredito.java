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
import javax.persistence.ManyToMany;
import javax.persistence.PersistenceContext;
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

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "DCR_ID")
@Table(name = "DIVISOES_CREDITO")
public class DivisaoCredito implements java.io.Serializable {

    @NotNull
    @Column(name = "DRC_CREDITOS")
    @Min(1L)
    @Max(99L)
    private Integer creditos;

    @NotNull
    @Column(name = "DRC_DIA1")
    @Min(1L)
    @Max(99L)
    private Integer dia1;

    @NotNull
    @Column(name = "DRC_DIA2")
    @Min(1L)
    @Max(99L)
    private Integer dia2;

    @NotNull
    @Column(name = "DRC_DIA3")
    @Min(1L)
    @Max(99L)
    private Integer dia3;

    @NotNull
    @Column(name = "DRC_DIA4")
    @Min(1L)
    @Max(99L)
    private Integer dia4;

    @NotNull
    @Column(name = "DRC_DIA5")
    @Min(1L)
    @Max(99L)
    private Integer dia5;

    @NotNull
    @Column(name = "DRC_DIA6")
    @Min(1L)
    @Max(99L)
    private Integer dia6;

    @NotNull
    @Column(name = "DRC_DIA7")
    @Min(1L)
    @Max(99L)
    private Integer dia7;

    @ManyToMany(cascade = CascadeType.ALL)
    private Set<com.gapso.trieda.domain.Cenario> cenario = new java.util.HashSet<com.gapso.trieda.domain.Cenario>();

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "DCR_ID")
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
            DivisaoCredito attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public DivisaoCredito merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        DivisaoCredito merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new DivisaoCredito().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countDivisaoCreditoes() {
        return ((Number) entityManager().createQuery("select count(o) from DivisaoCredito o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<DivisaoCredito> findAllDivisaoCreditoes() {
        return entityManager().createQuery("select o from DivisaoCredito o").getResultList();
    }

	public static DivisaoCredito findDivisaoCredito(Long id) {
        if (id == null) return null;
        return entityManager().find(DivisaoCredito.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<DivisaoCredito> findDivisaoCreditoEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from DivisaoCredito o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Creditos: ").append(getCreditos()).append(", ");
        sb.append("Dia1: ").append(getDia1()).append(", ");
        sb.append("Dia2: ").append(getDia2()).append(", ");
        sb.append("Dia3: ").append(getDia3()).append(", ");
        sb.append("Dia4: ").append(getDia4()).append(", ");
        sb.append("Dia5: ").append(getDia5()).append(", ");
        sb.append("Dia6: ").append(getDia6()).append(", ");
        sb.append("Dia7: ").append(getDia7()).append(", ");
        sb.append("Cenario: ").append(getCenario() == null ? "null" : getCenario().size());
        return sb.toString();
    }

	public Integer getCreditos() {
        return this.creditos;
    }

	public void setCreditos(Integer creditos) {
        this.creditos = creditos;
    }

	public Integer getDia1() {
        return this.dia1;
    }

	public void setDia1(Integer dia1) {
        this.dia1 = dia1;
    }

	public Integer getDia2() {
        return this.dia2;
    }

	public void setDia2(Integer dia2) {
        this.dia2 = dia2;
    }

	public Integer getDia3() {
        return this.dia3;
    }

	public void setDia3(Integer dia3) {
        this.dia3 = dia3;
    }

	public Integer getDia4() {
        return this.dia4;
    }

	public void setDia4(Integer dia4) {
        this.dia4 = dia4;
    }

	public Integer getDia5() {
        return this.dia5;
    }

	public void setDia5(Integer dia5) {
        this.dia5 = dia5;
    }

	public Integer getDia6() {
        return this.dia6;
    }

	public void setDia6(Integer dia6) {
        this.dia6 = dia6;
    }

	public Integer getDia7() {
        return this.dia7;
    }

	public void setDia7(Integer dia7) {
        this.dia7 = dia7;
    }

	public Set<Cenario> getCenario() {
        return this.cenario;
    }

	public void setCenario(Set<Cenario> cenario) {
        this.cenario = cenario;
    }

	private static final long serialVersionUID = -444577823741587690L;
}
