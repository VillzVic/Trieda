package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.AtendimentoServiceRelatorioResponse;

@ProgressDeclarationAnnotation
public class AulasExportExcel extends AbstractExportExcel {
	enum ExcelCellStyleReference {
		TEXT(8,2),
		NUMBER(8,6);

		private int row;
		private int col;

		private ExcelCellStyleReference(int row, int col) {
			this.row = row;
			this.col = col;
		}

		public int getRow() {
			return row;
		}

		public int getCol() {
			return col;
		}
	}

	private CellStyle [] cellStyles;
	private boolean removeUnusedSheets;

	public AulasExportExcel(Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
			InstituicaoEnsino instituicaoEnsino, String fileExtension) 
	{
		this(true,cenario,i18nConstants,i18nMessages,instituicaoEnsino, fileExtension);
	}

	public AulasExportExcel(boolean removeUnusedSheets,	Cenario cenario, 
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
			InstituicaoEnsino instituicaoEnsino, String fileExtension)
	{
		super(true,ExcelInformationType.AULAS.getSheetName(),cenario,i18nConstants,i18nMessages,instituicaoEnsino, fileExtension);

		this.cellStyles = new CellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
	}

	@Override
	public String getFileName() {
		return "Aulas";
	}

	@Override
	protected String getPathExcelTemplate() {
		if ( fileExtension.equals("xlsx") )
		{
			return "/templateExport.xlsx";
		}
		else
			return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return "Aulas";
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel(Workbook workbook) {
		RelatorioVisaoCursoExportExcel visaoCursoExpExcel = new RelatorioVisaoCursoExportExcel(false,getCenario(),getI18nConstants(),getI18nMessages(),this.instituicaoEnsino, fileExtension) {
			@Override
			protected List<AtendimentoRelatorioDTO> getAtendimentosRelatorioDTOFromCenario(Cenario cenario) {
				Set<AtendimentoTatico> atendimentosTatico = cenario.getAtendimentosTaticos();
				Set<AtendimentoOperacional> atendimentosOperacional = cenario.getAtendimentosOperacionais();
				
				List<AtendimentoRelatorioDTO> atendimentos = new ArrayList<AtendimentoRelatorioDTO>(atendimentosTatico.size()+atendimentosOperacional.size());
				if (!atendimentosTatico.isEmpty() || !atendimentosOperacional.isEmpty()) {
					// [OfertaId-DisciplinaId -> {totalDemandaP1,totalDemandaP2,totalDemanda}]
					Map<String,Integer[]> demandaKeyToQtdAlunosMap = AlunoDemanda.buildDemandaKeyToQtdAlunosMap(instituicaoEnsino,cenario);
					for (AtendimentoTatico atdTatico : atendimentosTatico) {
						atendimentos.add(ConvertBeans.toAtendimentoTaticoDTO(atdTatico,demandaKeyToQtdAlunosMap));
					}
					for (AtendimentoOperacional atdOperacional : atendimentosOperacional) {
						atendimentos.add(ConvertBeans.toAtendimentoOperacionalDTO(atdOperacional,demandaKeyToQtdAlunosMap));
					}
				}				
				
				return atendimentos;
			}

			@Override
			protected int writeHeader(List<List<ParDTO<String,?>>> rowsHeadersPairs, int row, boolean ehTatico) {
				return row;
			}
			
			@Override
			protected int writeAulas(List<AtendimentoRelatorioDTO> aulas, int row, int mdcTemposAula, boolean temInfoDeHorarios, List<String> horariosDaGradeHoraria, List<String> horariosDeInicioDeAula, List<String> horariosDeFimDeAula) {
				// agrupa as aulas por dia da semana e coleta disciplinas
				Map<Integer,List<AtendimentoRelatorioDTO>> colunaGradeHorariaToAulasMap = new HashMap<Integer,List<AtendimentoRelatorioDTO>>();
				for(AtendimentoRelatorioDTO aula : aulas){
					List<AtendimentoRelatorioDTO> aulasDoDia = colunaGradeHorariaToAulasMap.get(aula.getSemana());
					if(aulasDoDia == null){
						aulasDoDia = new ArrayList<AtendimentoRelatorioDTO>();
						colunaGradeHorariaToAulasMap.put(aula.getSemana(), aulasDoDia);
					}
					aulasDoDia.add(aula);
				}

				// para cada dia da semana, escreve as aulas no excel
				for (Integer colunaGradeHoraria : colunaGradeHorariaToAulasMap.keySet()) {
					List<AtendimentoRelatorioDTO> aulasDoDia = colunaGradeHorariaToAulasMap.get(colunaGradeHoraria);
					if(aulasDoDia == null || aulasDoDia.isEmpty()) continue;

					// para cada aula
					for (AtendimentoRelatorioDTO aula : aulasDoDia) {
						String horarioInicio = "N/A";
						String horarioFim = "N/A";
						
						// obtém a qtd de linhas que devem ser desenhadas para cada crédito da aula em questão
						int linhasDeExcelPorCreditoDaAula = aula.getDuracaoDeUmaAulaEmMinutos() / mdcTemposAula;
						
						// calcula horario de inicio e fim
						if(temInfoDeHorarios){
							horarioInicio = aula.getHorarioAulaString();
							int index = horariosDeInicioDeAula.indexOf(horarioInicio);
							if (index != -1) {
								index = index + aula.getTotalCreditos() * linhasDeExcelPorCreditoDaAula;
								horarioFim = horariosDeFimDeAula.get(index);
							}
						}
						
						/////////////////////////////////////////////////////////////////
						// Código Campus
						int column = 2;
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.TEXT.ordinal()],aula.getCampusString());
						// Turno
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.TEXT.ordinal()],aula.getTurnoString());
						// Código Curso
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.TEXT.ordinal()],aula.getCursoString());
						// Matriz Curricular
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.TEXT.ordinal()],aula.getCurriculoString());
						// Período
						//setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],aula.getPeriodo());
						// Código Disciplina
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.TEXT.ordinal()],aula.getDisciplinaString());
						// Disciplina - Créditos Teóricos
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],aula.getTotalCreditosTeoricosDisciplina());
						// Disciplina - Créditos Práticos
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],aula.getTotalCreditosPraticosDisciplina());
						// Disciplina - Usa Laboratórios?
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.TEXT.ordinal()],(aula.getDisciplinaUsaLaboratorio() ? getI18nConstants().sim() : HtmlUtils.htmlUnescape(getI18nConstants().nao())));
						// Demanda Alunos P1
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],aula.getQtdDemandaAlunosP1());
						// Demanda Alunos P2
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],aula.getQtdDemandaAlunosP2());
						// Demanda Alunos Total
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],aula.getQtdDemandaAlunosTotal());
						
						// Turma
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.TEXT.ordinal()],aula.getTurma());
						// Aula - Créditos Teóricos
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],(aula.isTeorico() ? aula.getTotalCreditos() : 0));
						// Aula - Créditos Práticos
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],(aula.isTeorico() ? 0 : aula.getTotalCreditos()));
						// Demanda Atendida P1
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],aula.getQuantidadeAlunosP1());
						// Demanda Atendida P2
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],aula.getQuantidadeAlunosP2());
						// Demanda Atendida Total
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],aula.getQuantidadeAlunos());
						// Dia Semana
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.TEXT.ordinal()],Semanas.get(aula.getDiaSemana()).name());
						// Horário Início
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.TEXT.ordinal()],horarioInicio);
						// Horário Fim
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.TEXT.ordinal()],horarioFim);
						// Professor
						String professorCPF = (aula.getProfessorId() != null) ? aula.getProfessorCPF() : aula.getProfessorVirtualCPF();
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.TEXT.ordinal()],professorCPF);
						// Sala
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.TEXT.ordinal()],aula.getSalaString());
						// Disciplina Substituta
						setCell(row,column++,sheet,AulasExportExcel.this.cellStyles[AulasExportExcel.ExcelCellStyleReference.TEXT.ordinal()],aula.getDisciplinaSubstitutaString());
						/////////////////////////////////////////////////////////////////
						row++;
					}
				}
				
				return row;
			}
		}; 
		
		List<AtendimentoServiceRelatorioResponse> structureReportControl = visaoCursoExpExcel.getStructureReportControl();
		
		if (visaoCursoExpExcel.getAtendimentosRelatorioDTOList(structureReportControl)) {
			if (this.removeUnusedSheets) {
				removeUnusedSheets(this.getSheetName(),workbook);
			}
			
			visaoCursoExpExcel.sheet = workbook.getSheet(this.getSheetName());
			fillInCellStyles(visaoCursoExpExcel.sheet);
			visaoCursoExpExcel.buildColorPaletteCellStyles(workbook);
			visaoCursoExpExcel.initialRow = 8;
			visaoCursoExpExcel.processStructureReportControl(structureReportControl);
			
			return true;
		}
		
		return false;
	}

	private void fillInCellStyles(Sheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}