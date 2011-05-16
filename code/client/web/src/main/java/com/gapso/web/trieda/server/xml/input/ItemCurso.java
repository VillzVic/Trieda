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
 * <p>Java class for ItemCurso complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemCurso">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="id" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="codigo" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="tipoId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="regraPercMinMestres" type="{}ItemPercentualMinimo"/>
 *         &lt;element name="regraPercMinDoutores" type="{}ItemPercentualMinimo"/>
 *         &lt;element name="qtdMaxProfDisc" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="maisDeUmaDiscPeriodo" type="{http://www.w3.org/2001/XMLSchema}boolean"/>
 *         &lt;element name="areasTitulacao" type="{}GrupoIdentificador"/>
 *         &lt;element name="curriculos" type="{}GrupoCurriculo"/>
 *         &lt;element name="minTempoIntegral" type="{http://www.w3.org/2001/XMLSchema}double"/>
 *         &lt;element name="minTempoIntegralParcial" type="{http://www.w3.org/2001/XMLSchema}double"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemCurso", propOrder = {
    "id",
    "codigo",
    "tipoId",
    "regraPercMinMestres",
    "regraPercMinDoutores",
    "qtdMaxProfDisc",
    "maisDeUmaDiscPeriodo",
    "areasTitulacao",
    "curriculos",
    "minTempoIntegral",
    "minTempoIntegralParcial"
})
public class ItemCurso {

    protected int id;
    @XmlElement(required = true)
    protected String codigo;
    protected int tipoId;
    @XmlElement(required = true)
    protected ItemPercentualMinimo regraPercMinMestres;
    @XmlElement(required = true)
    protected ItemPercentualMinimo regraPercMinDoutores;
    protected int qtdMaxProfDisc;
    protected boolean maisDeUmaDiscPeriodo;
    @XmlElement(required = true)
    protected GrupoIdentificador areasTitulacao;
    @XmlElement(required = true)
    protected GrupoCurriculo curriculos;
    protected double minTempoIntegral;
    protected double minTempoIntegralParcial;

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
     * Gets the value of the tipoId property.
     * 
     */
    public int getTipoId() {
        return tipoId;
    }

    /**
     * Sets the value of the tipoId property.
     * 
     */
    public void setTipoId(int value) {
        this.tipoId = value;
    }

    /**
     * Gets the value of the regraPercMinMestres property.
     * 
     * @return
     *     possible object is
     *     {@link ItemPercentualMinimo }
     *     
     */
    public ItemPercentualMinimo getRegraPercMinMestres() {
        return regraPercMinMestres;
    }

    /**
     * Sets the value of the regraPercMinMestres property.
     * 
     * @param value
     *     allowed object is
     *     {@link ItemPercentualMinimo }
     *     
     */
    public void setRegraPercMinMestres(ItemPercentualMinimo value) {
        this.regraPercMinMestres = value;
    }

    /**
     * Gets the value of the regraPercMinDoutores property.
     * 
     * @return
     *     possible object is
     *     {@link ItemPercentualMinimo }
     *     
     */
    public ItemPercentualMinimo getRegraPercMinDoutores() {
        return regraPercMinDoutores;
    }

    /**
     * Sets the value of the regraPercMinDoutores property.
     * 
     * @param value
     *     allowed object is
     *     {@link ItemPercentualMinimo }
     *     
     */
    public void setRegraPercMinDoutores(ItemPercentualMinimo value) {
        this.regraPercMinDoutores = value;
    }

    /**
     * Gets the value of the qtdMaxProfDisc property.
     * 
     */
    public int getQtdMaxProfDisc() {
        return qtdMaxProfDisc;
    }

    /**
     * Sets the value of the qtdMaxProfDisc property.
     * 
     */
    public void setQtdMaxProfDisc(int value) {
        this.qtdMaxProfDisc = value;
    }

    /**
     * Gets the value of the maisDeUmaDiscPeriodo property.
     * 
     */
    public boolean isMaisDeUmaDiscPeriodo() {
        return maisDeUmaDiscPeriodo;
    }

    /**
     * Sets the value of the maisDeUmaDiscPeriodo property.
     * 
     */
    public void setMaisDeUmaDiscPeriodo(boolean value) {
        this.maisDeUmaDiscPeriodo = value;
    }

    /**
     * Gets the value of the areasTitulacao property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoIdentificador }
     *     
     */
    public GrupoIdentificador getAreasTitulacao() {
        return areasTitulacao;
    }

    /**
     * Sets the value of the areasTitulacao property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoIdentificador }
     *     
     */
    public void setAreasTitulacao(GrupoIdentificador value) {
        this.areasTitulacao = value;
    }

    /**
     * Gets the value of the curriculos property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoCurriculo }
     *     
     */
    public GrupoCurriculo getCurriculos() {
        return curriculos;
    }

    /**
     * Sets the value of the curriculos property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoCurriculo }
     *     
     */
    public void setCurriculos(GrupoCurriculo value) {
        this.curriculos = value;
    }

    /**
     * Gets the value of the minTempoIntegral property.
     * 
     */
    public double getMinTempoIntegral() {
        return minTempoIntegral;
    }

    /**
     * Sets the value of the minTempoIntegral property.
     * 
     */
    public void setMinTempoIntegral(double value) {
        this.minTempoIntegral = value;
    }

    /**
     * Gets the value of the minTempoIntegralParcial property.
     * 
     */
    public double getMinTempoIntegralParcial() {
        return minTempoIntegralParcial;
    }

    /**
     * Sets the value of the minTempoIntegralParcial property.
     * 
     */
    public void setMinTempoIntegralParcial(double value) {
        this.minTempoIntegralParcial = value;
    }

}
