package com.gapso.trieda.domain;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.Version;
import javax.validation.constraints.Digits;
import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "DEC_ID")
@Table(name = "DESLOCAMENTOS_CAMPI")
public class DeslocamentoCampus
	implements java.io.Serializable, Clonable< DeslocamentoCampus >
{
	private static final long serialVersionUID = -7559927105823983148L;

    @NotNull
    @ManyToOne(targetEntity = Campus.class, fetch = FetchType.LAZY)
    @JoinColumn(name = "CAM_ORIG_ID")
    private Campus origem;

    @NotNull
    @ManyToOne(targetEntity = Campus.class, fetch = FetchType.LAZY)
    @JoinColumn(name = "CAM_DEST_ID")
    private Campus destino;

    @Column(name = "DEC_TEMPO")
    @Min(0L)
    @Max(999L)
    private Integer tempo;

    @Column(name = "DEC_CUSTO")
    @Digits(integer = 4, fraction = 2)
    private Double custo;

	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: " ).append( getId() ).append( ", " );
        sb.append( "Version: " ).append( getVersion() ).append( ", " );
        sb.append( "Origem: " ).append( getOrigem() ).append( ", " );
        sb.append( "Destino: " ).append( getDestino() ).append( ", " );
        sb.append( "Tempo: " ).append( getTempo() ).append( ", " );
        sb.append( "Custo: " ).append( getCusto() );

        return sb.toString();
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "DEC_ID")
    private Long id;

	@Version
    @Column(name = "version")
    private Integer version;

	public Long getId() {
        return this.id;
    }

	public void setId(Long id) {
        this.id = id;
    }

	public Integer getVersion() {
        return this.version;
    }

	public void setVersion(Integer version) {
        this.version = version;
    }

	@Transactional
	public void detach() {
		if (this.entityManager == null) this.entityManager = entityManager();
		this.entityManager.detach(this);
	}
	
	@Transactional
    public void persist() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.persist(this);
    }

	@Transactional
    public void remove() {
        if (this.entityManager == null) this.entityManager = entityManager();
        if (this.entityManager.contains(this)) {
            this.entityManager.remove(this);
        } else {
            DeslocamentoCampus attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public DeslocamentoCampus merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        DeslocamentoCampus merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager()
	{
        EntityManager em = new DeslocamentoCampus().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		"Entity manager has not been injected (is the Spring " +
        		"Aspects JAR configured as an AJC/AJDT aspects library?)" );
        }

        return em;
    }

	public static long countDeslocamentoCampuses(
		InstituicaoEnsino instituicaoEnsino )
	{
		return findAllDeslocamentoCampuses( instituicaoEnsino ).size();
    }

	@SuppressWarnings( "unchecked" )
    public static List< DeslocamentoCampus > findAllDeslocamentoCampuses(
    	InstituicaoEnsino instituicaoEnsino )
    {
		Query q = entityManager().createQuery(
	        " SELECT o FROM DeslocamentoCampus o " +
	        " WHERE o.origem.instituicaoEnsino = :instituicaoEnsino " +
	        " AND o.destino.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        return q.getResultList();
    }
	
	@SuppressWarnings( "unchecked" )
    public static List< DeslocamentoCampus > findByCenario(
    	InstituicaoEnsino instituicaoEnsino, Cenario cenario )
    {
		Query q = entityManager().createQuery(
	        " SELECT o FROM DeslocamentoCampus o " +
	        " WHERE o.origem.instituicaoEnsino = :instituicaoEnsino " +
	        " AND o.destino.instituicaoEnsino = :instituicaoEnsino " +
	        " AND o.destino.cenario = :cenario " +
	        " AND o.origem.cenario = :cenario " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

        return q.getResultList();
    }

	public static DeslocamentoCampus findDeslocamentoCampus(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
        if ( id == null || instituicaoEnsino == null )
        {
        	return null;
        }

        DeslocamentoCampus dc = entityManager().find( DeslocamentoCampus.class, id );

        if ( dc != null && dc.getOrigem() != null && dc.getDestino() != null
        	&& dc.getOrigem().getInstituicaoEnsino() != null
        	&& dc.getDestino().getInstituicaoEnsino() != null
        	&& dc.getOrigem().getInstituicaoEnsino() == instituicaoEnsino
        	&& dc.getDestino().getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return dc;
        }
        
        return null;
    }

	@SuppressWarnings("unchecked")
    public static List< DeslocamentoCampus > findDeslocamentoCampusEntries(
    	InstituicaoEnsino instituicaoEnsino, int firstResult, int maxResults )
    {
        return entityManager().createQuery(
        	" SELECT o FROM DeslocamentoCampus o " +
        	" WHERE o.origem.instituicaoEnsino = :instituicaoEnsino " +
        	" AND o.destino.instituicaoEnsino = :instituicaoEnsino " )
        	.setParameter( "instituicaoEnsino", instituicaoEnsino )
        	.setFirstResult( firstResult ).setMaxResults( maxResults ).getResultList();
    }

	@SuppressWarnings("unchecked")
    public static List< DeslocamentoCampus > findAllByCampus(
    	InstituicaoEnsino instituicaoEnsino, Cenario cenario )
    {
		Query q = entityManager().createQuery(
			" SELECT o FROM DeslocamentoCampus o " +
			" WHERE o.origem.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.destino.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.origem.cenario = :cenario " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        return q.getResultList();
    }
	
	public static Map<String, Double> criaMapDeslocamentosEntreCampi(List<DeslocamentoCampus> deslocamentosCampi) {
		Map<String, Double> origemDestinoCampusToTempoDeslocamentoMap = new HashMap<String, Double>();
		for (DeslocamentoCampus deslocCampi : deslocamentosCampi) {
			Campus camOrig = deslocCampi.getOrigem();
			Campus camDest = deslocCampi.getDestino();
			Integer tempoMinutos = deslocCampi.getTempo();
			String chaveOrigDestCampus = CriaChaveDeslocamentoCampus(camOrig,camDest);
			origemDestinoCampusToTempoDeslocamentoMap.put(chaveOrigDestCampus, (double)tempoMinutos);
		}
		return origemDestinoCampusToTempoDeslocamentoMap;
	}
	
	public static String CriaChaveDeslocamentoCampus(Campus camOrig, Campus camDest) {
		return camOrig.getId() + "->" + camDest.getId();
	}
	
	public static String CriaChaveDeslocamentoCampus(Long camOrigId, Long camDestId) {
		return camOrigId + "->" + camDestId;
	}

	public Campus getOrigem()
	{
        return this.origem;
    }

	public void setOrigem( Campus origem )
	{
        this.origem = origem;
    }

	public Campus getDestino()
	{
        return this.destino;
    }

	public void setDestino( Campus destino )
	{
        this.destino = destino;
    }

	public Integer getTempo()
	{
        return this.tempo;
    }

	public void setTempo( Integer tempo )
	{
        this.tempo = tempo;
    }

	public Double getCusto()
	{
        return this.custo;
    }

	public void setCusto( Double custo )
	{
        this.custo = custo;
    }

	public DeslocamentoCampus clone(CenarioClone novoCenario) {
		DeslocamentoCampus clone = new DeslocamentoCampus();
		clone.setCusto(this.getCusto());
		clone.setTempo(this.getTempo());
		clone.setOrigem(novoCenario.getEntidadeClonada(this.getOrigem()));
		clone.setDestino(novoCenario.getEntidadeClonada(this.getDestino()));
		
		return clone;
	}

	public void cloneChilds(CenarioClone novoCenario,
			DeslocamentoCampus entidadeClone) {
		
	}
}
