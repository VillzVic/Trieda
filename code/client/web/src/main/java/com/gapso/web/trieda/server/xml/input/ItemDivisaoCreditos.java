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
 * <p>Java class for ItemDivisaoCreditos complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemDivisaoCreditos">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="id" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="creditos" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="dia1" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="dia2" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="dia3" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="dia4" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="dia5" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="dia6" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="dia7" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemDivisaoCreditos", propOrder = {
    "id",
    "creditos",
    "dia1",
    "dia2",
    "dia3",
    "dia4",
    "dia5",
    "dia6",
    "dia7"
})
public class ItemDivisaoCreditos {

    protected int id;
    protected int creditos;
    protected int dia1;
    protected int dia2;
    protected int dia3;
    protected int dia4;
    protected int dia5;
    protected int dia6;
    protected int dia7;

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
     * Gets the value of the creditos property.
     * 
     */
    public int getCreditos() {
        return creditos;
    }

    /**
     * Sets the value of the creditos property.
     * 
     */
    public void setCreditos(int value) {
        this.creditos = value;
    }

    /**
     * Gets the value of the dia1 property.
     * 
     */
    public int getDia1() {
        return dia1;
    }

    /**
     * Sets the value of the dia1 property.
     * 
     */
    public void setDia1(int value) {
        this.dia1 = value;
    }

    /**
     * Gets the value of the dia2 property.
     * 
     */
    public int getDia2() {
        return dia2;
    }

    /**
     * Sets the value of the dia2 property.
     * 
     */
    public void setDia2(int value) {
        this.dia2 = value;
    }

    /**
     * Gets the value of the dia3 property.
     * 
     */
    public int getDia3() {
        return dia3;
    }

    /**
     * Sets the value of the dia3 property.
     * 
     */
    public void setDia3(int value) {
        this.dia3 = value;
    }

    /**
     * Gets the value of the dia4 property.
     * 
     */
    public int getDia4() {
        return dia4;
    }

    /**
     * Sets the value of the dia4 property.
     * 
     */
    public void setDia4(int value) {
        this.dia4 = value;
    }

    /**
     * Gets the value of the dia5 property.
     * 
     */
    public int getDia5() {
        return dia5;
    }

    /**
     * Sets the value of the dia5 property.
     * 
     */
    public void setDia5(int value) {
        this.dia5 = value;
    }

    /**
     * Gets the value of the dia6 property.
     * 
     */
    public int getDia6() {
        return dia6;
    }

    /**
     * Sets the value of the dia6 property.
     * 
     */
    public void setDia6(int value) {
        this.dia6 = value;
    }

    /**
     * Gets the value of the dia7 property.
     * 
     */
    public int getDia7() {
        return dia7;
    }

    /**
     * Sets the value of the dia7 property.
     * 
     */
    public void setDia7(int value) {
        this.dia7 = value;
    }

}