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
 * <p>Java class for ItemCurriculo complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemCurriculo">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="id" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="codigo" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="disciplinasPeriodo" type="{}GrupoDisciplinaPeriodo"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemCurriculo", propOrder = {
    "id",
    "codigo",
    "disciplinasPeriodo"
})
public class ItemCurriculo {

    protected int id;
    @XmlElement(required = true)
    protected String codigo;
    @XmlElement(required = true)
    protected GrupoDisciplinaPeriodo disciplinasPeriodo;

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
     * Gets the value of the codigo property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getCodigo() {
        return codigo;
    }

    /**
     * Sets the value of the codigo property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setCodigo(String value) {
        this.codigo = value;
    }

    /**
     * Gets the value of the disciplinasPeriodo property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoDisciplinaPeriodo }
     *     
     */
    public GrupoDisciplinaPeriodo getDisciplinasPeriodo() {
        return disciplinasPeriodo;
    }

    /**
     * Sets the value of the disciplinasPeriodo property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoDisciplinaPeriodo }
     *     
     */
    public void setDisciplinasPeriodo(GrupoDisciplinaPeriodo value) {
        this.disciplinasPeriodo = value;
    }

}
