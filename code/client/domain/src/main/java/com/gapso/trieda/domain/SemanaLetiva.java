package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.OneToMany;
import javax.persistence.OneToOne;
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
@RooEntity(identifierColumn = "SLE_ID")
@Table(name = "SEMANA_LETIVA")
public class SemanaLetiva implements Serializable {
	
    @OneToOne(mappedBy="semanaLetiva")
    private Cenario cenario;

    @NotNull
    @Column(name = "SLE_CODIGO")
    @Size(min = 3, max = 20)
    private String codigo;

    @Column(name = "SLE_DESCRICAO")
    @Size(max = 255)
    private String descricao;
    
    @OneToMany(mappedBy="semanaLetiva", fetch = FetchType.EAGER)
    private Set<HorarioAula> horariosAula =  new HashSet<HorarioAula>();

	private static final long serialVersionUID = 6807360646327130208L;

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
	
	public Set<HorarioAula> getHorariosAula() {
        return this.horariosAula;
    }

	public void setHorariosAula(Set<HorarioAula> horariosAula) {
        this.horariosAula = horariosAula;
    }
    
	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "SLE_ID")
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
            SemanaLetiva attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public SemanaLetiva merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        SemanaLetiva merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new SemanaLetiva().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static int count() {
        return ((Number) entityManager().createQuery("select count(o) from SemanaLetiva o").getSingleResult()).intValue();
    }

	@SuppressWarnings("unchecked")
    public static List<SemanaLetiva> findAll() {
        return entityManager().createQuery("select o from SemanaLetiva o").getResultList();
    }

	public static SemanaLetiva find(Long id) {
        if (id == null) return null;
        SemanaLetiva o = entityManager().find(SemanaLetiva.class, id);
        o.getHorariosAula();
        return o;
    }

	public static List<SemanaLetiva> find(int firstResult, int maxResults) {
		return find(firstResult, maxResults, null);
	}
	@SuppressWarnings("unchecked")
    public static List<SemanaLetiva> find(int firstResult, int maxResults, String orderBy) {
		orderBy = (orderBy != null)? "ORDER BY o."+orderBy : "";
        return entityManager().createQuery("select o from SemanaLetiva o "+orderBy).setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

    @SuppressWarnings("unchecked")
    public static List<SemanaLetiva> findByCodigoLikeAndDescricaoLike(String codigo, String descricao, int firstResult, int maxResults, String orderBy) {
        codigo = (codigo == null)? "" : codigo;
        codigo = "%" + codigo.replace('*', '%') + "%";
        descricao = (descricao == null)? "" : descricao;
        descricao = "%" + descricao.replace('*', '%') + "%";
        
        EntityManager em = Turno.entityManager();
        orderBy = (orderBy != null) ? "ORDER BY o." + orderBy : "";
        Query q = em.createQuery("SELECT o FROM SemanaLetiva o WHERE LOWER(o.codigo) LIKE LOWER(:codigo) AND LOWER(o.descricao) LIKE LOWER(:descricao) "+orderBy);
        q.setParameter("codigo", codigo);
        q.setParameter("descricao", descricao);
        return q.setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
	
	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Cenario: ").append(getCenario()).append(", ");
        sb.append("Codigo: ").append(getCodigo()).append(", ");
        sb.append("HorariosAula: ").append(getHorariosAula() == null ? "null" : getHorariosAula().size()).append(", ");
        sb.append("Descricao: ").append(getDescricao());
        return sb.toString();
    }
}
