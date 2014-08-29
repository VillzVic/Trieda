package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import javax.naming.NamingException;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Authority;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Parametro;
import com.gapso.trieda.domain.ParametroConfiguracao;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.TipoDisciplina;
import com.gapso.trieda.domain.TipoSala;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.trieda.domain.Usuario;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ParametroConfiguracaoDTO;
import com.gapso.web.trieda.shared.services.InstituicaoEnsinoService;
import com.gapso.web.trieda.shared.util.view.TriedaException;

public class InstituicaoEnsinoServiceImpl
	extends RemoteService implements InstituicaoEnsinoService
{
	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public InstituicaoEnsinoDTO getInstituicaoEnsino( Long id )
	{
		return ConvertBeans.toInstituicaoEnsinoDTO(	InstituicaoEnsino.find( id ) );
	}

	@Override
	public ListLoadResult< InstituicaoEnsinoDTO > getList()
	{
		List< InstituicaoEnsinoDTO > listDTO
			= new ArrayList< InstituicaoEnsinoDTO >();

		List< InstituicaoEnsino > list = InstituicaoEnsino.findAll();

		for ( InstituicaoEnsino instituicaoEnsino : list )
		{
			listDTO.add( ConvertBeans.toInstituicaoEnsinoDTO( instituicaoEnsino ) );
		}

		return new BaseListLoadResult< InstituicaoEnsinoDTO >( listDTO );
	}
	
	@Override
	public ParametroConfiguracaoDTO getConfiguracoes()
	{
		ParametroConfiguracaoDTO configs =
				ConvertBeans.toParametroConfiguracaoDTO( ParametroConfiguracao.findConfiguracoes(getInstituicaoEnsinoUser()) );
		try {
			configs.setDataSource(getDataSourceName());
		} catch (NamingException e) {
			configs.setDataSource("");
		}
		
		return configs;
	}
	
	@Override
	public void saveConfiguracoes( ParametroConfiguracaoDTO parametroConfiguracaoDTO )
	{
		ParametroConfiguracao configs = ParametroConfiguracao.findConfiguracoes(getInstituicaoEnsinoUser());
		
		if (configs == null)
		{
			configs = ConvertBeans.toParametroConfiguracao( parametroConfiguracaoDTO );
			configs.persist();
		}
		else
		{
			configs.setUrlOtimizacao( parametroConfiguracaoDTO.getUrlOtimizacao() );
			configs.setNomeOtimizacao( parametroConfiguracaoDTO.getNomeOtimizacao() );
			
			configs.merge();
		}
	}
	
	@Override
	public PagingLoadResult< InstituicaoEnsinoDTO > getBuscaList(
		String nome, PagingLoadConfig config )
	{
		List< InstituicaoEnsinoDTO > list = new ArrayList< InstituicaoEnsinoDTO >();
		String orderBy = config.getSortField();

		if ( orderBy != null )
		{
			if ( config.getSortDir() != null
				&& config.getSortDir().equals( SortDir.DESC ) )
			{
				orderBy = ( orderBy + " asc" );
			}
			else
			{
				orderBy = ( orderBy + " desc" );
			}
		}

		List< InstituicaoEnsino > listInstituicoes = InstituicaoEnsino.findBy( nome, 
				config.getOffset(), config.getLimit(), orderBy );

		for ( InstituicaoEnsino instituicao : listInstituicoes )
		{
			list.add( ConvertBeans.toInstituicaoEnsinoDTO( instituicao ) );
		}

		BasePagingLoadResult< InstituicaoEnsinoDTO > result
			= new BasePagingLoadResult< InstituicaoEnsinoDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( InstituicaoEnsino.count(
			nome) );

		return result;
	}
	
	@Override
	@Transactional
	public void remove( List< InstituicaoEnsinoDTO > instituicaoEnsinoDTOList ) throws TriedaException
	{
		try {
			for ( InstituicaoEnsinoDTO instituicaoEnsinoDTO : instituicaoEnsinoDTOList )
			{
				InstituicaoEnsino instituicaoEnsino = ConvertBeans.toInstituicaoEnsino( instituicaoEnsinoDTO );
				for (Cenario cenario : Cenario.findAll(instituicaoEnsino))
				{
					cenario.remove();
				}
				Cenario.findMasterData(instituicaoEnsino).remove();
				for (Usuario usuario : Usuario.findBy(instituicaoEnsino))
				{
					usuario.remove();
				}
				for (Authority authority : Authority.findBy(instituicaoEnsino))
				{
					authority.remove();
				}
				for (Parametro parametro : Parametro.findAll(instituicaoEnsino))
				{
					parametro.remove();
				}
				ParametroConfiguracao.findBy(instituicaoEnsino).remove();
				instituicaoEnsino.remove();
			}
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
	}
	
	@Override
	public void save( InstituicaoEnsinoDTO instituicaoEnsinoDTO ) throws TriedaException
	{
		try {
			InstituicaoEnsino instituicaoEnsino = ConvertBeans.toInstituicaoEnsino( instituicaoEnsinoDTO );
			if ( instituicaoEnsino.getId() != null
				&& instituicaoEnsino.getId() >= 0 )
			{
				instituicaoEnsino.merge();
			}
			else
			{
				instituicaoEnsino.persist();
				createMasterData(instituicaoEnsino);
			}
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
	}

	private void createMasterData(InstituicaoEnsino instituicaoEnsino) {
		Cenario cenario = new Cenario();

		cenario.setOficial( false );
		cenario.setMasterData( true );
		cenario.setNome( "MASTER DATA" );
		cenario.setAno( 1 );
		cenario.setSemestre( 1 );
		cenario.setComentario( "MASTER DATA" );
		cenario.setInstituicaoEnsino(instituicaoEnsino);
		cenario.setCriadoPor(getUsuario());
		cenario.setDataCriacao(new Date());
		cenario.setAtualizadoPor(getUsuario());
		cenario.setDataAtualizacao(new Date());
		cenario.persist();
		criarTiposContrato(cenario);
		criarTiposDisciplina(cenario);
		criarTiposSala(cenario);
		criarTitulacoes(cenario);
		
	}
	
	private void criarTiposSala(Cenario cenario) {
		TipoSala tipo1 = new TipoSala();
		tipo1.setNome( TipoSala.TIPO_SALA_DE_AULA );
		tipo1.setDescricao( TipoSala.TIPO_SALA_DE_AULA );
		tipo1.setAceitaAulaPratica( false );
		tipo1.setInstituicaoEnsino( getInstituicaoEnsinoUser() );
		tipo1.setCenario(cenario);
		tipo1.persist();

		TipoSala tipo2 = new TipoSala();
		tipo2.setNome( "Laboratório"  );
		tipo2.setDescricao( "Laboratório"  );
		tipo2.setAceitaAulaPratica( true );
		tipo2.setInstituicaoEnsino( getInstituicaoEnsinoUser() );
		tipo2.setCenario(cenario);
		tipo2.persist();
	}

	private void criarTitulacoes(Cenario cenario) {
		Titulacao titulacao2 = new Titulacao();
		titulacao2.setCenario(cenario);
		titulacao2.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		titulacao2.setNome("Bacharel");
		titulacao2.persist();
		
		Titulacao titulacao3 = new Titulacao();
		titulacao3.setCenario(cenario);
		titulacao3.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		titulacao3.setNome("Especialista");
		titulacao3.persist();
		
		Titulacao titulacao4 = new Titulacao();
		titulacao4.setCenario(cenario);
		titulacao4.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		titulacao4.setNome("Mestre");
		titulacao4.persist();
		
		Titulacao titulacao5 = new Titulacao();
		titulacao5.setCenario(cenario);
		titulacao5.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		titulacao5.setNome("Doutor");
		titulacao5.persist();
	}

	private void criarTiposDisciplina(Cenario cenario) {
		TipoDisciplina tipo1 = new TipoDisciplina();
		tipo1.setCenario(cenario);
		tipo1.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		tipo1.setNome("Presencial");
		tipo1.persist();
		
		TipoDisciplina tipo3 = new TipoDisciplina();
		tipo3.setCenario(cenario);
		tipo3.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		tipo3.setNome("Online");
		tipo3.persist();
	}

	private void criarTiposContrato(Cenario cenario) {
		TipoContrato tipo1 = new TipoContrato();
		tipo1.setCenario(cenario);
		tipo1.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		tipo1.setNome("Horista");
		tipo1.persist();
		
		TipoContrato tipo2 = new TipoContrato();
		tipo2.setCenario(cenario);
		tipo2.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		tipo2.setNome("Tempo Parcial");
		tipo2.persist();
		
		TipoContrato tipo3 = new TipoContrato();
		tipo3.setCenario(cenario);
		tipo3.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		tipo3.setNome("Tempo Integral");
		tipo3.persist();	
	}
}
