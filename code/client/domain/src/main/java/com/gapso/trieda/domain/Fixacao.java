package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.FetchType;
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
@RooEntity( identifierColumn = "FIX_ID" )
@Table( name = "FIXACOES" )
public class Fixacao
	implements Serializable
{
	private static final long serialVersionUID = -7545908494415718467L;

	@NotNull
    @Column(name = "FIX_CODIGO")
    @Size(min = 1, max = 50)
    private String codigo;
    
    @NotNull
    @Column(name = "FIX_DESCRICAO")
    @Size(min = 1, max = 50)
    private String descricao;
    
    @ManyToOne(targetEntity = Professor.class, fetch=FetchType.LAZY)
    @JoinColumn(name = "PRF_ID")
    private Professor professor;
	
    @ManyToOne(targetEntity = Disciplina.class, fetch=FetchType.LAZY)
    @JoinColumn(name = "DIS_ID")
    private Disciplina disciplina;
    
    @ManyToOne(targetEntity = Campus.class)
    @JoinColumn(name = "CAM_ID")
    private Campus campus;
    
    @ManyToOne(targetEntity = Unidade.class)
    @JoinColumn(name = "UNI_ID")
    private Unidade unidade;
    
    @ManyToOne(targetEntity = Sala.class)
    @JoinColumn(name = "SAL_ID")
    private Sala sala;
    
    @ManyToMany( cascade = { CascadeType.PERSIST, CascadeType.MERGE }, mappedBy = "fixacoes" )
    private Set< HorarioDisponivelCenario > horarios = new HashSet< HorarioDisponivelCenario >();

    @PersistenceContext
    transient EntityManager entityManager;

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "FIX_ID")
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
    public void persist()
    {
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        this.entityManager.persist( this );
    }

    @Transactional
    public void remove()
    {
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        if ( this.entityManager.contains( this ) )
        {
        	this.removeHorariosDisponivelCenario();
            this.entityManager.remove( this );
        }
        else
        {
            Fixacao attached = this.entityManager.find(
            	this.getClass(), this.id );

            attached.removeHorariosDisponivelCenario();
            this.entityManager.remove( attached );
        }
    }

    @Transactional
    public void removeHorariosDisponivelCenario()
    {
    	Set< HorarioDisponivelCenario > horarios = this.getHorarios();
    	for ( HorarioDisponivelCenario horario : horarios )
    	{
    		horario.getFixacoes().remove( this );
    		horario.merge();
    	}
    }

	@Transactional
	public void detach() {
		if (this.entityManager == null) this.entityManager = entityManager();
		this.entityManager.detach(this);
	}
    
    @Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

    @Transactional
    public Fixacao merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Fixacao merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

    public static final EntityManager entityManager() {
        EntityManager em = new Fixacao().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

    public static int count() {
        return ((Number) entityManager().createQuery("SELECT COUNT(o) FROM Fixacao o").getSingleResult()).intValue();
    }

    @SuppressWarnings("unchecked")
    public static List<Fixacao> findAll() {
        return entityManager().createQuery("SELECT o FROM Fixacao o").getResultList();
    }

    public static Fixacao find(Long id) {
        if (id == null) return null;
        return entityManager().find(Fixacao.class, id);
    }

    public static List<Fixacao> find(int firstResult, int maxResults) {
        return find(firstResult, maxResults, null);
    }

    @SuppressWarnings("unchecked")
    public static List<Fixacao> find(int firstResult, int maxResults, String orderBy) {
        orderBy = (orderBy != null) ? "ORDER BY o." + orderBy : "";
        return entityManager().createQuery("SELECT o FROM Fixacao o " + orderBy).setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
    
    @SuppressWarnings("unchecked")
    public static List<Fixacao> findAllBy(Disciplina disciplina) {
        Query q = entityManager().createQuery("SELECT o FROM Fixacao o WHERE o.disciplina = :disciplina");
        q.setParameter("disciplina", disciplina);
        return q.getResultList();
    }
    
    @SuppressWarnings("unchecked")
    public static List<Fixacao> findBy(String codigo, int firstResult, int maxResults, String orderBy) {
    	codigo = (codigo == null || codigo.length() == 0)? "" : codigo;
        codigo = codigo.replace('*', '%');
        if (codigo == "" || codigo.charAt(0) != '%') {
            codigo = "%" + codigo;
        }
        if (codigo.charAt(codigo.length() -1) != '%') {
            codigo = codigo + "%";
        }
        
        orderBy = (orderBy != null) ? "ORDER BY o." + orderBy : "";
        Query q = entityManager().createQuery("SELECT o FROM Fixacao o WHERE LOWER(o.codigo) LIKE LOWER(:codigo) "+orderBy);
        q.setParameter("codigo", codigo);
        return q.setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
    
	@SuppressWarnings("unchecked")
	public List<HorarioDisponivelCenario> getHorarios(SemanaLetiva semanaLetiva) {
		Query q = entityManager().createQuery("SELECT o FROM HorarioDisponivelCenario o, IN (o.fixacoes) c WHERE c = :fixacao AND o.horarioAula.semanaLetiva = :semanaLetiva");
		q.setParameter("fixacao", this);
		q.setParameter("semanaLetiva", semanaLetiva);
		return q.getResultList();
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
    
    public Professor getProfessor() {
    	return this.professor;
    }
    public void setProfessor(Professor professor) {
    	this.professor = professor;
    }

	public Disciplina getDisciplina() {
        return this.disciplina;
    }
	public void setDisciplina(Disciplina disciplina) {
        this.disciplina = disciplina;
    }
	
	public Campus getCampus() {
		return this.campus;
	}
	public void setCampus(Campus campus) {
		this.campus = campus;
	}
	
	public Unidade getUnidade() {
		return this.unidade;
	}
	public void setUnidade(Unidade unidade) {
		this.unidade = unidade;
	}
	
	public Sala getSala() {
		return this.sala;
	}
	public void setSala(Sala sala) {
		this.sala = sala;
	}

	private Set<HorarioDisponivelCenario> getHorarios() {
        return this.horarios;
    }

	public void setHorarios(Set<HorarioDisponivelCenario> horarios) {
        this.horarios = horarios;
    }
	
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Codigo: ").append(getCodigo()).append(", ");
        sb.append("Professor: ").append(getProfessor()).append(", ");
        sb.append("Descricao: ").append(getDescricao()).append(", ");
        sb.append("Disciplina: ").append(getDisciplina()).append(", ");
        sb.append("Campus: ").append(getCampus()).append(", ");
        sb.append("Unidade: ").append(getUnidade()).append(", ");
        sb.append("Sala: ").append(getSala()).append(", ");
        sb.append("Horarios: ").append(getHorarios() == null ? "null" : getHorarios().size()).append(", ");
        return sb.toString();
    }

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((id == null) ? 0 : id.hashCode());
		result = prime * result + ((version == null) ? 0 : version.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Fixacao other = (Fixacao) obj;
		if (id == null) {
			if (other.id != null)
				return false;
		} else if (!id.equals(other.id))
			return false;
		if (version == null) {
			if (other.version != null)
				return false;
		} else if (!version.equals(other.version))
			return false;
		return true;
	}
    
    
}
