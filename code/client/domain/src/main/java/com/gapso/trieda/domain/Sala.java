package com.gapso.trieda.domain;

import java.io.Serializable;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.JoinTable;
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

import com.gapso.trieda.misc.Semanas;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity( identifierColumn = "SAL_ID" )
@Table( name = "SALAS", uniqueConstraints =
@UniqueConstraint( columnNames = { "UNI_ID", "SAL_CODIGO" } ) )
public class Sala
	implements Serializable, Comparable< Sala >, Clonable< Sala >
{
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((codigo == null) ? 0 : codigo.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Sala other = (Sala) obj;
		if (codigo == null) {
			if (other.codigo != null)
				return false;
		} else if (!codigo.equals(other.codigo))
			return false;
		return true;
	}

	private static final long serialVersionUID = -2533999449644229682L;

    @NotNull
    @ManyToOne(targetEntity = TipoSala.class)
    @JoinColumn(name = "TSA_ID")
    private TipoSala tipoSala;

    @NotNull
    @ManyToOne(targetEntity = Unidade.class)
    @JoinColumn(name = "UNI_ID")
    private Unidade unidade;

    @NotNull
    @Column(name = "SAL_CODIGO")
    @Size(min = 1, max = 200)
    private String codigo;

    @NotNull
    @Column(name = "SAL_NUMERO")
    @Size(min = 1, max = 200)
    private String numero;
    
    @Column(name = "SAL_DESCRICAO")
    @Size( max = 255 )
    private String descricao;

    @NotNull
    @Column(name = "SAL_ANDAR")
    @Size(min = 1, max = 20)
    private String andar;

    @NotNull
    @Column(name = "SAL_CAPACIDADE")
    @Min(1L)
    @Max(9999L)
    private Integer capacidadeInstalada;
    
    @NotNull
    @Column(name = "SAL_CAPACIDADE_MAX")
    @Min(1L)
    @Max(9999L)
    private Integer capacidadeMax;
    
	@Column( name = "SAL_CUSTO_OPERACAO_CRED" )
	@Digits( integer = 6, fraction = 2 )
	private Double custoOperacaoCred;
	
	@NotNull
	@Column( name = "SAL_EXTERNA" )
	private Boolean externa;

/*    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE}, mappedBy = "salas")
    private Set< HorarioDisponivelCenario > horarios = new HashSet< HorarioDisponivelCenario >();*/

    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE})
	@JoinTable(name="disciplinas_salas",
	joinColumns={ @JoinColumn(name="sal_id") },
	inverseJoinColumns={ @JoinColumn(name="dis_id") })
    private Set< Disciplina > disciplinas = new HashSet< Disciplina >();

    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE}, mappedBy = "salas")
    private Set< GrupoSala > gruposSala = new HashSet< GrupoSala >();

    @OneToMany(cascade = CascadeType.ALL, mappedBy="sala")
    private Set< AtendimentoOperacional > atendimentosOperacionais =  new HashSet< AtendimentoOperacional >();

    @OneToMany(cascade = CascadeType.ALL, mappedBy="sala")
    private Set< AtendimentoTatico > atendimentosTaticos =  new HashSet< AtendimentoTatico >();

    @OneToMany(cascade = CascadeType.REMOVE, mappedBy = "sala")
    private Set< Fixacao > fixacoes = new HashSet< Fixacao >();
    
    @OneToMany( mappedBy="sala" )
    private Set< Aula > aulas =  new HashSet< Aula >();
    
	@OneToMany( cascade = CascadeType.ALL, mappedBy = "DisponibilidadeSala" )
	private Set< DisponibilidadeSala > disponibilidades = new HashSet< DisponibilidadeSala >();
    
    public boolean isLaboratorio() {
    	return getTipoSala().getAceitaAulaPratica();
    }

	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: " ).append( getId() ).append( ", " );
        sb.append( "Version: " ).append( getVersion() ).append( ", " );
        sb.append( "TipoSala: " ).append( getTipoSala() ).append( ", " );
        sb.append( "Unidade: " ).append( getUnidade() ).append( ", " );
        sb.append( "Codigo: " ).append( getCodigo() ).append( ", " );
        sb.append( "Numero: " ).append( getNumero() ).append( ", " );
        sb.append( "Andar: " ).append( getAndar() ).append( ", " );
        sb.append( "Capacidade Instalada: " ).append( getCapacidadeInstalada() ).append(", ");
        sb.append( "Capacidade Maxima: " ).append( getCapacidadeMax() ).append(", ");
        sb.append( "Disciplinas: " ).append(
        	getDisciplinas() == null ? "null" : getDisciplinas().size() ).append(", ");
        sb.append( "GruposSala: " ).append(
        	getGruposSala() == null ? "null" : getGruposSala().size() );
        sb.append( "Atendimentos Operacionais: " ).append(
        	getAtendimentosOperacionais() == null ? "null" : getAtendimentosOperacionais().size() );
        sb.append( "Atendimentos Taticos: " ).append(
        	getAtendimentosTaticos() == null ? "null" : getAtendimentosTaticos().size() );
        sb.append( "Fixacoes: " ).append( getFixacoes() == null ? "null" : getFixacoes().size() );

        return sb.toString();
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "SAL_ID")
    private Long id;

	@Version
    @Column(name = "version")
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

		InstituicaoEnsino instituicaoEnsino
			= this.getUnidade().getCampus().getInstituicaoEnsino();

        Sala find = Sala.find( this.getId(), instituicaoEnsino );
        if ( find == null )
        {
        	this.entityManager.persist( this );
        }
    }
	
	@Transactional
    public void persistAndPreencheHorarios()
	{
        persist();
        preencheHorarios();
    }
	
	@Transactional
	static public void preencheHorariosDasSalas(List<Sala> salas, List<SemanaLetiva> semanasLetivas) {
/*		for (SemanaLetiva semanaLetiva : semanasLetivas) {
			for (HorarioAula horarioAula : semanaLetiva.getHorariosAula()) {
				for (HorarioDisponivelCenario hdc : horarioAula.getHorariosDisponiveisCenario()) {
					hdc.getSalas().addAll(salas);
					hdc.merge();
				}
			}
		}*/
	}

	@Transactional
	public void preencheHorarios()
	{
		InstituicaoEnsino instituicaoEnsino
			= this.getUnidade().getCampus().getInstituicaoEnsino();

		List< SemanaLetiva > listDomains
			= SemanaLetiva.findByCenario( instituicaoEnsino, this.getUnidade().getCampus().getCenario() );

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
		for (int i = 1; i < todosHorariosOrdenados.size() ; i++)
		{
			DisponibilidadeSala disponibilidade = new DisponibilidadeSala();
			disponibilidade.setHorarioInicio(todosHorariosOrdenados.get(i-1));
			disponibilidade.setHorarioFim(todosHorariosOrdenados.get(i));
			disponibilidade.setSala(this);
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
	public void remove()
	{
		this.remove( true );
	}

	@Transactional
    public void remove( boolean removeDisciplinas )
	{
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        if ( this.entityManager.contains( this ) )
        {
			if ( removeDisciplinas )
			{
				this.removeDisciplinas();
			}

        	this.removeGruposSala();

            this.entityManager.remove( this );
        }
        else
        {
            Sala attached = this.entityManager.find(
            	this.getClass(), this.id );

			if ( attached != null )
			{
				if ( removeDisciplinas )
				{
					attached.removeDisciplinas();
				}

            	attached.removeGruposSala();

            	this.entityManager.remove( attached );
			}
        }
    }
	
    @Transactional
    public void removeDisciplinas()
    {
    	Set< Disciplina > disciplinas
    		= this.getDisciplinas();

    	for ( Disciplina disciplina : disciplinas )
    	{
    		disciplina.getSalas().remove( this );
    		disciplina.merge();
    	}
    }

    @Transactional
    public void removeGruposSala()
    {
    	Set< GrupoSala > gruposSala = this.getGruposSala();

    	for ( GrupoSala grupoSala : gruposSala )
    	{
    		grupoSala.getSalas().remove( this );
    		grupoSala.merge();
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
    public Sala merge()
	{
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        Sala merged = this.entityManager.merge( this );
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager()
	{
        EntityManager em = new Sala().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		"Entity manager has not been injected (is the Spring " +
        		"Aspects JAR configured as an AJC/AJDT aspects library?)" );
        }

        return em;
    }

	@Transactional
	static public void atualizaHorariosDasSalas(Map<Sala, List<TriedaTrio<Semanas,Calendar,Calendar>>> SalaToDisponibilidadesMap, List<SemanaLetiva> semanasLetivas) {
		// coleta as salas disponíveis por dia da semana e tempo de aula
		int count = 0;
		Map<HorarioDisponivelCenario, Set<Sala>> hdcToSalaMap = new HashMap<HorarioDisponivelCenario, Set<Sala>>();
		for (Entry<Sala, List<TriedaTrio<Semanas,Calendar,Calendar>>> entry : SalaToDisponibilidadesMap.entrySet()) {
			List<TriedaTrio<Semanas,Calendar,Calendar>> disponibilidades = entry.getValue();
			Sala sala = entry.getKey();
			for (SemanaLetiva semanaLetiva : semanasLetivas) {
				// para cada tempo de aula
				for (HorarioAula horarioAula : semanaLetiva.getHorariosAula()) {
					for (TriedaTrio<Semanas,Calendar,Calendar> trio : disponibilidades){
						// verifica se o intervalo de horas é compatível
						boolean horarioAulaEstahContidoEmDisponibilidade = horarioAula.estahContidoEm(trio.getSegundo(),trio.getTerceiro()); 
						// para cada dia da semana
						for (HorarioDisponivelCenario hdc : horarioAula.getHorariosDisponiveisCenario()) {
							Set<Sala> salasDisponiveisNoDiaEHorario = hdcToSalaMap.get(hdc);
							if (salasDisponiveisNoDiaEHorario == null) {
								salasDisponiveisNoDiaEHorario = new HashSet<Sala>();
								hdcToSalaMap.put(hdc,salasDisponiveisNoDiaEHorario);
							}
							
							// verifica se o dia da semana é compatível
							if (horarioAulaEstahContidoEmDisponibilidade && hdc.getDiaSemana().equals(trio.getPrimeiro())) {
								salasDisponiveisNoDiaEHorario.add(sala);
							}
						}
					}
					count++;if (count == 100) {System.out.println("   100 horários das salas processados"); count = 0;}
				}
			}
		}
		
/*		// atualiza disponibilidades das salas
		count = 0;
		for (Entry<HorarioDisponivelCenario, Set<Sala>> entry : hdcToSalaMap.entrySet()) {
			HorarioDisponivelCenario hdc = entry.getKey();
			Set<Sala> salasDisponiveisNoDiaEHorario = entry.getValue();
			
			hdc.getSalas().clear();
			hdc.getSalas().addAll(salasDisponiveisNoDiaEHorario);
			hdc.merge();
			
			count++;if (count == 100) {System.out.println("   100 horários das salas processados"); count = 0;}
		}*/
	}
	
	public static int count(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Sala o " +
			" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.unidade.campus.cenario = :cenario " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( (Number) q.getSingleResult() ).intValue();
	}

	public static int countLaboratorioNaoExternos(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Sala o " +
			" WHERE o.tipoSala.aceitaAulaPratica IS true " +
			" AND o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.unidade.campus.cenario = :cenario " +
			" AND o.externa IS FALSE ");

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( (Number) q.getSingleResult() ).intValue();
	}
	
	public static int countLaboratorioExternos(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario )
		{
			Query q = entityManager().createQuery(
				" SELECT COUNT ( o ) FROM Sala o " +
				" WHERE o.tipoSala.aceitaAulaPratica IS true " +
				" AND o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
				" AND o.unidade.campus.cenario = :cenario " +
				" AND o.externa IS TRUE " );

			q.setParameter( "cenario", cenario );
			q.setParameter( "instituicaoEnsino", instituicaoEnsino );

			return ( (Number) q.getSingleResult() ).intValue();
		}

	public static int countLaboratorio(
		InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Sala o " +
			" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.tipoSala.aceitaAulaPratica IS true " +
			" AND o.unidade.campus = :campus " );

		q.setParameter( "campus", campus );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( (Number) q.getSingleResult() ).intValue();
	}

	public static int countSalaDeAulaNaoExternas(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Sala o " +
			" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.tipoSala.aceitaAulaPratica IS false " +
			" AND o.unidade.campus.cenario = :cenario " +
			" AND o.externa IS FALSE ");

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( (Number) q.getSingleResult() ).intValue();
	}
	
	public static int countSalaDeAulaExternas(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario )
		{
			Query q = entityManager().createQuery(
				" SELECT COUNT ( o ) FROM Sala o " +
				" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
				" AND o.tipoSala.aceitaAulaPratica IS false " +
				" AND o.unidade.campus.cenario = :cenario " +
				" AND o.externa IS TRUE ");

			q.setParameter( "cenario", cenario );
			q.setParameter( "instituicaoEnsino", instituicaoEnsino );

			return ( (Number) q.getSingleResult() ).intValue();
		}

	public static int countSalaDeAula(
		InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Sala o " +
			" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.tipoSala.aceitaAulaPratica IS false " +
			" AND o.unidade.campus = :campus " );

		q.setParameter( "campus", campus );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( (Number) q.getSingleResult() ).intValue();
	}

	public static Map< String, Sala > buildSalaCodigoToSalaMap( List< Sala > salas )
	{
		Map< String, Sala > salasMap
			= new HashMap< String, Sala >();

		for ( Sala sala : salas )
		{
			salasMap.put( sala.getCodigo(), sala );
		}

		return salasMap;
	}
	
	public static Map< Long, Sala > buildSalaIdToSalaMap( List< Sala > salas )
	{
		Map< Long, Sala > salasMap
			= new HashMap< Long, Sala >();

		for ( Sala sala : salas )
		{
			salasMap.put( sala.getId(), sala );
		}

		return salasMap;
	}

	public static List< Sala > findAndaresAll( InstituicaoEnsino instituicaoEnsino )
	{
		return findAndaresAll( instituicaoEnsino, null );
	}

	@SuppressWarnings("unchecked")
	public static List< Sala > findAndaresAll(
		InstituicaoEnsino instituicaoEnsino, Unidade unidade )
	{
		List< Sala > list;
		Query q = null;

		if ( unidade == null )
		{
			q = entityManager().createQuery(
				" SELECT o FROM Sala o " +
				" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
				" GROUP BY o.andar " );

			q.setParameter( "instituicaoEnsino", instituicaoEnsino );

			list = q.getResultList();
		}
		else
		{
			q = entityManager().createQuery(
				" SELECT o FROM Sala o " +
				" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
				" AND o.unidade = :unidade GROUP BY o.andar " );

			q.setParameter( "unidade", unidade );
			q.setParameter( "instituicaoEnsino", instituicaoEnsino );

			list = q.getResultList();
		}

		return list;
	}

	@SuppressWarnings("unchecked")
	public static List< Sala > findSalasDoAndarAll(
		InstituicaoEnsino instituicaoEnsino,
		Unidade unidade, List< String > andares )
	{
		if ( andares.size() == 0 )
		{
			return new ArrayList< Sala >();
		}

		String whereQuery = " SELECT o FROM Sala o " +
			" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino AND ( ";

		for ( int i = 1; i < andares.size(); i++ )
		{
			whereQuery += ( " o.andar = :andares" + i + " OR " );
		}

		whereQuery += ( " o.andar = :andares0 ) AND o.unidade = :unidade " );
		Query query = entityManager().createQuery( whereQuery );
		for ( int i = 0; i < andares.size(); i++ )
		{
			query.setParameter( "andares" + i, andares.get( i ) );
		}

		query.setParameter( "unidade", unidade );
		query.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return query.getResultList();
	}

	@SuppressWarnings("unchecked")
    public static List< Sala > findAll(
    	InstituicaoEnsino instituicaoEnsino )
    {
		Query q = entityManager().createQuery(
	       	" SELECT o FROM Sala o " +
    		" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        return q.getResultList();
    }

	public static Sala find( Long id, InstituicaoEnsino instituicaoEnsino )
	{
        if ( id == null || instituicaoEnsino == null )
        {
        	return null;
        }

        Sala sala = entityManager().find( Sala.class, id );

        if ( sala != null && sala.getUnidade() != null
        	&& sala.getUnidade().getCampus() != null
        	&& sala.getUnidade().getCampus().getInstituicaoEnsino() != null
        	&& sala.getUnidade().getCampus().getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return sala;
        }

        return null;
    }

	@SuppressWarnings("unchecked")
	public static List< Sala > findByCenario(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
    	Query q = entityManager().createQuery(
    		" SELECT o FROM Sala o " +
    		" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
    		" AND o.unidade.campus.cenario = :cenario " );

    	q.setParameter( "cenario", cenario );
    	q.setParameter( "instituicaoEnsino", instituicaoEnsino );

    	return q.getResultList();
    }

	public static Sala findByCodigo(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario, String codigo )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Sala o " +
			" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.unidade.campus.cenario = :cenario " +
			" AND codigo = :codigo " );

		q.setParameter( "codigo", codigo );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

		return (Sala) q.getSingleResult();
	}

	@SuppressWarnings("unchecked")
	public static List< Sala > findByUnidade(
		InstituicaoEnsino instituicaoEnsino, Unidade unidade )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Sala o " +
			" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.unidade = :unidade " );

		q.setParameter( "unidade", unidade );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	public static Integer count( InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, Campus campus, Unidade unidade,
		TipoSala tipoSala, String operadorCapacidadeInstalada, Integer capacidadeInstalada,
		String operadorCapacidadeMaxima, Integer capacidadeMax, String operadorCustoOperacao, Double custoOperacao,
		String  numero, String descricao, String andar, String codigo)
	{
		String whereString = " WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino" +
				" AND o.unidade.campus.cenario = :cenario and ";

		if ( codigo != null )
		{
			whereString += " o.codigo = :codigo and ";
		}
		
		if ( campus != null )
		{
			whereString += " o.unidade.campus = :campus and ";
		}

		if ( unidade != null )
		{
			whereString += " o.unidade = :unidade and ";
		}
		
		if ( tipoSala != null )
		{
			whereString += " o.tipoSala = :tipoSala and ";
			
		}
		
		if ( numero != null )
		{
			whereString += " o.numero = :numero and ";
			
		}
		
		if ( andar != null )
		{
			whereString += " o.andar = :andar and ";
			
		}
		
		if ( descricao != null )
		{
			whereString += ( " LOWER ( o.descricao ) LIKE LOWER ( :descricao ) AND " );
			descricao = ( "%" + descricao.replace( '*', '%' ) + "%" );
		}
		
		if(capacidadeInstalada != null){
			if(operadorCapacidadeInstalada != null)
				whereString += " capacidadeInstalada " + operadorCapacidadeInstalada + " :capacidadeInstalada and ";
			else
				whereString += " capacidadeInstalada = :capacidadeInstalada and ";
		}
		
		if(capacidadeMax != null){
			if(operadorCapacidadeMaxima != null)
				whereString += " capacidadeMax " + operadorCapacidadeMaxima + " :capacidadeMax and ";
			else
				whereString += " capacidadeMax = :capacidadeMax and ";
		}
		
		if(custoOperacao != null){
			if(operadorCustoOperacao != null)
				whereString += " custoOperacaoCred " + operadorCustoOperacao + " :custoOperacaoCred and ";
			else
				whereString += " custoOperacaoCred = :custoOperacaoCred and ";
		}

		Query q = entityManager().createQuery(
			"SELECT COUNT ( o ) FROM Sala o " + whereString + " 1=1");

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

		if ( codigo != null )
		{
			q.setParameter( "codigo", codigo );
		}
		
		if ( campus != null )
		{
			q.setParameter( "campus", campus );
		}

		if ( unidade != null )
		{
			q.setParameter( "unidade", unidade );
		}
		
		if( tipoSala != null)
		{
			q.setParameter("tipoSala", tipoSala);
		}
		
		if ( numero != null )
		{
			q.setParameter("numero", numero);
		}
		
		if ( andar != null )
		{
			q.setParameter("andar", andar);
		}
		
		if ( descricao != null )
		{
			q.setParameter( "descricao", descricao );
		}
		
		if(capacidadeInstalada != null){
			q.setParameter("capacidadeInstalada", capacidadeInstalada);
		}
		
		if(capacidadeMax != null){
			q.setParameter("capacidadeMax", capacidadeMax);
		}
		
		if(custoOperacao != null){
			q.setParameter("custoOperacaoCred", custoOperacao);
		}

		return ( (Number) q.getSingleResult() ).intValue();
	}

	@SuppressWarnings("unchecked")
    public static List< Sala > find( InstituicaoEnsino instituicaoEnsino, Cenario cenario,
    	Campus campus, Unidade unidade,
    	TipoSala tipoSala, String operadorCapacidadeInstalada, Integer capacidadeInstalada,
		String operadorCapacidadeMaxima, Integer capacidadeMax, String operadorCustoOperacao, Double custoOperacao,
		String  numero, String descricao, String andar, String codigo,
    	int firstResult, int maxResults, String orderBy )
    {
		String whereString = " WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino" +
				" AND o.unidade.campus.cenario = :cenario and ";
		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy.replace("String", "") : "" );

		if ( codigo != null )
		{
			whereString += " o.codigo = :codigo and ";
		}
		
		if ( campus != null )
		{
			whereString += " o.unidade.campus = :campus and ";
		}

		if ( unidade != null )
		{
			whereString += " o.unidade = :unidade and ";
		}
		
		if ( tipoSala != null )
		{
			whereString += " o.tipoSala = :tipoSala and ";
			
		}
		
		if ( numero != null )
		{
			whereString += " o.numero = :numero and ";
			
		}
		
		if ( andar != null )
		{
			whereString += " o.andar = :andar and ";
			
		}
		
		if ( descricao != null )
		{
			whereString += ( " LOWER ( o.descricao ) LIKE LOWER ( :descricao ) AND " );
			descricao = ( "%" + descricao.replace( '*', '%' ) + "%" );
		}
		
		if(capacidadeInstalada != null){
			if(operadorCapacidadeInstalada != null)
				whereString += " capacidadeInstalada " + operadorCapacidadeInstalada + " :capacidadeInstalada and ";
			else
				whereString += " capacidadeInstalada = :capacidadeInstalada and ";
		}
		
		if(capacidadeMax != null){
			if(operadorCapacidadeMaxima != null)
				whereString += " capacidadeMax " + operadorCapacidadeMaxima + " :capacidadeMax and ";
			else
				whereString += " capacidadeMax = :capacidadeMax and ";
		}
		
		if(custoOperacao != null){
			if(operadorCustoOperacao != null)
				whereString += " custoOperacaoCred " + operadorCustoOperacao + " :custoOperacaoCred and ";
			else
				whereString += " custoOperacaoCred = :custoOperacaoCred and ";
		}

		Query q = entityManager().createQuery(
			"SELECT o FROM Sala o " + whereString + "  1=1 " +  orderBy);

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		
		if ( codigo != null )
		{
			q.setParameter( "codigo", codigo );
		}
		
		if ( campus != null )
		{
			q.setParameter( "campus", campus );
		}

		if ( unidade != null )
		{
			q.setParameter( "unidade", unidade );
		}
		
		if( tipoSala != null)
		{
			q.setParameter("tipoSala", tipoSala);
		}
		
		if ( numero != null )
		{
			q.setParameter("numero", numero);
		}
		
		if ( andar != null )
		{
			q.setParameter("andar", andar);
		}
		
		if ( descricao != null )
		{
			q.setParameter( "descricao", descricao );
		}
		
		if(capacidadeInstalada != null){
			q.setParameter("capacidadeInstalada", capacidadeInstalada);
		}
		
		if(capacidadeMax != null){
			q.setParameter("capacidadeMax", capacidadeMax);
		}
		
		if(custoOperacao != null){
			q.setParameter("custoOperacaoCred", custoOperacao);
		}

        return q.getResultList();
    }
	
	@SuppressWarnings("unchecked")
    public static List< Sala > find( InstituicaoEnsino instituicaoEnsino,
    	Cenario cenario, String codigo, int firstResult, int maxResults )
    {
		Query q = entityManager().createQuery(
			" SELECT o FROM Sala o " +
			" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.unidade.campus.cenario = :cenario " +
			" AND codigo LIKE LOWER (:codigo) " );

		q.setParameter( "codigo", "%"+codigo+"%" );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

        return q.getResultList();
    }

	@SuppressWarnings( "unchecked" )
	public List< HorarioDisponivelCenario > getHorarios(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM HorarioDisponivelCenario o, IN ( o.salas ) c " +
			" WHERE c.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND c = :sala " );

		q.setParameter( "sala", this );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	public static boolean checkCodigoUnique(
		InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, String codigo )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Sala o " +
			" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.unidade.campus.cenario = :cenario " +
			" AND o.codigo = :codigo " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "codigo", codigo );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		Number size = (Number) q.setMaxResults( 1 ).getSingleResult();
		return ( size.intValue() > 0 );
	}
	

	@SuppressWarnings("unchecked")
	public static List<Sala> findByCampus(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario,
		Campus campus)
	{
    	Query q = entityManager().createQuery(
       		" SELECT o FROM Sala o " +
       		" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
       		" AND o.unidade.campus.cenario = :cenario " +
       		" AND o.unidade.campus = :campus ");

       	q.setParameter( "cenario", cenario );
       	q.setParameter( "instituicaoEnsino", instituicaoEnsino );
       	q.setParameter( "campus", campus );

       	return q.getResultList();
	}

	public TipoSala getTipoSala()
	{
        return this.tipoSala;
    }

	public void setTipoSala( TipoSala tipoSala )
	{
        this.tipoSala = tipoSala;
    }

	public Unidade getUnidade()
	{
        return this.unidade;
    }

	public void setUnidade( Unidade unidade )
	{
        this.unidade = unidade;
    }

	public String getCodigo()
	{
        return this.codigo;
    }

	public void setCodigo( String codigo )
	{
        this.codigo = codigo;
    }

	public String getNumero()
	{
        return this.numero;
    }

	public void setNumero( String numero )
	{
        this.numero = numero;
    }
	
	public String getDescricao()
	{
        return this.descricao;
    }

	public void setDescricao( String descricao )
	{
        this.descricao = descricao;
    }

	public String getAndar()
	{
        return this.andar;
    }

	public void setAndar( String andar )
	{
        this.andar = andar;
    }

	public Integer getCapacidadeInstalada()
	{
        return this.capacidadeInstalada;
    }

	public void setCapacidadeInstalada( Integer capacidadeInstalada )
	{
        this.capacidadeInstalada = capacidadeInstalada;
    }
	
	public Integer getCapacidadeMax()
	{
        return this.capacidadeMax;
    }

	public void setCapacidadeMax( Integer capacidadeMax )
	{
        this.capacidadeMax = capacidadeMax;
    }
	
	public Double getCustoOperacaoCred()
	{
		return this.custoOperacaoCred;
	}
	
	public Double setCustoOperacaoCred( Double custoOperacaoCred )
	{
		return this.custoOperacaoCred = custoOperacaoCred;
	}
	
	public void setExterna( Boolean externa )
	{
		this.externa = externa;
	}
	
	public Boolean getExterna()
	{
		return this.externa;
	}

