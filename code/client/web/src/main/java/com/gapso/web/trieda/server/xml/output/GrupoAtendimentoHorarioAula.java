//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, v2.2.4 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.09.13 at 09:56:26 AM BRT 
//


package com.gapso.web.trieda.server.xml.output;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for GrupoAtendimentoHorarioAula complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="GrupoAtendimentoHorarioAula">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence maxOccurs="unbounded" minOccurs="0">
 *         &lt;element name="AtendimentoHorarioAula" type="{}ItemAtendimentoHorarioAula"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "GrupoAtendimentoHorarioAula", propOrder = {
    "atendimentoHorarioAula"
})
public class GrupoAtendimentoHorarioAula {

    @XmlElement(name = "AtendimentoHorarioAula")
    protected List<ItemAtendimentoHorarioAula> atendimentoHorarioAula;

    /**
     * Gets the value of the atendimentoHorarioAula property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the atendimentoHorarioAula property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getAtendimentoHorarioAula().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link ItemAtendimentoHorarioAula }
     * 
     * 
     */
    public List<ItemAtendimentoHorarioAula> getAtendimentoHorarioAula() {
        if (atendimentoHorarioAula == null) {
            atendimentoHorarioAula = new ArrayList<ItemAtendimentoHorarioAula>();
        }
        return this.atendimentoHorarioAula;
    }

}
