//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, v2.2.4 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.08.18 at 08:41:39 AM BRT 
//


package com.gapso.web.trieda.server.xml.input;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for GrupoCreditoDisponivel complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="GrupoCreditoDisponivel">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence maxOccurs="unbounded" minOccurs="0">
 *         &lt;element name="CreditoDisponivel" type="{}ItemCreditoDisponivel"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "GrupoCreditoDisponivel", propOrder = {
    "creditoDisponivel"
})
public class GrupoCreditoDisponivel {

    @XmlElement(name = "CreditoDisponivel")
    protected List<ItemCreditoDisponivel> creditoDisponivel;

    /**
     * Gets the value of the creditoDisponivel property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the creditoDisponivel property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getCreditoDisponivel().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link ItemCreditoDisponivel }
     * 
     * 
     */
    public List<ItemCreditoDisponivel> getCreditoDisponivel() {
        if (creditoDisponivel == null) {
            creditoDisponivel = new ArrayList<ItemCreditoDisponivel>();
        }
        return this.creditoDisponivel;
    }

}
