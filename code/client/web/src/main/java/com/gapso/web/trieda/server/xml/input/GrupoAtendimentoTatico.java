//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, vhudson-jaxb-ri-2.1-2 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2010.12.14 at 10:32:43 AM BRST 
//


package com.gapso.web.trieda.server.xml.input;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for GrupoAtendimentoTatico complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="GrupoAtendimentoTatico">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence maxOccurs="unbounded" minOccurs="0">
 *         &lt;element name="AtendimentoTatico" type="{}ItemAtendimentoTatico"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "GrupoAtendimentoTatico", propOrder = {
    "atendimentoTatico"
})
public class GrupoAtendimentoTatico {

    @XmlElement(name = "AtendimentoTatico")
    protected List<ItemAtendimentoTatico> atendimentoTatico;

    /**
     * Gets the value of the atendimentoTatico property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the atendimentoTatico property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getAtendimentoTatico().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link ItemAtendimentoTatico }
     * 
     * 
     */
    public List<ItemAtendimentoTatico> getAtendimentoTatico() {
        if (atendimentoTatico == null) {
            atendimentoTatico = new ArrayList<ItemAtendimentoTatico>();
        }
        return this.atendimentoTatico;
    }

}
