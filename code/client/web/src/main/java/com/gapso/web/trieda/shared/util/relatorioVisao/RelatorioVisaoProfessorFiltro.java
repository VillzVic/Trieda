package com.gapso.web.trieda.shared.util.relatorioVisao;

import java.util.HashMap;
import java.util.Map;

import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.FieldEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;

public class RelatorioVisaoProfessorFiltro extends RelatorioVisaoFiltro{
	private static final long serialVersionUID = 1L;
	private String professorNome;
	private String professorCpf;
	private ProfessorVirtualDTO professorVirtualDTO;
	
	public ExcelInformationType getExcelType(){
		return ExcelInformationType.RELATORIO_VISAO_PROFESSOR;
	}
	
	public Map<String, String> getMapStringIds(){
		Map<String, String> mapIds =  new HashMap<String, String>();
		
		mapIds.put("professorCpf", professorCpf);
		mapIds.put("professorNome", professorNome);
		mapIds.put("professorVirtualId", (professorVirtualDTO != null) ? professorVirtualDTO.getId().toString() : "-1");
		
		return mapIds;
	}

	public void addProfessorNomeValueListener(ComboBox<ProfessorDTO> f){
		f.setFireChangeEventOnSetValue(true);
		f.addListener(Events.Change, new Listener<FieldEvent>(){
			public void handleEvent(FieldEvent fe){
				if (fe.getValue() != null){
					professorNome = ((ProfessorDTO) fe.getValue()).getNome();
				}
				else {
					professorNome = null;
				}
			}
		});
	}
	
	public void addProfessorCpfValueListener(ComboBox<ProfessorDTO> f){
		f.setFireChangeEventOnSetValue(true);
		f.addListener(Events.Change, new Listener<FieldEvent>(){
			public void handleEvent(FieldEvent fe){
				if (fe.getValue() != null){
					professorCpf = ((ProfessorDTO) fe.getValue()).getCpf();
				}
				else {
					professorCpf = null;
				}
			}
		});
	}
	
	public void addProfessorVirtualValueListener(Field<ProfessorVirtualDTO> f){
		f.setFireChangeEventOnSetValue(true);
		f.addListener(Events.Change, new Listener<FieldEvent>(){
			public void handleEvent(FieldEvent fe){
				professorVirtualDTO = (ProfessorVirtualDTO) fe.getValue();
			}
		});
	}

	public String getProfessorNome(){
		return professorNome;
	}

	public void setProfessorNome(String professorNome){
		this.professorNome = professorNome;
	}

	public String getProfessorCpf(){
		return this.professorCpf;
	}

	public void setProfessorCpf(String professorCpf){
		this.professorCpf = professorCpf;
	}

	public ProfessorVirtualDTO getProfessorVirtualDTO(){
		return this.professorVirtualDTO;
	}

	public void setProfessorVirtualDTO(ProfessorVirtualDTO professorVirtualDTO){
		this.professorVirtualDTO = professorVirtualDTO;
	}

}
