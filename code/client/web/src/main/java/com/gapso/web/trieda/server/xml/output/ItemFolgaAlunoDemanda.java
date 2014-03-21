package com.gapso.web.trieda.server.xml.output;

import java.util.ArrayList;
import java.util.List;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;

/**
 * <p>Java class for ItemFolgaAlunoDemanda complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemFolgaAlunoDemanda">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="id" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="motivos" type="{}GrupoMotivos"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemFolgaAlunoDemanda", propOrder = {
    "id",
    "motivos"
})
public class ItemFolgaAlunoDemanda {
	
    protected int id;
    @XmlElement(required = true)
    protected ItemFolgaAlunoDemanda.Motivos motivos;
    
    /**
     * Gets the value of the id property.
     * 
     */
    public int getId() {
        return id;
    }

    /**
     * Sets the value of the id property.
     * 
     */
    public void setId(int value) {
        this.id = value;
    }
    
    /**
     * Gets the value of the motivos property.
     * 
     */
    public ItemFolgaAlunoDemanda.Motivos getMotivos() {
        return motivos;
    }

    /**
     * Sets the value of the motivos property.
     * 
     */
    public void setMotivos(ItemFolgaAlunoDemanda.Motivos value) {
        this.motivos = value;
    }
    
    /**
     * <p>Java class for Motivos complex type.
     * 
     * <p>The following schema fragment specifies the expected content contained within this class.
     * 
     * <pre>
     * &lt;complexType name="Motivos">
     *   &lt;complexContent>
     *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
     *       &lt;sequence>
     *         &lt;element name="motivo" type="{http://www.w3.org/2001/XMLSchema}string"/>
     *       &lt;/sequence>
     *     &lt;/restriction>
     *   &lt;/complexContent>
     * &lt;/complexType>
     * </pre>
     * 
     * 
     */
    @XmlAccessorType(XmlAccessType.FIELD)
    @XmlType(name = "", propOrder = {
        "motivo"
    })
    public static class Motivos {
    	protected List<String> motivo;
    	
        /**
         * Gets the value of the descricao property.
         * 
         * <p>
         * This accessor method returns a reference to the live list,
         * not a snapshot. Therefore any modification you make to the
         * returned list will be present inside the JAXB object.
         * This is why there is not a <CODE>set</CODE> method for the alteracao property.
         * 
         * <p>
         * For example, to add a new item, do as follows:
         * <pre>
         *    getDescricao().add(newItem);
         * </pre>
         * 
         * 
         * <p>
         * Objects of the following type(s) are allowed in the list
         * {@link String }
         * 
         * 
         */
        public List<String> getMotivo() {
            if (motivo == null) {
            	motivo = new ArrayList<String>();
            }
            return this.motivo;
        }
    }
}
