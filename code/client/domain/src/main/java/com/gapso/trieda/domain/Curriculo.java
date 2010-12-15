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
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
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
@RooEntity(identifierColumn = "CRC_ID")
@Table(name = "CURRICULOS")
public class Curriculo implements Serializable {

    @NotNull
    @ManyToOne(targetEntity = Curso.class)
    @JoinColumn(name = "CUR_ID")
    private Curso curso;

    @NotNull
    @ManyToOne(targetEntity = Cenario.class)
    @JoinColumn(name = "CEN_ID")
    private Cenario cenario;

    @NotNull
    @Column(name = "CRC_COD")
    @Size(min = 1, max = 20)
    private String codigo;

    @Column(name = "CRC_DESCRICAO")
    @Size(max = 255)
    private String descricao;

    @OneToMany(cascade = CascadeType.ALL, mappedBy = "curriculo")
    private Set<CurriculoDisciplina> disciplinas = new HashSet<CurriculoDisciplina>();

    @OneToMany(cascade = CascadeType.ALL, mappedBy = "curriculo")
    private Set<Oferta> ofertas = new HashSet<Oferta>();

	public Curso getCurso() {
        return this.curso;
    }

	public void setCurso(Curso curso) {
        this.curso = curso;
    }

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

	public Set<CurriculoDisciplina> getDisciplinas() {
        return this.disciplinas;
    }

	public void setDisciplinas(Set<CurriculoDisciplina> disciplinas) {
        this.disciplinas = disciplinas;
    }

	public Set<Oferta> getOfertas() {
        return this.ofertas;
    }

	public void setOfertas(Set<Oferta> ofertas) {
        this.ofertas = ofertas;
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "CRC_ID")
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
	public void detach() {
		if (this.entityManager == null) this.entityManager = entityManager();
		this.entityManager.detach(this);
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
            Curriculo attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Curriculo merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Curriculo merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Curriculo().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static int count() {
        return ((Number) entityManager().createQuery("select count(o) from Curriculo o").getSingleResult()).intValue();
    }
	
	@SuppressWarnings("unchecked")
    public static List<Curriculo> findAll() {
        return entityManager().createQuery("select o from Curriculo o").getResultList();
    }

	public static Curriculo find(Long id) {
        if (id == null) return null;
        return entityManager().find(Curriculo.class, id);
    }

	public static List<Curriculo> find(int firstResult, int maxResults) {
		return find(firstResult, maxResults, null);
	}
	@SuppressWarnings("unchecked")
    public static List<Curriculo> find(int firstResult, int maxResults, String orderBy) {
		orderBy = (orderBy != null) ? "ORDER BY o." + orderBy : "";
        return entityManager().createQuery("select o from Curriculo o "+orderBy).setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
	
	@SuppressWarnings("unchecked")
	public static List<Curriculo> findByCampusAndTurno(Campus campus, Turno turno) {
		Query q = entityManager().createQuery("SELECT o.curriculo FROM Oferta o WHERE o.campus = :campus AND o.turno = :turno");
		q.setParameter("campus", campus);
		q.setParameter("turno", turno);
		return q.getResultList();
	}
	
    @SuppressWarnings("unchecked")
	public static List<Curriculo> findByCursoAndCodigoLikeAndDescricaoLike(Curso curso, String codigo, String descricao, int firstResult, int maxResults, String orderBy) {

        codigo = (codigo == null)? "" : codigo;
        codigo = "%" + codigo.replace('*', '%') + "%";
        descricao = (descricao == null)? "" : descricao;
        descricao = "%" + descricao.replace('*', '%') + "%";
        
        orderBy = (orderBy != null) ? "ORDER BY o." + orderBy : "";
        String queryCurso = "";
        if(curso != null) {
        	queryCurso = "o.curso = :curso AND";
        }
        Query q = entityManager().createQuery("SELECT o FROM Curriculo o WHERE "+queryCurso+" LOWER(o.descricao) LIKE LOWER(:descricao) AND LOWER(o.codigo) LIKE LOWER(:codigo)");
        if(curso != null) {
        	q.setParameter("curso", curso);
        }
        q.setParameter("codigo", codigo);
        q.setParameter("descricao", descricao);
        return q.setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

    public int getPeriodo(Disciplina disciplina) {
    	Query q = entityManager().createQuery("SELECT o.periodo FROM CurriculoDisciplina o WHERE o.curriculo = :curriculo AND o.disciplina = :disciplina");
    	q.setParameter("curriculo", this);
    	q.setParameter("disciplina", disciplina);
    	return (Integer) q.getSingleResult();
    }
    
    @SuppressWarnings("unchecked")
    public List<Integer> getPeriodos() {
    	Query q = entityManager().createQuery("SELECT DISTINCT(o.periodo) FROM CurriculoDisciplina o WHERE o.curriculo = :curriculo");
    	q.setParameter("curriculo", this);
    	return (List<Integer>) q.getResultList();
    }
    
	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Curso: ").append(getCurso()).append(", ");
        sb.append("Cenario: ").append(getCenario()).append(", ");
        sb.append("Codigo: ").append(getCodigo()).append(", ");
        sb.append("Descricao: ").append(getDescricao()).append(", ");
        sb.append("Disciplinas: ").append(getDisciplinas() == null ? "null" : getDisciplinas().size()).append(", ");
        sb.append("Ofertas: ").append(getOfertas() == null ? "null" : getOfertas().size());
        return sb.toString();
    }

	private static final long serialVersionUID = -9204016994046445376L;
}
