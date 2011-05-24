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
import javax.persistence.UniqueConstraint;
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
@RooEntity(identifierColumn = "UNI_ID")
@Table(
	name = "UNIDADES",
	uniqueConstraints=
		@UniqueConstraint(columnNames={"CAM_ID", "UNI_CODIGO"})
)
public class Unidade implements Serializable {

    @NotNull
    @ManyToOne(targetEntity = Campus.class)
    @JoinColumn(name = "CAM_ID")
    private Campus campus;

    @NotNull
    @Column(name = "UNI_CODIGO")
    @Size(min = 1, max = 20)
    private String codigo;

    @NotNull
    @Column(name = "UNI_NOME")
    @Size(min = 1, max = 50)
    private String nome;

    @OneToMany(cascade = CascadeType.ALL, mappedBy = "origem")
    private Set<DeslocamentoUnidade> deslocamentos = new HashSet<DeslocamentoUnidade>();

    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE}, mappedBy = "unidades")
    private Set<HorarioDisponivelCenario> horarios = new HashSet<HorarioDisponivelCenario>();

    @OneToMany(cascade = CascadeType.ALL, mappedBy="unidade")
    private Set<GrupoSala> gruposSalas = new HashSet<GrupoSala>();
    
    @OneToMany(cascade = CascadeType.ALL, mappedBy="unidade")
    private Set<Sala> salas = new HashSet<Sala>();
    
    @PersistenceContext
    transient EntityManager entityManager;

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "UNI_ID")
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
        preencheHorarios();
    }

    @Transactional
    public void remove() {
        if (this.entityManager == null) this.entityManager = entityManager();
        if (this.entityManager.contains(this)) {
        	this.removeHorariosDisponivelCenario();
        	this.removeSalas();
            this.entityManager.remove(this);
        } else {
            Unidade attached = this.entityManager.find(this.getClass(), this.id);
            attached.removeHorariosDisponivelCenario();
            attached.removeSalas();
            this.entityManager.remove(attached);
        }
    }

	private void preencheHorarios() {
		for(SemanaLetiva semanaLetiva : SemanaLetiva.findAll()) {
			for(HorarioDisponivelCenario hdc : this.getCampus().getHorarios(semanaLetiva)) {
				hdc.getUnidades().add(this);
				hdc.merge();
			}
		}
	}
    
    @Transactional
    private void removeHorariosDisponivelCenario() {
    	Set<HorarioDisponivelCenario> horarios = this.getHorarios();
    	for(HorarioDisponivelCenario horario : horarios) {
    		horario.getUnidades().remove(this);
    		horario.merge();
    	}
    }
    
	@Transactional
	private void removeSalas() {
		Set<Sala> salas = this.getSalas();
		for(Sala sala : salas) {
			sala.remove();
		}
	}
    
    @Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

    @Transactional
    public Unidade merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Unidade merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

    public static final EntityManager entityManager() {
        EntityManager em = new Unidade().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

    @SuppressWarnings("unchecked")
    public static List<Unidade> findAll() {
        return entityManager().createQuery("select o from Unidade o").getResultList();
    }
    
    @SuppressWarnings("unchecked")
    public static List<Unidade> findByCenario(Cenario cenario) {
    	Query q = entityManager().createQuery("SELECT o FROM Unidade o WHERE o.campus.cenario = :cenario");
    	q.setParameter("cenario", cenario);
    	return q.getResultList();
    }
    
    public static Map<String,Unidade> buildUnidadeCodigoToUnidadeMap(List<Unidade> unidades) {
		Map<String,Unidade> unidadesMap = new HashMap<String,Unidade>();
		for (Unidade unidade : unidades) {
			unidadesMap.put(unidade.getCodigo(),unidade);
		}
		return unidadesMap;
	}

    public static Unidade find(Long id) {
        if (id == null) return null;
        return entityManager().find(Unidade.class, id);
    }

    public static List<Unidade> find(int firstResult, int maxResults) {
        return find(firstResult, maxResults, null);
    }
    @SuppressWarnings("unchecked")
    public static List<Unidade> find(int firstResult, int maxResults, String orderBy) {
        orderBy = (orderBy != null) ? "ORDER BY o." + orderBy : "";
        return entityManager().createQuery("select o from Unidade o " + orderBy).setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
    
    public static Integer getCapacidadeMedia(Unidade unidade) {
    	Query q =  entityManager().createQuery("SELECT AVG(o.capacidade) FROM Sala o WHERE o.unidade = :unidade");
    	q.setParameter("unidade", unidade);
    	Object obj = q.getSingleResult();
    	if(obj == null) return 0;
    	return ((Number) obj).intValue();
    }
    
    @SuppressWarnings("unchecked")
    public static List<Unidade> findByCampus(Campus campus) {
    	Query q = entityManager().createQuery("SELECT Unidade FROM Unidade AS unidade WHERE unidade.campus = :campus ORDER BY unidade.nome ASC");
		q.setParameter("campus", campus);
    	return q.getResultList();
    }
    
    public static int count(Campus campus, String nome, String codigo) {

        nome = (nome == null)? "" : nome;
        nome = "%" + nome.replace('*', '%') + "%";
        codigo = (codigo == null)? "" : codigo;
        codigo = "%" + codigo.replace('*', '%') + "%";
        
        String queryCampus = "";
        if(campus != null) {
        	queryCampus = "unidade.campus = :campus AND";
        }
        Query q = entityManager().createQuery("SELECT COUNT(Unidade) FROM Unidade AS unidade WHERE "+queryCampus+" LOWER(unidade.nome) LIKE LOWER(:nome)  AND LOWER(unidade.codigo) LIKE LOWER(:codigo)");
        if(campus != null) {
        	q.setParameter("campus", campus);
        }
        q.setParameter("nome", nome);
        q.setParameter("codigo", codigo);
        return ((Number)q.getSingleResult()).intValue();
    }
    @SuppressWarnings("unchecked")
	public static List<Unidade> findBy(Campus campus, String nome, String codigo, int firstResult, int maxResults, String orderBy) {

        nome = (nome == null)? "" : nome;
        nome = "%" + nome.replace('*', '%') + "%";
        codigo = (codigo == null)? "" : codigo;
        codigo = "%" + codigo.replace('*', '%') + "%";
        
        orderBy = (orderBy != null) ? "ORDER BY o." + orderBy : "";
        String queryCampus = "";
        if(campus != null) {
        	queryCampus = "unidade.campus = :campus AND";
        }
        Query q = entityManager().createQuery("SELECT Unidade FROM Unidade AS unidade WHERE "+queryCampus+" LOWER(unidade.nome) LIKE LOWER(:nome)  AND LOWER(unidade.codigo) LIKE LOWER(:codigo)");
        if(campus != null) {
        	q.setParameter("campus", campus);
        }
        q.setParameter("nome", nome);
        q.setParameter("codigo", codigo);
        return q.setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
    
	@SuppressWarnings("unchecked")
	public List<HorarioDisponivelCenario> getHorarios(SemanaLetiva semanaLetiva) {
		Query q = entityManager().createQuery("SELECT o FROM HorarioDisponivelCenario o, IN (o.unidades) u WHERE u = :unidade AND o.horarioAula.semanaLetiva = :semanaLetiva");
		q.setParameter("unidade", this);
		q.setParameter("semanaLetiva", semanaLetiva);
		return q.getResultList();
	}
    
	public static boolean checkCodigoUnique(Cenario cenario, String codigo) {
		Query q = entityManager().createQuery("SELECT COUNT(o) FROM Unidade o WHERE o.campus.cenario = :cenario AND o.codigo = :codigo");
		q.setParameter("cenario", cenario);
		q.setParameter("codigo", codigo);
		Number size = (Number) q.setMaxResults(1).getSingleResult();
		return size.intValue() > 0;
	}
	
    private static final long serialVersionUID = -5763084706316974453L;

    public Campus getCampus() {
        return this.campus;
    }

    public void setCampus(Campus campus) {
        this.campus = campus;
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

    public Set<DeslocamentoUnidade> getDeslocamentos() {
        return this.deslocamentos;
    }

    public void setDeslocamentos(Set<DeslocamentoUnidade> deslocamentos) {
        this.deslocamentos = deslocamentos;
    }

    private Set<HorarioDisponivelCenario> getHorarios() {
        return this.horarios;
    }

    public void setHorarios(Set<HorarioDisponivelCenario> horarios) {
        this.horarios = horarios;
    }
    
    public Set<GrupoSala> getGruposSalas() {
    	return this.gruposSalas;
    }
    
    public void setGruposSalas(Set<GrupoSala> gruposSalas) {
    	this.gruposSalas = gruposSalas;
    }
    
    public Set<Sala> getSalas() {
    	return this.salas;
    }
    
    public void setSalas(Set<Sala> salas) {
    	this.salas = salas;
    }

    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Campus: ").append(getCampus()).append(", ");
        sb.append("Codigo: ").append(getCodigo()).append(", ");
        sb.append("Nome: ").append(getNome()).append(", ");
        sb.append("Deslocamentos: ").append(getDeslocamentos() == null ? "null" : getDeslocamentos().size()).append(", ");
        sb.append("Horarios: ").append(getHorarios() == null ? "null" : getHorarios().size());
        sb.append("GruposSalas: ").append(getGruposSalas() == null ? "null" : getGruposSalas().size());
        sb.append("Salas: ").append(getSalas() == null ? "null" : getSalas().size());
        return sb.toString();
    }
}
