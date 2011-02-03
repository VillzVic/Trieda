package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.ArrayList;
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
@RooEntity(identifierColumn = "SAL_ID")
@Table(name = "SALAS")
public class Sala implements Serializable {

    @NotNull
    @ManyToOne(targetEntity = TipoSala.class)
    @JoinColumn(name = "TSA_ID")
    private TipoSala tipoSala;

    @NotNull
    @ManyToOne(targetEntity = Unidade.class)
    @JoinColumn(name = "UNI_ID")
    private Unidade unidade;

    @NotNull
    @Column(name = "SAL_CODIGO")
    @Size(min = 1, max = 20)
    private String codigo;

    @NotNull
    @Column(name = "SAL_NUMERO")
    @Size(min = 1, max = 20)
    private String numero;

    @NotNull
    @Column(name = "SAL_ANDAR")
    @Size(min = 1, max = 20)
    private String andar;

    @NotNull
    @Column(name = "SAL_CAPACIDADE")
    @Min(1L)
    @Max(9999L)
    private Integer capacidade;

    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE}, mappedBy = "salas")
    private Set<HorarioDisponivelCenario> horarios = new HashSet<HorarioDisponivelCenario>();

    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE}, mappedBy = "salas")
    private Set<CurriculoDisciplina> curriculoDisciplinas = new HashSet<CurriculoDisciplina>();

    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE}, mappedBy = "salas")
    private Set<GrupoSala> gruposSala = new HashSet<GrupoSala>();

    @OneToMany(cascade = CascadeType.ALL, mappedBy="sala")
    private Set<AtendimentoOperacional> atendimentosOperacionais =  new HashSet<AtendimentoOperacional>();

    @OneToMany(cascade = CascadeType.ALL, mappedBy="sala")
    private Set<AtendimentoTatico> atendimentosTaticos =  new HashSet<AtendimentoTatico>();
    
    @OneToMany(cascade = CascadeType.REMOVE, mappedBy = "sala")
    private Set<Fixacao> fixacoes = new HashSet<Fixacao>();
    
	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("TipoSala: ").append(getTipoSala()).append(", ");
        sb.append("Unidade: ").append(getUnidade()).append(", ");
        sb.append("Codigo: ").append(getCodigo()).append(", ");
        sb.append("Numero: ").append(getNumero()).append(", ");
        sb.append("Andar: ").append(getAndar()).append(", ");
        sb.append("Capacidade: ").append(getCapacidade()).append(", ");
        sb.append("Horarios: ").append(getHorarios() == null ? "null" : getHorarios().size()).append(", ");
        sb.append("CurriculoDisciplinas: ").append(getCurriculoDisciplinas() == null ? "null" : getCurriculoDisciplinas().size()).append(", ");
        sb.append("GruposSala: ").append(getGruposSala() == null ? "null" : getGruposSala().size());
        sb.append("Atendimentos Operacionais: ").append(getAtendimentosOperacionais() == null ? "null" : getAtendimentosOperacionais().size());
        sb.append("Atendimentos Taticos: ").append(getAtendimentosTaticos() == null ? "null" : getAtendimentosTaticos().size());
        sb.append("Fixacoes: ").append(getFixacoes() == null ? "null" : getFixacoes().size());
        return sb.toString();
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "SAL_ID")
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
        	this.removeHorariosDisponivelCenario();
        	this.removeCurriculoDisciplinas();
        	this.removeGruposSala();
            this.entityManager.remove(this);
        } else {
            Sala attached = this.entityManager.find(this.getClass(), this.id);
            attached.removeHorariosDisponivelCenario();
            attached.removeCurriculoDisciplinas();
            attached.removeGruposSala();
            this.entityManager.remove(attached);
        }
    }

    @Transactional
    public void removeHorariosDisponivelCenario() {
    	Set<HorarioDisponivelCenario> horarios = this.getHorarios();
    	for(HorarioDisponivelCenario horario : horarios) {
    		horario.getSalas().remove(this);
    		horario.merge();
    	}
    }
	
    @Transactional
    public void removeCurriculoDisciplinas() {
    	Set<CurriculoDisciplina> curriculoDisciplinas = this.getCurriculoDisciplinas();
    	for(CurriculoDisciplina curriculoDisciplina : curriculoDisciplinas) {
    		curriculoDisciplina.getSalas().remove(this);
    		curriculoDisciplina.merge();
    	}
    }
    
    @Transactional
    public void removeGruposSala() {
    	Set<GrupoSala> gruposSala = this.getGruposSala();
    	for(GrupoSala grupoSala : gruposSala) {
    		grupoSala.getSalas().remove(this);
    		grupoSala.merge();
    	}
    }
    
	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Sala merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Sala merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Sala().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static int count() {
        return ((Number) entityManager().createQuery("select count(o) from Sala o").getSingleResult()).intValue();
    }
	
	@SuppressWarnings("unchecked")
	public List<Curriculo> getCurriculos() {
		Query q = entityManager().createQuery("SELECT DISTINCT(cd.curriculo) FROM CurriculoDisciplina cd WHERE :sala IN ELEMENTS(cd.salas)");
		q.setParameter("sala", this);
		return q.getResultList();
	}
	
	public static List<Sala> findAndaresAll() {
		return findAndaresAll(null);
	}
	@SuppressWarnings("unchecked")
	public static List<Sala> findAndaresAll(Unidade unidade) {
		List<Sala> list;
		if(unidade == null) {
			list = entityManager().createQuery("SELECT o FROM Sala o GROUP BY o.andar").getResultList();
		} else {
			list = entityManager().createQuery("SELECT o FROM Sala o WHERE o.unidade = :unidade GROUP BY o.andar")
			.setParameter("unidade", unidade)
			.getResultList();
		}
		return list;
	}
	
	@SuppressWarnings("unchecked")
	public static List<Sala> findSalasDoAndarAll(List<String> andares) {
		if(andares.size() == 0) return new ArrayList<Sala>();
		String whereQuery = "SELECT o FROM Sala o WHERE ";
		for(int i = 1; i < andares.size(); i++) {
			whereQuery += " o.andar = :andares"+i+" OR ";
		}
		whereQuery += " o.andar = :andares0 ";
		Query query = entityManager().createQuery(whereQuery);
		for(int i = 0; i < andares.size(); i++) {
			query.setParameter("andares"+i, andares.get(i));
		}
		return query.getResultList();
	}

	@SuppressWarnings("unchecked")
    public static List<Sala> findAll() {
        return entityManager().createQuery("SELECT o FROM Sala o").getResultList();
    }

	public static Sala find(Long id) {
        if (id == null) return null;
        return entityManager().find(Sala.class, id);
    }
	
	@SuppressWarnings("unchecked")
	public static Sala findByCodigo(String codigo) {
		Query q = entityManager().createQuery("SELECT o FROM Sala o WHERE codigo = :codigo");
		q.setParameter("codigo", codigo);
		return (Sala) q.getSingleResult();
	}

	@SuppressWarnings("unchecked")
	public static List<Sala> findByUnidade(Unidade unidade) {
		return entityManager().createQuery("SELECT o FROM Sala o WHERE unidade = :unidade")
		.setParameter("unidade", unidade)
		.getResultList();
	}
	
	public static List<Sala> find(int firstResult, int maxResults) {
		return find(firstResult, maxResults, null);
	}
	@SuppressWarnings("unchecked")
    public static List<Sala> find(int firstResult, int maxResults, String orderBy) {
		orderBy = (orderBy != null)? "ORDER BY o."+orderBy : "";
        return entityManager().createQuery("SELECT o FROM Sala o "+orderBy).setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	public TipoSala getTipoSala() {
        return this.tipoSala;
    }

	public void setTipoSala(TipoSala tipoSala) {
        this.tipoSala = tipoSala;
    }

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

	public String getNumero() {
        return this.numero;
    }

	public void setNumero(String numero) {
        this.numero = numero;
    }

	public String getAndar() {
        return this.andar;
    }

	public void setAndar(String andar) {
        this.andar = andar;
    }

	public Integer getCapacidade() {
        return this.capacidade;
    }

	public void setCapacidade(Integer capacidade) {
        this.capacidade = capacidade;
    }

	public Set<HorarioDisponivelCenario> getHorarios() {
        return this.horarios;
    }

	public void setHorarios(Set<HorarioDisponivelCenario> horarios) {
        this.horarios = horarios;
    }

	public Set<CurriculoDisciplina> getCurriculoDisciplinas() {
        return this.curriculoDisciplinas;
    }

	public void setCurriculoDisciplinas(Set<CurriculoDisciplina> curriculoDisciplinas) {
        this.curriculoDisciplinas = curriculoDisciplinas;
    }

	public Set<GrupoSala> getGruposSala() {
        return this.gruposSala;
    }

	public void setGruposSala(Set<GrupoSala> gruposSala) {
        this.gruposSala = gruposSala;
    }
	
	public Set<AtendimentoOperacional> getAtendimentosOperacionais() {
		return this.atendimentosOperacionais;
	}
	
	public void setAtendimentosOperacionais(Set<AtendimentoOperacional> atendimentosOperacionais) {
		this.atendimentosOperacionais = atendimentosOperacionais;
	}
	
	public Set<AtendimentoTatico> getAtendimentosTaticos() {
		return this.atendimentosTaticos;
	}
	
	public void setAtendimentosTaticos(Set<AtendimentoTatico> atendimentosTaticos) {
		this.atendimentosTaticos = atendimentosTaticos;
	}
	
	public Set<Fixacao> getFixacoes() {
		return this.fixacoes;
	}
	
	public void setFixacoes(Set<Fixacao> fixacoes) {
		this.fixacoes = fixacoes;
	}
	
	private static final long serialVersionUID = -2533999449644229682L;
}
