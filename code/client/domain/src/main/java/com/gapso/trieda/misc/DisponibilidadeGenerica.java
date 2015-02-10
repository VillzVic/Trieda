package com.gapso.trieda.misc;

import java.util.Calendar;
import java.util.Date;
import java.util.List;

import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.TriedaPar;

public class DisponibilidadeGenerica {
    private Date horarioInicio;
    private Date horarioFim;
    private Boolean segunda;
    private Boolean terca;
    private Boolean quarta;
    private Boolean quinta;
    private Boolean sexta;
    private Boolean sabado;
    private Boolean domingo;
    
    public DisponibilidadeGenerica(Semanas diaSem, TriedaPar<Date, Date> intervalo) {
    	this.horarioInicio = intervalo.getPrimeiro();
    	this.horarioFim = intervalo.getSegundo();
    	this.segunda = false;
    	this.terca = false;
    	this.quarta = false;
    	this.quinta = false;
    	this.sexta = false;
    	this.sabado = false;
    	this.domingo = false;
    	switch (diaSem) {
			case SEG: this.segunda = true; break;
			case TER: this.terca = true; break;
			case QUA: this.quarta = true; break;
			case QUI: this.quinta = true; break;
			case SEX: this.sexta = true; break;
			case SAB: this.sabado = true; break;
			case DOM: this.domingo = true; break;
		}
    }

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
	
	public static boolean ehCompativelCom(HorarioDisponivelCenario hdc, List<DisponibilidadeGenerica> disponibilidades) {
		for (DisponibilidadeGenerica disponibilidade : disponibilidades) {
			if (disponibilidade.ehCompativelCom(hdc)) {
				return true;
			}
		}
		return false;
	}
}