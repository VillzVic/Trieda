package com.gapso.web.trieda.server.xml.output;

import java.util.ArrayList;
import java.util.List;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;

/**
 * <p>Java class for GrupoAlocacoes complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="GrupoAlocacoes">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="Alocacao" type="{}ItemAlocacao"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "GrupoAlocacoes", propOrder = {
    "alocacao"
})
public class GrupoAlocacoes {

    @XmlElement(name = "Alocacao")
    protected List<ItemAlocacao> alocacao;
    
    /**
     * Gets the value of the alocacao property.
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
     *    getAlocacao().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link ItemAlocacao }
     * 
     * 
     */
    public List<ItemAlocacao> getAlocacao() {
        if (alocacao == null) {
        	alocacao = new ArrayList<ItemAlocacao>();
        }
        return this.alocacao;
    }

    
}
