package com.gapso.web.trieda.server.excel.exp;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.springframework.security.core.context.SecurityContext;
import org.springframework.security.core.context.SecurityContextHolder;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Usuario;
import com.gapso.web.trieda.server.util.GTriedaI18nConstants;
import com.gapso.web.trieda.server.util.GTriedaI18nMessages;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class ExportExcelServlet extends HttpServlet
{	
	private static final long serialVersionUID = -7987228694777660184L;
	private static TriedaI18nMessages i18nMessages = null;
	private static TriedaI18nConstants i18nConstants = null;

	private Cenario cenario = null;
	{
		ExportExcelServlet.i18nConstants = new GTriedaI18nConstants();
		ExportExcelServlet.i18nMessages = new GTriedaI18nMessages();
	}

	private InstituicaoEnsino getInstituicaoEnsino()
	{
		SecurityContext context = SecurityContextHolder.getContext();
		String username = context.getAuthentication().getName();
		Usuario usuario = Usuario.find( username );
		return ( usuario == null ? null : usuario.getInstituicaoEnsino() );
	}

	@Override
	protected void doGet(
		HttpServletRequest request, HttpServletResponse response )
		throws ServletException, IOException
	{
		// Obtém os parâmetros
		String informationToBeExported = request.getParameter(
				ExcelInformationType.getInformationParameterName() );

		Long instituicaoEnsinoId = null;
		
		try
		{
			instituicaoEnsinoId = Long.parseLong(
				request.getParameter( "instituicaoEnsinoId" ) );
		}
		catch( Exception e )
		{
			System.out.println(
				"Não foi informado a instituição de ensino: " + informationToBeExported );
		}

		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( instituicaoEnsinoId );

		this.cenario = Cenario.findMasterData( instituicaoEnsino );

		if ( !informationToBeExported.isEmpty() )
		{
			ExportExcelFilter filter = null;
			if ( informationToBeExported.equals(
				ExcelInformationType.RELATORIO_VISAO_CURSO.toString() ) )
			{
				filter = verificaParametrosVisaoCurso( request );
			}
			else if ( informationToBeExported.equals(
				ExcelInformationType.RELATORIO_VISAO_SALA.toString() ) )
			{
				filter = verificaParametrosVisaoSala( request );
			}
			else if ( informationToBeExported.equals(
				ExcelInformationType.RELATORIO_VISAO_PROFESSOR.toString() ) )
			{
				filter = verificaParametrosVisaoProfessor( request );
			}
			else if ( informationToBeExported.equals(
				ExcelInformationType.RESUMO_DISCIPLINA.toString() ) )
			{
				filter = verificaParametrosResumoDisciplina( request );
			}
			else if ( informationToBeExported.equals(
				ExcelInformationType.RESUMO_CURSO.toString() ) )
			{
				filter = verificaParametrosResumoCurso( request );
			}

			// Get Excel Data
			IExportExcel exporter = ExportExcelFactory.createExporter(
				informationToBeExported, this.cenario, ExportExcelServlet.i18nConstants,
				ExportExcelServlet.i18nMessages, filter, getInstituicaoEnsino() );

			HSSFWorkbook workbook = exporter.export();

			if ( exporter.getErrors().isEmpty() )
			{
				// Write data on response output stream
				writeExcelToHttpResponse( exporter.getFileName(), workbook, response );
			}
			else
			{
				response.setContentType( "text/html" );

				for ( String msg : exporter.getWarnings() )
				{
					response.getWriter().println(
						ExcelInformationType.prefixWarning() + msg );
				}

				for ( String msg : exporter.getErrors() )
				{
					response.getWriter().println(
						ExcelInformationType.prefixError() + msg );
				}

				response.getWriter().flush();
			}
		}
	}

	private void writeExcelToHttpResponse( String excelFileName,
		HSSFWorkbook excel, HttpServletResponse response ) throws IOException
	{
		response.setContentType( "application/vnd.ms-excel" );  
		response.setHeader( "Content-disposition", "attachment; filename=" + excelFileName + ".xls" );

		ServletOutputStream out = null;
		try
		{
			out = response.getOutputStream();
			excel.write( out );
			out.flush();
		}
		catch ( IOException e )
		{
			throw e;
		}
		finally
		{
			if ( out != null )
			{
		        out.close();
			}
		}
	}

	private RelatorioVisaoProfessorFiltroExcel verificaParametrosVisaoProfessor( HttpServletRequest request )
	{
		Long campusId = null;
		Long turnoId = null;
		Long professorId = null;
		Long professorVirtualId = null;
		Long instituicaoEnsinoId = null;

		try
		{
			campusId = Long.parseLong( request.getParameter( "campusId" ) );
			turnoId = Long.parseLong( request.getParameter( "turnoId" ) );
			professorId = Long.parseLong( request.getParameter( "professorId" ) );
			professorVirtualId = Long.parseLong( request.getParameter( "professorVirtualId" ) );
			instituicaoEnsinoId = Long.parseLong( request.getParameter( "instituicaoEnsinoId" ) );

		}
		catch( Exception ex ) { return null; }

		RelatorioVisaoProfessorFiltroExcel filtro = new RelatorioVisaoProfessorFiltroExcel(
			campusId, turnoId, professorId, professorVirtualId, instituicaoEnsinoId );

		return filtro;
	}

	private RelatorioVisaoCursoFiltroExcel verificaParametrosVisaoCurso( HttpServletRequest request )
	{
		Long cursoId = null;
		Long curriculoId = null;
		Long campusId = null;
		Integer periodoId = null;
		Long turnoId = null;
		Long instituicaoEnsinoId = null;

		try
		{
			cursoId = Long.parseLong( request.getParameter( "cursoId" ) );
			curriculoId = Long.parseLong( request.getParameter( "curriculoId" ) );
			campusId = Long.parseLong( request.getParameter( "campusId" ) );
			periodoId = Integer.parseInt( request.getParameter( "periodoId" ) );
			turnoId = Long.parseLong( request.getParameter( "turnoId" ) );
			instituicaoEnsinoId = Long.parseLong( request.getParameter( "instituicaoEnsinoId" ) );
		}
		catch( Exception ex ) { return null; }

		RelatorioVisaoCursoFiltroExcel filtro = new RelatorioVisaoCursoFiltroExcel(
			cursoId, curriculoId, campusId, periodoId, turnoId, instituicaoEnsinoId );
		return filtro;
	}
	
	private RelatorioVisaoSalaFiltroExcel verificaParametrosVisaoSala( HttpServletRequest request )
	{
		Long campusId = null;
		Long unidadeId = null;
		Long salaId = null;
		Long turnoId = null;
		Long semanaLetivaId = null;
		Long instituicaoEnsinoId = null;

		try
		{
			campusId = Long.parseLong( request.getParameter( "campusId" ) );
			unidadeId = Long.parseLong( request.getParameter( "unidadeId" ) );
			salaId = Long.parseLong( request.getParameter( "salaId" ) );
			turnoId = Long.parseLong( request.getParameter( "turnoId" ) );
			semanaLetivaId = Long.parseLong( request.getParameter( "semanaLetivaId" ) );
			instituicaoEnsinoId = Long.parseLong( request.getParameter( "instituicaoEnsinoId" ) );
		}
		catch( Exception ex ) { return null; }

		RelatorioVisaoSalaFiltroExcel filtro = new RelatorioVisaoSalaFiltroExcel(
			campusId, unidadeId, salaId, turnoId, semanaLetivaId, instituicaoEnsinoId );

		return filtro;
	}

	private ResumoDisciplinaFiltroExcel verificaParametrosResumoDisciplina( HttpServletRequest request )
	{
		Long campusId = null;
		Long instituicaoEnsinoId = null;

		try
		{
			campusId = Long.parseLong( request.getParameter( "campusId" ) );
			instituicaoEnsinoId = Long.parseLong( request.getParameter( "instituicaoEnsinoId" ) );
		}
		catch( Exception ex )
		{
			return null;
		}

		ResumoDisciplinaFiltroExcel filtro
			= new ResumoDisciplinaFiltroExcel( instituicaoEnsinoId, campusId );

		return filtro;
	}

	private ResumoCursoFiltroExcel verificaParametrosResumoCurso( HttpServletRequest request )
	{
		Long campusId = null;
		Long instituicaoEnsinoId = null;

		try
		{
			campusId = Long.parseLong( request.getParameter( "campusId" ) );
			instituicaoEnsinoId = Long.parseLong( request.getParameter( "instituicaoEnsinoId" ) );
		}
		catch( Exception ex )
		{
			return null;
		}

		ResumoCursoFiltroExcel filtro
			= new ResumoCursoFiltroExcel( instituicaoEnsinoId, campusId );

		return filtro;
	}
}
