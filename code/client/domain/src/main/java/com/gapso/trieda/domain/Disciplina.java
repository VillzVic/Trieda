package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.Enumerated;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.OneToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.Version;
import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.misc.Dificuldades;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "DIS_ID")
@Table(name = "DISCIPLINAS")
public class Disciplina implements Serializable, Comparable<Disciplina> {

	@NotNull
    @ManyToOne(cascade = {CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH}, targetEntity = Cenario.class, fetch = FetchType.LAZY)
    @JoinColumn(name = "CEN_ID")
    private Cenario cenario;

    @NotNull
    @ManyToOne(cascade = {CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH}, targetEntity = TipoDisciplina.class, fetch = FetchType.LAZY)
    @JoinColumn(name = "TDI_ID")
    private TipoDisciplina tipoDisciplina;

    @OneToOne(cascade = CascadeType.ALL, targetEntity = DivisaoCredito.class, fetch = FetchType.LAZY)
    @JoinColumn(name = "DCR_ID")
    private DivisaoCredito divisaoCreditos;

    @NotNull
    @Column(name = "DIS_CODIGO")
    @Size(min = 1, max = 20)
    private String codigo;

    @NotNull
    @Column(name = "DIS_NOME")
    @Size(min = 1, max = 50)
    private String nome;

    @NotNull
    @Column(name = "DIS_CRED_TEORICO")
    @Min(0L)
    @Max(99L)
    private Integer creditosTeorico;

    @NotNull
    @Column(name = "DIS_CRED_PRATICO")
    @Min(0L)
    @Max(99L)
    private Integer creditosPratico;

    @Column(name = "DIS_LABORATORIO")
    private Boolean laboratorio;

    @NotNull
    @Enumerated
    private Dificuldades dificuldade;

    @NotNull
    @Column(name = "DIS_MAX_ALUN_TEORICO")
    @Min(0L)
    @Max(999L)
    private Integer maxAlunosTeorico;

