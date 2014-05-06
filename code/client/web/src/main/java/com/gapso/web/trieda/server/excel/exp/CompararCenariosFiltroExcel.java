package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;

public class CompararCenariosFiltroExcel
	implements ExportExcelFilter
{
	private List<CenarioDTO> cenariosDTO = new ArrayList<CenarioDTO>();

	public CompararCenariosFiltroExcel( Long instituicaoEnsinoId,
			List<Long> cenariosId )
	{
		InstituicaoEnsino instituicaoEnsino
			= InstituicaoEnsino.find( instituicaoEnsinoId );

		if ( cenariosId != null && !cenariosId.isEmpty())
		{
			for (Long cenarioId : cenariosId)
			{
				cenariosDTO.add(ConvertBeans.toCenarioDTO(
						Cenario.find(cenarioId, instituicaoEnsino)) );
			}
		}
	}

	public List<CenarioDTO> getCenariosDTO()
	{
		return cenariosDTO;
	}

	public void setCenariosDTO( List<CenarioDTO> cenariosDTO )
	{
		this.cenariosDTO = cenariosDTO;
	}

	public ExportExcelFilter getFilter()
	{
		return this;
	}
}