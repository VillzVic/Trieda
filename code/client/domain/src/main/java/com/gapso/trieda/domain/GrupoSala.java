package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.HashSet;
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
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
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
@RooEntity(identifierColumn = "GRS_ID")
@Table(name = "GRUPOS_SALA")
public class GrupoSala implements Serializable {

	@NotNull
    @ManyToOne(targetEntity = Unidade.class)
    @JoinColumn(name = "UNI_ID")
    private Unidade unidade;
	
	@NotNull
	@Column(name = "GRS_CODIGO")
	@Size(min = 3, max = 20)
	private String codigo;
	
    @NotNull
    @Column(name = "GRS_NOME")
    @Size(min = 3, max = 20)
    private String nome;

    @ManyToMany(cascade = CascadeType.ALL)
    private Set<Sala> salas = new HashSet<Sala>();

    @ManyToMany(cascade = CascadeType.ALL)
    private Set<Disciplina> disciplinas = new HashSet<Disciplina>();

	private static final long serialVersionUID = -3068409934520158819L;

	public Unidade getUnidade() {
        return this.unidade;
    }

	public void setUnidade(Unidade unidade) {
        this.unidade = unidade;
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

	public Set<Sala> getSalas() {
        return this.salas;
    }

	public void setSalas(Set<Sala> salas) {
        this.salas = salas;
    }

	public Set<Disciplina> getDisciplinas() {
        return this.disciplinas;
    }

	public void setDisciplinas(Set<Disciplina> disciplinas) {
        this.disciplinas = disciplinas;
    }

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Unidade: ").append(getUnidade()).append(", ");
        sb.append("Codigo: ").append(getCodigo()).append(", ");
        sb.append("Nome: ").append(getNome()).append(", ");
        sb.append("Salas: ").append(getSalas() == null ? "null" : getSalas().size()).append(", ");
        sb.append("Disciplinas: ").append(getDisciplinas() == null ? "null" : getDisciplinas().size());
        return sb.toString();
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "GRS_ID")
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
            GrupoSala attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public GrupoSala merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        GrupoSala merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new GrupoSala().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

    @SuppressWarnings("unchecked")
    public static List<GrupoSala> findByNomeLikeAndCodigoLikeAndUnidade(String nome, String codigo, Unidade unidade, int firstResult, int maxResults, String orderBy) {
        nome = (nome == null)? "" : nome;
        nome = "%" + nome.replace('*', '%') + "%";
        codigo = (codigo == null)? "" : codigo;
        codigo = "%" + codigo.replace('*', '%') + "%";
        
        orderBy = (orderBy != null) ? "ORDER BY o." + orderBy : "";
        String unidadeQuery = (unidade==null)? "" : "o.unidade = :unidade AND ";
        Query q = entityManager().createQuery("SELECT o FROM GrupoSala o WHERE " +
        		"LOWER(o.nome) LIKE LOWER(:nome) AND " +
        		"LOWER(o.codigo) LIKE LOWER(:codigo) AND " +
        		unidadeQuery +
        		" 1=1 " +
        		""+orderBy);
        q.setParameter("nome", nome);
        q.setParameter("codigo", codigo);
        if(unidade != null) q.setParameter("unidade", unidade);
        return q.setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
	
	public static int count() {
        return ((Number) entityManager().createQuery("select count(o) from GrupoSala o").getSingleResult()).intValue();
    }

	@SuppressWarnings("unchecked")
    public static List<GrupoSala> findAll() {
        return entityManager().createQuery("select o from GrupoSala o").getResultList();
    }

	public static GrupoSala find(Long id) {
        if (id == null) return null;
        return entityManager().find(GrupoSala.class, id);
    }
	
	public static List<GrupoSala> find(int firstResult, int maxResults) {
		return find(firstResult, maxResults, null);
	}

	@SuppressWarnings("unchecked")
	public static List<GrupoSala> find(int firstResult, int maxResults, String orderBy) {
		orderBy = (orderBy != null) ? "ORDER BY o." + orderBy : "";
        return entityManager().createQuery("select o from GrupoSala o " + orderBy).setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
}
