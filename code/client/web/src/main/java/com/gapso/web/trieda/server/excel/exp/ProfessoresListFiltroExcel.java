package com.gapso.web.trieda.server.excel.exp;

import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;

public class ProfessoresListFiltroExcel
	implements ExportExcelFilter
{
	private CampusDTO campusDTO;
	private CursoDTO cursoDTO;
	private TurnoDTO turnoDTO;
	private TitulacaoDTO titulacaoDTO;
	private AreaTitulacaoDTO areaTitulacaoDTO;
	private TipoContratoDTO tipoContratoDTO;

	public ProfessoresListFiltroExcel(
		Long instituicaoEnsinoId, Long campusId, Long cursoId, Long turnoId, Long titulacaoId,
		Long areaTitulacaoId, Long tipoContratoId )
	{
		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( instituicaoEnsinoId );
		if ( campusId != null && campusId > 0 )
		{
			this.campusDTO = ConvertBeans.toCampusDTO(
				Campus.find( campusId, instituicaoEnsino ) );
		}
		if ( cursoId != null && cursoId > 0 )
		{
			this.setCursoDTO(ConvertBeans.toCursoDTO(
				Curso.find( cursoId, instituicaoEnsino ) ));
		}
		if ( turnoId != null && turnoId > 0 )
		{
			this.setTurnoDTO(ConvertBeans.toTurnoDTO(
				Turno.find( turnoId, instituicaoEnsino ) ));
		}
		if ( titulacaoId != null && titulacaoId > 0 )
		{
			this.setTitulacaoDTO(ConvertBeans.toTitulacaoDTO(
				Titulacao.find( titulacaoId, instituicaoEnsino ) ));
		}
		if ( areaTitulacaoId != null && areaTitulacaoId > 0 )
		{
			this.setAreaTitulacaoDTO(ConvertBeans.toAreaTitulacaoDTO(
				AreaTitulacao.find( areaTitulacaoId, instituicaoEnsino ) ));
		}
		if ( tipoContratoId != null && tipoContratoId > 0 )
		{
			this.setTipoContratoDTO(ConvertBeans.toTipoContratoDTO(
				TipoContrato.find( tipoContratoId, instituicaoEnsino ) ));
		}
	}

	public CampusDTO getCampusDTO()
	{
		return campusDTO;
	}

	public void setCampusDTO( CampusDTO campusDTO )
	{
		this.campusDTO = campusDTO;
	}

	public ExportExcelFilter getFilter()
	{
		return this;
	}

	public CursoDTO getCursoDTO() {
		return cursoDTO;
	}

	public void setCursoDTO(CursoDTO cursoDTO) {
		this.cursoDTO = cursoDTO;
	}

	public TurnoDTO getTurnoDTO() {
		return turnoDTO;
	}

	public void setTurnoDTO(TurnoDTO turnoDTO) {
		this.turnoDTO = turnoDTO;
	}

	public TitulacaoDTO getTitulacaoDTO() {
		return titulacaoDTO;
	}

	public void setTitulacaoDTO(TitulacaoDTO titulacaoDTO) {
		this.titulacaoDTO = titulacaoDTO;
	}

	public AreaTitulacaoDTO getAreaTitulacaoDTO() {
		return areaTitulacaoDTO;
	}

	public void setAreaTitulacaoDTO(AreaTitulacaoDTO areaTitulacaoDTO) {
		this.areaTitulacaoDTO = areaTitulacaoDTO;
	}

	public TipoContratoDTO getTipoContratoDTO() {
		return tipoContratoDTO;
	}

	public void setTipoContratoDTO(TipoContratoDTO tipoContratoDTO) {
		this.tipoContratoDTO = tipoContratoDTO;
	}
}
