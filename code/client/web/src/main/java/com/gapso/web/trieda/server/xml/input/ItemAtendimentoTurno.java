//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, vhudson-jaxb-ri-2.1-2 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2010.11.24 at 04:13:11 PM BRST 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemAtendimentoTurno complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemAtendimentoTurno">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="turnoId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="atendimentosHorariosAula" type="{}GrupoAtendimentoHorarioAula"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemAtendimentoTurno", propOrder = {
    "turnoId",
    "atendimentosHorariosAula"
})
public class ItemAtendimentoTurno {

    protected int turnoId;
    @XmlElement(required = true)
    protected GrupoAtendimentoHorarioAula atendimentosHorariosAula;

    /**
     * Gets the value of the turnoId property.
     * 
     */
    public int getTurnoId() {
        return turnoId;
    }

    /**
     * Sets the value of the turnoId property.
     * 
     */
    public void setTurnoId(int value) {
        this.turnoId = value;
    }

    /**
     * Gets the value of the atendimentosHorariosAula property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoAtendimentoHorarioAula }
     *     
     */
    public GrupoAtendimentoHorarioAula getAtendimentosHorariosAula() {
        return atendimentosHorariosAula;
    }

    /**
     * Sets the value of the atendimentosHorariosAula property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoAtendimentoHorarioAula }
     *     
     */
    public void setAtendimentosHorariosAula(GrupoAtendimentoHorarioAula value) {
        this.atendimentosHorariosAula = value;
    }

}
