package com.gapso.web.trieda.server.excel.exp;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;

public class ResumoDisciplinaFiltroExcel
	implements ExportExcelFilter
{
	private CampusDTO campusDTO;

	public ResumoDisciplinaFiltroExcel(
		Long instituicaoEnsinoId, Long campusId )
	{
		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( instituicaoEnsinoId );

		if ( campusId != null && campusId > 0 )
		{
			this.campusDTO = ConvertBeans.toCampusDTO(
				Campus.find( campusId, instituicaoEnsino ) );
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
}
