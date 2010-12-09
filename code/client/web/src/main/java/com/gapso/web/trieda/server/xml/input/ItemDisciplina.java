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
 * <p>Java class for ItemDisciplina complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemDisciplina">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="id" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="codigo" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="nome" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *         &lt;element name="credTeoricos" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="credPraticos" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="laboratorio" type="{http://www.w3.org/2001/XMLSchema}boolean"/>
 *         &lt;element name="maxAlunosTeorico" type="{http://www.w3.org/2001/XMLSchema}int" minOccurs="0"/>
 *         &lt;element name="maxAlunosPratico" type="{http://www.w3.org/2001/XMLSchema}int" minOccurs="0"/>
 *         &lt;element name="tipoDisciplinaId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="nivelDificuldadeId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="divisaoDeCreditos" type="{}ItemDivisaoCreditos" minOccurs="0"/>
 *         &lt;element name="horariosDisponiveis" type="{}GrupoHorario"/>
 *         &lt;element name="disciplinasEquivalentes" type="{}GrupoIdentificador"/>
 *         &lt;element name="disciplinasIncompativeis" type="{}GrupoIdentificador"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemDisciplina", propOrder = {
    "id",
    "codigo",
    "nome",
    "credTeoricos",
    "credPraticos",
    "laboratorio",
    "maxAlunosTeorico",
    "maxAlunosPratico",
    "tipoDisciplinaId",
    "nivelDificuldadeId",
    "divisaoDeCreditos",
    "horariosDisponiveis",
    "disciplinasEquivalentes",
    "disciplinasIncompativeis"
})
public class ItemDisciplina {

    protected int id;
    @XmlElement(required = true)
    protected String codigo;
    @XmlElement(required = true)
    protected String nome;
    protected int credTeoricos;
    protected int credPraticos;
    protected boolean laboratorio;
    protected Integer maxAlunosTeorico;
    protected Integer maxAlunosPratico;
    protected int tipoDisciplinaId;
    protected int nivelDificuldadeId;
    protected ItemDivisaoCreditos divisaoDeCreditos;
    @XmlElement(required = true)
    protected GrupoHorario horariosDisponiveis;
    @XmlElement(required = true)
    protected GrupoIdentificador disciplinasEquivalentes;
    @XmlElement(required = true)
    protected GrupoIdentificador disciplinasIncompativeis;

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
     * Gets the value of the credTeoricos property.
     * 
     */
    public int getCredTeoricos() {
        return credTeoricos;
    }

    /**
     * Sets the value of the credTeoricos property.
     * 
     */
    public void setCredTeoricos(int value) {
        this.credTeoricos = value;
    }

    /**
     * Gets the value of the credPraticos property.
     * 
     */
    public int getCredPraticos() {
        return credPraticos;
    }

    /**
     * Sets the value of the credPraticos property.
     * 
     */
    public void setCredPraticos(int value) {
        this.credPraticos = value;
    }

    /**
     * Gets the value of the laboratorio property.
     * 
     */
    public boolean isLaboratorio() {
        return laboratorio;
    }

    /**
     * Sets the value of the laboratorio property.
     * 
     */
    public void setLaboratorio(boolean value) {
        this.laboratorio = value;
    }

    /**
     * Gets the value of the maxAlunosTeorico property.
     * 
     * @return
     *     possible object is
     *     {@link Integer }
     *     
     */
    public Integer getMaxAlunosTeorico() {
        return maxAlunosTeorico;
    }

    /**
     * Sets the value of the maxAlunosTeorico property.
     * 
     * @param value
     *     allowed object is
     *     {@link Integer }
     *     
     */
    public void setMaxAlunosTeorico(Integer value) {
        this.maxAlunosTeorico = value;
    }

    /**
     * Gets the value of the maxAlunosPratico property.
     * 
     * @return
     *     possible object is
     *     {@link Integer }
     *     
     */
    public Integer getMaxAlunosPratico() {
        return maxAlunosPratico;
    }

    /**
     * Sets the value of the maxAlunosPratico property.
     * 
     * @param value
     *     allowed object is
     *     {@link Integer }
     *     
     */
    public void setMaxAlunosPratico(Integer value) {
        this.maxAlunosPratico = value;
    }

    /**
     * Gets the value of the tipoDisciplinaId property.
     * 
     */
    public int getTipoDisciplinaId() {
        return tipoDisciplinaId;
    }

    /**
     * Sets the value of the tipoDisciplinaId property.
     * 
     */
    public void setTipoDisciplinaId(int value) {
        this.tipoDisciplinaId = value;
    }

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
     * Gets the value of the divisaoDeCreditos property.
     * 
     * @return
     *     possible object is
     *     {@link ItemDivisaoCreditos }
     *     
     */
    public ItemDivisaoCreditos getDivisaoDeCreditos() {
        return divisaoDeCreditos;
    }

    /**
     * Sets the value of the divisaoDeCreditos property.
     * 
     * @param value
     *     allowed object is
     *     {@link ItemDivisaoCreditos }
     *     
     */
    public void setDivisaoDeCreditos(ItemDivisaoCreditos value) {
        this.divisaoDeCreditos = value;
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
     * Gets the value of the disciplinasEquivalentes property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoIdentificador }
     *     
     */
    public GrupoIdentificador getDisciplinasEquivalentes() {
        return disciplinasEquivalentes;
    }

    /**
     * Sets the value of the disciplinasEquivalentes property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoIdentificador }
     *     
     */
    public void setDisciplinasEquivalentes(GrupoIdentificador value) {
        this.disciplinasEquivalentes = value;
    }

    /**
     * Gets the value of the disciplinasIncompativeis property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoIdentificador }
     *     
     */
    public GrupoIdentificador getDisciplinasIncompativeis() {
        return disciplinasIncompativeis;
    }

    /**
     * Sets the value of the disciplinasIncompativeis property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoIdentificador }
     *     
     */
    public void setDisciplinasIncompativeis(GrupoIdentificador value) {
        this.disciplinasIncompativeis = value;
    }

}
