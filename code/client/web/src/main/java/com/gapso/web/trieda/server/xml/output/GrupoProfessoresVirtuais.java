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
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for GrupoProfessoresVirtuais complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="GrupoProfessoresVirtuais">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence maxOccurs="unbounded" minOccurs="0">
 *         &lt;element name="ProfessorVirtual" type="{}ItemProfessorVirtual"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "GrupoProfessoresVirtuais", propOrder = {
    "professorVirtual"
})
public class GrupoProfessoresVirtuais {

    @XmlElement(name = "ProfessorVirtual")
    protected List<ItemProfessorVirtual> professorVirtual;

    /**
     * Gets the value of the professorVirtual property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the professorVirtual property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getProfessorVirtual().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link ItemProfessorVirtual }
     * 
     * 
     */
    public List<ItemProfessorVirtual> getProfessorVirtual() {
        if (professorVirtual == null) {
            professorVirtual = new ArrayList<ItemProfessorVirtual>();
        }
        return this.professorVirtual;
    }

}
