package com.gapso.web.trieda.server.xml.input;

import java.util.List;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlType;


@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemHorarioFixacao", propOrder = {
    "semanaLetivaId",
    "turnoId",
    "horariosId",
    "diaSemana"
})
public class ItemHorarioFixacao
{
	 protected int semanaLetivaId;
	 protected int turnoId;
	 protected int horariosId;
	 protected List<Integer> diaSemana;
	/**
	 * @return the semanaLetivaId
	 */
	public int getSemanaLetivaId()
	{
		return semanaLetivaId;
	}
	/**
	 * @param semanaLetivaId the semanaLetivaId to set
	 */
	public void setSemanaLetivaId(int semanaLetivaId)
	{
		this.semanaLetivaId = semanaLetivaId;
	}
	/**
	 * @return the turnoId
	 */
	public int getTurnoId()
	{
		return turnoId;
	}
	/**
	 * @param turnoId the turnoId to set
	 */
	public void setTurnoId(int turnoId)
	{
		this.turnoId = turnoId;
	}
	/**
	 * @return the horariosId
	 */
	public int getHorariosId()
	{
		return horariosId;
	}
	/**
	 * @param horariosId the horariosId to set
	 */
	public void setHorariosId(int horariosId)
	{
		this.horariosId = horariosId;
	}
	/**
	 * @return the diaSemana
	 */
	public List<Integer> getDiaSemana()
	{
		return diaSemana;
	}
	/**
	 * @param diaSemana the diaSemana to set
	 */
	public void setDiaSemana(List<Integer> diaSemana)
	{
		this.diaSemana = diaSemana;
	}

	 
}
