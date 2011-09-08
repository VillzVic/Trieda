package com.gapso.web.trieda.server.excel.exp;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;

public class RelatorioVisaoCursoFiltroExcel
	implements ExportExcelFilter
{
	private CursoDTO cursoDTO;
	private CurriculoDTO curriculoDTO;
	private CampusDTO campusDTO;
	private Integer periodo;
	private TurnoDTO turnoDTO;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;

	public RelatorioVisaoCursoFiltroExcel( Long cursoId, Long curriculoId,
		Long campusId, Integer periodoId, Long turnoId, Long instituicaoEnsinoId )
	{
		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( instituicaoEnsinoId );

		this.instituicaoEnsinoDTO = ConvertBeans.toInstituicaoEnsinoDTO( instituicaoEnsino );
		this.cursoDTO = ConvertBeans.toCursoDTO( Curso.find( cursoId, instituicaoEnsino ) );
		this.curriculoDTO = ConvertBeans.toCurriculoDTO( Curriculo.find( curriculoId, instituicaoEnsino ) );
		this.campusDTO = ConvertBeans.toCampusDTO( Campus.find( campusId, instituicaoEnsino ) );
		this.periodo = periodoId;
		this.turnoDTO = ConvertBeans.toTurnoDTO( Turno.find( turnoId, instituicaoEnsino ) );
	}

	public CursoDTO getCursoDTO() {
		return cursoDTO;
	}

	public void setCursoDTO(CursoDTO cursoDTO) {
		this.cursoDTO = cursoDTO;
	}

	public CurriculoDTO getCurriculoDTO() {
		return curriculoDTO;
	}

	public void setCurriculoDTO(CurriculoDTO curriculoDTO) {
		this.curriculoDTO = curriculoDTO;
	}

	public CampusDTO getCampusDTO() {
		return campusDTO;
	}

	public void setCampusDTO(CampusDTO campusDTO) {
		this.campusDTO = campusDTO;
	}

	public Integer getPeriodo() {
		return periodo;
	}

	public void setPeriodo(Integer periodo) {
		this.periodo = periodo;
	}

	public TurnoDTO getTurnoDTO() {
		return turnoDTO;
	}

	public void setTurnoDTO(TurnoDTO turnoDTO) {
		this.turnoDTO = turnoDTO;
	}

	public InstituicaoEnsinoDTO getInstituicaoEnsinoDTO()
	{
		return instituicaoEnsinoDTO;
	}

	public void setInstituicaoEnsinoDTO(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
	}

	public ExportExcelFilter getFilter(){
		return this;
	}
}
