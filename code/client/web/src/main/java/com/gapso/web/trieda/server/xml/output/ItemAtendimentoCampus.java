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
 * <p>Java class for ItemAtendimentoCampus complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemAtendimentoCampus">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="campusId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="campusCodigo" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="atendimentosUnidades" type="{}GrupoAtendimentoUnidade"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemAtendimentoCampus", propOrder = {
    "campusId",
    "campusCodigo",
    "atendimentosUnidades"
})
public class ItemAtendimentoCampus {

    protected int campusId;
    @XmlElement(required = true)
    protected String campusCodigo;
    @XmlElement(required = true)
    protected GrupoAtendimentoUnidade atendimentosUnidades;

    /**
     * Gets the value of the campusId property.
     * 
     */
    public int getCampusId() {
        return campusId;
    }

    /**
     * Sets the value of the campusId property.
     * 
     */
    public void setCampusId(int value) {
        this.campusId = value;
    }

    /**
     * Gets the value of the campusCodigo property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getCampusCodigo() {
        return campusCodigo;
    }

    /**
     * Sets the value of the campusCodigo property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setCampusCodigo(String value) {
        this.campusCodigo = value;
    }

    /**
     * Gets the value of the atendimentosUnidades property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoAtendimentoUnidade }
     *     
     */
    public GrupoAtendimentoUnidade getAtendimentosUnidades() {
        return atendimentosUnidades;
    }

    /**
     * Sets the value of the atendimentosUnidades property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoAtendimentoUnidade }
     *     
     */
    public void setAtendimentosUnidades(GrupoAtendimentoUnidade value) {
        this.atendimentosUnidades = value;
    }

}
