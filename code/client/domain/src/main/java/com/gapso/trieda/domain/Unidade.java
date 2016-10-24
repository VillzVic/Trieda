package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.UniqueConstraint;
import javax.persistence.Version;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.misc.Semanas;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity( identifierColumn = "UNI_ID" )
@Table( name = "UNIDADES", uniqueConstraints =
@UniqueConstraint( columnNames = { "CAM_ID", "UNI_CODIGO" } ) )
public class Unidade implements Serializable, Clonable< Unidade >
{
    private static final long serialVersionUID = -5763084706316974453L;

    @NotNull
    @ManyToOne( targetEntity = Campus.class )
    @JoinColumn( name = "CAM_ID" )
    private Campus campus;

    @NotNull
    @Column( name = "UNI_CODIGO" )
    @Size( min = 1, max = 20 )
    private String codigo;

    @NotNull
    @Column( name = "UNI_NOME" )
    @Size( min = 1, max = 500 )
    private String nome;

    @OneToMany( cascade = CascadeType.ALL, mappedBy = "origem" )
    private Set< DeslocamentoUnidade > deslocamentos = new HashSet< DeslocamentoUnidade >();

/*    @ManyToMany( cascade = { CascadeType.PERSIST, CascadeType.MERGE }, mappedBy = "unidades" )
    private Set< HorarioDisponivelCenario > horarios = new HashSet< HorarioDisponivelCenario >();*/
    
    @ManyToMany
	private Set< Fixacao > fixacoes = new HashSet< Fixacao >();

    @OneToMany( cascade = CascadeType.ALL, mappedBy="unidade" )
    private Set< GrupoSala > gruposSalas = new HashSet< GrupoSala >();

    @OneToMany( cascade = CascadeType.ALL, mappedBy="unidade" )
    private Set< Sala > salas = new HashSet< Sala >();

    @PersistenceContext
    transient EntityManager entityManager;

    @Id
    @GeneratedValue( strategy = GenerationType.AUTO )
    @Column( name = "UNI_ID" )
    private Long id;

    @Version
    @Column( name = "version" )
    private Integer version;

    public Long getId()
    {
        return this.id;
    }

    public void setId( Long id )
    {
        this.id = id;
    }

    public Integer getVersion()
    {
        return this.version;
    }

    public void setVersion( Integer version )
    {
        this.version = version;
    }
    
    public Set<Sala> getLaboratorios() {
		Set<Sala> laboratorios = new HashSet<Sala>();
		for (Sala sala : this.salas) {
			if (sala.isLaboratorio()) {
				laboratorios.add(sala);
			}
		}
		return laboratorios;
	}

