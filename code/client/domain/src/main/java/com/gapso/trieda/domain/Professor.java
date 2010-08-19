package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.List;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.PersistenceContext;
import javax.persistence.Version;
import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.serializable.RooSerializable;
import org.springframework.transaction.annotation.Transactional;
import javax.validation.constraints.Size;
import javax.validation.constraints.Min;
import javax.validation.constraints.Max;
import javax.validation.constraints.Digits;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity
@RooSerializable
public class Professor implements java.io.Serializable {

    @Size(max = 14)
    private String cpf;

    @Size(max = 255)
    private String nome;

    @Min(0L)
    @Max(100L)
    private byte cargaHorariaMinima;

    @Min(0L)
    @Max(100L)
    private byte cargaHorariaMaxima;

    @Min(0L)
    @Max(1000L)
    @Digits(integer = 3, fraction = 2)
    private float credito;

    @Min(0L)
    @Max(1000L)
    @Digits(integer = 3, fraction = 2)
    private float creditoAnterior;

    @Min(0L)
    @Max(10L)
    @Digits(integer = 2, fraction = 2)
    private float nota;

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Cpf: ").append(getCpf()).append(", ");
        sb.append("Nome: ").append(getNome()).append(", ");
        sb.append("CargaHorariaMinima: ").append(getCargaHorariaMinima()).append(", ");
        sb.append("CargaHorariaMaxima: ").append(getCargaHorariaMaxima()).append(", ");
        sb.append("Credito: ").append(getCredito()).append(", ");
        sb.append("CreditoAnterior: ").append(getCreditoAnterior()).append(", ");
        sb.append("Nota: ").append(getNota());
        return sb.toString();
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
            Professor attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
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

	public static long countProfessors() {
        return ((Number) entityManager().createQuery("select count(o) from Professor o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<Professor> findAllProfessors() {
        return entityManager().createQuery("select o from Professor o").getResultList();
    }

	public static Professor findProfessor(Long id) {
        if (id == null) return null;
        return entityManager().find(Professor.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<Professor> findProfessorEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from Professor o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
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

	public byte getCargaHorariaMinima() {
        return this.cargaHorariaMinima;
    }

	public void setCargaHorariaMinima(byte cargaHorariaMinima) {
        this.cargaHorariaMinima = cargaHorariaMinima;
    }

	public byte getCargaHorariaMaxima() {
        return this.cargaHorariaMaxima;
    }

	public void setCargaHorariaMaxima(byte cargaHorariaMaxima) {
        this.cargaHorariaMaxima = cargaHorariaMaxima;
    }

	public float getCredito() {
        return this.credito;
    }

	public void setCredito(float credito) {
        this.credito = credito;
    }

	public float getCreditoAnterior() {
        return this.creditoAnterior;
    }

	public void setCreditoAnterior(float creditoAnterior) {
        this.creditoAnterior = creditoAnterior;
    }

	public float getNota() {
        return this.nota;
    }

	public void setNota(float nota) {
        this.nota = nota;
    }

	private static final long serialVersionUID = -5052142933033873303L;
}
