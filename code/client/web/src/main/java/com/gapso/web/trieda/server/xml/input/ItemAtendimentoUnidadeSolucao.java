//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, v2.2.4 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.11.08 at 11:18:45 AM BRST 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemAtendimentoUnidadeSolucao complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemAtendimentoUnidadeSolucao">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="unidadeId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="unidadeCodigo" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="atendimentosSalas" type="{}GrupoAtendimentoSalaSolucao"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemAtendimentoUnidadeSolucao", propOrder = {
    "unidadeId",
    "unidadeCodigo",
    "atendimentosSalas"
})
public class ItemAtendimentoUnidadeSolucao {

    protected int unidadeId;
    @XmlElement(required = true)
    protected String unidadeCodigo;
    @XmlElement(required = true)
    protected GrupoAtendimentoSalaSolucao atendimentosSalas;

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
     *     {@link GrupoAtendimentoSalaSolucao }
     *     
     */
    public GrupoAtendimentoSalaSolucao getAtendimentosSalas() {
        return atendimentosSalas;
    }

    /**
     * Sets the value of the atendimentosSalas property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoAtendimentoSalaSolucao }
     *     
     */
    public void setAtendimentosSalas(GrupoAtendimentoSalaSolucao value) {
        this.atendimentosSalas = value;
    }

}
