package com.gapso.trieda.domain;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import javax.persistence.Column;
import javax.persistence.EntityManager;
import javax.persistence.MappedSuperclass;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import javax.validation.constraints.NotNull;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.format.annotation.DateTimeFormat;
import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.misc.Semanas;

@Configurable
@MappedSuperclass
public abstract class Disponibilidade
{

	@PersistenceContext
    transient EntityManager entityManager;
	
	abstract public Long getId();
	
	abstract public Long getEntidadeId();
	
	abstract public String getEntidadeTipo();
	
	@NotNull
    @Column( name = "HOR_INICIO" )
    @Temporal( TemporalType.TIMESTAMP )
    @DateTimeFormat( style = "--" )
    private Date horarioInicio;
    
    @NotNull
    @Column( name = "HOR_FIM" )
    @Temporal( TemporalType.TIMESTAMP )
    @DateTimeFormat( style = "--" )
    private Date horarioFim;
    
    @NotNull
    @Column( name = "SEGUNDA" )
    private Boolean segunda;
    
    @NotNull
    @Column( name = "TERCA" )
    private Boolean terca;
    
    @NotNull
    @Column( name = "QUARTA" )
    private Boolean quarta;
    
    @NotNull
    @Column( name = "QUINTA" )
    private Boolean quinta;
    
    @NotNull
    @Column( name = "SEXTA" )
    private Boolean sexta;
    
    @NotNull
    @Column( name = "SABADO" )
    private Boolean sabado;
    
    @NotNull
    @Column( name = "DOMINGO" )
    private Boolean domingo;

	public Date getHorarioInicio() {
		return horarioInicio;
	}

	public void setHorarioInicio(Date horarioInicio) {
		this.horarioInicio = horarioInicio;
	}

	public Date getHorarioFim() {
		return horarioFim;
	}

	public void setHorarioFim(Date horarioFim) {
		this.horarioFim = horarioFim;
	}

	public Boolean getSegunda() {
		return segunda;
	}

	public void setSegunda(Boolean segunda) {
		this.segunda = segunda;
	}

	public Boolean getTerca() {
		return terca;
	}

	public void setTerca(Boolean terca) {
		this.terca = terca;
	}

	public Boolean getQuarta() {
		return quarta;
	}

	public void setQuarta(Boolean quarta) {
		this.quarta = quarta;
	}

	public Boolean getQuinta() {
		return quinta;
	}

	public void setQuinta(Boolean quinta) {
		this.quinta = quinta;
	}

	public Boolean getSexta() {
		return sexta;
	}

	public void setSexta(Boolean sexta) {
		this.sexta = sexta;
	}

	public Boolean getSabado() {
		return sabado;
	}

	public void setSabado(Boolean sabado) {
		this.sabado = sabado;
	}

	public Boolean getDomingo() {
		return domingo;
	}

	public void setDomingo(Boolean domingo) {
		this.domingo = domingo;
	}
	
	public static final EntityManager entityManager()
	{
        EntityManager em = new DisponibilidadeDisciplina().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		"Entity manager has not been injected (is the Spring " +
        		"Aspects JAR configured as an AJC/AJDT aspects library?)" );
        }

        return em;
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
            this.entityManager.remove( this );
        }
        else
        {
            Disponibilidade attached
            	= this.entityManager.find( this.getClass(),getId() );

            this.entityManager.remove( attached );
        }
    }
	
	public abstract Disponibilidade merge();
	
	public List<Semanas> getDiasSemana()
	{
		List<Semanas> diasSemana = new ArrayList<Semanas>();
		
		if (getSegunda())
			diasSemana.add(Semanas.SEG);
		if (getTerca())
			diasSemana.add(Semanas.TER);
		if (getQuarta())
			diasSemana.add(Semanas.QUA);
		if (getQuinta())
			diasSemana.add(Semanas.QUI);
		if (getSexta())
			diasSemana.add(Semanas.SEX);
		if (getSabado())
			diasSemana.add(Semanas.SAB);
		if (getDomingo())
			diasSemana.add(Semanas.DOM);
		
		return diasSemana;
	}
	
    @SuppressWarnings("unchecked")
	public static List<Disponibilidade> findBy(
    	Cenario cenario, Long entidadeId, String entidadeTipo )
    {
		Query q = entityManager().createQuery(
			" SELECT o FROM " + entidadeTipo + " o " +
			" WHERE o." + entidadeTipo  + ".id = :entidadeId " +
			" ORDER BY o.horarioInicio, o.horarioFim" );
		
		q.setParameter( "entidadeId", entidadeId );

		return q.getResultList();
	}
}
