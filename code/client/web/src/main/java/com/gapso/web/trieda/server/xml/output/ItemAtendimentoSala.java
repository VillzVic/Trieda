//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, v2.2.2-hudson-jaxb-ri-2.2-63- 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.04.27 at 09:12:24 AM BRT 
//


package com.gapso.web.trieda.server.xml.output;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemAtendimentoSala complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemAtendimentoSala">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="salaId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="salaNome" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="atendimentosDiasSemana" type="{}GrupoAtendimentoDiaSemana"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemAtendimentoSala", propOrder = {
    "salaId",
    "salaNome",
    "atendimentosDiasSemana"
})
public class ItemAtendimentoSala {

    protected int salaId;
    @XmlElement(required = true)
    protected String salaNome;
    @XmlElement(required = true)
    protected GrupoAtendimentoDiaSemana atendimentosDiasSemana;

    /**
     * Gets the value of the salaId property.
     * 
     */
    public int getSalaId() {
        return salaId;
    }

    /**
     * Sets the value of the salaId property.
     * 
     */
    public void setSalaId(int value) {
        this.salaId = value;
    }

    /**
     * Gets the value of the salaNome property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getSalaNome() {
        return salaNome;
    }

    /**
     * Sets the value of the salaNome property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setSalaNome(String value) {
        this.salaNome = value;
    }

    /**
     * Gets the value of the atendimentosDiasSemana property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoAtendimentoDiaSemana }
     *     
     */
    public GrupoAtendimentoDiaSemana getAtendimentosDiasSemana() {
        return atendimentosDiasSemana;
    }

    /**
     * Sets the value of the atendimentosDiasSemana property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoAtendimentoDiaSemana }
     *     
     */
    public void setAtendimentosDiasSemana(GrupoAtendimentoDiaSemana value) {
        this.atendimentosDiasSemana = value;
    }

}
