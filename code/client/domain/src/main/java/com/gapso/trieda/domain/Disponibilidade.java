package com.gapso.trieda.domain;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

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

import com.gapso.trieda.misc.DisponibilidadeGenerica;
import com.gapso.trieda.misc.Semanas;
import com.gapso.trieda.misc.TriedaDomainUtil;

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
	
	public boolean temAlgumaDisponibilidade() {
		return this.segunda || this.terca || this.quarta || this.quinta || this.sexta || this.sabado || this.domingo;
	}
	
	public boolean ehCompativelCom(HorarioDisponivelCenario hdc) {
		// Checa compatibilidade de dia da semana
		boolean diaSemCompativel = false;
		switch (hdc.getDiaSemana()) {
			case SEG: diaSemCompativel = this.getSegunda(); break;
			case TER: diaSemCompativel = this.getTerca(); break;
			case QUA: diaSemCompativel = this.getQuarta(); break;
			case QUI: diaSemCompativel = this.getQuinta(); break;
			case SEX: diaSemCompativel = this.getSexta(); break;
			case SAB: diaSemCompativel = this.getSabado(); break;
			case DOM: diaSemCompativel = this.getDomingo(); break;
		}
		
		if (diaSemCompativel) {
			// Checa compatibilidade de horário
			Calendar horaInicio = Calendar.getInstance();
			horaInicio.setTime(this.getHorarioInicio());
			horaInicio.set(1979,Calendar.NOVEMBER,6);
			
			Calendar horaFim = Calendar.getInstance();
			horaFim.setTime(this.getHorarioFim());
			horaFim.set(1979,Calendar.NOVEMBER,6);
			
			Calendar oHoraInicio = Calendar.getInstance();
			oHoraInicio.setTime(hdc.getHorarioAula().getHorario());
			oHoraInicio.set(1979,Calendar.NOVEMBER,6);
			
			Calendar oHoraFim = Calendar.getInstance();
			oHoraFim.setTime(hdc.getHorarioAula().getHorario());
			oHoraFim.set(1979,Calendar.NOVEMBER,6);
			oHoraFim.add(Calendar.MINUTE,hdc.getHorarioAula().getSemanaLetiva().getTempo());
	
			return (horaInicio.compareTo(oHoraInicio) <= 0 && horaFim.compareTo(oHoraFim) >= 0 );
		}
		return false;
	}
	
	public static boolean ehCompativelCom(HorarioDisponivelCenario hdc, List<Disponibilidade> disponibilidades) {
		for (Disponibilidade disponibilidade : disponibilidades) {
			if (disponibilidade.ehCompativelCom(hdc)) {
				return true;
			}
		}
		return false;
	}
	
	protected void clone(Disponibilidade clone) {
		clone.setHorarioInicio(this.getHorarioInicio());
		clone.setHorarioFim(this.getHorarioFim());
		clone.setSegunda(this.getSegunda());
		clone.setTerca(this.getTerca());
		clone.setQuarta(this.getQuarta());
		clone.setQuinta(this.getQuinta());
		clone.setSexta(this.getSexta());
		clone.setSabado(this.getSabado());
		clone.setDomingo(this.getDomingo());
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
    
    public static boolean temAlgumaDisponibilidade(List<Disponibilidade> disponibilidades) {
    	if (disponibilidades != null) {
    		for (Disponibilidade disp : disponibilidades) {
    			if (disp.temAlgumaDisponibilidade()) {
    				return true;
    			}
    		}
    	}
    	return false;
    }
    
    public static List<DisponibilidadeGenerica> geraDisponibilidadesCompactadasPorDiaSemana(List<Disponibilidade> disponibilidadesOriginais) {
    	List<DisponibilidadeGenerica> disponibilidadesCompactadas = new ArrayList<DisponibilidadeGenerica>();
    	
    	Map<Semanas, List<TriedaPar<Date, Date>>> diaSemToHiHfsMap = new HashMap<Semanas, List<TriedaPar<Date, Date>>>();
    	for (Disponibilidade dispOrig : disponibilidadesOriginais) {
    		for (Semanas diaSem : dispOrig.getDiasSemana()) {
    			List<TriedaPar<Date, Date>> hiHfsDoDiaSem = diaSemToHiHfsMap.get(diaSem);
    			if (hiHfsDoDiaSem == null) {
    				hiHfsDoDiaSem = new ArrayList<TriedaPar<Date,Date>>();
    				diaSemToHiHfsMap.put(diaSem, hiHfsDoDiaSem);
    			}
    			hiHfsDoDiaSem.add(TriedaPar.create(dispOrig.getHorarioInicio(), dispOrig.getHorarioFim()));
    		}
    	}
    	
    	for (Entry<Semanas, List<TriedaPar<Date, Date>>> entry : diaSemToHiHfsMap.entrySet()) {
    		Semanas diaSem = entry.getKey();
    		List<TriedaPar<Date, Date>> novosIntervalos = new ArrayList<TriedaPar<Date, Date>>();
    		
    		// ordena as disponibilidades do dia da semana
    		List<TriedaPar<Date, Date>> intervalosOrdenados = new ArrayList<TriedaPar<Date, Date>>(entry.getValue());
    		Collections.sort(intervalosOrdenados, new Comparator<TriedaPar<Date, Date>>() {
				public int compare(TriedaPar<Date, Date> o1, TriedaPar<Date, Date> o2) {
					if (o1 == null)
	                    return -1;
	                if (o2 == null)
	                    return 1;
	                
	                Calendar o1HI = TriedaDomainUtil.dateToCalendar(o1.getPrimeiro());
	                Calendar o1HF = TriedaDomainUtil.dateToCalendar(o1.getSegundo());
	                
	                Calendar o2HI = TriedaDomainUtil.dateToCalendar(o2.getPrimeiro());
	                Calendar o2HF = TriedaDomainUtil.dateToCalendar(o2.getSegundo());

	                int res = o1HI.compareTo(o2HI);
	                if (res == 0)
	                {
	                    res = o1HF.compareTo(o2HF);
	                }
	                return res;
				}
    		});
    		
    		// compacta as disponibilidades do dia da semana
    		for (TriedaPar<Date, Date> intervalo : intervalosOrdenados) {
                if (novosIntervalos.isEmpty()) {
                    novosIntervalos.add(intervalo);
                } else {
                	TriedaPar<Date, Date> ultIntervalo = novosIntervalos.get(novosIntervalos.size()-1);
                    if (TriedaDomainUtil.estahContido(ultIntervalo, intervalo)) {
                        novosIntervalos.remove(ultIntervalo);
                        novosIntervalos.add(intervalo);
                    } else if (TriedaDomainUtil.estahContido(intervalo, ultIntervalo)) {
                        // não faz nada
                    } else {
                    	Calendar hf_ultInterv = TriedaDomainUtil.dateToCalendar(ultIntervalo.getSegundo());
                        Calendar hi_Interv = TriedaDomainUtil.dateToCalendar(intervalo.getPrimeiro());
                        if (hf_ultInterv.compareTo(hi_Interv) < 0) {//(hf_ultInterv < hi_Interv)
                            novosIntervalos.add(intervalo);
                        } else {
                            novosIntervalos.remove(ultIntervalo);
                            novosIntervalos.add(TriedaPar.create(ultIntervalo.getPrimeiro(), intervalo.getSegundo()));
                        }
                    }
                }
            }
    		
    		// coletas as novas disponibilidades
    		for (TriedaPar<Date, Date> intervalo : novosIntervalos) {
    			disponibilidadesCompactadas.add(new DisponibilidadeGenerica(diaSem, intervalo));
    		}
    	}
    	
    	return disponibilidadesCompactadas;
    }
}
