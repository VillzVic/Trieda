package com.gapso.web.trieda.server.xml.output;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlType;

/**
 * <p>Java class for ItemAlocacao complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ItemAlocacao">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="turma" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="disciplinaId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="campusId" type="{http://www.w3.org/2001/XMLSchema}int"/>
 *         &lt;element name="pratica" type="{http://www.w3.org/2001/XMLSchema}boolean"/>
 *         &lt;element name="motivosDeUso" type="{}GrupoMotivos"/>
 *         &lt;element name="dicasEliminacao" type="{}GrupoDicasEliminacao"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ItemAlocacao", propOrder = {
    "turma",
    "disciplinaId",
    "campusId",
    "pratica",
    "motivosDeUso",
    "dicasEliminacao"
})
public class ItemAlocacao {

    protected int turma;
    protected int disciplinaId;
    protected int campusId;
    protected boolean pratica;
    @XmlElement(required = true)
    protected GrupoMotivos motivosDeUso;
    @XmlElement(required = true)
    protected GrupoDicasEliminacao dicasEliminacao;
    
    /**
     * Gets the value of the turma property.
     * 
     */
    public int getTurma() {
        return turma;
    }

    /**
     * Sets the value of the turma property.
     * 
     */
    public void setTurma(int value) {
        this.turma = value;
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
     * Gets the value of the campusId property.
     * 
     */
    public int getCampusId() {
        return campusId;
    }

    /**
     * Sets the value of the campusId property.
     * 
     */
    public void setCampusId(int value) {
        this.campusId = value;
    }
    
    /**
     * Gets the value of the pratica property.
     * 
     */
    public boolean getPratica() {
        return pratica;
    }

    /**
     * Sets the value of the pratica property.
     * 
     */
    public void setPratica(boolean value) {
        this.pratica = value;
    }
    
    /**
     * Gets the value of the motivosDeUso property.
     * 
     */
    public GrupoMotivos getMotivosDeUso() {
        return motivosDeUso;
    }

    /**
     * Sets the value of the motivosDeUso property.
     * 
     */
    public void setPratica(GrupoMotivos value) {
        this.motivosDeUso = value;
    }
    
    /**
     * Gets the value of the dicasEliminacao property.
     * 
     */
    public GrupoDicasEliminacao getDicasEliminacao() {
        return dicasEliminacao;
    }

    /**
     * Sets the value of the dicasEliminacao property.
     * 
     */
    public void setDicasEliminacao(GrupoDicasEliminacao value) {
        this.dicasEliminacao = value;
    }
}
