package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
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
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.misc.Estados;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "CAM_ID")
@Table(name = "CAMPI", uniqueConstraints = @UniqueConstraint(columnNames = {
		"CAM_CODIGO", "CEN_ID" }))
public class Campus implements Serializable, Comparable< Campus >
{
	private static final long serialVersionUID = 6690100103369325015L;

	@NotNull
	@ManyToOne(targetEntity = Cenario.class)
	@JoinColumn(name = "CEN_ID")
	private Cenario cenario;

	@NotNull
	@Column(name = "CAM_CODIGO")
	@Size(min = 1, max = 20)
	private String codigo;

	@NotNull
	@Column(name = "CAM_NOME")
	@Size(min = 1, max = 50)
	private String nome;

	@Column(name = "CAM_VALOR_CREDITO")
	@Digits(integer = 6, fraction = 2)
	private Double valorCredito;

	@Enumerated
	@Column(name = "CAM_ESTADO")
	private Estados estado;

	@Column(name = "CAM_MUNICIPIO")
	@Size(max = 25)
	private String municipio;

	@Column(name = "CAM_BAIRRO")
	@Size(max = 25)
	private String bairro;

	@Column(name = "CAM_PUBLICAR")
	private Boolean publicado;

	@OneToMany(cascade = CascadeType.ALL, mappedBy = "campus")
	private Set<Unidade> unidades = new HashSet<Unidade>();

	@OneToMany(cascade = CascadeType.ALL, mappedBy = "origem")
	private Set<DeslocamentoCampus> deslocamentos = new HashSet<DeslocamentoCampus>();

	@OneToMany(cascade = CascadeType.ALL, mappedBy = "destino")
	private Set<DeslocamentoCampus> deslocamentosDestino = new HashSet<DeslocamentoCampus>();

	@ManyToMany(cascade = { CascadeType.PERSIST, CascadeType.MERGE }, mappedBy = "campi")
	private Set<Professor> professores = new HashSet<Professor>();

	@ManyToMany(cascade = { CascadeType.PERSIST, CascadeType.MERGE }, mappedBy = "campi")
	private Set<HorarioDisponivelCenario> horarios = new HashSet<HorarioDisponivelCenario>();

	@NotNull
	@OneToMany(cascade = CascadeType.ALL, mappedBy = "campus")
	private Set<Oferta> ofertas = new HashSet<Oferta>();

	@OneToMany(cascade = CascadeType.ALL, mappedBy = "campus")
	private Set<Parametro> parametros = new HashSet<Parametro>();

	public Cenario getCenario() {
		return this.cenario;
	}

	public void setCenario(Cenario cenario) {
		this.cenario = cenario;
	}

	public String getCodigo() {
		return this.codigo;
	}

	public void setCodigo(String codigo) {
		this.codigo = codigo;
	}

	public String getNome() {
		return this.nome;
	}

	public void setNome(String nome) {
		this.nome = nome;
	}

	public Double getValorCredito() {
		return this.valorCredito;
	}

	public void setValorCredito(Double valorCredito) {
		this.valorCredito = valorCredito;
	}

	public Estados getEstado() {
		return estado;
	}

	public void setEstado(Estados estado) {
		this.estado = estado;
	}

	public String getMunicipio() {
		return municipio;
	}

	public void setMunicipio(String municipio) {
		this.municipio = municipio;
	}

	public String getBairro() {
		return bairro;
	}

	public void setBairro(String bairro) {
		this.bairro = bairro;
	}

	public Set<Unidade> getUnidades() {
		return this.unidades;
	}

	public void setUnidades(Set<Unidade> unidades) {
		this.unidades = unidades;
	}

	public Set<DeslocamentoCampus> getDeslocamentos() {
		return this.deslocamentos;
	}

	public void setDeslocamentos(Set<DeslocamentoCampus> deslocamentos) {
		this.deslocamentos = deslocamentos;
	}

	public Set<DeslocamentoCampus> getDeslocamentosDestino() {
		return this.deslocamentosDestino;
	}

	public void setDeslocamentosDestino(
			Set<DeslocamentoCampus> deslocamentosDestino) {
		this.deslocamentosDestino = deslocamentosDestino;
	}

	public Set<Professor> getProfessores() {
		return this.professores;
	}

	public void setProfessores(Set<Professor> professores) {
		this.professores = professores;
	}

	private Set<HorarioDisponivelCenario> getHorarios() {
		return this.horarios;
	}

