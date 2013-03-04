package com.gapso.web.trieda.server.excel.exp;

import java.io.IOException;

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
import com.gapso.web.trieda.server.util.GTriedaI18nConstants;
import com.gapso.web.trieda.server.util.GTriedaI18nMessages;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
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
				ExportExcelServlet.i18nMessages, filter, getInstituicaoEnsino(), fileExtension
			);

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
		response.setContentType( "application/vnd.ms-excel" );  
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

}
