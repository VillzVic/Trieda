package com.gapso.web.trieda.server.xml.output;

import java.util.ArrayList;
import java.util.List;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;

/**
 * <p>Java class for ItemMotivoDeUso complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemMotivoDeUso">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="id" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="alteracoesNecessarias" type="{http://www.w3.org/2001/XMLSchema}Descricoes"/>
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
    "profRealId",
    "descricoes"
})
public class ItemMotivoDeUso {

    protected int profRealId;
    @XmlElement(required = true)
    protected ItemMotivoDeUso.Descricoes descricoes;
    
    /**
     * Gets the value of the profRealId property.
     * 
     */
    public int getProfRealId() {
        return profRealId;
    }

    /**
     * Sets the value of the profRealId property.
     * 
     */
    public void setProfRealId(int value) {
        this.profRealId = value;
    }
    
    /**
     * Gets the value of the descricoes property.
     * 
     * @return
     *     possible object is
     *     {@link ItemMotivoDeUso.Descricoes }
     *     
     */
    public ItemMotivoDeUso.Descricoes getDescricoes() {
        return descricoes;
    }

    /**
     * Sets the value of the descricoes property.
     * 
     * @param value
     *     allowed object is
     *     {@link ItemMotivoDeUso.Descricoes }
     *     
     */
    public void setDescricoes(ItemMotivoDeUso.Descricoes value) {
        this.descricoes = value;
    }
    
    
    /**
     * <p>Java class for Descricoes complex type.
     * 
     * <p>The following schema fragment specifies the expected content contained within this class.
     * 
     * <pre>
     * &lt;complexType name="Descricoes">
     *   &lt;complexContent>
     *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
     *       &lt;sequence>
     *         &lt;element name="descricao" type="{http://www.w3.org/2001/XMLSchema}string"/>
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
        "descricao"
    })
    public static class Descricoes {
    	protected List<String> descricao;
    	
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
        public List<String> getDescricao() {
            if (descricao == null) {
            	descricao = new ArrayList<String>();
            }
            return this.descricao;
        }
    }
}
