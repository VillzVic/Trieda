package com.gapso.web.trieda.server.xml.input;

import java.util.List;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlType;


@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemHorarioFixacao", propOrder = {
    "diaSemanaId",
    "horariosId",
    "teorica",
    "salaId",
    "professorId"
})
public class ItemHorarioFixacao
{
	 protected List<Integer> diaSemanaId;
	 protected int horariosId;
	 protected boolean teorica;
	 protected GrupoIdentificador salaId;
	 protected GrupoIdentificador professorId;
	 
	 
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
		return diaSemanaId;
	}
	/**
	 * @param diaSemana the diaSemana to set
	 */
	public void setDiaSemana(List<Integer> diaSemana)
	{
		this.diaSemanaId = diaSemana;
	}
	public boolean isTeorica()
	{
		return teorica;
	}
	public void setTeorica(boolean teorica)
	{
		this.teorica = teorica;
	}
	public GrupoIdentificador getSala()
	{
		return salaId;
	}
	public void setSala(GrupoIdentificador sala)
	{
		this.salaId = sala;
	}
	public GrupoIdentificador getPorfessor()
	{
		return professorId;
	}
	public void setPorfessor(GrupoIdentificador porfessor)
	{
		this.professorId = porfessor;
	}

	 
}