	@Transactional
	public void detach()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		this.entityManager.detach( this );
	}
	
	@Transactional
    public void persist()
    {
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        this.entityManager.persist( this );
    }
    
    @Transactional
    public void persistAndPreencheHorarios()
    {
    	persist();
        preencheHorarios();
    }
    
    @Transactional
	static public void preencheHorariosDasUnidades(List<Unidade> unidades, List<SemanaLetiva> semanasLetivas) {
/*		for (SemanaLetiva semanaLetiva : semanasLetivas) {
			for (HorarioAula horarioAula : semanaLetiva.getHorariosAula()) {
				for (HorarioDisponivelCenario hdc : horarioAula.getHorariosDisponiveisCenario()) {
					hdc.getUnidades().addAll(unidades);
					hdc.merge();
				}
			}
		}*/
	}

    @Transactional
    public void remove()
    {
    	this.remove( true );
    }

    @Transactional
    public void remove( boolean removeCurriculosDisciplinas )
    {
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        if ( this.entityManager.contains( this ) )
        {

        	if ( removeCurriculosDisciplinas )
        	{
        		removeCurriculoDisciplinas();
        	}

        	this.removeSalas( false );
        	this.removeDeslocamentos();

            this.entityManager.remove( this );
        }
        else
        {
            Unidade attached = this.entityManager.find(
            	this.getClass(), this.id );

			if ( attached != null )
			{

	        	if ( removeCurriculosDisciplinas )
	        	{
	        		removeCurriculoDisciplinas();
	        	}

            	attached.removeSalas( false );
            	attached.removeDeslocamentos();

            	this.entityManager.remove( attached );
			}
        }
    }

    @Transactional
    private void removeCurriculoDisciplinas()
    {
    	Set< CurriculoDisciplina > curriculoDisciplinas
    		= new HashSet< CurriculoDisciplina >();

    	List< Sala > listSalas = Sala.findByUnidade(
    		this.getCampus().getInstituicaoEnsino(), this );

    	for ( Sala sala : listSalas )
    	{
    		List< CurriculoDisciplina > listCurriculosDisciplinas
    			= CurriculoDisciplina.findBySala(
    				this.getCampus().getInstituicaoEnsino(), sala );

			for ( CurriculoDisciplina cd : listCurriculosDisciplinas )
			{
				curriculoDisciplinas.add( cd );
			}
    	}

    	for ( CurriculoDisciplina cd : curriculoDisciplinas )
    	{
    		for ( Sala s : this.getSalas() )
    		{
    			cd.getDisciplina().getSalas().remove( s );
    			cd.merge();
    		}
    	}
    }

	@Transactional
	public void preencheHorarios()
	{
		InstituicaoEnsino instituicaoEnsino
			= this.getCampus().getInstituicaoEnsino();

		List< SemanaLetiva > listDomains
			= SemanaLetiva.findByCenario( instituicaoEnsino, this.getCampus().getCenario() );

		//Cria estrutura de dados inicial com mapeamento de horarios de todas as semanas letivas para cada dia da semana
		Map<Semanas, Map<Date, Integer>> diaSemanaMapHorarioInicioMapHorarioFim = new HashMap<Semanas, Map<Date, Integer>>();
		for ( SemanaLetiva semanaLetiva : listDomains )
		{
			for ( HorarioAula ha : semanaLetiva.getHorariosAula() )
			{
				for ( HorarioDisponivelCenario hdc : ha.getHorariosDisponiveisCenario() )
				{
					if (diaSemanaMapHorarioInicioMapHorarioFim.get(hdc.getDiaSemana()) == null)
					{
						Map<Date, Integer> horaInicioMapHoraFim = new HashMap<Date, Integer>();
						horaInicioMapHoraFim.put(ha.getHorario(), semanaLetiva.getTempo());
						diaSemanaMapHorarioInicioMapHorarioFim.put(hdc.getDiaSemana(), horaInicioMapHoraFim);
					}
					else
					{
						if (diaSemanaMapHorarioInicioMapHorarioFim.get(hdc.getDiaSemana()).keySet().contains(ha.getHorario()))
						{
							if (diaSemanaMapHorarioInicioMapHorarioFim.get(hdc.getDiaSemana()).get(ha.getHorario()) < semanaLetiva.getTempo())
								diaSemanaMapHorarioInicioMapHorarioFim.get(hdc.getDiaSemana()).put(ha.getHorario(), semanaLetiva.getTempo());
						}
						else
						{
							diaSemanaMapHorarioInicioMapHorarioFim.get(hdc.getDiaSemana()).put(ha.getHorario(), semanaLetiva.getTempo());
						}
					}
				}
			}
		}
		//Cria a segunda estrutura de dados com os horarios concatenados para cada dia da semana
		Map<Semanas, Map<Date, Date>> diaSemanaMapHorarioInicioMapHorarioFimConcatenado = new HashMap<Semanas, Map<Date, Date>>();
		Set<Date> todosHorarios = new HashSet<Date>();
		for (Semanas diaSemana : diaSemanaMapHorarioInicioMapHorarioFim.keySet())
		{
			Map<Date, Date> horarioInicioHorarioFimMap = new HashMap<Date, Date>();
			List<Date> horariosOrdenados = new ArrayList<Date>();
			horariosOrdenados.addAll(diaSemanaMapHorarioInicioMapHorarioFim.get(diaSemana).keySet());
			Collections.sort(horariosOrdenados);
			
			Date primeiroHorario = horariosOrdenados.get(0);
			Calendar horaFim = Calendar.getInstance();
			horaFim.setTime(horariosOrdenados.get(0));
			horaFim.set(1979,Calendar.NOVEMBER,6);
			horaFim.add(Calendar.MINUTE,diaSemanaMapHorarioInicioMapHorarioFim.get(diaSemana).get(horariosOrdenados.get(0)));
			for (int i = 1; i < horariosOrdenados.size(); i++)
			{
				Calendar horaInicio = Calendar.getInstance();
				horaInicio.setTime(horariosOrdenados.get(i));
				horaInicio.set(1979,Calendar.NOVEMBER,6);
				
				if (horaFim.compareTo(horaInicio) >= 0)
				{
					horaFim.setTime(horariosOrdenados.get(i));
					horaFim.set(1979,Calendar.NOVEMBER,6);
					horaFim.add(Calendar.MINUTE,diaSemanaMapHorarioInicioMapHorarioFim.get(diaSemana).get(horariosOrdenados.get(i)));
				}
				else
				{
					horarioInicioHorarioFimMap.put(primeiroHorario, horaFim.getTime());
					primeiroHorario = horariosOrdenados.get(i);
					horaFim = Calendar.getInstance();
					horaFim.setTime(horariosOrdenados.get(i));
					horaFim.set(1979,Calendar.NOVEMBER,6);
					horaFim.add(Calendar.MINUTE,diaSemanaMapHorarioInicioMapHorarioFim.get(diaSemana).get(horariosOrdenados.get(i)));
				}
			}
			horarioInicioHorarioFimMap.put(primeiroHorario, horaFim.getTime());
			todosHorarios.addAll(horarioInicioHorarioFimMap.values());
			todosHorarios.addAll(horarioInicioHorarioFimMap.keySet());
			diaSemanaMapHorarioInicioMapHorarioFimConcatenado.put(diaSemana, horarioInicioHorarioFimMap);
		}
		
		//Cria a ultima estrutura de dados juntando os dias da semana. Para juntar os dias da semana os horarios concatenados
		//deverao ser quebrados caso exista diferenca entre os dias da semana
		List<Date> todosHorariosOrdenados = new ArrayList<Date>();
		todosHorariosOrdenados.addAll(todosHorarios);
		Collections.sort(todosHorariosOrdenados, new Comparator<Date>() {
		    public int compare(Date o1, Date o2) {
				Calendar horaInicio = Calendar.getInstance();
				horaInicio.setTime(o1);
				horaInicio.set(1979,Calendar.NOVEMBER,6);
		    	
				Calendar horaFim = Calendar.getInstance();
				horaFim.setTime(o2);
				horaFim.set(1979,Calendar.NOVEMBER,6);
				
				return horaInicio.compareTo(horaFim);
		    }
		});
		//DateFormat df = new SimpleDateFormat("HH:mm");
		for (int i = 1; i < todosHorariosOrdenados.size() ; i++)
		{
			DisponibilidadeUnidade disponibilidade = new DisponibilidadeUnidade();
			disponibilidade.setHorarioInicio(todosHorariosOrdenados.get(i-1));
			disponibilidade.setHorarioFim(todosHorariosOrdenados.get(i));
			disponibilidade.setUnidade(this);
			disponibilidade.setSegunda(false);
			disponibilidade.setTerca(false);
			disponibilidade.setQuarta(false);
			disponibilidade.setQuinta(false);
			disponibilidade.setSexta(false);
			disponibilidade.setSabado(false);
			disponibilidade.setDomingo(false);
			boolean nenhumaDisponibilidade = true;
			for (Semanas diaSemana : Semanas.values())
			{
				if (estaContidoEm(
						todosHorariosOrdenados.get(i-1), todosHorariosOrdenados.get(i), diaSemanaMapHorarioInicioMapHorarioFimConcatenado.get(diaSemana)))
				{
					switch(diaSemana)
					{
					case SEG:
						disponibilidade.setSegunda(true);
						nenhumaDisponibilidade = false;
						break;
					case TER:
						disponibilidade.setTerca(true);
						nenhumaDisponibilidade = false;
						break;
					case QUA:
						disponibilidade.setQuarta(true);
						nenhumaDisponibilidade = false;
						break;
					case QUI:
						disponibilidade.setQuinta(true);
						nenhumaDisponibilidade = false;
						break;
					case SEX:
						disponibilidade.setSexta(true);
						nenhumaDisponibilidade = false;
						break;
					case SAB:
						disponibilidade.setSabado(true);
						nenhumaDisponibilidade = false;
						break;
					case DOM:
						disponibilidade.setDomingo(true);
						nenhumaDisponibilidade = false;
						break;
					default:
						break;
					
					}
				}
			}
			if (!nenhumaDisponibilidade)
				disponibilidade.persist();
		}
	}

	private boolean estaContidoEm(Date inicio, Date fim,
			Map<Date, Date> horarioInicioMapHorarioFim) {
		
		boolean estaContido = false;
		Calendar horaInicio = Calendar.getInstance();
		horaInicio.setTime(inicio);
		horaInicio.set(1979,Calendar.NOVEMBER,6);
    	
		Calendar horaFim = Calendar.getInstance();
		horaFim.setTime(fim);
		horaFim.set(1979,Calendar.NOVEMBER,6);
		if (horarioInicioMapHorarioFim == null) return false;
		for (Entry<Date, Date> horarios : horarioInicioMapHorarioFim.entrySet())
		{
			Calendar dispInicio = Calendar.getInstance();
			dispInicio.setTime(horarios.getKey());
			dispInicio.set(1979,Calendar.NOVEMBER,6);
	    	
			Calendar dispFim = Calendar.getInstance();
			dispFim.setTime(horarios.getValue());
			dispFim.set(1979,Calendar.NOVEMBER,6);
			if (dispInicio.compareTo(horaInicio) <= 0 && dispFim.compareTo(horaFim) >= 0)
			{
				estaContido = true;
			}
		}
		return estaContido;
	}

	@Transactional
	private void removeDeslocamentos()
	{
		List< DeslocamentoUnidade > deslocamentos
			= DeslocamentoUnidade.findAllByUnidade(
				this.getCampus().getInstituicaoEnsino(), this );

		for ( DeslocamentoUnidade deslocamento : deslocamentos )
		{
			deslocamento.remove();
		}
	}

	@Transactional
	private void removeSalas( boolean removeDisciplinas )
	{
		Set< Sala > salas = this.getSalas();

		for ( Sala sala : salas )
		{
			sala.remove( removeDisciplinas );
		}
	}
    
    @Transactional
    public void flush()
    {
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        this.entityManager.flush();
    }

    @Transactional
    public Unidade merge()
    {
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        Unidade merged = this.entityManager.merge( this );
        this.entityManager.flush();
        return merged;
    }

    public static final EntityManager entityManager()
    {
        EntityManager em = new Unidade().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		"Entity manager has not been injected (is the Spring " +
        		"Aspects JAR configured as an AJC/AJDT aspects library?)" );
        }

        return em;
    }

    @SuppressWarnings("unchecked")
    public static List< Unidade > findAll(
    	InstituicaoEnsino instituicaoEnsino )
    {
    	Query q = entityManager().createQuery(
    		" SELECT o FROM Unidade o " +
    		" WHERE o.campus.instituicaoEnsino = :instituicaoEnsino " );

    	q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        return q.getResultList();
    }

    @SuppressWarnings("unchecked")
    public static List< Unidade > findByCenario(
    	InstituicaoEnsino instituicaoEnsino, Cenario cenario )
    {
    	Query q = entityManager().createQuery(
    		" SELECT o FROM Unidade o " +
    		" WHERE o.campus.cenario = :cenario " +
    		" AND o.campus.instituicaoEnsino = :instituicaoEnsino " );

    	q.setParameter( "cenario", cenario );
    	q.setParameter( "instituicaoEnsino", instituicaoEnsino ); 

    	return q.getResultList();
    }

    
    public static Unidade findBy(InstituicaoEnsino instituicaoEnsino, Cenario cenario, String codigo)
    {
    	Query q = entityManager().createQuery(
    		" SELECT o FROM Unidade o " +
    		" WHERE o.campus.instituicaoEnsino = :instituicaoEnsino " +
    		" AND o.campus.cenario = :cenario " +
			" AND o.codigo = :codigo ");

    	q.setParameter("instituicaoEnsino", instituicaoEnsino);
    	q.setParameter("cenario", cenario);
    	q.setParameter("codigo", codigo);

    	return (Unidade) q.getSingleResult();
    }

    public static Map< String, Unidade > buildUnidadeCodigoToUnidadeMap(
    	List< Unidade > unidades )
    {
		Map< String, Unidade > unidadesMap
			= new HashMap< String, Unidade >();

		if ( unidades != null && unidades.size() > 0 )
		{
			for ( Unidade unidade : unidades )
			{
				unidadesMap.put( unidade.getCodigo(), unidade );
			}
		}

		return unidadesMap;
	}
    
    public static Map< String, Unidade > buildUnidadeNomeToUnidadeMap(
        	List< Unidade > unidades )
        {
    		Map< String, Unidade > unidadesMap
    			= new HashMap< String, Unidade >();

    		if ( unidades != null && unidades.size() > 0 )
    		{
    			for ( Unidade unidade : unidades )
    			{
    				unidadesMap.put( unidade.getNome(), unidade );
    			}
    		}

    		return unidadesMap;
    	}

    public static Unidade find( Long id, InstituicaoEnsino instituicaoEnsino )
    {
        if ( id == null && instituicaoEnsino == null )
        {
        	return null;
        }

        Unidade u = entityManager().find( Unidade.class, id );
        if ( u != null
        	&& u.getCampus().getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return u;
        }

        return null;
    }

    public static List< Unidade > find( InstituicaoEnsino instituicaoEnsino,
    	int firstResult, int maxResults )
    {
        return find( instituicaoEnsino, firstResult, maxResults, null );
    }

    @SuppressWarnings("unchecked")
    public static List< Unidade > find( InstituicaoEnsino instituicaoEnsino,
    	int firstResult, int maxResults, String orderBy )
    {
        orderBy = ( ( orderBy != null ) ? "ORDER BY o." + orderBy : "" );

        Query q = entityManager().createQuery(
            " SELECT o FROM Unidade o " +
            " WHERE o.campus.instituicaoEnsino = :instituicaoEnsino " + orderBy );

        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setFirstResult( firstResult );
        q.setMaxResults( maxResults );

        return q.getResultList();
    }

    public static Integer getCapacidadeMedia(
    	InstituicaoEnsino instituicaoEnsino, Unidade unidade )
    {
    	Query q =  entityManager().createQuery(
    		" SELECT AVG ( o.capacidadeInstalada ) FROM Sala o " +
    		" WHERE o.unidade = :unidade " +
    		" AND o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " );

    	q.setParameter( "unidade", unidade );
    	q.setParameter( "instituicaoEnsino", instituicaoEnsino );

    	Object obj = q.getSingleResult();

    	if ( obj == null )
    	{
    		return 0;
    	}

    	return ( (Number) obj ).intValue();
    }

    @SuppressWarnings("unchecked")
    public static List< Unidade > findByCampus(
    	InstituicaoEnsino instituicaoEnsino, Campus campus )
    {
    	Query q = entityManager().createQuery(
    		" SELECT Unidade FROM Unidade AS unidade " +
    		" WHERE unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
    		" AND unidade.campus = :campus ORDER BY unidade.nome ASC " );

		q.setParameter( "campus", campus );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

    	return q.getResultList();
    }

    @SuppressWarnings("unchecked")
    public static List< Unidade > findByTurmaOtimizada(
    	InstituicaoEnsino instituicaoEnsino, String turma )
    {
    	Query q = entityManager().createQuery(
    		" SELECT DISTINCT o FROM Unidade o " +
    		" JOIN o.salas s JOIN s.atendimentosOperacionais a " +
    		" WHERE o.campus.instituicaoEnsino = :instituicaoEnsino " +
    		" AND a.turma = :turma ORDER BY o.nome ASC " );

		q.setParameter( "turma", turma );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

    	return q.getResultList();
    }
    
    public static int count( InstituicaoEnsino instituicaoEnsino,
    	Cenario cenario, Campus campus, String nome, String codigo, String operadorCapSalas, Double capSalas )
    {
        nome = ( ( nome == null ) ? "" : nome );
        nome = ( "%" + nome.replace( '*', '%' ) + "%" );
        codigo = ( ( codigo == null ) ? "" : codigo );
        codigo = ( "%" + codigo.replace( '*', '%' ) + "%" );
        
        String operadorCapSalasStr = "";
        
		if(capSalas != null){
			if(operadorCapSalas != null)
				operadorCapSalasStr = " (SELECT AVG ( s.capacidadeInstalada ) FROM Sala s " +
			    		" WHERE s.unidade = unidade " +
			    		" AND s.unidade.campus.instituicaoEnsino = :instituicaoEnsino ) " + operadorCapSalas + " :capSalas  and ";
			else
				operadorCapSalasStr = " (SELECT AVG ( s.capacidadeInstalada ) FROM Sala s " +
			    		" WHERE s.unidade = unidade " +
			    		" AND s.unidade.campus.instituicaoEnsino = :instituicaoEnsino ) = :capSalas  and ";
    	}
        String queryCampus = "";
        if ( campus != null )
        {
        	queryCampus = " unidade.campus = :campus AND ";
        }

        Query q = entityManager().createQuery(
        	" SELECT COUNT ( Unidade ) FROM Unidade AS unidade " +
        	" WHERE " + queryCampus + operadorCapSalasStr +
        	" LOWER ( unidade.nome ) LIKE LOWER ( :nome ) " +
        	" AND unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
          	" AND unidade.campus.cenario = :cenario " +
        	" AND LOWER ( unidade.codigo ) LIKE LOWER ( :codigo ) " );

        if ( campus != null )
        {
        	q.setParameter( "campus", campus );
        }
        
		if( capSalas != null){
			q.setParameter("capSalas", capSalas);
		}

        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setParameter( "cenario", cenario );
        q.setParameter( "nome", nome );
        q.setParameter( "codigo", codigo );

        return ( (Number) q.getSingleResult() ).intValue();
    }

    @SuppressWarnings("unchecked")
	public static List< Unidade > findBy(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario, Campus campus,
		String nome, String codigo, String operadorCapSalas, Double capSalas, int firstResult, int maxResults, String orderBy )
	{
        nome = ( ( nome == null )? "" : nome );
        nome = ( "%" + nome.replace( '*', '%' ) + "%" );
        codigo = ( ( codigo == null )? "" : codigo );
        codigo = ( "%" + codigo.replace( '*', '%' ) + "%" );
        
        String operadorCapSalasStr = "";
        
		if(capSalas != null){
			if(operadorCapSalas != null)
				operadorCapSalasStr = " (SELECT AVG ( s.capacidadeInstalada ) FROM Sala s " +
			    		" WHERE s.unidade = unidade " +
			    		" AND s.unidade.campus.instituicaoEnsino = :instituicaoEnsino ) " + operadorCapSalas + " :capSalas  and ";
			else
				operadorCapSalasStr = " (SELECT AVG ( s.capacidadeInstalada ) FROM Sala s " +
			    		" WHERE s.unidade = unidade " +
			    		" AND s.unidade.campus.instituicaoEnsino = :instituicaoEnsino ) = :capSalas  and ";
    	}

        if (orderBy == null) {
        	orderBy = "";
        }
        else if (orderBy.contains("capSalas")) {
        	orderBy = orderBy.replace("capSalas", "ORDER BY AVG(s.capacidadeInstalada)");
        }
        else {
        	orderBy = ( ( orderBy != null ) ? " ORDER BY unidade." + orderBy.replace("String", "") : "" );
        }
        
        String queryCampus = "";
        if ( campus != null )
        {
        	queryCampus = " unidade.campus = :campus AND ";
        }

        Query q = entityManager().createQuery(
        	"SELECT Unidade FROM Unidade AS unidade LEFT JOIN unidade.salas s" +
        	" WHERE " + queryCampus + operadorCapSalasStr +
        	" LOWER ( unidade.nome ) LIKE LOWER ( :nome ) " +
        	" AND unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
        	" AND unidade.campus.cenario = :cenario" +
        	" AND LOWER ( unidade.codigo ) LIKE LOWER ( :codigo ) GROUP BY unidade " + orderBy );

        if ( campus != null )
        {
        	q.setParameter( "campus", campus );
        }
        
		if( capSalas != null){
			q.setParameter("capSalas", capSalas);
		}


        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setParameter( "cenario", cenario );
        q.setParameter( "nome", nome );
        q.setParameter( "codigo", codigo );
        q.setFirstResult( firstResult );
        q.setMaxResults( maxResults );

        return q.getResultList();
    }

	@SuppressWarnings( "unchecked" )
	public List< HorarioDisponivelCenario > getHorarios(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM HorarioDisponivelCenario o, IN ( o.atendimentosOperacionais ) u " +
			" WHERE u.sala.unidade = :unidade " +
			" AND u.sala.unidade.campus.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "unidade", this );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList(); 
	}

	public static boolean checkCodigoUnique( 
		InstituicaoEnsino instituicaoEnsino, Cenario cenario, String codigo )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Unidade o " +
			" WHERE o.campus.cenario = :cenario " +
			" AND o.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.codigo = :codigo " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "codigo", codigo );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setMaxResults( 1 );

		Number size = (Number) q.getSingleResult();
		return ( size.intValue() > 0 );
	}

    public Campus getCampus()
    {
        return this.campus;
    }

    public void setCampus( Campus campus )
    {
        this.campus = campus;
    }

    public String getCodigo()
    {
        return this.codigo;
    }

    public void setCodigo( String codigo )
    {
        this.codigo = codigo;
    }

    public String getNome()
    {
        return this.nome;
    }

    public void setNome( String nome )
    {
        this.nome = nome;
    }

    public Set< DeslocamentoUnidade > getDeslocamentos()
    {
        return this.deslocamentos;
    }

    public void setDeslocamentos( Set< DeslocamentoUnidade > deslocamentos )
    {
        this.deslocamentos = deslocamentos;
    }

