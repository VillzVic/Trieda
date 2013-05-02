package com.gapso.web.trieda.server.excel.exp;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.security.core.context.SecurityContext;
import org.springframework.security.core.context.SecurityContextHolder;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Usuario;
import com.gapso.web.trieda.server.ProgressReportServiceImpl;
import com.gapso.web.trieda.server.util.GTriedaI18nConstants;
import com.gapso.web.trieda.server.util.GTriedaI18nMessages;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclaration;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportListReader;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportReader;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.excel.PlanilhasExportExcel;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;

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

	private InstituicaoEnsino getInstituicaoEnsino(){
		SecurityContext context = SecurityContextHolder.getContext();
		String username = context.getAuthentication().getName();
		Usuario usuario = Usuario.find( username );
		return (usuario == null) ? null : usuario.getInstituicaoEnsino();
	}

	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException{
		// Obtém os parâmetros
		String informationToBeExported = request.getParameter(
			ExcelInformationType.getInformationParameterName() );
		
		String fileExtension = request.getParameter(
			ExcelInformationType.getFileExtensionParameterName() );
		

		Map< String, Boolean > planilhasExportExcel = getPlanilhasExportExcel(request);
		
		String chaveRegistro = request.getParameter("chaveRegistro");
		
		if(fileExtension.isEmpty()) {
			fileExtension = "xls";
		}

		InstituicaoEnsino instituicaoEnsino = null;
		try{
			Long instituicaoEnsinoId = Long.parseLong(request.getParameter("instituicaoEnsinoId"));
			instituicaoEnsino = InstituicaoEnsino.find(instituicaoEnsinoId);
		}
		catch(Exception e){
			System.out.println("Não foi informado a instituição de ensino: " + informationToBeExported);
		}

		this.cenario = Cenario.findMasterData(instituicaoEnsino);

		if(!informationToBeExported.isEmpty()){
			ExportExcelFilter filter = ExportExcelFilterFactory.createExporter(informationToBeExported, request, instituicaoEnsino);

			// Get Excel Data
			IExportExcel exporter = ExportExcelFactory.createExporter(
				informationToBeExported, this.cenario, ExportExcelServlet.i18nConstants,
				ExportExcelServlet.i18nMessages, filter, getInstituicaoEnsino(), fileExtension,
				planilhasExportExcel
			);
			
			if ( exporter != null )
			{
				if(exporter instanceof ProgressDeclaration){
					try{
						List<String> feedbackList = new ArrayList<String>();

						((ProgressDeclaration) exporter).setProgressReport(feedbackList);

						ProgressReportReader progressSource = new ProgressReportListReader(feedbackList);
						progressSource.start();
						ProgressReportServiceImpl.getProgressReportSession(request).put(chaveRegistro, progressSource);
					}
					catch(Exception e){
						System.out.println("Nao foi possivel realizar o acompanhamento da progressao.");
					}
				}
			}

			Workbook workbook = exporter.export();

			if(exporter.getErrors().isEmpty()){
				// Write data on response output stream
				writeExcelToHttpResponse(exporter.getFileName(), fileExtension, workbook, response);
			}
			else{
				response.setContentType("text/html");

				for(String msg : exporter.getWarnings()){
					response.getWriter().println(ExcelInformationType.prefixWarning() + msg);
				}

				for(String msg : exporter.getErrors()){
					response.getWriter().println(ExcelInformationType.prefixError() + msg);
				}

				response.getWriter().flush();
			}
		}
	}

	private void writeExcelToHttpResponse( String excelFileName, String fileExtension,
		Workbook excel, HttpServletResponse response ) throws IOException
	{
		if ( fileExtension == "xlsx" )
		{
			response.setContentType( "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet" );
		}
		else
		{
			response.setContentType( "application/vnd.ms-excel" );
		}
		
		response.setHeader( "Content-disposition", "attachment; filename=" + excelFileName + "." + fileExtension );

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
	
	private Map< String, Boolean > getPlanilhasExportExcel( HttpServletRequest request )
	{
		Map< String, Boolean > parametros = new HashMap< String, Boolean >();
		parametros.put( PlanilhasExportExcel.CAMPI, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.CAMPI)) );
		parametros.put( PlanilhasExportExcel.CURRICULOS, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.CURRICULOS)) );
		parametros.put( PlanilhasExportExcel.CURSOS, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.CURSOS)) );
		parametros.put( PlanilhasExportExcel.AREAS_TITULACAO, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.AREAS_TITULACAO)) );
		parametros.put( PlanilhasExportExcel.CURSO_AREAS_TITULACAO, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.CURSO_AREAS_TITULACAO)) );
		parametros.put( PlanilhasExportExcel.DEMANDAS, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.DEMANDAS)) );
		parametros.put( PlanilhasExportExcel.DEMANDAS_POR_ALUNO, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.DEMANDAS_POR_ALUNO)) );
		parametros.put( PlanilhasExportExcel.DISCIPLINAS, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.DISCIPLINAS)) );
		parametros.put( PlanilhasExportExcel.EQUIVALENCIAS, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.EQUIVALENCIAS)) );
		parametros.put( PlanilhasExportExcel.DISCIPLINAS_SALAS, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.DISCIPLINAS_SALAS)) );
		parametros.put( PlanilhasExportExcel.PROFESSORES, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.PROFESSORES)) );
		parametros.put( PlanilhasExportExcel.DISPONIBILIDADES_PROFESSORES, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.DISPONIBILIDADES_PROFESSORES)) );
		parametros.put( PlanilhasExportExcel.CAMPI_TRABALHO, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.CAMPI_TRABALHO)) );
		parametros.put( PlanilhasExportExcel.RELATORIO_VISAO_SALA, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.RELATORIO_VISAO_SALA)) );
		parametros.put( PlanilhasExportExcel.RELATORIO_VISAO_CURSO, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.RELATORIO_VISAO_CURSO)) );
		parametros.put( PlanilhasExportExcel.RELATORIO_VISAO_PROFESSOR, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.RELATORIO_VISAO_PROFESSOR)) );
		parametros.put( PlanilhasExportExcel.RELATORIO_VISAO_ALUNO, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.RELATORIO_VISAO_ALUNO)) );
		parametros.put( PlanilhasExportExcel.SALAS, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.SALAS)) );
		parametros.put( PlanilhasExportExcel.UNIDADES, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.UNIDADES)) );
		parametros.put( PlanilhasExportExcel.RESUMO_CURSO, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.RESUMO_CURSO)) );
		parametros.put( PlanilhasExportExcel.RESUMO_DISCIPLINA, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.RESUMO_DISCIPLINA)) );
		parametros.put( PlanilhasExportExcel.HABILITACAO_PROFESSORES, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.HABILITACAO_PROFESSORES)) );
		parametros.put( PlanilhasExportExcel.ATENDIMENTOS_POR_ALUNO, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.ATENDIMENTOS_POR_ALUNO)) );
		parametros.put( PlanilhasExportExcel.AULAS, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.AULAS)) );
		parametros.put( PlanilhasExportExcel.ALUNOS, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.ALUNOS)) );
		parametros.put( PlanilhasExportExcel.ATENDIMENTOS_MATRICULA, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.ATENDIMENTOS_MATRICULA)) );
		parametros.put( PlanilhasExportExcel.ATENDIMENTOS_DISCIPLINA, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.ATENDIMENTOS_DISCIPLINA)) );
		parametros.put( PlanilhasExportExcel.ATENDIMENTOS_FAIXA_DEMANDA, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.ATENDIMENTOS_FAIXA_DEMANDA)) );
		parametros.put( PlanilhasExportExcel.PERCENT_MESTRES_DOUTORES, Boolean.parseBoolean(request.getParameter(PlanilhasExportExcel.PERCENT_MESTRES_DOUTORES)) );
		
		return parametros;
		
	}
}
