//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, v2.2.4 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.09.23 at 05:13:14 PM BRT 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemAlunoDemanda complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemAlunoDemanda">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="id" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="alunoId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="nomeAluno" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="demandaId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemAlunoDemanda", propOrder = {
    "id",
    "alunoId",
    "nomeAluno",
    "demandaId"
})
public class ItemAlunoDemanda {

    protected int id;
    protected int alunoId;
    @XmlElement(required = true)
    protected String nomeAluno;
    protected int demandaId;

    /**
     * Gets the value of the id property.
     * 
     */
    public int getId() {
        return id;
    }

    /**
     * Sets the value of the id property.
     * 
     */
    public void setId(int value) {
        this.id = value;
    }

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
     * Gets the value of the demandaId property.
     * 
     */
    public int getDemandaId() {
        return demandaId;
    }

    /**
     * Sets the value of the demandaId property.
     * 
     */
    public void setDemandaId(int value) {
        this.demandaId = value;
    }

}
