//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, vhudson-jaxb-ri-2.1-2 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2010.12.09 at 09:57:16 AM BRST 
//


package com.gapso.web.trieda.server.xml.input;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ItemDeslocamento complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemDeslocamento">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="origemId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="destinoId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="tempo" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="custo" type="{http://www.w3.org/2001/XMLSchema}double"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemDeslocamento", propOrder = {
    "origemId",
    "destinoId",
    "tempo",
    "custo"
})
public class ItemDeslocamento {

    protected int origemId;
    protected int destinoId;
    protected int tempo;
    protected double custo;

    /**
     * Gets the value of the origemId property.
     * 
     */
    public int getOrigemId() {
        return origemId;
    }

    /**
     * Sets the value of the origemId property.
     * 
     */
    public void setOrigemId(int value) {
        this.origemId = value;
    }

    /**
     * Gets the value of the destinoId property.
     * 
     */
    public int getDestinoId() {
        return destinoId;
    }

    /**
     * Sets the value of the destinoId property.
     * 
     */
    public void setDestinoId(int value) {
        this.destinoId = value;
    }

    /**
     * Gets the value of the tempo property.
     * 
     */
    public int getTempo() {
        return tempo;
    }

    /**
     * Sets the value of the tempo property.
     * 
     */
    public void setTempo(int value) {
        this.tempo = value;
    }

    /**
     * Gets the value of the custo property.
     * 
     */
    public double getCusto() {
        return custo;
    }

    /**
     * Sets the value of the custo property.
     * 
     */
    public void setCusto(double value) {
        this.custo = value;
    }

}
