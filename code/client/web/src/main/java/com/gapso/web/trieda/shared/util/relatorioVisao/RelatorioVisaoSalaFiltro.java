package com.gapso.web.trieda.shared.util.relatorioVisao;

import java.util.Map;

import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.FieldEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;

public class RelatorioVisaoSalaFiltro extends RelatorioVisaoFiltro{
	private static final long serialVersionUID = 1L;
	private CampusDTO campusDTO;
	private UnidadeDTO unidadeDTO;
	private SalaDTO salaDTO;
	
	public ExcelInformationType getExcelType(){
		return ExcelInformationType.RELATORIO_VISAO_SALA;
	}
	
	public Map<String, String> getMapStringIds(){
		Map<String, String> mapIds = super.getMapStringIds();
		
		mapIds.put("campusId", campusDTO.getId().toString());
		mapIds.put("unidadeId", unidadeDTO.getId().toString());
		mapIds.put("salaId", salaDTO.getId().toString());
		
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
	
	public void addUnidadeValueListener(Field<UnidadeDTO> f){
		f.setFireChangeEventOnSetValue(true);
		f.addListener(Events.Change, new Listener<FieldEvent>(){
			public void handleEvent(FieldEvent fe){
				unidadeDTO = (UnidadeDTO) fe.getValue();
			}
		});
	}
	
	public void addSalaValueListener(Field<SalaDTO> f){
		f.setFireChangeEventOnSetValue(true);
		f.addListener(Events.Change, new Listener<FieldEvent>(){
			public void handleEvent(FieldEvent fe){
				salaDTO = (SalaDTO) fe.getValue();
			}
		});
	}
	
	public CampusDTO getCampusDTO(){
		return this.campusDTO;
	}

	public void setCampusDTO(CampusDTO campusDTO){
		this.campusDTO = campusDTO;
	}

	public UnidadeDTO getUnidadeDTO(){
		return this.unidadeDTO;
	}

	public void setUnidadeDTO(UnidadeDTO unidadeDTO){
		this.unidadeDTO = unidadeDTO;
	}

	public SalaDTO getSalaDTO(){
		return this.salaDTO;
	}

	public void setSalaDTO(SalaDTO salaDTO){
		this.salaDTO = salaDTO;
	}
	
}
