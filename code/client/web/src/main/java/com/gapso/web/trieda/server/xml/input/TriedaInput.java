//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, vhudson-jaxb-ri-2.1-2 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.01.12 at 01:06:23 PM BRST 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for anonymous complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType>
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="calendario" type="{}ItemCalendario"/>
 *         &lt;element name="tiposSala" type="{}GrupoTipoSala"/>
 *         &lt;element name="tiposContrato" type="{}GrupoTipoContrato"/>
 *         &lt;element name="tiposTitulacao" type="{}GrupoTipoTitulacao"/>
 *         &lt;element name="areasTitulacao" type="{}GrupoAreaTitulacao"/>
 *         &lt;element name="tiposDisciplina" type="{}GrupoTipoDisciplina"/>
 *         &lt;element name="niveisDificuldade" type="{}GrupoNivelDificuldade"/>
 *         &lt;element name="tiposCurso" type="{}GrupoTipoCurso"/>
 *         &lt;element name="regrasDivisaoCredito" type="{}GrupoDivisaoCreditos"/>
 *         &lt;element name="campi" type="{}GrupoCampus"/>
 *         &lt;element name="temposDeslocamentosCampi" type="{}GrupoDeslocamento"/>
 *         &lt;element name="temposDeslocamentosUnidades" type="{}GrupoDeslocamento"/>
 *         &lt;element name="disciplinas" type="{}GrupoDisciplina"/>
 *         &lt;element name="cursos" type="{}GrupoCurso"/>
 *         &lt;element name="ofertaCursosCampi" type="{}GrupoOfertaCurso"/>
 *         &lt;element name="demandas" type="{}GrupoDemanda"/>
 *         &lt;element name="parametrosPlanejamento" type="{}ItemParametrosPlanejamento"/>
 *         &lt;element name="fixacoes" type="{}GrupoFixacao"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "", propOrder = {
    "calendario",
    "tiposSala",
    "tiposContrato",
    "tiposTitulacao",
    "areasTitulacao",
    "tiposDisciplina",
    "niveisDificuldade",
    "tiposCurso",
    "regrasDivisaoCredito",
    "campi",
    "temposDeslocamentosCampi",
    "temposDeslocamentosUnidades",
    "disciplinas",
    "cursos",
    "ofertaCursosCampi",
    "demandas",
    "parametrosPlanejamento",
    "fixacoes"
})
@XmlRootElement(name = "TriedaInput")
public class TriedaInput {

    @XmlElement(required = true)
    protected ItemCalendario calendario;
    @XmlElement(required = true)
    protected GrupoTipoSala tiposSala;
    @XmlElement(required = true)
    protected GrupoTipoContrato tiposContrato;
    @XmlElement(required = true)
    protected GrupoTipoTitulacao tiposTitulacao;
    @XmlElement(required = true)
    protected GrupoAreaTitulacao areasTitulacao;
    @XmlElement(required = true)
    protected GrupoTipoDisciplina tiposDisciplina;
    @XmlElement(required = true)
    protected GrupoNivelDificuldade niveisDificuldade;
    @XmlElement(required = true)
    protected GrupoTipoCurso tiposCurso;
    @XmlElement(required = true)
    protected GrupoDivisaoCreditos regrasDivisaoCredito;
    @XmlElement(required = true)
    protected GrupoCampus campi;
    @XmlElement(required = true)
    protected GrupoDeslocamento temposDeslocamentosCampi;
    @XmlElement(required = true)
    protected GrupoDeslocamento temposDeslocamentosUnidades;
    @XmlElement(required = true)
    protected GrupoDisciplina disciplinas;
    @XmlElement(required = true)
    protected GrupoCurso cursos;
    @XmlElement(required = true)
    protected GrupoOfertaCurso ofertaCursosCampi;
    @XmlElement(required = true)
    protected GrupoDemanda demandas;
    @XmlElement(required = true)
    protected ItemParametrosPlanejamento parametrosPlanejamento;
    @XmlElement(required = true)
    protected GrupoFixacao fixacoes;

    /**
     * Gets the value of the calendario property.
     * 
     * @return
     *     possible object is
     *     {@link ItemCalendario }
     *     
     */
    public ItemCalendario getCalendario() {
        return calendario;
    }

    /**
     * Sets the value of the calendario property.
     * 
     * @param value
     *     allowed object is
     *     {@link ItemCalendario }
     *     
     */
    public void setCalendario(ItemCalendario value) {
        this.calendario = value;
    }

    /**
     * Gets the value of the tiposSala property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoTipoSala }
     *     
     */
    public GrupoTipoSala getTiposSala() {
        return tiposSala;
    }

    /**
     * Sets the value of the tiposSala property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoTipoSala }
     *     
     */
    public void setTiposSala(GrupoTipoSala value) {
        this.tiposSala = value;
    }

    /**
     * Gets the value of the tiposContrato property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoTipoContrato }
     *     
     */
    public GrupoTipoContrato getTiposContrato() {
        return tiposContrato;
    }

    /**
     * Sets the value of the tiposContrato property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoTipoContrato }
     *     
     */
    public void setTiposContrato(GrupoTipoContrato value) {
        this.tiposContrato = value;
    }

    /**
     * Gets the value of the tiposTitulacao property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoTipoTitulacao }
     *     
     */
    public GrupoTipoTitulacao getTiposTitulacao() {
        return tiposTitulacao;
    }

    /**
     * Sets the value of the tiposTitulacao property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoTipoTitulacao }
     *     
     */
    public void setTiposTitulacao(GrupoTipoTitulacao value) {
        this.tiposTitulacao = value;
    }

