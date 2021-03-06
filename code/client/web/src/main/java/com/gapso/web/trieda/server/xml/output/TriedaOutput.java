//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, v2.2.4 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.09.23 at 05:21:16 PM BRT 
//


package com.gapso.web.trieda.server.xml.output;

import java.util.ArrayList;
import java.util.List;
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
 *         &lt;element name="atendimentos" type="{}GrupoAtendimentoCampus"/>
 *         &lt;element name="Warnings" type="{}GrupoWarning"/>
 *         &lt;element name="Errors" type="{}GrupoError"/>
 *         &lt;element name="restricoesVioladas">
 *           &lt;complexType>
 *             &lt;complexContent>
 *               &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *                 &lt;sequence minOccurs="0">
 *                   &lt;element name="restricaoViolada" maxOccurs="unbounded" minOccurs="0">
 *                     &lt;complexType>
 *                       &lt;complexContent>
 *                         &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *                           &lt;sequence>
 *                             &lt;element name="restricao" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *                             &lt;element name="valor" type="{http://www.w3.org/2001/XMLSchema}double"/>
 *                             &lt;element name="unidade" type="{http://www.w3.org/2001/XMLSchema}string"/>
 *                           &lt;/sequence>
 *                         &lt;/restriction>
 *                       &lt;/complexContent>
 *                     &lt;/complexType>
 *                   &lt;/element>
 *                 &lt;/sequence>
 *               &lt;/restriction>
 *             &lt;/complexContent>
 *           &lt;/complexType>
 *         &lt;/element>
 *         &lt;element name="professoresVirtuais" type="{}GrupoProfessoresVirtuais"/>
 *         &lt;element name="NaoAtendimentosTatico" type="{}GrupoFolgaAlunoDemanda"/>
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
    "atendimentos",
    "warnings",
    "errors",
    "restricoesVioladas",
    "professoresVirtuais",
    "folgaAlunoDemanda"
})
@XmlRootElement(name = "TriedaOutput")
public class TriedaOutput {

    @XmlElement(required = true)
    protected GrupoAtendimentoCampus atendimentos;
    @XmlElement(name = "Warnings", required = true)
    protected GrupoWarning warnings;
    @XmlElement(name = "Errors", required = true)
    protected GrupoError errors;
    @XmlElement(required = true)
    protected TriedaOutput.RestricoesVioladas restricoesVioladas;
    @XmlElement(required = true)
    protected GrupoProfessoresVirtuais professoresVirtuais;
    @XmlElement(name = "NaoAtendimentosTatico", required = true)
    protected GrupoFolgaAlunoDemanda folgaAlunoDemanda;

    /**
     * Gets the value of the atendimentos property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoAtendimentoCampus }
     *     
     */
    public GrupoAtendimentoCampus getAtendimentos() {
        return atendimentos;
    }

    /**
     * Sets the value of the atendimentos property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoAtendimentoCampus }
     *     
     */
    public void setAtendimentos(GrupoAtendimentoCampus value) {
        this.atendimentos = value;
    }

    /**
     * Gets the value of the warnings property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoWarning }
     *     
     */
    public GrupoWarning getWarnings() {
        return warnings;
    }

    /**
     * Sets the value of the warnings property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoWarning }
     *     
     */
    public void setWarnings(GrupoWarning value) {
        this.warnings = value;
    }

    /**
     * Gets the value of the errors property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoError }
     *     
     */
    public GrupoError getErrors() {
        return errors;
    }

    /**
     * Sets the value of the errors property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoError }
     *     
     */
    public void setErrors(GrupoError value) {
        this.errors = value;
    }

    /**
     * Gets the value of the restricoesVioladas property.
     * 
     * @return
     *     possible object is
     *     {@link TriedaOutput.RestricoesVioladas }
     *     
     */
    public TriedaOutput.RestricoesVioladas getRestricoesVioladas() {
        return restricoesVioladas;
    }

    /**
     * Sets the value of the restricoesVioladas property.
     * 
     * @param value
     *     allowed object is
     *     {@link TriedaOutput.RestricoesVioladas }
     *     
     */
    public void setRestricoesVioladas(TriedaOutput.RestricoesVioladas value) {
        this.restricoesVioladas = value;
    }

    /**
     * Gets the value of the professoresVirtuais property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoProfessoresVirtuais }
     *     
     */
    public GrupoProfessoresVirtuais getProfessoresVirtuais() {
        return professoresVirtuais;
    }

