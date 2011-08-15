package com.gapso.trieda.domain;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
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
@RooEntity(identifierColumn = "TCU_ID")
@Table(name = "TIPOS_CURSO")
public class TipoCurso	
	implements java.io.Serializable
{
    @NotNull
    @Column(name = "TCU_CODIGO")
    @Size(min = 1, max = 50)
    private String codigo;
    
    @Column(name = "TCU_DESCRICAO")
    @Size(min = 3, max = 255)
    private String descricao;

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Codigo: ").append(getCodigo()).append(", ");
        sb.append("Descricao: ").append(getDescricao());
        return sb.toString();
    }

	private static final long serialVersionUID = 8519531461330290008L;

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
    @Column(name = "TCU_ID")
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
            TipoCurso attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public TipoCurso merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        TipoCurso merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager()
	{
        EntityManager em = new TipoCurso().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		"Entity manager has not been injected (is the Spring " +
        		"Aspects JAR configured as an AJC/AJDT aspects library?)" );
        }

        return em;
    }

	@SuppressWarnings("unchecked")
    public static List<TipoCurso> findAll() {
        return entityManager().createQuery("select o from TipoCurso o").getResultList();
    }
	
	public static Map< String, TipoCurso > buildTipoCursoCodigoToTipoCursoMap(
		List< TipoCurso > tiposCurso )
	{
		Map< String, TipoCurso > tiposCursoMap = new HashMap< String, TipoCurso >();

		for ( TipoCurso tipoCurso : tiposCurso )
		{
			tiposCursoMap.put( tipoCurso.getCodigo(),tipoCurso );
		}

		return tiposCursoMap;
	}

	public static TipoCurso find(Long id) {
        if (id == null) return null;
        return entityManager().find(TipoCurso.class, id);
    }
	
	public static List<TipoCurso> find(int firstResult, int maxResults) {
		return find(firstResult, maxResults, null);
	}
	@SuppressWarnings("unchecked")
    public static List<TipoCurso> find(int firstResult, int maxResults, String orderBy) {
		orderBy = (orderBy != null) ? "ORDER BY o." + orderBy : "";
        return entityManager().createQuery("select o from TipoCurso o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
	
	public static int count(String codigo, String descricao) {
		codigo = (codigo == null)? "" : codigo;
		codigo = "%" + codigo.replace('*', '%') + "%";
		if(descricao != null) {
			descricao = "%" + descricao.replace('*', '%') + "%";
		}
		
		String descricaoQuery = (descricao == null)? "" : "AND LOWER(o.descricao) LIKE LOWER(:descricao)";
		Query q = entityManager().createQuery("SELECT COUNT(o) FROM TipoCurso o WHERE LOWER(o.codigo) LIKE LOWER(:codigo) " + descricaoQuery );
		q.setParameter("codigo", codigo);
		if(descricao != null) q.setParameter("descricao", descricao);
		return ((Number)q.getSingleResult()).intValue();
	}
	
    @SuppressWarnings("unchecked")
    public static List<TipoCurso> findBy(String codigo, String descricao, int firstResult, int maxResults, String orderBy) {
    	codigo = (codigo == null)? "" : codigo;
        codigo = "%" + codigo.replace('*', '%') + "%";
        if(descricao != null) {
        	descricao = "%" + descricao.replace('*', '%') + "%";
        }
        
        orderBy = (orderBy != null) ? "ORDER BY o." + orderBy : "";
        String descricaoQuery = (descricao == null)? "" : "AND LOWER(o.descricao) LIKE LOWER(:descricao)";
        Query q = entityManager().createQuery("SELECT o FROM TipoCurso o WHERE LOWER(o.codigo) LIKE LOWER(:codigo) "+descricaoQuery+" "+orderBy);
        q.setParameter("codigo", codigo);
        if(descricao != null) q.setParameter("descricao", descricao);
        return q.setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
}
