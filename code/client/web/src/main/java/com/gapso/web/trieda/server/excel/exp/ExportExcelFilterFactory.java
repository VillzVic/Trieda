package com.gapso.web.trieda.server.excel.exp;

import javax.servlet.http.HttpServletRequest;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoAlunoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoCursoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoProfessorFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoSalaFiltro;

public class ExportExcelFilterFactory{

	public static ExportExcelFilter createExporter(String info, HttpServletRequest request, InstituicaoEnsino instituicaoEnsino){
		ExcelInformationType informationToBeExported = ExcelInformationType.valueOf(info);

		switch(informationToBeExported){
			case PERCENT_MESTRES_DOUTORES: return getExportPercentMestresDoutores(request, instituicaoEnsino.getId());
			case RELATORIO_VISAO_CURSO: return getExportVisaoCurso(request, instituicaoEnsino);
			case RELATORIO_VISAO_SALA: return getExportVisaoSala(request, instituicaoEnsino);
			case RELATORIO_VISAO_PROFESSOR: return getExportVisaoProfessor(request, instituicaoEnsino);
			case RELATORIO_VISAO_ALUNO: return getExportVisaoAluno(request, instituicaoEnsino);
			case RESUMO_DISCIPLINA: return getExportResumoDisciplina(request, instituicaoEnsino.getId());
			case RESUMO_CURSO: return getExportResumoCurso(request, instituicaoEnsino.getId());
			case ATENDIMENTOS_FAIXA_DEMANDA: return getExportAtendimentosFaixaDemanda(request, instituicaoEnsino.getId());
		default:
			break;
		}

		return null;
	}
	
	private static RelatorioVisaoProfessorFiltro getExportVisaoProfessor(HttpServletRequest request, InstituicaoEnsino instituicaoEnsino){
		try{
			RelatorioVisaoProfessorFiltro filtro = new RelatorioVisaoProfessorFiltro();
			
			String professorNome = request.getParameter("professorNome");
			String professorCpf = request.getParameter("professorCpf");
			Long professorVirtualId = Long.parseLong(request.getParameter("professorVirtualId"));

			if(professorVirtualId < 0) {
				filtro.setProfessorNome(professorNome);
				filtro.setProfessorCpf(professorCpf);
			}
			else filtro.setProfessorVirtualDTO(ConvertBeans.toProfessorVirtualDTO(ProfessorVirtual.find(professorVirtualId, instituicaoEnsino)));
			
			if (professorCpf.isEmpty() && professorNome.isEmpty() && professorVirtualId < 0) return null;
			
			return filtro;
		}
		catch(Exception ex){
			return null;
		}
	}
	
	private static RelatorioVisaoAlunoFiltro getExportVisaoAluno(HttpServletRequest request, InstituicaoEnsino instituicaoEnsino){
		try{
			RelatorioVisaoAlunoFiltro filtro = new RelatorioVisaoAlunoFiltro();
			
			String alunoNome = request.getParameter("alunoNome");
			String alunoMatricula = request.getParameter("alunoMatricula");
			
			if (alunoNome.isEmpty() && alunoMatricula.isEmpty()) return null;

			filtro.setAlunoNome(alunoNome);
			filtro.setAlunoMatricula(alunoMatricula);

			return filtro;
		}
		catch(Exception ex){
			return null;
		}
	}

	private static RelatorioVisaoCursoFiltro getExportVisaoCurso(HttpServletRequest request, InstituicaoEnsino instituicaoEnsino){
		try{
			RelatorioVisaoCursoFiltro filtro = new RelatorioVisaoCursoFiltro();
			
			Long cursoId = Long.parseLong(request.getParameter("cursoId"));
			Long curriculoId = Long.parseLong(request.getParameter("curriculoId"));
			Long campusId = Long.parseLong(request.getParameter("campusId"));
			Integer periodoId = Integer.parseInt(request.getParameter("periodoId"));
			Long turnoId = Long.parseLong(request.getParameter("turnoId"));
			
			filtro.setCursoDTO(ConvertBeans.toCursoDTO(Curso.find(cursoId, instituicaoEnsino)));
			filtro.setCurriculoDTO(ConvertBeans.toCurriculoDTO(Curriculo.find(curriculoId, instituicaoEnsino)));
			filtro.setCampusDTO(ConvertBeans.toCampusDTO(Campus.find(campusId, instituicaoEnsino)));
			filtro.setPeriodo(periodoId);
			filtro.setTurnoDTO(ConvertBeans.toTurnoDTO(Turno.find(turnoId, instituicaoEnsino)));
			
			return filtro;
		}
		catch(Exception ex){
			return null;
		}
	}
	
	private static RelatorioVisaoSalaFiltro getExportVisaoSala(HttpServletRequest request, InstituicaoEnsino instituicaoEnsino){
		try{
			RelatorioVisaoSalaFiltro filtro = new RelatorioVisaoSalaFiltro();

			String salaCodigo = request.getParameter("salaCodigo");
			
			if ( salaCodigo.isEmpty() ) return null;

			filtro.setSalaCodigo(salaCodigo);
			
			return filtro;
		}
		catch(Exception ex){
			return null;
		}
	}

	private static ResumoDisciplinaFiltroExcel getExportResumoDisciplina(HttpServletRequest request, Long instituicaoEnsinoId){
		try{
			Long campusId = Long.parseLong(request.getParameter("campusId"));
			return new ResumoDisciplinaFiltroExcel(instituicaoEnsinoId, campusId);
		}
		catch(Exception ex){
			return null;
		}
	}

	private static ResumoCursoFiltroExcel getExportResumoCurso(HttpServletRequest request, Long instituicaoEnsinoId){
		try{
			Long campusId = Long.parseLong(request.getParameter("campusId"));
			return new ResumoCursoFiltroExcel(instituicaoEnsinoId, campusId);
		}
		catch(Exception ex){
			return null;
		}
	}
	
	private static AtendimentosFaixaDemandaFiltroExcel getExportAtendimentosFaixaDemanda(HttpServletRequest request, Long instituicaoEnsinoId){
		try{
			Long campusId = Long.parseLong(request.getParameter("campusId"));
			return new AtendimentosFaixaDemandaFiltroExcel(instituicaoEnsinoId, campusId);
		}
		catch(Exception ex){
			return null;
		}
	}
	
	private static PercentMestresDoutoresFiltroExcel getExportPercentMestresDoutores(HttpServletRequest request, Long instituicaoEnsinoId){
		try{
			Long campusId = Long.parseLong(request.getParameter("campusId"));
			return new PercentMestresDoutoresFiltroExcel(instituicaoEnsinoId, campusId);
		}
		catch(Exception ex){
			return null;
		}
	}
	
}
