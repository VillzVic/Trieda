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
 * <p>Java class for ItemSala complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemSala">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="id" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="codigo" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="andar" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="numero" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="tipoSalaId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="capacidade" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;choice>
 *           &lt;element name="horariosDisponiveis" type="{}GrupoHorario"/>
 *           &lt;element name="creditosDisponiveis" type="{}GrupoCreditoDisponivel"/>
 *         &lt;/choice>
 *         &lt;element name="disciplinasAssociadas" type="{}GrupoIdentificador"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemSala", propOrder = {
    "id",
    "codigo",
    "andar",
    "numero",
    "tipoSalaId",
    "capacidade",
    "horariosDisponiveis",
    "creditosDisponiveis",
    "disciplinasAssociadas"
})
public class ItemSala {

    protected int id;
    @XmlElement(required = true)
    protected String codigo;
    @XmlElement(required = true)
    protected String andar;
    @XmlElement(required = true)
    protected String numero;
    protected int tipoSalaId;
    protected int capacidade;
    protected GrupoHorario horariosDisponiveis;
    protected GrupoCreditoDisponivel creditosDisponiveis;
    @XmlElement(required = true)
    protected GrupoIdentificador disciplinasAssociadas;

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
     * Gets the value of the andar property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getAndar() {
        return andar;
    }

    /**
     * Sets the value of the andar property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setAndar(String value) {
        this.andar = value;
    }

    /**
     * Gets the value of the numero property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getNumero() {
        return numero;
    }

    /**
     * Sets the value of the numero property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setNumero(String value) {
        this.numero = value;
    }

    /**
     * Gets the value of the tipoSalaId property.
     * 
     */
    public int getTipoSalaId() {
        return tipoSalaId;
    }

    /**
     * Sets the value of the tipoSalaId property.
     * 
     */
    public void setTipoSalaId(int value) {
        this.tipoSalaId = value;
    }

    /**
     * Gets the value of the capacidade property.
     * 
     */
    public int getCapacidade() {
        return capacidade;
    }

    /**
     * Sets the value of the capacidade property.
     * 
     */
    public void setCapacidade(int value) {
        this.capacidade = value;
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

    /**
     * Gets the value of the creditosDisponiveis property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoCreditoDisponivel }
     *     
     */
    public GrupoCreditoDisponivel getCreditosDisponiveis() {
        return creditosDisponiveis;
    }

    /**
     * Sets the value of the creditosDisponiveis property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoCreditoDisponivel }
     *     
     */
    public void setCreditosDisponiveis(GrupoCreditoDisponivel value) {
        this.creditosDisponiveis = value;
    }

    /**
     * Gets the value of the disciplinasAssociadas property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoIdentificador }
     *     
     */
    public GrupoIdentificador getDisciplinasAssociadas() {
        return disciplinasAssociadas;
    }

    /**
     * Sets the value of the disciplinasAssociadas property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoIdentificador }
     *     
     */
    public void setDisciplinasAssociadas(GrupoIdentificador value) {
        this.disciplinasAssociadas = value;
    }

}
