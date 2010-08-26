package com.gapso.trieda.domain;

import java.util.Date;
import java.util.List;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import javax.persistence.Version;
import javax.validation.constraints.NotNull;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.format.annotation.DateTimeFormat;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "HOR_ID")
@Table(name = "HORARIOS_AULA")
public class HorarioAula implements java.io.Serializable {

    @NotNull
    @ManyToOne(targetEntity = Calendario.class)
    @JoinColumn(name = "CAL_ID")
    private Calendario calendario;

    @NotNull
    @ManyToOne(targetEntity = Turno.class)
    @JoinColumn(name = "TUR_ID")
    private Turno turno;

    @NotNull
    @Column(name = "HOR_HORARIO")
    @Temporal(TemporalType.TIMESTAMP)
    @DateTimeFormat(style = "--")
    private Date horario;

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Calendario: ").append(getCalendario()).append(", ");
        sb.append("Turno: ").append(getTurno()).append(", ");
        sb.append("Horario: ").append(getHorario());
        return sb.toString();
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "HOR_ID")
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
            HorarioAula attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public HorarioAula merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        HorarioAula merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new HorarioAula().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countHorarioAulas() {
        return ((Number) entityManager().createQuery("select count(o) from HorarioAula o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<HorarioAula> findAllHorarioAulas() {
        return entityManager().createQuery("select o from HorarioAula o").getResultList();
    }

	public static HorarioAula findHorarioAula(Long id) {
        if (id == null) return null;
        return entityManager().find(HorarioAula.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<HorarioAula> findHorarioAulaEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from HorarioAula o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	public Calendario getCalendario() {
        return this.calendario;
    }

	public void setCalendario(Calendario calendario) {
        this.calendario = calendario;
    }

	public Turno getTurno() {
        return this.turno;
    }

	public void setTurno(Turno turno) {
        this.turno = turno;
    }

	public Date getHorario() {
        return this.horario;
    }

	public void setHorario(Date horario) {
        this.horario = horario;
    }

	private static final long serialVersionUID = -2085494379463432562L;
}
