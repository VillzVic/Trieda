//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, vhudson-jaxb-ri-2.1-2 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.03.10 at 10:21:31 AM BRT 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemAtendimentoOfertaSolucao complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemAtendimentoOfertaSolucao">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="ofertaCursoCampiId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="disciplinaId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="quantidade" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="turma" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemAtendimentoOfertaSolucao", propOrder = {
    "ofertaCursoCampiId",
    "disciplinaId",
    "quantidade",
    "turma"
})
public class ItemAtendimentoOfertaSolucao {

    protected int ofertaCursoCampiId;
    protected int disciplinaId;
    protected int quantidade;
    @XmlElement(required = true)
    protected String turma;

    /**
     * Gets the value of the ofertaCursoCampiId property.
     * 
     */
    public int getOfertaCursoCampiId() {
        return ofertaCursoCampiId;
    }

    /**
     * Sets the value of the ofertaCursoCampiId property.
     * 
     */
    public void setOfertaCursoCampiId(int value) {
        this.ofertaCursoCampiId = value;
    }

    /**
     * Gets the value of the disciplinaId property.
     * 
     */
    public int getDisciplinaId() {
        return disciplinaId;
    }

    /**
     * Sets the value of the disciplinaId property.
     * 
     */
    public void setDisciplinaId(int value) {
        this.disciplinaId = value;
    }

    /**
     * Gets the value of the quantidade property.
     * 
     */
    public int getQuantidade() {
        return quantidade;
    }

    /**
     * Sets the value of the quantidade property.
     * 
     */
    public void setQuantidade(int value) {
        this.quantidade = value;
    }

    /**
     * Gets the value of the turma property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getTurma() {
        return turma;
    }

    /**
     * Sets the value of the turma property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setTurma(String value) {
        this.turma = value;
    }

}