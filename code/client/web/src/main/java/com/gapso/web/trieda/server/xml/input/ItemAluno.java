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
 * <p>Java class for ItemAluno complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemAluno">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="alunoId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="nomeAluno" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="formando" type="{http://www.w3.org/2001/XMLSchema}boolean"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemAluno", propOrder = {
    "alunoId",
    "nomeAluno",
    "formando"
})
public class ItemAluno {

    protected int alunoId;
    @XmlElement(required = true)
    protected String nomeAluno;
    protected boolean formando;

    /**
     * Gets the value of the alunoId property.
     * 
     */
    public int getAlunoId() {
        return alunoId;
    }

    /**
     * Sets the value of the alunoId property.
     * 
     */
    public void setAlunoId(int value) {
        this.alunoId = value;
    }

    /**
     * Gets the value of the nomeAluno property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getNomeAluno() {
        return nomeAluno;
    }

    /**
     * Sets the value of the nomeAluno property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setNomeAluno(String value) {
        this.nomeAluno = value;
    }
    
    /**
     * Gets the value of the formando property.
     * 
     */
    public boolean isFormando() {
        return formando;
    }

    /**
     * Sets the value of the Formando property.
     * 
     */
    public void setFormando(boolean value) {
        this.formando = value;
    }
}