package com.gapso.web.trieda.server.excel;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.web.trieda.shared.excel.ExportedInformationType;

public class ExportExcelServlet extends HttpServlet {
	
	private static final long serialVersionUID = -7987228694777660184L;

	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		boolean success = false;
		
		// Obtém os parâmetros
		String informationToBeExported = request.getParameter(ExportedInformationType.getParameterName());
		if (!informationToBeExported.isEmpty()) {
			// Get Excel Data
			IExportExcel exporter = ExportExcelFactory.createExporter(informationToBeExported);			
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