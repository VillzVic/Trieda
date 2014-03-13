package com.gapso.web.trieda.server.xml.output;

import java.util.ArrayList;
import java.util.List;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;

/**
 * <p>Java class for GrupoFolgaAlunoDemanda complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="GrupoFolgaAlunoDemanda">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence maxOccurs="unbounded" minOccurs="0">
 *         &lt;element name="FolgaAlunoDemanda" type="{}ItemFolgaAlunoDemanda"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "GrupoFolgaAlunoDemanda", propOrder = {
    "folgaAlunoDemanda"
})
public class GrupoFolgaAlunoDemanda {

    @XmlElement(name = "FolgaAlunoDemanda")
    protected List<ItemFolgaAlunoDemanda> folgaAlunoDemanda;

    /**
     * Gets the value of the folgaAlunoDemanda property.
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
     *    getFolgaAlunoDemanda().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link ItemFolgaAlunoDemanda }
     * 
     * 
     */
    public List<ItemFolgaAlunoDemanda> getFolgaAlunoDemanda() {
        if (folgaAlunoDemanda == null) {
        	folgaAlunoDemanda = new ArrayList<ItemFolgaAlunoDemanda>();
        }
        return this.folgaAlunoDemanda;
    }

}
