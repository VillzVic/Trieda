//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, v2.2.2-hudson-jaxb-ri-2.2-63- 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.05.16 at 06:32:51 PM BRT 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemNivelDificuldadeHorario complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemNivelDificuldadeHorario">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="nivelDificuldadeId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="horariosAula" type="{}GrupoIdentificador"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemNivelDificuldadeHorario", propOrder = {
    "nivelDificuldadeId",
    "horariosAula"
})
public class ItemNivelDificuldadeHorario {

    protected int nivelDificuldadeId;
    @XmlElement(required = true)
    protected GrupoIdentificador horariosAula;

    /**
     * Gets the value of the nivelDificuldadeId property.
     * 
     */
    public int getNivelDificuldadeId() {
        return nivelDificuldadeId;
    }

    /**
     * Sets the value of the nivelDificuldadeId property.
     * 
     */
    public void setNivelDificuldadeId(int value) {
        this.nivelDificuldadeId = value;
    }

    /**
     * Gets the value of the horariosAula property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoIdentificador }
     *     
     */
    public GrupoIdentificador getHorariosAula() {
        return horariosAula;
    }

    /**
     * Sets the value of the horariosAula property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoIdentificador }
     *     
     */
    public void setHorariosAula(GrupoIdentificador value) {
        this.horariosAula = value;
    }

}
