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
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemTurno complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemTurno">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="id" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="nome" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="tempoAula" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="HorariosAula" type="{}GrupoHorarioAula"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemTurno", propOrder = {
    "id",
    "nome",
    "tempoAula",
    "horariosAula"
})
public class ItemTurno {

    protected int id;
    @XmlElement(required = true)
    protected String nome;
    protected int tempoAula;
    @XmlElement(name = "HorariosAula", required = true)
    protected GrupoHorarioAula horariosAula;

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
     * Gets the value of the nome property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getNome() {
        return nome;
    }

    /**
     * Sets the value of the nome property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setNome(String value) {
        this.nome = value;
    }

    /**
     * Gets the value of the tempoAula property.
     * 
     */
    public int getTempoAula() {
        return tempoAula;
    }

    /**
     * Sets the value of the tempoAula property.
     * 
     */
    public void setTempoAula(int value) {
        this.tempoAula = value;
    }

    /**
     * Gets the value of the horariosAula property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoHorarioAula }
     *     
     */
    public GrupoHorarioAula getHorariosAula() {
        return horariosAula;
    }

    /**
     * Sets the value of the horariosAula property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoHorarioAula }
     *     
     */
    public void setHorariosAula(GrupoHorarioAula value) {
        this.horariosAula = value;
    }

}