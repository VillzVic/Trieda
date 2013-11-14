package com.gapso.web.trieda.shared.util.view;

import java.io.Serializable;

import com.gapso.web.trieda.shared.dtos.FaixaCapacidadeSalaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;

public class RelatorioSalaFiltro implements Serializable {

	private static final long serialVersionUID = -6535763124280249423L;
	
	private TurnoDTO turno;
	private FaixaCapacidadeSalaDTO faixaCapacidadeSala;

	public TurnoDTO getTurno() {
		return turno;
	}

	public void setTurno(TurnoDTO turno) {
		this.turno = turno;
	}

	public FaixaCapacidadeSalaDTO getFaixaCapacidadeSala() {
		return faixaCapacidadeSala;
	}

	public void setFaixaCapacidadeSala(FaixaCapacidadeSalaDTO faixaCapacidadeSala) {
		this.faixaCapacidadeSala = faixaCapacidadeSala;
	}
}