    /**
     * Sets the value of the professoresVirtuais property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoProfessoresVirtuais }
     *     
     */
    public void setProfessoresVirtuais(GrupoProfessoresVirtuais value) {
        this.professoresVirtuais = value;
    }
    
    /**
     * Gets the value of the folgaAlunoDemanda property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoProfessoresVirtuais }
     *     
     */
    public GrupoFolgaAlunoDemanda getFolgaAlunoDemanda() {
        return folgaAlunoDemanda;
    }

    /**
     * Sets the value of the folgaAlunoDemanda property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoProfessoresVirtuais }
     *     
     */
    public void setFolgaAlunoDemanda(GrupoFolgaAlunoDemanda value) {
        this.folgaAlunoDemanda = value;
    }

    /**
     * <p>Java class for anonymous complex type.
     * 
     * <p>The following schema fragment specifies the expected content contained within this class.
     * 
     * <pre>
     * &lt;complexType>
     *   &lt;complexContent>
     *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
     *       &lt;sequence minOccurs="0">
     *         &lt;element name="restricaoViolada" maxOccurs="unbounded" minOccurs="0">
     *           &lt;complexType>
     *             &lt;complexContent>
     *               &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
     *                 &lt;sequence>
     *                   &lt;element name="restricao" type="{http://www.w3.org/2001/XMLSchema}string"/>
     *                   &lt;element name="valor" type="{http://www.w3.org/2001/XMLSchema}double"/>
     *                   &lt;element name="unidade" type="{http://www.w3.org/2001/XMLSchema}string"/>
     *                 &lt;/sequence>
     *               &lt;/restriction>
     *             &lt;/complexContent>
     *           &lt;/complexType>
     *         &lt;/element>
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
        "restricaoViolada"
    })
    public static class RestricoesVioladas {

        protected List<TriedaOutput.RestricoesVioladas.RestricaoViolada> restricaoViolada;

        /**
         * Gets the value of the restricaoViolada property.
         * 
         * <p>
         * This accessor method returns a reference to the live list,
         * not a snapshot. Therefore any modification you make to the
         * returned list will be present inside the JAXB object.
         * This is why there is not a <CODE>set</CODE> method for the restricaoViolada property.
         * 
         * <p>
         * For example, to add a new item, do as follows:
         * <pre>
         *    getRestricaoViolada().add(newItem);
         * </pre>
         * 
         * 
         * <p>
         * Objects of the following type(s) are allowed in the list
         * {@link TriedaOutput.RestricoesVioladas.RestricaoViolada }
         * 
         * 
         */
        public List<TriedaOutput.RestricoesVioladas.RestricaoViolada> getRestricaoViolada() {
            if (restricaoViolada == null) {
                restricaoViolada = new ArrayList<TriedaOutput.RestricoesVioladas.RestricaoViolada>();
            }
            return this.restricaoViolada;
        }


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
         *         &lt;element name="restricao" type="{http://www.w3.org/2001/XMLSchema}string"/>
         *         &lt;element name="valor" type="{http://www.w3.org/2001/XMLSchema}double"/>
         *         &lt;element name="unidade" type="{http://www.w3.org/2001/XMLSchema}string"/>
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
            "restricao",
            "valor",
            "unidade"
        })
        public static class RestricaoViolada {

            @XmlElement(required = true)
            protected String restricao;
            protected double valor;
            @XmlElement(required = true)
            protected String unidade;

            /**
             * Gets the value of the restricao property.
             * 
             * @return
             *     possible object is
             *     {@link String }
             *     
             */
            public String getRestricao() {
                return restricao;
            }

            /**
             * Sets the value of the restricao property.
             * 
             * @param value
             *     allowed object is
             *     {@link String }
             *     
             */
            public void setRestricao(String value) {
                this.restricao = value;
            }

            /**
             * Gets the value of the valor property.
             * 
             */
            public double getValor() {
                return valor;
            }

            /**
             * Sets the value of the valor property.
             * 
             */
            public void setValor(double value) {
                this.valor = value;
            }

            /**
             * Gets the value of the unidade property.
             * 
             * @return
             *     possible object is
             *     {@link String }
             *     
             */
            public String getUnidade() {
                return unidade;
            }

            /**
             * Sets the value of the unidade property.
             * 
             * @param value
             *     allowed object is
             *     {@link String }
             *     
             */
            public void setUnidade(String value) {
                this.unidade = value;
            }

        }

    }

}
