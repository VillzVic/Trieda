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
import javax.persistence.OneToMany;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.Version;
import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
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
@RooEntity(identifierColumn = "TUR_ID")
@Table(name = "TURNOS")
public class Turno implements java.io.Serializable {

    @NotNull
    @Column(name = "TUR_NOME")
    @Size(max = 50)
    private String nome;

    @NotNull
    @Column(name = "TUR_TEMPO")
    @Min(1L)
    @Max(1000L)
    private Integer tempo;

    @NotNull
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "turno")
    private Set<com.gapso.trieda.domain.CampusCurriculo> campusCurriculo = new java.util.HashSet<com.gapso.trieda.domain.CampusCurriculo>();

    @PersistenceContext
    transient EntityManager entityManager;

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "TUR_ID")
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
            Turno attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

    @Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

    @Transactional
    public Turno merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Turno merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

    public static final EntityManager entityManager() {
        EntityManager em = new Turno().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

    public static int count() {
        return ((Number) entityManager().createQuery("select count(o) from Turno o").getSingleResult()).intValue();
    }

    @SuppressWarnings("unchecked")
    public static List<Turno> findAll() {
        return entityManager().createQuery("select o from Turno o").getResultList();
    }

    public static Turno find(Long id) {
        if (id == null) return null;
        return entityManager().find(Turno.class, id);
    }

    public static List<Turno> find(int firstResult, int maxResults) {
        return find(firstResult, maxResults, null);
    }

    @SuppressWarnings("unchecked")
    public static List<Turno> find(int firstResult, int maxResults, String orderBy) {
        orderBy = (orderBy != null) ? "ORDER BY o." + orderBy : "";
        return entityManager().createQuery("SELECT o FROM Turno o " + orderBy).setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

    @SuppressWarnings("unchecked")
    public static List<Turno> findByNomeLikeAndTempo(String nome, Integer tempo, int firstResult, int maxResults, String orderBy) {
    	nome = (nome == null || nome.length() == 0)? "" : nome;
        nome = nome.replace('*', '%');
        if (nome == "" || nome.charAt(0) != '%') {
            nome = "%" + nome;
        }
        if (nome.charAt(nome.length() -1) != '%') {
            nome = nome + "%";
        }
        
        EntityManager em = Turno.entityManager();
        String queryTempo = (tempo != null)? "AND turno.tempo = :tempo" : "";
        orderBy = (orderBy != null) ? "ORDER BY o." + orderBy : "";
        Query q = em.createQuery("SELECT Turno FROM Turno AS turno WHERE LOWER(turno.nome) LIKE LOWER(:nome) "+queryTempo+" "+orderBy);
        q.setParameter("nome", nome);
        if(tempo != null) q.setParameter("tempo", tempo);
        return q.setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
    
    public String getNome() {
        return this.nome;
    }

    public void setNome(String nome) {
        this.nome = nome;
    }

    public Integer getTempo() {
        return this.tempo;
    }

    public void setTempo(Integer tempo) {
        this.tempo = tempo;
    }

    public Set<CampusCurriculo> getCampusCurriculo() {
        return this.campusCurriculo;
    }

    public void setCampusCurriculo(Set<CampusCurriculo> campusCurriculo) {
        this.campusCurriculo = campusCurriculo;
    }

    private static final long serialVersionUID = 2608398950191790873L;

    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Nome: ").append(getNome()).append(", ");
        sb.append("Tempo: ").append(getTempo()).append(", ");
        sb.append("CampusCurriculo: ").append(getCampusCurriculo() == null ? "null" : getCampusCurriculo().size());
        return sb.toString();
    }
}