/*    private Set< HorarioDisponivelCenario > getHorarios()
    {
        return this.horarios;
    }

    public void setHorarios( Set< HorarioDisponivelCenario > horarios )
    {
        this.horarios = horarios;
    }*/
    
    

    public Set< GrupoSala > getGruposSalas()
    {
    	return this.gruposSalas;
    }

    public Set<Fixacao> getFixacoes() {
		return fixacoes;
	}

	public void setFixacoes(Set<Fixacao> fixacoes) {
		this.fixacoes = fixacoes;
	}

	public void setGruposSalas( Set< GrupoSala > gruposSalas )
    {
    	this.gruposSalas = gruposSalas;
    }

    public Set< Sala > getSalas()
    {
    	return this.salas;
    }

    public void setSalas( Set< Sala > salas )
    {
    	this.salas = salas;
    }

    public String toString()
    {
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: ").append( getId() ).append( ", " );
        sb.append( "Version: " ).append( getVersion() ).append( ", " );
        sb.append( "Campus: " ).append( getCampus() ).append( ", " );
        sb.append( "Codigo: " ).append( getCodigo() ).append( ", " );
        sb.append( "Nome: " ).append( getNome() ).append( ", " );
        sb.append( "Deslocamentos: " ).append(
        	getDeslocamentos() == null ? "null" : getDeslocamentos().size() ).append( ", " );
        sb.append( "GruposSalas: " ).append( getGruposSalas() == null ? "null" : getGruposSalas().size() );
        sb.append( "Fixações: " ).append( getFixacoes() == null ? "null" : getFixacoes().size() );
        sb.append( "Salas: ").append( getSalas() == null ? "null" : getSalas().size() );

        return sb.toString();
    }
   
	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null || !( obj instanceof Unidade ) )
		{
			return false;
		}

		Unidade other = (Unidade) obj;

		if ( this.id == null )
		{
			if ( other.id != null )
			{
				return false;
			}
		}
		else if ( !this.id.equals( other.id ) )
		{
			return false;
		}

		return true;
	}

	public Unidade clone(CenarioClone novoCenario) {
		Unidade clone = new Unidade();
		clone.setCampus(novoCenario.getEntidadeClonada(this.getCampus()));
		clone.setCodigo(this.getCodigo());
		clone.setNome(this.getNome());
		
		return clone;
	}

	public void cloneChilds(CenarioClone novoCenario, Unidade entidadeClone) {
		for (Sala sala : this.getSalas())
		{
			entidadeClone.getSalas().add(novoCenario.clone(sala));
		}
		
		for (GrupoSala grupoSala : this.getGruposSalas())
		{
			entidadeClone.getGruposSalas().add(novoCenario.clone(grupoSala));
		}
	}
	
}
