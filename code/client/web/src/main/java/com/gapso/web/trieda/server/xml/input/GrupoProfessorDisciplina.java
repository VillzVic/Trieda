//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, v2.2.2-hudson-jaxb-ri-2.2-63- 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.04.04 at 10:04:18 AM BRT 
//


package com.gapso.web.trieda.server.xml.input;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for GrupoProfessorDisciplina complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="GrupoProfessorDisciplina">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence maxOccurs="unbounded" minOccurs="0">
 *         &lt;element name="ProfessorDisciplina" type="{}ItemProfessorDisciplina"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "GrupoProfessorDisciplina", propOrder = {
    "professorDisciplina"
})
public class GrupoProfessorDisciplina {

    @XmlElement(name = "ProfessorDisciplina")
    protected List<ItemProfessorDisciplina> professorDisciplina;

    /**
     * Gets the value of the professorDisciplina property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the professorDisciplina property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getProfessorDisciplina().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link ItemProfessorDisciplina }
     * 
     * 
     */
    public List<ItemProfessorDisciplina> getProfessorDisciplina() {
        if (professorDisciplina == null) {
            professorDisciplina = new ArrayList<ItemProfessorDisciplina>();
        }
        return this.professorDisciplina;
    }

}