    @NotNull
    @Column(name = "DIS_MAX_ALUN_PRATICO")
    @Min(0L)
    @Max(999L)
    private Integer maxAlunosPratico;

    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE}, mappedBy = "disciplinas")
    private Set<HorarioDisponivelCenario> horarios = new HashSet<HorarioDisponivelCenario>();

    @NotNull
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "disciplina")
    private Set<ProfessorDisciplina> professores = new HashSet<ProfessorDisciplina>();

    @NotNull
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "disciplina1")
    private Set<Incompatibilidade> incompatibilidades = new HashSet<Incompatibilidade>();

    @OneToMany(cascade = CascadeType.ALL, mappedBy = "cursou")
    private Set<Equivalencia> equivalencias = new HashSet<Equivalencia>();
    
    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE}, mappedBy = "elimina")
    private Set<Equivalencia> eliminadaPor = new HashSet<Equivalencia>();

    @NotNull
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "disciplina")
    private Set<CurriculoDisciplina> curriculos = new HashSet<CurriculoDisciplina>();

    @NotNull
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "disciplina")
    private Set<Demanda> demandas = new HashSet<Demanda>();
    
    @OneToMany(cascade = CascadeType.REMOVE, mappedBy = "disciplina")
    private Set<Fixacao> fixacoes = new HashSet<Fixacao>();
    
    @OneToMany(cascade = CascadeType.ALL, mappedBy="disciplina")
    private Set<AtendimentoOperacional> atendimentosOperacionais =  new HashSet<AtendimentoOperacional>();

    @OneToMany(cascade = CascadeType.ALL, mappedBy="disciplina")
    private Set<AtendimentoTatico> atendimentosTaticos =  new HashSet<AtendimentoTatico>();
    
	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Cenario: ").append(getCenario()).append(", ");
        sb.append("TipoDisciplina: ").append(getTipoDisciplina()).append(", ");
        sb.append("DivisaoCreditos: ").append(getDivisaoCreditos()).append(", ");
        sb.append("Codigo: ").append(getCodigo()).append(", ");
        sb.append("Nome: ").append(getNome()).append(", ");
        sb.append("CreditosTeorico: ").append(getCreditosTeorico()).append(", ");
        sb.append("CreditosPratico: ").append(getCreditosPratico()).append(", ");
        sb.append("Laboratorio: ").append(getLaboratorio()).append(", ");
        sb.append("Dificuldade: ").append(getDificuldade()).append(", ");
        sb.append("MaxAlunosTeorico: ").append(getMaxAlunosTeorico()).append(", ");
        sb.append("MaxAlunosPratico: ").append(getMaxAlunosPratico()).append(", ");
        sb.append("Horarios: ").append(getHorarios() == null ? "null" : getHorarios().size()).append(", ");
        sb.append("Professores: ").append(getProfessores() == null ? "null" : getProfessores().size()).append(", ");
        sb.append("Incompatibilidades: ").append(getIncompatibilidades() == null ? "null" : getIncompatibilidades().size()).append(", ");
        sb.append("Equivalencias: ").append(getEquivalencias() == null ? "null" : getEquivalencias().size()).append(", ");
        sb.append("EliminadaPor: ").append(getEliminadaPor() == null ? "null" : getEliminadaPor().size()).append(", ");
        sb.append("Curriculos: ").append(getCurriculos() == null ? "null" : getCurriculos().size()).append(", ");
        sb.append("Demandas: ").append(getDemandas() == null ? "null" : getDemandas().size());
        sb.append("Fixacoes: ").append(getFixacoes() == null ? "null" : getFixacoes().size());
        sb.append("Atendimentos Operacionais: ").append(getAtendimentosOperacionais() == null ? "null" : getAtendimentosOperacionais().size());
        sb.append("Atendimentos Taticos: ").append(getAtendimentosTaticos() == null ? "null" : getAtendimentosTaticos().size());
        return sb.toString();
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "DIS_ID")
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
        	this.removeEliminadasPor();
            this.entityManager.remove(this);
        } else {
            Disciplina attached = this.entityManager.find(this.getClass(), this.id);
            attached.removeHorariosDisponivelCenario();
            attached.removeEliminadasPor();
            this.entityManager.remove(attached);
        }
    }

    @Transactional
    public void removeHorariosDisponivelCenario() {
    	Set<HorarioDisponivelCenario> horarios = this.getHorarios();
    	for(HorarioDisponivelCenario horario : horarios) {
    		horario.getDisciplinas().remove(this);
    		horario.merge();
    	}
    }
    
    @Transactional
    public void removeEliminadasPor() {
    	Set<Equivalencia> eliminadasPor = this.getEliminadaPor();
    	for(Equivalencia equivalencia : eliminadasPor) {
    		equivalencia.getElimina().remove(this);
    		equivalencia.merge();
    	}
    }
	
	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Disciplina merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Disciplina merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Disciplina().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public Incompatibilidade getIncompatibilidadeWith(Disciplina disciplina) {
		Query q = entityManager().createQuery("SELECT o FROM Incompatibilidade o WHERE o.disciplina1 = :disciplina1 AND o.disciplina2 = :disciplina2");
		q.setParameter("disciplina1", this);
		q.setParameter("disciplina2", disciplina);
		Incompatibilidade incomp = null;
		try{
			incomp = (Incompatibilidade) q.getSingleResult();
		} catch (EmptyResultDataAccessException e) {
			incomp = null;
		}
		return incomp;
	}

	public static int count(Cenario cenario) {
		Query q = entityManager().createQuery("SELECT COUNT(o) FROM Disciplina o WHERE o.cenario = :cenario");
		q.setParameter("cenario", cenario);
		return ((Number) q.getSingleResult()).intValue();
	}
	
	@SuppressWarnings("unchecked")
    public static List<Disciplina> findAll() {
        return entityManager().createQuery("SELECT o FROM Disciplina o").getResultList();
    }
	
	public static Map<String,Disciplina> buildDisciplinaCodigoToDisciplinaMap(List<Disciplina> disciplinas) {
		Map<String,Disciplina> disciplinasMap = new HashMap<String,Disciplina>();
		for (Disciplina disciplina : disciplinas) {
			disciplinasMap.put(disciplina.getCodigo(),disciplina);
		}
		return disciplinasMap;
	}

	public static Disciplina find(Long id) {
        if (id == null) return null;
        return entityManager().find(Disciplina.class, id);
    }
	
	public static List<Disciplina> find(int firstResult, int maxResults) {
		return find(firstResult, maxResults, null); 
	}

	@SuppressWarnings("unchecked")
    public static List<Disciplina> find(int firstResult, int maxResults, String orderBy) {
		orderBy = (orderBy != null)? "ORDER BY o."+orderBy : "";
        return entityManager().createQuery("SELECT o FROM Disciplina o "+orderBy).setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
	
	@SuppressWarnings("unchecked")
	public static List<Disciplina> findByCenario(Cenario cenario) {
    	Query q = entityManager().createQuery("SELECT o FROM Disciplina o WHERE o.cenario = :cenario");
    	q.setParameter("cenario", cenario);
    	return q.getResultList();
    }
	
	@SuppressWarnings("unchecked")
	public static List<Disciplina> findBySalas(List<Sala> salas) {
		Query q = entityManager().createQuery("SELECT DISTINCT(o.disciplina) FROM SalasDisciplinas o WHERE o.sala IN (:salas)")
		.setParameter("salas", salas);
		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List<Disciplina> findByCursos(List<Curso> cursos) {
		Query q = entityManager().createQuery("SELECT DISTINCT(o.disciplina) FROM CurriculoDisciplina o WHERE o.curriculo IN (SELECT c FROM Curriculo c WHERE c.curso IN (:cursos))")
			.setParameter("cursos", cursos);
		return q.getResultList();
	}
	
//	CampusDTO campusDTO, TurnoDTO turnoDTO, Integer periodo
	@SuppressWarnings("unchecked")
	public static List<Disciplina> findByCampusAndTurnoAndPeriodo(Campus campus, Turno turno, Integer periodo) {
		Query q = entityManager().createQuery("SELECT DISTINCT(o.disciplina) FROM CurriculoDisciplina o WHERE o.periodo = :periodo AND o.curriculo.turno = :turno AND o.curriculo.campus = :campus")
		.setParameter("campus", campus)
		.setParameter("turno", turno)
		.setParameter("periodo", periodo);
		return q.getResultList();
	}
	
	public static Disciplina findByCodigo(String codigo) {
		Query q = entityManager().createQuery("SELECT o FROM Disciplina o WHERE o.codigo = :codigo");
		q.setParameter("codigo", codigo);
		return (Disciplina) q.getSingleResult();
	}
	
	public static int count(String codigo, String nome, TipoDisciplina tipoDisciplina) {
		
		nome = (nome == null)? "" : nome;
		nome = "%" + nome.replace('*', '%') + "%";
		codigo = (codigo == null)? "" : codigo;
		codigo = "%" + codigo.replace('*', '%') + "%";
		
		String queryCampus = "";
		if(tipoDisciplina != null) {
			queryCampus = " o.tipoDisciplina = :tipoDisciplina AND ";
		}
		Query q = entityManager().createQuery("SELECT COUNT(o) FROM Disciplina o WHERE "+queryCampus+" LOWER(o.nome) LIKE LOWER(:nome) AND LOWER(o.codigo) LIKE LOWER(:codigo)");
		if(tipoDisciplina != null) {
			q.setParameter("tipoDisciplina", tipoDisciplina);
		}
		q.setParameter("nome", nome);
		q.setParameter("codigo", codigo);
		return ((Number)q.getSingleResult()).intValue();
	}
	
    @SuppressWarnings("unchecked")
	public static List<Disciplina> findBy(String codigo, String nome, TipoDisciplina tipoDisciplina, int firstResult, int maxResults, String orderBy) {

        nome = (nome == null)? "" : nome;
        nome = "%" + nome.replace('*', '%') + "%";
        codigo = (codigo == null)? "" : codigo;
        codigo = "%" + codigo.replace('*', '%') + "%";
        
        orderBy = (orderBy != null) ? " ORDER BY o." + orderBy : "";
        String queryCampus = "";
        if(tipoDisciplina != null) {
        	queryCampus = " o.tipoDisciplina = :tipoDisciplina AND ";
        }
        Query q = entityManager().createQuery("SELECT o FROM Disciplina o WHERE "+queryCampus+" LOWER(o.nome) LIKE LOWER(:nome) AND LOWER(o.codigo) LIKE LOWER(:codigo)");
        if(tipoDisciplina != null) {
        	q.setParameter("tipoDisciplina", tipoDisciplina);
        }
        q.setParameter("nome", nome);
        q.setParameter("codigo", codigo);
        return q.setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
	
    public Boolean isIncompativelCom(Disciplina disciplina) {
    	Query q = entityManager().createQuery("SELECT COUNT(o) FROM Incompatibilidade o WHERE o.disciplina1 = :disciplina1 AND o.disciplina2 = :disciplina2");
    	q.setParameter("disciplina1", this);
    	q.setParameter("disciplina2", disciplina);
    	return ((Number) q.getSingleResult()).intValue() > 0;
    }
    
	@SuppressWarnings("unchecked")
	public List<HorarioDisponivelCenario> getHorarios(SemanaLetiva semanaLetiva) {
		Query q = entityManager().createQuery("SELECT o FROM HorarioDisponivelCenario o, IN (o.disciplinas) c WHERE c = :disciplina AND o.horarioAula.semanaLetiva = :semanaLetiva");
		q.setParameter("disciplina", this);
		q.setParameter("semanaLetiva", semanaLetiva);
		return q.getResultList();
	}
    
	private static final long serialVersionUID = 7980821696468062987L;

	public Cenario getCenario() {
        return this.cenario;
    }

	public Integer getTotalCreditos() {
		return getCreditosPratico() + getCreditosTeorico();
	}
	
	public void setCenario(Cenario cenario) {
        this.cenario = cenario;
    }

	public TipoDisciplina getTipoDisciplina() {
        return this.tipoDisciplina;
    }

	public void setTipoDisciplina(TipoDisciplina tipoDisciplina) {
        this.tipoDisciplina = tipoDisciplina;
    }

	public DivisaoCredito getDivisaoCreditos() {
        return this.divisaoCreditos;
    }

	public void setDivisaoCreditos(DivisaoCredito divisaoCreditos) {
        this.divisaoCreditos = divisaoCreditos;
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

	public Integer getCreditosTeorico() {
        return this.creditosTeorico;
    }

	public void setCreditosTeorico(Integer creditosTeorico) {
        this.creditosTeorico = creditosTeorico;
    }

	public Integer getCreditosPratico() {
        return this.creditosPratico;
    }

	public void setCreditosPratico(Integer creditosPratico) {
        this.creditosPratico = creditosPratico;
    }

	public Boolean getLaboratorio() {
        return this.laboratorio;
    }

	public void setLaboratorio(Boolean laboratorio) {
        this.laboratorio = laboratorio;
    }

	public Dificuldades getDificuldade() {
        return this.dificuldade;
    }

	public void setDificuldade(Dificuldades dificuldade) {
        this.dificuldade = dificuldade;
    }

	public Integer getMaxAlunosTeorico() {
        return this.maxAlunosTeorico;
    }

	public void setMaxAlunosTeorico(Integer maxAlunosTeorico) {
        this.maxAlunosTeorico = maxAlunosTeorico;
    }

	public Integer getMaxAlunosPratico() {
        return this.maxAlunosPratico;
    }

	public void setMaxAlunosPratico(Integer maxAlunosPratico) {
        this.maxAlunosPratico = maxAlunosPratico;
    }

	public Set<HorarioDisponivelCenario> getHorarios() {
        return this.horarios;
    }

	public void setHorarios(Set<HorarioDisponivelCenario> horarios) {
        this.horarios = horarios;
    }

	public Set<ProfessorDisciplina> getProfessores() {
        return this.professores;
    }

	public void setProfessores(Set<ProfessorDisciplina> professores) {
        this.professores = professores;
    }

	public Set<Incompatibilidade> getIncompatibilidades() {
        return this.incompatibilidades;
    }

	public void setIncompatibilidades(Set<Incompatibilidade> incompatibilidades) {
        this.incompatibilidades = incompatibilidades;
    }

	public Set<Equivalencia> getEquivalencias() {
        return this.equivalencias;
    }

	public void setEquivalencias(Set<Equivalencia> equivalencias) {
        this.equivalencias = equivalencias;
    }
	
	public Set<Equivalencia> getEliminadaPor() {
		return this.eliminadaPor;
	}
	
	public void setEliminadaPor(Set<Equivalencia> eliminadaPor) {
		this.eliminadaPor = eliminadaPor;
	}

	public Set<CurriculoDisciplina> getCurriculos() {
        return this.curriculos;
    }

	public void setCurriculos(Set<CurriculoDisciplina> curriculos) {
        this.curriculos = curriculos;
    }

	public Set<Demanda> getDemandas() {
        return this.demandas;
    }

	public void setDemandas(Set<Demanda> demandas) {
        this.demandas = demandas;
    }
	
	public Set<Fixacao> getFixacoes() {
		return this.fixacoes;
	}
	
	public void setFixacoes(Set<Fixacao> fixacoes) {
		this.fixacoes = fixacoes;
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

	@Override
	public int compareTo(Disciplina o) {
		return getCodigo().compareTo(o.getCodigo());
	}
}
