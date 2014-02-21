package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;

import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;

public class ProfessoresUtilizadosExportExcel
	extends ProfessorListExportExcel{

	public ProfessoresUtilizadosExportExcel( Cenario cenario,
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
		Campus campus = (getFilter().getCampusDTO() == null ? null : Campus.find(getFilter().getCampusDTO().getId(), instituicaoEnsino));
		Curso curso = (getFilter().getCursoDTO() == null ? null : Curso.find(getFilter().getCursoDTO().getId(), instituicaoEnsino));
		Turno turno = (getFilter().getTurnoDTO() == null ? null : Turno.find(getFilter().getTurnoDTO().getId(), instituicaoEnsino));
		Titulacao titulacao = (getFilter().getTitulacaoDTO() == null ? null : Titulacao.find(getFilter().getTitulacaoDTO().getId(), instituicaoEnsino));
		AreaTitulacao areaTitulacao = (getFilter().getAreaTitulacaoDTO() == null ? null : AreaTitulacao.find(getFilter().getAreaTitulacaoDTO().getId(), instituicaoEnsino));
		TipoContrato tipoContrato = (getFilter().getTipoContratoDTO() == null ? null : TipoContrato.find(getFilter().getTipoContratoDTO().getId(), instituicaoEnsino));
		
		return AtendimentoOperacional.findProfessoresUtilizados(instituicaoEnsino, getCenario(),
				campus, curso, turno, titulacao, areaTitulacao, tipoContrato, null, null, null, null );
	}

}
