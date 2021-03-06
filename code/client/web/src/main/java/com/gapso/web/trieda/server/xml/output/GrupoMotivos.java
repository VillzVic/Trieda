package com.gapso.web.trieda.server.xml.output;

import java.util.ArrayList;
import java.util.List;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;

/**
 * <p>Java class for GrupoMotivos complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="GrupoMotivos">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="motivo" type="{}ItemMotivoDeUso"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "GrupoMotivos", propOrder = {
    "motivo"
})
public class GrupoMotivos {

    @XmlElement(required = true)
    protected List<ItemMotivoDeUso> motivo;
    
    public List<ItemMotivoDeUso> getMotivo() {
        if (motivo == null) {
        	motivo = new ArrayList<ItemMotivoDeUso>();
        }
        return this.motivo;
    }
}
