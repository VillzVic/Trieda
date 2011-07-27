package com.gapso.web.trieda.server.excel.exp;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;

public class RelatorioVisaoProfessorFiltroExcel
	implements ExportExcelFilter
{
	private CampusDTO campusDTO;
	private TurnoDTO turnoDTO;
	private ProfessorDTO professorDTO;
	private ProfessorVirtualDTO professorVirtualDTO;

	public RelatorioVisaoProfessorFiltroExcel( Long campusId, Long turnoId,
		Long professorId, Long professorVirtualId )
	{
		this.campusDTO = ConvertBeans.toCampusDTO( Campus.find( campusId ) );
		this.turnoDTO = ConvertBeans.toTurnoDTO( Turno.find( turnoId ) );

		if ( professorId == null || professorId < 0 )
		{
			professorDTO = null;
			professorVirtualDTO = ConvertBeans.toProfessorVirtualDTO( ProfessorVirtual.find( professorVirtualId ) );
		}
		else if ( professorVirtualId == null || professorVirtualId < 0 )
		{
			professorDTO = ConvertBeans.toProfessorDTO( Professor.find( professorId ) );;
			professorVirtualDTO = null;
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
		return turnoDTO;
	}

	public void setTurnoDTO( TurnoDTO turnoDTO )
	{
		this.turnoDTO = turnoDTO;
	}

	public ProfessorDTO getProfessorDTO()
	{
		return professorDTO;
	}

	public void setProfessorDTO( ProfessorDTO professorDTO )
	{
		this.professorDTO = professorDTO;
	}

	public ProfessorVirtualDTO getProfessorVirtualDTO()
	{
		return professorVirtualDTO;
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
