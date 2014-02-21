package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.web.trieda.server.ProfessoresServiceImpl;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;
import com.gapso.web.trieda.shared.util.view.RelatorioProfessorFiltro;

public class ProfessoresMalAlocadosExportExcel
	extends ProfessorListExportExcel
{

	public ProfessoresMalAlocadosExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.PROFESSORES.getSheetName(), cenario, i18nConstants, filter, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = true;
		this.initialRow = 6;
	}
	
	@Override
	public List<Professor> getProfessores() {
		RelatorioProfessorFiltro filtro = new RelatorioProfessorFiltro();
		filtro.setCurso(getFilter().getCursoDTO());
		filtro.setTurno(getFilter().getTurnoDTO());
		filtro.setTitulacao(getFilter().getTitulacaoDTO());
		filtro.setAreaTitulacao(getFilter().getAreaTitulacaoDTO());
		filtro.setTipoContrato(getFilter().getTipoContratoDTO());
		
		ProfessoresServiceImpl professoresService = new ProfessoresServiceImpl();
		
		return professoresService.getBuscaListMalAlocadosExport(getCenario(), 
				getFilter().getCampusDTO().getId(), new RelatorioProfessorFiltro());
	}

}