	public void setHorarios(Set<HorarioDisponivelCenario> horarios) {
		this.horarios = horarios;
	}

	public Set<Oferta> getOfertas() {
		return this.ofertas;
	}

	public void setOfertas(Set<Oferta> ofertas) {
		this.ofertas = ofertas;
	}

	public Boolean getPublicado() {
		return publicado;
	}

	public void setPublicado(Boolean publicado) {
		this.publicado = publicado;
	}

	public Set<Parametro> getParametros() {
		return parametros;
	}

	public void setParametros(Set<Parametro> parametros) {
		this.parametros = parametros;
	}

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue(strategy = GenerationType.AUTO)
	@Column(name = "CAM_ID")
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
		preencheHorarios();
	}

	@Transactional
	public void remove()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		if (this.entityManager.contains( this ) )
		{
			removeProfessores();
			removeHorariosDisponivelCenario();
			removeDeslocamentosDestino();
			removeUnidades();

			this.entityManager.remove( this );
		}
		else
		{
			Campus attached = this.entityManager.find( this.getClass(), this.id );

			attached.removeProfessores();
			attached.removeHorariosDisponivelCenario();
			attached.removeDeslocamentosDestino();
			attached.removeUnidades();

			this.entityManager.remove( attached );
		}
	}

	public void preencheHorarios() {
		for (SemanaLetiva semanaLetiva : SemanaLetiva.findAll()) {
			for (HorarioAula horarioAula : semanaLetiva.getHorariosAula()) {
				for (HorarioDisponivelCenario hdc : horarioAula
						.getHorariosDisponiveisCenario()) {
					hdc.getCampi().add(this);
					hdc.merge();
				}
			}
		}
	}

	@Transactional
	public void removeProfessores() {
		Set<Professor> professores = this.getProfessores();
		for (Professor professor : professores) {
			professor.getCampi().remove(this);
			professor.merge();
		}
	}

	@Transactional
	public void removeHorariosDisponivelCenario() {
		Set<HorarioDisponivelCenario> horarios = this.getHorarios();
		for (HorarioDisponivelCenario horario : horarios) {
			horario.getCampi().remove(this);
			horario.merge();
		}
	}

	@Transactional
	public void removeDeslocamentosDestino() {
		Set<DeslocamentoCampus> deslocamentosDestino = this
				.getDeslocamentosDestino();
		for (DeslocamentoCampus deslocamentoDestino : deslocamentosDestino) {
			deslocamentoDestino.getOrigem().getDeslocamentos()
					.remove(deslocamentoDestino);
			deslocamentoDestino.getOrigem().merge();
		}
	}

	@Transactional
	public void removeUnidades()
	{
		Set< Unidade > unidades = this.getUnidades();
		for ( Unidade unidade : unidades )
		{
			unidade.remove();
		}
	}

	@Transactional
	public void flush() {
		if (this.entityManager == null)
			this.entityManager = entityManager();
		this.entityManager.flush();
	}

	@Transactional
	public Campus merge() {
		if (this.entityManager == null)
			this.entityManager = entityManager();
		Campus merged = this.entityManager.merge(this);
		this.entityManager.flush();
		return merged;
	}

	public static final EntityManager entityManager() {
		EntityManager em = new Campus().entityManager;
		if (em == null)
			throw new IllegalStateException(
					"Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
		return em;
	}

	public static int count() {
		return ((Number) entityManager().createQuery(
				"SELECT COUNT(o) FROM Campus o").getSingleResult()).intValue();
	}

	public static int count(Cenario cenario) {
		Query q = entityManager().createQuery(
				"SELECT COUNT(o) FROM Campus o WHERE o.cenario = :cenario");
		q.setParameter("cenario", cenario);
		return ((Number) q.getSingleResult()).intValue();
	}

	@SuppressWarnings("unchecked")
	public static List<Campus> findAll() {
		return entityManager().createQuery("SELECT o FROM Campus o")
				.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List<Campus> findAllOtimized() {
		Query q = entityManager()
				.createQuery(
						"SELECT DISTINCT(o.oferta.campus) FROM AtendimentoOperacional o");
		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List<Campus> findByCenario(Cenario cenario) {
		Query q = entityManager().createQuery(
				"SELECT o FROM Campus o WHERE cenario = :cenario");
		q.setParameter("cenario", cenario);
		return q.getResultList();
	}

	public static Map<String, Campus> buildCampusCodigoToCampusMap(
			List<Campus> campi) {
		Map<String, Campus> campusMap = new HashMap<String, Campus>();
		for (Campus campus : campi) {
			campusMap.put(campus.getCodigo(), campus);
		}
		return campusMap;
	}

	public static Campus find(Long id) {
		if (id == null)
			return null;
		return entityManager().find(Campus.class, id);
	}

	public static List<Campus> find(int firstResult, int maxResults) {
		return find(firstResult, maxResults, null);
	}

	@SuppressWarnings("unchecked")
	public static List<Campus> find(int firstResult, int maxResults,
			String orderBy) {
		orderBy = (orderBy != null) ? "ORDER BY o." + orderBy : "";
		return entityManager().createQuery("SELECT o FROM Campus o " + orderBy)
				.setFirstResult(firstResult).setMaxResults(maxResults)
				.getResultList();
	}

	public static Campus getByCodigo(Cenario cenario, String codigo) {
		Query q = entityManager()
				.createQuery(
						"SELECT o FROM Campus o WHERE cenario = :cenario AND codigo = :codigo");
		q.setParameter("cenario", cenario);
		q.setParameter("codigo", codigo);
		return (Campus) q.getSingleResult();
	}

	public static int count(Cenario cenario, String nome, String codigo,
			Estados estado, String municipio, String bairro) {
		nome = (nome == null) ? "" : nome;
		nome = "%" + nome.replace('*', '%') + "%";
		codigo = (codigo == null) ? "" : codigo;
		codigo = "%" + codigo.replace('*', '%') + "%";

		String municipioQuery = "";
		if (municipio != null) {
			municipioQuery = " LOWER(o.municipio) LIKE LOWER(:municipio) AND ";
			municipio = "%" + municipio.replace('*', '%') + "%";
		}

		String bairroQuery = "";
		if (bairro != null) {
			bairroQuery = " LOWER(o.bairro) LIKE LOWER(:bairro) AND ";
			bairro = "%" + bairro.replace('*', '%') + "%";
		}

		EntityManager em = Campus.entityManager();
		String estadoQuery = (estado == null) ? "" : "o.estado = :estado AND ";
		Query q = em.createQuery("SELECT COUNT(o) FROM Campus o WHERE "
				+ "LOWER(o.nome) LIKE LOWER(:nome) AND "
				+ "LOWER(o.codigo) LIKE LOWER(:codigo) AND "
				+ "o.cenario = :cenario AND " + estadoQuery + bairroQuery
				+ municipioQuery + " 1=1 ");
		q.setParameter("nome", nome);
		q.setParameter("codigo", codigo);
		q.setParameter("cenario", cenario);
		if (estado != null)
			q.setParameter("estado", estado);
		if (municipio != null)
			q.setParameter("municipio", municipio);
		if (bairro != null)
			q.setParameter("bairro", bairro);
		return ((Number) q.getSingleResult()).intValue();
	}

	@SuppressWarnings("unchecked")
	public static List<Campus> findBy( Cenario cenario, String nome,
			String codigo, Estados estado, String municipio, String bairro,
			int firstResult, int maxResults, String orderBy )
	{
		nome = ( ( nome == null ) ? "" : nome );
		nome = ( "%" + nome.replace( '*', '%' ) + "%" );
		codigo = ( ( codigo == null ) ? "" : codigo );
		codigo = ( "%" + codigo.replace('*', '%') + "%" );

		String municipioQuery = "";
		if ( municipio != null )
		{
			municipioQuery = " LOWER(o.municipio) LIKE LOWER(:municipio) AND ";
			municipio = ( "%" + municipio.replace( '*', '%' ) + "%" );
		}

		String bairroQuery = "";
		if ( bairro != null )
		{
			bairroQuery = ( " LOWER(o.bairro) LIKE LOWER(:bairro) AND " );
			bairro = ( "%" + bairro.replace( '*', '%' ) + "%" );
		}

		EntityManager em = Campus.entityManager();
		orderBy = ( ( orderBy != null ) ? "ORDER BY o." + orderBy : "" );
		String estadoQuery = ( ( estado == null ) ? "" : "o.estado = :estado AND " );
		Query q = em.createQuery( "SELECT o FROM Campus o WHERE "
				+ "LOWER(o.nome) LIKE LOWER(:nome) AND "
				+ "LOWER(o.codigo) LIKE LOWER(:codigo) AND "
				+ "o.cenario = :cenario AND " + estadoQuery + bairroQuery
				+ municipioQuery + " 1=1 " + orderBy );

		q.setParameter( "nome", nome );
		q.setParameter( "codigo", codigo );
		q.setParameter( "cenario", cenario );

		if ( estado != null )
		{
			q.setParameter("estado", estado );
		}

		if ( municipio != null )
		{
			q.setParameter( "municipio", municipio );
		}

		if ( bairro != null )
		{
			q.setParameter( "bairro", bairro );
		}

		return q.setFirstResult( firstResult ).setMaxResults( maxResults ).getResultList();
	}

	@SuppressWarnings("unchecked")
	public List< HorarioDisponivelCenario > getHorarios( SemanaLetiva semanaLetiva )
	{
		Query q = entityManager().createQuery(
						"SELECT o FROM HorarioDisponivelCenario o, IN (o.campi) c "
					+ "WHERE c = :campus AND o.horarioAula.semanaLetiva = :semanaLetiva" );

		q.setParameter( "campus", this );
		q.setParameter( "semanaLetiva", semanaLetiva );

		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List<Campus> findByCurriculo(Curriculo curriculo) {
		Query q = entityManager()
				.createQuery(
						"SELECT c FROM Campus c, IN (c.ofertas) o WHERE o.curriculo = :curriculo");
		q.setParameter("curriculo", curriculo);
		return q.getResultList();
	}

	public boolean isOtimizadoTatico() {
		Query q = entityManager()
				.createQuery(
						"SELECT COUNT(o) FROM AtendimentoTatico o WHERE o.oferta.campus = :campus");
		q.setParameter("campus", this);
		Number size = (Number) q.setMaxResults(1).getSingleResult();
		return size.intValue() > 0;
	}

	public boolean isOtimizadoOperacional() {
		Query q = entityManager()
				.createQuery(
						"SELECT COUNT(o) FROM AtendimentoOperacional o WHERE o.oferta.campus = :campus");
		q.setParameter("campus", this);
		Number size = (Number) q.setMaxResults(1).getSingleResult();
		return size.intValue() > 0;
	}

	public boolean isOtimizadoTotal() {
		return isOtimizadoTatico() && isOtimizadoOperacional();
	}

	public boolean isOtimizado() {
		return isOtimizadoTatico() || isOtimizadoOperacional();
	}

	public static boolean checkCodigoUnique(Cenario cenario, String codigo) {
		Query q = entityManager()
				.createQuery(
						"SELECT COUNT(o) FROM Campus o WHERE o.cenario = :cenario AND o.codigo = :codigo");
		q.setParameter("cenario", cenario);
		q.setParameter("codigo", codigo);
		Number size = (Number) q.setMaxResults(1).getSingleResult();
		return size.intValue() > 0;
	}

	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("Id: ").append(getId()).append(", ");
		sb.append("Version: ").append(getVersion()).append(", ");
		sb.append("Cenario: ").append(getCenario()).append(", ");
		sb.append("Codigo: ").append(getCodigo()).append(", ");
		sb.append("Nome: ").append(getNome()).append(", ");
		sb.append("ValorCredito: ").append(getValorCredito()).append(", ");
		sb.append("Estado: ").append(getEstado().name()).append(", ");
		sb.append("Municipio: ").append(getMunicipio()).append(", ");
		sb.append("Bairro: ").append(getBairro()).append(", ");
		sb.append("Unidades: ")
				.append(getUnidades() == null ? "null" : getUnidades().size())
				.append(", ");
		sb.append("Deslocamentos: ")
				.append(getDeslocamentos() == null ? "null"
						: getDeslocamentos().size()).append(", ");
		sb.append("DeslocamentosDestino: ")
				.append(getDeslocamentosDestino() == null ? "null"
						: getDeslocamentosDestino().size()).append(", ");
		sb.append("Professores: ")
				.append(getProfessores() == null ? "null" : getProfessores()
						.size()).append(", ");
		sb.append("Horarios: ")
				.append(getHorarios() == null ? "null" : getHorarios().size())
				.append(", ");
		sb.append("Ofertas: ").append(
				getOfertas() == null ? "null" : getOfertas().size());
		sb.append("Publicado: ").append(getPublicado());
		sb.append("Parametros: ")
				.append(getParametros() == null ? "null" : getParametros()
						.size()).append(", ");
		return sb.toString();
	}

	@Override
	public int compareTo(Campus c) {
		return this.getCodigo().compareTo(c.getCodigo());
	}
}
