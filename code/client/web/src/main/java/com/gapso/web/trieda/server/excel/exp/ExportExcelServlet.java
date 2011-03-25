package com.gapso.web.trieda.server.excel.exp;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.scb.gwt.web.server.i18n.GWTI18N;

import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class ExportExcelServlet extends HttpServlet {
	
	private static final long serialVersionUID = -7987228694777660184L;
	
	private static TriedaI18nMessages i18nMessages = null;
	{
		try {
			i18nMessages = GWTI18N.create(TriedaI18nMessages.class);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		boolean success = false;
		
		// Obtém os parâmetros
		String informationToBeExported = request.getParameter(ExcelInformationType.getInformationParameterName());
		if (!informationToBeExported.isEmpty()) {
			// Get Excel Data
			IExportExcel exporter = ExportExcelFactory.createExporter(informationToBeExported,i18nMessages);			
			HSSFWorkbook workbook = exporter.export();
			
			if (exporter.getErrors().isEmpty()) {
				// Write data on response output stream
				writeExcelToHttpResponse(exporter.getFileName(),workbook,response);
				success = true;
			}			
		}
		
		if (!success) {
			response.setContentType("text/plain");
			response.getWriter().print("An error has occured");// TODO: incluir mensagem de erro
		}
	}
	
	private void writeExcelToHttpResponse(String excelFileName, HSSFWorkbook excel, HttpServletResponse response) throws IOException {
		response.setContentType("application/vnd.ms-excel");  
		response.setHeader("Content-disposition","attachment; filename="+excelFileName+".xls");
		ServletOutputStream out = null;
		try {
			out = response.getOutputStream();
			excel.write(out);
			out.flush();
		} catch (IOException e) {
			throw e;
		} finally {
			if (out != null) {
		        out.close();
			}
		}
	}
}