//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, v2.2.4 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.11.08 at 11:18:45 AM BRST 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemAtendimentoDiaSemanaSolucao complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemAtendimentoDiaSemanaSolucao">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="diaSemana" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="atendimentosTatico" type="{}GrupoAtendimentoTaticoSolucao"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemAtendimentoDiaSemanaSolucao", propOrder = {
    "diaSemana",
    "atendimentosTatico"
})
public class ItemAtendimentoDiaSemanaSolucao {

    protected int diaSemana;
    @XmlElement(required = true)
    protected GrupoAtendimentoTaticoSolucao atendimentosTatico;

    /**
     * Gets the value of the diaSemana property.
     * 
     */
    public int getDiaSemana() {
        return diaSemana;
    }

    /**
     * Sets the value of the diaSemana property.
     * 
     */
    public void setDiaSemana(int value) {
        this.diaSemana = value;
    }

    /**
     * Gets the value of the atendimentosTatico property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoAtendimentoTaticoSolucao }
     *     
     */
    public GrupoAtendimentoTaticoSolucao getAtendimentosTatico() {
        return atendimentosTatico;
    }

    /**
     * Sets the value of the atendimentosTatico property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoAtendimentoTaticoSolucao }
     *     
     */
    public void setAtendimentosTatico(GrupoAtendimentoTaticoSolucao value) {
        this.atendimentosTatico = value;
    }

}
