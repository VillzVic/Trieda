//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, vhudson-jaxb-ri-2.1-2 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2010.12.02 at 05:08:00 PM BRST 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemOperacional complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemOperacional">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="horarioAulaId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="professorId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemOperacional", propOrder = {
    "horarioAulaId",
    "professorId"
})
public class ItemOperacional {

    protected int horarioAulaId;
    protected int professorId;

    /**
     * Gets the value of the horarioAulaId property.
     * 
     */
    public int getHorarioAulaId() {
        return horarioAulaId;
    }

    /**
     * Sets the value of the horarioAulaId property.
     * 
     */
    public void setHorarioAulaId(int value) {
        this.horarioAulaId = value;
    }

    /**
     * Gets the value of the professorId property.
     * 
     */
    public int getProfessorId() {
        return professorId;
    }

    /**
     * Sets the value of the professorId property.
     * 
     */
    public void setProfessorId(int value) {
        this.professorId = value;
    }

}
