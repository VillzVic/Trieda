//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, vhudson-jaxb-ri-2.1-2 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2010.12.09 at 09:57:16 AM BRST 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for anonymous complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType>
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="atendimentos" type="{}GrupoAtendimentoCampus"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "", propOrder = {
    "atendimentos"
})
@XmlRootElement(name = "TriedaOutput")
public class TriedaOutput {

    @XmlElement(required = true)
    protected GrupoAtendimentoCampus atendimentos;

    /**
     * Gets the value of the atendimentos property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoAtendimentoCampus }
     *     
     */
    public GrupoAtendimentoCampus getAtendimentos() {
        return atendimentos;
    }

    /**
     * Sets the value of the atendimentos property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoAtendimentoCampus }
     *     
     */
    public void setAtendimentos(GrupoAtendimentoCampus value) {
        this.atendimentos = value;
    }

}