/*	public Set< HorarioDisponivelCenario > getHorarios()
	{
        return this.horarios;
    }

	public void setHorarios( Set< HorarioDisponivelCenario > horarios )
	{
        this.horarios = horarios;
    }*/

	public Set< Disciplina > getDisciplinas()
	{
        return this.disciplinas;
    }

	public void setDisciplinas(
		Set< Disciplina > disciplinas )
	{
        this.disciplinas = disciplinas;
    }

	public Set< GrupoSala > getGruposSala()
	{
        return this.gruposSala;
    }

	public void setGruposSala( Set< GrupoSala > gruposSala )
	{
        this.gruposSala = gruposSala;
    }

	public Set< AtendimentoOperacional > getAtendimentosOperacionais()
	{
		return this.atendimentosOperacionais;
	}

	public void setAtendimentosOperacionais(
		Set< AtendimentoOperacional > atendimentosOperacionais )
	{
		this.atendimentosOperacionais = atendimentosOperacionais;
	}

	public Set< AtendimentoTatico > getAtendimentosTaticos()
	{
		return this.atendimentosTaticos;
	}

	public void setAtendimentosTaticos(
		Set< AtendimentoTatico > atendimentosTaticos )
	{
		this.atendimentosTaticos = atendimentosTaticos;
	}

	public Set< Fixacao > getFixacoes()
	{
		return this.fixacoes;
	}

	public void setFixacoes( Set< Fixacao > fixacoes )
	{
		this.fixacoes = fixacoes;
	}

	//@Override
	public int compareTo( Sala o )
	{
		return getCodigo().compareTo( o.getCodigo() );
	}

	public static int findCapacidadeMax(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario) {
    	Query q = entityManager().createQuery(
       		" SELECT MAX(o.capacidadeInstalada) FROM Sala o " +
       		" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
       		" AND o.unidade.campus.cenario = :cenario " );

       	q.setParameter( "cenario", cenario );
       	q.setParameter( "instituicaoEnsino", instituicaoEnsino );

       	return (Integer) q.getSingleResult();
	}

	public Set< Aula > getAulas() {
		return aulas;
	}
	
	public Set<Aula> getAulas(Semanas semana) {
		Set<Aula> aulas = new HashSet<Aula>();
		for (Aula aula : getAulas())
		{
			if(aula.getHorarioDisponivelCenario().getDiaSemana().equals(semana))
			{
				aulas.add(aula);
			}
		}
		return aulas;
	}

	public void setAulas(Set< Aula > aulas) {
		this.aulas = aulas;
	}

	public static Sala findProx(InstituicaoEnsino instituicaoEnsino,
			Cenario cenario, Sala sala) {
		
		Query q = entityManager().createQuery(
	       	" SELECT o FROM Sala o " +
    		" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " + 
    		" AND o.unidade.campus.cenario = :cenario " +
    		" AND o.codigo > :sala " +
    		" ORDER BY o.codigo");

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		q.setParameter( "sala", sala.getCodigo() );
		q.setMaxResults(1);

        return (Sala)q.getSingleResult();
	}
	
	public static Sala findAnt(InstituicaoEnsino instituicaoEnsino,
			Cenario cenario, Sala sala) {
		
		Query q = entityManager().createQuery(
	       	" SELECT o FROM Sala o " +
    		" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " + 
    		" AND o.unidade.campus.cenario = :cenario " +
    		" AND o.codigo < :sala " +
    		" ORDER BY o.codigo DESC");

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		q.setParameter( "sala", sala.getCodigo() );
		q.setMaxResults(1);

        return (Sala)q.getSingleResult();
	}

	public Sala clone(CenarioClone novoCenario) {
		Sala clone = new Sala();
		clone.setAndar(this.getAndar());
		clone.setCapacidadeInstalada(this.getCapacidadeInstalada());
		clone.setCapacidadeMax(this.getCapacidadeInstalada());
		clone.setCodigo(this.getCodigo());
		clone.setCustoOperacaoCred(this.getCustoOperacaoCred());
		clone.setDescricao(this.getDescricao());
		clone.setNumero(this.getNumero());
		clone.setTipoSala(novoCenario.getEntidadeClonada(this.getTipoSala()));
		clone.setUnidade(novoCenario.getEntidadeClonada(this.getUnidade()));
		clone.setExterna(this.getExterna());
		
		return clone;
	}

	public void cloneChilds(CenarioClone novoCenario, Sala entidadeClone) {
		
	}

	public Set< DisponibilidadeSala > getDisponibilidades() {
		return disponibilidades;
	}

	public void setDisponibilidades(Set< DisponibilidadeSala > disponibilidades) {
		this.disponibilidades = disponibilidades;
	}
	
	public boolean estaDisponivelNoHorario(HorarioDisponivelCenario hdc)
	{
		Calendar hdcHoraInicio = Calendar.getInstance();
		hdcHoraInicio.setTime(hdc.getHorarioAula().getHorario());
		hdcHoraInicio.set(1979,Calendar.NOVEMBER,6);
		
		Calendar hdcHoraFim = Calendar.getInstance();
		hdcHoraFim.setTime(hdc.getHorarioAula().getHorario());
		hdcHoraFim.set(1979,Calendar.NOVEMBER,6);
		hdcHoraFim.add(Calendar.MINUTE,hdc.getHorarioAula().getSemanaLetiva().getTempo());
		boolean estaContido = false;
		for (Disponibilidade disponibilidade : getDisponibilidades())
		{
			Calendar dispHoraInicio = Calendar.getInstance();
			dispHoraInicio.setTime(disponibilidade.getHorarioInicio());
			dispHoraInicio.set(1979,Calendar.NOVEMBER,6);
			
			Calendar dispHoraFim = Calendar.getInstance();
			dispHoraFim.setTime(disponibilidade.getHorarioFim());
			dispHoraFim.set(1979,Calendar.NOVEMBER,6);

			if (dispHoraInicio.compareTo(hdcHoraInicio) <= 0 && dispHoraFim.compareTo(hdcHoraFim) >= 0)
			{
				estaContido = true;
			}
		}
		
		return estaContido;
	}
}
