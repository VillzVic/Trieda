//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, v2.2.4 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.08.18 at 08:41:39 AM BRT 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemProfessorDisciplina complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemProfessorDisciplina">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="nota" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="preferencia" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="disciplinaId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemProfessorDisciplina", propOrder = {
    "nota",
    "preferencia",
    "disciplinaId"
})
public class ItemProfessorDisciplina {

    protected int nota;
    protected int preferencia;
    protected int disciplinaId;

    /**
     * Gets the value of the nota property.
     * 
     */
    public int getNota() {
        return nota;
    }

    /**
     * Sets the value of the nota property.
     * 
     */
    public void setNota(int value) {
        this.nota = value;
    }

    /**
     * Gets the value of the preferencia property.
     * 
     */
    public int getPreferencia() {
        return preferencia;
    }

    /**
     * Sets the value of the preferencia property.
     * 
     */
    public void setPreferencia(int value) {
        this.preferencia = value;
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

}
