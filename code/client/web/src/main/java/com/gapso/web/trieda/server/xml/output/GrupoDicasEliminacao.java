package com.gapso.web.trieda.server.xml.output;

import java.util.ArrayList;
import java.util.List;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;

/**
 * <p>Java class for GrupoDicasEliminacao complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="GrupoDicasEliminacao">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="dicaEliminacao" type="{}ItemDicaEliminacao"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "GrupoDicasEliminacao", propOrder = {
    "dicaEliminacao"
})
public class GrupoDicasEliminacao {
    @XmlElement(required = true)
    protected List<ItemDicaEliminacao> dicaEliminacao;
    
    public List<ItemDicaEliminacao> getDicaEliminacao() {
        if (dicaEliminacao == null) {
        	dicaEliminacao = new ArrayList<ItemDicaEliminacao>();
        }
        return this.dicaEliminacao;
    }
}
