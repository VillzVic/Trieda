package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;

/**
 * <p>Java class for ItemDisciplina complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemEquivalencia">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="id" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="disciplinaCursouId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="disciplinaEliminaId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="geral" type="{http://www.w3.org/2001/XMLSchema}boolean"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemEquivalencia", propOrder = {
    "id",
    "disciplinaCursouId",
    "disciplinaEliminaId",
    "geral"
})
public class ItemEquivalencia {

    protected int id;
    @XmlElement(required = true)
    protected int disciplinaCursouId;
    @XmlElement(required = true)
    protected int disciplinaEliminaId;
    @XmlElement(required = true)
    protected Boolean geral;

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
     * Gets the value of the disciplinaCursouId property.
     * 
     * @return
     *     possible object is
     *     {@link Integer }
     *     
     */
    public int getDisciplinaCursouId() {
        return disciplinaCursouId;
    }

    /**
     * Sets the value of the disciplinaCursouId property.
     * 
     * @param value
     *     allowed object is
     *     {@link Integer }
     *     
     */
    public void setDisciplinaCursouId(int value) {
        this.disciplinaCursouId = value;
    }
    
    /**
     * Gets the value of the disciplinaEliminaId property.
     * 
     * @return
     *     possible object is
     *     {@link Integer }
     *     
     */
    public int getDisciplinaEliminaId() {
        return disciplinaEliminaId;
    }

    /**
     * Sets the value of the disciplinaEliminaId property.
     * 
     * @param value
     *     allowed object is
     *     {@link Integer }
     *     
     */
    public void setDisciplinaEliminaId(int value) {
        this.disciplinaEliminaId = value;
    }
    
    /**
     * Gets the value of the geral property.
     * 
     * @return
     *     possible object is
     *     {@link Integer }
     *     
     */
    public Boolean getGeral() {
        return geral;
    }

    /**
     * Sets the value of the geral property.
     * 
     * @param value
     *     allowed object is
     *     {@link Integer }
     *     
     */
    public void setGeral(Boolean value) {
        this.geral = value;
    }

}
