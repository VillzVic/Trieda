//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, vhudson-jaxb-ri-2.1-2 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2010.12.02 at 05:08:00 PM BRST 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemAtendimentoTatico complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemAtendimentoTatico">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="atendimentoOferta" type="{}ItemAtendimentoOferta"/>
 *         &lt;element name="qtdeCreditosTeoricos" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="qtdeCreditosPraticos" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemAtendimentoTatico", propOrder = {
    "atendimentoOferta",
    "qtdeCreditosTeoricos",
    "qtdeCreditosPraticos"
})
public class ItemAtendimentoTatico {

    @XmlElement(required = true)
    protected ItemAtendimentoOferta atendimentoOferta;
    protected int qtdeCreditosTeoricos;
    protected int qtdeCreditosPraticos;

    /**
     * Gets the value of the atendimentoOferta property.
     * 
     * @return
     *     possible object is
     *     {@link ItemAtendimentoOferta }
     *     
     */
    public ItemAtendimentoOferta getAtendimentoOferta() {
        return atendimentoOferta;
    }

    /**
     * Sets the value of the atendimentoOferta property.
     * 
     * @param value
     *     allowed object is
     *     {@link ItemAtendimentoOferta }
     *     
     */
    public void setAtendimentoOferta(ItemAtendimentoOferta value) {
        this.atendimentoOferta = value;
    }

    /**
     * Gets the value of the qtdeCreditosTeoricos property.
     * 
     */
    public int getQtdeCreditosTeoricos() {
        return qtdeCreditosTeoricos;
    }

    /**
     * Sets the value of the qtdeCreditosTeoricos property.
     * 
     */
    public void setQtdeCreditosTeoricos(int value) {
        this.qtdeCreditosTeoricos = value;
    }

    /**
     * Gets the value of the qtdeCreditosPraticos property.
     * 
     */
    public int getQtdeCreditosPraticos() {
        return qtdeCreditosPraticos;
    }

    /**
     * Sets the value of the qtdeCreditosPraticos property.
     * 
     */
    public void setQtdeCreditosPraticos(int value) {
        this.qtdeCreditosPraticos = value;
    }

}
