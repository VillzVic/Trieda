package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
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
import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.misc.Semanas;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "TUR_ID")
@Table(name = "TURNOS")
public class Turno implements Serializable {

	@NotNull
    @ManyToOne(targetEntity = Cenario.class)
    @JoinColumn(name = "CEN_ID")
    private Cenario cenario;
	
    @NotNull
    @Column(name = "TUR_NOME")
    @Size(min = 1, max = 50)
    private String nome;

    @NotNull
    @Column(name = "TUR_TEMPO")
    @Min(1L)
    @Max(1000L)
    private Integer tempo;

    @NotNull
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "turno")
    private Set<Oferta> ofertas = new HashSet<Oferta>();

    @OneToMany(mappedBy="turno")
    Set<HorarioAula> horariosAula = new HashSet<HorarioAula>();
    
    @OneToMany(cascade = CascadeType.ALL, mappedBy="turno")
    private Set<Parametro> parametros =  new HashSet<Parametro>();
    
    @PersistenceContext
    transient EntityManager entityManager;

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "TUR_ID")
    private Long id;

    @Version
    @Column(name = "version")
    private Integer version;
    
    public int calculaMaxCreditos () {
    	Map<Integer,Integer> countHorariosAula = new HashMap<Integer, Integer>();
		for (HorarioAula ha : getHorariosAula()) {
			for (HorarioDisponivelCenario hdc : ha.getHorariosDisponiveisCenario()) {
				int semanaInt = Semanas.toInt(hdc.getSemana());
				Integer value = countHorariosAula.get(semanaInt);
				value = (value == null) ? 0 : value;
				countHorariosAula.put(semanaInt,value+1);
			}
		}
		
		return Collections.max(countHorariosAula.values());
    }

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
    
    @SuppressWarnings("unchecked")
    public static List<Turno> findBy(Campus campus) {
    	Query q = entityManager().createQuery("SELECT distinct(o.turno) FROM Oferta o WHERE o.campus = :campus");
    	q.setParameter("campus", campus);
    	return q.getResultList();
    }
    
    @SuppressWarnings("unchecked")
    public static List<Turno> findByNome(String nome) {
    	Query q = entityManager().createQuery("SELECT o FROM Turno o WHERE o.nome = :nome");
    	q.setParameter("nome", nome);
    	return q.getResultList();
    }

    @SuppressWarnings("unchecked")
    public static List<Turno> findAll() {
        return entityManager().createQuery("SELECT o FROM Turno o").getResultList();
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
    
    public static int count(String nome, Integer tempo) {
    	nome = (nome == null || nome.length() == 0)? "" : nome;
    	nome = nome.replace('*', '%');
    	if (nome == "" || nome.charAt(0) != '%') {
    		nome = "%" + nome;
    	}
    	if (nome.charAt(nome.length() -1) != '%') {
    		nome = nome + "%";
    	}
    	
    	String queryTempo = (tempo != null)? "AND o.tempo = :tempo" : "";
    	Query q = entityManager().createQuery("SELECT COUNT(o) FROM Turno o WHERE LOWER(o.nome) LIKE LOWER(:nome) "+queryTempo);
    	q.setParameter("nome", nome);
    	if(tempo != null) q.setParameter("tempo", tempo);
    	return ((Number)q.getSingleResult()).intValue();
    }

    @SuppressWarnings("unchecked")
    public static List<Turno> findBy(String nome, Integer tempo, int firstResult, int maxResults, String orderBy) {
    	nome = (nome == null || nome.length() == 0)? "" : nome;
        nome = nome.replace('*', '%');
        if (nome == "" || nome.charAt(0) != '%') {
            nome = "%" + nome;
        }
        if (nome.charAt(nome.length() -1) != '%') {
            nome = nome + "%";
        }
        
        String queryTempo = (tempo != null)? "AND turno.tempo = :tempo" : "";
        orderBy = (orderBy != null) ? "ORDER BY o." + orderBy : "";
        Query q = entityManager().createQuery("SELECT o FROM Turno o WHERE LOWER(o.nome) LIKE LOWER(:nome) "+queryTempo+" "+orderBy);
        q.setParameter("nome", nome);
        if(tempo != null) q.setParameter("tempo", tempo);
        return q.setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
    
	public static Map<String,Turno> buildTurnoNomeToTurnoMap(List<Turno> turnos) {
		Map<String,Turno> turnosMap = new HashMap<String,Turno>();
		for (Turno turno : turnos) {
			turnosMap.put(turno.getNome(),turno);
		}
		return turnosMap;
	}
    
	public Cenario getCenario() {
        return this.cenario;
    }
	public void setCenario(Cenario cenario) {
        this.cenario = cenario;
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

    public Set<Oferta> getOfertas() {
        return this.ofertas;
    }
    public void setOfertas(Set<Oferta> ofertas) {
        this.ofertas = ofertas;
    }

    public Set<HorarioAula> getHorariosAula() {
		return horariosAula;
	}
	public void setHorariosAula(Set<HorarioAula> horariosAula) {
		this.horariosAula = horariosAula;
	}

	public Set<Parametro> getParametros() {
		return parametros;
	}
	public void setParametros(Set<Parametro> parametros) {
		this.parametros = parametros;
	}

	private static final long serialVersionUID = 2608398950191790873L;

    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Cenario: ").append(getCenario()).append(", ");
        sb.append("Nome: ").append(getNome()).append(", ");
        sb.append("Tempo: ").append(getTempo()).append(", ");
        sb.append("Oferta: ").append(getOfertas() == null ? "null" : getOfertas().size());
        sb.append("HorariosAula: ").append(getHorariosAula() == null ? "null" : getHorariosAula().size());
        sb.append("Parametros: ").append(getParametros() == null ? "null" : getParametros().size()).append(", ");
        return sb.toString();
    }
}
