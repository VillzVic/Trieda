//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, v2.2.4 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.09.23 at 05:21:16 PM BRT 
//


package com.gapso.web.trieda.server.xml.output;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemAtendimentoUnidade complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemAtendimentoUnidade">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="unidadeId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="unidadeCodigo" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="atendimentosSalas" type="{}GrupoAtendimentoSala"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemAtendimentoUnidade", propOrder = {
    "unidadeId",
    "unidadeCodigo",
    "atendimentosSalas"
})
public class ItemAtendimentoUnidade {

    protected int unidadeId;
    @XmlElement(required = true)
    protected String unidadeCodigo;
    @XmlElement(required = true)
    protected GrupoAtendimentoSala atendimentosSalas;

    /**
     * Gets the value of the unidadeId property.
     * 
     */
    public int getUnidadeId() {
        return unidadeId;
    }

    /**
     * Sets the value of the unidadeId property.
     * 
     */
    public void setUnidadeId(int value) {
        this.unidadeId = value;
    }

    /**
     * Gets the value of the unidadeCodigo property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getUnidadeCodigo() {
        return unidadeCodigo;
    }

    /**
     * Sets the value of the unidadeCodigo property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setUnidadeCodigo(String value) {
        this.unidadeCodigo = value;
    }

    /**
     * Gets the value of the atendimentosSalas property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoAtendimentoSala }
     *     
     */
    public GrupoAtendimentoSala getAtendimentosSalas() {
        return atendimentosSalas;
    }

    /**
     * Sets the value of the atendimentosSalas property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoAtendimentoSala }
     *     
     */
    public void setAtendimentosSalas(GrupoAtendimentoSala value) {
        this.atendimentosSalas = value;
    }

}
