package com.gapso.web.trieda.server;

import java.io.ByteArrayOutputStream;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.transform.stream.StreamSource;

import org.springframework.stereotype.Repository;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Parametro;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.SolverInput;
import com.gapso.web.trieda.server.util.SolverOutput;
import com.gapso.web.trieda.server.util.solverclient.SolverClient;
import com.gapso.web.trieda.server.xml.input.TriedaInput;
import com.gapso.web.trieda.server.xml.output.ItemError;
import com.gapso.web.trieda.server.xml.output.ItemWarning;
import com.gapso.web.trieda.server.xml.output.TriedaOutput;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ErrorsWarningsInputSolverDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.services.OtimizarService;
import com.gapso.web.trieda.shared.util.view.TriedaException;

@Transactional
@Service
@Repository
public class OtimizarServiceImpl
	extends RemoteService
	implements OtimizarService
{
	private static final long serialVersionUID = 5716065588362358065L;
	private static final String linkSolverDefault = "http://localhost:3402/SolverWS";

	@Override
	@Transactional
	public ParametroDTO getParametro( CenarioDTO cenarioDTO )
	{
		InstituicaoEnsino instituicaoEnsino = this.getInstituicaoEnsinoUser();

		Cenario cenario = Cenario.find( cenarioDTO.getId(), instituicaoEnsino );
		Parametro parametro = cenario.getUltimoParametro( instituicaoEnsino );

		if ( parametro == null )
		{
			parametro = this.getParametroDefault( instituicaoEnsino, cenario );
		}

		ParametroDTO parametroDTO = ConvertBeans.toParametroDTO( parametro );
		return parametroDTO;
	}

	private Parametro getParametroDefault(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Parametro parametro = new Parametro();

		List< Campus > listCampus = Campus.findAll( instituicaoEnsino );
		List< Turno > listTurnos = Turno.findAll( instituicaoEnsino );

		if ( cenario == null )
		{
			CenariosServiceImpl cenariosService = new CenariosServiceImpl();
			CenarioDTO cenarioDTO = cenariosService.getMasterData();
			cenario = Cenario.find( cenarioDTO.getId(), instituicaoEnsino );
		}

		parametro.setCenario( cenario );
		parametro.setCampus( ( listCampus == null || listCampus.size() == 0 ? null : listCampus.get( 0 ) ) );
		parametro.setTurno( ( listTurnos == null || listTurnos.size() == 0 ? null : listTurnos.get( 0 ) ) );
		parametro.setSemanaLetiva( cenario.getSemanaLetiva() );

		return parametro;
	}

	@Override
	@Transactional
	public ErrorsWarningsInputSolverDTO validaInput(
		ParametroDTO parametroDTO ) throws Exception
	{
		if ( parametroDTO.getCampusId() == null
			|| parametroDTO.getTurnoId() == null )
		{
			String message = "";

			if ( parametroDTO.getCampusId() == null )
			{
				message += "o campus n&atilde;o foi informado, ";
			}

			if ( parametroDTO.getTurnoId() == null )
			{
				message += "o turno n&atilde;o foi informado, ";
			}

			message = message.substring( 0, message.length() - 2 );

			throw new TriedaException( new Exception(
				HtmlUtils.htmlUnescape( message ) ) );
		}

		Parametro parametro = ConvertBeans.toParametro( parametroDTO );

		parametro.setId( null );
		parametro.flush();
		parametro.save();

		Cenario cenario = parametro.getCenario();
		cenario.getParametros().add( parametro );
		List< Campus > campi = new ArrayList< Campus >( 1 );
		campi.add( parametro.getCampus() );

		TriedaInput triedaInput = null;
		SolverInput solverInput = new SolverInput(
			getInstituicaoEnsinoUser(), cenario, parametro, campi );

		if ( parametro.isTatico() )
		{
			triedaInput = solverInput.generateTaticoTriedaInput();
		}
		else
		{
			triedaInput = solverInput.generateOperacionalTriedaInput();
		}

		ErrorsWarningsInputSolverDTO response
			= new ErrorsWarningsInputSolverDTO();

		response.setValidInput( triedaInput != null );

		if ( solverInput.getErrors().size() > 0
			|| solverInput.getWarnings().size() > 0 )
		{
			for ( String error : solverInput.getErrors() )
			{
				response.getErrorsWarnings().get( "errors" ).add( error );
			}

			for ( String warning : solverInput.getWarnings() )
			{
				response.getErrorsWarnings().get( "warnings" ).add( warning );
			}
		}

		return response;
	}

	@Override
	@Transactional
	public Long sendInput( ParametroDTO parametroDTO )
	{
		if ( parametroDTO.getCampusId() == null
			|| parametroDTO.getTurnoId() == null )
		{
			String message = "";

			if ( parametroDTO.getCampusId() == null )
			{
				message += "o campus não foi informado, ";
			}

			if ( parametroDTO.getTurnoId() == null )
			{
				message += "o turno não foi informado, ";
			}

			message = message.substring( 0, message.length() - 2 );
			throw new RuntimeException( message );
		}

		Parametro parametro = ConvertBeans.toParametro( parametroDTO );

		parametro.setId( null );
		parametro.flush();
		parametro.save();

		Cenario cenario = parametro.getCenario();
		cenario.getParametros().add( parametro );
		List< Campus > campi = new ArrayList< Campus >( 1 );
		campi.add( parametro.getCampus() );

		TriedaInput triedaInput = null;
		SolverInput solverInput = new SolverInput(
			getInstituicaoEnsinoUser(), cenario, parametro, campi );

		if ( parametro.isTatico() )
		{
			triedaInput = solverInput.generateTaticoTriedaInput();
		}
		else
		{
			triedaInput = solverInput.generateOperacionalTriedaInput();
		}

		byte [] fileBytes = null;

		try
		{
			final ByteArrayOutputStream temp = new ByteArrayOutputStream();

			JAXBContext jc = JAXBContext.newInstance(
				"com.gapso.web.trieda.server.xml.input" );

			Marshaller m = jc.createMarshaller();
			m.setProperty( Marshaller.JAXB_FORMATTED_OUTPUT, true );
			m.setProperty( Marshaller.JAXB_ENCODING, "ISO-8859-1" );
			m.marshal( triedaInput, temp );
			fileBytes = temp.toByteArray();
		}
		catch ( Exception e )
		{
			e.printStackTrace();
		}

		SolverClient solverClient
			= new SolverClient( getLinkSolver(), "trieda" );

		return solverClient.requestOptimization( fileBytes );
	}

	@Override
	public Boolean isOptimizing( Long round )
	{
		SolverClient solverClient
			= new SolverClient( getLinkSolver(), "trieda" );

		return ( !solverClient.isFinished( round ) );
	}

	@Override
	public Map< String, List< String > > saveContent(
		CenarioDTO cenarioDTO, Long round )
	{
		Cenario cenario = Cenario.find(
			cenarioDTO.getId(), this.getInstituicaoEnsinoUser() );

		Map< String, List< String > > ret
			= new HashMap< String, List< String > >( 2 );

		ret.put( "warning", new ArrayList< String >() );
		ret.put( "error", new ArrayList< String >() );

		try
		{
			SolverClient solverClient = new SolverClient( getLinkSolver(), "trieda" );
			byte [] xmlBytes = solverClient.getContent( round );

			if ( xmlBytes == null )
			{
				ret.get( "error" ).add( "Erro no servidor" );
				return ret;
			}

			JAXBContext jc = JAXBContext.newInstance(
				"com.gapso.web.trieda.server.xml.output" );

			Unmarshaller u = jc.createUnmarshaller();
			StringBuffer xmlStr = new StringBuffer( new String( xmlBytes ) );

			TriedaOutput triedaOutput = ( TriedaOutput ) u.unmarshal(
				new StreamSource( new StringReader( xmlStr.toString() ) ) );

			for ( ItemError erro : triedaOutput.getErrors().getError() )
			{
				ret.get( "error" ).add( erro.getMessage() );
			}

			for ( ItemWarning warning : triedaOutput.getWarnings().getWarning() )
			{
				ret.get( "warning" ).add( warning.getMessage() );
			}

			if ( !triedaOutput.getErrors().getError().isEmpty() )
			{
				return ret;
			}

			Parametro parametro = cenario.getUltimoParametro(
				this.getInstituicaoEnsinoUser() );

			SolverOutput solverOutput = new SolverOutput(
				getInstituicaoEnsinoUser(), cenario, triedaOutput );

			solverOutput.generateAlunosDemanda();
			solverOutput.salvarAlunosDemanda(
				parametro.getCampus(), parametro.getTurno() );

			if ( parametro.isTatico() )
			{
				solverOutput.generateAtendimentosTatico();

				solverOutput.salvarAtendimentosTatico(
					parametro.getCampus(), parametro.getTurno() );
			}
			else
			{
				solverOutput.generateAtendimentosOperacional();

				solverOutput.salvarAtendimentosOperacional(
					parametro.getCampus(), parametro.getTurno() );
			}
		}
		catch ( JAXBException e )
		{
			e.printStackTrace();
			ret.get( "error" ).add( "Erro ao salvar o resultado na base de dados" );
			return ret;
		}

		return ret;
	}

	private String getLinkSolver()
	{
		// TODO -- Ler o link do solver a partir do arquivo properties
		String link = OtimizarServiceImpl.linkSolverDefault;
		return link;
	}
}
