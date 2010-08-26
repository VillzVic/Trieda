package com.gapso.trieda.domain;

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
import javax.persistence.Table;
import javax.persistence.Version;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.misc.Estados;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "END_ID")
@Table(name = "ENDERECOS")
public class Endereco implements java.io.Serializable {

    @NotNull
    @ManyToOne(targetEntity = Campus.class)
    @JoinColumn(name = "CAM_ID")
    private Campus campus;

    @Enumerated
    private Estados estado;

    @Column(name = "END_CIDADE")
    @Size(max = 25)
    private String cidade;

    @Column(name = "END_MUNICIPIO")
    @Size(max = 25)
    private String municipio;

    @Column(name = "END_LOGRADOURO")
    @Size(max = 25)
    private String logradouro;

    @Column(name = "END_NUMERO")
    @Size(max = 25)
    private String numero;

	public Campus getCampus() {
        return this.campus;
    }

	public void setCampus(Campus campus) {
        this.campus = campus;
    }

	public Estados getEstado() {
        return this.estado;
    }

	public void setEstado(Estados estado) {
        this.estado = estado;
    }

	public String getCidade() {
        return this.cidade;
    }

	public void setCidade(String cidade) {
        this.cidade = cidade;
    }

	public String getMunicipio() {
        return this.municipio;
    }

	public void setMunicipio(String municipio) {
        this.municipio = municipio;
    }

	public String getLogradouro() {
        return this.logradouro;
    }

	public void setLogradouro(String logradouro) {
        this.logradouro = logradouro;
    }

	public String getNumero() {
        return this.numero;
    }

	public void setNumero(String numero) {
        this.numero = numero;
    }

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Campus: ").append(getCampus()).append(", ");
        sb.append("Estado: ").append(getEstado()).append(", ");
        sb.append("Cidade: ").append(getCidade()).append(", ");
        sb.append("Municipio: ").append(getMunicipio()).append(", ");
        sb.append("Logradouro: ").append(getLogradouro()).append(", ");
        sb.append("Numero: ").append(getNumero());
        return sb.toString();
    }

	private static final long serialVersionUID = -5643489759251582328L;

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "END_ID")
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
            Endereco attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Endereco merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Endereco merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Endereco().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countEnderecoes() {
        return ((Number) entityManager().createQuery("select count(o) from Endereco o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<Endereco> findAllEnderecoes() {
        return entityManager().createQuery("select o from Endereco o").getResultList();
    }

	public static Endereco findEndereco(Long id) {
        if (id == null) return null;
        return entityManager().find(Endereco.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<Endereco> findEnderecoEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from Endereco o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
}
