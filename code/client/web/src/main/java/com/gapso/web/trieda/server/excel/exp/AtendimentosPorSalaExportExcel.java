package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.SextetoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class AtendimentosPorSalaExportExcel extends AbstractExportExcel {
	enum ExcelCellStyleReference {
		TEXT(7,2),
		NUMBER(7,8);

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

	public AtendimentosPorSalaExportExcel(Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
			InstituicaoEnsino instituicaoEnsino, String fileExtension) 
	{
		this(true,cenario,i18nConstants,i18nMessages,instituicaoEnsino, fileExtension);
	}

	public AtendimentosPorSalaExportExcel(boolean removeUnusedSheets, Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
			InstituicaoEnsino instituicaoEnsino, String fileExtension)
	{
		super(true,ExcelInformationType.ATENDIMENTOS_POR_ALUNO.getSheetName(),cenario,i18nConstants,i18nMessages,instituicaoEnsino, fileExtension);

		this.cellStyles = new CellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
	}

	@Override
	public String getFileName() {
		return "Atendimentos por Aluno";
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
		return "Atendimentos por Aluno";
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel(Workbook workbook) {
		RelatorioVisaoSalaExportExcel visaoSalaExpExcel = new RelatorioVisaoSalaExportExcel(false,getCenario(),getI18nConstants(),getI18nMessages(),this.instituicaoEnsino, fileExtension) {
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
						
						// para cada aluno na aula
						for (AlunoDemandaDTO alunoDTO : aula.getAlunosDemandas()) {
							writeCells(row,alunoDTO,aula,horarioInicio,horarioFim);
							row++;
						}
					}
				}
				
				return row;
			}
			
			private void writeCells(int row, AlunoDemandaDTO alunoDTO, AtendimentoRelatorioDTO aula, String horarioInicio, String horarioFim) {
				int column = 2;
				// Sala
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],aula.getSalaString());
				// Dia Semana
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],Semanas.get(aula.getDiaSemana()).name());
				// Horário Início
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],horarioInicio);
				// Horário Fim
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],horarioFim);
				// Disciplina da Aula
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],aula.getDisciplinaString());
				// Disciplina da Demanda
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],alunoDTO.getDisciplinaString());
				// Turma
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],aula.getTurma());
				// Professor
				String professorCPF = (aula.getProfessorId() != null) ? aula.getProfessorCPF() : aula.getProfessorVirtualCPF();
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],professorCPF);
				// Aula - Créditos Teóricos
				int credT = (aula.isTeorico() ? aula.getTotalCreditos() : 0);
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],credT);
				// Aula - Créditos Práticos
				int credP = (aula.isTeorico() ? 0 : aula.getTotalCreditos());
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],credP);
				// Aula - Total
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],credT+credP);
				// Aula - Carga Horária Teórica
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],credT*aula.getSemanaLetivaTempoAula());
				// Aula - Carga Horária Prática
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],credP*aula.getSemanaLetivaTempoAula());
				// Aula - Carga Horária Total
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],(credT+credP)*aula.getSemanaLetivaTempoAula());
				
				// Aluno - Matrícula
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],alunoDTO.getAlunoMatricula());
				// Aluno - Prioridade
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],alunoDTO.getAlunoPrioridade());
			}
		}; 
		
		List<SextetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>,List<String>>> structureReportControl = visaoSalaExpExcel.getStructureReportControl();
		
		if (visaoSalaExpExcel.getAtendimentosRelatorioDTOList(structureReportControl)) {
			if (this.removeUnusedSheets) {
				removeUnusedSheets(this.getSheetName(),workbook);
			}
			
			visaoSalaExpExcel.sheet = workbook.getSheet(this.getSheetName());
			fillInCellStyles(visaoSalaExpExcel.sheet);
			visaoSalaExpExcel.buildColorPaletteCellStyles(workbook);
			visaoSalaExpExcel.initialRow = 7;
			visaoSalaExpExcel.processStructureReportControl(structureReportControl);
			
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