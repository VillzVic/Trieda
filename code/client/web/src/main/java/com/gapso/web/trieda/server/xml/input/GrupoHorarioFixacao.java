package com.gapso.web.trieda.server.xml.input;

import java.util.ArrayList;
import java.util.List;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "GrupoHorarioFixacao", propOrder = {
    "horario"
})

public class GrupoHorarioFixacao
{
	 @XmlElement(name = "Horario")
	    protected List<ItemHorarioFixacao> horario;
	 
	 
	 public List<ItemHorarioFixacao> getHorarioFixacao() {
	        if (horario == null) {
	            horario = new ArrayList<ItemHorarioFixacao>();
	        }
	        return this.horario;
	    }

}
