//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, vhudson-jaxb-ri-2.1-2 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.01.12 at 01:06:23 PM BRST 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemPercentualMinimo complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemPercentualMinimo">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="percMinimo" type="{http://www.w3.org/2001/XMLSchema}double"/>
 *         &lt;element name="tipoTitulacaoId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemPercentualMinimo", propOrder = {
    "percMinimo",
    "tipoTitulacaoId"
})
public class ItemPercentualMinimo {

    protected double percMinimo;
    protected int tipoTitulacaoId;

    /**
     * Gets the value of the percMinimo property.
     * 
     */
    public double getPercMinimo() {
        return percMinimo;
    }

    /**
     * Sets the value of the percMinimo property.
     * 
     */
    public void setPercMinimo(double value) {
        this.percMinimo = value;
    }

    /**
     * Gets the value of the tipoTitulacaoId property.
     * 
     */
    public int getTipoTitulacaoId() {
        return tipoTitulacaoId;
    }

    /**
     * Sets the value of the tipoTitulacaoId property.
     * 
     */
    public void setTipoTitulacaoId(int value) {
        this.tipoTitulacaoId = value;
    }

}