    /**
     * Gets the value of the areasTitulacao property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoAreaTitulacao }
     *     
     */
    public GrupoAreaTitulacao getAreasTitulacao() {
        return areasTitulacao;
    }

    /**
     * Sets the value of the areasTitulacao property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoAreaTitulacao }
     *     
     */
    public void setAreasTitulacao(GrupoAreaTitulacao value) {
        this.areasTitulacao = value;
    }

    /**
     * Gets the value of the tiposDisciplina property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoTipoDisciplina }
     *     
     */
    public GrupoTipoDisciplina getTiposDisciplina() {
        return tiposDisciplina;
    }

    /**
     * Sets the value of the tiposDisciplina property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoTipoDisciplina }
     *     
     */
    public void setTiposDisciplina(GrupoTipoDisciplina value) {
        this.tiposDisciplina = value;
    }

    /**
     * Gets the value of the niveisDificuldade property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoNivelDificuldade }
     *     
     */
    public GrupoNivelDificuldade getNiveisDificuldade() {
        return niveisDificuldade;
    }

    /**
     * Sets the value of the niveisDificuldade property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoNivelDificuldade }
     *     
     */
    public void setNiveisDificuldade(GrupoNivelDificuldade value) {
        this.niveisDificuldade = value;
    }

    /**
     * Gets the value of the tiposCurso property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoTipoCurso }
     *     
     */
    public GrupoTipoCurso getTiposCurso() {
        return tiposCurso;
    }

    /**
     * Sets the value of the tiposCurso property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoTipoCurso }
     *     
     */
    public void setTiposCurso(GrupoTipoCurso value) {
        this.tiposCurso = value;
    }

    /**
     * Gets the value of the regrasDivisaoCredito property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoDivisaoCreditos }
     *     
     */
    public GrupoDivisaoCreditos getRegrasDivisaoCredito() {
        return regrasDivisaoCredito;
    }

    /**
     * Sets the value of the regrasDivisaoCredito property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoDivisaoCreditos }
     *     
     */
    public void setRegrasDivisaoCredito(GrupoDivisaoCreditos value) {
        this.regrasDivisaoCredito = value;
    }

    /**
     * Gets the value of the campi property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoCampus }
     *     
     */
    public GrupoCampus getCampi() {
        return campi;
    }

    /**
     * Sets the value of the campi property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoCampus }
     *     
     */
    public void setCampi(GrupoCampus value) {
        this.campi = value;
    }

    /**
     * Gets the value of the temposDeslocamentosCampi property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoDeslocamento }
     *     
     */
    public GrupoDeslocamento getTemposDeslocamentosCampi() {
        return temposDeslocamentosCampi;
    }

    /**
     * Sets the value of the temposDeslocamentosCampi property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoDeslocamento }
     *     
     */
    public void setTemposDeslocamentosCampi(GrupoDeslocamento value) {
        this.temposDeslocamentosCampi = value;
    }

    /**
     * Gets the value of the temposDeslocamentosUnidades property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoDeslocamento }
     *     
     */
    public GrupoDeslocamento getTemposDeslocamentosUnidades() {
        return temposDeslocamentosUnidades;
    }

    /**
     * Sets the value of the temposDeslocamentosUnidades property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoDeslocamento }
     *     
     */
    public void setTemposDeslocamentosUnidades(GrupoDeslocamento value) {
        this.temposDeslocamentosUnidades = value;
    }

    /**
     * Gets the value of the disciplinas property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoDisciplina }
     *     
     */
    public GrupoDisciplina getDisciplinas() {
        return disciplinas;
    }

    /**
     * Sets the value of the disciplinas property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoDisciplina }
     *     
     */
    public void setDisciplinas(GrupoDisciplina value) {
        this.disciplinas = value;
    }

    /**
     * Gets the value of the cursos property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoCurso }
     *     
     */
    public GrupoCurso getCursos() {
        return cursos;
    }

    /**
     * Sets the value of the cursos property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoCurso }
     *     
     */
    public void setCursos(GrupoCurso value) {
        this.cursos = value;
    }

    /**
     * Gets the value of the ofertaCursosCampi property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoOfertaCurso }
     *     
     */
    public GrupoOfertaCurso getOfertaCursosCampi() {
        return ofertaCursosCampi;
    }

    /**
     * Sets the value of the ofertaCursosCampi property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoOfertaCurso }
     *     
     */
    public void setOfertaCursosCampi(GrupoOfertaCurso value) {
        this.ofertaCursosCampi = value;
    }

    /**
     * Gets the value of the demandas property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoDemanda }
     *     
     */
    public GrupoDemanda getDemandas() {
        return demandas;
    }

    /**
     * Sets the value of the demandas property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoDemanda }
     *     
     */
    public void setDemandas(GrupoDemanda value) {
        this.demandas = value;
    }

    /**
     * Gets the value of the parametrosPlanejamento property.
     * 
     * @return
     *     possible object is
     *     {@link ItemParametrosPlanejamento }
     *     
     */
    public ItemParametrosPlanejamento getParametrosPlanejamento() {
        return parametrosPlanejamento;
    }

    /**
     * Sets the value of the parametrosPlanejamento property.
     * 
     * @param value
     *     allowed object is
     *     {@link ItemParametrosPlanejamento }
     *     
     */
    public void setParametrosPlanejamento(ItemParametrosPlanejamento value) {
        this.parametrosPlanejamento = value;
    }

    /**
     * Gets the value of the fixacoes property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoFixacao }
     *     
     */
    public GrupoFixacao getFixacoes() {
        return fixacoes;
    }

    /**
     * Sets the value of the fixacoes property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoFixacao }
     *     
     */
    public void setFixacoes(GrupoFixacao value) {
        this.fixacoes = value;
    }

}
