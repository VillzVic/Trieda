//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, v2.2.4 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.11.08 at 11:18:45 AM BRST 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemDisciplinaPeriodo complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemDisciplinaPeriodo">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="periodo" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="disciplinaId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *          &lt;element name="disciplinasCoRequisito" type="{}GrupoDisciplinaCoRequisito"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemDisciplinaPeriodo", propOrder = {
    "periodo",
    "disciplinaId",
    "disciplinasCoRequisito"
})
public class ItemDisciplinaPeriodo {

    protected int periodo;
    protected int disciplinaId;
    
    protected GrupoDisciplinaCoRequisito disciplinasCoRequisito;


	/**
     * Gets the value of the periodo property.
     * 
     */
    public int getPeriodo() {
        return periodo;
    }

    /**
     * Sets the value of the periodo property.
     * 
     */
    public void setPeriodo(int value) {
        this.periodo = value;
    }

    /**
     * Gets the value of the disciplinaId property.
     * 
     */
    public int getDisciplinaId() {
        return disciplinaId;
    }

    /**
     * Sets the value of the disciplinaId property.
     * 
     */
    public void setDisciplinaId(int value) {
        this.disciplinaId = value;
    }
    
    /**
     * Gets the value of the disciplinasCoRequisito property.
     * 
     * @return
     *     possible object is
     *     {@link GrupoDisciplinaCoRequisito }
     *     
     */
    public GrupoDisciplinaCoRequisito getDisciplinasCoRequisito() {
		return disciplinasCoRequisito;
	}

    /**
     * Sets the value of the disciplinasCoRequisito property.
     * 
     * @param value
     *     allowed object is
     *     {@link GrupoDisciplinaCoRequisito }
     *     
     */
	public void setDisciplinasCoRequisito(
			GrupoDisciplinaCoRequisito disciplinasCoRequisito) {
		this.disciplinasCoRequisito = disciplinasCoRequisito;
	}

}
