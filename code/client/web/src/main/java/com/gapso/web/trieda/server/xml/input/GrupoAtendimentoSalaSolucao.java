//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, v2.2.4 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.11.08 at 11:18:45 AM BRST 
//


package com.gapso.web.trieda.server.xml.input;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for GrupoAtendimentoSalaSolucao complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="GrupoAtendimentoSalaSolucao">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence maxOccurs="unbounded" minOccurs="0">
 *         &lt;element name="AtendimentoSala" type="{}ItemAtendimentoSalaSolucao"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "GrupoAtendimentoSalaSolucao", propOrder = {
    "atendimentoSala"
})
public class GrupoAtendimentoSalaSolucao {

    @XmlElement(name = "AtendimentoSala")
    protected List<ItemAtendimentoSalaSolucao> atendimentoSala;

    /**
     * Gets the value of the atendimentoSala property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the atendimentoSala property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getAtendimentoSala().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link ItemAtendimentoSalaSolucao }
     * 
     * 
     */
    public List<ItemAtendimentoSalaSolucao> getAtendimentoSala() {
        if (atendimentoSala == null) {
            atendimentoSala = new ArrayList<ItemAtendimentoSalaSolucao>();
        }
        return this.atendimentoSala;
    }

}
