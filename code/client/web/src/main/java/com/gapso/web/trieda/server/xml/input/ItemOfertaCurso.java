//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, v2.2.4 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.11.08 at 11:18:45 AM BRST 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemOfertaCurso complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemOfertaCurso">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="id" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="curriculoId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="cursoId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="turnoId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="campusId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="receita" type="{http://www.w3.org/2001/XMLSchema}double"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemOfertaCurso", propOrder = {
    "id",
    "curriculoId",
    "cursoId",
    "turnoId",
    "campusId",
    "receita"
})
public class ItemOfertaCurso {

    protected int id;
    protected int curriculoId;
    protected int cursoId;
    protected int turnoId;
    protected int campusId;
    protected double receita;

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
     * Gets the value of the curriculoId property.
     * 
     */
    public int getCurriculoId() {
        return curriculoId;
    }

    /**
     * Sets the value of the curriculoId property.
     * 
     */
    public void setCurriculoId(int value) {
        this.curriculoId = value;
    }

    /**
     * Gets the value of the cursoId property.
     * 
     */
    public int getCursoId() {
        return cursoId;
    }

    /**
     * Sets the value of the cursoId property.
     * 
     */
    public void setCursoId(int value) {
        this.cursoId = value;
    }

    /**
     * Gets the value of the turnoId property.
     * 
     */
    public int getTurnoId() {
        return turnoId;
    }

    /**
     * Sets the value of the turnoId property.
     * 
     */
    public void setTurnoId(int value) {
        this.turnoId = value;
    }

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
     * Gets the value of the receita property.
     * 
     */
    public double getReceita() {
        return receita;
    }

    /**
     * Sets the value of the receita property.
     * 
     */
    public void setReceita(double value) {
        this.receita = value;
    }

}
