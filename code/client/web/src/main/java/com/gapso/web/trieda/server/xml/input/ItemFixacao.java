//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, v2.2.2-hudson-jaxb-ri-2.2-63- 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.05.16 at 06:32:51 PM BRT 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemFixacao complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemFixacao">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="id" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="professorId" type="{http://www.w3.org/2001/XMLSchema}int" minOccurs="0"/>
 *         &lt;element name="disciplinaId" type="{http://www.w3.org/2001/XMLSchema}int" minOccurs="0"/>
 *         &lt;element name="salaId" type="{http://www.w3.org/2001/XMLSchema}int" minOccurs="0"/>
 *         &lt;element name="diaSemana" type="{http://www.w3.org/2001/XMLSchema}int" minOccurs="0"/>
 *         &lt;element name="turnoId" type="{http://www.w3.org/2001/XMLSchema}int" minOccurs="0"/>
 *         &lt;element name="horarioAulaId" type="{http://www.w3.org/2001/XMLSchema}int" minOccurs="0"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemFixacao", propOrder = {
    "id",
    "professorId",
    "disciplinaId",
    "salaId",
    "diaSemana",
    "turnoId",
    "horarioAulaId"
})
public class ItemFixacao {

    protected int id;
    protected Integer professorId;
    protected Integer disciplinaId;
    protected Integer salaId;
    protected Integer diaSemana;
    protected Integer turnoId;
    protected Integer horarioAulaId;

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
     * Gets the value of the professorId property.
     * 
     * @return
     *     possible object is
     *     {@link Integer }
     *     
     */
    public Integer getProfessorId() {
        return professorId;
    }

    /**
     * Sets the value of the professorId property.
     * 
     * @param value
     *     allowed object is
     *     {@link Integer }
     *     
     */
    public void setProfessorId(Integer value) {
        this.professorId = value;
    }

    /**
     * Gets the value of the disciplinaId property.
     * 
     * @return
     *     possible object is
     *     {@link Integer }
     *     
     */
    public Integer getDisciplinaId() {
        return disciplinaId;
    }

    /**
     * Sets the value of the disciplinaId property.
     * 
     * @param value
     *     allowed object is
     *     {@link Integer }
     *     
     */
    public void setDisciplinaId(Integer value) {
        this.disciplinaId = value;
    }

    /**
     * Gets the value of the salaId property.
     * 
     * @return
     *     possible object is
     *     {@link Integer }
     *     
     */
    public Integer getSalaId() {
        return salaId;
    }

    /**
     * Sets the value of the salaId property.
     * 
     * @param value
     *     allowed object is
     *     {@link Integer }
     *     
     */
    public void setSalaId(Integer value) {
        this.salaId = value;
    }

    /**
     * Gets the value of the diaSemana property.
     * 
     * @return
     *     possible object is
     *     {@link Integer }
     *     
     */
    public Integer getDiaSemana() {
        return diaSemana;
    }

    /**
     * Sets the value of the diaSemana property.
     * 
     * @param value
     *     allowed object is
     *     {@link Integer }
     *     
     */
    public void setDiaSemana(Integer value) {
        this.diaSemana = value;
    }

    /**
     * Gets the value of the turnoId property.
     * 
     * @return
     *     possible object is
     *     {@link Integer }
     *     
     */
    public Integer getTurnoId() {
        return turnoId;
    }

    /**
     * Sets the value of the turnoId property.
     * 
     * @param value
     *     allowed object is
     *     {@link Integer }
     *     
     */
    public void setTurnoId(Integer value) {
        this.turnoId = value;
    }

    /**
     * Gets the value of the horarioAulaId property.
     * 
     * @return
     *     possible object is
     *     {@link Integer }
     *     
     */
    public Integer getHorarioAulaId() {
        return horarioAulaId;
    }

    /**
     * Sets the value of the horarioAulaId property.
     * 
     * @param value
     *     allowed object is
     *     {@link Integer }
     *     
     */
    public void setHorarioAulaId(Integer value) {
        this.horarioAulaId = value;
    }

}
