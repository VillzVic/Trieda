//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, vhudson-jaxb-ri-2.1-2 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.01.05 at 10:11:59 AM BRST 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemHorario complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemHorario">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="turnoId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="horarioAulaId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="diasSemana" type="{}GrupoDiaSemana"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemHorario", propOrder = {
    "turnoId",
    "horarioAulaId",
    "diasSemana"
})
public class ItemHorario {

    protected int turnoId;
    protected int horarioAulaId;
    @XmlElement(required = true)
    protected GrupoDiaSemana diasSemana;

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
     * Gets the value of the horarioAulaId property.
     * 
     */
    public int getHorarioAulaId() {
        return horarioAulaId;
    }

    /**
     * Sets the value of the horarioAulaId property.
     * 
     */
    public void setHorarioAulaId(int value) {
        this.horarioAulaId = value;
    }

    /**
     * Gets the value of the diasSemana property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoDiaSemana }
     *     
     */
    public GrupoDiaSemana getDiasSemana() {
        return diasSemana;
    }

    /**
     * Sets the value of the diasSemana property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoDiaSemana }
     *     
     */
    public void setDiasSemana(GrupoDiaSemana value) {
        this.diasSemana = value;
    }

}
