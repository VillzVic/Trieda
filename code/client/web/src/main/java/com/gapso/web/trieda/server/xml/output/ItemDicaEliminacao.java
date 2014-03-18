package com.gapso.web.trieda.server.xml.output;

import java.util.ArrayList;
import java.util.List;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;

/**
 * <p>Java class for ItemDicaEliminacao complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemDicaEliminacao">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="id" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="alteracoesNecessarias" type="{http://www.w3.org/2001/XMLSchema}AlteracoesNecessarias"/>
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
    "alteracoesNecessarias"
})
public class ItemDicaEliminacao {

    @XmlElement(required = true)
    protected int profRealId;
    @XmlElement(required = true)
    protected ItemDicaEliminacao.AlteracoesNecessarias alteracoesNecessarias;
    
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
     * Gets the value of the alteracoesNecessarias property.
     * 
     * @return
     *     possible object is
     *     {@link ItemDicaEliminacao.AlteracoesNecessarias }
     *     
     */
    public ItemDicaEliminacao.AlteracoesNecessarias getAlteracoesNecessarias() {
        return alteracoesNecessarias;
    }

    /**
     * Sets the value of the alteracoesNecessarias property.
     * 
     * @param value
     *     allowed object is
     *     {@link ItemDicaEliminacao.AlteracoesNecessarias }
     *     
     */
    public void setAlteracoesNecessarias(ItemDicaEliminacao.AlteracoesNecessarias value) {
        this.alteracoesNecessarias = value;
    }
    
    
    /**
     * <p>Java class for AlteracoesNecessarias complex type.
     * 
     * <p>The following schema fragment specifies the expected content contained within this class.
     * 
     * <pre>
     * &lt;complexType name="AlteracoesNecessarias">
     *   &lt;complexContent>
     *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
     *       &lt;sequence>
     *         &lt;element name="alteracao" type="{http://www.w3.org/2001/XMLSchema}string"/>
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
        "alteracao"
    })
    public static class AlteracoesNecessarias {
    	protected List<String> alteracao;
    	
        /**
         * Gets the value of the alteracao property.
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
         *    getAlteracao().add(newItem);
         * </pre>
         * 
         * 
         * <p>
         * Objects of the following type(s) are allowed in the list
         * {@link String }
         * 
         * 
         */
        public List<String> getAlteracao() {
            if (alteracao == null) {
            	alteracao = new ArrayList<String>();
            }
            return this.alteracao;
        }
    }
}
