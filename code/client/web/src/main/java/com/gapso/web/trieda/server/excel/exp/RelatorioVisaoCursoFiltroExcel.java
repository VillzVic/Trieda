package com.gapso.web.trieda.server.excel.exp;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;

public class RelatorioVisaoCursoFiltroExcel
	implements ExportExcelFilter
{
	private CursoDTO cursoDTO;
	private CurriculoDTO curriculoDTO;
	private CampusDTO campusDTO;
	private Integer periodo;
	private TurnoDTO turnoDTO;

	public RelatorioVisaoCursoFiltroExcel( Long cursoId, Long curriculoId,
		Long campusId, Integer periodoId, Long turnoId )
	{
		this.cursoDTO = ConvertBeans.toCursoDTO( Curso.find( cursoId ) );
		this.curriculoDTO = ConvertBeans.toCurriculoDTO( Curriculo.find( curriculoId ) );
		this.campusDTO = ConvertBeans.toCampusDTO( Campus.find( campusId ) );
		this.periodo = periodoId;
		this.turnoDTO = ConvertBeans.toTurnoDTO( Turno.find( turnoId ) );
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

	public ExportExcelFilter getFilter(){
		return this;
	}
}
