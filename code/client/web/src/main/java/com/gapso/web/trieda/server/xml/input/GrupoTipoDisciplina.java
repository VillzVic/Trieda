//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, vhudson-jaxb-ri-2.1-2 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2010.11.24 at 04:13:11 PM BRST 
//


package com.gapso.web.trieda.server.xml.input;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for GrupoTipoDisciplina complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="GrupoTipoDisciplina">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence maxOccurs="unbounded" minOccurs="0">
 *         &lt;element name="TipoDisciplina" type="{}ItemTipoDisciplina"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "GrupoTipoDisciplina", propOrder = {
    "tipoDisciplina"
})
public class GrupoTipoDisciplina {

    @XmlElement(name = "TipoDisciplina")
    protected List<ItemTipoDisciplina> tipoDisciplina;

    /**
     * Gets the value of the tipoDisciplina property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the tipoDisciplina property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getTipoDisciplina().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link ItemTipoDisciplina }
     * 
     * 
     */
    public List<ItemTipoDisciplina> getTipoDisciplina() {
        if (tipoDisciplina == null) {
            tipoDisciplina = new ArrayList<ItemTipoDisciplina>();
        }
        return this.tipoDisciplina;
    }

}
