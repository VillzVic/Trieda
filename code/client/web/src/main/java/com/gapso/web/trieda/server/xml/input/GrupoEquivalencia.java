package com.gapso.web.trieda.server.xml.input;

import java.util.ArrayList;
import java.util.List;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;

/**
 * caso seja necessario, esta informacao representara a preferencia de divisao de credito desta disciplina
 * 
 * <p>Java class for GrupoEquivalencia complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="GrupoEquivalencia">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence maxOccurs="unbounded" minOccurs="0">
 *         &lt;element name="Equivalencia" type="{}ItemEquivalencia"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "GrupoEquivalencia", propOrder = {
    "equivalencia"
})
public class GrupoEquivalencia {

    @XmlElement(name = "Equivalencia")
    protected List<ItemEquivalencia> equivalencia;


    public List<ItemEquivalencia> getEquivalencia() {
        if (equivalencia == null) {
        	equivalencia = new ArrayList<ItemEquivalencia>();
        }
        return this.equivalencia;
    }

}
