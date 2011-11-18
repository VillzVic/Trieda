package com.gapso.web.trieda.server.excel.exp;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;

public class RelatorioVisaoProfessorFiltroExcel
	implements ExportExcelFilter
{
	private CampusDTO campusDTO;
	private TurnoDTO turnoDTO;
	private SemanaLetivaDTO semanaLetivaDTO;
	private ProfessorDTO professorDTO;
	private ProfessorVirtualDTO professorVirtualDTO;

	public RelatorioVisaoProfessorFiltroExcel( Long campusId, Long turnoId,
		Long professorId, Long professorVirtualId, Long instituicaoEnsinoId, Long semanaLetivaId )
	{
		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( instituicaoEnsinoId ); 

		Campus campus = Campus.find( campusId, instituicaoEnsino );

		this.campusDTO = ConvertBeans.toCampusDTO( campus );
		this.turnoDTO = ConvertBeans.toTurnoDTO(
			Turno.find( turnoId, instituicaoEnsino ) );
		
		this.semanaLetivaDTO = ConvertBeans.toSemanaLetivaDTO(
			SemanaLetiva.find( semanaLetivaId, instituicaoEnsino ) );

		if ( professorId == null || professorId < 0 )
		{
			this.professorDTO = null;

			this.professorVirtualDTO = ConvertBeans.toProfessorVirtualDTO(
				ProfessorVirtual.find( professorVirtualId, instituicaoEnsino ) );
		}
		else if ( professorVirtualId == null || professorVirtualId < 0 )
		{
			this.professorDTO = ConvertBeans.toProfessorDTO(
				Professor.find( professorId, instituicaoEnsino ) );

			this.professorVirtualDTO = null;
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

	public TurnoDTO getTurnoDTO()
	{
		return this.turnoDTO;
	}

	public void setTurnoDTO( TurnoDTO turnoDTO )
	{
		this.turnoDTO = turnoDTO;
	}

	public SemanaLetivaDTO getSemanaLetivaDTO()
	{
		return this.semanaLetivaDTO;
	}

	public void setSemanaLetivaDTO( SemanaLetivaDTO semanaLetivaDTO )
	{
		this.semanaLetivaDTO = semanaLetivaDTO;
	}

	public ProfessorDTO getProfessorDTO()
	{
		return this.professorDTO;
	}

	public void setProfessorDTO( ProfessorDTO professorDTO )
	{
		this.professorDTO = professorDTO;
	}

	public ProfessorVirtualDTO getProfessorVirtualDTO()
	{
		return this.professorVirtualDTO;
	}

	public void setProfessorVirtualDTO( ProfessorVirtualDTO professorVirtualDTO )
	{
		this.professorVirtualDTO = professorVirtualDTO;
	}

	public ExportExcelFilter getFilter()
	{
		return this;
	}
}
