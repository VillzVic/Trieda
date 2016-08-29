package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collection;
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
import javax.persistence.Enumerated;
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
import javax.validation.constraints.Digits;
import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.misc.Estados;
import com.gapso.trieda.misc.Semanas;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity( identifierColumn = "CAM_ID" )
@Table( name = "CAMPI", uniqueConstraints =
@UniqueConstraint( columnNames = { "CAM_CODIGO", "CEN_ID" } ) )
public class Campus
	implements Serializable, Comparable< Campus >, Clonable< Campus >
{
	private static final long serialVersionUID = 6690100103369325015L;

	@NotNull
	@ManyToOne( targetEntity = Cenario.class )
	@JoinColumn( name = "CEN_ID" )
	private Cenario cenario;

	@NotNull
	@Column( name = "CAM_CODIGO" )
	@Size( min = 1, max = 20 )
	private String codigo;

	@NotNull
	@Column( name = "CAM_NOME" )
	@Size( min = 1, max = 500 )
	private String nome;

	@Column( name = "CAM_VALOR_CREDITO" )
	@Digits( integer = 6, fraction = 2 )
	private Double valorCredito;

	@Enumerated
	@Column( name = "CAM_ESTADO" )
	private Estados estado;

	@Column( name = "CAM_MUNICIPIO" )
	@Size( max = 25 )
	private String municipio;

	@Column( name = "CAM_BAIRRO" )
	@Size( max = 25 )
	private String bairro;

	@Column( name = "CAM_PUBLICAR" )
	private Boolean publicado;
	
	@Column( name = "CAM_QTD_PROFESSOR_VIRTUAL" )
	private String qtdProfessorVirtual;

	@OneToMany( cascade = CascadeType.ALL, mappedBy = "campus" )
	private Set< Unidade > unidades = new HashSet< Unidade >();

	@OneToMany( cascade = CascadeType.ALL, mappedBy = "origem" )
	private Set< DeslocamentoCampus > deslocamentos = new HashSet< DeslocamentoCampus >();

	@OneToMany( cascade = CascadeType.ALL, mappedBy = "destino" )
	private Set< DeslocamentoCampus > deslocamentosDestino = new HashSet< DeslocamentoCampus >();

	@ManyToMany( cascade = { CascadeType.PERSIST, CascadeType.MERGE }, mappedBy = "campi" )
	private Set< Professor > professores = new HashSet< Professor >();

/*	@ManyToMany( cascade = { CascadeType.PERSIST, CascadeType.MERGE }, mappedBy = "campi" )
	private Set< HorarioDisponivelCenario > horarios = new HashSet< HorarioDisponivelCenario >();*/

	@NotNull
	@OneToMany( cascade = CascadeType.ALL, mappedBy = "campus" )
	private Set< Oferta > ofertas = new HashSet< Oferta >();

	@ManyToMany( cascade = CascadeType.ALL, mappedBy = "campi" )
	private Set< Parametro > parametros = new HashSet< Parametro >();

	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH },
		targetEntity = InstituicaoEnsino.class )
	@JoinColumn( name = "INS_ID" )
	private InstituicaoEnsino instituicaoEnsino;

	public InstituicaoEnsino getInstituicaoEnsino()
	{
		return this.instituicaoEnsino;
	}

	public void setInstituicaoEnsino(
		InstituicaoEnsino instituicaoEnsino )
	{
		this.instituicaoEnsino = instituicaoEnsino;
	}

	public Cenario getCenario()
	{
		return this.cenario;
	}

	public void setCenario( Cenario cenario )
	{
		this.cenario = cenario;
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

	public Double getValorCredito()
	{
		return this.valorCredito;
	}

	public void setValorCredito( Double valorCredito )
	{
		this.valorCredito = valorCredito;
	}

	public Estados getEstado()
	{
		return this.estado;
	}

	public void setEstado( Estados estado )
	{
		this.estado = estado;
	}

	public String getMunicipio()
	{
		return this.municipio;
	}

	public void setMunicipio( String municipio )
	{
		this.municipio = municipio;
	}

	public String getBairro()
	{
		return this.bairro;
	}

	public void setBairro( String bairro )
	{
		this.bairro = bairro;
	}

	public Set< Unidade > getUnidades()
	{
		return this.unidades;
	}

	public void setUnidades( Set< Unidade > unidades )
	{
		this.unidades = unidades;
	}

	public Set< DeslocamentoCampus > getDeslocamentos()
	{
		return this.deslocamentos;
	}

	public void setDeslocamentos( Set< DeslocamentoCampus > deslocamentos )
	{
		this.deslocamentos = deslocamentos;
	}

	public Set< DeslocamentoCampus > getDeslocamentosDestino()
	{
		return this.deslocamentosDestino;
	}

	public void setDeslocamentosDestino(
		Set< DeslocamentoCampus > deslocamentosDestino )
	{
		this.deslocamentosDestino = deslocamentosDestino;
	}

	public Set< Professor > getProfessores()
	{
		return this.professores;
	}

	public void setProfessores(
		Set< Professor > professores )
	{
		this.professores = professores;
	}

/*	private Set< HorarioDisponivelCenario > getHorarios()
	{
		return this.horarios;
	}

	public void setHorarios(
		Set< HorarioDisponivelCenario > horarios )
	{
		this.horarios = horarios;
	}
*/
	public Set< Oferta > getOfertas()
	{
		return this.ofertas;
	}

	public void setOfertas( Set< Oferta > ofertas )
	{
		this.ofertas = ofertas;
	}

	public Boolean getPublicado()
	{
		return this.publicado;
	}

	public void setPublicado( Boolean publicado )
	{
		this.publicado = publicado;
	}
			
	public String getQtdProfessorVirtual()
	{
		return this.qtdProfessorVirtual;
	}

	public void setQtdProfessorVirtual( String qtdProfessorVirtual )
	{
		this.qtdProfessorVirtual = qtdProfessorVirtual;
	}
	

	public Set< Parametro > getParametros()
	{
		return this.parametros;
	}

	public void setParametros( Set< Parametro > parametros )
	{
		this.parametros = parametros;
	}
	
	public Set<Sala> getLaboratorios() {
		Set<Sala> laboratorios = new HashSet<Sala>();
		for (Unidade unidade : this.unidades) {
			laboratorios.addAll(unidade.getLaboratorios());
		}
		return laboratorios;
	}

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "CAM_ID" )
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
	static public void preencheHorariosDosCampi(List<Campus> campi, List<SemanaLetiva> semanasLetivas) {
/*		for (SemanaLetiva semanaLetiva : semanasLetivas) {
			for (HorarioAula horarioAula : semanaLetiva.getHorariosAula()) {
				for (HorarioDisponivelCenario hdc : horarioAula.getHorariosDisponiveisCenario()) {
					hdc.getCampi().addAll(campi);
					hdc.merge();
				}
			}
		}*/
	}

	@Transactional
	public void remove()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		if ( this.entityManager.contains( this ) )
		{
			this.removeCurriculoDisciplinas();
			this.removeProfessores();
			this.removeDeslocamentosDestino();
			this.removeUnidades();
			this.removeDeslocamentos();

			this.entityManager.remove( this );
		}
		else
		{
			Campus attached = this.entityManager.find(
				this.getClass(), this.id );

			if ( attached != null )
			{
				this.removeCurriculoDisciplinas();
				
				attached.removeProfessores();
				attached.removeDeslocamentosDestino();
				attached.removeUnidades();
				attached.removeDeslocamentos();

				this.entityManager.remove( attached );
			}
		}
	}

	@Transactional
	private void removeDeslocamentos()
	{
		List< DeslocamentoCampus > deslocamentos
			= DeslocamentoCampus.findAllDeslocamentoCampuses(
				this.getInstituicaoEnsino() );

		for ( DeslocamentoCampus deslocamento : deslocamentos )
		{
			if ( deslocamento.getOrigem().getId() == this.getId()
				|| deslocamento.getDestino().getId() == this.getId() )
			{
				deslocamento.remove();
			}
		}
	}

    @Transactional
    private void removeCurriculoDisciplinas()
    {
    	Set< CurriculoDisciplina > curriculoDisciplinas
    		= new HashSet< CurriculoDisciplina >();

    	Set< Sala > salas = new HashSet< Sala >();

    	List< Unidade > listUnidades = Unidade.findByCampus(
    		this.getInstituicaoEnsino(), this );

    	for ( Unidade unidade : listUnidades )
    	{
    		List< Sala > listSalas = Sala.findByUnidade(
    			this.getInstituicaoEnsino(), unidade );

    		for ( Sala s : listSalas )
    		{
    			salas.add( s );

        		List< CurriculoDisciplina > listCurriculosDisciplinas
    				= CurriculoDisciplina.findBySala( this.getInstituicaoEnsino(), s );

    			for ( CurriculoDisciplina cd : listCurriculosDisciplinas )
    			{
    				curriculoDisciplinas.add( cd );
    			}
    		}
    	}

    	for ( CurriculoDisciplina cd : curriculoDisciplinas )
    	{
    		for ( Sala s : salas )
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
			= this.getInstituicaoEnsino();

		List< SemanaLetiva > listDomains
			= SemanaLetiva.findByCenario( instituicaoEnsino, this.getCenario() );

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
//		DateFormat df = new SimpleDateFormat("HH:mm");
		for (int i = 1; i < todosHorariosOrdenados.size() ; i++)
		{
			DisponibilidadeCampus disponibilidade = new DisponibilidadeCampus();
			disponibilidade.setHorarioInicio(todosHorariosOrdenados.get(i-1));
			disponibilidade.setHorarioFim(todosHorariosOrdenados.get(i));
			disponibilidade.setCampus(this);
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
	public void removeProfessores()
	{
		Set< Professor > professores = this.getProfessores();

		for ( Professor professor : professores )
		{
			professor.getCampi().remove( this );
			professor.merge();
		}
	}

	@Transactional
	public void removeDeslocamentosDestino()
	{
		Set< DeslocamentoCampus > deslocamentosDestino =
			this.getDeslocamentosDestino();

		for ( DeslocamentoCampus deslocamentoDestino : deslocamentosDestino )
		{
			deslocamentoDestino.getOrigem().getDeslocamentos().remove( deslocamentoDestino );
			deslocamentoDestino.getOrigem().merge();
		}
	}

	@Transactional
	public void removeUnidades()
	{
		Set< Unidade > unidades = this.getUnidades();

		for ( Unidade unidade : unidades )
		{
			unidade.remove( false );
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
	public Campus merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		Campus merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new Campus().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
				" Entity manager has not been injected (is the Spring " +
				" Aspects JAR configured as an AJC/AJDT aspects library?) " );
		}

		return em;
	}

	public static int count( InstituicaoEnsino instituicaoEnsino )
	{
		return Campus.findAll( instituicaoEnsino ).size();
	}

	public static int count(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Campus o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario" );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( (Number) q.getSingleResult() ).intValue();
	}

	@SuppressWarnings( "unchecked" )
	public static List< Campus > findAll(
		InstituicaoEnsino instituicaoEnsino )
	{ 
		Query q = entityManager().createQuery(
			" SELECT o FROM Campus o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino" );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< Campus > findAllOtimized(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o.oferta.campus ) " +
			" FROM AtendimentoOperacional o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.oferta.campus.cenario = :cenario " +
			" AND o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< Campus > findAllOtimizedTatico(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o.oferta.campus ) " +
			" FROM AtendimentoTatico o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< Campus > findByCenario(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Campus o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND cenario = :cenario " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	public static Campus findBy(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario, String codigo )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Campus o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " +
			" AND o.codigo = :codigo ");

		q.setParameter( "cenario", cenario );
		q.setParameter( "codigo", codigo );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return (Campus) q.getSingleResult();
	}

	public static Map< String, Campus > buildCampusCodigoToCampusMap( 
		List< Campus > campi )
	{
		Map< String, Campus > campusMap
			= new HashMap< String, Campus >();

		for ( Campus campus : campi )
		{
			campusMap.put( campus.getCodigo(), campus );
		}

		return campusMap;
	}
	
	public static Map< String, Campus > buildCampusNomeToCampusMap( 
			List< Campus > campi )
		{
			Map< String, Campus > campusMap
				= new HashMap< String, Campus >();

			for ( Campus campus : campi )
			{
				campusMap.put( campus.getNome(), campus );
			}

			return campusMap;
		}

	public static Campus find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
		if ( id == null || instituicaoEnsino == null )
		{
			return null;
		}

		Campus campus = entityManager().find( Campus.class, id );

		if ( campus != null
			&& campus.getInstituicaoEnsino() != null
			&& campus.getInstituicaoEnsino() == instituicaoEnsino )
		{
			return campus;
		}

		return null;
	}
	
	@SuppressWarnings("unchecked")
	public static List<Campus> find(Collection<Long> ids, InstituicaoEnsino instituicaoEnsino) {
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM Campus o "
		  + " WHERE o.id IN ( :ids ) "
		  + " AND o.instituicaoEnsino = :instituicaoEnsino"
		);

		q.setParameter("ids",ids);
		q.setParameter("instituicaoEnsino",instituicaoEnsino);

		return q.getResultList();
	}

	public static List< Campus > find(
		InstituicaoEnsino instituicaoEnsino,
		int firstResult, int maxResults )
	{
		return Campus.find( instituicaoEnsino,
			firstResult, maxResults, null );
	}

	@SuppressWarnings( "unchecked" )
	public static List< Campus > find(
		InstituicaoEnsino instituicaoEnsino,
		int firstResult, int maxResults, String orderBy )
	{
		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

		Query q = entityManager().createQuery(
			" SELECT o FROM Campus o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " + orderBy );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		return q.getResultList();
	}

	public static Campus getByCodigo(
		InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, String codigo )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Campus o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND cenario = :cenario " +
			" AND codigo = :codigo " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "codigo", codigo );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return (Campus) q.getSingleResult();
	}

	public static int count(
		InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, String nome, String codigo,
		Estados estado, String municipio, String bairro,
		String operadorCustoMedioCredito, Double custoMedioCredito, Boolean otimizadoOperacional, Boolean otimizadoTatico)
	{
		nome = ( ( nome == null ) ? "" : nome );
		nome = ( "%" + nome.replace( '*', '%' ) + "%" );
		codigo = ( ( codigo == null ) ? "" : codigo );
		codigo = ( "%" + codigo.replace( '*', '%' ) + "%" );
		
		String operadorCustoMedioCreditoStr = "";
		
		if(custoMedioCredito != null){
			if(operadorCustoMedioCredito != null)
				operadorCustoMedioCreditoStr = " valorCredito " + operadorCustoMedioCredito + " :valorCredito and ";
			else
				operadorCustoMedioCreditoStr = " valorCredito = :valorCredito and ";
		}
		
		String otiOperacionalStr = "";
		String otiTaticoStr = "";
		
		if(otimizadoOperacional != null){
			if(otimizadoOperacional){
				otiOperacionalStr = " (SELECT COUNT ( a ) FROM AtendimentoOperacional a " +
						" WHERE a.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
						" AND a.instituicaoEnsino = :instituicaoEnsino " +
						" AND a.oferta.campus = o )  > 0 and ";
			} else {
				otiOperacionalStr = " (SELECT COUNT ( a ) FROM AtendimentoOperacional a " +
						" WHERE a.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
						" AND a.instituicaoEnsino = :instituicaoEnsino " +
						" AND a.oferta.campus = o )  = 0 and ";
			}
		}
		
		if(otimizadoTatico != null){
			if(otimizadoTatico){
				otiTaticoStr = "  (SELECT COUNT ( t ) FROM AtendimentoTatico t " +
						" WHERE t.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
						" AND t.instituicaoEnsino = :instituicaoEnsino " +
						" AND t.oferta.campus = o )  > 0 and ";
			} else {
				otiTaticoStr = "  (SELECT COUNT ( t ) FROM AtendimentoTatico t " +
						" WHERE t.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
						" AND t.instituicaoEnsino = :instituicaoEnsino " +
						" AND t.oferta.campus = o )  = 0 and ";
			}
		}
		

		String municipioQuery = "";
		if ( municipio != null )
		{
			municipioQuery = " LOWER ( o.municipio ) LIKE LOWER ( :municipio ) AND ";
			municipio = ( "%" + municipio.replace( '*', '%' ) + "%" );
		}

		String bairroQuery = "";
		if ( bairro != null )
		{
			bairroQuery = " LOWER ( o.bairro ) LIKE LOWER ( :bairro ) AND ";
			bairro = ( "%" + bairro.replace( '*', '%' ) + "%" );
		}

		EntityManager em = Campus.entityManager();
		String estadoQuery = ( ( estado == null ) ? "" : " o.estado = :estado AND " );

		Query q = em.createQuery(
			" SELECT COUNT ( o ) FROM Campus o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND LOWER ( o.nome ) LIKE LOWER ( :nome ) " +
			" AND LOWER ( o.codigo ) LIKE LOWER ( :codigo ) " +
			" AND o.cenario = :cenario " +
			" AND " + estadoQuery +	bairroQuery + municipioQuery
			 + operadorCustoMedioCreditoStr + otiOperacionalStr + otiTaticoStr + " 1=1 " );

		q.setParameter( "nome", nome );
		q.setParameter( "codigo", codigo );
		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( estado != null )
		{
			q.setParameter( "estado", estado );
		}

		if ( municipio != null )
		{
			q.setParameter( "municipio", municipio );
		}

		if ( bairro != null )
		{
			q.setParameter( "bairro", bairro );
		}
		
		if( custoMedioCredito != null){
			q.setParameter("valorCredito", custoMedioCredito);
		}

		return ( (Number) q.getSingleResult() ).intValue();
	}

	@SuppressWarnings( "unchecked" )
	public static List< Campus > findBy(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario,
		String nome, String codigo, Estados estado, String municipio,
		String bairro,
		String operadorCustoMedioCredito, Double custoMedioCredito, Boolean otimizadoOperacional, Boolean otimizadoTatico,
		int firstResult, int maxResults, String orderBy )
	{
		nome = ( ( nome == null ) ? "" : nome );
		nome = ( "%" + nome.replace( '*', '%' ) + "%" );
		codigo = ( ( codigo == null ) ? "" : codigo );
		codigo = ( "%" + codigo.replace( '*', '%' ) + "%" );
		
		String operadorCustoMedioCreditoStr = "";
		
		if(custoMedioCredito != null){
			if(operadorCustoMedioCredito != null)
				operadorCustoMedioCreditoStr = " valorCredito " + operadorCustoMedioCredito + " :valorCredito and ";
			else
				operadorCustoMedioCreditoStr = " valorCredito = :valorCredito and ";
		}
		
		String otiOperacionalStr = "";
		String otiTaticoStr = "";
		
		if(otimizadoOperacional != null){
			if(otimizadoOperacional){
				otiOperacionalStr = " (SELECT COUNT ( a ) FROM AtendimentoOperacional a " +
						" WHERE a.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
						" AND a.instituicaoEnsino = :instituicaoEnsino " +
						" AND a.oferta.campus = o )  > 0 and ";
			} else {
				otiOperacionalStr = " (SELECT COUNT ( a ) FROM AtendimentoOperacional a " +
						" WHERE a.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
						" AND a.instituicaoEnsino = :instituicaoEnsino " +
						" AND a.oferta.campus = o )  = 0 and ";
			}
		}
		
		if(otimizadoTatico != null){
			if(otimizadoTatico){
				otiTaticoStr = "  (SELECT COUNT ( t ) FROM AtendimentoTatico t " +
						" WHERE t.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
						" AND t.instituicaoEnsino = :instituicaoEnsino " +
						" AND t.oferta.campus = o )  > 0 and ";
			} else {
				otiTaticoStr = "  (SELECT COUNT ( t ) FROM AtendimentoTatico t " +
						" WHERE t.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
						" AND t.instituicaoEnsino = :instituicaoEnsino " +
						" AND t.oferta.campus = o )  = 0 and ";
			}
		}

		String municipioQuery = "";
		if ( municipio != null )
		{
			municipioQuery = " LOWER ( o.municipio ) LIKE LOWER ( :municipio ) AND ";
			municipio = ( "%" + municipio.replace( '*', '%' ) + "%" );
		}

		String bairroQuery = "";
		if ( bairro != null )
		{
			bairroQuery = ( " LOWER ( o.bairro ) LIKE LOWER ( :bairro ) AND " );
			bairro = ( "%" + bairro.replace( '*', '%' ) + "%" );
		}

		EntityManager em = Campus.entityManager();
		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );
		String estadoQuery = ( ( estado == null ) ? "" : " o.estado = :estado AND " );
		Query q = em.createQuery(
			" SELECT o FROM Campus o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND LOWER ( o.nome ) LIKE LOWER ( :nome ) " +
			" AND LOWER ( o.codigo ) LIKE LOWER ( :codigo ) " +
			" AND o.cenario = :cenario " +
			" AND " + estadoQuery + bairroQuery + municipioQuery + operadorCustoMedioCreditoStr +  otiOperacionalStr + otiTaticoStr + " 1=1 " + orderBy );

		q.setParameter( "nome", nome );
		q.setParameter( "codigo", codigo );
		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( estado != null )
		{
			q.setParameter( "estado", estado );
		}

		if ( municipio != null )
		{
			q.setParameter( "municipio", municipio );
		}

		if ( bairro != null )
		{
			q.setParameter( "bairro", bairro );
		}
		
		if( custoMedioCredito != null){
			q.setParameter("valorCredito", custoMedioCredito);
		}

		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public List< HorarioDisponivelCenario > getHorarios(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM HorarioDisponivelCenario o, IN ( o.atendimentosOperacionais ) c " +
			" WHERE c.sala.unidade.campus = :campus " +
			" AND o.horarioAula.semanaLetiva.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "campus", this );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< Campus > findByCurriculo(
		Cenario cenario, InstituicaoEnsino instituicaoEnsino, Curriculo curriculo )
	{
		Query q = entityManager().createQuery(
			" SELECT c FROM Campus c, IN ( c.ofertas ) o " +
			" WHERE c.instituicaoEnsino = :instituicaoEnsino " +
			" AND c.cenario = :cenario " +
			" AND o.curriculo = :curriculo " +
			" AND o.campus.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "curriculo", curriculo );
		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	public boolean isOtimizadoTatico(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM AtendimentoTatico o " +
			" WHERE o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.oferta.campus = :campus " );

		q.setParameter( "campus", this );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		Number size = (Number) q.setMaxResults( 1 ).getSingleResult();
		return ( size.intValue() > 0 );
	}

	public boolean isOtimizadoOperacional(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM AtendimentoOperacional o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.oferta.campus = :campus" );

		q.setParameter( "campus", this );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		Number size = (Number) q.setMaxResults( 1 ).getSingleResult();
		return ( size.intValue() > 0 );
	}

	public boolean isOtimizadoTotal(
		InstituicaoEnsino instituicaoEnsino )
	{
		return ( isOtimizadoTatico( instituicaoEnsino )
			&& isOtimizadoOperacional( instituicaoEnsino ) );
	}

	public boolean isOtimizado(
		InstituicaoEnsino instituicaoEnsino )
	{
		return ( isOtimizadoTatico( instituicaoEnsino )
			|| isOtimizadoOperacional( instituicaoEnsino ) );
	}

	public static boolean checkCodigoUnique(
		InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, String codigo )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Campus o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " +
			" AND o.codigo = :codigo" );

		q.setParameter( "cenario", cenario );
		q.setParameter( "codigo", codigo );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		Number size = ( (Number) q.setMaxResults( 1 ).getSingleResult() );
		return ( size.intValue() > 0 );
	}

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append( "Id: " ).append( getId() ).append( ", " );
		sb.append( "Version: " ).append( getVersion() ).append( ", " );
		sb.append( "Instituicao de Ensino: " ).append( getInstituicaoEnsino() ).append( ", " );
		sb.append( "Cenario: " ).append( getCenario() ).append( ", " );
		sb.append( "Codigo: " ).append( getCodigo() ).append( ", " );
		sb.append( "Nome: " ).append( getNome() ).append( ", " );
		sb.append( "ValorCredito: " ).append( getValorCredito() ).append( ", " );
		sb.append( "Estado: " ).append( getEstado().name() ).append( ", " );
		sb.append( "Municipio: " ).append( getMunicipio() ).append( ", " );
		sb.append( "Bairro: " ).append( getBairro() ).append( ", " );
		sb.append( "Unidades: " ).append(getUnidades() == null ? "null" : getUnidades().size() ).append( ", " );
		sb.append( "Deslocamentos: " ).append(getDeslocamentos() == null ? "null" : getDeslocamentos().size() ).append( ", " );
		sb.append( "DeslocamentosDestino: " ).append(getDeslocamentosDestino() == null ? "null" : getDeslocamentosDestino().size() ).append( ", " );
		sb.append( "Professores: ").append(getProfessores() == null ? "null" : getProfessores().size() ).append( ", " );
		sb.append( "Horarios: ").append(getOfertas() == null ? "null" : getOfertas().size() );
		sb.append( "Publicado: " ).append( getPublicado() );
		sb.append( "Parametros: " ).append(getParametros() == null ? "null" : getParametros().size() ).append(", ");
		sb.append( "QtdProfessorVirtual: " ).append( getQtdProfessorVirtual());

		return sb.toString();
	}

	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null || !( obj instanceof Campus ) )
		{
			return false;
		}

		Campus other = (Campus) obj;

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

	//@Override
	public int compareTo( Campus c )
	{
		return this.getCodigo().compareTo( c.getCodigo() );
	}
	
	public Campus clone(Cenario novoCenario)
	{
		Campus clone = new Campus();
		clone.setBairro(this.getBairro());
		clone.setCenario(novoCenario);
		clone.setCodigo(this.getCodigo());
		clone.setEstado(this.getEstado());
		clone.setInstituicaoEnsino(this.getInstituicaoEnsino());
		clone.setMunicipio(this.getMunicipio());
		clone.setNome(this.getNome());
		clone.setPublicado(this.getPublicado());
		clone.setValorCredito(this.getValorCredito());
		clone.setQtdProfessorVirtual(this.getQtdProfessorVirtual());
		
		return clone;
	}

	public Campus clone(CenarioClone novoCenario) {
		Campus clone = new Campus();
		clone.setBairro(this.getBairro());
		clone.setCenario(novoCenario.getCenario());
		clone.setCodigo(this.getCodigo());
		clone.setEstado(this.getEstado());
		clone.setInstituicaoEnsino(this.getInstituicaoEnsino());
		clone.setMunicipio(this.getMunicipio());
		clone.setNome(this.getNome());
		clone.setPublicado(this.getPublicado());
		clone.setValorCredito(this.getValorCredito());
		clone.setQtdProfessorVirtual(this.getQtdProfessorVirtual());
		
		
		return clone;
	}

	@Transactional
	public void cloneChilds(CenarioClone novoCenario, Campus entidadeClone) {
		for (Unidade unidade : this.getUnidades())
		{
			entidadeClone.getUnidades().add(novoCenario.clone(unidade));
		}
	}
}
