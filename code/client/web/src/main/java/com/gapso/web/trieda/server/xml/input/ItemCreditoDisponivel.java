//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, v2.2.4 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.08.18 at 08:41:39 AM BRT 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemCreditoDisponivel complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemCreditoDisponivel">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="turnoId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="diaSemana" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="maxCreditos" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemCreditoDisponivel", propOrder = {
    "turnoId",
    "diaSemana",
    "maxCreditos"
})
public class ItemCreditoDisponivel {

    protected int turnoId;
    protected int diaSemana;
    protected int maxCreditos;

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
     * Gets the value of the diaSemana property.
     * 
     */
    public int getDiaSemana() {
        return diaSemana;
    }

    /**
     * Sets the value of the diaSemana property.
     * 
     */
    public void setDiaSemana(int value) {
        this.diaSemana = value;
    }

    /**
     * Gets the value of the maxCreditos property.
     * 
     */
    public int getMaxCreditos() {
        return maxCreditos;
    }

    /**
     * Sets the value of the maxCreditos property.
     * 
     */
    public void setMaxCreditos(int value) {
        this.maxCreditos = value;
    }

}
