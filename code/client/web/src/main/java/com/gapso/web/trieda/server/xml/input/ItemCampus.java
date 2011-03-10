//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, vhudson-jaxb-ri-2.1-2 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.03.10 at 10:21:31 AM BRT 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemCampus complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemCampus">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="id" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="codigo" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="nome" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="unidades" type="{}GrupoUnidade"/>
 *         &lt;element name="professores" type="{}GrupoProfessor"/>
 *         &lt;element name="horariosDisponiveis" type="{}GrupoHorario"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemCampus", propOrder = {
    "id",
    "codigo",
    "nome",
    "unidades",
    "professores",
    "horariosDisponiveis"
})
public class ItemCampus {

    protected int id;
    @XmlElement(required = true)
    protected String codigo;
    @XmlElement(required = true)
    protected String nome;
    @XmlElement(required = true)
    protected GrupoUnidade unidades;
    @XmlElement(required = true)
    protected GrupoProfessor professores;
    @XmlElement(required = true)
    protected GrupoHorario horariosDisponiveis;

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
     * Gets the value of the unidades property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoUnidade }
     *     
     */
    public GrupoUnidade getUnidades() {
        return unidades;
    }

    /**
     * Sets the value of the unidades property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoUnidade }
     *     
     */
    public void setUnidades(GrupoUnidade value) {
        this.unidades = value;
    }

    /**
     * Gets the value of the professores property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoProfessor }
     *     
     */
    public GrupoProfessor getProfessores() {
        return professores;
    }

    /**
     * Sets the value of the professores property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoProfessor }
     *     
     */
    public void setProfessores(GrupoProfessor value) {
        this.professores = value;
    }

    /**
     * Gets the value of the horariosDisponiveis property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoHorario }
     *     
     */
    public GrupoHorario getHorariosDisponiveis() {
        return horariosDisponiveis;
    }

    /**
     * Sets the value of the horariosDisponiveis property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoHorario }
     *     
     */
    public void setHorariosDisponiveis(GrupoHorario value) {
        this.horariosDisponiveis = value;
    }

}
