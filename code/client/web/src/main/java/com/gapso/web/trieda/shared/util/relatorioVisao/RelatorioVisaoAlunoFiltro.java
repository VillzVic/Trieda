package com.gapso.web.trieda.shared.util.relatorioVisao;

import java.util.Map;

import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.FieldEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;

public class RelatorioVisaoAlunoFiltro extends RelatorioVisaoFiltro{
	private static final long serialVersionUID = 1L;
	private CampusDTO campusDTO;
	private AlunoDTO alunoDTO;
	
	public ExcelInformationType getExcelType(){
		return ExcelInformationType.RELATORIO_VISAO_ALUNO;
	}
	
	public Map<String, String> getMapStringIds(){
		Map<String, String> mapIds = super.getMapStringIds();
		
		mapIds.put("campusId", campusDTO.getId().toString());
		mapIds.put("alunoId", alunoDTO.getId().toString());
		
		return mapIds;
	}

	public void addCampusValueListener(Field<CampusDTO> f){
		f.setFireChangeEventOnSetValue(true);
		f.addListener(Events.Change, new Listener<FieldEvent>(){
			public void handleEvent(FieldEvent fe){
				campusDTO = (CampusDTO) fe.getValue();
			}
		});
	}
	
	public void addAlunoValueListener(Field<AlunoDTO> f){
		f.setFireChangeEventOnSetValue(true);
		f.addListener(Events.Change, new Listener<FieldEvent>(){
			public void handleEvent(FieldEvent fe){
				alunoDTO = (AlunoDTO) fe.getValue();
			}
		});
	}
	
	public CampusDTO getCampusDTO(){
		return this.campusDTO;
	}

	public void setCampusDTO(CampusDTO campusDTO){
		this.campusDTO = campusDTO;
	}

	public AlunoDTO getAlunoDTO(){
		return this.alunoDTO;
	}

	public void setAlunoDTO(AlunoDTO alunoDTO){
		this.alunoDTO = alunoDTO;
	}
	
}
